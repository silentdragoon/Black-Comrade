#ifndef CEGUIRQLISTENER_H
#define CEGUIRQLISTENER_H

#include <OGRE/Ogre.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/CEGUIBase.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderTarget.h>

class CEGUIRQListener : public Ogre::RenderQueueListener
{

private:
    CEGUI::OgreRenderer* d_renderer;
    Ogre::uint8 d_queue_id;
    bool d_post_queue;

public:
    CEGUIRQListener(CEGUI::OgreRenderer* renderer, Ogre::uint8 queue_id, bool post_queue);

    void renderQueueStarted(Ogre::uint8 id, const Ogre::String& invocation, bool& skipThisQueue);

    void renderQueueEnded(Ogre::uint8 id, const Ogre::String& invocation, bool& repeatThisQueue);

    // methods for adjusting target queue settings
    void setTargetRenderQueue(Ogre::uint8 queue_id);

    void setPostRenderQueue(bool post_queue);
};

#endif
