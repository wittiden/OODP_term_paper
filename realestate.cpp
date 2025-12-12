#include "realestate.h"
#include "functions.h"
#include <iostream>
#include <vector>
#include "secondary_menu.h"
#include "database_manager.h"

DatabaseManager dbManager;
extern AuthSystem authSystem;
size_t RealEstate::est_count = 1;
std::vector<Apartment> RealEstateManager::apartments;

RealEstate::RealEstate() : fullUsername("unknown"), address("unknown"), area(0), rooms(0), price(0.0f), commissYear(0), est_id(est_count++) {}
RealEstate::RealEstate(const std::string& fullUsername, const std::string& address, double area, int rooms, double price, int commissYear)
    : fullUsername(fullUsername), address(address), area(area), rooms(rooms), price(price), commissYear(commissYear), est_id(est_count++) {
}
RealEstate::~RealEstate() {}

std::string RealEstate::getAddress() const { return address; }
double RealEstate::getArea() const { return area; }
int RealEstate::getRooms() const { return rooms; }
double RealEstate::getPrice() const { return price; }
int RealEstate::getCommissYear() const { return commissYear; }
int RealEstate::getEst_id() const { return est_id; }
std::string RealEstate::getFullUsername() const { return fullUsername; }

void RealEstate::setAddress(const std::string& address) { this->address = address; }
void RealEstate::setArea(double area) { this->area = area; }
void RealEstate::setRooms(int rooms) { this->rooms = rooms; }
void RealEstate::setPrice(double price) { this->price = price; }
void RealEstate::setCommissYear(int commissYear) { this->commissYear = commissYear; }
void RealEstate::setEst_id(int est_id) { this->est_id = est_id; }
void RealEstate::setFullUsername(const std::string& fullUsername) { this->fullUsername = fullUsername; }

Apartment::Apartment() : floor(0), hasElevator(false), userRating(0.0) {}
Apartment::Apartment(const std::string& fullUsername, const std::string& address, double area, int rooms, double price, int commissYear, int floor, bool hasElevator)
    : RealEstate(fullUsername, address, area, rooms, price, commissYear), floor(floor), hasElevator(hasElevator), userRating(0.0) {
}
Apartment::~Apartment() {}

int Apartment::getFloor() const { return floor; }
bool Apartment::getHasElevator() const { return hasElevator; }
double Apartment::getUserRating() const { return userRating; }
bool Apartment::hasUserRating() const { return userRating > 0.0; }
void Apartment::setFloor(int floor) { this->floor = floor; }
void Apartment::setHasElevator(bool hasElevator) { this->hasElevator = hasElevator; }
void Apartment::setUserRating(double rating) { this->userRating = rating; }  

RealEstateType Apartment::getType() const {
    return RealEstateType::APARTMENT;
}

void Apartment::displayInfo() const {
    std::cout << "=== Квартира ===" << std::endl;
    std::cout << "ID: " << est_id << std::endl;
    std::cout << "Адрес: " << address << std::endl;
    std::cout << "Площадь: " << area << " м2" << std::endl;
    std::cout << "Комнат: " << rooms << std::endl;
    std::cout << "Цена: " << price << "$" << std::endl;
    std::cout << "Год ввода: " << commissYear << std::endl;
    std::cout << "Этаж: " << floor << std::endl;
    std::cout << "Лифт: " << (hasElevator ? "Да" : "Нет") << std::endl;
    std::cout << "Рейтинг (АИС): " << calculateRating() << std::endl;
    if (hasUserRating()) {
        std::cout << "Рейтинг пользователя: " << userRating << std::endl;
    }
}

double Apartment::calculateRating() const {

    double totalRating = 0.0;
    int maxPoints = 0;

    // 1. Рейтинг по площади (макс 2 балла)
    double areaPerRoom = area / rooms;
    double areaRating = 0.0;
    if (areaPerRoom >= 25.0) areaRating = 2.0;
    else if (areaPerRoom >= 20.0) areaRating = 1.5;
    else if (areaPerRoom >= 15.0) areaRating = 1.0;
    else areaRating = 0.5;

    totalRating += areaRating;
    maxPoints += 2;

    // 2. Рейтинг по цене (макс 2 балла)
    double pricePerSqmUSD = (price / area);
    double priceRating = 0.0;
    if (pricePerSqmUSD <= 900) priceRating = 2.0;
    else if (pricePerSqmUSD <= 1300) priceRating = 1.5;
    else if (pricePerSqmUSD <= 1700) priceRating = 1.0;
    else priceRating = 0.5;

    totalRating += priceRating;
    maxPoints += 2;

    // 3. Рейтинг по году (макс 2 балла)
    double yearRating = 0.0;
    int age = 2024 - commissYear;
    if (age <= 5) yearRating = 2.0;
    else if (age <= 15) yearRating = 1.5;
    else if (age <= 30) yearRating = 1.0;
    else yearRating = 0.5;

    totalRating += yearRating;
    maxPoints += 2;

    // 4. Рейтинг по этажу (макс 2 балла)
    double floorRating = 0.0;
    if (floor == 1) floorRating = 1.0;
    else if (floor >= 2 && floor <= 5) floorRating = 2.0;
    else if (floor <= 9) floorRating = 1.5;
    else floorRating = 1.0;

    totalRating += floorRating;
    maxPoints += 2;

    // 5. Бонус за лифт (макс 1 балл)
    double elevatorBonus = hasElevator ? 1.0 : 0.0;
    totalRating += elevatorBonus;
    maxPoints += 1;

    // 6. Рейтинг по комнатам (макс 1 балл)
    double roomsRating = 0.0;
    if (rooms >= 2 && rooms <= 4) roomsRating = 1.0;
    else roomsRating = 0.5;

    totalRating += roomsRating;
    maxPoints += 1;

    double normalizedRating = (totalRating / maxPoints) * 10.0;
    if (normalizedRating > 10.0) normalizedRating = 10.0;

    return std::round(normalizedRating * 10.0) / 10.0;
}

void RealEstateManager::addObj() {
    std::cout << "\n==================================================" << std::endl;
    std::cout << "          ДОБАВЛЕНИЕ ОБЪЕКТА НЕДВИЖИМОСТИ          " << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << "Тип объекта: APARTMENT" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;

    std::string address;
    double area;
    int rooms;
    double price;
    int commissYear;
    int floor;
    bool hasElevator;
    int choice;
    std::string fullUsername = authSystem.getCurrentUser().getFullname();

    bool validArea = false;
    bool validRooms = false;
    bool validPrice = false;
    bool validYear = false;
    bool validFloor = false;
    bool validElevator = false;
    bool validChoice = false;

    do {
        std::cout << "\n[ ВВОД ДАННЫХ ]" << std::endl;
        std::cout << "------------------------" << std::endl;

        std::cout << "Адрес: ";
        std::getline(std::cin, address);

        do {
            std::cout << "Площадь (м2): ";
            number_filteredInput<double>(area, 0, 1);
            if (area > 0) validArea = true;
            else {
                std::cerr << "ОШИБКА! Введите правильное число" << std::endl;
                validArea = false;
            }
        } while (!validArea);

        do {
            std::cout << "Количество комнат: ";
            number_filteredInput<int>(rooms, 0, 1);
            if (rooms > 0) validRooms = true;
            else {
                std::cerr << "ОШИБКА! Введите правильное число" << std::endl;
                validRooms = false;
            }
        } while (!validRooms);

        do {
            std::cout << "Цена ($): ";
            number_filteredInput<double>(price, 0, 1);
            if (price > 0) validPrice = true;
            else {
                std::cerr << "ОШИБКА! Введите правильное число" << std::endl;
                validPrice = false;
            }
        } while (!validPrice);

        do {
            std::cout << "Год ввода в эксплуатацию: ";
            number_filteredInput<int>(commissYear, 0, 1);
            if (commissYear >= 1850 && commissYear <= 2024) {
                validYear = true;
            }
            else {
                std::cerr << "ОШИБКА! Год должен быть от 1850 до 2024" << std::endl;
                validYear = false;
            }
        } while (!validYear);

        do {
            std::cout << "Этаж: ";
            number_filteredInput<int>(floor, 0, 1);
            if (floor > 0) validFloor = true;
            else {
                std::cerr << "ОШИБКА! Введите правильное число" << std::endl;
                validFloor = false;
            }
        } while (!validFloor);

        do {
            std::cout << "Наличие лифта (1 - да/0 - нет): ";
            number_filteredInput<bool>(hasElevator);
            if (hasElevator == 1 || hasElevator == 0) validElevator = true;
            else {
                std::cout << "ОШИБКА! Введите '1' или '0'" << std::endl;
                validElevator = false;
            }
        } while (!validElevator);

        Apartment apart(fullUsername, address, area, rooms, price, commissYear, floor, hasElevator);
        apart.calculateRating();
        apartments.push_back(apart);

        if (dbManager.saveApartment(apart)) {
            std::cout << "------------------------" << std::endl;
            std::cout << "[ УСПЕХ ] Объект добавлен в базу данных!" << std::endl;
            std::cout << "  ID квартиры: " << apart.getEst_id() << std::endl;
            std::cout << "  Всего квартир в базе: " << apartments.size() << std::endl;
        }
        else {
            std::cerr << "[ ОШИБКА ] Не удалось сохранить квартиру в базу данных!" << std::endl;
            apartments.pop_back();
        }

        validArea = false;
        validRooms = false;
        validPrice = false;
        validYear = false;
        validFloor = false;
        validElevator = false;

        std::cout << "\n--------------------------------------------------" << std::endl;
        std::cout << "ВЫБЕРИТЕ ДЕЙСТВИЕ:" << std::endl;
        std::cout << "  1 - Добавить еще одну квартиру" << std::endl;
        std::cout << "  2 - Завершить добавление и выйти" << std::endl;
        std::cout << "--------------------------------------------------" << std::endl;
        std::cout << "Ваш выбор: ";

        number_filteredInput<int>(choice, 0, 1);
        switch (choice) {
        case 1:
            break;
        case 2:
            validChoice = true;
            break;
        default:
            validChoice = true;
        }
        std::cout << "--------------------------------------------------" << std::endl;

    } while (!validChoice);

    std::cout << "\n[ ИНФОРМАЦИЯ ] Добавление объектов завершено." << std::endl;
    std::cout << "  Всего добавлено квартир: " << apartments.size() << std::endl;

}

