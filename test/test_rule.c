#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include "../src/board/board.h"
#include "../src/rule/rule.h"

#define ASSERT(expr, msg) \
    do { \
        if (!(expr)) { \
            printf("测试失败: %s\n", msg); \
            return false; \
        } \
    } while(0)

void initTestBoard(Board* board, Piece pieces[BOARD_SIZE+1][BOARD_SIZE+1]) {
    boardInit(board);
    for (int i = 1; i <= BOARD_SIZE; i++) {
        for (int j = 1; j <= BOARD_SIZE; j++) {
            if (pieces[i][j] != BLANK) {
                dropPiece(board, i, j, pieces[i][j]);
            }
        }
    }
}

Piece color = BLACK;
Piece opp_color = WHITE;

void printCnt(int shape_cnt[]){
    printf("五连：%d\n", shape_cnt[FIVE_IN_ROW]);
    printf("长连：%d\n", shape_cnt[LONG_CHAIN]);
    printf("活二：%d\n", shape_cnt[LIVE_TWO]);
    printf("活三：%d\n", shape_cnt[LIVE_THREE]);
    printf("活四：%d\n", shape_cnt[LIVE_FOUR]);
    printf("冲四：%d\n", shape_cnt[BREAKTHROUGH_FOUR]);
    return;
}

bool testFiveInRowVertical() {
    Board board;
    Piece pieces[BOARD_SIZE+1][BOARD_SIZE+1] = {0};
    pieces[3][5] = color;
    pieces[4][5] = color;
    pieces[5][5] = color;
    pieces[6][5] = color;
    pieces[7][5] = color;
    initTestBoard(&board, pieces);

    int shape_cnt[CHESS_SHAPE_CNT] = {0};
    checkChessShape(&board, 5, 5, shape_cnt, (color==BLACK?PLAYER_BLACK:PLAYER_WHITE));
    printCnt(shape_cnt);
    ASSERT(shape_cnt[FIVE_IN_ROW] == 1, "纵向五连判定失败");
    printf("测试通过: 纵向五连\n");
    return true;
}

bool testFiveInRowDiagonal1() {
    Board board;
    Piece pieces[BOARD_SIZE+1][BOARD_SIZE+1] = {0};
    pieces[3][3] = color;
    pieces[4][4] = color;
    pieces[5][5] = color;
    pieces[6][6] = color;
    pieces[7][7] = color;
    initTestBoard(&board, pieces);

    int shape_cnt[CHESS_SHAPE_CNT] = {0};
    checkChessShape(&board, 5, 5, shape_cnt, (color==BLACK?PLAYER_BLACK:PLAYER_WHITE));
    printCnt(shape_cnt);
    ASSERT(shape_cnt[FIVE_IN_ROW] == 1, "正斜向五连判定失败");
    printf("测试通过: 正斜向五连\n");
    return true;
}

bool testFiveInRowDiagonal2() {
    Board board;
    Piece pieces[BOARD_SIZE+1][BOARD_SIZE+1] = {0};
    pieces[3][7] = color;
    pieces[4][6] = color;
    pieces[5][5] = color;
    pieces[6][4] = color;
    pieces[7][3] = color;
    initTestBoard(&board, pieces);

    int shape_cnt[CHESS_SHAPE_CNT] = {0};
    checkChessShape(&board, 5, 5, shape_cnt, (color==BLACK?PLAYER_BLACK:PLAYER_WHITE));
    printCnt(shape_cnt);
    ASSERT(shape_cnt[FIVE_IN_ROW] == 1, "反斜向五连判定失败");
    printf("测试通过: 反斜向五连\n");
    return true;
}

