#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "cursor_menu.h"
#include "threads_options.h"

void user_term_paper_menu();
void admin_term_paper_menu();
void user_mgmt_term_paper_menu();

enum UserMenu {
    USER_RATE_PROPERTY,
    USER_EDIT_RATING,
    USER_DELETE_RATING,
    USER_VIEW_MY_RATINGS,
    USER_VIEW_ALL_PROPERTIES,
    USER_SORT_BY_RATING,
    USER_FILTER_BY_PARAMS,
    USER_EXIT,
    USER_PROGRAM_EXIT
};

enum AdminMenu {
    ADMIN_MANAGE_USER_MENU,
    ADMIN_EDIT_ANY_RATING,
    ADMIN_DELETE_ANY_RATING,
    ADMIN_ADD_PROPERTY,
    ADMIN_EDIT_PROPERTY,
    ADMIN_DELETE_PROPERTY,
    ADMIN_VIEW_STATISTICS,
    ADMIN_VIEW_ALL_RATINGS,
    ADMIN_SORT_BY_RATING,
    ADMIN_FILTER_BY_PARAMS,
    ADMIN_EXIT,
    ADMIN_PROGRAM_EXIT
};

enum UserManagementMenu {
    USER_MGMT_VIEW_ALL_USERS,
    USER_MGMT_BLOCK_USER,
    USER_MGMT_UNBLOCK_USER,
    USER_MGMT_DELETE_USER,
    USER_MGMT_VIEW_USER_STATS,
    USER_MGMT_EXIT,
    USER_MGMT_PROGRAM_EXIT
};