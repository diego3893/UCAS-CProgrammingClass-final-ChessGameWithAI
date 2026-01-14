#include "tt.h"

ULL zobrist_table[BOARD_SIZE+1][BOARD_SIZE+1][3]; 
TT_Entry transposition_table[TT_SIZE]; 

void initZobrist(){
    static int initialized = 0;
    if(initialized) return;
    for(int i=1; i<=BOARD_SIZE; ++i){
        for(int j=1; j<=BOARD_SIZE; ++j){
            for(int c=0; c<3; ++c){
                zobrist_table[i][j][c] = ((ULL)rand()<<32)|rand();
            }
        }
    }
    initialized = 1;
    return;
}

void clearTT(){
    for(int i=0; i<TT_SIZE; ++i){
        transposition_table[i].hash = 0;
    }
    return;
}

ULL hashBoard(const Board* board){
    ULL hash = 0;
    for(int i=1; i<=BOARD_SIZE; ++i){
        for(int j=1; j<=BOARD_SIZE; ++j){
            hash ^= zobrist_table[i][j][board->pieceColor[i][j]];
        }
    }
    return hash;
}

void updateHash(ULL* hash, int row, int col, int old_color, int new_color){
    *hash ^= zobrist_table[row][col][old_color];
    *hash ^= zobrist_table[row][col][new_color];
    return;
}

void storeTT(ULL hash, int score, int depth, int type, int r, int c){
    int idx = hash%TT_PRIME;
    if (transposition_table[idx].hash==0 || transposition_table[idx].depth<=depth) {
        transposition_table[idx].hash = hash;
        transposition_table[idx].score = score;
        transposition_table[idx].depth = depth;
        transposition_table[idx].type = type;
        transposition_table[idx].best_row = r;
        transposition_table[idx].best_col = c;
    }
    return;
}

bool retrieveTT(ULL hash, int* score, int* depth, int* type, int* r, int* c){
    int idx = hash % TT_PRIME;
    if(transposition_table[idx].hash != hash){
        return false;
    }
    *score = transposition_table[idx].score;
    *depth = transposition_table[idx].depth;
    *type = transposition_table[idx].type;
    *r = transposition_table[idx].best_row;
    *c = transposition_table[idx].best_col;
    return true;
}