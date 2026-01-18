# C语言程序设计课程期中作业—人人对战五子棋

## TO-DO List
- [x] 棋盘显示
- [x] 人机交互（下棋）
- [x] 胜负判定
- [x] 禁手规则
  - [x] 三三
  - [x] 四四
  - [x] 长连
- [x] 完善注释
- [x] AI算法调试
  - [x] alpha-beta剪枝
  - [x] 排序优先搜索
  - [x] 邻域搜索
  - [x] 迭代加深
  - [x] 置换表
  - [x] PVS (主要变例搜索)
  - [x] 杀手启发式搜索
- [x] 修改文件结构，添加AI模块
- [x] 适配三种游戏模式
- [x] 修改README
## 项目简介

本项目为人机对战五子棋，在控制台（cmd）运行，由玩家和AI轮流输入落子坐标，程序将自动判定玩家是否获胜、是否出现禁手。
用递归处理禁手判断，可以对复杂禁手进行判断。
AI使用极大极小搜索，优化方法为alpha-beta剪枝、排序优先搜索、邻域搜索、迭代加深和置换表，可以有效地尽早剪枝、减少重复运算。

## 编译与运行方法

- 使用CMake和MinGW编译，仅支持**windows**环境
- 请先检查环境是否安装好，输入下列指令后若能输出对应版本号，则环境配置完成
```bash
gcc -v
cmake --version
mingw32-make -v
```
- 编译方法1：先进入项目根目录，输入下列指令：
```bash
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make
```
- 编译方法2：使用写好的批处理脚本，在根目录中输入下列指令`./cmake_build.bat`
- 运行方法：双击build/中的P2PChessGame.exe或者在根目录中`./build/P2PChessGame.exe`
- 测试方法：测试文件位于`./test/test_rule.c`，可以自行添加测试用例，编译内置于`CMakeLists.txt`中，编译后可执行文件为`./build/test_rule.exe`
## 文件结构

本项目包含多个文件，在此节中说明文件组织结构
```
2024K8009929011/       # 根目录
├── src/               # 源代码目录
│   ├── ai/  
│   │   ├── ai.h       # AI相关的头文件
│   │   ├── ai.c       # AI相关函数的实现（搜索以及优化等）
│   │   ├── evaluate.h # 评估函数相关的头文件
│   │   ├── evaluate.c # 评估函数相关函数的实现(评估函数、比较函数等)
│   │   ├── tt.h       # 置换表相关的头文件
│   │   └── tt.c       # 置换表相关函数的实现(初始化、存储、查询等)
│   ├── board/      
│   │   ├── board.h    # 棋盘相关的头文件
│   │   └── board.c    # 棋盘相关函数的实现(初始化、落子等)
│   ├── display/   
│   │   ├── display.h  # 显示相关的头文件
│   │   └── display.c  # 显示相关函数的实现(棋盘绘制、清屏等)
│   ├── rule/  
│   │   ├── rule.h     # 规则相关的头文件
│   │   └── rule.c     # 规则相关函数的实现(胜负判定、禁手等)
│   └── main.c         # 主程序
├── test/              # 测试用例
│   └── test_rule.c    # 规则测试文件
├── .gitignore         # Git忽略配置
├── CMakeLists.txt     # Cmake文件
├── cmake_build.bat    # Cmake编译批处理脚本
└── README.md          # 说明文档
```
## 操作说明
- 所有输入均在stdin，所有输出均在stdout
- 输入时有明确文字提示，用键盘输入坐标即可，若输入不合法会有提示并需要重新输入
- 坐标输入结束后需要按回车才能将输入提交，程序可以即时反应并将结果打印
- 游戏结束时会有明确的提示，可以自由选择是否重新开始游戏

## 功能列表
- main.c
  - 棋盘显示
  - 三种游戏模式
  - 玩家输入坐标落子并判断落子位置是否合法，不合法要求重新输入
  - 自动判定胜负（五连、禁手、平局）
