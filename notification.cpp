#include "notification.h"

Notification::Notification(NotificationType type, std::string consoleText,
                           int soundNameConst, int soundLength, int music)
    : type(type)
    , consoleText(consoleText)
    , soundNameConst(soundNameConst)
    , soundLength(soundLength)
    , music(music)
{}

NotificationType Notification::getType() { return type; }

std::string Notification::getConsoleText() { return consoleText; }

int Notification::getSoundNameConst() { return soundNameConst; }

int Notification::getSoundLength() { return soundLength; }

int Notification::getMusic() { return music; } 
