#include "functions.h"
#include <iostream>
#include <string>
#include <vector>

size_t User::count = 0;

// Hash
int Hash::receivingExistCodes(int x) {
    x += 256;
    while (!(((x <= 57) && (x >= 48)) || ((x <= 90) && (x >= 65)) || ((x <= 122) && (x >= 97)))) {
        if (x < 48) { x += 24; }
        else { x -= 47; }
    }
    return x;
}

std::string Hash::GetHash(std::string str, unsigned hashLength) {
    hash.clear();

    if (hashLength < 4) {
        return "";
    }
    std::hash<std::string> hasher;
    size_t hashValue = hasher(str + "s@ltS1gn");

    for (unsigned i = 0; i < hashLength; i++) {
        int code = (hashValue >> (i * 8)) % 256;
        hash += receivingExistCodes(code);
        if (hash.size() >= hashLength) break;
    }

    return hash;
}

int Hash::GetControlSum(std::string str) {
    unsigned sault = 0;
    for (size_t i = 0; i < str.size(); i++) {
        sault += static_cast<unsigned char>(str[i]);
    }
    return sault;
}

// User
User::User() : fullname(""), username(""), passwordHash(""), isBlocked(false), id(count++) {}
User::User(const User& other) : fullname(other.fullname), username(other.username), passwordHash(other.passwordHash), isBlocked(other.isBlocked), id(other.id) {}
User::User(std::string fullname) : User() { this->fullname = fullname; }
User::User(std::string fullname, std::string username) : User(fullname) { this->username = username; }
User::User(std::string fullname, std::string username, std::string passwordHash) : User(fullname, username) { this->passwordHash = passwordHash; }
User::User(std::string fullname, std::string username, std::string passwordHash, bool isBlocked) : User(fullname, username, passwordHash) { this->isBlocked = isBlocked; }
User::~User() {}

std::string User::getFullname() const { return fullname; }
std::string User::getUsername() const { return username; }
std::string User::getPasswordHash() const { return passwordHash; }
bool User::getIsBlocked() const { return isBlocked; }
int User::getId() const { return id; }

void User::setFullname(const std::string& fullname) { this->fullname = fullname; }
void User::setUsername(const std::string& username) { this->username = username; }
void User::setPasswordHash(const std::string& passwordHash) { this->passwordHash = passwordHash; }
void User::setIsBlocked(bool isBlocked) { this->isBlocked = isBlocked; }

// AuthSystem
AuthSystem::AuthSystem() {}
AuthSystem::~AuthSystem() {}

bool AuthSystem::userExists(std::string& username) {
    for (char& c : username) {
        c = std::tolower((char)c);
    }
    for (const auto& container : users) {
        if (container.getUsername() == username) {
            return true;
        }
    }
    return false;
}
bool AuthSystem::registerUser() {
    std::string fullname, username, password;

    std::cout << "\nРЕГИСТРАЦИЯ ПОЛЬЗОВАТЕЛЯ:\n\n";

    std::cout << "Ваше имя: ";
    letter_filteredInput<std::string>(fullname, 0, 1);

    std::cout << "Логин: ";
    letter_filteredInput<std::string>(username);

    if (userExists(username)) {
        std::cerr << "\nОшибка: Пользователь с таким логином уже существует!\n";
        system("pause");
        return false;
    }
    letter_filteredInput<std::string>(password, 0, 0, 1, 1);

    std::string hashedPassword = passwordHasher.GetHash(password, 16);
    User newUser(fullname, username, hashedPassword, false);
    users.emplace_back(newUser);

    currentUsername = username;
    currentUserData = newUser;

    std::cout << "\nПользователь успешно зарегистрирован и вошел в систему!" << std::endl;
    std::cout << "ID: " << newUser.getId() << std::endl;
    std::cout << "Добро пожаловать, " << fullname << "!\n";
    system("pause");
    return true;
}

bool AuthSystem::loginUser() {
    std::string username, password;
    bool login_flag = false;
    bool password_flag = false;

    std::cout << "\nВХОД В АККАУНТ:\n\n";

    do {
        std::cout << "Логин: ";
        letter_filteredInput<std::string>(username);
        if (userExists(username)) {
            for (auto& container : users) {
                if (container.getIsBlocked()) {
                    std::cerr << "\nОшибка: Аккаунт заблокирован!\n";
                    system("pause");
                    return false;
                }
                login_flag = true;
                break;
            }
        }
        else {
            std::cerr << "\nОшибка: Пользователя с таким логином не существует!\n";
            unsigned login_choice;
            std::cout << "\nВведите номер действия:\n";
            std::cout << "1 - Ввести логин повторно\n";
            std::cout << "2 - Выйти\n";
            number_filteredInput<unsigned>(login_choice);
            switch (login_choice) {
            case 1:
                break;
            case 2:
                return false;
            default:
                return false;
            }
        }
    } while (!login_flag);

    do {
        std::cout << "Пароль: ";
        letter_filteredInput<std::string>(password, 0, 0, 1);
        std::string hashedPassword = passwordHasher.GetHash(password, 16);
        bool password_correct = false;
        for (auto& container : users) {
            if (container.getUsername() == username && container.getPasswordHash() == hashedPassword) {
                password_correct = true;
                password_flag = true;
                currentUsername = username;
                currentUserData = container;
                std::cout << "\nУспешный вход!\n";
                std::cout << "Добро пожаловать, " << container.getFullname() << "!\n";
                system("pause");
                return true;
            }
        }

        if (!password_correct) {
            std::cerr << "\nОшибка: Неверный пароль!\n";
            unsigned password_choice;
            std::cout << "\nВведите номер действия:\n";
            std::cout << "1 - Ввести пароль повторно\n";
            std::cout << "2 - Выйти\n";
            number_filteredInput<unsigned>(password_choice);
            switch (password_choice) {
            case 1:
                break;
            case 2:
                return false;
            default:
                return false;
            }
        }
    } while (!password_flag);

    return false;
}