- board.h
  - `void boardInit(Board* board)`：棋盘初始化
  - `int dropPiece(Board* board, int row, int col, Piece piece_color)`：落子，包括棋子总数、最新落子、颜色修改和想、邻域标识修改
  - `Piece getPiece(const Board* board, int row, int col)`：获取棋子的颜色（调用前需要判断坐标合法性）
  - `bool isEmpty(const Board* board, int row, int col)`：判断坐标点是否为空
  - `bool transInput2Coord(const char s[], int* row, int* col)`：将字符坐标输入转换为有序整数对(row, col)，同时确保范围在(1,1)~(15,15)
  - `bool boardIsFull(const Board* board)`：判断棋盘是否下满，判断和棋

- display.h
  - `void showBoard(const Board* board)`：显示棋盘
  - `void showInputPrompt(Player current_player)`：用户输入的提示语
  - `void showWelcomeMsg()`：开始游戏时的提示语
  - `void showGameOver(GameStatus game_status)`：游戏结束时的提示语
  - `void getInput(char s[], int max_len)`：读取用户输入的坐标

- ai.h
  - `double aiMakeDecision(const Board* board, Piece ai_color, int* row, int* col)`：AI决策函数，返回最佳落子位置
  - `int alphaBetaSearch(Board* board, int depth, int alpha, int beta, bool is_max_player, Piece ai_color, ULL current_hash)`：极大极小搜索算法带alpha-beta剪枝
  - `void initAI()`：初始化随机种子，初始化置换表，获取搜索起始时间
  - `int generatePossibleMoves(const Board* board, PossibleMoves possible_moves[], Piece ai_color, bool is_max_player)`：邻域搜索生成函数
  - `int iterativeDeepeningSearch(Board* board, Piece ai_color, int start_depth, int target_depth, int* best_r, int* best_c)`：迭代加深搜索
  - `void sortMovesHeuristic(PossibleMoves moves[], int count, int depth, int tt_r, int tt_c)`：走法综合排序

- tt.h
  - `void initZobrist()`：初始化哈希计算的随机数
  - `void clearTT()`：清空置换表
  - `ULL hashBoard(const Board* board)`：计算整个棋盘哈希
  - `void updateHash(ULL* hash, int row, int col, int old_color, int new_color)`：增量更新哈希
  - `void storeTT(ULL hash, int score, int depth, int type, int r, int c)`：保存哈希
  - `bool retrieveTT(ULL hash, int* score, int* depth, int* type, int* r, int* c)`：哈希查找

- evaluate.h
  - `int evaluateFullBoard(const Board* board, Player ai_player)`：棋局评估函数
  - `int evaluatePostion(const Board* board, int row, int col, Piece ai_color, Piece current_color)`：对特定位置的评估函数
  - `int cmp(const void* a, const void* b)`：用于qsort的结构体降序比较

- rule.h
  - `GameStatus judgeStatus(const Board* board, int row, int col, Player current_player)`：判断游戏状态
  - `void checkChessShape(const Board* board, int row, int col, int chess_shape_cnt[], Player current_player)`：统计当前棋子参与构成的棋型
  - `bool isForbiddenMove(const int chess_shape_cnt[])`：判断是否为禁手
  - `bool checkPieceInRowWithDir(const Board* board, int row, int col, int num, Pair dir, Player current_player)`：检查某个特定方向上连成的棋型，判断是否为活三、活四或五连
  - `bool isForbiddenPosition(const Board* board, int row, int col)`：模拟落子，判断空白位置是否为禁手点位

## 已知问题

- 棋盘显示时部分字符编码有问题，仅支持UTF-8，调试时无法支持GBK编码
- 棋盘边框仅支持圆角边框，使用直角边框会出现乱码
- 禁手判断函数中冲四计数器会把活四计为2个冲四，在统计时用`chess_shape_cnt[BREAKTHROUGH_FOUR]-2*chess_shape_cnt[LIVE_FOUR]`解决
  - 对冲四判定不太严格
  - 已解决该问题

## 其他
参数修改：
- `board.h`中的NEIGHBORHOOD_SIZE：邻域搜索范围
- `ai.h`中的ST_SEARCH_DEPTH：起始搜索深度
- `ai.h`中的TG_SEARCH_DEPTH：终止搜索深度
- `ai.h`中的MAX_TIME：最大搜索时间