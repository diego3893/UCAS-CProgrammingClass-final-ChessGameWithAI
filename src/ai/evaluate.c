#include "evaluate.h"


// 棋型评分表（根据对胜负的影响程度赋值）
const int SHAPE_SCORES[] = {
    10,        // 活二
    100,        // 活三
    1000,       // 冲四
    10000,       // 活四
    100000,      // 五连
    -100000      // 禁手（仅黑棋）
};

int evaluatePosition(const Board* board, Player current_player){
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
            total_score += (p==(current_player==PLAYER_BLACK?BLACK:WHITE))?score:-score;
        }
    }

    return total_score;
}