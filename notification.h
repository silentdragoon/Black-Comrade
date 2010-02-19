#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <string>
#include "notificationType.h"

class Notification
{
private:
    NotificationType type;
    std::string consoleText;
    int soundNameConst;
    int soundLength;

public:

    Notification(NotificationType type, std::string consoleText,
                 int soundNameConst, int soundLength);

    NotificationType getType();
    std::string getConsoleText();
    int getSoundNameConst();
    int getSoundLength();

};

#endif
