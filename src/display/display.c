#include"display.h"

void showBoard(const Board* board){
    system("cls");
    for(int i=BOARD_SIZE; i>=1; --i){
        printf("%-3d", i);
        for(int j=1; j<=BOARD_SIZE; ++j){
            if(board->pieceColor[i][j] == BLANK){
                if(i==BOARD_SIZE && j==1){
                    printf("%s ", LEFT_TOP_CORNER);
                    continue;
                }
                if(i==BOARD_SIZE && (j!=1&&j!=BOARD_SIZE)){
                    printf("%s ", TOP_EDGE);
                    continue;
                }
                if(i==BOARD_SIZE && j==BOARD_SIZE){
                    printf("%s ", RIGHT_TOP_CORNER);
                    continue;
                }
                if(j==BOARD_SIZE && (i!=1&&i!=BOARD_SIZE)){
                    printf("%s ", RIGHT_EDGE);
                    continue;
                }
                if(i==1 && j==1){
                    printf("%s ", LEFT_BOTTOM_CORNER);
                    continue;
                }
                if(i==1 && (j!=1&&j!=BOARD_SIZE)){
                    printf("%s ", BOTTOM_EDGE);
                    continue;
                }
                if(i==1 && j==BOARD_SIZE){
                    printf("%s ", RIGHT_BOTTOM_CORNER);
                    continue;
                }
                if(j==1 && (i!=1&&i!=BOARD_SIZE)){
                    printf("%s ", LEFT_EDGE);
                    continue;
                }
                printf("%s ", CROSS);
            }else{
                if(i==board->last_row && j==board->last_col){
                    if(board->pieceColor[i][j] == WHITE){
                        printf("%s ", WHITE_PIECE_LAST);
                    }
                    if(board->pieceColor[i][j] == BLACK){
                        printf("%s ", BLACK_PIECE_LAST);
                    }
                }else{
                    if(board->pieceColor[i][j] == WHITE){
                        printf("%s ", WHITE_PIECE);
                    }
                    if(board->pieceColor[i][j] == BLACK){
                        printf("%s ", BLACK_PIECE);
                    }
                }
            }
        }
        printf("\n");
    }
    printf("   ");
    for(int i=1; i<=BOARD_SIZE; ++i){
        printf("%c ", 'A'+i-1);
    }
    printf("\n");
    // system("pause");
    return;
}

void showWelcomeMsg(){
    printf("==================== 欢迎游玩五子棋 ====================\n");
    
    printf("【输入方式】\n");
    printf("  棋盘列对应字母 A-O（A=第1列，O=第15列），行对应数字 1-15\n");
    printf("  落子需输入「列字母+行数字」（字母大写），例如：H11 代表第8列、第11行\n\n");

    printf("【游戏规则】\n");
    printf("  1. 由黑方率先落子，之后黑、白方轮流落子\n");
    printf("  2. 黑方存在「禁手」限制：不可同时形成双活三、双四连、长连（6子及以上相连）\n");
    printf("  3. 白方无禁手限制\n\n");

    printf("【棋盘表示】\n");
    printf("  1. 非当前棋子用圆表示，黑棋为%s，白棋为%s\n", BLACK_PIECE, WHITE_PIECE);
    printf("  2. 当前棋子用三角表示，黑棋为%s，白棋为%s\n", BLACK_PIECE_LAST, WHITE_PIECE_LAST);
    printf("  3. 其余非棋子交叉点均为空白棋盘，可以落子\n\n");

    printf("【胜负判定】\n");
    printf("  1. 任意一方率先将 5 颗同色棋子连成一线（横、竖、斜向均可），直接获胜\n");
    printf("  2. 黑方触发禁手时，直接判负，白方获胜\n\n");

    printf("======================== GL & HF ========================\n");
    // system("pause");
    return;
}

void showInputPrompt(Player current_player){
    printf("落子列坐标范围A~O，行坐标范围1~15\n");
    if(current_player == PLAYER_BLACK){
        printf("您是黑方，请输入落子坐标：");
    }
    if(current_player == PLAYER_WHITE){
        printf("您是白方，请输入落子坐标：");
    }
    return;
}

void getInput(char s[], int max_len){
    char c;
    int Index = 0;
    while((c=getchar())!='\n'){
        if(Index < max_len){
            s[Index++] = c;
        }
    }
    s[Index] = '\0';
    return;
}

void showGameOver(GameStatus game_status){
    printf("\n=================================================\n");
    printf("                    游戏结束                     \n");
    printf("=================================================\n\n");
    if(game_status == BLACK_WIN){
        printf("                    黑方获胜！                     \n\n");
    }
    if(game_status == WHITE_WIN){
        printf("                    白方获胜！                     \n\n");
    }
    if(game_status == FORBIDDEN_MOVE){
        printf("               黑方出现禁手，白方获胜！                \n\n");
    }
    if(game_status == DRAW){
        printf("                 棋盘已满，和棋！                    \n\n");
    }
    //system("pause");
    return;
}