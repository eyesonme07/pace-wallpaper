#include"../header/settings.h"

void Settings::LoadSettings()
{
    API_KEY = qEnvironmentVariable("STRIPE_KEY").toStdString();
}
