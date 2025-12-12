#include "functions.h"
#include <iostream>
#include <string>
#include <vector>

size_t User::count = 0;

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
void User::setId(int id) { this->id = id; }

void User::displayInfo() const
{
    std::cout << "=== Ïîëüçîâàòåëü ===" << std::endl;
    std::cout << "ID: " << id << std::endl;
    std::cout << "Èìÿ: " << fullname << std::endl;
    std::cout << "Ëîãèí: " << username << std::endl;
    std::cout << "Ïàðîëü â çàøèâðîâàííîì âèäå: " << passwordHash << std::endl;
    std::cout << "Çàáëîêèðîâàí: " << (isBlocked ? "Äà" : "Íåò") << std::endl;
}

AuthSystem::AuthSystem() : dbManager(nullptr) 
{
    dbManager = new DatabaseManager("database/mydatabase.db");
    loadAllData();
}
AuthSystem::~AuthSystem()
{
    saveAllData();
    if (dbManager) {
        delete dbManager;
        dbManager = nullptr;
    }
}

bool AuthSystem::saveAllData() {
    bool usersSaved = dbManager->saveUsers(users);
    return usersSaved;
}
bool AuthSystem::loadAllData() {
    users = dbManager->loadUsers();
    return true;
}

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

    std::cout << "\nÐÅÃÈÑÒÐÀÖÈß ÏÎËÜÇÎÂÀÒÅËß:\n\n";

    std::cout << "Âàøå èìÿ: ";
    letter_filteredInput<std::string>(fullname, 0, 1);

    std::cout << "Ëîãèí: ";
    letter_filteredInput<std::string>(username);

    if (userExists(username)) {
        std::cerr << "\nÎøèáêà: Ïîëüçîâàòåëü ñ òàêèì ëîãèíîì óæå ñóùåñòâóåò!\n";
        system("pause");
        return false;
    }
    letter_filteredInput<std::string>(password, 0, 0, 1, 1);

    std::string hashedPassword = passwordHasher.GetHash(password, 16);
    User newUser(fullname, username, hashedPassword, false);
    users.emplace_back(newUser);

    if (dbManager) {
        dbManager->saveUser(newUser);
    }

    currentUsername = username;
    currentUserData = newUser;

    std::cout << "\nÏîëüçîâàòåëü óñïåøíî çàðåãèñòðèðîâàí è âîøåë â ñèñòåìó!" << std::endl;
    std::cout << "ID: " << newUser.getId() << std::endl;
    std::cout << "Äîáðî ïîæàëîâàòü, " << fullname << "!\n";
    system("pause");
    return true;
}