void RealEstateManager::displayObj() {
    std::cout << "\n==================================================" << std::endl;
    std::cout << "          ПРОСМОТР ОБЪЕКТОВ НЕДВИЖИМОСТИ          " << std::endl;
    std::cout << "==================================================" << std::endl;

    std::cout << "Тип объектов: APARTMENT" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;

    if (apartments.empty()) {
        std::cout << "\n[ ВНИМАНИЕ ] База данных квартир пуста!" << std::endl;
        std::cout << "  Используйте функцию добавления объектов." << std::endl;
        std::cout << "==================================================" << std::endl;
        return;
    }

    std::cout << "\n[ СТАТИСТИКА ]" << std::endl;
    std::cout << "  Всего квартир в базе: " << apartments.size() << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;

    int counter = 1;
    for (const auto& apartment : apartments) {
        std::cout << "\n[ КВАРТИРА №" << counter << " ]" << std::endl;
        std::cout << "--------------------------------------------------" << std::endl;
        apartment.displayInfo();
        counter++;
    }

}

void RealEstateManager::deleteObj()
{
    if (!apartments.empty()) {

        displayObj();

        std::cout << "\n==================================================" << std::endl;
        std::cout << "           УДАЛЕНИЕ ОБЪЕКТА НЕДВИЖИМОСТИ          " << std::endl;
        std::cout << "==================================================" << std::endl;
        std::cout << "Тип объекта: APARTMENT" << std::endl;
        std::cout << "--------------------------------------------------" << std::endl;

        int delete_id;
        bool validChoice = false;
        int choice;

        do {
            std::cout << "\n[ ВВОД ДАННЫХ ]" << std::endl;
            std::cout << "------------------------" << std::endl;
            std::cout << "ID для удаления: ";
            number_filteredInput<int>(delete_id);

            bool found = false;
            for (auto it = apartments.begin(); it != apartments.end(); ++it) {
                if (delete_id == it->getEst_id()) {
                    if (dbManager.deleteApartment(delete_id)) {
                        apartments.erase(it);
                        found = true;
                        std::cout << "[ УСПЕХ ] Квартира с ID " << delete_id << " удалена из базы данных." << std::endl;
                    }
                    else {
                        std::cerr << "[ ОШИБКА ] Не удалось удалить квартиру из базы данных." << std::endl;
                    }
                    break;
                }
            }

            if (!found) {
                std::cerr << "[ ОШИБКА ] Квартира с ID " << delete_id << " не найдена." << std::endl;
            }

            std::cout << "\n--------------------------------------------------" << std::endl;
            std::cout << "ВЫБЕРИТЕ ДЕЙСТВИЕ:" << std::endl;
            std::cout << "  1 - Продолжить удаление по id" << std::endl;
            std::cout << "  2 - Завершить удаление и выйти" << std::endl;
            std::cout << "--------------------------------------------------" << std::endl;
            std::cout << "Ваш выбор: ";
            number_filteredInput<int>(choice, 0, 1);

            switch (choice) {
            case 1:
                break;
            case 2:
                validChoice = true;
                break;
            default:
                validChoice = true;
            }

        } while (!validChoice);

        std::cout << "--------------------------------------------------" << std::endl;
        std::cout << "\n[ ИНФОРМАЦИЯ ] Удаление объектов завершено." << std::endl;
        std::cout << "  Всего квартир осталось: " << apartments.size() << std::endl;
        system("pause");
    }
    else {
        std::cout << "[ СПИСОК ОБЪЕКТОВ ПУСТ ]\n";
        system("pause");
    }
}

