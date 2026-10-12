#ifndef SETTINGS_H
#define SETTINGS_H

#include<QSettings>
#include<string>
#include<QStandardPaths>

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
    int getLeftPadding(){return leftPadding;}
    int getRightPadding(){return rightPadding;}
    int getTopPadding(){return topPadding;}
    int getBottomPadding(){return bottomPadding;}
    std::string getApplicationDir(){
        return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation).toStdString();
    }
    std::string getDocumentDir(){
        return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString();
    }

    Settings(const Settings&)=delete;
    Settings &operator=(const Settings&)=delete;
private:

    Settings(){LoadSettings();}
    std::string API_KEY="none";

    int leftPadding,rightPadding,topPadding,bottomPadding;
};

//для удобства вместо Setings::getInstance пишм settings()
inline Settings& settings(){return Settings::getInstance();}

#endif // SETTINGS_H
