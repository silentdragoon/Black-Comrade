
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
{
	console->makeBlank();

    console->setString("Sheild Gen Phase Allignment Program",30,0);
    console->setString("Welcome to the",1,1);
    console->setString("sheld generator",1,2);
    console->setString("phase matrix",1,3);
    console->setString("allignment system.",1,4);
    console->setString("Hold down the",1,6);
    console->setString("maching keys [ASDF]",1,7);
    console->setString("and press any",1,8);
    console->setString("arrow key as each",1,9);
    console->setString("line of charactors",1,10);
    console->setString("pass the",1,11);
    console->setString("calibration point.",1,12);
    console->setString("Good luck...",1,14);
    console->setString("Calibration point    ---->",1,21);
    
    drawBoard();
    cout << "Level: " << level << endl;
    switch(level)
    {
        case 1:
            dTime = 1 / ConstManager::getFloat("tick_period");
            loadFile("sheildLevel1");
            break;
        case 2:
            dTime = 0.6 / ConstManager::getFloat("tick_period");
            loadFile("sheildLevel2");
            break;
        case 3:
            dTime = 0.4 / ConstManager::getFloat("tick_period");
            loadFile("sheildLevel3");
            break;
    }
    
    currentTime = (boardHeight - 6) * dTime;
}

int SheildMiniGame::calcHeal() {
    
    int perNote = (int)ceil(100.0 / numNotes);
    
    int subTotal = perNote * currentChoird.size();
    
    return subTotal;
}

void SheildMiniGame::tick()
{
	//drawBoard();

    drawKeyStates();
    
    heal = 0;
    
    if(winLine) {
        drawLine(0,"");
        if(!healed) {
            heal = 5;//calcHeal();
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
        
        for(int i = 0; i <= boardHeight; i++) {
            int index = currentQ + i - boardHeight;
            if(index >= 0 && index < keys.size()) {
                drawLine(i,keys[index]);
            } else drawLine(i,"");
            
            winLine = false;
            loseLine = false;
            heal = false;
            
        }
    }
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

	console->setString("- (A) - (S) - (D) - (F) -",boardX + 1, 
	    boardY + boardHeight + 1);

    if(inputState->isKeyDown(OIS::KC_A)) {
        console->setChar('^', boardX + 4, boardY + boardHeight + 1);
        currentChoird.append("A");
    } else {
        console->setChar('A', boardX + 4, boardY + boardHeight + 1);
    }
    
    if(inputState->isKeyDown(OIS::KC_S)) {
        console->setChar('^', boardX + 10, boardY + boardHeight + 1);
        currentChoird.append("S");
    } else {
        console->setChar('S', boardX + 10, boardY + boardHeight + 1);
    }
    
    if(inputState->isKeyDown(OIS::KC_D)) {
        console->setChar('^', boardX + 16, boardY + boardHeight + 1);
        currentChoird.append("D");
    } else {
        console->setChar('D', boardX + 16, boardY + boardHeight + 1);
    }
    
    if(inputState->isKeyDown(OIS::KC_F)) {
        console->setChar('^', boardX + 22, boardY + boardHeight + 1);
        currentChoird.append("F");
    } else {
        console->setChar('F', boardX + 22, boardY + boardHeight + 1);
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

    if(chars.find_first_of("A") != chars.npos) {
        console->setChar('A', boardX + 4, boardY + boardHeight - index);
    } else {
        console->setChar(blank, boardX + 4, 
            boardY + boardHeight - index);
    }
    
    if(chars.find_first_of("S") != chars.npos) {
        console->setChar('S', boardX + 10, boardY + boardHeight - index);
    } else {
        console->setChar(blank, boardX + 10, 
            boardY + boardHeight - index);
    }
    
    if(chars.find_first_of("D") != chars.npos) {
        console->setChar('D', boardX + 16, boardY + boardHeight - index);
    } else {
        console->setChar(blank, boardX + 16, 
            boardY + boardHeight - index);
    }
    
    if(chars.find_first_of("F") != chars.npos) {
        console->setChar('F', boardX + 22, boardY + boardHeight - index);
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
    return true;
}

int SheildMiniGame::getScore()
{
    return heal;
}

ShipSystem SheildMiniGame::getSystem()
{
    return SS_SHIELD_GENERATOR;
}

string SheildMiniGame::getName()
{
    return string("shieldGame");
}

void SheildMiniGame::alphaNumKeyPressed(const OIS::KeyEvent &arg) {}

void SheildMiniGame::returnKeyPressed() {}

void SheildMiniGame::backspaceKeyPressed() {}

void SheildMiniGame::otherKeyPressed(const OIS::KeyEvent &arg)
{
    if(arg.key != OIS::KC_UP && arg.key != OIS::KC_DOWN
    	&& arg.key != OIS::KC_LEFT && arg.key != OIS::KC_RIGHT)
    		return;
    
    if(winLine) {
    	winLine = false;
    	loseLine = true;
    	return;
    }
    
    int index = currentQ - boardHeight;
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
        } else loseLine = true;
    } else loseLine = true;
}

        
