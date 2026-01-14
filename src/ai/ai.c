#include "ai.h"

DWORD st_ms, now_ms;
bool deepening_stop;
int timeout_cnt = 0;
bool reset_neighborhood_size = false;

// 启发式数据结构
int killer_moves[TG_SEARCH_DEPTH + 1][2][2]; // [深度][两个名额][行/列]
int history_table[BOARD_SIZE + 1][BOARD_SIZE + 1];

void clearHeuristics() {
    memset(killer_moves, 0, sizeof(killer_moves));
    memset(history_table, 0, sizeof(history_table));
}

void initAI(){
    static int initialized = 0;
    if(!initialized){
        srand((unsigned int)time(NULL));
        initZobrist();
        clearTT();
        initialized = 1;
    }
    st_ms = GetTickCount();
}

int generatePossibleMoves(const Board* board, PossibleMoves possible_moves[], Piece ai_color, bool is_max_player){
    int Index = 0;
    // 确定当前这一层是谁在落子
    Piece current_color = is_max_player ? ai_color : (ai_color == BLACK ? WHITE : BLACK);
    
    for(int i=1; i<=BOARD_SIZE; ++i){
        for(int j=1; j<=BOARD_SIZE; ++j){
            if(board->pieceColor[i][j] == BLANK && board->possibleMove[i][j] > 0){
                // 新增：如果当前落子方是黑棋，且该位置是禁手点，则跳过（除非能成五）
                // isForbiddenPosition 内部已处理五连优先逻辑
                if (current_color == BLACK && isForbiddenPosition(board, i, j)) {
                    continue;
                }

                possible_moves[Index].row = i;
                possible_moves[Index].col = j;
                possible_moves[Index++].score = evaluatePostion(board, i, j, ai_color, current_color);
            }
        }
    }
    return Index;
}

// 综合移动排序函数
void sortMovesHeuristic(PossibleMoves moves[], int count, int depth, int tt_r, int tt_c) {
    for (int i = 0; i < count; i++) {
        // 1. 置换表推荐走法权重最高
        if (moves[i].row == tt_r && moves[i].col == tt_c) moves[i].score += 1000000;
        // 2. 杀手走法次之
        else if ((moves[i].row == killer_moves[depth][0][0] && moves[i].col == killer_moves[depth][0][1]) ||
                 (moves[i].row == killer_moves[depth][1][0] && moves[i].col == killer_moves[depth][1][1])) {
            moves[i].score += 500000;
        }
        // 3. 历史得分
        else moves[i].score += history_table[moves[i].row][moves[i].col];
    }
    qsort(moves, count, sizeof(PossibleMoves), cmp);
}

int alphaBetaSearch(Board* board, int depth, int alpha, int beta, bool is_max_player, Piece ai_color, ULL current_hash){
    if((double)(GetTickCount()-st_ms) >= MAX_TIME){
        deepening_stop = true;
        return is_max_player ? INT_MIN + 100 : INT_MAX - 100;
    }

    int tt_score, tt_depth, tt_type, tt_r = -1, tt_c = -1;
    if(retrieveTT(current_hash, &tt_score, &tt_depth, &tt_type, &tt_r, &tt_c)){
        if(tt_depth >= depth){
            if(tt_type == EXACT) return tt_score;
            if(tt_type == LOWER && tt_score >= beta) return tt_score;
            if(tt_type == UPPER && tt_score <= alpha) return tt_score;
        }
    }

    if(depth == 0) return evaluateFullBoard(board, (ai_color==BLACK)?PLAYER_BLACK:PLAYER_WHITE);
    
    PossibleMoves moves[BOARD_SIZE*BOARD_SIZE+1];
    int move_cnt = generatePossibleMoves(board, moves, ai_color, is_max_player);
    if(move_cnt == 0) return evaluateFullBoard(board, (ai_color==BLACK)?PLAYER_BLACK:PLAYER_WHITE);
    
    sortMovesHeuristic(moves, move_cnt, depth, tt_r, tt_c);

    int best_score = is_max_player ? INT_MIN : INT_MAX;
    int best_r = -1, best_c = -1;
    Piece current_color = is_max_player ? ai_color : (ai_color==BLACK?WHITE:BLACK);
    bool found_pv = false;

    for(int i=0; i<move_cnt; ++i){
        int row = moves[i].row;
        int col = moves[i].col;
        if(dropPiece(board, row, col, current_color) != 1) continue;
        updateHash(&current_hash, row, col, BLANK, current_color);

        int score;
        GameStatus status = judgeStatus(board, row, col, is_max_player ? 
                            ((ai_color==BLACK)?PLAYER_BLACK:PLAYER_WHITE) : 
                            ((ai_color==BLACK)?PLAYER_WHITE:PLAYER_BLACK));
        if (status == FORBIDDEN_MOVE) {
            // 当前玩家走出了禁手，对于当前层玩家来说是极大的惩罚
            score = is_max_player ? (INT_MIN + 1000 + depth) : (INT_MAX - 1000 - depth);
        } else if (status == BLACK_WIN || status == WHITE_WIN) {
            // 当前玩家获胜
            score = is_max_player ? (INT_MAX - 1000 - depth) : (INT_MIN + 1000 + depth);
        } else if (depth == 0 || status == DRAW) {
            score = evaluateFullBoard(board, (ai_color==BLACK)?PLAYER_BLACK:PLAYER_WHITE);
        } else {
            if(!found_pv){
                // 第一个节点使用全窗口
                score = alphaBetaSearch(board, depth - 1, alpha, beta, !is_max_player, ai_color, current_hash);
            } else {
                // PVS：后续节点先尝试零窗口搜索
                if(is_max_player){
                    score = alphaBetaSearch(board, depth - 1, alpha, alpha + 1, false, ai_color, current_hash);
                    if(score > alpha && score < beta) // 零窗口失败，重搜
                        score = alphaBetaSearch(board, depth - 1, alpha, beta, false, ai_color, current_hash);
                } else {
                    score = alphaBetaSearch(board, depth - 1, beta - 1, beta, true, ai_color, current_hash);
                    if(score < beta && score > alpha)
                        score = alphaBetaSearch(board, depth - 1, alpha, beta, true, ai_color, current_hash);
                }
            }
        }   

        // 撤销落子
        board->pieceColor[row][col] = BLANK;
        board->pieceTotal--;
        updateHash(&current_hash, row, col, current_color, BLANK);
        for(int dx=-NEIGHBORHOOD_SIZE; dx<=NEIGHBORHOOD_SIZE; ++dx)
            for(int dy=-NEIGHBORHOOD_SIZE; dy<=NEIGHBORHOOD_SIZE; ++dy)
                if(1<=(row+dx) &&(row+dx)<=BOARD_SIZE && 1<=(col+dy) &&(col+dy)<=BOARD_SIZE)
                    board->possibleMove[row+dx][col+dy] -= 1;

        if(is_max_player){
            if(score > best_score){
                best_score = score;
                best_r = row; best_c = col;
            }
            if(best_score > alpha){
                alpha = best_score;
                found_pv = true;
                history_table[row][col] += depth * depth;
            }
        } else {
            if(score < best_score){
                best_score = score;
                best_r = row; best_c = col;
            }
            if(best_score < beta){
                beta = best_score;
                found_pv = true;
            }
        }

        if(alpha >= beta){
            // 记录杀手移动
            killer_moves[depth][1][0] = killer_moves[depth][0][0];
            killer_moves[depth][1][1] = killer_moves[depth][0][1];
            killer_moves[depth][0][0] = row;
            killer_moves[depth][0][1] = col;
            storeTT(current_hash, best_score, depth, is_max_player ? LOWER : UPPER, row, col);
            return best_score;
        }
    }

    storeTT(current_hash, best_score, depth, EXACT, best_r, best_c);
    return best_score;
}

