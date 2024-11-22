#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <random>

// ANSI 转义码定义
#define RED     "\033[31m"    // 红色
#define BLUE    "\033[34m"    // 蓝色 
#define GREEN   "\033[32m"    //绿色 
#define RESET   "\033[0m"     // 重置为默认颜色

using namespace std;

int n; // 游戏区域大小
vector<vector<vector<int>>> a; // a[x][y][0] = 周围雷数，a[x][y][1] = 是否已选择，a[x][y][2] = 是否是雷

// 初始化游戏
void init() {
    cout << "Please input size (<=100): ";
    cin >> n;

    if (n > 100) {
        cout << "Size exceeds maximum limit of 100." << endl;
        return;
    }

    a.resize(n, vector<vector<int>>(n, vector<int>(3, 0)));  // 初始化三维数组
    srand(time(0)); // 随机种子

    // 随机放置雷
    int numMines = n; // 雷的数量
    for (int i = 0; i < numMines; ++i) {
        int x, y;
        do {
            x = rand() % n;
            y = rand() % n;
        } while (a[x][y][2] == 1);  // 确保雷不重复
        a[x][y][2] = 1;  // 标记雷
    }

    // 计算每个非雷区的周围雷数
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (a[i][j][2] == 1) continue; // 如果是雷，跳过
            int count = 0;
            // 检查周围的 8 个方向
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    int nx = i + dx, ny = j + dy;
                    if (nx >= 0 && nx < n && ny >= 0 && ny < n && a[nx][ny][2] == 1) {
                        count++;
                    }
                }
            }
            a[i][j][0] = count; // 设置周围雷数
        }
    }
}

// 显示游戏状态
void show() {
    system("cls");  // 清屏

    cout << "Current Game State:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (a[i][j][1] == 0) {
                cout << " |?| ";  // 未被选择的区域
            } else if (a[i][j][2] == 0) {
            	if (a[i][j][0] != 0)
                	cout << BLUE << " |" << a[i][j][0] << "| " << RESET;  // 打开的区域，显示数字
                else 
                	cout << GREEN << " |" << a[i][j][0] << "| " << RESET;  // 打开的区域，显示数字
            } else {
                cout << RED << " |*| " << RESET;  // 雷区
            }
        }
        cout << "\n" << endl;
    }
}

// 判断游戏是否结束
int win_or_lose(int x, int y) {
    if (a[x][y][2] == 1) {
        a[x][y][1] = 1;  // 标记为已选择
        return -1;  // 游戏失败
    } else {
        a[x][y][1] = 1;  // 标记为已选择
    }

    // 判断是否胜利
    int flag = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (a[i][j][1] == 0 && a[i][j][2] == 0) {
                flag++;
            }
        }
    }
    if (flag == 0) return 1;  // 所有非雷区域都已被打开，游戏胜利
    return 0;  // 游戏继续
}

// 扫描相邻区域
void open_surroundings(int x, int y) {
    if (x < 0 || x >= n || y < 0 || y >= n || a[x][y][1] == 1) return;  // 越界或已选择的区域
    a[x][y][1] = 1;  // 标记为已选择
    if (a[x][y][0] == 0) {  // 如果周围没有雷，递归打开周围区域
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                open_surroundings(x + dx, y + dy);
            }
        }
    }
}

// 主程序
int main() {
    init();

    int game_status = 0;
    while (game_status == 0) {
        show();

        int x, y;
        cout << "Enter coordinates (x y): ";
        cin >> x >> y;

        if (x < 0 || x >= n || y < 0 || y >= n) {
            cout << "Invalid coordinates!" << endl;
            continue;
        }

        // 扫描周围区域
        open_surroundings(x, y);

        // 检查游戏状态
        game_status = win_or_lose(x, y);
    }

    // 游戏结束
    if (game_status == -1) {
    	show();
        cout << "Game Over! You hit a mine." << endl;
    } else if (game_status == 1) {
    	show();
        cout << "Congratulations! You win!" << endl;
    }

    return 0;
}

