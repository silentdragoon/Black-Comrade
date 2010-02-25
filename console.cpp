#include "console.h"

Console::Console(SceneManager *sceneMgr) :
    sceneMgr(sceneMgr),
    rollTick(0),
    isVisible(false),
    prompt(">> ")
{
    lines = new std::list<std::string>();
    height=0;
    rect = new Rectangle2D(true);
    rect->setCorners(-0.5,1,0.5,1-height);
    rect->setMaterial("console/Background");
    rect->setRenderQueueGroup(RENDER_QUEUE_OVERLAY);
    rect->setBoundingBox(AxisAlignedBox(-100000.0*Vector3::UNIT_SCALE, 100000.0*Vector3::UNIT_SCALE));
    node = sceneMgr->getRootSceneNode()->createChildSceneNode("#Console");
    node->attachObject(rect);

    textbox=OverlayManager::getSingleton().createOverlayElement("TextArea","ConsoleText");
    textbox->setCaption("");
    textbox->hide();
    textbox->setMetricsMode(GMM_RELATIVE);
    textbox->setPosition(0.25,0);
    textbox->setDimensions(0.5,0.5);
    textbox->setParameter("font_name","Console");
    textbox->setParameter("colour_top","1 1 1");
    textbox->setParameter("colour_bottom","1 1 1");
    textbox->setParameter("char_height","0.02");

    overlay=OverlayManager::getSingleton().create("Console");   
    overlay->add2D((OverlayContainer*)textbox);
    overlay->show();
}

Console::~Console() {
    OverlayManager::getSingleton().destroy(overlay);
    sceneMgr->destroySceneNode(node);
}

void Console::tick() {
    if((isVisible)&&(rollTick<10)) {
        // Roll out
        height+=0.1;
        rect->setCorners(-0.5,1,0.5,1-height);
        rollTick++;        
    }

    if((!isVisible)&&(rollTick>0)) {
        // Roll in
        height-=0.1;
        rect->setCorners(-0.5,1,0.5,1-height);
        rollTick--;
    }

    if(rollTick==10) {
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
    prompt=">> ";
}

void Console::backSpacePrompt() {
    if(prompt.size()>3) {
        prompt=prompt.substr(0,prompt.length()-1);
    }
}

void Console::clearPrompt() {
    prompt = prompt.substr(0,3);
}

void Console::appendLine(std::string s) {
    if(s.size()>CONSOLE_LENGTH) {
        // TODO: Do something about the length
    }
    lines->push_front(s);
}

