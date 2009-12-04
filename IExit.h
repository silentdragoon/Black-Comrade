
#ifndef I_EXIT_H
#define I_EXIT_H

class IExit {
public:
	virtual ~IExit() {}
	
	virtual void exit() = 0;
};

#endif
