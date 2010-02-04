#include "lines.h"

#include <math.h>

Lines::Lines( SceneManager *mSceneMgr )
	:mSceneMgr(mSceneMgr)
{
    lines = new DynamicLines(RenderOperation::OT_LINE_LIST);

    linesNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("lines");
    linesNode->attachObject(lines);
}

void Lines::addLine(  Vector3 *a, Vector3 *b )
{
    lines->addPoint( *a );
    lines->addPoint( *b );
    lines->update();
    linesNode->needUpdate();
}

void Lines::addLine( double ax, double ay, double az, double bx, double by, double bz)
{
    lines->addPoint( ax,ay,az );
    lines->addPoint( bx,by,bz );
}

void Lines::addCross(  Vector3 *a )
{
    lines->addPoint( a->x-1000,a->y,a->z );
    lines->addPoint( a->x+1000,a->y,a->z );
    
    lines->addPoint( a->x,a->y-1000,a->z );
    lines->addPoint( a->x,a->y+1000,a->z );
    
    lines->addPoint( a->x,a->y,a->z-1000 );
    lines->addPoint( a->x,a->y,a->z+1000 );

    lines->update();
    linesNode->needUpdate();
}


void Lines::addPoly(  Vector3 *a, Vector3 *b, Vector3 *c )
{
    lines->addPoint( *a );
    lines->addPoint( *b );
    
    lines->addPoint( *b );
    lines->addPoint( *c );
    
    lines->addPoint( *c );
    lines->addPoint( *a );
    
    lines->update();
    linesNode->needUpdate();
}

void Lines::clear()
{
    lines->clear();
    lines->update();
    linesNode->needUpdate();
}

void Lines::clearWithoutUpdate()
{
    lines->clear();
}
