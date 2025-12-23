/**
 * @file display.h
 * @author diego3893 (diegozcx@foxmail.com)
 * @brief 棋盘显示相关头文件，使用UTF-8编码
 * @version 1.0
 * @date 2025-12-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef DISPLAY_H
#define DISPLAY_H

#include<windows.h>
#include<stdio.h>
#include"../board/board.h"

#define CROSS "\xE2\x94\xBC"
#define WHITE_PIECE "\xE2\x97\x8B"
#define BLACK_PIECE "\xE2\x97\x8F"
#define WHITE_PIECE_LAST "\xE2\x96\xB3"
#define BLACK_PIECE_LAST "\xE2\x96\xB2"
#define LEFT_TOP_CORNER "\xE2\x94\x8F"
#define RIGHT_TOP_CORNER "\xE2\x94\x93"
#define LEFT_BOTTOM_CORNER "\xE2\x94\x97"
#define RIGHT_BOTTOM_CORNER "\xE2\x94\x9B"
#define TOP_EDGE "\xE2\x94\xB3"
#define LEFT_EDGE "\xE2\x94\xA3"
#define RIGHT_EDGE "\xE2\x94\xAB"
#define BOTTOM_EDGE "\xE2\x94\xBB"

/**
 * @brief 当前玩家 黑/白
 * 
 */
typedef enum{
    PLAYER_BLACK,
    PLAYER_WHITE
}Player;

/**
 * @brief 游戏状态：黑胜，白胜，黑禁手，和棋，正在游戏
 * 
 */
typedef enum{
    BLACK_WIN,
    WHITE_WIN,
    FORBIDDEN_MOVE,
    DRAW,
    PLAYING
}GameStatus;

/**
 * @brief 显示棋盘
 * 
 * @param board 棋盘
 */
void showBoard(const Board* board);

/**
 * @brief 用户输入的提示语
 * 
 * @param current_player 当前玩家
 */
void showInputPrompt(Player current_player);

/**
 * @brief 开始游戏时的提示语
 * 
 */
void showWelcomeMsg();

/**
 * @brief 游戏结束时的提示语
 * 
 * @param game_status 游戏结果
 */
void showGameOver(GameStatus game_status);

/**
 * @brief 读取用户输入的坐标
 * 
 * @param s 存储输入的字符串
 * @param max_len 会被存储的最大字符个数
 */
void getInput(char s[], int max_len);

#endif 