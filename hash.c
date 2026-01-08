#include "ai.h"
#include <limits.h>
#include <time.h>
#include <windows.h>
#include <stdlib.h>

// ===================== 优化：Zobrist哈希+大质数取模 =====================
#define BOARD_SIZE 15          // 假设棋盘大小为15×15（可根据实际调整）
#define NEIGHBORHOOD_SIZE 3    // 邻域大小
#define SEARCH_DEPTH 4         // 搜索深度
#define BLACK 1
#define WHITE 2
#define BLANK 0
#define PLAYER_BLACK 1
#define PLAYER_WHITE 2

// 1. 定义大质数（哈希表大小，优先选接近2^20的质数）
#define TT_PRIME 1048573       // 1048573是质数，接近2^20（1048576）
#define TT_SIZE TT_PRIME       // 哈希表大小设为大质数

// 2. Zobrist哈希表：预生成随机数（每个位置+每种颜色对应一个随机数）
unsigned long long zobrist_table[BOARD_SIZE+1][BOARD_SIZE+1][3]; // [行][列][颜色：空/黑/白]

// 置换表条目（不变，仅哈希表大小改为质数）
typedef struct {
    unsigned long long hash; // 棋盘的Zobrist哈希值
    int score;
    int depth;
    enum { EXACT, LOWER, UPPER } type;
} TT_Entry;
TT_Entry transposition_table[TT_SIZE]; // 哈希表大小为大质数

// 初始化Zobrist随机表（仅初始化一次）
void initZobrist() {
    static int initialized = 0;
    if (initialized) return;
    // 生成64位随机数（保证唯一性）
    for (int i = 1; i <= BOARD_SIZE; i++) {
        for (int j = 1; j <= BOARD_SIZE; j++) {
            for (int c = 0; c < 3; c++) { // c=0:空, c=1:黑, c=2:白
                zobrist_table[i][j][c] = ((unsigned long long)rand() << 32) | rand();
            }
        }
    }
    initialized = 1;
}

// 清空置换表
void clearTT() {
    for (int i = 0; i < TT_SIZE; i++) {
        transposition_table[i].hash = 0;
    }
}

// 3. 计算棋盘的Zobrist哈希（增量计算，效率极高）
unsigned long long hashBoard(const Board* board) {
    unsigned long long hash = 0;
    for (int i = 1; i <= BOARD_SIZE; i++) {
        for (int j = 1; j <= BOARD_SIZE; j++) {
            int color = board->pieceColor[i][j]; // 0=空,1=黑,2=白
            hash ^= zobrist_table[i][j][color]; // 异或操作生成哈希
        }
    }
    return hash;
}

// 4. 落子/撤子的增量哈希更新（无需重新计算整个棋盘，核心优化）
void updateHash(unsigned long long* hash, int row, int col, int old_color, int new_color) {
    *hash ^= zobrist_table[row][col][old_color]; // 移除旧颜色的哈希
    *hash ^= zobrist_table[row][col][new_color]; // 添加新颜色的哈希
}

// 5. 存储到置换表（哈希值对大质数取模）
void storeTT(unsigned long long hash, int score, int depth, int type) {
    int idx = hash % TT_PRIME; // 对大质数取模，映射到哈希表索引
    // 替换策略：直接覆盖（也可加时间戳/深度优先替换）
    transposition_table[idx].hash = hash;
    transposition_table[idx].score = score;
    transposition_table[idx].depth = depth;
    transposition_table[idx].type = type;
}

// 6. 从置换表读取（同样对大质数取模查索引）
bool retrieveTT(unsigned long long hash, int* score, int* depth, int* type) {
    int idx = hash % TT_PRIME; // 大质数取模定位
    if (transposition_table[idx].hash != hash) return false; // 未命中（无冲突）
    *score = transposition_table[idx].score;
    *depth = transposition_table[idx].depth;
    *type = transposition_table[idx].type;
    return true;
}

// ===================== 原有函数适配（增量哈希优化） =====================
void initAI() {
    static int initialized = 0;
    if (!initialized) {
        srand((unsigned int)time(NULL));
        initZobrist(); // 初始化Zobrist表
        clearTT();
        initialized = 1;
    }
    st_ms = GetTickCount();
}

