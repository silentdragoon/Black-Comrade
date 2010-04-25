#include "notification.h"

Notification::Notification(NotificationType type, std::string consoleText,
                           std::string soundNameConst, int soundLength)
    : type(type)
    , consoleText(consoleText)
    , soundNameConst(soundNameConst)
    , soundLength(soundLength)
    , local(false)
{}

bool Notification::isLocal() { return local; }

void Notification::makeLocal() { local = true; }

NotificationType Notification::getType() { return type; }

std::string Notification::getConsoleText() { return consoleText; }

std::string Notification::getSoundNameConst() { return soundNameConst; }

int Notification::getSoundLength() { return soundLength; }
