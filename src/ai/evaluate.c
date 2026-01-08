#include "evaluate.h"

// double position_weight[BOARD_SIZE+1][BOARD_SIZE+1] = {0};

// 棋型评分表（根据对胜负的影响程度赋值）
const int SHAPE_SCORES[] = {
    10,        // 活二
    100,        // 活三
    1000,       // 冲四
    10000,       // 活四
    100000,      // 五连
    -100000      // 禁手（仅黑棋）
};

int evaluateFullBoard(const Board* board, Player ai_player){
    int total_score = 0;

    // 遍历棋盘所有落子点，评估双方棋型
    for(int row=1; row<=BOARD_SIZE; ++row){
        for(int col=1; col<=BOARD_SIZE; ++col){
            Piece p = getPiece(board, row, col);
            if(p == BLANK){
                continue;
            }

            // 统计当前位置的棋型
            int shape_cnt[CHESS_SHAPE_CNT] = {0};
            checkChessShape(board, row, col, shape_cnt, (p==BLACK)?PLAYER_BLACK:PLAYER_WHITE);

            // 累加评分（己方为正，对方为负）
            int score = 0;
            score += shape_cnt[LIVE_TWO]*SHAPE_SCORES[LIVE_TWO];
            score += shape_cnt[LIVE_THREE]*SHAPE_SCORES[LIVE_THREE];
            score += shape_cnt[BREAKTHROUGH_FOUR]*SHAPE_SCORES[BREAKTHROUGH_FOUR];
            score += shape_cnt[LIVE_FOUR]*SHAPE_SCORES[LIVE_FOUR];
            score += shape_cnt[FIVE_IN_ROW]*SHAPE_SCORES[FIVE_IN_ROW];

            // 黑棋禁手扣分
            if(p==BLACK && isForbiddenMove(shape_cnt)){
                score += SHAPE_SCORES[5];
            }
            // 区分己方和对方评分
            total_score += (p==(ai_player==PLAYER_BLACK?BLACK:WHITE))?score:-score;
        }
    }

    return total_score;
}

int evaluatePostion(const Board* board, int row, int col, Piece ai_color, Piece current_color){

    // 统计当前位置的棋型
    int shape_cnt[CHESS_SHAPE_CNT] = {0}, opp_shape_cnt[CHESS_SHAPE_CNT] = {0};
    checkChessShape(board, row, col, shape_cnt, (ai_color==BLACK)?PLAYER_BLACK:PLAYER_WHITE);

    // 累加评分（己方为正，对方为负）
    int score = 0;
    score += shape_cnt[LIVE_TWO]*SHAPE_SCORES[LIVE_TWO];
    score += shape_cnt[LIVE_THREE]*SHAPE_SCORES[LIVE_THREE];
    score += shape_cnt[BREAKTHROUGH_FOUR]*SHAPE_SCORES[BREAKTHROUGH_FOUR];
    score += shape_cnt[LIVE_FOUR]*SHAPE_SCORES[LIVE_FOUR];
    score += shape_cnt[FIVE_IN_ROW]*SHAPE_SCORES[FIVE_IN_ROW];

    // 黑棋禁手扣分
    if(ai_color==BLACK && isForbiddenMove(shape_cnt)){
        score += SHAPE_SCORES[5];
    }

    checkChessShape(board, row, col, opp_shape_cnt, (ai_color==BLACK)?PLAYER_WHITE:PLAYER_BLACK);
    if(current_color == ai_color){
        score += opp_shape_cnt[LIVE_TWO]*SHAPE_SCORES[LIVE_TWO];
        score += 5*opp_shape_cnt[LIVE_THREE]*SHAPE_SCORES[LIVE_THREE];
        score += 10*opp_shape_cnt[BREAKTHROUGH_FOUR]*SHAPE_SCORES[BREAKTHROUGH_FOUR];
        score += 10*opp_shape_cnt[LIVE_FOUR]*SHAPE_SCORES[LIVE_FOUR];
        score += 20*opp_shape_cnt[FIVE_IN_ROW]*SHAPE_SCORES[FIVE_IN_ROW];
    }else{
        score += opp_shape_cnt[LIVE_TWO]*SHAPE_SCORES[LIVE_TWO];
        score += opp_shape_cnt[LIVE_THREE]*SHAPE_SCORES[LIVE_THREE];
        score += opp_shape_cnt[BREAKTHROUGH_FOUR]*SHAPE_SCORES[BREAKTHROUGH_FOUR];
        score += opp_shape_cnt[LIVE_FOUR]*SHAPE_SCORES[LIVE_FOUR];
        score += opp_shape_cnt[FIVE_IN_ROW]*SHAPE_SCORES[FIVE_IN_ROW];
    }

    return score;
}

int cmp(const void* a, const void* b){
    
    // 1. 将通用指针转换为结构体指针（C语言只能通过指针访问）
    const PossibleMoves *moveA = (const PossibleMoves *)a;
    const PossibleMoves *moveB = (const PossibleMoves *)b;
    
    // 2. 实现 a.score > b.score 的降序逻辑（对应你想要的 return a.score>b.score）
    if (moveA->score > moveB->score) {
        return -1;  // 成绩高的在前（降序），返回-1
    } else if (moveA->score < moveB->score) {
        return 1;   
    } else {
        return 0;   // 成绩相等，顺序不变
    }

}