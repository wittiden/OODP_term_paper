#include "secondary_menu.h"
#include <iostream>
#include <vector>
#include <string>

#include "realestate.h"
#include "functions.h"

RealEstateManager rem;
AuthSystem authSystem;

void user_term_paper_menu() {
    bool user_term_paper_menu_flag = false;
    while (!user_term_paper_menu_flag) {
        const std::vector<std::string> user_term_paper_menu_const{
            "Оценить недвижимость",
            "Редактировать свою оценку",
            "Удалить свою оценку",
            "Посмотреть все свои оценки",
            "Посмотреть рейтинг всей недвижимости",
            "Сортировка по рейтингу", 
            "Фильтрация по параметрам",
            "Выйти в предыдущее меню",
            "Выйти из программы"
        };
        int user_term_paper_menu_choice = main_showMenu(".../AUTHENTICATION_menu/USER_menu/", user_term_paper_menu_const, false);

        switch (user_term_paper_menu_choice) {
        case USER_RATE_PROPERTY:
            rem.calculateObjectRating();
            system("pause");
            break;
        case USER_EDIT_RATING:
            rem.changeObjectRating("user");
            system("pause");
            break;
        case USER_DELETE_RATING:
            rem.deleteObjectRating("user");
            system("pause");
            break;
        case USER_VIEW_MY_RATINGS:
            rem.displayUserObj();
            system("pause");
            break;
        case USER_VIEW_ALL_PROPERTIES:
            rem.displayObj();
            system("pause");
            break;
        case USER_SORT_BY_RATING:
            rem.sortByRating();
            break;
        case USER_FILTER_BY_PARAMS:
            filter_term_paper_menu();
            break;
        case USER_EXIT:
            user_term_paper_menu_flag = true;
            break;
        case USER_PROGRAM_EXIT:
            std::cout << "Выход из программы";
            loadingImitation();
            exit(0);
        default:
            std::cout << "Ошибка в работе меню пользователя";
            system("pause");
        }
    }
}

void admin_term_paper_menu() {
    rem.loadFromDatabase();
    bool admin_term_paper_menu_flag = false;
    while (!admin_term_paper_menu_flag) {
        const std::vector<std::string> admin_term_paper_menu_const{
            "Управление пользователями",
            "Редактировать любую оценку",
            "Удалить любую оценку",
            "Добавить объект недвижимости",
            "Редактировать объект недвижимости",
            "Удалить объект недвижимости",
            "Просмотр всей иноформации",
            "Сортировка по рейтингу",
            "Фильтрация по параметрам",
            "Выйти в главное меню",
            "Выйти из программы"
        };
        int admin_term_paper_menu_choice = main_showMenu(".../AUTHENTICATION_menu/ADMIN_menu/", admin_term_paper_menu_const, false);

        switch (admin_term_paper_menu_choice) {
        case ADMIN_MANAGE_USER_MENU:
            user_mgmt_term_paper_menu();
            break;
        case ADMIN_EDIT_ANY_RATING:
            rem.changeObjectRating("admin");
            system("pause");
            break;
        case ADMIN_DELETE_ANY_RATING:
            rem.deleteObjectRating("admin");
            system("pause");
            break;
        case ADMIN_ADD_PROPERTY:
            rem.addObj();
            rem.displayObj();
            system("pause");
            break;
        case ADMIN_EDIT_PROPERTY:
            rem.changeObj();
            break;
        case ADMIN_DELETE_PROPERTY:
            rem.deleteObj();
            break;
        case ADMIN_VIEW_ALL_INFO:
            rem.displayObj();
            system("pause");
            break;
        case ADMIN_SORT_BY_RATING:
            rem.sortByRating();
            break;
        case ADMIN_FILTER_BY_PARAMS:
            filter_term_paper_menu();
            break;
        case ADMIN_EXIT:
            admin_term_paper_menu_flag = true;
            break;
        case ADMIN_PROGRAM_EXIT:
            std::cout << "Выход из программы";
            loadingImitation();
            exit(0);
        default:
            std::cout << "Ошибка в работе меню администратора" << std::endl;
            system("pause");
        }
    }
}

