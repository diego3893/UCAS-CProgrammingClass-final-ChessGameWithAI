/**
 * @file rule.h
 * @author diego3893 (diegozcx@foxmail.com)
 * @brief 规则相关头文件
 * @version 1.0
 * @date 2025-12-11
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef RULE_H
#define RULE_H

#include "../board/board.h"
#include "../display/display.h"

/**
 * @brief 控制坐标移动
 * 
 */
typedef struct{
    int x;
    int y;
}Pair;

/**
 * @brief 当前棋子构成的棋型
 * 
 */
typedef enum{
    LIVE_TWO,
    LIVE_THREE,
    BREAKTHROUGH_FOUR,
    LIVE_FOUR,
    FIVE_IN_ROW,
    LONG_CHAIN,
    CHESS_SHAPE_CNT
}ChessShape;

/**
 * @brief 坐标移动的方向，通过正负号完成8个方向的移动
 * 
 */
#define DELTA_RIGHT ((Pair){1, 0})   
#define DELTA_DOWN ((Pair){0, -1})  
#define DELTA_UPRIGHT ((Pair){1, 1}) 
#define DELTA_DOWNRIGHT ((Pair){1, -1})

/**
 * @brief 判断游戏状态
 * 
 * @param board 棋盘
 * @param row 落子行坐标
 * @param col 落子列坐标
 * @param current_player 落子玩家 
 * @return GameStatus 继续游戏，黑/白胜，禁手，平局
 */
GameStatus judgeStatus(const Board* board, int row, int col, Player current_player);

/**
 * @brief 统计当前棋子参与构成的棋型
 * 
 * @param board 棋盘
 * @param row 棋子行坐标
 * @param col 棋子列坐标
 * @param chess_shape_cnt 存储各个棋型的统计值
 * @param current_player 当前玩家
 */
void checkChessShape(const Board* board, int row, int col, int chess_shape_cnt[], Player current_player);

/**
 * @brief 统计活三个数（包括跳活三）
 * 
 * @param board 棋盘
 * @param row 棋子行坐标
 * @param col 棋子列坐标
 * @param current_player 当前玩家
 * @return int 活三个数
 */
int checkLiveThree(const Board* board, int row, int col, Player current_player);

/**
 * @brief 统计活四个数
 * 
 * @param board 棋盘
 * @param row 棋子行坐标
 * @param col 棋子列坐标
 * @param current_player 当前玩家
 * @return int 活四个数
 */
int checkLiveFour(const Board* board, int row, int col, Player current_player);

/**
 * @brief 统计冲四个数
 * 
 * @param board 棋盘
 * @param row 棋子行坐标
 * @param col 棋子列坐标
 * @param current_player 当前玩家
 * @return int 冲四个数
 */
int checkBreakthroughFour(const Board* board, int row, int col, Player current_player);

/**
 * @brief 统计长连个数
 * 
 * @param board 棋盘
 * @param row 棋子行坐标
 * @param col 棋子列坐标
 * @param current_player 当前玩家
 * @return int 长连个数
 */
int checkLongChain(const Board* board, int row, int col, Player current_player);

/**
 * @brief 统计五连个数
 * 
 * @param board 棋盘
 * @param row 棋子行坐标
 * @param col 棋子列坐标
 * @param current_player 当前玩家
 * @return int 五连个数
 */
int checkFiveInRow(const Board* board, int row, int col, Player current_player);

/**
 * @brief 判断是否为禁手
 * 
 * @param chess_shape_cnt 统计的棋型数组
 * @return true 是
 * @return false 否
 */
bool isForbiddenMove(const int chess_shape_cnt[]);

/**
 * @brief 检查某个特定方向上连成的棋型，判断是否为活三、活四或五连
 * 
 * @param board 棋盘
 * @param row 行
 * @param col 列
 * @param num 相连棋子数
 * @param dir 方向
 * @param current_player 当前玩家
 * @return true 是
 * @return false 否
 */
bool checkPieceInRowWithDir(const Board* board, int row, int col, int num, Pair dir, Player current_player);

/**
 * @brief 模拟落子，判断空白位置是否为禁手点位
 * 
 * @param board 棋盘
 * @param row 目标行
 * @param col 目标列
 * @return true 该点位为禁手点位
 * @return false 该点位不是禁手点位
 */
bool isForbiddenPosition(const Board* board, int row, int col);

/**
 * @brief 统计活二个数
 * 
 * @param board 棋盘
 * @param row 棋子行坐标
 * @param col 棋子列坐标
 * @param current_player 当前玩家
 * @return int 活二个数
 */
int checkLiveTwo(const Board* board, int row, int col, Player current_player);
#endif 