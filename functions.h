#pragma once
#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include "input_check.h"

class Hash;
class User;
class AuthSystem;

class Hash {
    std::string hash;

    inline int receivingExistCodes(int x);
public:
    inline std::string GetHash(std::string str, unsigned hashLength);
    inline int GetControlSum(std::string str);
};

class User {
    std::string fullname;
    std::string username;
    std::string passwordHash;
    bool isBlocked;
    int id;
    static size_t count;
public:
    User();
    User(const User& other);
    User(std::string fullname);
    User(std::string fullname, std::string username);
    User(std::string fullname, std::string username, std::string passwordHash);
    User(std::string fullname, std::string username, std::string passwordHash, bool isBlocked);
    ~User();

    std::string getFullname() const;
    std::string getUsername() const;
    std::string getPasswordHash() const;
    bool getIsBlocked() const;
    int getId() const;

    void setFullname(const std::string& fullname);
    void setUsername(const std::string& username);
    void setPasswordHash(const std::string& passwordHash);
    void setIsBlocked(bool isBlocked);
};

class AuthSystem {
public:
    std::vector<User> users;
    Hash passwordHasher;
    std::string currentUsername;
    User currentUserData;

    AuthSystem();
    ~AuthSystem();

    bool userExists(std::string& username);
    bool registerUser();
    bool loginUser();

    std::string getCurrentUsername() const { return currentUsername; }
    const User& getCurrentUser() const { return currentUserData; }
    void logout() {
        currentUsername = "";
        currentUserData = User();
    }
};