void RealEstateManager::changeObj() {
    if (!apartments.empty()) {

        displayObj();

        std::cout << "\n==================================================" << std::endl;
        std::cout << "          ИЗМЕНЕНИЕ ОБЪЕКТА НЕДВИЖИМОСТИ          " << std::endl;
        std::cout << "==================================================" << std::endl;
        std::cout << "Тип объекта: APARTMENT" << std::endl;
        std::cout << "--------------------------------------------------" << std::endl;

        int change_id;
        bool validChoice = false;
        int choice;

        do {
            std::cout << "\n[ ВВОД ДАННЫХ ]" << std::endl;
            std::cout << "------------------------" << std::endl;
            std::cout << "ID для изменения: ";
            number_filteredInput<int>(change_id);

            bool found = false;

            for (auto it = apartments.begin(); it != apartments.end(); ++it) {
                if (change_id == it->getEst_id()) {
                    int saved_id = change_id;
                    Apartment oldApartment = *it;

                    apartments.erase(it);

                    found = true;

                    std::string address;
                    double area;
                    int rooms;
                    double price;
                    int commissYear;
                    int floor;
                    bool hasElevator;
                    std::string fullUsername = authSystem.getCurrentUser().getFullname();

                    bool validArea = false;
                    bool validRooms = false;
                    bool validPrice = false;
                    bool validYear = false;
                    bool validFloor = false;
                    bool validElevator = false;

                    std::cout << "\n[ ВВОД НОВЫХ ДАННЫХ ]" << std::endl;
                    std::cout << "------------------------" << std::endl;

                    std::cout << "Адрес: ";
                    std::getline(std::cin, address);

                    do {
                        std::cout << "Площадь (м2): ";
                        number_filteredInput<double>(area, 0, 1);
                        if (area > 0) validArea = true;
                        else {
                            std::cerr << "ОШИБКА! Введите положительное число" << std::endl;
                            validArea = false;
                        }
                    } while (!validArea);

                    do {
                        std::cout << "Количество комнат: ";
                        number_filteredInput<int>(rooms, 0, 1);
                        if (rooms > 0) validRooms = true;
                        else {
                            std::cerr << "ОШИБКА! Введите положительное число" << std::endl;
                            validRooms = false;
                        }
                    } while (!validRooms);

                    do {
                        std::cout << "Цена ($): ";
                        number_filteredInput<double>(price, 0, 1);
                        if (price > 0) validPrice = true;
                        else {
                            std::cerr << "ОШИБКА! Введите положительное число" << std::endl;
                            validPrice = false;
                        }
                    } while (!validPrice);

                    do {
                        std::cout << "Год ввода в эксплуатацию: ";
                        number_filteredInput<int>(commissYear, 0, 1);
                        if (commissYear >= 1850 && commissYear <= 2025) {
                            validYear = true;
                        }
                        else {
                            std::cerr << "ОШИБКА! Год должен быть от 1850 до 2025" << std::endl;
                            validYear = false;
                        }
                    } while (!validYear);

                    do {
                        std::cout << "Этаж: ";
                        number_filteredInput<int>(floor, 0, 1);
                        if (floor > 0) validFloor = true;
                        else {
                            std::cerr << "ОШИБКА! Введите положительное число" << std::endl;
                            validFloor = false;
                        }
                    } while (!validFloor);

                    do {
                        std::cout << "Наличие лифта (1 - да/0 - нет): ";
                        number_filteredInput<bool>(hasElevator);
                        if (hasElevator == 1 || hasElevator == 0) validElevator = true;
                        else {
                            std::cout << "ОШИБКА! Введите '1' или '0'" << std::endl;
                            validElevator = false;
                        }
                    } while (!validElevator);

                    Apartment apart(fullUsername, address, area, rooms, price, commissYear, floor, hasElevator);

                    apart.setEst_id(saved_id);
                    apartments.push_back(apart);

                    if (dbManager.updateApartment(apart)) {
                        apartments.push_back(apart);
                        RealEstate::est_count--;

                        std::cout << "[ УСПЕХ ] Квартира с ID " << saved_id << " изменена в базе данных." << std::endl;
                    }
                    else {
                        apartments.push_back(oldApartment);
                        std::cerr << "[ ОШИБКА ] Не удалось обновить квартиру в базе данных." << std::endl;
                    }
                    break;
                }
            }

            if (!found) {
                std::cerr << "[ ОШИБКА ] Квартира с ID " << change_id << " не найдена." << std::endl;
            }

            std::cout << "\n--------------------------------------------------" << std::endl;
            std::cout << "ВЫБЕРИТЕ ДЕЙСТВИЕ:" << std::endl;
            std::cout << "  1 - Продолжить изменение по id" << std::endl;
            std::cout << "  2 - Завершить изменение и выйти" << std::endl;
            std::cout << "--------------------------------------------------" << std::endl;
            std::cout << "Ваш выбор: ";
            number_filteredInput<int>(choice, 0, 1);

            switch (choice) {
            case 1:
                break;
            case 2:
                validChoice = true;
                break;
            default:
                validChoice = true;
            }

        } while (!validChoice);

        std::cout << "--------------------------------------------------" << std::endl;
        std::cout << "\n[ ИНФОРМАЦИЯ ] Изменение объектов завершено." << std::endl;
        std::cout << "  Всего квартир: " << apartments.size() << std::endl;
        system("pause");
    }
    else {
        std::cout << "[ СПИСОК ОБЪЕКТОВ ПУСТ ]\n";
        system("pause");
    }
}
double RealEstateManager::calculateObjectRating() {
    if (apartments.empty()) {
        std::cout << "[ СПИСОК КВАРТИР ПУСТ ]\n";
        return 0.0;
    }

    const User& currentUser = authSystem.getCurrentUser();

    displayObj();
    system("pause");

    std::string fullUsername;
    int find_id;
    bool validChoice = false;
    int choice;

    do {

        bool found = false;

        std::cout << "\n==================================================" << std::endl;
        std::cout << "    ПЕРСОНАЛЬНАЯ ОЦЕНКА КВАРТИРЫ ПОЛЬЗОВАТЕЛЕМ    " << std::endl;
        std::cout << "==================================================" << std::endl;
        std::cout << "--------------------------------------------------" << std::endl;

        std::cout << "\n[ ВВОД ДАННЫХ ]" << std::endl;
        std::cout << "------------------------" << std::endl;
        std::cout << "ID квартиры для оценки: ";
        number_filteredInput<int>(find_id);

        for (auto it = apartments.begin(); it != apartments.end(); ++it) {
            fullUsername = currentUser.getFullname();
            it->setFullUsername(fullUsername);

            if (find_id == it->getEst_id() && !it->hasUserRating()) {
                found = true;

                std::cout << "\n==================================================" << std::endl;
                std::cout << "    ОЦЕНКА КВАРТИРЫ ПОЛЬЗОВАТЕЛЕМ    " << std::endl;
                std::cout << "==================================================" << std::endl;
                std::cout << "Найдена квартира:" << std::endl;
                std::cout << "ID: " << it->getEst_id() << std::endl;
                std::cout << "Оценивающий: " << it->getFullUsername() << std::endl;
                std::cout << "Адрес: " << it->getAddress() << std::endl;
                std::cout << "Площадь: " << it->getArea() << " м2" << std::endl;
                std::cout << "Комнат: " << it->getRooms() << std::endl;
                std::cout << "Цена: " << it->getPrice() << " $" << std::endl;
                std::cout << "Год ввода: " << it->getCommissYear() << std::endl;
                std::cout << "Этаж: " << it->getFloor() << std::endl;
                std::cout << "Лифт: " << (it->getHasElevator() ? "Да" : "Нет") << std::endl;
                std::cout << "Системный рейтинг (АИС): " << it->calculateRating() << "/10" << std::endl;
                if (it->hasUserRating()) {
                    std::cout << "Текущий пользовательский рейтинг: " << it->getUserRating() << "/10" << std::endl;
                }
                std::cout << "==================================================" << std::endl;

                double totalUserRating = 0.0;
                int maxUserPoints = 0;
                double userInput = 0.0;

                std::cout << "\n[1. ОЦЕНКА ПО ПАРАМЕТРАм КВАРТИРЫ]" << std::endl;
                std::cout << "====================================" << std::endl;

                std::cout << "\n[1.1 ПЛОЩАДЬ: " << it->getArea() << " м2]" << std::endl;
                std::cout << "Рекомендации:" << std::endl;
                std::cout << "  2.0 - Отличная площадь, просторно (" << it->getArea() / it->getRooms() << " м2/комнату)" << std::endl;
                std::cout << "  1.5 - Хорошая площадь, комфортно" << std::endl;
                std::cout << "  1.0 - Средняя площадь, достаточно" << std::endl;
                std::cout << "  0.5 - Маленькая площадь, тесно" << std::endl;
                std::cout << "Ваша оценка площади (0.5 - 2.0): ";
                number_filteredInput<double>(userInput, 0, 1);
                if (userInput < 0.5) userInput = 0.5;
                if (userInput > 2.0) userInput = 2.0;
                totalUserRating += userInput;
                maxUserPoints += 2;

                std::cout << "\n[1.2 КОЛИЧЕСТВО КОМНАТ: " << it->getRooms() << "]" << std::endl;
                std::cout << "Рекомендации:" << std::endl;
                std::cout << "  2.0 - Идеальное количество для ваших нужд" << std::endl;
                std::cout << "  1.5 - Хорошее количество, подходит" << std::endl;
                std::cout << "  1.0 - Среднее количество, можно рассмотреть" << std::endl;
                std::cout << "  0.5 - Не подходящее количество комнат" << std::endl;
                std::cout << "Ваша оценка количества комнат (0.5 - 2.0): ";
                number_filteredInput<double>(userInput, 0, 1);
                if (userInput < 0.5) userInput = 0.5;
                if (userInput > 2.0) userInput = 2.0;
                totalUserRating += userInput;
                maxUserPoints += 2;

                std::cout << "\n[1.3 ЭТАЖ: " << it->getFloor() << "]" << std::endl;
                std::cout << "Рекомендации:" << std::endl;
                std::cout << "  2.0 - Идеальный этаж (2-5 этаж), удобно" << std::endl;
                std::cout << "  1.5 - Хороший этаж, нормально" << std::endl;
                std::cout << "  1.0 - Средний этаж, есть неудобства" << std::endl;
                std::cout << "  0.5 - Неудобный этаж (1 или очень высокий)" << std::endl;
                std::cout << "Ваша оценка этажа (0.5 - 2.0): ";
                number_filteredInput<double>(userInput, 0, 1);
                if (userInput < 0.5) userInput = 0.5;
                if (userInput > 2.0) userInput = 2.0;
                totalUserRating += userInput;
                maxUserPoints += 2;

                std::cout << "\n[1.4 ЛИФТ: " << (it->getHasElevator() ? "Да" : "Нет") << "]" << std::endl;
                std::cout << "Рекомендации:" << std::endl;
                std::cout << "  2.0 - Лифт есть и это важно для вас" << std::endl;
                std::cout << "  1.0 - Без лифта, но это не критично" << std::endl;
                std::cout << "  0.5 - Нет лифта и это проблема" << std::endl;
                std::cout << "Ваша оценка наличия лифта (0.5 - 2.0): ";
                number_filteredInput<double>(userInput, 0, 1);
                if (userInput < 0.5) userInput = 0.5;
                if (userInput > 2.0) userInput = 2.0;
                totalUserRating += userInput;
                maxUserPoints += 2;

                int age = 2024 - it->getCommissYear();
                std::cout << "\n[1.5 ВОЗРАСТ ДОМА: " << age << " лет]" << std::endl;
                std::cout << "Рекомендации:" << std::endl;
                std::cout << "  2.0 - Новый дом (до 5 лет), современные коммуникации" << std::endl;
                std::cout << "  1.5 - Не очень старый (до 15 лет), хорошее состояние" << std::endl;
                std::cout << "  1.0 - Средней давности (до 30 лет), требует внимания" << std::endl;
                std::cout << "  0.5 - Старый дом (более 30 лет), возможны проблемы" << std::endl;
                std::cout << "Ваша оценка возраста дома (0.5 - 2.0): ";
                number_filteredInput<double>(userInput, 0, 1);
                if (userInput < 0.5) userInput = 0.5;
                if (userInput > 2.0) userInput = 2.0;
                totalUserRating += userInput;
                maxUserPoints += 2;

                double pricePerSqm = it->getPrice() / it->getArea();
                std::cout << "\n[1.6 ЦЕНА: " << it->getPrice() << " $ (" << pricePerSqm << " $/м2)]" << std::endl;
                std::cout << "Рекомендации:" << std::endl;
                std::cout << "  2.0 - Отличная цена, хорошее соотношение цены и качества" << std::endl;
                std::cout << "  1.5 - Хорошая цена, справедливо" << std::endl;
                std::cout << "  1.0 - Средняя цена, можно договориться" << std::endl;
                std::cout << "  0.5 - Высокая цена, не соответствует качеству" << std::endl;
                std::cout << "Ваша оценка цены (0.5 - 2.0): ";
                number_filteredInput<double>(userInput, 0, 1);
                if (userInput < 0.5) userInput = 0.5;
                if (userInput > 2.0) userInput = 2.0;
                totalUserRating += userInput;
                maxUserPoints += 2;

                std::cout << "\n[2. СУБЪЕКТИВНЫЕ ФАКТОРЫ]" << std::endl;
                std::cout << "==========================" << std::endl;

                std::cout << "\n[2.1 МЕСТОПОЛОЖЕНИЕ: " << it->getAddress() << "]" << std::endl;
                std::cout << "Рекомендации:" << std::endl;
                std::cout << "  2.0 - Отличное расположение, рядом все необходимое" << std::endl;
                std::cout << "  1.5 - Хорошее расположение, удобно" << std::endl;
                std::cout << "  1.0 - Среднее расположение, есть неудобства" << std::endl;
                std::cout << "  0.5 - Неудобное расположение" << std::endl;
                std::cout << "Ваша оценка местоположения (0.5 - 2.0): ";
                number_filteredInput<double>(userInput, 0, 1);
                if (userInput < 0.5) userInput = 0.5;
                if (userInput > 2.0) userInput = 2.0;
                totalUserRating += userInput;
                maxUserPoints += 2;

                std::cout << "\n[2.2 ОБЩЕЕ ВПЕЧАТЛЕНИЕ]" << std::endl;
                std::cout << "Рекомендации:" << std::endl;
                std::cout << "  2.0 - Прекрасная квартира, хочу здесь жить" << std::endl;
                std::cout << "  1.5 - Хорошая квартира, устраивает" << std::endl;
                std::cout << "  1.0 - Средняя квартира, есть недостатки" << std::endl;
                std::cout << "  0.5 - Плохая квартира, не рекомендую" << std::endl;
                std::cout << "Ваша общая оценка (0.5 - 2.0): ";
                number_filteredInput<double>(userInput, 0, 1);
                if (userInput < 0.5) userInput = 0.5;
                if (userInput > 2.0) userInput = 2.0;
                totalUserRating += userInput;
                maxUserPoints += 2;

                double userNormalizedRating = (totalUserRating / maxUserPoints) * 10.0;
                if (userNormalizedRating > 10.0) userNormalizedRating = 10.0;
                userNormalizedRating = std::round(userNormalizedRating * 10.0) / 10.0;

                double systemRating = it->calculateRating();

                it->setUserRating(userNormalizedRating);

                if (dbManager.updateApartmentRating(it->getEst_id(), userNormalizedRating, fullUsername)) {
                    std::cout << "\n[ УСПЕХ ] Рейтинг сохранен в базе данных!" << std::endl;
                }
                else {
                    std::cerr << "\n[ ОШИБКА ] Не удалось сохранить рейтинг в базу данных!" << std::endl;
                }

                std::cout << "\n==================================================" << std::endl;
                std::cout << "          РЕЗУЛЬТАТЫ ОЦЕНКИ               " << std::endl;
                std::cout << "==================================================" << std::endl;
                std::cout << "Ваш рейтинг: " << userNormalizedRating << "/10" << std::endl;
                std::cout << "Системный рейтинг (АИС): " << systemRating << "/10" << std::endl;
                std::cout << "Разница: " << std::abs(systemRating - userNormalizedRating) << " баллов" << std::endl;

                if (userNormalizedRating >= 8.0) {
                    std::cout << "Вердикт: [*][*][*][*][*] ОТЛИЧНАЯ КВАРТИРА!" << std::endl;
                    std::cout << "   Рекомендуем к покупке!" << std::endl;
                }
                else if (userNormalizedRating >= 6.0) {
                    std::cout << "Вердикт: [*][*][*][*][ ] ХОРОШАЯ КВАРТИРА" << std::endl;
                    std::cout << "   Можно рассматривать" << std::endl;
                }
                else if (userNormalizedRating >= 4.0) {
                    std::cout << "Вердикт: [*][*][*][ ][ ] УДОВЛЕТВОРИТЕЛЬНО" << std::endl;
                    std::cout << "   Есть недостатки, но можно рассмотреть" << std::endl;
                }
                else {
                    std::cout << "Вердикт: [*][*][ ][ ][ ] НЕ РЕКОМЕНДУЕТСЯ" << std::endl;
                    std::cout << "   Рекомендуем поискать другие варианты" << std::endl;
                }
                std::cout << "==================================================" << std::endl;

                fullUsername = currentUser.getFullname();

                return userNormalizedRating;
            }
        }

        if (!found) {
            std::cerr << "[ ОШИБКА ] Квартира с ID " << find_id << " не найдена или уже оценена." << std::endl;
        }

        std::cout << "\n--------------------------------------------------" << std::endl;
        std::cout << "ВЫБЕРИТЕ ДЕЙСТВИЕ:" << std::endl;
        std::cout << "  1 - Попробовать другой ID" << std::endl;
        std::cout << "  2 - Завершить оценку и выйти" << std::endl;
        std::cout << "--------------------------------------------------" << std::endl;
        std::cout << "Ваш выбор: ";
        number_filteredInput<int>(choice, 0, 1);

        switch (choice) {
        case 1:
            break;
        case 2:
            validChoice = true;
            break;
        default:
            validChoice = true;
        }

    } while (!validChoice);

    return 0.0;
}
void RealEstateManager::deleteObjectRating(std::string type) {
    if (apartments.empty()) {
        std::cout << "[ СПИСОК КВАРТИР ПУСТ ]\n";
        return;
    }

    const User& currentUser = authSystem.getCurrentUser();
    bool hasSuitableApartments = false;

    if (type == "admin") {
        for (const auto& apartment : apartments) {
            if (apartment.hasUserRating()) {
                hasSuitableApartments = true;
                break;
            }
        }

        if (!hasSuitableApartments) {
            std::cout << "\n[ ВНИМАНИЕ ] Нет квартир с пользовательской оценкой для удаления." << std::endl;
            std::cout << "==================================================" << std::endl;
            return;
        }
    }
    else if (type == "user") {
        std::string currentUsername = currentUser.getFullname();
        for (const auto& apartment : apartments) {
            if (apartment.hasUserRating() && apartment.getFullUsername() == currentUsername) {
                hasSuitableApartments = true;
                break;
            }
        }

        if (!hasSuitableApartments) {
            std::cout << "\n[ ВНИМАНИЕ ] У вас нет оцененных квартир для удаления оценки." << std::endl;
            std::cout << "==================================================" << std::endl;
            return;
        }
    }

    std::string fullUsername;
    int find_id;
    bool validChoice = false;
    int choice;

    do {
        bool found = false;

        if (type == "admin") {
            displayObj();

            std::cout << "\n==================================================" << std::endl;
            std::cout << "        УДАЛЕНИЕ ОЦЕНКИ КВАРТИРЫ                  " << std::endl;
            std::cout << "==================================================" << std::endl;
            std::cout << "--------------------------------------------------" << std::endl;

            std::cout << "\n[ ВВОД ДАННЫХ ]" << std::endl;
            std::cout << "------------------------" << std::endl;
            std::cout << "ID квартиры для удаления оценки: ";
            number_filteredInput<int>(find_id);

            for (auto it = apartments.begin(); it != apartments.end(); ++it) {
                if (find_id == it->getEst_id()) {
                    found = true;

                    std::cout << "\n==================================================" << std::endl;
                    std::cout << "   ИНФОРМАЦИЯ О КВАРТИРЕ ДЛЯ УДАЛЕНИЯ ОЦЕНКИ   " << std::endl;
                    std::cout << "==================================================" << std::endl;
                    std::cout << "ID: " << it->getEst_id() << std::endl;
                    std::cout << "Адрес: " << it->getAddress() << std::endl;
                    std::cout << "Площадь: " << it->getArea() << " м2" << std::endl;
                    std::cout << "Комнат: " << it->getRooms() << std::endl;
                    std::cout << "Цена: " << it->getPrice() << " $" << std::endl;

                    bool hasRating = it->hasUserRating();

                    if (!hasRating) {
                        std::cout << "\n[ ИНФОРМАЦИЯ ] У этой квартиры нет пользовательской оценки." << std::endl;
                        std::cout << "==================================================" << std::endl;

                        std::cout << "\n--------------------------------------------------" << std::endl;
                        std::cout << "ВЫБЕРИТЕ ДЕЙСТВИЕ:" << std::endl;
                        std::cout << "  1 - Попробовать другой ID" << std::endl;
                        std::cout << "  2 - Завершить и выйти" << std::endl;
                        std::cout << "--------------------------------------------------" << std::endl;
                        std::cout << "Ваш выбор: ";
                        number_filteredInput<int>(choice, 0, 1);

                        switch (choice) {
                        case 1:
                            break;
                        case 2:
                            validChoice = true;
                            return;
                        default:
                            validChoice = true;
                            return;
                        }

                        break;
                    }

                    std::cout << "Текущая пользовательская оценка: " << it->getUserRating() << "/10" << std::endl;
                    std::cout << "Системный рейтинг (АИС): " << it->calculateRating() << "/10" << std::endl;
                    std::cout << "==================================================" << std::endl;

                    int confirm;
                    std::cout << "\n[ ПОДТВЕРЖДЕНИЕ УДАЛЕНИЯ ]" << std::endl;
                    std::cout << "Вы уверены, что хотите удалить пользовательскую оценку?" << std::endl;
                    std::cout << "1 - Да, удалить оценку" << std::endl;
                    std::cout << "2 - Нет, оставить оценку" << std::endl;
                    std::cout << "Ваш выбор: ";
                    number_filteredInput<int>(confirm, 0, 1);

                    if (confirm == 1) {
                        it->setUserRating(0.0);

                        std::cout << "\n[ УСПЕХ ] Пользовательская оценка для квартиры с ID "
                            << find_id << " успешно удалена." << std::endl;
                        std::cout << "[ ИНФОРМАЦИЯ ] Теперь будет использоваться только системный рейтинг." << std::endl;

                        return;
                    }
                    else {
                        std::cout << "\n[ ОТМЕНА ] Удаление оценки отменено пользователем." << std::endl;
                        return;
                    }
                }
            }
        }
        else if (type == "user") {
            std::string currentUsername = currentUser.getFullname();

            std::cout << "\n==================================================" << std::endl;
            std::cout << "          ВАШИ ОЦЕНЕННЫЕ КВАРТИРЫ               " << std::endl;
            std::cout << "==================================================" << std::endl;

            bool hasUserApartments = false;
            int counter = 1;
            for (const auto& apartment : apartments) {
                if (apartment.hasUserRating() && apartment.getFullUsername() == currentUsername) {
                    hasUserApartments = true;
                    std::cout << "\n[ КВАРТИРА №" << counter << " ]" << std::endl;
                    std::cout << "--------------------------------------------------" << std::endl;
                    apartment.displayInfo();
                    counter++;
                }
            }

            if (!hasUserApartments) {
                std::cout << "\n[ ВНИМАНИЕ ] У вас нет оцененных квартир." << std::endl;
                std::cout << "==================================================" << std::endl;
                system("pause");
                return;
            }

            system("pause");

            std::cout << "\n==================================================" << std::endl;
            std::cout << "        УДАЛЕНИЕ ОЦЕНКИ КВАРТИРЫ                  " << std::endl;
            std::cout << "==================================================" << std::endl;
            std::cout << "--------------------------------------------------" << std::endl;

            std::cout << "\n[ ВВОД ДАННЫХ ]" << std::endl;
            std::cout << "------------------------" << std::endl;
            std::cout << "ID квартиры для удаления оценки: ";
            number_filteredInput<int>(find_id);

            for (auto it = apartments.begin(); it != apartments.end(); ++it) {
                if (find_id == it->getEst_id() && it->getFullUsername() == currentUsername) {
                    found = true;

                    std::cout << "\n==================================================" << std::endl;
                    std::cout << "   ИНФОРМАЦИЯ О КВАРТИРЕ ДЛЯ УДАЛЕНИЯ ОЦЕНКИ   " << std::endl;
                    std::cout << "==================================================" << std::endl;
                    std::cout << "ID: " << it->getEst_id() << std::endl;
                    std::cout << "Адрес: " << it->getAddress() << std::endl;
                    std::cout << "Площадь: " << it->getArea() << " м2" << std::endl;
                    std::cout << "Комнат: " << it->getRooms() << std::endl;
                    std::cout << "Цена: " << it->getPrice() << " $" << std::endl;

                    bool hasRating = it->hasUserRating();

                    if (!hasRating) {
                        std::cout << "\n[ ИНФОРМАЦИЯ ] У этой квартиры нет пользовательской оценки." << std::endl;
                        std::cout << "==================================================" << std::endl;

                        std::cout << "\n--------------------------------------------------" << std::endl;
                        std::cout << "ВЫБЕРИТЕ ДЕЙСТВИЕ:" << std::endl;
                        std::cout << "  1 - Попробовать другой ID" << std::endl;
                        std::cout << "  2 - Завершить и выйти" << std::endl;
                        std::cout << "--------------------------------------------------" << std::endl;
                        std::cout << "Ваш выбор: ";
                        number_filteredInput<int>(choice, 0, 1);

                        switch (choice) {
                        case 1:
                            break;
                        case 2:
                            validChoice = true;
                            return;
                        default:
                            validChoice = true;
                            return;
                        }

                        break;
                    }

                    std::cout << "Текущая пользовательская оценка: " << it->getUserRating() << "/10" << std::endl;
                    std::cout << "Системный рейтинг (АИС): " << it->calculateRating() << "/10" << std::endl;
                    std::cout << "==================================================" << std::endl;

                    int confirm;
                    std::cout << "\n[ ПОДТВЕРЖДЕНИЕ УДАЛЕНИЯ ]" << std::endl;
                    std::cout << "Вы уверены, что хотите удалить свою оценку этой квартиры?" << std::endl;
                    std::cout << "1 - Да, удалить оценку" << std::endl;
                    std::cout << "2 - Нет, оставить оценку" << std::endl;
                    std::cout << "Ваш выбор: ";
                    number_filteredInput<int>(confirm, 0, 1);

                    if (confirm == 1) {
                        it->setUserRating(0.0);

                        if (dbManager.deleteApartmentRating(find_id)) {
                            std::cout << "\n[ УСПЕХ ] Пользовательская оценка для квартиры с ID " << find_id << " успешно удалена из базы данных." << std::endl;
                        }
                        else {
                            std::cerr << "\n[ ОШИБКА ] Не удалось удалить оценку из базы данных." << std::endl;
                        }

                        return;
                    }
                    else {
                        std::cout << "\n[ ОТМЕНА ] Удаление оценки отменено." << std::endl;
                        return;
                    }
                }
            }
        }

        if (!found) {
            std::cerr << "[ ОШИБКА ] Квартира с ID " << find_id << " не найдена или у вас нет доступа." << std::endl;
        }

        std::cout << "\n--------------------------------------------------" << std::endl;
        std::cout << "ВЫБЕРИТЕ ДЕЙСТВИЕ:" << std::endl;
        std::cout << "  1 - Попробовать другой ID" << std::endl;
        std::cout << "  2 - Завершить удаление и выйти" << std::endl;
        std::cout << "--------------------------------------------------" << std::endl;
        std::cout << "Ваш выбор: ";
        number_filteredInput<int>(choice, 0, 1);

        switch (choice) {
        case 1:
            break;
        case 2:
            validChoice = true;
            break;
        default:
            validChoice = true;
        }

    } while (!validChoice);
}

