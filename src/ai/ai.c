#include "ai.h"
// 修改：迭代加深超时后减小深度

DWORD st_ms, now_ms;
bool deepening_stop;

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
    for(int i=1; i<=BOARD_SIZE; ++i){
        for(int j=1; j<=BOARD_SIZE; ++j){
            if(board->pieceColor[i][j] == BLANK){
                if(board->possibleMove[i][j] > 0){
                    possible_moves[Index].row = i;
                    possible_moves[Index].col = j;
                    possible_moves[Index++].score = evaluatePostion(board, i, j, ai_color, is_max_player?ai_color:(ai_color==BLACK?WHITE:BLACK));
                }else{
                    continue;
                }
            }else{
                continue;
            }
        }
    }
    // if(is_max_player){
    //     qsort(possible_moves, Index, sizeof(PossibleMoves), cmp_max);
    // }else{
    //     qsort(possible_moves, Index, sizeof(PossibleMoves), cmp_max);
    // }
    qsort(possible_moves, Index, sizeof(PossibleMoves), cmp);
    return Index;
}

int alphaBetaSearch(Board* board, int depth, int alpha, int beta, bool is_max_player, Piece ai_color, ULL current_hash){
    if((double)(GetTickCount()-st_ms) >= MAX_TIME){
        deepening_stop = true;
        return is_max_player?INT_MIN:INT_MAX;
    }
    int tt_score, tt_depth, tt_type;
    if(retrieveTT(current_hash, &tt_score, &tt_depth, &tt_type)){
        if(tt_depth >= depth){
            if(tt_type == EXACT){
                return tt_score;
            }
            if(tt_type == LOWER && tt_score >= beta){
                return tt_score;
            }
            if(tt_type == UPPER && tt_score <= alpha){
                return tt_score;
            }
        }
    }

    if(depth == 0){
        int score = evaluateFullBoard(board,(ai_color==BLACK)?PLAYER_BLACK:PLAYER_WHITE);
        storeTT(current_hash, score, 0, EXACT);
        return score;
    }
    
    Piece current_color = is_max_player?ai_color:(ai_color==BLACK?WHITE:BLACK);
    int row, col;
    int best_score;
    int tt_entry_type = UPPER;

    if(is_max_player){
        best_score = INT_MIN;
        PossibleMoves possible_moves[BOARD_SIZE*BOARD_SIZE+1];
        int move_cnt = generatePossibleMoves(board, possible_moves, ai_color, is_max_player);
        for(int i=0; i<move_cnt; ++i){
            if(possible_moves[i].score < 0){
                continue;
            }
            row = possible_moves[i].row;
            col = possible_moves[i].col;       
            if(dropPiece(board, row, col, current_color) != 1){
                continue;
            }

            updateHash(&current_hash, row, col, BLANK, current_color);
            
            int score = alphaBetaSearch(board, depth - 1, alpha, beta, false, ai_color, current_hash);
            
            board->pieceColor[row][col] = BLANK;
            board->pieceTotal--;
            updateHash(&current_hash, row, col, current_color, BLANK);
            for(int dx=-NEIGHBORHOOD_SIZE; dx<=NEIGHBORHOOD_SIZE; ++dx){
                for(int dy=-NEIGHBORHOOD_SIZE; dy<=NEIGHBORHOOD_SIZE; ++dy){
                    if(1<=(row+dx) &&(row+dx)<=BOARD_SIZE && 1<=(col+dy) &&(col+dy)<=BOARD_SIZE){
                        board->possibleMove[row+dx][col+dy] -= 1;
                    }
                }
            }
            
            if(score > best_score){
                best_score = score;
            }
            if(best_score > alpha){
                alpha = best_score;
                tt_entry_type = EXACT;
            }
            if(alpha >= beta){
                tt_entry_type = LOWER;
                break;
            }
        }
    }else{
        best_score = INT_MAX;
        PossibleMoves possible_moves[BOARD_SIZE*BOARD_SIZE+1];
        int move_cnt = generatePossibleMoves(board, possible_moves, ai_color, is_max_player);
        for(int i=0; i<move_cnt; ++i){
            if(possible_moves[i].score < 0){
                continue;
            }
            row = possible_moves[i].row;
            col = possible_moves[i].col;
            if(dropPiece(board, row, col, current_color) != 1){
                continue;
            }
            
            updateHash(&current_hash, row, col, BLANK, current_color);
            
            int score = alphaBetaSearch(board, depth - 1, alpha, beta, true, ai_color, current_hash);
            
            board->pieceColor[row][col] = BLANK;
            board->pieceTotal--;
            updateHash(&current_hash, row, col, current_color, BLANK);
            for(int dx=-NEIGHBORHOOD_SIZE; dx<=NEIGHBORHOOD_SIZE; ++dx){
                for(int dy=-NEIGHBORHOOD_SIZE; dy<=NEIGHBORHOOD_SIZE; ++dy){
                    if(1<=(row+dx) &&(row+dx)<=BOARD_SIZE && 1<=(col+dy) &&(col+dy)<=BOARD_SIZE){
                        board->possibleMove[row+dx][col+dy] -= 1;
                    }
                }
            }
            
            if(score < best_score){
                best_score = score;
            }
            if(best_score < beta){
                beta = best_score;
                tt_entry_type = EXACT;
            }
            if(alpha >= beta){
                tt_entry_type = UPPER;
                break;
            }
        }
    }

    storeTT(current_hash, best_score, depth, tt_entry_type);
    return best_score;
}

