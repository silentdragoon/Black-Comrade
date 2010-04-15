#ifndef SERVER_INFO_H
#define SERVER_INFO_H

#include <string>
#include <iostream>

using namespace std;

class ServerInfo
{
    private:

    int lastPong;
    int latency;

    string address;
    string name;
    bool pilotTaken;
    bool engTaken;
    bool navTaken;

    public:
    ServerInfo(string address, string gameName,
               bool pilotTaken, bool navTaken, bool engTaken,
               int latency);

    int getLatency(); 
    int getLastPong();
    string getAddress();
    string getName();
    bool isPilotTaken();
    bool isNavigatorTaken();
    bool isEngineerTaken();

    void setLatency(int nLatency);
    void setLastPonged(int when);
    void setPilotTaken(bool taken);
    void setEngineerTaken(bool taken);
    void setNavigatorTaken(bool taken);

    void print();
};

#endif
