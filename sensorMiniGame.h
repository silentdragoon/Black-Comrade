#include "IMiniGame.h"
#include "inputState.h"
#include "console.h"
#include <OIS/OIS.h>
#include <math.h>

class SensorMiniGame : public IMiniGame
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
        
        char field;
        
        char* generateLine();
        char* checkCollisions(char* line, int position);
        int width;
        int height;
        int seed;
        char line[20];
        
    public:
        void tick();
        bool end();
        int getScore();

        void otherKeyPressed(const OIS::KeyEvent &arg);
        void returnKeyPressed();

        ShipSystem getSystem();

        SensorMiniGame(Console *console, InputState *inputState);
        ~SensorMiniGame();
};