double RealEstateManager::changeObjectRating(std::string type) {
    if (apartments.empty()) {
        std::cout << "[ СПИСОК КВАРТИР ПУСТ ]\n";
        return 0.0;
    }

    const User& currentUser = authSystem.getCurrentUser();
    bool hasSuitableApartments = false;

    if (type == "admin") {
        for (const auto& apartment : apartments) {
            if (apartment.hasUserRating()) {
                hasSuitableApartments = true;
                break;
            }
        }

        if (!hasSuitableApartments) {
            std::cout << "\n[ ВНИМАНИЕ ] Нет квартир с пользовательской оценкой для изменения." << std::endl;
            std::cout << "==================================================" << std::endl;
            return 0.0;
        }
    }
    else if (type == "user") {
        std::string currentUsername = currentUser.getFullname();
        for (const auto& apartment : apartments) {
            if (apartment.hasUserRating() && apartment.getFullUsername() == currentUsername) {
                hasSuitableApartments = true;
                break;
            }
        }

        if (!hasSuitableApartments) {
            std::cout << "\n[ ВНИМАНИЕ ] У вас нет оцененных квартир для изменения оценки." << std::endl;
            std::cout << "==================================================" << std::endl;
            return 0.0;
        }
    }

    std::string fullUsername;
    int find_id;
    bool validChoice = false;
    int choice;

    do {
        bool found = false;

        if (type == "admin") {
            displayObj();

            std::cout << "\n==================================================" << std::endl;
            std::cout << "    ИЗМЕНЕНИЕ ОЦЕНКИ КВАРТИРЫ ПОЛЬЗОВАТЕЛЕМ    " << std::endl;
            std::cout << "==================================================" << std::endl;
            std::cout << "--------------------------------------------------" << std::endl;

            std::cout << "\n[ ВВОД ДАННЫХ ]" << std::endl;
            std::cout << "------------------------" << std::endl;
            std::cout << "ID квартиры для изменения оценки: ";
            number_filteredInput<int>(find_id);

            for (auto it = apartments.begin(); it != apartments.end(); ++it) {
                if (find_id == it->getEst_id() && it->hasUserRating()) {
                    found = true;

                    std::cout << "\n==================================================" << std::endl;
                    std::cout << "    ОЦЕНКА КВАРТИРЫ ПОЛЬЗОВАТЕЛЕМ    " << std::endl;
                    std::cout << "==================================================" << std::endl;
                    std::cout << "Найдена квартира:" << std::endl;
                    std::cout << "ID: " << it->getEst_id() << std::endl;
                    std::cout << "Адрес: " << it->getAddress() << std::endl;
                    std::cout << "Площадь: " << it->getArea() << " м2" << std::endl;
                    std::cout << "Комнат: " << it->getRooms() << std::endl;
                    std::cout << "Цена: " << it->getPrice() << " $" << std::endl;
                    std::cout << "Год ввода: " << it->getCommissYear() << std::endl;
                    std::cout << "Этаж: " << it->getFloor() << std::endl;
                    std::cout << "Лифт: " << (it->getHasElevator() ? "Да" : "Нет") << std::endl;
                    std::cout << "Системный рейтинг (АИС): " << it->calculateRating() << "/10" << std::endl;
                    if (it->hasUserRating()) {
                        std::cout << "Текущий пользовательский рейтинг: " << it->getUserRating() << "/10" << std::endl;
                    }
                    std::cout << "==================================================" << std::endl;

                    double totalUserRating = 0.0;
                    int maxUserPoints = 0;
                    double userInput = 0.0;

                    std::cout << "\n[1. ОЦЕНКА ПО ПАРАМЕТРАМ КВАРТИРЫ]" << std::endl;
                    std::cout << "====================================" << std::endl;

                    std::cout << "\n[1.1 ПЛОЩАДЬ: " << it->getArea() << " м2]" << std::endl;
                    std::cout << "Рекомендации:" << std::endl;
                    std::cout << "  2.0 - Отличная площадь, просторно (" << it->getArea() / it->getRooms() << " м2/комнату)" << std::endl;
                    std::cout << "  1.5 - Хорошая площадь, комфортно" << std::endl;
                    std::cout << "  1.0 - Средняя площадь, достаточно" << std::endl;
                    std::cout << "  0.5 - Маленькая площадь, тесно" << std::endl;
                    std::cout << "Ваша оценка площади (0.5 - 2.0): ";
                    number_filteredInput<double>(userInput, 0, 1);
                    if (userInput < 0.5) userInput = 0.5;
                    if (userInput > 2.0) userInput = 2.0;
                    totalUserRating += userInput;
                    maxUserPoints += 2;

                    std::cout << "\n[1.2 КОЛИЧЕСТВО КОМНАТ: " << it->getRooms() << "]" << std::endl;
                    std::cout << "Рекомендации:" << std::endl;
                    std::cout << "  2.0 - Идеальное количество для ваших нужд" << std::endl;
                    std::cout << "  1.5 - Хорошее количество, подходит" << std::endl;
                    std::cout << "  1.0 - Среднее количество, можно рассмотреть" << std::endl;
                    std::cout << "  0.5 - Не подходящее количество комнат" << std::endl;
                    std::cout << "Ваша оценка количества комнат (0.5 - 2.0): ";
                    number_filteredInput<double>(userInput, 0, 1);
                    if (userInput < 0.5) userInput = 0.5;
                    if (userInput > 2.0) userInput = 2.0;
                    totalUserRating += userInput;
                    maxUserPoints += 2;

                    std::cout << "\n[1.3 ЭТАЖ: " << it->getFloor() << "]" << std::endl;
                    std::cout << "Рекомендации:" << std::endl;
                    std::cout << "  2.0 - Идеальный этаж (2-5 этаж), удобно" << std::endl;
                    std::cout << "  1.5 - Хороший этаж, нормально" << std::endl;
                    std::cout << "  1.0 - Средний этаж, есть неудобства" << std::endl;
                    std::cout << "  0.5 - Неудобный этаж (1 или очень высокий)" << std::endl;
                    std::cout << "Ваша оценка этажа (0.5 - 2.0): ";
                    number_filteredInput<double>(userInput, 0, 1);
                    if (userInput < 0.5) userInput = 0.5;
                    if (userInput > 2.0) userInput = 2.0;
                    totalUserRating += userInput;
                    maxUserPoints += 2;

                    std::cout << "\n[1.4 ЛИФТ: " << (it->getHasElevator() ? "Да" : "Нет") << "]" << std::endl;
                    std::cout << "Рекомендации:" << std::endl;
                    std::cout << "  2.0 - Лифт есть и это важно для вас" << std::endl;
                    std::cout << "  1.0 - Без лифта, но это не критично" << std::endl;
                    std::cout << "  0.5 - Нет лифта и это проблема" << std::endl;
                    std::cout << "Ваша оценка наличия лифта (0.5 - 2.0): ";
                    number_filteredInput<double>(userInput, 0, 1);
                    if (userInput < 0.5) userInput = 0.5;
                    if (userInput > 2.0) userInput = 2.0;
                    totalUserRating += userInput;
                    maxUserPoints += 2;

                    int age = 2024 - it->getCommissYear();
                    std::cout << "\n[1.5 ВОЗРАСТ ДОМА: " << age << " лет]" << std::endl;
                    std::cout << "Рекомендации:" << std::endl;
                    std::cout << "  2.0 - Новый дом (до 5 лет), современные коммуникации" << std::endl;
                    std::cout << "  1.5 - Не очень старый (до 15 лет), хорошее состояние" << std::endl;
                    std::cout << "  1.0 - Средней давности (до 30 лет), требует внимания" << std::endl;
                    std::cout << "  0.5 - Старый дом (более 30 лет), возможны проблемы" << std::endl;
                    std::cout << "Ваша оценка возраста дома (0.5 - 2.0): ";
                    number_filteredInput<double>(userInput, 0, 1);
                    if (userInput < 0.5) userInput = 0.5;
                    if (userInput > 2.0) userInput = 2.0;
                    totalUserRating += userInput;
                    maxUserPoints += 2;

                    double pricePerSqm = it->getPrice() / it->getArea();
                    std::cout << "\n[1.6 ЦЕНА: " << it->getPrice() << " $ (" << pricePerSqm << " $/м2)]" << std::endl;
                    std::cout << "Рекомендации:" << std::endl;
                    std::cout << "  2.0 - Отличная цена, хорошее соотношение цены и качества" << std::endl;
                    std::cout << "  1.5 - Хорошая цена, справедливо" << std::endl;
                    std::cout << "  1.0 - Средняя цена, можно договориться" << std::endl;
                    std::cout << "  0.5 - Высокая цена, не соответствует качеству" << std::endl;
                    std::cout << "Ваша оценка цены (0.5 - 2.0): ";
                    number_filteredInput<double>(userInput, 0, 1);
                    if (userInput < 0.5) userInput = 0.5;
                    if (userInput > 2.0) userInput = 2.0;
                    totalUserRating += userInput;
                    maxUserPoints += 2;

                    std::cout << "\n[2. СУБЪЕКТИВНЫЕ ФАКТОРЫ]" << std::endl;
                    std::cout << "==========================" << std::endl;

                    std::cout << "\n[2.1 МЕСТОПОЛОЖЕНИЕ: " << it->getAddress() << "]" << std::endl;
                    std::cout << "Рекомендации:" << std::endl;
                    std::cout << "  2.0 - Отличное расположение, рядом все необходимое" << std::endl;
                    std::cout << "  1.5 - Хорошее расположение, удобно" << std::endl;
                    std::cout << "  1.0 - Среднее расположение, есть неудобства" << std::endl;
                    std::cout << "  0.5 - Неудобное расположение" << std::endl;
                    std::cout << "Ваша оценка местоположения (0.5 - 2.0): ";
                    number_filteredInput<double>(userInput, 0, 1);
                    if (userInput < 0.5) userInput = 0.5;
                    if (userInput > 2.0) userInput = 2.0;
                    totalUserRating += userInput;
                    maxUserPoints += 2;

                    std::cout << "\n[2.2 ОБЩЕЕ ВПЕЧАТЛЕНИЕ]" << std::endl;
                    std::cout << "Рекомендации:" << std::endl;
                    std::cout << "  2.0 - Прекрасная квартира, хочу здесь жить" << std::endl;
                    std::cout << "  1.5 - Хорошая квартира, устраивает" << std::endl;
                    std::cout << "  1.0 - Средняя квартира, есть недостатки" << std::endl;
                    std::cout << "  0.5 - Плохая квартира, не рекомендую" << std::endl;
                    std::cout << "Ваша общая оценка (0.5 - 2.0): ";
                    number_filteredInput<double>(userInput, 0, 1);
                    if (userInput < 0.5) userInput = 0.5;
                    if (userInput > 2.0) userInput = 2.0;
                    totalUserRating += userInput;
                    maxUserPoints += 2;

                    double userNormalizedRating = (totalUserRating / maxUserPoints) * 10.0;
                    if (userNormalizedRating > 10.0) userNormalizedRating = 10.0;
                    userNormalizedRating = std::round(userNormalizedRating * 10.0) / 10.0;

                    double systemRating = it->calculateRating();

                    double oldRating = it->getUserRating();
                    std::string oldUsername = it->getFullUsername();
                    it->setUserRating(userNormalizedRating);

                    std::string usernameToSave = it->getFullUsername();
                    if (dbManager.updateApartmentRating(it->getEst_id(), userNormalizedRating, usernameToSave)) {
                        std::cout << "\n[ УСПЕХ ] Рейтинг обновлен в базе данных!" << std::endl;
                        std::cout << "  ID квартиры: " << it->getEst_id() << std::endl;
                        std::cout << "  Новый рейтинг: " << userNormalizedRating << "/10" << std::endl;
                    }
                    else {
                        it->setUserRating(oldRating);
                        std::cerr << "\n[ ОШИБКА БАЗЫ ДАННЫХ ] Не удалось обновить рейтинг в базе данных!" << std::endl;
                        std::cerr << "  Ошибка: " << dbManager.getLastError() << std::endl;

                        std::cout << "\n==================================================" << std::endl;
                        std::cout << "          РЕЗУЛЬТАТЫ ОЦЕНКИ               " << std::endl;
                        std::cout << "==================================================" << std::endl;
                        std::cout << "Ваш рейтинг (не сохранен): " << userNormalizedRating << "/10" << std::endl;
                        std::cout << "Системный рейтинг (АИС): " << systemRating << "/10" << std::endl;
                        std::cout << "Разница: " << std::abs(systemRating - userNormalizedRating) << " баллов" << std::endl;
                        std::cout << "==================================================" << std::endl;

                        return 0.0;
                    }

                    std::cout << "\n==================================================" << std::endl;
                    std::cout << "          РЕЗУЛЬТАТЫ ОЦЕНКИ               " << std::endl;
                    std::cout << "==================================================" << std::endl;
                    std::cout << "Ваш рейтинг: " << userNormalizedRating << "/10" << std::endl;
                    std::cout << "Системный рейтинг (АИС): " << systemRating << "/10" << std::endl;
                    std::cout << "Разница: " << std::abs(systemRating - userNormalizedRating) << " баллов" << std::endl;

                    if (userNormalizedRating >= 8.0) {
                        std::cout << "Вердикт: [*][*][*][*][*] ОТЛИЧНАЯ КВАРТИРА!" << std::endl;
                        std::cout << "   Рекомендуем к покупке!" << std::endl;
                    }
                    else if (userNormalizedRating >= 6.0) {
                        std::cout << "Вердикт: [*][*][*][*][ ] ХОРОШАЯ КВАРТИРА" << std::endl;
                        std::cout << "   Можно рассматривать" << std::endl;
                    }
                    else if (userNormalizedRating >= 4.0) {
                        std::cout << "Вердикт: [*][*][*][ ][ ] УДОВЛЕТВОРИТЕЛЬНО" << std::endl;
                        std::cout << "   Есть недостатки, но можно рассмотреть" << std::endl;
                    }
                    else {
                        std::cout << "Вердикт: [*][*][ ][ ][ ] НЕ РЕКОМЕНДУЕТСЯ" << std::endl;
                        std::cout << "   Рекомендуем поискать другие варианты" << std::endl;
                    }
                    std::cout << "==================================================" << std::endl;

                    return userNormalizedRating;
                }
            }
        }
        else if (type == "user") {
            std::string currentUsername = currentUser.getFullname();

            std::cout << "\n==================================================" << std::endl;
            std::cout << "          ВАШИ ОЦЕНЕННЫЕ КВАРТИРЫ               " << std::endl;
            std::cout << "==================================================" << std::endl;

            bool hasUserApartments = false;
            int counter = 1;
            for (const auto& apartment : apartments) {
                if (apartment.hasUserRating() && apartment.getFullUsername() == currentUsername) {
                    hasUserApartments = true;
                    std::cout << "\n[ КВАРТИРА №" << counter << " ]" << std::endl;
                    std::cout << "--------------------------------------------------" << std::endl;
                    apartment.displayInfo();
                    counter++;
                }
            }

            if (!hasUserApartments) {
                std::cout << "\n[ ВНИМАНИЕ ] У вас нет оцененных квартир." << std::endl;
                std::cout << "==================================================" << std::endl;
                system("pause");
                return 0.0;
            }

            system("pause");

            std::cout << "\n==================================================" << std::endl;
            std::cout << "    ИЗМЕНЕНИЕ ОЦЕНКИ КВАРТИРЫ ПОЛЬЗОВАТЕЛЕМ    " << std::endl;
            std::cout << "==================================================" << std::endl;
            std::cout << "--------------------------------------------------" << std::endl;

            std::cout << "\n[ ВВОД ДАННЫХ ]" << std::endl;
            std::cout << "------------------------" << std::endl;
            std::cout << "ID квартиры для изменения оценки: ";
            number_filteredInput<int>(find_id);

            for (auto it = apartments.begin(); it != apartments.end(); ++it) {
                if (find_id == it->getEst_id() && it->hasUserRating() && it->getFullUsername() == currentUsername) {
                    found = true;

                    std::cout << "\n==================================================" << std::endl;
                    std::cout << "    ОЦЕНКА КВАРТИРЫ ПОЛЬЗОВАТЕЛЕМ    " << std::endl;
                    std::cout << "==================================================" << std::endl;
                    std::cout << "Найдена квартира:" << std::endl;
                    std::cout << "ID: " << it->getEst_id() << std::endl;
                    std::cout << "Адрес: " << it->getAddress() << std::endl;
                    std::cout << "Площадь: " << it->getArea() << " м2" << std::endl;
                    std::cout << "Комнат: " << it->getRooms() << std::endl;
                    std::cout << "Цена: " << it->getPrice() << " $" << std::endl;
                    std::cout << "Год ввода: " << it->getCommissYear() << std::endl;
                    std::cout << "Этаж: " << it->getFloor() << std::endl;
                    std::cout << "Лифт: " << (it->getHasElevator() ? "Да" : "Нет") << std::endl;
                    std::cout << "Системный рейтинг (АИС): " << it->calculateRating() << "/10" << std::endl;
                    if (it->hasUserRating()) {
                        std::cout << "Текущий пользовательский рейтинг: " << it->getUserRating() << "/10" << std::endl;
                    }
                    std::cout << "==================================================" << std::endl;

                    double totalUserRating = 0.0;
                    int maxUserPoints = 0;
                    double userInput = 0.0;

                    std::cout << "\n[1. ОЦЕНКА ПО ПАРАМЕТРАМ КВАРТИРЫ]" << std::endl;
                    std::cout << "====================================" << std::endl;

                    std::cout << "\n[1.1 ПЛОЩАДЬ: " << it->getArea() << " м2]" << std::endl;
                    std::cout << "Рекомендации:" << std::endl;
                    std::cout << "  2.0 - Отличная площадь, просторно (" << it->getArea() / it->getRooms() << " м2/комнату)" << std::endl;
                    std::cout << "  1.5 - Хорошая площадь, комфортно" << std::endl;
                    std::cout << "  1.0 - Средняя площадь, достаточно" << std::endl;
                    std::cout << "  0.5 - Маленькая площадь, тесно" << std::endl;
                    std::cout << "Ваша оценка площади (0.5 - 2.0): ";
                    number_filteredInput<double>(userInput, 0, 1);
                    if (userInput < 0.5) userInput = 0.5;
                    if (userInput > 2.0) userInput = 2.0;
                    totalUserRating += userInput;
                    maxUserPoints += 2;

                    std::cout << "\n[1.2 КОЛИЧЕСТВО КОМНАТ: " << it->getRooms() << "]" << std::endl;
                    std::cout << "Рекомендации:" << std::endl;
                    std::cout << "  2.0 - Идеальное количество для ваших нужд" << std::endl;
                    std::cout << "  1.5 - Хорошее количество, подходит" << std::endl;
                    std::cout << "  1.0 - Среднее количество, можно рассмотреть" << std::endl;
                    std::cout << "  0.5 - Не подходящее количество комнат" << std::endl;
                    std::cout << "Ваша оценка количества комнат (0.5 - 2.0): ";
                    number_filteredInput<double>(userInput, 0, 1);
                    if (userInput < 0.5) userInput = 0.5;
                    if (userInput > 2.0) userInput = 2.0;
                    totalUserRating += userInput;
                    maxUserPoints += 2;

                    std::cout << "\n[1.3 ЭТАЖ: " << it->getFloor() << "]" << std::endl;
                    std::cout << "Рекомендации:" << std::endl;
                    std::cout << "  2.0 - Идеальный этаж (2-5 этаж), удобно" << std::endl;
                    std::cout << "  1.5 - Хороший этаж, нормально" << std::endl;
                    std::cout << "  1.0 - Средний этаж, есть неудобства" << std::endl;
                    std::cout << "  0.5 - Неудобный этаж (1 или очень высокий)" << std::endl;
                    std::cout << "Ваша оценка этажа (0.5 - 2.0): ";
                    number_filteredInput<double>(userInput, 0, 1);
                    if (userInput < 0.5) userInput = 0.5;
                    if (userInput > 2.0) userInput = 2.0;
                    totalUserRating += userInput;
                    maxUserPoints += 2;

                    std::cout << "\n[1.4 ЛИФТ: " << (it->getHasElevator() ? "Да" : "Нет") << "]" << std::endl;
                    std::cout << "Рекомендации:" << std::endl;
                    std::cout << "  2.0 - Лифт есть и это важно для вас" << std::endl;
                    std::cout << "  1.0 - Без лифта, но это не критично" << std::endl;
                    std::cout << "  0.5 - Нет лифта и это проблема" << std::endl;
                    std::cout << "Ваша оценка наличия лифта (0.5 - 2.0): ";
                    number_filteredInput<double>(userInput, 0, 1);
                    if (userInput < 0.5) userInput = 0.5;
                    if (userInput > 2.0) userInput = 2.0;
                    totalUserRating += userInput;
                    maxUserPoints += 2;

                    int age = 2024 - it->getCommissYear();
                    std::cout << "\n[1.5 ВОЗРАСТ ДОМА: " << age << " лет]" << std::endl;
                    std::cout << "Рекомендации:" << std::endl;
                    std::cout << "  2.0 - Новый дом (до 5 лет), современные коммуникации" << std::endl;
                    std::cout << "  1.5 - Не очень старый (до 15 лет), хорошее состояние" << std::endl;
                    std::cout << "  1.0 - Средней давности (до 30 лет), требует внимания" << std::endl;
                    std::cout << "  0.5 - Старый дом (более 30 лет), возможны проблемы" << std::endl;
                    std::cout << "Ваша оценка возраста дома (0.5 - 2.0): ";
                    number_filteredInput<double>(userInput, 0, 1);
                    if (userInput < 0.5) userInput = 0.5;
                    if (userInput > 2.0) userInput = 2.0;
                    totalUserRating += userInput;
                    maxUserPoints += 2;

                    double pricePerSqm = it->getPrice() / it->getArea();
                    std::cout << "\n[1.6 ЦЕНА: " << it->getPrice() << " $ (" << pricePerSqm << " $/м2)]" << std::endl;
                    std::cout << "Рекомендации:" << std::endl;
                    std::cout << "  2.0 - Отличная цена, хорошее соотношение цены и качества" << std::endl;
                    std::cout << "  1.5 - Хорошая цена, справедливо" << std::endl;
                    std::cout << "  1.0 - Средняя цена, можно договориться" << std::endl;
                    std::cout << "  0.5 - Высокая цена, не соответствует качеству" << std::endl;
                    std::cout << "Ваша оценка цены (0.5 - 2.0): ";
                    number_filteredInput<double>(userInput, 0, 1);
                    if (userInput < 0.5) userInput = 0.5;
                    if (userInput > 2.0) userInput = 2.0;
                    totalUserRating += userInput;
                    maxUserPoints += 2;

                    std::cout << "\n[2. СУБЪЕКТИВНЫЕ ФАКТОРЫ]" << std::endl;
                    std::cout << "==========================" << std::endl;

                    std::cout << "\n[2.1 МЕСТОПОЛОЖЕНИЕ: " << it->getAddress() << "]" << std::endl;
                    std::cout << "Рекомендации:" << std::endl;
                    std::cout << "  2.0 - Отличное расположение, рядом все необходимое" << std::endl;
                    std::cout << "  1.5 - Хорошее расположение, удобно" << std::endl;
                    std::cout << "  1.0 - Среднее расположение, есть неудобства" << std::endl;
                    std::cout << "  0.5 - Неудобное расположение" << std::endl;
                    std::cout << "Ваша оценка местоположения (0.5 - 2.0): ";
                    number_filteredInput<double>(userInput, 0, 1);
                    if (userInput < 0.5) userInput = 0.5;
                    if (userInput > 2.0) userInput = 2.0;
                    totalUserRating += userInput;
                    maxUserPoints += 2;

                    std::cout << "\n[2.2 ОБЩЕЕ ВПЕЧАТЛЕНИЕ]" << std::endl;
                    std::cout << "Рекомендации:" << std::endl;
                    std::cout << "  2.0 - Прекрасная квартира, хочу здесь жить" << std::endl;
                    std::cout << "  1.5 - Хорошая квартира, устраивает" << std::endl;
                    std::cout << "  1.0 - Средняя квартира, есть недостатки" << std::endl;
                    std::cout << "  0.5 - Плохая квартира, не рекомендую" << std::endl;
                    std::cout << "Ваша общая оценка (0.5 - 2.0): ";
                    number_filteredInput<double>(userInput, 0, 1);
                    if (userInput < 0.5) userInput = 0.5;
                    if (userInput > 2.0) userInput = 2.0;
                    totalUserRating += userInput;
                    maxUserPoints += 2;

                    double userNormalizedRating = (totalUserRating / maxUserPoints) * 10.0;
                    if (userNormalizedRating > 10.0) userNormalizedRating = 10.0;
                    userNormalizedRating = std::round(userNormalizedRating * 10.0) / 10.0;

                    double systemRating = it->calculateRating();

                    double oldRating = it->getUserRating();
                    it->setUserRating(userNormalizedRating);
                    if (dbManager.updateApartmentRating(it->getEst_id(), userNormalizedRating, currentUsername)) {
                        std::cout << "\n[ УСПЕХ ] Ваш рейтинг обновлен в базе данных!" << std::endl;
                        std::cout << "  ID квартиры: " << it->getEst_id() << std::endl;
                        std::cout << "  Новый рейтинг: " << userNormalizedRating << "/10" << std::endl;
                    }
                    else {
                        it->setUserRating(oldRating);
                        std::cerr << "\n[ ОШИБКА БАЗЫ ДАННЫХ ] Не удалось обновить рейтинг в базе данных!" << std::endl;
                        std::cerr << "  Ошибка: " << dbManager.getLastError() << std::endl;

                        std::cout << "\n==================================================" << std::endl;
                        std::cout << "          РЕЗУЛЬТАТЫ ОЦЕНКИ               " << std::endl;
                        std::cout << "==================================================" << std::endl;
                        std::cout << "Ваш рейтинг (не сохранен): " << userNormalizedRating << "/10" << std::endl;
                        std::cout << "Системный рейтинг (АИС): " << systemRating << "/10" << std::endl;
                        std::cout << "Разница: " << std::abs(systemRating - userNormalizedRating) << " баллов" << std::endl;
                        std::cout << "==================================================" << std::endl;

                        return 0.0;
                    }

                    std::cout << "\n==================================================" << std::endl;
                    std::cout << "          РЕЗУЛЬТАТЫ ОЦЕНКИ               " << std::endl;
                    std::cout << "==================================================" << std::endl;
                    std::cout << "Ваш рейтинг: " << userNormalizedRating << "/10" << std::endl;
                    std::cout << "Системный рейтинг (АИС): " << systemRating << "/10" << std::endl;
                    std::cout << "Разница: " << std::abs(systemRating - userNormalizedRating) << " баллов" << std::endl;

                    if (userNormalizedRating >= 8.0) {
                        std::cout << "Вердикт: [*][*][*][*][*] ОТЛИЧНАЯ КВАРТИРА!" << std::endl;
                        std::cout << "   Рекомендуем к покупке!" << std::endl;
                    }
                    else if (userNormalizedRating >= 6.0) {
                        std::cout << "Вердикт: [*][*][*][*][ ] ХОРОШАЯ КВАРТИРА" << std::endl;
                        std::cout << "   Можно рассматривать" << std::endl;
                    }
                    else if (userNormalizedRating >= 4.0) {
                        std::cout << "Вердикт: [*][*][*][ ][ ] УДОВЛЕТВОРИТЕЛЬНО" << std::endl;
                        std::cout << "   Есть недостатки, но можно рассмотреть" << std::endl;
                    }
                    else {
                        std::cout << "Вердикт: [*][*][ ][ ][ ] НЕ РЕКОМЕНДУЕТСЯ" << std::endl;
                        std::cout << "   Рекомендуем поискать другие варианты" << std::endl;
                    }
                    std::cout << "==================================================" << std::endl;

                    return userNormalizedRating;
                }
            }
        }

        if (!found) {
            std::cerr << "[ ОШИБКА ] Квартира с ID " << find_id << " не найдена или у вас нет доступа." << std::endl;
        }

        std::cout << "\n--------------------------------------------------" << std::endl;
        std::cout << "ВЫБЕРИТЕ ДЕЙСТВИЕ:" << std::endl;
        std::cout << "  1 - Попробовать другой ID" << std::endl;
        std::cout << "  2 - Завершить изменение оценки и выйти" << std::endl;
        std::cout << "--------------------------------------------------" << std::endl;
        std::cout << "Ваш выбор: ";
        number_filteredInput<int>(choice, 0, 1);

        switch (choice) {
        case 1:
            break;
        case 2:
            validChoice = true;
            break;
        default:
            validChoice = true;
        }

    } while (!validChoice);

    return 0.0;
}

