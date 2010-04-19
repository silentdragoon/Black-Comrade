#include "console.h"

Console::Console(SceneManager *sceneMgr) :
    sceneMgr(sceneMgr),
    rollTick(0),
    isVisible(false),
    prompt("")
{
    lines = new std::list<std::string>();

    openHeight = 0.5;
    slideTicks = 6;
    top=-openHeight;

    // Create a panel
    panel = static_cast<OverlayContainer*>(
        OverlayManager::getSingleton().createOverlayElement("Panel", "ConsoleBackground"));
    panel->setMetricsMode(Ogre::GMM_RELATIVE);
    panel->setPosition(0.15, -openHeight); 
    panel->setDimensions(0.7, openHeight);
    panel->setMaterialName("console/Background");

    textbox=OverlayManager::getSingleton().createOverlayElement("TextArea","ConsoleText");
    textbox->setCaption("");
    textbox->setMetricsMode(GMM_RELATIVE);
    textbox->setPosition(0.005,0);
    textbox->setParameter("font_name","Console");
    textbox->setParameter("colour_top","0.85 0.85 0.85");
    textbox->setParameter("colour_bottom","0.85 0.85 0.85");
    textbox->setParameter("char_height","0.02");

    panel->addChild(textbox);

    overlay=OverlayManager::getSingleton().create("Console");
    overlay->add2D(panel);
    overlay->setZOrder(649);
    overlay->show();
}

Console::~Console() {
    OverlayManager::getSingleton().destroy(overlay);
}

void Console::forceHide() {
    panel->setTop(-openHeight);
}

void Console::tick() {
    if((isVisible)&&(rollTick<slideTicks)) {
        // Open
        top+= openHeight / slideTicks;
        panel->setTop(top);
        rollTick++;
    }

    if((!isVisible)&&(rollTick>0)) {
        // Close
        top-= openHeight / slideTicks;
        panel->setTop(top);
        rollTick--;
    }

    displayText();

    // Check to see if too many lines stored and remove from list
    if(lines->size()>CONSOLE_HEIGHT) {
        lines->pop_back();
    }
}

bool Console::getVisible() { return isVisible; }

void Console::setVisible(bool visible) {
    isVisible = visible;
}

void Console::displayText() {
    std::string output = "";
    std::list<std::string>::reverse_iterator i;
    for(i=lines->rbegin(); i!=lines->rend(); i++) {
        std::string tmp = *i;
        tmp.append("\n");
        output.append(tmp);
    }
    output.append(prompt);
    textbox->setCaption(output);
}

void Console::appendToPrompt(std::string s) {
    std::string current = textbox->getCaption();
    prompt.append(s);
    current.append(prompt);
    textbox->setCaption(current);
}

void Console::appendToPrompt(char c) {
    std::string current = textbox->getCaption();
    if(prompt.size()<CONSOLE_LENGTH) {
        prompt += c;
    }
    current.append(prompt);
    textbox->setCaption(current);
}

void Console::returnKeyPrompt() {
    // Clears the prompt buffer thing and appends to command history
    appendLine(prompt);
    prompt="";
}

void Console::backSpacePrompt() {
    if(prompt.size()>0) {
        prompt=prompt.substr(0,prompt.length()-1);
    }
}

void Console::clearPrompt() {
    prompt = "";
}

int Console::getWidth() { return CONSOLE_WIDTH; }

int Console::getHeight() { return CONSOLE_HEIGHT; }

void Console::setChar(char c, int x, int y) {
    if (x >= getWidth() || y >= getHeight()) return;

    std::string str = "";
    str += c;

    std::list<std::string>::reverse_iterator it = lines->rbegin();
    std::string line = *it;
    for(int i = 0 ; i < y ; i++) {
        it++;
    }
    it->replace(x,1,str);
}

void Console::setString(std::string str, int xStart, int yStart) {
    for(int i = 0; i < str.length();i++) {
        setChar(str.c_str()[i],xStart+i,yStart);
    }
}

void Console::makeBlank() {
    appendBlankLines(getHeight());
}

void Console::appendBlankLines(int number) {
    for (int i = 0 ; i < number ; i++) {
        appendBlankLine();
    }
}

void Console::appendBlankLine() {
    std::string blank = std::string(getWidth(),' ');
    appendLine(blank);
}

void Console::appendLine(std::string s) {
    if(s.size()>CONSOLE_LENGTH) {
        // TODO: Do something about the length
    }
    lines->push_front(s);

    // Check to see if too many lines stored and remove from list
    if(lines->size()>CONSOLE_HEIGHT) {
        lines->pop_back();
    }
}

