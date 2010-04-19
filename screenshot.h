#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include "ITickable.h"

class screenshot : public ITickable {
    private:
    
    public:
        screenshot();
        ~screenshot();
        virtual void tick();
 
};

#endif
