#include "guiManager.h"

GuiManager::GuiManager()
{
    CEGUI::OgreRenderer::bootstrapSystem();
    CEGUI::Imageset::setDefaultResourceGroup("imagesets");
    CEGUI::Font::setDefaultResourceGroup("fonts");
    CEGUI::Scheme::setDefaultResourceGroup("schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
    CEGUI::WindowManager::setDefaultResourceGroup("layouts");
    //CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");

    CEGUI::XMLParser *parser = CEGUI::System::getSingleton().getXMLParser();
    if(parser->isPropertyPresent("SchemaDefaultResourceGroup")) {
        parser->setProperty("SchemaDefaultResourceGroup","schemas");
    }

    guiMgr = CEGUI::WindowManager::getSingletonPtr();

    // Set the scheme we want to use for skinning the gui and getting layouts and stuff
    //CEGUI::SchemeManager::getSingleton().create("BlackComrade.scheme");
    CEGUI::SchemeManager::getSingleton().create("BlackComrade.scheme");
    CEGUI::FontManager::getSingleton().create("DejaVuSans-6.font");

    // Create the overall frame to add windows to
    guiRoot = guiMgr->createWindow("DefaultWindow","root");
    CEGUI::System::getSingleton().setGUISheet(guiRoot);

    // Add the crosshair to the default frame and centre it
    crossHair = static_cast<CEGUI::FrameWindow*>(guiMgr->createWindow("BlackComrade/CrossHair","crossHair"));
    guiRoot->addChildWindow(crossHair);
    crossHair->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5f,0),CEGUI::UDim(0.5f,0)));

    // Add output window to screen
    status = static_cast<CEGUI::Editbox*>(guiMgr->createWindow("BlackComrade/Editbox","status"));
    guiRoot->addChildWindow(status);
    status->setSize(CEGUI::UVector2(CEGUI::UDim(0.1f,0),CEGUI::UDim(0.05f,0)));
    status->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05f,0),CEGUI::UDim(0.05f,0)));
}

GuiManager::~GuiManager(){}

void GuiManager::setStatus(std::string stat) {
    status->setText(stat);
}
