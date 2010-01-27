#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <string>

using namespace std;

class Waypoint {
    private:
        string *name;
        int x;
        int y;
    public:
        Waypoint(string *fu, int x, int y);
        ~Waypoint();
        string* getName();
        int getX();
        int getY();
};

#endif
