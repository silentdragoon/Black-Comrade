
#ifndef I_PLAYER_CONTROLS
#define I_PLAYER_CONTROLS

class IPlayerControls {
public:
	virtual ~IPlayerControls() {}
	virtual bool fire() = 0;
};

#endif
