
#include "rect2D.h"

Rect2D::Rect2D(String &name, Real width, Real height)
    : ManualObject(name)
{
    setRenderQueueGroup(RENDER_QUEUE_OVERLAY);
    
    setUseIdentityProjection(true);
    setUseIdentityView(true);
    setQueryFlags(0);
    
    clear();
    begin("",RenderOperation::OT_TRIANGLE_STRIP);
    position(0.0,0.0,-1.0);
    position(width,0.0,-1.0);
    position(width,height,-1.0);
    position(0.0,height,-1.0);
    position(0.0,0.0,-1.0);
    end();
    
    AxisAlignedBox *box = new AxisAlignedBox();
    box->setInfinite();
    setBoundingBox(*box);
    
    
}
