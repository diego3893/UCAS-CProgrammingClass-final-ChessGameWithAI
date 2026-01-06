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

// Alpha-Beta剪枝搜索
int alphaBetaSearch(Board* board, int depth, int alpha, int beta, bool is_max_player, Piece ai_color) {
    // 到达搜索深度或游戏结束，返回评估值
    if (depth == 0) {
        return evaluatePosition(board, (ai_color==BLACK)?PLAYER_BLACK:PLAYER_WHITE);
    }
    
    Piece current_color = is_max_player ? ai_color : (ai_color == BLACK ? WHITE : BLACK);

    int row, col;
    bool pruning = false;
    if (is_max_player) {
        int max_score = INT_MIN;
        for (int i = 1; i <=15; ++i) {
            if(pruning){
                break;
            }
            for(int j=1; j<=15; ++j){
                if(board->pieceColor[i][j] == BLANK){
                    if(board->possibleMoves[i][j] > 0){
                        row = i;
                        col = j;
                    }else{
                        continue;
                    }
                }else{
                    continue;
                }
                
                // 检查是否为禁手（仅对黑棋有效）
                if (current_color == BLACK && isForbiddenPosition(board, row, col)) {
                    continue;
                }
                
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
                            board->possibleMoves[row+dx][col+dy] -= 1;
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
                    pruning = true;
                    break;
                }
            }
        }
        return max_score;
    } else {
        int min_score = INT_MAX;
        for (int i = 1; i <=15; i++) {
            if(pruning){
                break;
            }
            for(int j=1; j<=15; ++j){
                if(board->pieceColor[i][j] == BLANK){
                    if(board->possibleMoves[i][j] > 0){
                        row = i;
                        col = j;
                    }else{
                        continue;
                    }
                }else{
                    continue;
                }
                
                // 检查是否为禁手（仅对黑棋有效）
                if (current_color == BLACK && isForbiddenPosition(board, row, col)) {
                    continue;
                }
                
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
                            board->possibleMoves[row+dx][col+dy] -= 1;
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
                    pruning = true;
                    break;
                }
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
    for (int i = 1; i <= 15; i++) {
        for(int j=1; j<=15; ++j){
            if(board->pieceColor[i][j] == BLANK){
                if(board->possibleMoves[i][j] > 0){
                    current_row = i;
                    current_col = j;
                }else{
                    continue;
                }
            }else{
                continue;
            }
            
            // 检查是否为禁手（仅对黑棋有效）
            if (ai_color == BLACK && isForbiddenPosition(&board_copy, current_row, current_col)) {
                continue;
            }
            
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
                        board_copy.possibleMoves[current_row+dx][current_col+dy] -= 1;
                    }
                }
            }
            
            // 更新最佳分数和位置
            if (score > best_score) {
                best_score = score;
                best_move_row = i;
                best_move_col = j;
            }
        }
        
    }
    
    // 返回最佳落子位置
    *row = best_move_row;
    *col = best_move_col;
    ed_ms = GetTickCount();
    double time_use = (double)(ed_ms-st_ms);
    return time_use;
}