void user_mgmt_term_paper_menu() {
    bool user_mgmt_term_paper_menu_flag = false;
    while (!user_mgmt_term_paper_menu_flag) {
        const std::vector<std::string> user_mgmt_term_paper_menu_const{
            "Просмотр всех пользователей",
            "Блокировка пользователя",
            "Разблокировка пользователя",
            "Удаление пользователя",
            "Выйти в предыдущее меню",
            "Выйти из программы"
        };
        int user_mgmt_term_paper_menu_choice = main_showMenu(".../ADMIN_menu/MGMT_menu/", user_mgmt_term_paper_menu_const, false);

        switch (user_mgmt_term_paper_menu_choice) {
        case USER_MGMT_VIEW_ALL_USERS:
            authSystem.displayUserInfo();
            system("pause");
            break;
        case USER_MGMT_BLOCK_USER:
            authSystem.blockUser();
            break;
        case USER_MGMT_UNBLOCK_USER:
            authSystem.unblockUser();
            break;
        case USER_MGMT_DELETE_USER:
            authSystem.deleteUser();
            break;
        case USER_MGMT_EXIT:
            user_mgmt_term_paper_menu_flag = true;
            break;
        case USER_MGMT_PROGRAM_EXIT:
            std::cout << "Выход из программы";
            loadingImitation();
            exit(0);
        default:
            std::cout << "Ошибка в работе меню пользователя";
            system("pause");
        }
    }
}
void filter_term_paper_menu() {
    bool filter_term_paper_menu_flag = false;
    static bool selectedParams[9];
    static bool initialized = false;

    if (!initialized) {
        for (int i = 0; i < 9; i++) {
            selectedParams[i] = false;
        }
        initialized = true;
    }

    while (!filter_term_paper_menu_flag) {
        std::vector<std::string> filter_term_paper_menu_const{
            std::string(selectedParams[FILTER_ADDRESS] ? "[*]" : "[ ]") + " Адрес",
            std::string(selectedParams[FILTER_AREA] ? "[*]" : "[ ]") + " Площадь",
            std::string(selectedParams[FILTER_ROOMS] ? "[*]" : "[ ]") + " Комнаты",
            std::string(selectedParams[FILTER_PRICE] ? "[*]" : "[ ]") + " Цена",
            std::string(selectedParams[FILTER_YEAR] ? "[*]" : "[ ]") + " Год",
            std::string(selectedParams[FILTER_FLOOR] ? "[*]" : "[ ]") + " Этаж",
            std::string(selectedParams[FILTER_ELEVATOR] ? "[*]" : "[ ]") + " Лифт",
            std::string(selectedParams[FILTER_RATING] ? "[*]" : "[ ]") + " Рейтинг АИС",
            std::string(selectedParams[FILTER_USER_RATING] ? "[*]" : "[ ]") + " Рейтинг пользователя",
            "Показать результаты",
            "Выйти в предыдущее меню",
            "Выйти из программы"
        };
        int filter_term_paper_menu_choice = main_showMenu(".../_menu/FILTER_menu/", filter_term_paper_menu_const, true);

        switch (filter_term_paper_menu_choice) {
        case FILTER_ADDRESS:
        case FILTER_AREA:
        case FILTER_ROOMS:
        case FILTER_PRICE:
        case FILTER_YEAR:
        case FILTER_FLOOR:
        case FILTER_ELEVATOR:
        case FILTER_RATING:
        case FILTER_USER_RATING:
            selectedParams[filter_term_paper_menu_choice] = !selectedParams[filter_term_paper_menu_choice];
            break;
        case FILTER_SHOW_RESULTS:
            rem.showFilteredResults(selectedParams);
            break;
        case FILTER_EXIT_BACK:
            filter_term_paper_menu_flag = true;
            break;
        case FILTER_PROGRAM_EXIT:
            std::cout << "Выход из программы";
            loadingImitation();
            exit(0);
        default:
            std::cout << "Ошибка в работе меню фильтрации";
            system("pause");
        }
    }
}
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