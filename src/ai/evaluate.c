#include "evaluate.h"

// 棋盘位置权重表（中心分值更高）
const int POSITION_WEIGHT[BOARD_SIZE+1][BOARD_SIZE+1] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,1,2,2,2,2,2,2,2,2,2,2,2,1,0},
    {0,0,1,2,3,3,3,3,3,3,3,3,3,2,1,0},
    {0,0,1,2,3,4,4,4,4,4,4,4,3,2,1,0},
    {0,0,1,2,3,4,5,5,5,5,5,4,3,2,1,0},
    {0,0,1,2,3,4,5,6,6,6,5,4,3,2,1,0},
    {0,0,1,2,3,4,5,6,8,6,5,4,3,2,1,0}, // 天元(8,8)
    {0,0,1,2,3,4,5,6,6,6,5,4,3,2,1,0},
    {0,0,1,2,3,4,5,5,5,5,5,4,3,2,1,0},
    {0,0,1,2,3,4,4,4,4,4,4,4,3,2,1,0},
    {0,0,1,2,3,3,3,3,3,3,3,3,3,2,1,0},
    {0,0,1,2,2,2,2,2,2,2,2,2,2,2,1,0},
    {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

const int SHAPE_SCORES[] = { 10, 100, 1000, 10000, 100000, -100000 };

int evaluateFullBoard(const Board* board, Player ai_player){
    int total_score = 0;
    Piece ai_color = (ai_player == PLAYER_BLACK) ? BLACK : WHITE;

    for(int row=1; row<=BOARD_SIZE; ++row){
        for(int col=1; col<=BOARD_SIZE; ++col){
            Piece p = board->pieceColor[row][col];
            if(p == BLANK) continue;

            int shape_cnt[CHESS_SHAPE_CNT] = {0};
            checkChessShape(board, row, col, shape_cnt, (p==BLACK)?PLAYER_BLACK:PLAYER_WHITE);

            int score = 0;
            score += shape_cnt[LIVE_TWO]*SHAPE_SCORES[0];
            score += shape_cnt[LIVE_THREE]*SHAPE_SCORES[1];
            score += shape_cnt[BREAKTHROUGH_FOUR]*SHAPE_SCORES[2];
            score += shape_cnt[LIVE_FOUR]*SHAPE_SCORES[3];
            score += shape_cnt[FIVE_IN_ROW]*SHAPE_SCORES[4];
            
            // 叠加位置分
            score += POSITION_WEIGHT[row][col] * 2;

            if(p==BLACK && isForbiddenMove(shape_cnt)) score += SHAPE_SCORES[5];
            total_score += (p == ai_color) ? score : -score;
        }
    }
    return total_score;
}

int evaluatePostion(const Board* board, int row, int col, Piece ai_color, Piece current_color){
    int shape_cnt[CHESS_SHAPE_CNT] = {0}, opp_shape_cnt[CHESS_SHAPE_CNT] = {0};
    checkChessShape(board, row, col, shape_cnt, (ai_color==BLACK)?PLAYER_BLACK:PLAYER_WHITE);
    
    int score = 0;
    score += shape_cnt[LIVE_TWO]*SHAPE_SCORES[0];
    score += shape_cnt[LIVE_THREE]*SHAPE_SCORES[1];
    score += shape_cnt[BREAKTHROUGH_FOUR]*SHAPE_SCORES[2];
    score += shape_cnt[LIVE_FOUR]*SHAPE_SCORES[3];
    score += shape_cnt[FIVE_IN_ROW]*SHAPE_SCORES[4];
    if(ai_color==BLACK && isForbiddenMove(shape_cnt)) score += SHAPE_SCORES[5];

    checkChessShape(board, row, col, opp_shape_cnt, (ai_color==BLACK)?PLAYER_WHITE:PLAYER_BLACK);
    score += opp_shape_cnt[LIVE_TWO]*SHAPE_SCORES[0];
    score += opp_shape_cnt[LIVE_THREE]*SHAPE_SCORES[1];
    score += opp_shape_cnt[BREAKTHROUGH_FOUR]*SHAPE_SCORES[2];
    score += opp_shape_cnt[LIVE_FOUR]*SHAPE_SCORES[3];
    score += opp_shape_cnt[FIVE_IN_ROW]*SHAPE_SCORES[4];
    
    // 在位置评估中也加入基础权重，使候选走法排序更优
    score += POSITION_WEIGHT[row][col] * 5;
    
    return score;
}

int cmp(const void* a, const void* b){
    const PossibleMoves *moveA = (const PossibleMoves *)a;
    const PossibleMoves *moveB = (const PossibleMoves *)b;
    return (moveB->score - moveA->score);
}