bool testLongChainVertical() {
    Board board;
    Piece pieces[BOARD_SIZE+1][BOARD_SIZE+1] = {0};
    pieces[2][5] = color;
    pieces[3][5] = color;
    pieces[4][5] = color;
    pieces[5][5] = color;
    pieces[6][5] = color;
    pieces[7][5] = color;
    initTestBoard(&board, pieces);

    int shape_cnt[CHESS_SHAPE_CNT] = {0};
    checkChessShape(&board, 5, 5, shape_cnt, (color==BLACK?PLAYER_BLACK:PLAYER_WHITE));
    printCnt(shape_cnt);
    ASSERT(shape_cnt[LONG_CHAIN] == 1, "纵向长连判定失败");
    printf("测试通过: 纵向长连\n");
    return true;
}

bool testLongChainDiagonal() {
    Board board;
    Piece pieces[BOARD_SIZE+1][BOARD_SIZE+1] = {0};
    pieces[2][2] = color;
    pieces[3][3] = color;
    pieces[4][4] = color;
    pieces[5][5] = color;
    pieces[6][6] = color;
    pieces[7][7] = color;
    initTestBoard(&board, pieces);

    int shape_cnt[CHESS_SHAPE_CNT] = {0};
    checkChessShape(&board, 5, 5, shape_cnt, (color==BLACK?PLAYER_BLACK:PLAYER_WHITE));
    printCnt(shape_cnt);
    ASSERT(shape_cnt[LONG_CHAIN] == 1, "斜向长连判定失败");
    printf("测试通过: 斜向长连\n");
    return true;
}

bool testDoubleLiveThree1() {
    Board board;
    Piece pieces[BOARD_SIZE+1][BOARD_SIZE+1] = {0};
    pieces[5][3] = color;
    pieces[5][4] = color;
    pieces[3][3] = color;
    pieces[4][4] = color;
    pieces[5][5] = color;
    initTestBoard(&board, pieces);

    int shape_cnt[CHESS_SHAPE_CNT] = {0};
    checkChessShape(&board, 5, 5, shape_cnt, (color==BLACK?PLAYER_BLACK:PLAYER_WHITE));
    printCnt(shape_cnt);
    ASSERT(shape_cnt[LIVE_THREE] == 2, "横向+斜向双活三判定失败");
    printf("测试通过: 横向+斜向双活三\n");
    return true;
}

bool testDoubleLiveThree2() {
    Board board;
    Piece pieces[BOARD_SIZE+1][BOARD_SIZE+1] = {0};
    pieces[5][2] = color;
    pieces[5][4] = color;
    pieces[3][5] = color;
    pieces[4][5] = color;
    pieces[5][5] = color;
    initTestBoard(&board, pieces);

    int shape_cnt[CHESS_SHAPE_CNT] = {0};
    checkChessShape(&board, 5, 5, shape_cnt, (color==BLACK?PLAYER_BLACK:PLAYER_WHITE));
    printCnt(shape_cnt);
    ASSERT(shape_cnt[LIVE_THREE] == 2, "跳活三+普通活三判定失败");
    printf("测试通过: 跳活三+普通活三\n");
    return true;
}

bool testDoubleLiveFour() {
    Board board;
    Piece pieces[BOARD_SIZE+1][BOARD_SIZE+1] = {0};
    pieces[5][2] = color;
    pieces[5][3] = color;
    pieces[5][4] = color;
    pieces[2][5] = color;
    pieces[3][5] = color;
    pieces[4][5] = color;
    pieces[5][5] = color; 
    initTestBoard(&board, pieces);

    int shape_cnt[CHESS_SHAPE_CNT] = {0};
    checkChessShape(&board, 5, 5, shape_cnt, (color==BLACK?PLAYER_BLACK:PLAYER_WHITE));
    printCnt(shape_cnt);
    ASSERT((shape_cnt[LIVE_FOUR] == 2), "双活四判定失败");
    printf("测试通过: 双活四\n");
    return true;
}

