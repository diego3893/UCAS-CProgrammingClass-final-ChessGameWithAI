#include"board/board.h"
#include"display/display.h"
#include"rule/rule.h"
#include"ai/ai.h"

Board board;
Piece piece_color = WHITE;
Player current_player = PLAYER_WHITE;
GameStatus game_status = PLAYING;
GameMode game_mode;
int mode_code;
char restart = 'n';
char input[4];
int row, col;
int trans_flag, drop_flag;

int main(){
    // 设置编码为UTF-8
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD whiteBgBlackText = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
    SetConsoleTextAttribute(hOut, whiteBgBlackText);

    do{
        system("cls");
        do{
            showWelcomeMsg();
            printf("请选择游戏模式（0为P2P，1为P2AI且玩家为黑子，2为P2AI且玩家为白子）：");
            scanf("%d", &mode_code);
            getchar();
            if(mode_code<0 || mode_code>2){
                printf("输入有误，请重新输入游戏模式\n");
                system("pause");
                system("cls");
            }
        }while(mode_code<0 || mode_code>2);
        game_mode = mode_code;

        //初始化
        if(game_mode == P2P){
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
                            if(drop_flag == -2){
                                showBoard(&board);
                                printf("黑子第一手应在天元H8，请重新输入！\n");
                            }
                        }
                    }
                    showInputPrompt(current_player);
                    getInput(input, INPUT_MAX_LEN);
                    trans_flag = transInput2Coord(input, &row, &col); //坐标转换
                    // printf("%d,%d\n", row, col);
                    // system("pause");
                    if(!trans_flag){
                        continue;
                    }
                    //转换成功则落子
                    //判断第一手是否在天元
                    if(board.pieceTotal == 0){
                        if(row!=8 || col!=8){
                            drop_flag = -2;
                        }else{
                            drop_flag = dropPiece(&board, row, col, piece_color);
                        }
                    }else{
                        drop_flag = dropPiece(&board, row, col, piece_color);
                    }
                }while(drop_flag != 1); //直到落子成功才退出输入环节
                showBoard(&board);
                game_status = judgeStatus(&board, row, col, current_player);
            }
        }
        if(game_mode == P2AI_PLAYER_BLACK){
            boardInit(&board);
            showBoard(&board);
            Piece ai_color = WHITE;
            double time_use_now = 0, time_use_total = 0, time_use_avr = 0;
            int ai_cnt = 0;
            restart = 'n';
            piece_color = WHITE;
            current_player = PLAYER_WHITE;
            game_status = PLAYING;
            //游戏开始
            while(game_status == PLAYING){
                current_player = (current_player==PLAYER_WHITE) ? PLAYER_BLACK : PLAYER_WHITE; //轮流落子
                piece_color = (piece_color==WHITE) ? BLACK : WHITE;
                if(piece_color != ai_color){
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
                                if(drop_flag == -2){
                                    showBoard(&board);
                                    printf("黑子第一手应在天元H8，请重新输入！\n");
                                }
                            }
                        }
                        showInputPrompt(current_player);
                        getInput(input, INPUT_MAX_LEN);
                        trans_flag = transInput2Coord(input, &row, &col); //坐标转换
                        // printf("%d,%d\n", row, col);
                        // system("pause");
                        if(!trans_flag){
                            continue;
                        }
                        //转换成功则落子
                        //判断第一手是否在天元
                        if(board.pieceTotal == 0){
                            if(row!=8 || col!=8){
                                drop_flag = -2;
                            }else{
                                drop_flag = dropPiece(&board, row, col, piece_color);
                            }
                        }else{
                            drop_flag = dropPiece(&board, row, col, piece_color);
                        }
                    }while(drop_flag != 1); //直到落子成功才退出输入环节
                }else{
                    printf("AI思考中……\n");
                    time_use_now = aiMakeDecision(&board, ai_color, &row, &col);
                    time_use_total += time_use_now;
                    drop_flag = dropPiece(&board, row, col, piece_color);
                    ai_cnt++;
                    time_use_avr = time_use_total/(double)ai_cnt;
                    printf("本次落子位置：%c%d\n", (char)(col+'A'-1), row);
                    printf("本次思考用时：%.3fs，平均用时：%.3fs\n", time_use_now/1000, time_use_avr/1000);
                    system("pause");
                }
                showBoard(&board);
                game_status = judgeStatus(&board, row, col, current_player);
            }
        }
        if(game_mode == P2AI_PLAYER_WHITE){
            boardInit(&board);
            showBoard(&board);
            Piece ai_color = BLACK;
            double time_use_now = 0, time_use_total = 0, time_use_avr = 0;
            int ai_cnt = 0;
            restart = 'n';
            piece_color = WHITE;
            current_player = PLAYER_WHITE;
            game_status = PLAYING;
            //游戏开始
            while(game_status == PLAYING){
                current_player = (current_player==PLAYER_WHITE) ? PLAYER_BLACK : PLAYER_WHITE; //轮流落子
                piece_color = (piece_color==WHITE) ? BLACK : WHITE;
                if(piece_color != ai_color){
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
                                if(drop_flag == -2){
                                    showBoard(&board);
                                    printf("黑子第一手应在天元H8，请重新输入！\n");
                                }
                            }
                        }
                        showInputPrompt(current_player);
                        getInput(input, INPUT_MAX_LEN);
                        trans_flag = transInput2Coord(input, &row, &col); //坐标转换
                        // printf("%d,%d\n", row, col);
                        // system("pause");
                        if(!trans_flag){
                            continue;
                        }
                        //转换成功则落子
                        //判断第一手是否在天元
                        if(board.pieceTotal == 0){
                            if(row!=8 || col!=8){
                                drop_flag = -2;
                            }else{
                                drop_flag = dropPiece(&board, row, col, piece_color);
                            }
                        }else{
                            drop_flag = dropPiece(&board, row, col, piece_color);
                        }
                    }while(drop_flag != 1); //直到落子成功才退出输入环节
                }else{
                    printf("AI思考中……\n");
                    time_use_now = aiMakeDecision(&board, ai_color, &row, &col);
                    time_use_total += time_use_now;
                    drop_flag = dropPiece(&board, row, col, piece_color);
                    ai_cnt++;
                    time_use_avr = time_use_total/(double)ai_cnt;
                    printf("本次落子位置：%c%d\n", (char)(col+'A'-1), row);
                    printf("本次思考用时：%.3fs，平均用时：%.3fs\n", time_use_now/1000, time_use_avr/1000);
                    system("pause");
                }
                showBoard(&board);
                game_status = judgeStatus(&board, row, col, current_player);
            }
        }
        showGameOver(game_status);
        printf("要重新开始游戏吗？(y/n)：");
        scanf("%c", &restart);
        getchar();
    }while(restart == 'y');
    return 0;
}