#pragma once
#include <sqlite3.h>
#include <vector>
#include <string>
#include "functions.h"
#include "realestate.h"

class User;
class Apartment;

class DatabaseManager {
    sqlite3* db;
    std::string dbPath;

    bool executeSQL(const std::string& sql);
    bool tableExists(const std::string& tableName);

public:
    DatabaseManager(const std::string& path = "database/mydatabase.db");
    ~DatabaseManager();

    bool connect();
    void disconnect();

    bool initializeDatabase();

    bool saveUsers(const std::vector<User>& users);
    bool saveUser(const User& user);
    std::vector<User> loadUsers();
    bool deleteUser(int userId);
    bool updateUser(const User& user);

    bool saveApartment(const Apartment& apartment);
    std::vector<Apartment> loadApartments();
    bool deleteApartment(int apartmentId);
    bool updateApartment(const Apartment& apartment);
    bool deleteApartmentRating(int apartmentId);
    bool updateApartmentRating(int apartmentId, double rating, const std::string& username);

    bool backupDatabase(const std::string& backupPath);

    std::string getLastError() const;
};