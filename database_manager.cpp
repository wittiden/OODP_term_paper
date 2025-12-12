#include "database_manager.h"
#include <iostream>

DatabaseManager::DatabaseManager(const std::string& path) : db(nullptr), dbPath(path) {
    connect();
    initializeDatabase();
}

DatabaseManager::~DatabaseManager() {
    disconnect();
}

bool DatabaseManager::connect() {
    int rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка открытия БД: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    executeSQL("PRAGMA foreign_keys = ON;");

    return true;
}

void DatabaseManager::disconnect() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

std::string DatabaseManager::getLastError() const {
    if (db) {
        return sqlite3_errmsg(db);
    }
    return "Database not connected";
}

bool DatabaseManager::executeSQL(const std::string& sql) {
    if (!db) return false;

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        if (errMsg) {
            std::cerr << "SQL ошибка: " << errMsg << std::endl;
            sqlite3_free(errMsg);
        }
        return false;
    }

    return true;
}

bool DatabaseManager::tableExists(const std::string& tableName) {
    if (!db) return false;

    std::string sql = "SELECT name FROM sqlite_master WHERE type='table' AND name='" + tableName + "';";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка проверки таблицы: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    bool exists = (sqlite3_step(stmt) == SQLITE_ROW);
    sqlite3_finalize(stmt);

    return exists;
}

bool DatabaseManager::initializeDatabase() {
    const char* usersTableSQL =
        "CREATE TABLE IF NOT EXISTS users ("
        "    id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    fullname TEXT NOT NULL,"
        "    username TEXT UNIQUE NOT NULL,"
        "    password_hash TEXT NOT NULL,"
        "    is_blocked INTEGER DEFAULT 0,"
        "    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
        ");";

    if (!executeSQL(usersTableSQL)) {
        std::cerr << "Ошибка создания таблицы users" << std::endl;
        return false;
    }

    const char* apartmentsTableSQL =
        "CREATE TABLE IF NOT EXISTS apartments ("
        "    id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    user_id INTEGER,"
        "    full_username TEXT,"
        "    address TEXT NOT NULL,"
        "    area REAL NOT NULL,"
        "    rooms INTEGER NOT NULL,"
        "    price REAL NOT NULL,"
        "    commiss_year INTEGER NOT NULL,"
        "    floor INTEGER NOT NULL,"
        "    has_elevator INTEGER DEFAULT 0,"
        "    user_rating REAL DEFAULT 0,"
        "    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
        "    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE SET NULL"
        ");";

    if (!executeSQL(apartmentsTableSQL)) {
        std::cerr << "Ошибка создания таблицы apartments" << std::endl;
        return false;
    }

    return true;
}

bool DatabaseManager::saveUsers(const std::vector<User>& users) {
    if (!db) return false;

    executeSQL("BEGIN TRANSACTION");

    try {
        if (!executeSQL("DELETE FROM users;")) {
            executeSQL("ROLLBACK");
            return false;
        }

        for (const auto& user : users) {
            if (!saveUser(user)) {
                executeSQL("ROLLBACK");
                return false;
            }
        }

        executeSQL("COMMIT");
        return true;

    }
    catch (...) {
        executeSQL("ROLLBACK");
        return false;
    }
}

bool DatabaseManager::saveUser(const User& user) {
    if (!db) return false;

    std::string sql =
        "INSERT OR REPLACE INTO users (id, fullname, username, password_hash, is_blocked) "
        "VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка подготовки запроса: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, user.getId());
    sqlite3_bind_text(stmt, 2, user.getFullname().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, user.getUsername().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, user.getPasswordHash().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, user.getIsBlocked() ? 1 : 0);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Ошибка сохранения пользователя: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    return true;
}

