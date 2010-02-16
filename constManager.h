
#ifndef CONSTANT_MANAGER_H
#define CONSTANT_MANAGER_H
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <sys/types.h>
#include <dirent.h>

using namespace std;

class ConstManager
{
private:
	std::map<std::string, float> constMap;
	static ConstManager *singleton;
	
	int getIntInternal(std::string name);
    float getFloatInternal(std::string name);

protected:
    ConstManager(char *fileName);

public:
    static int getInt(std::string name);
    static float getFloat(std::string name);
    
    void add(std::string name, float value);
    
    static ConstManager *getSingleton();
    
};

#endif
