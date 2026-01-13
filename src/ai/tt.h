#ifndef TT_H
#define TT_H

#include "../board/board.h"
#include <stdlib.h>
#include <stdbool.h>

#define TT_PRIME 2097149 
#define TT_SIZE TT_PRIME 
#define ULL unsigned long long

typedef struct {
    ULL hash;
    int score;
    int depth;
    int best_row; // 新增：记录该局面的最佳落子行
    int best_col; // 新增：记录该局面的最佳落子列
    enum { EXACT, LOWER, UPPER } type;
} TT_Entry;

void initZobrist();
void clearTT();
ULL hashBoard(const Board* board);
void updateHash(ULL* hash, int row, int col, int old_color, int new_color);

// 更新：增加最佳落子坐标参数
void storeTT(ULL hash, int score, int depth, int type, int r, int c);
// 更新：增加最佳落子坐标输出参数
bool retrieveTT(ULL hash, int* score, int* depth, int* type, int* r, int* c);

#endif