int iterativeDeepeningSearch(Board* board, Piece ai_color, int start_depth, int target_depth, int* best_r, int* best_c){
    int final_best_score = INT_MIN;
    deepening_stop = false;
    clearHeuristics(); // 每次大搜索开始前清空历史记录

    ULL board_hash = hashBoard(board);

    for(int current_depth=start_depth; current_depth<=target_depth; current_depth+=1){
        if(GetTickCount()-st_ms > MAX_TIME){
            timeout_cnt++;
            break;
        }
        
        int tt_r = -1, tt_c = -1, tt_s, tt_d, tt_t;
        retrieveTT(board_hash, &tt_s, &tt_d, &tt_t, &tt_r, &tt_c);

        PossibleMoves moves[BOARD_SIZE*BOARD_SIZE+1];
        int move_cnt = generatePossibleMoves(board, moves, ai_color, true);
        sortMovesHeuristic(moves, move_cnt, current_depth, tt_r, tt_c);

        int current_best_r = -1, current_best_c = -1;
        int current_best_score = INT_MIN;

        for(int i=0; i<move_cnt; ++i){
            int row = moves[i].row;
            int col = moves[i].col;
            if(dropPiece(board, row, col, ai_color) != 1) continue;
            updateHash(&board_hash, row, col, BLANK, ai_color);

            int score = alphaBetaSearch(board, current_depth - 1, INT_MIN + 100, INT_MAX - 100, false, ai_color, board_hash);
            
            board->pieceColor[row][col] = BLANK; board->pieceTotal--;
            updateHash(&board_hash, row, col, ai_color, BLANK); 
            for(int dx=-NEIGHBORHOOD_SIZE; dx<=NEIGHBORHOOD_SIZE; ++dx)
                for(int dy=-NEIGHBORHOOD_SIZE; dy<=NEIGHBORHOOD_SIZE; ++dy)
                    if(1<=(row+dx) &&(row+dx)<=BOARD_SIZE && 1<=(col+dy) &&(col+dy)<=BOARD_SIZE)
                        board->possibleMove[row+dx][col+dy] -= 1;

            if(score > current_best_score){
                current_best_score = score;
                current_best_r = row; current_best_c = col;
            }
        }
        if(deepening_stop){
            timeout_cnt++;
            break;
        }
        
        *best_r = current_best_r; *best_c = current_best_c;
        final_best_score = current_best_score;
        printf("迭代加深：深度%d 分数=%d 位置=%c%d\n", current_depth, final_best_score, (char)(*best_c+'A'-1), *best_r);
    }
    return final_best_score;
}

double aiMakeDecision(const Board* board, Piece ai_color, int* row, int* col){
    initAI();
    Board board_copy = *board;
    if(board_copy.pieceTotal==0 && ai_color==BLACK){
        *row = 8; *col = 8;
        return (double)(GetTickCount()-st_ms);
    }
    
    // 检查必杀点
    PossibleMoves pm[BOARD_SIZE*BOARD_SIZE+1];
    int count = generatePossibleMoves(board, pm, ai_color, true);
    for(int i=0; i<count; i++){
        if(pm[i].score >= 100000){
            *row = pm[i].row; *col = pm[i].col;
            return (double)(GetTickCount()-st_ms);
        }
    }

    int tg_depth = max(ST_SEARCH_DEPTH, TG_SEARCH_DEPTH-floor(timeout_cnt/3.0));
    iterativeDeepeningSearch(&board_copy, ai_color, ST_SEARCH_DEPTH, tg_depth, row, col);
    
    return (double)(GetTickCount()-st_ms);
}