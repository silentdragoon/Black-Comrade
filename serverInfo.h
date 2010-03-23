#ifndef SERVER_INFO_H
#define SERVER_INFO_H

#include <string>
#include <iostream>

using namespace std;

class ServerInfo
{
    private:
    string address;
    string name;
    bool pilotTaken;
    bool engTaken;
    bool navTaken;

    public:
    ServerInfo(string address, string gameName, bool pilotTaken, bool navTaken, bool engTaken);

    string getAddress();
    string getName();
    bool isPilotTaken();
    bool isNavigatorTaken();
    bool isEngineerTaken();

    void setPilotTaken(bool taken);
    void setEngineerTaken(bool taken);
    void setNavigatorTaken(bool taken);

    void print();
};

#endif
