#include "rule.h"
#include "../display/display.h"

GameStatus judgeStatus(const Board* board, int row, int col, Player current_player){
    int chess_shape_cnt[CHESS_SHAPE_CNT] = {0};
    checkChessShape(board, row, col, chess_shape_cnt, current_player);
    if(chess_shape_cnt[FIVE_IN_ROW] > 0){
        if(current_player == PLAYER_WHITE){
            return WHITE_WIN;
        }else{
            return BLACK_WIN;
        }
    }
    if(current_player == PLAYER_BLACK){
        if(isForbiddenMove(chess_shape_cnt)){
            return FORBIDDEN_MOVE;
        }
    }
    if(boardIsFull(board)){
        return DRAW;
    }
    return PLAYING;
}

void checkChessShape(const Board* board, int row, int col, int chess_shape_cnt[], Player current_player){
    for(int i=0; i<CHESS_SHAPE_CNT; ++i){
        chess_shape_cnt[i] = 0;
    }
    // showBoard(board);
    Piece color = (current_player==PLAYER_BLACK)?BLACK:WHITE;
    Pair dirs[] = {DELTA_RIGHT, DELTA_DOWN, DELTA_UPRIGHT, DELTA_DOWNRIGHT};
    for(int i=0; i<4; ++i){
        int dx = dirs[i].x;
        int dy = dirs[i].y;
        int same = 1; 
        int blank_ends_become_5 = 0, blank_ends_become_4 = 0, blank_ends_become_3 = 0;
        int x = row+dx;
        int y = col+dy;
        while(x>=1 && x<=BOARD_SIZE && y>=1 && y<=BOARD_SIZE && getPiece(board, x, y)==color){
            same++;
            x += dx;
            y += dy;
        }
        if(x>=1 && x<=BOARD_SIZE && y>=1 && y<=BOARD_SIZE && getPiece(board, x, y)==BLANK){
            if(checkPieceInRowWithDir(board, x, y, 5, dirs[i], current_player)){
                blank_ends_become_5++;
            }
            if(checkPieceInRowWithDir(board, x, y, 3, dirs[i], current_player)){
                blank_ends_become_3++;
            }
            if(checkPieceInRowWithDir(board, x, y, 4, dirs[i], current_player)){
                if((color==BLACK&&!isForbiddenPosition(board, x, y)) || color==WHITE){
                    blank_ends_become_4++;
                }
            }
        }
        x = row-dx;
        y = col-dy;
        while(x>=1 && x<=BOARD_SIZE && y>=1 && y<=BOARD_SIZE && getPiece(board, x, y)==color){
            same++;
            x -= dx;
            y -= dy;
        }
        if(x>=1 && x<=BOARD_SIZE && y>=1 && y<=BOARD_SIZE && getPiece(board, x, y)==BLANK){
            if(checkPieceInRowWithDir(board, x, y, 5, dirs[i], current_player)){
                blank_ends_become_5++;
            }
            if(checkPieceInRowWithDir(board, x, y, 3, dirs[i], current_player)){
                blank_ends_become_3++;
            }
            if(checkPieceInRowWithDir(board, x, y, 4, dirs[i], current_player)){
                if((color==BLACK&&!isForbiddenPosition(board, x, y)) || color==WHITE){
                    blank_ends_become_4++;
                }
            }
        }
        if(same==4 && blank_ends_become_5==2){
            chess_shape_cnt[LIVE_FOUR]++;
        }
        chess_shape_cnt[BREAKTHROUGH_FOUR] += blank_ends_become_5;
        if((color==BLACK&&same==5) || (color==WHITE&&same>=5)){
            chess_shape_cnt[FIVE_IN_ROW]++;
        }
        if(same >= 6){
            chess_shape_cnt[LONG_CHAIN]++;
        }
        chess_shape_cnt[LIVE_THREE] += blank_ends_become_4;
        if(same==3 && blank_ends_become_4==2){
            chess_shape_cnt[LIVE_THREE]--;
        }
        chess_shape_cnt[LIVE_TWO] += blank_ends_become_3;
        if(same==2 && blank_ends_become_3==2){
            chess_shape_cnt[LIVE_TWO]--;
        }
    }
    chess_shape_cnt[BREAKTHROUGH_FOUR] -= 2*chess_shape_cnt[LIVE_FOUR];
    return;
}

bool isForbiddenMove(const int chess_shape_cnt[]){
    if(chess_shape_cnt[FIVE_IN_ROW] > 0){
        return false;
    }
    if(chess_shape_cnt[LONG_CHAIN] > 0){
        return true;
    }
    if(chess_shape_cnt[LIVE_THREE] >= 2){
        return true;
    }
    if(chess_shape_cnt[LIVE_FOUR]+chess_shape_cnt[BREAKTHROUGH_FOUR] >= 2){
        return true;
    }
    return false;
}

