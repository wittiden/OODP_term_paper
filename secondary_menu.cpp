#include "secondary_menu.h"
#include <iostream>
#include <vector>
#include <string>

#include "realestate.h"

RealEstateManager rem;

void user_term_paper_menu() {
    bool user_term_paper_menu_flag = false;
    while (!user_term_paper_menu_flag) {
        const std::vector<std::string> user_term_paper_menu_const{
            "Оценить недвижимость",
            "Редактировать свою оценку",
            "Удалить свою оценку",
            "Посмотреть все свои оценки",
            "Посмотреть рейтинг всей недвижимости",
            "Отсортировать всю недвижимость по рейтингу",
            "Отфильтровать по необходимым параметрам",
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
            // Реализация
            break;
        case USER_FILTER_BY_PARAMS:
            // Реализация
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
            break;
        case ADMIN_DELETE_ANY_RATING:
            rem.deleteObjectRating("admin");
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
            // Реализация
            break;
        case ADMIN_FILTER_BY_PARAMS:
            // Реализация
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
            // Реализация
            break;
        case USER_MGMT_BLOCK_USER:
            // Реализация
            break;
        case USER_MGMT_UNBLOCK_USER:
            // Реализация
            break;
        case USER_MGMT_DELETE_USER:
            // Реализация
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