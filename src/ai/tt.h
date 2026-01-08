/**
 * @file tt.h
 * @author diego3893 (diegozcx@foxmail.com)
 * @brief 置换表相关头文件
 * @version 1.0
 * @date 2026-01-08
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef TT_H
#define TT_H

#include "../board/board.h"
#include <stdlib.h>

#define TT_PRIME 2097149 // 2^21左右的大质数，取模
#define TT_SIZE TT_PRIME // 置换表大小
#define ULL unsigned long long

/**
 * @brief 置换表结构体，保存棋盘数据
 * 
 */
typedef struct {
    ULL hash;
    int score;
    int depth;
    enum { EXACT, LOWER, UPPER } type;
} TT_Entry;

/**
 * @brief 初始化Zobrist随机数
 * 
 */
void initZobrist();

/**
 * @brief 清空置换表哈希值
 * 
 */
void clearTT();

/**
 * @brief 计算整个棋盘的哈希值
 * 
 * @param board 棋盘
 * @return ULL 棋盘哈希值
 */
ULL hashBoard(const Board* board);

/**
 * @brief 异或更新哈希值
 * 
 * @param hash 哈希值
 * @param row 行坐标
 * @param col 列坐标
 * @param old_color 更改前该位置的棋子颜色
 * @param new_color 更改后该位置的棋子颜色
 */
void updateHash(ULL* hash, int row, int col, int old_color, int new_color);

/**
 * @brief 保存置换表
 * 
 * @param hash 哈希值
 * @param score 分数
 * @param depth 深度
 * @param type 保存类型
 */
void storeTT(ULL hash, int score, int depth, int type);

/**
 * @brief 读取置换表，查询数据
 * 
 * @param hash 哈希值
 * @param score 分数
 * @param depth 深度
 * @param type 保存类型
 * @return true 查询成功
 * @return false 查询失败
 */
bool retrieveTT(ULL hash, int* score, int* depth, int* type);

#endif