void RealEstateManager::displayUserObj()
{
    if (apartments.empty()) {
        std::cout << "[ СПИСОК КВАРТИР ПУСТ ]\n";
        system("pause");
        return;
    }

    const User& currentUser = authSystem.getCurrentUser();
    bool hasSuitableApartments = false;

    std::string currentUsername = currentUser.getFullname();

    for (const auto& apartment : apartments) {
        if (apartment.hasUserRating() && apartment.getFullUsername() == currentUsername) {
            hasSuitableApartments = true;
            break;
        }
    }

    if (!hasSuitableApartments) {
        std::cout << "\n[ ВНИМАНИЕ ] У вас нет оцененных квартир." << std::endl;
        std::cout << "==================================================" << std::endl;
        system("pause");
        return;
    }

    std::cout << "\n==================================================" << std::endl;
    std::cout << "          ВАШИ ОЦЕНЕННЫЕ КВАРТИРЫ               " << std::endl;
    std::cout << "==================================================" << std::endl;

    int counter = 1;
    int totalRatedApartments = 0;

    for (const auto& apartment : apartments) {
        if (apartment.hasUserRating() && apartment.getFullUsername() == currentUsername) {
            totalRatedApartments++;
            std::cout << "\n[ КВАРТИРА №" << counter << " ]" << std::endl;
            std::cout << "--------------------------------------------------" << std::endl;
            apartment.displayInfo();
            counter++;
        }
    }

    std::cout << "\n==================================================" << std::endl;
    std::cout << "[ СТАТИСТИКА ]" << std::endl;
    std::cout << "  Всего квартир в базе: " << apartments.size() << std::endl;
    std::cout << "  Ваших оцененных квартир: " << totalRatedApartments << std::endl;

    std::cout << "==================================================" << std::endl;
}

