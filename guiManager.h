#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <OGRE/Ogre.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/CEGUIBase.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <sstream>
#include <math.h>
#include "mapManager2.h"
#include "shipState.h"
#include "const.h"
#include "CEGUIRQListener.h"

class GuiManager {
private:

    CEGUI::OgreRenderer* d_renderer;
    CEGUI::WindowManager *guiMgr;
    CEGUI::Window *guiRoot;

    CEGUIRQListener *renderQueueListener;
    SceneManager *sceneMgr;


public:
    CEGUI::Window *black;
    GuiManager(SceneManager *sceneMgr);
    ~GuiManager();

    CEGUI::FrameWindow *addStaticImage(const char *name, 
                                float xCenter, float yCenter,
                                float width,   float height,
                                char *imageSet, char *imageName);

    CEGUI::FrameWindow *addStaticImagePix(const char *name, float xCenter, float yCenter,
                                float width,   float height,
                                char *imageSet, char *imageName);

    CEGUI::FrameWindow *addStaticText(std::string name, std::string text, float xCenter, float yCenter,
                                      int size);

    CEGUI::Window *getRootWindow();

    bool fadeToBlack(bool slow);
    bool fadeFromBlack(bool slow);
    bool fadeToBlack();
    bool fadeFromBlack();

    void cutToBlack();
    void cutFromBlack();

    void setOverlayAboveCEGUI(bool above);

    void destroyAllWindows();
};

#endif
