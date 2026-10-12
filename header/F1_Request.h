#ifndef F1_REQUEST_H
#define F1_REQUEST_H

#include"../header/API.h"
#include<optional>
#include<QDate>
#include<QDateTime>

struct Team {
    std::string id{};
    std::string name{};
    std::string shortName{};
    std::string fullName{};
    std::string color{};
    std::string city{};
};

struct Driver
{
    std::string id{};
    std::string firstName{};
    std::string lastName{};
    QDate birthDate{};
    std::string tla{};
    std::string number{};
    std::string position{};
    std::string point{};
    Team team{};
};

struct Season
{
    std::string id{};
    std::string year{};
    std::string roundCount{};
    std::string status{};
};

struct Event{
    struct Location{
        std::string name{};
        std::string city{};
        std::string id{};
        std::string counryName{};
        std::string countryTwoCode{};
        std::string countryThreeCode{};
    };

    struct Schedule{
        std::string id{};
        std::string name{};
        std::string type{};
        QDateTime timeStart{};
        QDateTime timeEnd{};
        std::string status{};
    };

    QDateTime start{};
    QDateTime end{};
    std::string name{};
    std::string id{};
    std::string status{};

    Location location{};
    std::vector<Schedule> schedules{};

    bool operator<(const Event &e){
        return start < e.start;
    }
};

//проверяет есть или ошибка в запрсе если да то возвращает текст ошибки
std::optional<std::string> requestError(const std::string &jsonString);

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
    std::string getCurrentYearSeasonId();
    std::vector<Season> getSeasonList();
    std::string request()override;
};

class EventParser:public F1API{
public:
    std::string request()override;

    std::vector<Event> getEventList();
    Event getNextEvent();

    Event::Schedule getNextSchedule(const Event &currentEvent);
};


#endif // F1_REQUEST_H