std::vector<User> DatabaseManager::loadUsers() {
    std::vector<User> users;

    if (!db) return users;

    const char* sql = "SELECT id, fullname, username, password_hash, is_blocked FROM users ORDER BY id;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка загрузки пользователей: " << sqlite3_errmsg(db) << std::endl;
        return users;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        User user;

        user.setId(sqlite3_column_int(stmt, 0));

        const unsigned char* fullname = sqlite3_column_text(stmt, 1);
        if (fullname) user.setFullname(reinterpret_cast<const char*>(fullname));

        const unsigned char* username = sqlite3_column_text(stmt, 2);
        if (username) user.setUsername(reinterpret_cast<const char*>(username));

        const unsigned char* password_hash = sqlite3_column_text(stmt, 3);
        if (password_hash) user.setPasswordHash(reinterpret_cast<const char*>(password_hash));

        user.setIsBlocked(sqlite3_column_int(stmt, 4) != 0);

        users.push_back(user);
    }

    sqlite3_finalize(stmt);

    return users;
}

bool DatabaseManager::deleteUser(int userId) {
    if (!db) return false;

    std::string sql = "DELETE FROM users WHERE id = ?;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка удаления пользователя: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, userId);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Ошибка выполнения удаления: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    return sqlite3_changes(db) > 0;
}

bool DatabaseManager::updateUser(const User& user) {
    if (!db) return false;

    std::string sql =
        "UPDATE users SET fullname = ?, username = ?, password_hash = ?, is_blocked = ? "
        "WHERE id = ?;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка обновления пользователя: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, user.getFullname().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, user.getUsername().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, user.getPasswordHash().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, user.getIsBlocked() ? 1 : 0);
    sqlite3_bind_int(stmt, 5, user.getId());

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Ошибка выполнения обновления: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    return sqlite3_changes(db) > 0;
}

bool DatabaseManager::saveApartment(const Apartment& apartment) {
    if (!db) {
        std::cerr << "[ОШИБКА БД] База данных не подключена!" << std::endl;
        return false;
    }

    std::string sql;

    bool exists = false;
    std::string checkSql = "SELECT COUNT(*) FROM apartments WHERE id = ?;";
    sqlite3_stmt* checkStmt;

    if (sqlite3_prepare_v2(db, checkSql.c_str(), -1, &checkStmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(checkStmt, 1, apartment.getEst_id());
        if (sqlite3_step(checkStmt) == SQLITE_ROW) {
            exists = (sqlite3_column_int(checkStmt, 0) > 0);
        }
        sqlite3_finalize(checkStmt);
    }

    if (exists) {
        sql = "UPDATE apartments SET "
            "full_username = ?, address = ?, area = ?, rooms = ?, "
            "price = ?, commiss_year = ?, floor = ?, has_elevator = ?, user_rating = ? "
            "WHERE id = ?;";
    }
    else {
        sql = "INSERT INTO apartments (id, full_username, address, area, rooms, price, "
            "commiss_year, floor, has_elevator, user_rating) "
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
    }

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "[ОШИБКА БД] Ошибка подготовки запроса: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    if (exists) {
        sqlite3_bind_text(stmt, 1, apartment.getFullUsername().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, apartment.getAddress().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, 3, apartment.getArea());
        sqlite3_bind_int(stmt, 4, apartment.getRooms());
        sqlite3_bind_double(stmt, 5, apartment.getPrice());
        sqlite3_bind_int(stmt, 6, apartment.getCommissYear());
        sqlite3_bind_int(stmt, 7, apartment.getFloor());
        sqlite3_bind_int(stmt, 8, apartment.getHasElevator() ? 1 : 0);
        sqlite3_bind_double(stmt, 9, apartment.getUserRating());
        sqlite3_bind_int(stmt, 10, apartment.getEst_id());
    }
    else {
        sqlite3_bind_int(stmt, 1, apartment.getEst_id());
        sqlite3_bind_text(stmt, 2, apartment.getFullUsername().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, apartment.getAddress().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, 4, apartment.getArea());
        sqlite3_bind_int(stmt, 5, apartment.getRooms());
        sqlite3_bind_double(stmt, 6, apartment.getPrice());
        sqlite3_bind_int(stmt, 7, apartment.getCommissYear());
        sqlite3_bind_int(stmt, 8, apartment.getFloor());
        sqlite3_bind_int(stmt, 9, apartment.getHasElevator() ? 1 : 0);
        sqlite3_bind_double(stmt, 10, apartment.getUserRating());
    }

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "[ОШИБКА БД] Ошибка сохранения квартиры: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    return true;
}