bool AuthSystem::loginUser() {
    std::string username, password;
    bool login_flag = false;
    bool password_flag = false;

    std::cout << "\nÂÕÎÄ Â ÀÊÊÀÓÍÒ:\n\n";

    do {
        std::cout << "Ëîãèí: ";
        letter_filteredInput<std::string>(username);
        if (userExists(username)) {
            for (auto& container : users) {
                if (container.getIsBlocked()) {
                    std::cerr << "\nÎøèáêà: Àêêàóíò çàáëîêèðîâàí!\n";
                    system("pause");
                    return false;
                }
                login_flag = true;
                break;
            }
        }
        else {
            std::cerr << "\nÎøèáêà: Ïîëüçîâàòåëÿ ñ òàêèì ëîãèíîì íå ñóùåñòâóåò!\n";
            unsigned login_choice;
            std::cout << "\nÂâåäèòå íîìåð äåéñòâèÿ:\n";
            std::cout << "1 - Ââåñòè ëîãèí ïîâòîðíî\n";
            std::cout << "2 - Âûéòè\n";
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
        std::cout << "Ïàðîëü: ";
        letter_filteredInput<std::string>(password, 0, 0, 1);
        std::string hashedPassword = passwordHasher.GetHash(password, 16);
        bool password_correct = false;
        for (auto& container : users) {
            if (container.getUsername() == username && container.getPasswordHash() == hashedPassword) {
                password_correct = true;
                password_flag = true;
                currentUsername = username;
                currentUserData = container;
                std::cout << "\nÓñïåøíûé âõîä!\n";
                std::cout << "Äîáðî ïîæàëîâàòü, " << container.getFullname() << "!\n";
                system("pause");
                return true;
            }
        }

        if (!password_correct) {
            std::cerr << "\nÎøèáêà: Íåâåðíûé ïàðîëü!\n";
            unsigned password_choice;
            std::cout << "\nÂâåäèòå íîìåð äåéñòâèÿ:\n";
            std::cout << "1 - Ââåñòè ïàðîëü ïîâòîðíî\n";
            std::cout << "2 - Âûéòè\n";
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

void AuthSystem::displayUserInfo()
{
    std::cout << "\n==================================================" << std::endl;
    std::cout << "          ÏÐÎÑÌÎÒÐ ÑÏÈÑÊÀ ÏÎËÜÇÎÂÀÒÅËÅÉ          " << std::endl;
    std::cout << "==================================================" << std::endl;

    if (users.empty()) {
        std::cout << "\n[ ÂÍÈÌÀÍÈÅ ] Áàçà äàííûõ ïîëüçîâàòåëåé ïóñòà!" << std::endl;
        std::cout << "  Èñïîëüçóéòå ôóíêöèþ ðåãèñòðàöèè ïîëüçîâàòåëåé." << std::endl;
        std::cout << "==================================================" << std::endl;
        return;
    }

    std::cout << "\n[ ÑÒÀÒÈÑÒÈÊÀ ]" << std::endl;
    std::cout << "  Âñåãî Ïîëüçîâàòåëåé â áàçå: " << users.size() << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;

    int counter = 1;
    for (const auto& user : users) {
        std::cout << "\n[ ÏÎËÜÇÎÂÀÒÅËÜ ¹" << counter << " ]" << std::endl;
        std::cout << "--------------------------------------------------" << std::endl;
        user.displayInfo();
        counter++;
    }
}

void AuthSystem::deleteUser()
{
    if (!users.empty()) {

        displayUserInfo();
        system("pause");

        std::cout << "\n==================================================" << std::endl;
        std::cout << "          ÓÄÀËÅÍÈÅ ÏÎËÜÇÎÂÀÒÅËß ÈÇ ÈÑÏÈÑÊÀ          " << std::endl;
        std::cout << "==================================================" << std::endl;

        int delete_id;
        bool validChoice = false;
        int choice;

        do {
            std::cout << "\n[ ÂÂÎÄ ÄÀÍÍÛÕ ]" << std::endl;
            std::cout << "------------------------" << std::endl;
            std::cout << "ID äëÿ óäàëåíèÿ: ";
            number_filteredInput<int>(delete_id);

            bool found = false;
            for (auto it = users.begin(); it != users.end(); it++) {
                if (delete_id == it->getId()) {
                    users.erase(it);

                    if (dbManager) {
                        dbManager->deleteUser(delete_id);
                    }

                    found = true;
                    std::cout << "[ ÓÑÏÅÕ ] Ïîëüçîâàòåëü ñ ID " << delete_id << " óäàëåí." << std::endl;
                    break;
                }
            }

            if (!found) {
                std::cerr << "[ ÎØÈÁÊÀ ] Ïîëüçîâàòåëü ñ ID " << delete_id << " íå íàéäåí." << std::endl;
            }

            std::cout << "\n--------------------------------------------------" << std::endl;
            std::cout << "ÂÛÁÅÐÈÒÅ ÄÅÉÑÒÂÈÅ:" << std::endl;
            std::cout << "  1 - Ïðîäîëæèòü óäàëåíèå ïî id" << std::endl;
            std::cout << "  2 - Çàâåðøèòü óäàëåíèå è âûéòè" << std::endl;
            std::cout << "--------------------------------------------------" << std::endl;
            std::cout << "Âàø âûáîð: ";
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
        std::cout << "\n[ ÈÍÔÎÐÌÀÖÈß ] Óäàëåíèå ïîëüçîâàòåëåé çàâåðøåíî." << std::endl;
        std::cout << "  Âñåãî ïîëüçîâàòåëåé îñòàëîñü: " << users.size() << std::endl;
        system("pause");
    }
    else {
        std::cout << "[ ÑÏÈÑÎÊ ÏÎËÜÇÎÂÀÒÅËÅÉ ÏÓÑÒ ]\n";
        system("pause");
    }
}

void AuthSystem::blockUser()
{
    if (!users.empty()) {

        displayUserInfo();
        system("pause");

        std::cout << "\n==================================================" << std::endl;
        std::cout << "          ÁËÎÊÈÐÎÂÊÀ ÏÎËÜÇÎÂÀÒÅËß ÈÇ ÈÑÏÈÑÊÀ          " << std::endl;
        std::cout << "==================================================" << std::endl;

        int block_id;
        bool validChoice = false;
        int choice;

        do {
            std::cout << "\n[ ÂÂÎÄ ÄÀÍÍÛÕ ]" << std::endl;
            std::cout << "------------------------" << std::endl;
            std::cout << "ID äëÿ áëîêèðîâêè: ";
            number_filteredInput<int>(block_id);

            bool found = false;
            for (auto it = users.begin(); it != users.end(); it++) {
                if (block_id == it->getId() && it->getIsBlocked() == false) {
                    it->setIsBlocked(true);
                    found = true;
                    std::cout << "[ ÓÑÏÅÕ ] Ïîëüçîâàòåëü ñ ID " << block_id << " çàáëîêèðîâàí." << std::endl;
                    break;
                }
            }

            if (!found) {
                std::cerr << "[ ÎØÈÁÊÀ ] Ïîëüçîâàòåëü ñ ID " << block_id << " íå íàéäåí èëè óæå çàáëîêèðîâàí." << std::endl;
            }

            std::cout << "\n--------------------------------------------------" << std::endl;
            std::cout << "ÂÛÁÅÐÈÒÅ ÄÅÉÑÒÂÈÅ:" << std::endl;
            std::cout << "  1 - Ïðîäîëæèòü áëîêèðîâêó ïî id" << std::endl;
            std::cout << "  2 - Çàâåðøèòü áëîêèðîâêó è âûéòè" << std::endl;
            std::cout << "--------------------------------------------------" << std::endl;
            std::cout << "Âàø âûáîð: ";
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
        std::cout << "\n[ ÈÍÔÎÐÌÀÖÈß ] Áëîêèðîâêà ïîëüçîâàòåëåé çàâåðøåíà." << std::endl;
        std::cout << "  Âñåãî ïîëüçîâàòåëåé: " << users.size() << std::endl;
        system("pause");
    }
    else {
        std::cout << "[ ÑÏÈÑÎÊ ÏÎËÜÇÎÂÀÒÅËÅÉ ÏÓÑÒ ]\n";
        system("pause");
    }
}

void AuthSystem::unblockUser()
{
    if (!users.empty()) {

        displayUserInfo();
        system("pause");

        std::cout << "\n==================================================" << std::endl;
        std::cout << "       ÐÀÇÁËÎÊÈÐÎÂÊÀ ÏÎËÜÇÎÂÀÒÅËß ÈÇ ÈÑÏÈÑÊÀ          " << std::endl;
        std::cout << "==================================================" << std::endl;

        int block_id;
        bool validChoice = false;
        int choice;

        do {
            std::cout << "\n[ ÂÂÎÄ ÄÀÍÍÛÕ ]" << std::endl;
            std::cout << "------------------------" << std::endl;
            std::cout << "ID äëÿ ðàçáëîêèðîâêè: ";
            number_filteredInput<int>(block_id);

            bool found = false;
            for (auto it = users.begin(); it != users.end(); it++) {
                if (block_id == it->getId() && it->getIsBlocked() == true) {
                    it->setIsBlocked(false);
                    found = true;
                    std::cout << "[ ÓÑÏÅÕ ] Ïîëüçîâàòåëü ñ ID " << block_id << " ðàçáëîêèðîâàí." << std::endl;
                    break;
                }
            }

            if (!found) {
                std::cerr << "[ ÎØÈÁÊÀ ] Ïîëüçîâàòåëü ñ ID " << block_id << " íå íàéäåí èëè íå çàáëîêèðîâàí." << std::endl;
            }

            std::cout << "\n--------------------------------------------------" << std::endl;
            std::cout << "ÂÛÁÅÐÈÒÅ ÄÅÉÑÒÂÈÅ:" << std::endl;
            std::cout << "  1 - Ïðîäîëæèòü ðàçáëîêèðîâêó ïî id" << std::endl;
            std::cout << "  2 - Çàâåðøèòü ðàçáëîêèðîâêó è âûéòè" << std::endl;
            std::cout << "--------------------------------------------------" << std::endl;
            std::cout << "Âàø âûáîð: ";
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
        std::cout << "\n[ ÈÍÔÎÐÌÀÖÈß ] Ðàçáëîêèðîâêà ïîëüçîâàòåëåé çàâåðøåíà." << std::endl;
        std::cout << "  Âñåãî ïîëüçîâàòåëåé: " << users.size() << std::endl;
        system("pause");
    }
    else {
        std::cout << "[ ÑÏÈÑÎÊ ÏÎËÜÇÎÂÀÒÅËÅÉ ÏÓÑÒ ]\n";
        system("pause");
    }
}
