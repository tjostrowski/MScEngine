#include "common_header.h"

#include "BasicObject.h"
#include "Layer.h"
#include "MapGrid.h"
#include "DrawableObject.h"
#include "GridElement.h"
#include "GridArea.h"
#include "Position.h"
#include "BasicConfiguration.h"
#include "HexagonalMapGrid.h"
#include "TextureObject.h"
#include "TexturePattern.h"
#include "GameView.h"
#include "ConfigurationReader.h"
#include "ConfigurationWriter.h"
#include "MovementManager.h"
#include "Group.h"
#include "Formation.h"
#include "Behavior.h"
#include "PatrolBehavior.h"
#include "TraceBehavior.h"
#include "EscapeBehavior.h"
#include "AttackBehavior.h"
#include "BasicObjectParamsManager.h"
#include "BasicObjectWorldInfoFrame.h"
#include "BasicObjectWorldInfoManager.h"
#include "BasicObjectCommunicationManager.h"
#include "BasicObjectBehaviorManager.h"
#include "BasicObjectDecisionManager.h"
#include "TimeFrameScheduledObject.h"
#include "Gadget.h"
#include "TacticalGroup.h"
#include "TacticalGroupsManager.h"
#include "LineMove.h"
#include "UI.h"
#include "Flock.h"

#include <limits.h>
#include <algorithm>
#include <functional>
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//#include <time.h>
#include <ctime>

/*#ifndef __BITMAP_
#define __BITMAP_
struct BITMAP
{
    int w, h;   //size of the bitmap in pixels
    int clip;   // non-zero if clipping is turned on
    int cl, cr, ct, cb;  // clip rectangle left, right, top,
                         //     and bottom
    unsigned char *line[];  // pointers to the start of each line
};
#endif // __BITMAP_*/

extern void ext_blit(BITMAP* pBmpSrc,
					 BITMAP* pBmpDest, 
					 int source_x,
					 int source_y,
					 int source_width, 
					 int source_height,
					 int dest_x,
					 int dest_y,
					 int dest_width,
					 int dest_height);

extern void ext_masked_blit(BITMAP* pBmpSrc,
					 BITMAP* pBmpDest, 
					 int source_x,
					 int source_y,
					 int source_width, 
					 int source_height,
					 int dest_x,
					 int dest_y,
					 int dest_width,
					 int dest_height);

extern int  sgn(long a_nVal);

extern int  is_power2(long a_nVal);

extern int  is_poly_rectangle(vector<INT_COORDS>& vVertices);

// transforms vertices so the order in vVertices is:
// [0] - upper left 
// [1] - upper right
// [2] - down right
// [3] - down left
extern void transform_to_uniform_rect(vector<INT_COORDS>& vVertices);

#define IN_RANGE(X, Y, A) (((X) >= (Y) - (A)) && ((X) <= (Y) + (A)))

struct LINE
{
	FLOAT_COORDS v_p1,
	             v_p2;
};

extern RECTANGLE make_rect(INT_COORDS a_ulVertex, int a_nWidth, int a_nHeight);

/*extern int getr(int c);
extern int getg(int c);
extern int getb(int c);

extern void     destroy_bitmap(BITMAP* pBmp);
extern BITMAP*  create_bitmap(int nWidth, int nHeight);
extern int      makecol(int a_nRed, int a_nGreen, int a_nBlue);
extern void     clear_to_color(BITMAP* a_pBmp, int a_nColor);
extern void     rectfill(BITMAP *a_pBmp, int a_nX1, int a_nY1, int a_nX2, int a_nY2, int a_nColor);
extern int      bitmap_mask_color(BITMAP *a_pBmp);
extern BITMAP*  load_bitmap(const char *a_strFilename, RGB *a_pPal);
//extern BITMAP*  load_bitmap(const char *filename, RGB *pal);

extern BITMAP* screen;*/

bool makeTextureObj(const int a_txtId, TextureObject& a_txtObj);

#define __TRACE_ME

