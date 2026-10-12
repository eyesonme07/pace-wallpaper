#include"../header/F1_Request.h"
#include<boost/json/src.hpp>
#include<boost/json.hpp>

std::string safeString(const boost::json::value & jv){
    if (jv.is_null()) return "";
    if (jv.is_string()) return std::string(jv.as_string());
    if (jv.is_int64())  return std::to_string(jv.as_int64());
    if (jv.is_uint64()) return std::to_string(jv.as_uint64());
    return "";
}

std::string mergeData(const std::vector<std::string>& data)
{
    if(data.size()==1){
        return data[0];
    }

    boost::json::array temp;

    for (size_t i = 0; i < data.size(); ++i)
    {
        boost::json::value parsed = boost::json::parse(data[i]);
        boost::json::array& array = parsed.as_object().at("data").as_array();

        for (auto& el : array)
            temp.push_back(std::move(el));
    }

    boost::json::object result;
    result["data"] = std::move(temp);

    return boost::json::serialize(result);
}

std::string DriverParser::request()
{
    try {
        connect();
        qDebug()<<"connect";
        SeasonParser s;
        auto opt = doGet("/v1/formula1/seasons/"+s.getCurrentYearSeasonId());

        if(opt == std::nullopt){
            return "";
        }
        else{
            return opt.value();
        }

    } catch (...) {
        qDebug()<<"ERROR DriverParser";
        disconnect();
    }
    return "";
}
std::string SeasonParser::getCurrentYearSeasonId()
{
    std::string currentSeasonId;


    std::vector<Season> seasonList = getSeasonList();



    for(const auto &el:seasonList){

        if(std::stoi(el.year) == QDate::currentDate().year()){
            qDebug()<<"ID "<<el.id;
            return el.id;
        }
    }

    return currentSeasonId;
}

std::vector<Season> SeasonParser::getSeasonList()
{
    std::vector<Season> result;

    std::string jsonString = request();

    if(requestError(jsonString)){
        qDebug()<<jsonString;
        return result;
    }

    boost::system::error_code error;
    boost::json::object doc = boost::json::parse(jsonString,error).as_object();

    if(error){
        qDebug()<<"ERROR parse Season";
        return result;
    }


    boost::json::array data = doc["data"].as_array();

    for(auto &el:data){
        try {
        Season season;
        boost::json::object obj = el.as_object();


        season.id = safeString(obj["id"]);
        season.roundCount = safeString(obj["roundCount"]);
        season.status = safeString(obj["status"]);
        season.year = safeString(obj["year"]);

        result.push_back(season);

        }catch (const std::exception &ex) {
            qDebug()<<ex.what();
            qDebug()<<boost::json::serialize(ex).c_str();
        }
    }

    return result;
}

std::string EventParser::request()
{
    int currentPage = 1;
    int maxPages{};
    try {
        connect();
        std::vector<std::string> result;

        std::optional<std::string> opt = doGet(QString("/v1/formula1/events?page=%1&limit=20&year=%2").arg(currentPage).arg(QDate::currentDate().toString("yyyy")).toStdString());

        if(opt == std::nullopt){return "";}

        std::string jsonRes = opt.value();

        if(jsonRes.empty()){return "";}
        result.push_back(jsonRes);

        boost::json::value json = boost::json::parse(jsonRes);

        boost::json::object obj = json.as_object();
        boost::json::object meta = obj["meta"].as_object();

        maxPages = meta["totalPages"].as_int64();


        for(int i = currentPage;i<maxPages;){
            i++;
            opt = doGet(QString("/v1/formula1/events?page=%1&limit=20&year=%2").arg(i).arg(QDate::currentDate().toString("yyyy")).toStdString());
            if(opt == std::nullopt){return "";}
            result.push_back(opt.value());
        }
        return mergeData(result);
    } catch (...) {
        qDebug()<<"ERROR Event Parse";
        disconnect();
    }

    return "";
}