bool checkPieceInRowWithDir(const Board* board, int row, int col, int num, Pair dir, Player current_player){
    Piece color = (current_player==PLAYER_BLACK)?BLACK:WHITE;
    int dx = dir.x;
    int dy = dir.y;
    if(num == 4){ //能否形成活四
        Board* mut_board = (Board*)board;
        mut_board->pieceColor[row][col] = color;
        mut_board->pieceTotal++;
        int same = 1; 
        int blank_ends = 0;
        int x = row+dx;
        int y = col+dy;
        while(x>=1 && x<=BOARD_SIZE && y>=1 && y<=BOARD_SIZE && getPiece(board, x, y)==color){
            same++;
            x += dx;
            y += dy;
        }
        if(x>=1 && x<=BOARD_SIZE && y>=1 && y<=BOARD_SIZE && getPiece(board, x, y)==BLANK){
            if(checkPieceInRowWithDir(mut_board, x, y, 5, dir, current_player)){
                blank_ends++;
            }
        }
        x = row-dx;
        y = col-dy;
        while(x>=1 && x<=BOARD_SIZE && y>=1 && y<=BOARD_SIZE && getPiece(board, x, y)==color){
            same++;
            x -= dx;
            y -= dy;
        }
        if(x>=1 && x<=BOARD_SIZE && y>=1 && y<=BOARD_SIZE && getPiece(board, x, y)==BLANK){
            if(checkPieceInRowWithDir(mut_board, x, y, 5, dir, current_player)){
                blank_ends++;
            }
        }
        mut_board->pieceColor[row][col] = BLANK;
        mut_board->pieceTotal--;
        if(same==4 && blank_ends==2){
            return true;
        }
        return false;
    }else if(num == 5){ //能否五连
        int length = 1;
        int x = row+dx;
        int y = col+dy;
        while(x>=1 && x<=BOARD_SIZE && y>= 1&& y<=BOARD_SIZE && getPiece(board, x, y)==color){
            length++;
            x += dx;
            y += dy;
        }
        x = row-dx;
        y = col-dy;
        while(x>=1 && x<=BOARD_SIZE && y>=1 && y<=BOARD_SIZE && getPiece(board, x, y)==color){
            length++;
            x -= dx;
            y -= dy;
        }
        if(color == BLACK){
            if(length == 5){
                return true;
            }
        }else if(color == WHITE){
            if(length >= 5){
                return true;
            }
        }
        return false;
    }else if(num == 3){
        Board* mut_board = (Board*)board;
        mut_board->pieceColor[row][col] = color;
        mut_board->pieceTotal++;
        int same = 1; 
        int blank_ends = 0;
        int x = row+dx;
        int y = col+dy;
        while(x>=1 && x<=BOARD_SIZE && y>=1 && y<=BOARD_SIZE && getPiece(board, x, y)==color){
            same++;
            x += dx;
            y += dy;
        }
        if(x>=1 && x<=BOARD_SIZE && y>=1 && y<=BOARD_SIZE && getPiece(board, x, y)==BLANK){
            if(checkPieceInRowWithDir(mut_board, x, y, 4, dir, current_player)){
                blank_ends++;
            }
        }
        x = row-dx;
        y = col-dy;
        while(x>=1 && x<=BOARD_SIZE && y>=1 && y<=BOARD_SIZE && getPiece(board, x, y)==color){
            same++;
            x -= dx;
            y -= dy;
        }
        if(x>=1 && x<=BOARD_SIZE && y>=1 && y<=BOARD_SIZE && getPiece(board, x, y)==BLANK){
            if(checkPieceInRowWithDir(mut_board, x, y, 4, dir, current_player)){
                blank_ends++;
            }
        }
        mut_board->pieceColor[row][col] = BLANK;
        mut_board->pieceTotal--;
        if(same==3 && blank_ends==2){
            return true;
        }
        return false;
    }
    return false;
}

bool isForbiddenPosition(const Board* board, int row, int col){
    if(row<1 || row>BOARD_SIZE || col<1 || col>BOARD_SIZE){
        return false;
    }
    if(getPiece(board, row, col) != BLANK){
        return false;
    }
    Board* mut_board = (Board*)board;
    mut_board->pieceColor[row][col] = BLACK;
    mut_board->pieceTotal++;
    int chess_shape_cnt[CHESS_SHAPE_CNT] = {0};
    checkChessShape(mut_board, row, col, chess_shape_cnt, PLAYER_BLACK); //递归判断
    mut_board->pieceColor[row][col] = BLANK;
    mut_board->pieceTotal--;
    return isForbiddenMove(chess_shape_cnt);
}