bool testLiveFourPlusBreakthrough() {
    Board board;
    Piece pieces[BOARD_SIZE+1][BOARD_SIZE+1] = {0};
    pieces[5][2] = color;
    pieces[5][3] = color;
    pieces[5][4] = color;
    pieces[2][5] = color;
    pieces[3][5] = color;
    pieces[4][5] = color;
    pieces[6][5] = opp_color;  
    pieces[5][5] = color; 
    initTestBoard(&board, pieces);

    int shape_cnt[CHESS_SHAPE_CNT] = {0};
    checkChessShape(&board, 5, 5, shape_cnt, (color==BLACK?PLAYER_BLACK:PLAYER_WHITE));
    printCnt(shape_cnt);
    ASSERT((shape_cnt[LIVE_FOUR] == 1 && shape_cnt[BREAKTHROUGH_FOUR] == 1), "活四+冲四判定失败");
    printf("测试通过: 活四+冲四\n");
    return true;
}

bool testDoubleBreakthroughFour() {
    Board board;
    Piece pieces[BOARD_SIZE+1][BOARD_SIZE+1] = {0};
    pieces[5][2] = color;
    pieces[5][3] = color;
    pieces[5][4] = color;
    pieces[5][6] = opp_color; 
    pieces[2][5] = color;
    pieces[3][5] = color;
    pieces[4][5] = color;
    pieces[6][5] = opp_color;  
    pieces[5][5] = color; 
    initTestBoard(&board, pieces);

    int shape_cnt[CHESS_SHAPE_CNT] = {0};
    checkChessShape(&board, 5, 5, shape_cnt, (color==BLACK?PLAYER_BLACK:PLAYER_WHITE));
    printCnt(shape_cnt);
    ASSERT(shape_cnt[BREAKTHROUGH_FOUR] == 2, "普通双冲四判定失败");
    printf("测试通过: 普通双冲四\n");
    return true;
}

bool testDoubleBreakthroughFour2() {
    Board board;
    Piece pieces[BOARD_SIZE+1][BOARD_SIZE+1] = {0};
    pieces[5][2] = color;
    pieces[5][3] = color;
    pieces[5][4] = color;
    pieces[5][6] = color; 
    pieces[5][8] = color;
    pieces[5][9] = color;
    pieces[5][10] = color;
    initTestBoard(&board, pieces);

    int shape_cnt[CHESS_SHAPE_CNT] = {0};
    checkChessShape(&board, 5, 6, shape_cnt, (color==BLACK?PLAYER_BLACK:PLAYER_WHITE));
    printCnt(shape_cnt);
    ASSERT(shape_cnt[BREAKTHROUGH_FOUR] == 2, "双向二跳四判定失败");
    printf("测试通过: 双向二跳四\n");
    return true;
}

bool testSpecialBreakthroughFour() {
    Board board;
    Piece pieces[BOARD_SIZE+1][BOARD_SIZE+1] = {0};
    pieces[5][1] = color;
    pieces[5][2] = color;
    pieces[5][3] = color;
    pieces[5][4] = color; 
    pieces[5][6] = color;
    pieces[6][5] = color;
    pieces[7][5] = color;
    pieces[8][5] = color;
    pieces[9][5] = color;
    initTestBoard(&board, pieces);

    int shape_cnt[CHESS_SHAPE_CNT] = {0};
    checkChessShape(&board, 9, 5, shape_cnt, (color==BLACK?PLAYER_BLACK:PLAYER_WHITE));
    printCnt(shape_cnt);
    ASSERT(shape_cnt[LIVE_FOUR] == 1, "特殊活四（单边长连，成五非禁手）");
    printf("测试通过: 特殊活四（单边长连，成五非禁手）\n");
    return true;
}

bool testSingleLiveThree() {
    Board board;
    Piece pieces[BOARD_SIZE+1][BOARD_SIZE+1] = {0};
    pieces[5][4] = color;
    pieces[5][5] = color;
    pieces[5][6] = color;

    initTestBoard(&board, pieces);

    int shape_cnt[CHESS_SHAPE_CNT] = {0};
    checkChessShape(&board, 5, 5, shape_cnt, (color==BLACK?PLAYER_BLACK:PLAYER_WHITE));
    printCnt(shape_cnt);
    ASSERT(shape_cnt[LIVE_THREE] == 1 && shape_cnt[LIVE_FOUR] == 0 && shape_cnt[BREAKTHROUGH_FOUR] == 0, "单个活三判定失败");
    printf("测试通过: 单个活三\n");
    return true;
}