std::vector<Event> EventParser::getEventList()
{
    std::vector<Event> result;

    std::string jsonString = request();

    if(jsonString.empty()){return result;}

    boost::json::value jsonDoc = boost::json::parse(jsonString);

    boost::json::object object = jsonDoc.as_object();

    boost::json::array array = object.at("data").as_array();

    for(auto &el:array){
        Event event;

        boost::json::object obj = el.as_object();

        event.id = safeString(obj["id"]);
        event.name = safeString(obj["name"]);
        event.start = QDateTime::fromString(QString::fromStdString(safeString(obj["dateStart"])),Qt::ISODateWithMs);
        event.end = QDateTime::fromString(QString::fromStdString(safeString(obj["dateEnd"])),Qt::ISODateWithMs);
        event.status = safeString(obj["status"]);

        boost::json::object loc = obj["location"].as_object();

        event.location.city = safeString(loc["city"]);
        event.location.name = safeString(loc["name"]);
        event.location.id = safeString(loc["id"]);

        boost::json::object cou = loc["country"].as_object();

        event.location.counryName = safeString(cou["name"]);
        event.location.countryThreeCode = safeString(cou["threeCode"]);
        event.location.countryTwoCode = safeString(cou["twoCode"]);



        boost::json::array schedule = obj["schedule"].as_array();

        for(auto &schEl:schedule){
            boost::json::object sch = schEl.as_object();

            Event::Schedule s;

            s.id = safeString(sch["id"]);
            s.name = safeString(sch["name"]);
            s.status = safeString(sch["status"]);
            s.timeStart = QDateTime::fromString(QString::fromStdString(safeString(sch["startTime"])),Qt::ISODateWithMs);
            s.timeEnd = QDateTime::fromString(QString::fromStdString(safeString(sch["endTime"])),Qt::ISODateWithMs);
            s.type = safeString(sch["type"]);

            event.schedules.push_back(s);
        }

        result.push_back(event);
    }

    std::sort(result.begin(),result.end());

    return result;
}

Event EventParser::getNextEvent()
{
    Event next;

    QDateTime currentDate = QDateTime::currentDateTime();

    std::vector<Event> events = getEventList();


    for(const auto &el:events){
        if(currentDate<el.end){
            return el;
        }
    }

    next.name = "Сезон закончился!";

    return next;
}

Event::Schedule EventParser::getNextSchedule(const Event &currentEvent)
{
    Event::Schedule schedule;

    QDateTime currentDateTime = QDateTime::currentDateTime();

    for(const auto &el:currentEvent.schedules){
        if(currentDateTime<el.timeStart){
            return el;
        }
    }

    return schedule;
}

std::string SeasonParser::request()
{
    try {
        connect();
        std::optional<std::string> opt = doGet("/v1/formula1/seasons?page=1&limit=20");
        if(opt == std::nullopt){
            return "";
        }
        else{
            return opt.value();
        }
    } catch (...) {
        qDebug()<<"ERROR seasonParse";
        disconnect();
    }
    return "";
}

std::string TeamParser::request()
{
    try {
        connect();
        auto opt = doGet("/v1/formula1/teams?page=1&limit=20");
        if(opt == std::nullopt){
            return "";
        }
        else{
            return opt.value();
        }
    } catch (...) {
        qDebug()<<"ERROR teamParser";
        disconnect();   
    }
    return "";
}


std::vector<Driver> DriverParser::getDriversList()
{
    std::vector<Driver> drivers;

    std::string jsonString = request();

    if(requestError(jsonString)){
        qDebug()<<jsonString;
        return drivers;
    }

    boost::system::error_code error;
    boost::json::value jsonDoc = boost::json::parse(jsonString,error);

    if(error){
        qDebug()<<error.message().c_str();
        return drivers;
    }

    boost::json::object object = jsonDoc.as_object();

    boost::json::array list = object["drivers"].as_array();

    for(auto el : list){
        try {
            Driver d;
            boost::json::object driver = el.as_object();


            //про гонщиква
            d.lastName  = safeString(driver["lastName"]);
            d.firstName = safeString(driver["firstName"]);
            d.birthDate = QDate::fromString(QString::fromStdString(safeString(driver["birthDate"])),"yyyy-mm-dd");
            d.tla       = safeString(driver["code"]);
            d.id        = safeString(driver["id"]);
            d.number   = safeString(driver["number"]);
            d.point     = safeString(driver["points"]);
            d.position  = safeString(driver["position"]);

            //про команду
            boost::json::object team = (driver["teams"].as_array()).at(0).as_object();

            d.team.id = safeString(team["id"]);
            d.team.name = safeString(team["name"]);
            d.team.color = safeString(team["color"]);
            d.team.shortName = safeString(team["shortName"]);

            drivers.push_back(d);
        }
        catch (const std::exception &ex) {
            qDebug()<<ex.what();
            qDebug()<<boost::json::serialize(el).c_str();
        }
    }

    return drivers;
}



std::optional<std::string> requestError(const std::string &jsonString)
{
    std::optional<std::string> result;

    if(jsonString.empty()){return "json is empty";}


    boost::system::error_code error;
    boost::json::value jsonDoc = boost::json::parse(jsonString,error);

    if(error){
        return error.message();
    }

    boost::json::object object = jsonDoc.as_object();

    if(auto *p = object.if_contains("message")){
        return boost::json::value_to<std::string>(*p);
    }

    return std::nullopt;
}


