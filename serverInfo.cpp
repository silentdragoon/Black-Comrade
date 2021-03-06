#include "serverInfo.h"

ServerInfo::ServerInfo(string address, string name,
                       bool pilotTaken, bool navTaken,
                       bool engTaken, int latency)
    : address(address)
    , name(name)
    , pilotTaken(pilotTaken)
    , navTaken(navTaken)
    , engTaken(engTaken)
    , lastPong(0)
    , latency(latency)
{}

int ServerInfo::getLatency() { return latency; }

int ServerInfo::getLastPong() { return lastPong; }

string ServerInfo::getAddress() { return address; }

string ServerInfo::getName() { return name; }

bool ServerInfo::isPilotTaken() { return pilotTaken; }

bool ServerInfo::isNavigatorTaken() { return navTaken; }

bool ServerInfo::isEngineerTaken() { return engTaken; }

void ServerInfo::setLastPonged(int when) { lastPong = when; }

void ServerInfo::setLatency(int nLatency) { latency = nLatency; }

void ServerInfo::setPilotTaken(bool taken) { pilotTaken = taken; }

void ServerInfo::setNavigatorTaken(bool taken) { navTaken = taken; }

void ServerInfo::setEngineerTaken(bool taken) { engTaken = taken; }

void ServerInfo::print() {
    std::cout << address << " " << name << " " << pilotTaken << " " << navTaken << " " << engTaken << " " << lastPong << " " << latency << "\n";
}
