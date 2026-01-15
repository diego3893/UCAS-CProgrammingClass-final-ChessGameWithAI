/**
 * @file ai.h 
 * @author diego3893 (diegozcx@foxmail.com)
 * @brief AI搜索算法相关头文件
 * @version 1.0
 * @date 2026-01-08
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef AI_H
#define AI_H

#include "../board/board.h"
#include "../rule/rule.h"
#include "evaluate.h"
#include "tt.h"
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>

#define ST_SEARCH_DEPTH 2 // 起始搜索深度
#define TG_SEARCH_DEPTH 7 // 终止搜索深度
#define MAX_TIME 15000 // 最大搜索时间

/**
 * @brief AI决策函数，返回最佳落子位置
 * 
 * @param board 棋盘
 * @param ai_color AI棋子颜色
 * @param row 输出参数，最佳落子行
 * @param col 输出参数，最佳落子列
 */
double aiMakeDecision(const Board* board, Piece ai_color, int* row, int* col);

/**
 * @brief 极大极小搜索算法带alpha-beta剪枝
 * 
 * @param board 棋盘
 * @param depth 当前搜索深度
 * @param alpha alpha值
 * @param beta beta值
 * @param is_max_player 是否为最大化玩家
 * @param ai_color AI棋子颜色
 * @param current_hash 目前棋盘的hash值
 * @return int 评估分数
 */
int alphaBetaSearch(Board* board, int depth, int alpha, int beta, bool is_max_player, Piece ai_color, ULL current_hash);

/**
 * @brief 初始化随机种子，初始化置换表，获取搜索起始时间
 * 
 */
void initAI();

/**
 * @brief 邻域搜索生成函数
 * 
 * @param board 棋盘
 * @param possible_moves 搜索目标的数组
 * @param ai_color AI颜色
 * @param is_max_player 当前搜索层数是否是max层
 * @return int 邻域搜索目标的个数
 */
int generatePossibleMoves(const Board* board, PossibleMoves possible_moves[], Piece ai_color, bool is_max_player);

/**
 * @brief 迭代加深搜索
 * 
 * @param board 棋盘
 * @param ai_color AI颜色
 * @param start_depth 起始深度
 * @param target_depth 终止深度
 * @param best_r 最优行坐标
 * @param best_c 最优列坐标
 * @return int 最优分数
 */
int iterativeDeepeningSearch(Board* board, Piece ai_color, int start_depth, int target_depth, int* best_r, int* best_c);

/**
 * @brief 走法综合排序
 * 
 * @param moves 可能的走法
 * @param count 走法的总数
 * @param depth 当前深度
 * @param tt_r 置换表记录的最好走法行号
 * @param tt_c 置换表记录的最好走法列号
 */
void sortMovesHeuristic(PossibleMoves moves[], int count, int depth, int tt_r, int tt_c);

#endif