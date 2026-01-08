/**
 * @file evaluate.h
 * @author diego3893 (diegozcx@foxmail.com)
 * @brief 评估函数相关头文件
 * @version 1.0
 * @date 2025-12-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef EVALUATE_H
#define EVALUATE_H

#include "../board/board.h"
#include "../rule/rule.h"
#include <string.h>
#include <math.h>

/**
 * @brief 棋局评估函数
 * 
 * @param board 棋盘
 * @param ai_player AI玩家
 * @return int 评估值
 */
int evaluateFullBoard(const Board* board, Player ai_player);

/**
 * @brief 对特定位置的评估函数
 * 
 * @param board 棋盘
 * @param row 行
 * @param col 列
 * @param ai_color AI颜色
 * @param current_color 目前落子的颜色
 * @return int 该点位评估值
 */
int evaluatePostion(const Board* board, int row, int col, Piece ai_color, Piece current_color);

/**
 * @brief 用于qsort的结构体降序比较
 * 
 * @param a 比较对象a
 * @param b 比较对象b
 * @return int 比较结果，0 a=b，-1 a>b，1 a<b
 */
int cmp(const void* a, const void* b);

#endif