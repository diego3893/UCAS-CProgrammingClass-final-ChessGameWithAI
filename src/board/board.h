/**
 * @file board.h
 * @author diego3893 (diegozcx@foxmail.com)
 * @brief 棋盘相关头文件
 * @version 1.0
 * @date 2025-12-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>
#include <string.h>

#define BOARD_SIZE 15 //棋盘大小
#define COL_FROM_CHAR(c) ((c)-'A'+1) //列坐标转换
#define ROW_FROM_NUM(n) ((n)-'0') //行坐标转换
#define INPUT_MAX_LEN 3 //输入最大限制
#define INPUT_MIN_LEN 2 //输入最大限制

#define NEIGHBORHOOD_SIZE 1 // 邻域标记范围

/**
 * @brief 棋子，枚举类型，标识当前棋子的状态是黑/白/空白
 * 
 */
typedef enum{
    BLANK,
    BLACK,
    WHITE
}Piece;

/**
 * @brief 棋盘，存储所有位置的棋子状态，存储最后落子的位置便于特殊显示，存储落子总数判断和棋
 * 
 */
typedef struct{
    Piece pieceColor[BOARD_SIZE+1][BOARD_SIZE+1];
    int possibleMove[BOARD_SIZE+1][BOARD_SIZE+1];
    int last_row;
    int last_col;
    int pieceTotal;
}Board;

typedef enum{
    P2P,
    P2AI_PLAYER_BLACK,
    P2AI_PLAYER_WHITE
}GameMode;

typedef struct{
    int row;
    int col;
    int score;
}PossibleMoves;

/**
 * @brief 棋盘初始化
 * 
 * @param board 棋盘
 */
void boardInit(Board* board);

/**
 * @brief 
 * 
 * @param board 棋盘
 * @param row 落子行
 * @param col 落子列
 * @param piece_color 落子颜色类型
 * @return int 1：落子成功，0：落子失败，位置被占用，-1：超出范围，落子不合法
 */
int dropPiece(Board* board, int row, int col, Piece piece_color);

/**
 * @brief 获取棋子的颜色（调用前需要判断坐标合法性）
 * 
 * @param board 棋盘
 * @param row 目标行
 * @param col 目标列
 * @return Piece 返回棋子颜色
 */
Piece getPiece(const Board* board, int row, int col);

/**
 * @brief 判断坐标点是否为空
 * 
 * @param board 棋盘
 * @param row 目标行
 * @param col 目标列
 * @return true 空
 * @return false 非空或者坐标不合法
 */
bool isEmpty(const Board* board, int row, int col);

/**
 * @brief 将字符坐标输入转换为有序整数对(row, col)，同时确保范围在(1,1)~(15,15)
 * 
 * @param s 输入字符串
 * @param row 行坐标
 * @param col 列坐标
 * @return true 转换成功
 * @return false 字符串不合法或者坐标不合法
 */
bool transInput2Coord(const char s[], int* row, int* col);

/**
 * @brief 判断棋盘是否下满，判断和棋
 * 
 * @param board 棋盘
 * @return true 已满
 * @return false 未满
 */
bool boardIsFull(const Board* board);

#endif