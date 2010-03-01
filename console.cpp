#include "console.h"

Console::Console(SceneManager *sceneMgr) :
    sceneMgr(sceneMgr),
    rollTick(0),
    isVisible(false),
    prompt("")
{
    lines = new std::list<std::string>();
    height=0;

    openHeight = 0.5;
    slideTicks = 10;

    // Create a panel
    panel = static_cast<OverlayContainer*>(
        OverlayManager::getSingleton().createOverlayElement("Panel", "ConsoleBackground"));
    panel->setMetricsMode(Ogre::GMM_RELATIVE);
    panel->setPosition(0.15, 0); 
    panel->setDimensions(0.7, 0.0);
    panel->setMaterialName("console/Background");

    textbox=OverlayManager::getSingleton().createOverlayElement("TextArea","ConsoleText");
    textbox->setCaption("");
    textbox->hide();
    textbox->setMetricsMode(GMM_RELATIVE);
    textbox->setPosition(0.005,0);
    textbox->setParameter("font_name","Console");
    textbox->setParameter("colour_top","0.85 0.85 0.85");
    textbox->setParameter("colour_bottom","0.85 0.85 0.85");
    textbox->setParameter("char_height","0.02");

    panel->addChild(textbox);

    overlay=OverlayManager::getSingleton().create("Console");   
    overlay->add2D(panel);
    overlay->show();
}

Console::~Console() {
    OverlayManager::getSingleton().destroy(overlay);
}

void Console::tick() {
    if((isVisible)&&(rollTick<slideTicks)) {
        // Open
        height+= openHeight / slideTicks;
        panel->setHeight(height);
        rollTick++;
    }

    if((!isVisible)&&(rollTick>0)) {
        // Close
        height-= openHeight / slideTicks;
        panel->setHeight(height);
        rollTick--;
    }

    if(rollTick==slideTicks) {
        // Show text
        textbox->show();
        displayText();
    } else {
        // Hide Text
        textbox->hide();
    }

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