// 修改Alpha-Beta函数，使用增量哈希（减少哈希计算耗时）
int alphaBetaSearch(Board* board, int depth, int alpha, int beta, bool is_max_player, Piece ai_color, unsigned long long current_hash) {
    // 先查置换表（对大质数取模已在retrieveTT中完成）
    int tt_score, tt_depth, tt_type;
    if (retrieveTT(current_hash, &tt_score, &tt_depth, &tt_type)) {
        if (tt_depth >= depth) {
            if (tt_type == EXACT) return tt_score;
            if (tt_type == LOWER && tt_score >= beta) return tt_score;
            if (tt_type == UPPER && tt_score <= alpha) return tt_score;
        }
    }

    if (depth == 0) {
        int score = evaluateFullBoard(board, (ai_color==BLACK)?PLAYER_BLACK:PLAYER_WHITE);
        storeTT(current_hash, score, 0, EXACT);
        return score;
    }
    
    Piece current_color = is_max_player ? ai_color : (ai_color == BLACK ? WHITE : BLACK);
    int row, col;
    int best_score;
    int tt_entry_type = UPPER;

    if (is_max_player) {
        best_score = INT_MIN;
        PossibleMoves possible_moves[BOARD_SIZE*BOARD_SIZE+1];
        int move_cnt = generatePossibleMoves(board, possible_moves, current_color);
        for(int i=0; i<move_cnt; ++i){
            if(possible_moves[i].score < 0) continue;
            row = possible_moves[i].row;
            col = possible_moves[i].col;
            
            if (dropPiece(board, row, col, current_color) != 1) continue;
            
            // 增量更新哈希（落子：空→当前颜色）
            updateHash(&current_hash, row, col, BLANK, current_color);
            
            int score = alphaBetaSearch(board, depth - 1, alpha, beta, false, ai_color, current_hash);
            
            // 撤销落子+恢复哈希
            board->pieceColor[row][col] = BLANK;
            board->pieceTotal--;
            updateHash(&current_hash, row, col, current_color, BLANK); // 恢复哈希
            for(int dx=-NEIGHBORHOOD_SIZE; dx<=NEIGHBORHOOD_SIZE; ++dx){
                for(int dy=-NEIGHBORHOOD_SIZE; dy<=NEIGHBORHOOD_SIZE; ++dy){
                    if(1<=(row+dx) && (row+dx)<=BOARD_SIZE && 1<=(col+dy) && (col+dy)<=BOARD_SIZE){
                        board->possibleMove[row+dx][col+dy] -= 1;
                    }
                }
            }
            
            if (score > best_score) best_score = score;
            if (best_score > alpha) {
                alpha = best_score;
                tt_entry_type = EXACT;
            }
            if (alpha >= beta) {
                tt_entry_type = LOWER;
                break;
            }
        }
    } else {
        best_score = INT_MAX;
        PossibleMoves possible_moves[BOARD_SIZE*BOARD_SIZE+1];
        int move_cnt = generatePossibleMoves(board, possible_moves, current_color);
        for(int i=0; i<move_cnt; ++i){
            if(possible_moves[i].score < 0) continue;
            row = possible_moves[i].row;
            col = possible_moves[i].col;
            
            if (dropPiece(board, row, col, current_color) != 1) continue;
            
            // 增量更新哈希
            updateHash(&current_hash, row, col, BLANK, current_color);
            
            int score = alphaBetaSearch(board, depth - 1, alpha, beta, true, ai_color, current_hash);
            
            // 撤销落子+恢复哈希
            board->pieceColor[row][col] = BLANK;
            board->pieceTotal--;
            updateHash(&current_hash, row, col, current_color, BLANK);
            for(int dx=-NEIGHBORHOOD_SIZE; dx<=NEIGHBORHOOD_SIZE; ++dx){
                for(int dy=-NEIGHBORHOOD_SIZE; dy<=NEIGHBORHOOD_SIZE; ++dy){
                    if(1<=(row+dx) && (row+dx)<=BOARD_SIZE && 1<=(col+dy) && (col+dy)<=BOARD_SIZE){
                        board->possibleMove[row+dx][col+dy] -= 1;
                    }
                }
            }
            
            if (score < best_score) best_score = score;
            if (best_score < beta) {
                beta = best_score;
                tt_entry_type = EXACT;
            }
            if (alpha >= beta) {
                tt_entry_type = UPPER;
                break;
            }
        }
    }

    storeTT(current_hash, best_score, depth, tt_entry_type);
    return best_score;
}

// 迭代加深函数适配增量哈希
int iterativeDeepeningSearch(Board* board, Piece ai_color, int start_depth, int target_depth, DWORD time_limit_ms, int* best_r, int* best_c) {
    int final_best_score = INT_MIN;
    *best_r = -1;
    *best_c = -1;
    DWORD start_time = GetTickCount();
    
    // 初始哈希（只计算一次）
    unsigned long long board_hash = hashBoard(board);

    for (int current_depth = start_depth; current_depth <= target_depth; current_depth++) {
        if (GetTickCount() - start_time > time_limit_ms) {
            printf("迭代加深超时，终止于深度：%d\n", current_depth - 1);
            break;
        }
        
        int current_best_score = INT_MIN;
        int current_best_r = -1, current_best_c = -1;
        PossibleMoves possible_moves[BOARD_SIZE*BOARD_SIZE+1];
        int move_cnt = generatePossibleMoves(board, possible_moves, ai_color);
        
        for (int i = 0; i < move_cnt; i++) {
            if (possible_moves[i].score < 0) continue;
            int row = possible_moves[i].row;
            int col = possible_moves[i].col;
            
            if (dropPiece(board, row, col, ai_color) != 1) continue;
            
            // 增量更新哈希（落子）
            updateHash(&board_hash, row, col, BLANK, ai_color);
            int score = alphaBetaSearch(board, current_depth - 1, INT_MIN, INT_MAX, false, ai_color, board_hash);
            
            // 撤销落子+恢复哈希
            board->pieceColor[row][col] = BLANK;
            board->pieceTotal--;
            updateHash(&board_hash, row, col, ai_color, BLANK); // 恢复哈希
            for(int dx=-NEIGHBORHOOD_SIZE; dx<=NEIGHBORHOOD_SIZE; ++dx){
                for(int dy=-NEIGHBORHOOD_SIZE; dy<=NEIGHBORHOOD_SIZE; ++dy){
                    if(1<=(row+dx) && (row+dx)<=BOARD_SIZE && 1<=(col+dy) && (col+dy)<=BOARD_SIZE){
                        board->possibleMove[row+dx][col+dy] -= 1;
                    }
                }
            }
            
            if (score > current_best_score) {
                current_best_score = score;
                current_best_r = row;
                current_best_c = col;
            }
        }
        
        if (current_best_score > final_best_score && current_best_r != -1) {
            final_best_score = current_best_score;
            *best_r = current_best_r;
            *best_c = current_best_c;
        }
        
        printf("迭代加深：深度%d 最佳分数=%d 位置=(%d,%d)\n", current_depth, final_best_score, *best_r, *best_c);
    }
    
    return final_best_score;
}

// 其他函数（initAI/generatePossibleMoves/aiMakeDecision等）保持不变，仅调用时适配哈希参数