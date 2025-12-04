#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "input_check.h"

enum class RealEstateType {
    APARTMENT
};

class RealEstate {
protected:
    std::string fullUsername;
    std::string address;
    double area;
    int rooms;
    double price;
    int commissYear;
    int est_id;
public:
    static size_t est_count;

    RealEstate();
    RealEstate(const std::string& fullUsername, const std::string& address, double area, int rooms, double price, int comissYear);
    virtual ~RealEstate();

    virtual std::string getAddress() const;
    virtual double getArea() const;
    virtual int getRooms() const;
    virtual double getPrice() const;
    virtual int getCommissYear() const;
    virtual int getEst_id() const;
    virtual std::string getFullUsername() const;
    virtual void setAddress(const std::string& address);
    virtual void setArea(double area);
    virtual void setRooms(int rooms);
    virtual void setPrice(double price);
    virtual void setCommissYear(int commissYear);
    virtual void setEst_id(int est_id);
    virtual void setFullUsername(const std::string& fullUsername);

    virtual void displayInfo() const = 0;
    virtual double calculateRating() const = 0;
    virtual RealEstateType getType() const = 0;
};

class Apartment : public RealEstate {
    int floor;
    bool hasElevator;
    double userRating;

public:
    Apartment();
    Apartment(const std::string& fullUsername, const std::string& address, double area, int rooms, double price, int commissYear, int floor, bool hasElevator);
    ~Apartment() override;

    int getFloor() const;
    bool getHasElevator() const;
    double getUserRating() const;
    bool hasUserRating() const;
    void setFloor(int floor);
    void setHasElevator(bool hasElevator);
    void setUserRating(double rating);

    RealEstateType getType() const override;
    void displayInfo() const override;
    double calculateRating() const override;
};

class RealEstateManager {
public:
    static std::vector<Apartment> apartments;

    void addObj();
    void displayObj();
    void deleteObj();
    void changeObj();
    double calculateObjectRating();
    void deleteObjectRating(std::string type);
    double changeObjectRating(std::string type);
    void displayUserObj();
};