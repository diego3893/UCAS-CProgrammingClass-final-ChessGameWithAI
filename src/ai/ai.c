#include "ai.h"

DWORD st_ms;

// 初始化随机数种子
void initAI() {
    static int initialized = 0;
    if (!initialized) {
        srand((unsigned int)time(NULL));
        initialized = 1;
    }
    st_ms = GetTickCount();
}

int generatePossibleMoves(const Board* board, PossibleMoves possible_moves[], Piece color){
    int Index = 0;
    for(int i=1; i<=BOARD_SIZE; ++i){
        for(int j=1; j<=BOARD_SIZE; ++j){
            if(board->pieceColor[i][j] == BLANK){
                if(board->possibleMove[i][j] > 0){
                    possible_moves[Index].row = i;
                    possible_moves[Index].col = j;
                    possible_moves[Index++].score = evaluatePostion(board, i, j, color);
                }else{
                    continue;
                }
            }else{
                continue;
            }
        }
    }
    qsort(possible_moves, Index, sizeof(PossibleMoves), cmp);
    return Index;
}

// Alpha-Beta剪枝搜索
int alphaBetaSearch(Board* board, int depth, int alpha, int beta, bool is_max_player, Piece ai_color) {
    // 到达搜索深度或游戏结束，返回评估值
    if (depth == 0) {
        return evaluateFullBoard(board, (ai_color==BLACK)?PLAYER_BLACK:PLAYER_WHITE);
    }
    
    Piece current_color = is_max_player ? ai_color : (ai_color == BLACK ? WHITE : BLACK);

    int row, col;
    if (is_max_player) {
        int max_score = INT_MIN;
        PossibleMoves possible_moves[BOARD_SIZE*BOARD_SIZE+1];
        int move_cnt = generatePossibleMoves(board, possible_moves, current_color);
        for(int i=0; i<move_cnt; ++i){
            if(possible_moves[i].score < 0){
                continue;
            }
            row = possible_moves[i].row;
            col = possible_moves[i].col;
            // 尝试落子
            if (dropPiece(board, row, col, current_color) != 1) {
                continue;
            }
            
            // 递归搜索
            int score = alphaBetaSearch(board, depth - 1, alpha, beta, false, ai_color);
            
            // 撤销落子
            board->pieceColor[row][col] = BLANK;
            board->pieceTotal--;
            for(int dx=-2; dx<=2; ++dx){
                for(int dy=-2; dy<=2; ++dy){
                    if(1<=(row+dx) && (row+dx)<=BOARD_SIZE && 1<=(col+dy) && (col+dy)<=BOARD_SIZE){
                        board->possibleMove[row+dx][col+dy] -= 1;
                    }
                }
            }
            if (score > max_score) {
                max_score = score;
            }
            if (max_score > alpha) {
                alpha = max_score;
            }
            // Alpha剪枝
            if (alpha >= beta) {
                break;
            }
        }

        return max_score;
    } else {
        int min_score = INT_MAX;
        PossibleMoves possible_moves[BOARD_SIZE*BOARD_SIZE+1];
        int move_cnt = generatePossibleMoves(board, possible_moves, current_color);
        for(int i=0; i<move_cnt; ++i){
            if(possible_moves[i].score < 0){
                continue;
            }
            row = possible_moves[i].row;
            col = possible_moves[i].col;
            // 尝试落子
            if (dropPiece(board, row, col, current_color) != 1) {
                continue;
            }
            
            // 递归搜索
            int score = alphaBetaSearch(board, depth - 1, alpha, beta, true, ai_color);
            
            // 撤销落子
            board->pieceColor[row][col] = BLANK;
            board->pieceTotal--;
            for(int dx=-2; dx<=2; ++dx){
                for(int dy=-2; dy<=2; ++dy){
                    if(1<=(row+dx) && (row+dx)<=BOARD_SIZE && 1<=(col+dy) && (col+dy)<=BOARD_SIZE){
                        board->possibleMove[row+dx][col+dy] -= 1;
                    }
                }
            }
            
            if (score < min_score) {
                min_score = score;
            }
            if (min_score < beta) {
                beta = min_score;
            }
            // Beta剪枝
            if (alpha >= beta) {
                break;
            }
        }

        return min_score;
    }
}

// AI决策函数
double aiMakeDecision(const Board* board, Piece ai_color, int* row, int* col) {
    DWORD ed_ms;
    initAI();
    
    // 创建棋盘副本，避免修改原棋盘
    Board board_copy = *board;
    
    
    // 处理第一手棋，直接落在天元
    if (board_copy.pieceTotal == 0 && ai_color == BLACK) {
        *row = 8;
        *col = 8;
        ed_ms = GetTickCount();
        double time_use = (double)(ed_ms-st_ms);
        return time_use;
    }
    
    int best_score = INT_MIN;
    int best_move_row = -1, best_move_col = -1;
    
    int current_row, current_col;
    // 遍历所有可能的落子位置
    PossibleMoves possible_moves[BOARD_SIZE*BOARD_SIZE+1];
    int move_cnt = generatePossibleMoves(board, possible_moves, ai_color);

    for(int i=0; i<move_cnt; ++i){
        if(possible_moves[i].score < 0){
            continue;
        }
        if(possible_moves[i].score >= 100000){
            *row = possible_moves[i].row;
            *col = possible_moves[i].col;
            ed_ms = GetTickCount();
            double time_use = (double)(ed_ms-st_ms);
            return time_use;
        }
        current_row = possible_moves[i].row;
        current_col = possible_moves[i].col;
        // 尝试落子
        if (dropPiece(&board_copy, current_row, current_col, ai_color) != 1) {
            continue;
        }
        
        // 搜索评估
        int score = alphaBetaSearch(&board_copy, SEARCH_DEPTH - 1, INT_MIN, INT_MAX, false, ai_color);
        
        // 撤销落子
        board_copy.pieceColor[current_row][current_col] = BLANK;
        board_copy.pieceTotal--;
        for(int dx=-2; dx<=2; ++dx){
            for(int dy=-2; dy<=2; ++dy){
                if(1<=(current_row+dx) && (current_row+dx)<=BOARD_SIZE && 
                    1<=(current_col+dy) && (current_col+dy)<=BOARD_SIZE){
                    board_copy.possibleMove[current_row+dx][current_col+dy] -= 1;
                }
            }
        }
        
        // 更新最佳分数和位置
        if (score > best_score) {
            best_score = score;
            best_move_row = current_row;
            best_move_col = current_col;
        }
    }

    
    // 返回最佳落子位置
    *row = best_move_row;
    *col = best_move_col;
    ed_ms = GetTickCount();
    double time_use = (double)(ed_ms-st_ms);
    return time_use;
}