int iterativeDeepeningSearch(Board* board, Piece ai_color, int start_depth, int target_depth, int* best_r, int* best_c){
    int final_best_score = INT_MIN;
    deepening_stop = false;
    *best_r = -1;
    *best_c = -1;
    
    ULL board_hash = hashBoard(board);

    for(int current_depth=start_depth; current_depth<=target_depth; current_depth+=2){
        if(GetTickCount()-st_ms > MAX_TIME){
            printf("迭代加深超时，终止于深度：%d\n", current_depth-2);
            break;
        }
        
        int current_best_score = INT_MIN;
        int current_best_r = -1, current_best_c = -1;
        PossibleMoves possible_moves[BOARD_SIZE*BOARD_SIZE+1];
        int move_cnt = generatePossibleMoves(board, possible_moves, ai_color, true);
        
        for(int i=0; i<move_cnt; ++i){
            if(possible_moves[i].score < 0){
                continue;
            }
            int row = possible_moves[i].row;
            int col = possible_moves[i].col;
            if(dropPiece(board, row, col, ai_color) != 1){
                continue;
            }
            
            updateHash(&board_hash, row, col, BLANK, ai_color);
            int score = alphaBetaSearch(board, current_depth - 1, INT_MIN, INT_MAX, false, ai_color, board_hash);
            
            board->pieceColor[row][col] = BLANK;
            board->pieceTotal--;
            updateHash(&board_hash, row, col, ai_color, BLANK); 
            for(int dx=-NEIGHBORHOOD_SIZE; dx<=NEIGHBORHOOD_SIZE; ++dx){
                for(int dy=-NEIGHBORHOOD_SIZE; dy<=NEIGHBORHOOD_SIZE; ++dy){
                    if(1<=(row+dx) &&(row+dx)<=BOARD_SIZE && 1<=(col+dy) &&(col+dy)<=BOARD_SIZE){
                        board->possibleMove[row+dx][col+dy] -= 1;
                    }
                }
            }
            
            if(score > current_best_score){
                current_best_score = score;
                current_best_r = row;
                current_best_c = col;
            }
        }
        if(deepening_stop){
            printf("迭代加深超时，终止于深度：%d\n", current_depth-2);
            break;
        }
        
        if(current_best_score>final_best_score && current_best_r!=-1 && current_best_c!=-1){
            final_best_score = current_best_score;
            *best_r = current_best_r;
            *best_c = current_best_c;
        }
        
        printf("迭代加深：深度%d 最佳分数=%d 位置=%c%d\n", current_depth, final_best_score, (char)(*best_c+'A'-1), *best_r);
    }
    
    return final_best_score;
}

double aiMakeDecision(const Board* board, Piece ai_color, int* row, int* col){
    initAI();
    
    Board board_copy = *board;
    
    if(board_copy.pieceTotal==0 && ai_color==BLACK){
        *row = 8;
        *col = 8;
        now_ms = GetTickCount();
        double time_use =(double)(now_ms-st_ms);
        return time_use;
    }
    
    PossibleMoves possible_moves[BOARD_SIZE*BOARD_SIZE+1];
    int move_cnt = generatePossibleMoves(board, possible_moves, ai_color, true);

    for(int i=0; i<move_cnt; ++i){
        now_ms = GetTickCount();
        if((double)(now_ms-st_ms) > MAX_TIME){
            break;
        }
        if(possible_moves[i].score < 100000){
            break;
        }
        if(possible_moves[i].score >= 100000){
            *row = possible_moves[i].row;
            *col = possible_moves[i].col;
            now_ms = GetTickCount();
            double time_use =(double)(now_ms-st_ms);
            return time_use;
        }
    }

    int score = iterativeDeepeningSearch(&board_copy, ai_color, ST_SEARCH_DEPTH, TG_SEARCH_DEPTH, row, col);
    
    now_ms = GetTickCount();
    double time_use =(double)(now_ms-st_ms);
    if(*row != -1 && *col != -1){
        printf("AI迭代加深搜索完成，最终落子分数为：%d\n", score);
    }else{
        printf("AI未找到有效落子，在已知合法落子中挑选排序最高的\n");
        for(int i = 0; i < move_cnt; i++){
            *row = possible_moves[i].row;
            *col = possible_moves[i].col;
            break;
        }
    }
    return time_use;
}