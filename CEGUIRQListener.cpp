#include "CEGUIRQListener.h"

CEGUIRQListener::CEGUIRQListener(CEGUI::OgreRenderer* renderer, Ogre::uint8 queue_id, bool post_queue) :
        d_renderer(renderer),
        d_queue_id(queue_id),
        d_post_queue(post_queue)
   {
   }

    void CEGUIRQListener::renderQueueStarted(Ogre::uint8 id, const Ogre::String& invocation, bool& skipThisQueue)
    {
        if (!d_post_queue && d_queue_id == id)
            CEGUI::System::getSingleton().renderGUI();
    }

    void CEGUIRQListener::renderQueueEnded(Ogre::uint8 id, const Ogre::String& invocation, bool& repeatThisQueue)
    {
        if (!d_post_queue || d_queue_id != id)
            return;

        // need to reset Ogre renderer viewport here for whichever vp is being drawn.
        Ogre::Viewport* vp = Ogre::Root::getSingleton().getRenderSystem()->_getViewport();

        int left, top, width, height;
        vp->getActualDimensions(left, top, width, height);

        const CEGUI::Rect vp_area(left, top, left + width, top + height);
        dynamic_cast<CEGUI::OgreRenderTarget&>(
            d_renderer->getDefaultRenderingRoot().getRenderTarget()).
                setArea(vp_area);

        CEGUI::System::getSingleton().renderGUI();
    }

    // methods for adjusting target queue settings
    void CEGUIRQListener::setTargetRenderQueue(Ogre::uint8 queue_id)
    {
        d_queue_id = queue_id;
    }

    void CEGUIRQListener::setPostRenderQueue(bool post_queue)
    {
        d_post_queue = post_queue;
    }
