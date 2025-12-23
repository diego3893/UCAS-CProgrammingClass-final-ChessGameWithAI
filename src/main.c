#include"board/board.h"
#include"display/display.h"
#include"rule/rule.h"

Board board;
Piece piece_color = WHITE;
Player current_player = PLAYER_WHITE;
GameStatus game_status = PLAYING;
char restart = 'n';
char input[4];
int row, col;
int trans_flag, drop_flag;

int main(){
    // 设置编码为UTF-8
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    showWelcomeMsg();
    do{
        //初始化
        boardInit(&board);
        showBoard(&board);
        restart = 'n';
        piece_color = WHITE;
        current_player = PLAYER_WHITE;
        game_status = PLAYING;
        //游戏开始
        while(game_status == PLAYING){
            current_player = (current_player==PLAYER_WHITE) ? PLAYER_BLACK : PLAYER_WHITE; //轮流落子
            piece_color = (piece_color==WHITE) ? BLACK : WHITE;
            input[0] = '\0'; //重置输入
            trans_flag = 0, drop_flag = 0;
            do{
                if(strlen(input) != 0){
                    if(!trans_flag){
                        showBoard(&board);
                        printf("输入有误，请重新输入！\n");
                    }else{
                        if(drop_flag == -1){
                            showBoard(&board);
                            printf("落子坐标超出范围，请重新输入！\n");
                        }
                        if(drop_flag == 0){
                            showBoard(&board);
                            printf("落子坐标已有棋子，请重新输入！\n");
                        }
                    }
                }
                showInputPrompt(current_player);
                getInput(input, INPUT_MAX_LEN);
                trans_flag = transInput2Coord(input, &row, &col); //坐标转换
                if(!trans_flag){
                    continue;
                }
                //转换成功则落子
                drop_flag = dropPiece(&board, row, col, piece_color);
            }while(drop_flag != 1); //直到落子成功才退出输入环节
            showBoard(&board);
            game_status = judgeStatus(&board, row, col, current_player);
        }
        showGameOver(game_status);
        printf("要重新开始游戏吗？(y/n)：");
        scanf("%c", &restart);
        getchar();
    }while(restart == 'y');
    return 0;
}