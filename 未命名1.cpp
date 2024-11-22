#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <random>

// ANSI ת���붨��
#define RED     "\033[31m"    // ��ɫ
#define BLUE    "\033[34m"    // ��ɫ 
#define GREEN   "\033[32m"    //��ɫ 
#define RESET   "\033[0m"     // ����ΪĬ����ɫ

using namespace std;

int n; // ��Ϸ�����С
vector<vector<vector<int>>> a; // a[x][y][0] = ��Χ������a[x][y][1] = �Ƿ���ѡ��a[x][y][2] = �Ƿ�����

// ��ʼ����Ϸ
void init() {
    cout << "Please input size (<=100): ";
    cin >> n;

    if (n > 100) {
        cout << "Size exceeds maximum limit of 100." << endl;
        return;
    }

    a.resize(n, vector<vector<int>>(n, vector<int>(3, 0)));  // ��ʼ����ά����
    srand(time(0)); // �������

    // ���������
    int numMines = n; // �׵�����
    for (int i = 0; i < numMines; ++i) {
        int x, y;
        do {
            x = rand() % n;
            y = rand() % n;
        } while (a[x][y][2] == 1);  // ȷ���ײ��ظ�
        a[x][y][2] = 1;  // �����
    }

    // ����ÿ������������Χ����
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (a[i][j][2] == 1) continue; // ������ף�����
            int count = 0;
            // �����Χ�� 8 ������
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    int nx = i + dx, ny = j + dy;
                    if (nx >= 0 && nx < n && ny >= 0 && ny < n && a[nx][ny][2] == 1) {
                        count++;
                    }
                }
            }
            a[i][j][0] = count; // ������Χ����
        }
    }
}

// ��ʾ��Ϸ״̬
void show() {
    system("cls");  // ����

    cout << "Current Game State:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (a[i][j][1] == 0) {
                cout << " |?| ";  // δ��ѡ�������
            } else if (a[i][j][2] == 0) {
            	if (a[i][j][0] != 0)
                	cout << BLUE << " |" << a[i][j][0] << "| " << RESET;  // �򿪵�������ʾ����
                else 
                	cout << GREEN << " |" << a[i][j][0] << "| " << RESET;  // �򿪵�������ʾ����
            } else {
                cout << RED << " |*| " << RESET;  // ����
            }
        }
        cout << "\n" << endl;
    }
}

// �ж���Ϸ�Ƿ����
int win_or_lose(int x, int y) {
    if (a[x][y][2] == 1) {
        a[x][y][1] = 1;  // ���Ϊ��ѡ��
        return -1;  // ��Ϸʧ��
    } else {
        a[x][y][1] = 1;  // ���Ϊ��ѡ��
    }

    // �ж��Ƿ�ʤ��
    int flag = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (a[i][j][1] == 0 && a[i][j][2] == 0) {
                flag++;
            }
        }
    }
    if (flag == 0) return 1;  // ���з��������ѱ��򿪣���Ϸʤ��
    return 0;  // ��Ϸ����
}

// ɨ����������
void open_surroundings(int x, int y) {
    if (x < 0 || x >= n || y < 0 || y >= n || a[x][y][1] == 1) return;  // Խ�����ѡ�������
    a[x][y][1] = 1;  // ���Ϊ��ѡ��
    if (a[x][y][0] == 0) {  // �����Χû���ף��ݹ����Χ����
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                open_surroundings(x + dx, y + dy);
            }
        }
    }
}

// ������
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

        // ɨ����Χ����
        open_surroundings(x, y);

        // �����Ϸ״̬
        game_status = win_or_lose(x, y);
    }

    // ��Ϸ����
    if (game_status == -1) {
    	show();
        cout << "Game Over! You hit a mine." << endl;
    } else if (game_status == 1) {
    	show();
        cout << "Congratulations! You win!" << endl;
    }

    return 0;
}

