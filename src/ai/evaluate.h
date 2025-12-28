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

/**
 * @brief 棋局评估函数
 * 
 * @param board 棋盘
 * @param current_player 当前玩家
 * @return int 评估值
 */
int evaluatePosition(const Board* board, Player current_player);

#endif