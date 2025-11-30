#include "main_menu.h"
#include "secondary_menu.h"
#include "authentication_menu.h"

void main_term_paper_menu() {
    bool main_term_paper_menu_flag = false;
    while (!main_term_paper_menu_flag) {
        const std::vector<std::string> main_term_paper_menu_const{
            "Пользователь",
            "Администратор",
            "Выход из программы"
        };
        int main_term_paper_menu_choice = main_showMenu(".../MAIN_menu/", main_term_paper_menu_const, false);

        switch (main_term_paper_menu_choice) {
        case MENU_USER:
            if (auth_term_paper_menu()) {
                user_term_paper_menu();
            }
            break;
        case MENU_ADMIN:
            if (checkAdminPassword()) {
                if (auth_term_paper_menu()) {
                    admin_term_paper_menu();
                }
            }
            break;
        case EXIT_PROGRAM:
            std::cout << "Выход из программы";
            loadingImitation();
            return;
        default:
            std::cout << "Ошибка в работе меню";
            system("pause");
        }
    }
}