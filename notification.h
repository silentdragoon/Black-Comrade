#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <string>
#include "notificationType.h"

class Notification
{
private:
    NotificationType type;
    std::string consoleText;
    bool local;
    std::string soundNameConst;
    int soundLength;

public:

    Notification(NotificationType type, std::string consoleText,
                 std::string soundNameConst, int soundLength);

    NotificationType getType();
    std::string getConsoleText();
    std::string getSoundNameConst();
    int getSoundLength();
    bool isLocal();
    void makeLocal();
};

#endif