bool testSingleLiveFour() {
    Board board;
    Piece pieces[BOARD_SIZE+1][BOARD_SIZE+1] = {0};
    pieces[5][4] = color;
    pieces[5][5] = color;
    pieces[5][6] = color;
    pieces[5][7] = color;

    initTestBoard(&board, pieces);

    int shape_cnt[CHESS_SHAPE_CNT] = {0};
    memset(shape_cnt, 0, sizeof(shape_cnt));
    checkChessShape(&board, 5, 5, shape_cnt, (color==BLACK?PLAYER_BLACK:PLAYER_WHITE));
    printCnt(shape_cnt);
    ASSERT(shape_cnt[LIVE_FOUR] == 1 && shape_cnt[LIVE_THREE] == 0 && shape_cnt[BREAKTHROUGH_FOUR] == 0, "单个活四判定失败");
    printf("测试通过: 单个活四\n");
    return true;
}

bool testSingleBreakthroughFour() {
    Board board;
    Piece pieces[BOARD_SIZE+1][BOARD_SIZE+1] = {0};
    pieces[5][4] = color;
    pieces[5][5] = color;
    pieces[5][6] = color;
    pieces[5][7] = color;
    pieces[5][8] = opp_color; 

    initTestBoard(&board, pieces);

    int shape_cnt[CHESS_SHAPE_CNT] = {0};
    memset(shape_cnt, 0, sizeof(shape_cnt));
    checkChessShape(&board, 5, 5, shape_cnt, (color==BLACK?PLAYER_BLACK:PLAYER_WHITE));
    printCnt(shape_cnt);
    ASSERT(shape_cnt[BREAKTHROUGH_FOUR] == 1 && shape_cnt[LIVE_THREE] == 0 && shape_cnt[LIVE_FOUR] == 0, "单个冲四判定失败");
    printf("测试通过: 单个冲四\n");
    return true;
}

//参考 五子棋禁手规则详解.pdf 第12页
bool testFinal1() {
    Board board;
    Piece pieces[BOARD_SIZE+1][BOARD_SIZE+1] = {0};
    pieces[12][6] = color;
    pieces[11][7] = color;
    pieces[10][8] = color;
    pieces[8][4] = color; 
    pieces[8][5] = color;
    pieces[8][6] = color;
    pieces[8][7] = color;
    pieces[7][5] = color;
    pieces[6][4] = color;
    pieces[5][3] = color;
    pieces[4][2] = opp_color;
    initTestBoard(&board, pieces);

    int shape_cnt[CHESS_SHAPE_CNT] = {0};
    checkChessShape(&board, 8, 6, shape_cnt, (color==BLACK?PLAYER_BLACK:PLAYER_WHITE));
    printCnt(shape_cnt);
    ASSERT(shape_cnt[LIVE_FOUR]==1 && shape_cnt[BREAKTHROUGH_FOUR]==0, "复杂禁手1测试失败");
    printf("测试通过: 复杂禁手1\n");
    return true;
}

//参考 五子棋禁手规则详解.pdf 第13页
bool testFinal2() {
    Board board;
    Piece pieces[BOARD_SIZE+1][BOARD_SIZE+1] = {0};
    pieces[6][4] = color;
    pieces[5][4] = color;
    pieces[4][5] = color;
    pieces[4][6] = color; 
    pieces[6][7] = color;
    pieces[5][7] = color;
    pieces[7][5] = color;
    pieces[7][6] = color;
    pieces[6][5] = color;
    initTestBoard(&board, pieces);

    int shape_cnt[CHESS_SHAPE_CNT] = {0};
    checkChessShape(&board, 6, 5, shape_cnt, (color==BLACK?PLAYER_BLACK:PLAYER_WHITE));
    printCnt(shape_cnt);
    ASSERT(shape_cnt[LIVE_THREE]==1, "复杂禁手2测试失败");
    printf("测试通过: 复杂禁手2\n");
    return true;
}