void RealEstateManager::filterObj() {
    if (apartments.empty()) {
        std::cout << "\n[ ВНИМАНИЕ ] База данных квартир пуста!" << std::endl;
        system("pause");
        return;
    }

    bool defaultParams[9] = { false };

    showFilteredResults(defaultParams);
}

void RealEstateManager::showFilteredResults(bool selectedParams[]) {
    std::cout << "\n==================================================" << std::endl;
    std::cout << "          ОТФИЛЬТРОВАННЫЕ КВАРТИРЫ                " << std::endl;
    std::cout << "==================================================" << std::endl;
    bool anyParamSelected = false;

    for (int i = 0; i < 9; i++) {
        if (selectedParams[i]) {
            anyParamSelected = true;
        }
    }

    if (!anyParamSelected) {
        std::cout << "Не выбрано ни одного параметра!" << std::endl;
        std::cout << "Будет показан только ID квартир." << std::endl;
    }

    int counter = 1;
    for (const auto& apartment : apartments) {
        std::cout << "\n[ КВАРТИРА №" << counter << " ]" << std::endl;
        std::cout << "--------------------------------------------------" << std::endl;

        std::cout << "ID: " << apartment.getEst_id() << std::endl;

        if (selectedParams[0]) {
            std::cout << "Адрес: " << apartment.getAddress() << std::endl;
        }
        if (selectedParams[1]) {
            std::cout << "Площадь: " << apartment.getArea() << " м2" << std::endl;
        }
        if (selectedParams[2]) {
            std::cout << "Комнат: " << apartment.getRooms() << std::endl;
        }
        if (selectedParams[3]) {
            std::cout << "Цена: " << apartment.getPrice() << " $" << std::endl;
        }
        if (selectedParams[4]) {
            std::cout << "Год ввода: " << apartment.getCommissYear() << std::endl;
        }
        if (selectedParams[5]) {
            std::cout << "Этаж: " << apartment.getFloor() << std::endl;
        }
        if (selectedParams[6]) {
            std::cout << "Лифт: " << (apartment.getHasElevator() ? "Да" : "Нет") << std::endl;
        }
        if (selectedParams[7]) {
            std::cout << "Рейтинг АИС: " << apartment.calculateRating() << "/10" << std::endl;
        }
        if (selectedParams[8]) {
            if (apartment.hasUserRating()) {
                std::cout << "Рейтинг пользователя: " << apartment.getUserRating() << "/10" << std::endl;
            }
            else {
                std::cout << "Рейтинг пользователя: нет оценки" << std::endl;
            }
        }
        counter++;
    }

    std::cout << "\n==================================================" << std::endl;
    std::cout << "[ ИТОГО ] Показано квартир: " << apartments.size() << std::endl;
    std::cout << "==================================================" << std::endl;

    system("pause");
}
void RealEstateManager::sortByRating() {
    if (apartments.empty()) {
        std::cout << "\n[ ВНИМАНИЕ ] База данных квартир пуста!" << std::endl;
        system("pause");
        return;
    }
    if (apartments.size() < 2) {
        std::cout << "\n[ ИНФОРМАЦИЯ ] В базе только " << apartments.size()
            << " квартира. Сортировка не требуется." << std::endl;
        system("pause");
        return;
    }

    std::vector<Apartment> sortedApartments = apartments;

    int n = sortedApartments.size();

    std::cout << "\n==================================================" << std::endl;
    std::cout << "     СОРТИРОВКА КВАРТИР ПО РЕЙТИНГУ АИС          " << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << "Метод: пузырьковая сортировка (по убыванию)" << std::endl;
    std::cout << "Количество элементов для сортировки: " << n << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            double rating1 = sortedApartments[j].calculateRating();
            double rating2 = sortedApartments[j + 1].calculateRating();
            if (rating1 < rating2) {
                std::swap(sortedApartments[j], sortedApartments[j + 1]);
            }
        }
    }

    std::cout << "\n[ УСПЕХ ] Сортировка завершена!" << std::endl;
    std::cout << "==================================================" << std::endl;

    displaySortedApartments(sortedApartments);
    system("pause");
}

