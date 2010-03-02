#include "IMiniGame.h"
#include "inputState.h"
#include "console.h"
#include <OIS/OIS.h>
#include <math.h>

class ButtonMashMiniGame : public IMiniGame
{
    private:
        InputState *inputState;
        Console *console;
        
        bool win;
        int broke;

        int tickLimit;
        int ticks;

        bool begin;
        bool isEnd;
        double dscore;
        int score;
        
    public:
        void tick();
        bool end();
        int getScore();

        void alphaNumKeyPressed(const OIS::KeyEvent &arg);
        void returnKeyPressed();

        ShipSystem getSystem();

        ButtonMashMiniGame(Console *console, InputState *inputState);
        ~ButtonMashMiniGame();
};
