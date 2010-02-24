#include "console.h"

Console::Console(SceneManager *sceneMgr) :
    sceneMgr(sceneMgr),
    rollTick(0),
    isVisible(false)
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
    textbox->setMetricsMode(GMM_RELATIVE);
    textbox->setPosition(0.25,0);
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

void Console::append(std::string c) {
    if(c.size()>CONSOLE_LENGTH) {
        // TODO: Do something about the length
    }
    lines->push_front(c);
}

void Console::tick() {
    if((isVisible)&&(rollTick<10)) {
        // Roll out
        height+=0.1;
        rollTick++;        
    }

    if((!isVisible)&&(rollTick>0)) {
        // Roll in
        height-=0.1;
        rollTick--;
    }

    if(rollTick==10) {
        // Show text
        displayText();
    } else {
        // Hide Text
        textbox->setCaption("");
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
    textbox->setCaption(output);
}
