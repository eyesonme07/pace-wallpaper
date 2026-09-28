#ifndef SETTINGS_H
#define SETTINGS_H

#include<QSettings>
#include<string>

//синглтон класс где будут все настройки программы
class Settings{
public:
    static Settings& getInstance(){
        static Settings instance;
        return instance;
    }

    void LoadSettings();

    //геттеры
    std::string &getApiKey(){return API_KEY;}



    Settings(const Settings&)=delete;
    Settings &operator=(const Settings&)=delete;
private:
    Settings(){LoadSettings();}
    std::string API_KEY="none";
};

//для удобства вместо Setings::getInstance пишм settings()
inline Settings& settings(){return Settings::getInstance();}

#endif // SETTINGS_H
