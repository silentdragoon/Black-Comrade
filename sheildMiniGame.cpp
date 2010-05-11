
#include "sheildMiniGame.h"

SheildMiniGame::SheildMiniGame(Console *console, InputState *inputState, int level)
    : console(console)
    , isEnd(false)
    , boardX(36)
    , boardY(3)
    , boardHeight(18)
    , boardWidth(25)
    , inputState(inputState)
    , currentQ(0)
    , currentTime(0)
    , loseLine(false)
    , winLine(false)
    , heal(0)
    , healed(false)
    , numNotes(0)
    , streak(0)
    , started(false)
    , healTotal(0)
{
	console->makeBlank();

    console->setString("Engine Phase Allignment Program",34,0);

    int t = 6;
    int c = 4;
    
    console->setString("",c,t++);
    console->setString("Hold down the maching",c,t++);
    console->setString("keys [1234] for each",c,t++);
    console->setString("line.",c,t++);
    
    
    console->setString("Press any ARROW key",c,t++);
    console->setString("as each line reaches",c,t++);
    console->setString("the calibration point.",c,t++);
    console->setString("",c,t++);
    console->setString("",c,t++);
    console->setString("Good luck...",c,t++);
    console->setString("",c,t++);
    console->setString("",c,t++);
    console->setString("Hit RETURN to EXIT",c,t++);
    console->setString("Calibration point    ---->",3,21);
    
    console->setString("Hit SPACE to Start",40,15);
    
    console->setString("Multiplier:", 64, 17);
    
    console->setString("Streak:", 64, 20);

    drawBoard();
    cout << "Level: " << level << endl;
    switch(level)
    {
        case 1:
            dTime = 1.4 / ConstManager::getFloat("tick_period");
            loadFile("sheildLevel0");
            break;
        case 2:
            dTime = 1.2 / ConstManager::getFloat("tick_period");
            loadFile("sheildLevel1");
            break;
        case 3:
            dTime = 0.7 / ConstManager::getFloat("tick_period");
            loadFile("sheildLevel2");
            break;
        case 4:
            dTime = 0.5 / ConstManager::getFloat("tick_period");
            loadFile("sheildLevel3");
            break;
    }
    
    currentTime = (boardHeight - 2) * dTime;
}

int SheildMiniGame::calcHeal() {
    
    int perNote = (int)ceil(100.0 / numNotes);
    
    int subTotal = perNote * currentChoird.size();
    
    return subTotal * mult / 2;
}

void SheildMiniGame::tick()
{
	//drawBoard();

    std::stringstream ss;

    // Multiplier
    mult = 1 + streak / (numNotes / 5);
    mult = mult > 4 ? 4 : mult;
    ss << mult << "x";
    console->setString(ss.str(), 64, 18);
    
    // Streak
    std::stringstream ss2;
    ss2 << streak;
    console->setString(ss2.str(), 64, 21);

    if(started) {
        drawKeyStates();
        
        int index = currentQ - boardHeight;
        
        // Line
		if(index >= 0 && index < keys.size()) {
        	std::string str;
        
        	str.append(keys[index]);
        
        	if(str.size() == currentChoird.size() && str.size() != 0) {
        
        		while(str.size() > 0) {

            		if(currentChoird.find_first_of(str[0]) ==
            			currentChoird.npos) {
            			loseLine = true;
            			return;
            		}
            		str.erase(0);
        		}
        	
        		winLine = true;
        	
        	}
    	}
        
        heal = 0;
        
        if(winLine) {
            drawLine(0,"");
            if(!healed) {
                heal = calcHeal();
                healed = true;
            }
        }
        
        currentTime++;
        
        int newQ = (int) floor(currentTime / dTime);
        if(currentQ != newQ) {
            currentQ = newQ;
            
            // Check for end game
            
            if((currentQ - boardHeight) >= (int)keys.size()) {
                isEnd = true;
            }
            
            if(!winLine) streak = 0;
            
            for(int i = 0; i <= boardHeight; i++) {
                int index = currentQ + i - boardHeight;
                if(index >= 0 && index < keys.size()) {
                    drawLine(i,keys[index]);
                } else drawLine(i,"");
            }
            
            winLine = false;
            loseLine = false;
            healed = false;
        }
    }
    
    healTotal += heal;
}

void SheildMiniGame::loadFile(string fileName)
{
    ifstream fin(fileName.c_str());
    
    if(fin.is_open()) {
        
        string line;
        while(!fin.eof()) {
            getline(fin,line);
            
            numNotes += line.size();
            
            keys.push_back(line); 
        }
    } else {
        cerr << "Cant open file :(\n";
        isEnd = true;
    }
}

