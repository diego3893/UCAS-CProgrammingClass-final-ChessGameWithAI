/**
 * @file tt.h
 * @author diego3893 (diegozcx@foxmail.com)
 * @brief 置换表相关头文件
 * @version 1.0
 * @date 2025-12-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef TT_H
#define TT_H

#include "../board/board.h"
#include <stdlib.h>
#include <stdbool.h>

#define TT_PRIME 4194301
#define TT_SIZE TT_PRIME 
#define ULL unsigned long long

/**
 * @brief 置换表结构体
 * 
 */
typedef struct{
    ULL hash;
    int score;
    int depth;
    int best_row;
    int best_col;
    enum {EXACT, LOWER, UPPER} type;
}TT_Entry;

/**
 * @brief 初始化哈希计算的随机数
 * 
 */
void initZobrist();

/**
 * @brief 清空置换表
 * 
 */
void clearTT();

/**
 * @brief 计算整个棋盘哈希
 * 
 * @param board 棋盘
 * @return ULL 哈希值
 */
ULL hashBoard(const Board* board);

/**
 * @brief 增量更新哈希
 * 
 * @param hash 哈希值
 * @param row 行号
 * @param col 列号
 * @param old_color 更改前的颜色
 * @param new_color 更改后的颜色
 */
void updateHash(ULL* hash, int row, int col, int old_color, int new_color);

/**
 * @brief 保存哈希
 * 
 * @param hash 哈希值
 * @param score 分数
 * @param depth 深度
 * @param type 类型
 * @param r 行
 * @param c 列
 */
void storeTT(ULL hash, int score, int depth, int type, int r, int c);

/**
 * @brief 哈希查找
 * 
 * @param hash 哈希
 * @param score 分数
 * @param depth 深度
 * @param type 类型
 * @param r 行
 * @param c 列
 * @return true 查找成功
 * @return false 查找失败
 */
bool retrieveTT(ULL hash, int* score, int* depth, int* type, int* r, int* c);

#endif