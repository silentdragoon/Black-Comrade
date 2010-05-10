
#include "constManager.h"
#include <boost/algorithm/string/trim.hpp>

ConstManager *ConstManager::singleton = NULL;

ConstManager::ConstManager(char *fileName)
{   
    parseFile(fileName);    
}

void ConstManager::parseFile(const char *fileName)
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
                
                value = boost::algorithm::trim_copy(value);
                
                // Check for include 
                if(label.compare("INCLUDE") == 0) {
                    
                    parseFile(value.c_str());
                } else {
                    
                    add(label,new string(value.c_str()));
                }
            }
        }
        
    } else {
        std::cerr << "Unable to open constant file '"
            << fileName << "'" << std::endl;
    }
}
    
void ConstManager::add(std::string name, string *value)
{
    constMap.insert(std::pair<std::string, string*>(name,value));
    std::cout << name << " = " << *value << std::endl;
}
   
int ConstManager::getIntInternal(std::string name)
{
    float vFloat = getFloatInternal(name);

    return static_cast<int>(vFloat);
}

float ConstManager::getFloatInternal(std::string name)
{
    std::map<std::string, string*>::const_iterator itr;
    itr = constMap.find(name);
    
    string *value;
    
    float vFloat;
    
    if(itr != constMap.end()) {
    
        value = itr->second;
        
        std::istringstream convert(*value);
        
        convert >> vFloat;

    } else {
        
        cerr << "Constant Not Found: " << name << endl;
        std::exit(1);
        
    }

    return vFloat;
}

string ConstManager::getStringInternal(std::string name)
{
    std::map<std::string, string*>::const_iterator itr;
    itr = constMap.find(name);
    
    string *value;itr->second;
    
    if(itr != constMap.end()) {
        return *itr->second;
    } else {
        return string();    
    }
    
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

string ConstManager::getString(std::string name)
{
    return getSingleton()->getStringInternal(name);
}