void SheildMiniGame::drawKeyStates()
{
    currentChoird.clear();

	console->setString("- (1) - (2) - (3) - (4) -",boardX + 1, 
	    boardY + boardHeight + 1);

    if(inputState->isKeyDown(OIS::KC_1)) {
        console->setChar('^', boardX + 4, boardY + boardHeight + 1);
        currentChoird.append("1");
    } else {
        console->setChar('1', boardX + 4, boardY + boardHeight + 1);
    }
    
    if(inputState->isKeyDown(OIS::KC_2)) {
        console->setChar('^', boardX + 10, boardY + boardHeight + 1);
        currentChoird.append("2");
    } else {
        console->setChar('2', boardX + 10, boardY + boardHeight + 1);
    }
    
    if(inputState->isKeyDown(OIS::KC_3)) {
        console->setChar('^', boardX + 16, boardY + boardHeight + 1);
        currentChoird.append("3");
    } else {
        console->setChar('3', boardX + 16, boardY + boardHeight + 1);
    }
    
    if(inputState->isKeyDown(OIS::KC_4)) {
        console->setChar('^', boardX + 22, boardY + boardHeight + 1);
        currentChoird.append("4");
    } else {
        console->setChar('4', boardX + 22, boardY + boardHeight + 1);
    }
    
    if(loseLine) {
    	for(int i = 1; i < boardWidth - 1; ++i) {
			console->setChar('X', boardX + i, boardY + boardHeight + 1);
		}
	}
}

void SheildMiniGame::drawLine(int index, std::string chars)
{
    char blank = index == 0 ? '-' : ' ';

    if(chars.find_first_of("1") != chars.npos) {
        console->setChar('1', boardX + 4, boardY + boardHeight - index);
    } else {
        console->setChar(blank, boardX + 4, 
            boardY + boardHeight - index);
    }
    
    if(chars.find_first_of("2") != chars.npos) {
        console->setChar('2', boardX + 10, boardY + boardHeight - index);
    } else {
        console->setChar(blank, boardX + 10, 
            boardY + boardHeight - index);
    }
    
    if(chars.find_first_of("3") != chars.npos) {
        console->setChar('3', boardX + 16, boardY + boardHeight - index);
    } else {
        console->setChar(blank, boardX + 16, 
            boardY + boardHeight - index);
    }
    
    if(chars.find_first_of("4") != chars.npos) {
        console->setChar('4', boardX + 22, boardY + boardHeight - index);
    } else {
        console->setChar(blank, boardX + 22, 
            boardY + boardHeight - index);
    }
}

void SheildMiniGame::drawBoard()
{
	for(int i = 0; i < boardHeight; ++i)
	{
		console->setChar('|', boardX, boardY + i);
		console->setChar('|', boardX + boardWidth, boardY + i);
	}
	
	console->setChar('|', boardX, boardY + boardHeight);
	console->setChar('|', boardX + boardWidth, boardY + boardHeight);
	console->setChar('+', boardX, boardY + boardHeight);
	console->setChar('+', boardX + boardWidth, boardY + boardHeight);
	console->setChar('|', boardX, boardY + boardHeight + 1);
	console->setChar('|', boardX + boardWidth, boardY + boardHeight + 1);
	console->setChar('+', boardX, boardY + boardHeight + 2);
	console->setChar('+', boardX + boardWidth, boardY + boardHeight + 2);
	
	for(int i = 1; i < boardWidth - 1; ++i) {
		console->setChar('-', boardX + i, boardY + boardHeight);
		console->setChar('-', boardX + i, boardY + boardHeight + 2);
	}
}

bool SheildMiniGame::end()
{
    return isEnd;
}

bool SheildMiniGame::complete()
{
    return healTotal >= 50;
}

int SheildMiniGame::getScore()
{
    return heal;
}

ShipSystem SheildMiniGame::getSystem()
{
    return SS_ENGINES;
}

string SheildMiniGame::getName()
{
    return string("shieldGame");
}

void SheildMiniGame::alphaNumKeyPressed(const OIS::KeyEvent &arg) 
{
    if(arg.key == OIS::KC_SPACE) {
    
        if(!started) {
            started = true;
            console->setString("                  ",40,15);
        }
    
        
    }

}

void SheildMiniGame::returnKeyPressed() 
{
	isEnd = true;
}

void SheildMiniGame::backspaceKeyPressed() {}

void SheildMiniGame::otherKeyPressed(const OIS::KeyEvent &arg)
{   return;
    if(winLine) {
    	winLine = false;
    	loseLine = true;
    	return;
    }
    
     if(winLine) streak += currentChoird.size();
    if(loseLine) streak = 0;
}

        