std::vector<Apartment> DatabaseManager::loadApartments() {
    std::vector<Apartment> apartments;

    if (!db) return apartments;

    const char* sql =
        "SELECT id, full_username, address, area, rooms, price, "
        "commiss_year, floor, has_elevator, user_rating FROM apartments ORDER BY id;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка загрузки квартир: " << sqlite3_errmsg(db) << std::endl;
        return apartments;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string fullUsername = "";
        const unsigned char* full_username = sqlite3_column_text(stmt, 1);
        if (full_username) fullUsername = reinterpret_cast<const char*>(full_username);

        std::string address = "";
        const unsigned char* addr = sqlite3_column_text(stmt, 2);
        if (addr) address = reinterpret_cast<const char*>(addr);

        double area = sqlite3_column_double(stmt, 3);
        int rooms = sqlite3_column_int(stmt, 4);
        double price = sqlite3_column_double(stmt, 5);
        int commissYear = sqlite3_column_int(stmt, 6);
        int floor = sqlite3_column_int(stmt, 7);
        bool hasElevator = sqlite3_column_int(stmt, 8) != 0;
        double userRating = sqlite3_column_double(stmt, 9);

        Apartment apartment(fullUsername, address, area, rooms, price, commissYear, floor, hasElevator);
        apartment.setEst_id(sqlite3_column_int(stmt, 0));
        if (userRating > 0) {
            apartment.setUserRating(userRating);
        }

        apartments.push_back(apartment);
    }

    sqlite3_finalize(stmt);
    return apartments;
}

bool DatabaseManager::deleteApartment(int apartmentId) {
    if (!db) return false;

    std::string sql = "DELETE FROM apartments WHERE id = ?;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка удаления квартиры: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, apartmentId);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Ошибка выполнения удаления квартиры: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    return sqlite3_changes(db) > 0;
}

bool DatabaseManager::updateApartment(const Apartment& apartment) {
    if (!db) return false;

    std::string sql =
        "UPDATE apartments SET full_username = ?, address = ?, area = ?, rooms = ?, "
        "price = ?, commiss_year = ?, floor = ?, has_elevator = ?, user_rating = ? "
        "WHERE id = ?;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка обновления квартиры: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, apartment.getFullUsername().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, apartment.getAddress().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 3, apartment.getArea());
    sqlite3_bind_int(stmt, 4, apartment.getRooms());
    sqlite3_bind_double(stmt, 5, apartment.getPrice());
    sqlite3_bind_int(stmt, 6, apartment.getCommissYear());
    sqlite3_bind_int(stmt, 7, apartment.getFloor());
    sqlite3_bind_int(stmt, 8, apartment.getHasElevator() ? 1 : 0);
    sqlite3_bind_double(stmt, 9, apartment.getUserRating());
    sqlite3_bind_int(stmt, 10, apartment.getEst_id());

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Ошибка выполнения обновления квартиры: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    return sqlite3_changes(db) > 0;
}

bool DatabaseManager::deleteApartmentRating(int apartmentId) {
    if (!db) return false;

    std::string sql = "UPDATE apartments SET user_rating = 0 WHERE id = ?;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка удаления рейтинга: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, apartmentId);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Ошибка выполнения удаления рейтинга: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    return sqlite3_changes(db) > 0;
}

bool DatabaseManager::updateApartmentRating(int apartmentId, double rating, const std::string& username) {
    if (!db) return false;

    std::string sql = "UPDATE apartments SET user_rating = ?, full_username = ? WHERE id = ?;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка обновления рейтинга: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_double(stmt, 1, rating);
    sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, apartmentId);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Ошибка выполнения обновления рейтинга: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    return sqlite3_changes(db) > 0;
}

bool DatabaseManager::backupDatabase(const std::string& backupPath) {
    if (!db) return false;

    sqlite3* backupDb;
    int rc = sqlite3_open(backupPath.c_str(), &backupDb);

    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка создания резервной копии: " << sqlite3_errmsg(backupDb) << std::endl;
        sqlite3_close(backupDb);
        return false;
    }

    sqlite3_backup* backup = sqlite3_backup_init(backupDb, "main", db, "main");
    if (backup) {
        sqlite3_backup_step(backup, -1);
        sqlite3_backup_finish(backup);
    }

    sqlite3_close(backupDb);
    return true;
}