bool diff4() {
    Board board;
    Piece pieces[BOARD_SIZE+1][BOARD_SIZE+1] = {0};
    pieces[6][4] = color;
    pieces[6][6] = color;
    pieces[6][7] = color;
    pieces[6][8] = color; 
    pieces[6][9] = color;
    initTestBoard(&board, pieces);

    int shape_cnt[CHESS_SHAPE_CNT] = {0};
    checkChessShape(&board, 6, 9, shape_cnt, (color==BLACK?PLAYER_BLACK:PLAYER_WHITE));
    printCnt(shape_cnt);
    ASSERT(shape_cnt[BREAKTHROUGH_FOUR]==1, "单侧长连禁手冲四判断测试失败");
    printf("测试通过: 单侧长连禁手冲四判断\n");
    return true;
}

bool spLive3Check() {
    Board board;
    Piece pieces[BOARD_SIZE+1][BOARD_SIZE+1] = {0};
    pieces[6][4] = color;
    pieces[6][7] = color;
    pieces[6][8] = color;
    pieces[6][9] = color; 
    pieces[6][12] = color;
    initTestBoard(&board, pieces);

    int shape_cnt[CHESS_SHAPE_CNT] = {0};
    checkChessShape(&board, 6, 7, shape_cnt, (color==BLACK?PLAYER_BLACK:PLAYER_WHITE));
    printCnt(shape_cnt);
    ASSERT(shape_cnt[LIVE_THREE]==0, "假活三测试失败");
    printf("测试通过: 假活三\n");
    return true;
}

bool testDoubleLiveTwo() {
    Board board;
    Piece pieces[BOARD_SIZE+1][BOARD_SIZE+1] = {0};
    pieces[8][7] = color;
    pieces[8][8] = color;
    pieces[10][8] = color;
    initTestBoard(&board, pieces);

    int shape_cnt[CHESS_SHAPE_CNT] = {0};
    checkChessShape(&board, 8, 8, shape_cnt, (color==BLACK?PLAYER_BLACK:PLAYER_WHITE));
    printCnt(shape_cnt);
    ASSERT((shape_cnt[LIVE_TWO] == 2), "双活二（连+跳）判定失败");
    printf("测试通过: 双活二（连+跳）\n");
    return true;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    printf("===== 开始测试规则模块 =====\n");
    bool all_pass = true;

    all_pass &= testFiveInRowVertical();
    all_pass &= testFiveInRowDiagonal1();
    all_pass &= testFiveInRowDiagonal2();

    all_pass &= testLongChainVertical();
    all_pass &= testLongChainDiagonal();

    all_pass &= testDoubleLiveThree1();
    all_pass &= testDoubleLiveThree2();

    all_pass &= testDoubleLiveFour();
    all_pass &= testLiveFourPlusBreakthrough();
    all_pass &= testDoubleBreakthroughFour();
    all_pass &= testDoubleBreakthroughFour2();
    all_pass &= testSpecialBreakthroughFour();

    all_pass &= testSingleBreakthroughFour();
    all_pass &= testSingleLiveFour();
    all_pass &= testSingleLiveThree();

    all_pass &= testFinal1();
    all_pass &= testFinal2();

    all_pass &= diff4();
    all_pass &= spLive3Check();

    all_pass &= testDoubleLiveTwo();

    printf("\n===== 测试结束 =====\n");
    if (all_pass) {
        printf("所有测试通过！\n");
    } else {
        printf("部分测试失败，请检查代码。\n");
    }
    system("pause");
    return 0;
}