void RealEstateManager::displaySortedApartments(const std::vector<Apartment>& sortedApartments) {
    std::cout << "\n==================================================" << std::endl;
    std::cout << "     ОТСОРТИРОВАННЫЕ КВАРТИРЫ ПО РЕЙТИНГУ        " << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << "Порядок: от наибольшего рейтинга к наименьшему" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;

    int counter = 1;
    for (const auto& apartment : sortedApartments) {
        std::cout << "\n[ КВАРТИРА №" << counter << " ]" << std::endl;
        std::cout << "--------------------------------------------------" << std::endl;
        std::cout << "Рейтинг АИС: " << apartment.calculateRating() << "/10" << std::endl;
        std::cout << "ID: " << apartment.getEst_id() << std::endl;
        std::cout << "Адрес: " << apartment.getAddress() << std::endl;
        std::cout << "Площадь: " << apartment.getArea() << " м2" << std::endl;
        std::cout << "Комнат: " << apartment.getRooms() << std::endl;
        std::cout << "Цена: " << apartment.getPrice() << " $" << std::endl;

        counter++;
    }
}
void RealEstateManager::loadFromDatabase() {
    apartments = dbManager.loadApartments();

    if (!apartments.empty()) {
        int maxId = 0;
        for (const auto& apartment : apartments) {
            if (apartment.getEst_id() > maxId) {
                maxId = apartment.getEst_id();
            }
        }
        RealEstate::est_count = maxId + 1;
    }
}