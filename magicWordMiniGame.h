#include "IMiniGame.h"
#include "inputState.h"
#include "console.h"

class MagicWordMiniGame : public IMiniGame
{
    private:
        InputState *inputState;
        Console *console;
        
        bool win;
        int broke;

        bool begin;
        bool isEnd;
        int score;
        int ticks;
        int saveTick;
        int gameTick;
        
    public:
        void tick();
        bool end();
        int getScore();

        MagicWordMiniGame(Console *console, InputState *inputState);
        ~MagicWordMiniGame(); 
};
