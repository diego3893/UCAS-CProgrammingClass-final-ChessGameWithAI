#ifndef AI_H
#define AI_H

#include "../board/board.h"
#include "../rule/rule.h"
#include <stdbool.h>
#include "evaluate.h"
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

// 搜索深度，可根据需要调整
#define SEARCH_DEPTH 4
#define MAX_TIME 15000

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
 * @return int 评估分数
 */
int alphaBetaSearch(Board* board, int depth, int alpha, int beta, bool is_max_player, Piece ai_color);

/**
 * @brief 初始化随机种子
 * 
 */
void initAI();

int generatePossibleMoves(const Board* board, PossibleMoves possible_moves[], Piece color, bool is_max_player);



#endif