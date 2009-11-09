
#ifndef I_TICKABLE_H
#define I_TICKABLE_H

class ITickable {
public:
	virtual ~ITickable() {}
	
	virtual void tick() = 0;
};

#endif
