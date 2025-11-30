#pragma once
#include <string>
#include <vector>

const int MENU_INDENT = 2;

void setCursorPos(int y, int x = MENU_INDENT);
int main_showMenu(const std::string& title, const std::vector<std::string>& opts, bool rememberPosition);