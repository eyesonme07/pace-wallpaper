#ifndef F1_REQUEST_H
#define F1_REQUEST_H

#include"../header/API.h"

struct Driver
{
    std::string id;
    std::string firstName;
    std::string lastName;
    std::string birthDate;
    std::string tla;
    std::string nuumber;
};

struct Team{
    std::string id;
    std::string name;
    std::string shortName;
    std::string fullName;
    std::string color;
    std::string city;
};

struct Season
{
    std::string id;
    unsigned short year;
    unsigned short roundCount;
    std::string status;
};


class DriverParser:public F1API{
public:
    std::vector<Driver> getDriversList();

private:
    std::string request()override;
};

class TeamParser:public F1API{
public:
    std::string request()override;
};

class SeasonParser:public F1API{
public:
    std::string request()override;
};

#endif // F1_REQUEST_H