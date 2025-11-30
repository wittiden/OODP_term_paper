#pragma once
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "cursor_menu.h"
#include "threads_options.h"

enum AuthMenu {
    AUTH_REGISTRATION,
    AUTH_LOGIN,
    AUTH_EXIT,
    AUTH_PROGRAM_EXIT
};

bool auth_term_paper_menu();
bool checkAdminPassword();