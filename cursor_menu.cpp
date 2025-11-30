#include "cursor_menu.h"
#include <windows.h>
#include <ctime>
#include <conio.h>
#include <iostream>
#include <vector>

static int lastSelectedPosition = 0;

void setCursorPos(int y, int x) {
    COORD pos = { short(x), short(y) };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int main_showMenu(const std::string& title, const std::vector<std::string>& opts, bool rememberPosition) {
    srand(time(NULL));
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    system("cls");
    std::cout << title << '\n' << std::string(40, '~') << '\n';

    int ch = rememberPosition ? lastSelectedPosition : 0;
    if (ch >= opts.size()) ch = 0;

    for (int i = 0; i < opts.size(); i++) {
        setCursorPos(i + 2, 0);
        std::cout << "  " << opts[i];
    }

    int prev = -1;
    while (true) {
        if (prev != ch) {
            if (prev >= 0) {
                setCursorPos(prev + 2, 0);
                SetConsoleTextAttribute(h, 7);
                std::cout << "  " << opts[prev];
            }

            setCursorPos(ch + 2, 0);
            int pipeColor = 9;
            int backgroundColor = (pipeColor - 8) << 4;

            SetConsoleTextAttribute(h, pipeColor);
            std::cout << " |";
            SetConsoleTextAttribute(h, backgroundColor | 7);
            std::cout << opts[ch];
            SetConsoleTextAttribute(h, 7);

            prev = ch;
        }

        switch (_getch()) {
        case 224: case 0:
            switch (_getch()) {
            case 72: ch = (ch - 1 + opts.size()) % opts.size(); break;
            case 80: ch = (ch + 1) % opts.size(); break;
            } break;
        case 13:
            if (rememberPosition) {
                lastSelectedPosition = ch;
            }
            setCursorPos(opts.size() + 3, 0);
            return ch;
        }
    }
}