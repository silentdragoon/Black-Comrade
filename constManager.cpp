
#include "constManager.h"
#include <boost/algorithm/string/trim.hpp>

ConstManager *ConstManager::singleton = NULL;

ConstManager::ConstManager(char *fileName)
{   
    // Read the file
    ifstream fin(fileName);
    
    if(fin.is_open()) {
        string line;
        while(!fin.eof()) {
            getline(fin,line);
            
            std::istringstream iss(line);
            string label, value;
            getline(iss,label,'=');
            label = boost::algorithm::trim_copy(label);
            
            if(label[0] != '#' && getline(iss,value)) {
                std::istringstream convert(value);
                float vFloat;
                convert >> vFloat;
                add(label,vFloat);
            }
        }
        
    } else {
        std::cerr << "Unable to open constant file." << std::endl;
    }
}
    
void ConstManager::add(std::string name, float value)
{
    constMap.insert(std::pair<std::string, float>(name,value));
    std::cout << name << " = " << value << std::endl;
}
   
int ConstManager::getIntInternal(std::string name)
{
    std::map<std::string, float>::const_iterator itr;
    itr = constMap.find(name);
    
    float result = 0;
    
    if(itr != constMap.end()) result = itr->second;
    
    return static_cast<int>(result);
}

float ConstManager::getFloatInternal(std::string name)
{
    std::map<std::string, float>::const_iterator itr;
    itr = constMap.find(name);
    
    if(itr != constMap.end()) return itr->second;
    else return 0.0;
}

ConstManager *ConstManager::getSingleton()
{
    if(singleton == NULL)
        singleton = new ConstManager("constants");
    
    return singleton;
}

int ConstManager::getInt(std::string name)
{
    return getSingleton()->getIntInternal(name);
}

float ConstManager::getFloat(std::string name)
{
    return getSingleton()->getFloatInternal(name);
}
