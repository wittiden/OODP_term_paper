#include "authentication_menu.h"
#include "functions.h"
#include <iostream>
#include <vector>
#include <string>

AuthSystem authSystem;

bool auth_term_paper_menu() {
    bool userAuthenticated = false;

    while (!userAuthenticated) {
        const std::vector<std::string> auth_term_paper_menu_const{
            "Зарегистрироваться",
            "Войти в аккаунт",
            "Выйти в предыдущее меню",
            "Выход из программы"
        };
        int auth_term_paper_menu_choice = main_showMenu("MAIN_menu/AUTHENTICATION_menu/", auth_term_paper_menu_const, false);

        switch (auth_term_paper_menu_choice) {
        case AUTH_REGISTRATION:
            if (authSystem.registerUser()) {
                userAuthenticated = true;
            }
            break;
        case AUTH_LOGIN:
            if (authSystem.loginUser()) {
                userAuthenticated = true;
            }
            break;
        case AUTH_EXIT:
            return false;
        case AUTH_PROGRAM_EXIT:
            std::cout << "Выход из программы";
            loadingImitation();
            exit(0);
        default:
            std::cout << "Ошибка в работе меню аутентификации";
            system("pause");
        }
    }

    return userAuthenticated;
}
bool checkAdminPassword() {
    std::string password;
    std::cout << "Введите ключ администратора: ";
    letter_filteredInput<std::string>(password, 0, 0, 1);
    return password == "admin";
}