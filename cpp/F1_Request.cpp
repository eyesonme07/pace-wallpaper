#include"../header/F1_Request.h"
#include<boost/json/src.hpp>
#include<boost/json.hpp>

std::string DriverParser::request()
{
    try {
        connect();
        return doGet("/v1/formula1/drivers?page=1&limit=20");
    } catch (...) {
        qDebug()<<"ERROR";
        disconnect();
        throw;
    }
}
std::string SeasonParser::request()
{
    try {
        connect();
        return doGet("/v1/formula1/seasons?page=1&limit=20");
    } catch (...) {
        qDebug()<<"ERROR";
        disconnect();
        throw;
    }
}

std::string TeamParser::request()
{
    try {
        connect();
        return doGet("/v1/formula1/teams?page=1&limit=20");
    } catch (...) {
        qDebug()<<"ERROR";
        disconnect();
        throw;
    }
}


std::vector<Driver> DriverParser::getDriversList()
{
    std::vector<Driver> drivers;

    std::string jsonString = request();
    if(jsonString.empty()){return drivers;}


    boost::system::error_code error;
    boost::json::value jsonDoc = boost::json::parse(jsonString,error);

    if(error){
        qDebug()<<error.message().c_str();
        return drivers;
    }

    boost::json::object object = jsonDoc.as_object();

    if(auto *p = object.if_contains("message")){
        qDebug()<<"Message - "<<boost::json::value_to<std::string>(*p);
        return drivers;
    }

    boost::json::array list = object["data"].as_array();

    auto safeString = [](const boost::json::value & jv) -> std::string {
        if (jv.is_null()) return "";
        if (jv.is_string()) return std::string(jv.as_string());
        if (jv.is_int64())  return std::to_string(jv.as_int64());
        if (jv.is_uint64()) return std::to_string(jv.as_uint64());
        return "";
    };

    for(auto el : list){
        try {
            Driver d;
            boost::json::object driver = el.as_object();

            d.lastName  = safeString(driver["lastName"]);
            d.firstName = safeString(driver["firstName"]);
            d.birthDate = safeString(driver["birthDate"]);
            d.tla       = safeString(driver["tla"]);
            d.id        = safeString(driver["id"]);
            d.nuumber   = safeString(driver["number"]);

            drivers.push_back(d);
        }
        catch (const std::exception &ex) {
            qDebug()<<ex.what();
            qDebug()<<boost::json::serialize(el).c_str();
        }
    }

    return drivers;
}







