#include "common.h"
#include "behavior_defs.h"


// => texture ids
uint G_DEF_FLY_TXT_ID       = 100;
uint G_DEF2_FLY_TXT_ID      = 101;
uint G_BOMB_TXT_ID			= 102;
uint G_BOMB_EXPL_TXT_ID		= 103;
uint G_CATAPULT_TXT_ID      = 200;
uint BU_CATAPULT_TXT_IDS[]  = {201, 202};
uint G_CANNON_TXT_ID        = 203;
uint BU_CANNON_TXT_IDS[]    = {204, 205};
uint G_POWERUP_TXT_ID       = 206;
uint BU_POWERUP_TXT_IDS[]   = {207};  
uint G_UNIT_TXT_ID          = 300;
uint G_ENEMY_UNIT_TXT_ID	= 301;
uint G_TANK_TXT_ID			= 400;
uint G_ENEMY_TANK_TXT_ID	= 401;
uint G_HOUSE_TXT_ID			= 402;
uint G_CURSOR1_TXT_ID		= 500;
uint G_CURSOR2_TXT_ID		= 501;
uint G_BUTTON_JOIN_TXT_ID   = 600;
uint G_BUTTON_SPLIT_TXT_ID  = 601;
uint G_UITLO_TXT_ID         = 602;
uint G_BUTTON_JOIN_CC_TXT_ID = 603;
uint G_BUTTON_JOIN_MM_TXT_ID = G_BUTTON_JOIN_TXT_ID;
uint G_BUTTON_SPLIT_CC_TXT_ID = 604;
uint G_BUTTON_SPLIT_MM_TXT_ID = G_BUTTON_SPLIT_TXT_ID;
uint G_BUTTON_GO_TXT_ID   = 610;
uint G_BUTTON_GO_CC_TXT_ID = 611;
uint G_BUTTON_GO_MM_TXT_ID = G_BUTTON_GO_TXT_ID;

// => texture templates
TxtOT G_DEF_FLY_TXT_TMPL        = TxtOT(G_DEF_FLY_TXT_ID, (BITMAP*)0, 1 /*Dir num*/, 1 /* Step num */, "bullet_chujowy.tga");
TxtOT G_DEF2_FLY_TXT_TMPL       = TxtOT(G_DEF2_FLY_TXT_ID, (BITMAP*)0, 1 /*Dir num*/, 1 /* Step num */, "bullet_chujowy2.tga");
TxtOT G_BOMB_TXT_TMPL       	= TxtOT(G_BOMB_TXT_ID, (BITMAP*)0, 1 /*Dir num*/, 1 /* Step num */, "bullet_chujowy.tga");
TxtOT G_BOMB_EXPL_TXT_TMPL		= TxtOT(G_BOMB_EXPL_TXT_ID, (BITMAP*)0, 1 /*Dir num*/, 1 /* Step num */, "explosion.tga");
TxtOT G_CATAPULT_TXT_TMPL       = TxtOT(G_CATAPULT_TXT_ID, (BITMAP*)0, 1 /*Dir num*/, 1 /* Step num */, "catapult.tga");
TxtOT BU_CATAPULT_TXT_TMPLS[]   = {};
TxtOT G_CANNON_TXT_TMPL         = TxtOT(G_CANNON_TXT_ID, (BITMAP*)0, 1 /*Dir num*/, 1 /* Step num */, "cannon.tga");
TxtOT BU_CANNON_TXT_TMPLS[]     = {};
TxtOT G_POWERUP_TXT_TMPL        = TxtOT(G_POWERUP_TXT_ID, (BITMAP*)0, 1 /*Dir num*/, 1 /* Step num */, "powerup.tga");
TxtOT BU_POWERUP_TXT_TMPLS[]    = {};
TxtOT G_UNIT_TXT_TMPL           = TxtOT(G_UNIT_TXT_ID, (BITMAP*)0, 8 /*Dir num*/, 10 /* Step num */, "walk_01.tga");
TxtOT G_ENEMY_UNIT_TXT_TMPL		= TxtOT(G_ENEMY_UNIT_TXT_ID, (BITMAP*)0, 8 /*Dir num*/, 10 /* Step num */, "walk_02.tga");
TxtOT G_TANK_TXT_TMPL			= TxtOT(G_TANK_TXT_ID, (BITMAP*)0, 1 /*Dir num*/, 3 /* Step num */, "tank.tga");
TxtOT G_ENEMY_TANK_TXT_TMPL		= TxtOT(G_ENEMY_TANK_TXT_ID, (BITMAP*)0, 1 /*Dir num*/, 3 /* Step num */, "enemy_tank.tga");
TxtOT G_CURSOR1_TXT_TMPL		= TxtOT(G_CURSOR1_TXT_ID, (BITMAP*)0, 1 /* Dir num */, 1 /* Step num */, "cursor1.tga");
TxtOT G_CURSOR2_TXT_TMPL		= TxtOT(G_CURSOR2_TXT_ID, (BITMAP*)0, 1 /* Dir num */, 1 /* Step num */, "planet.tga");
TxtOT G_BUTTON_JOIN_TXT_TMPL    = TxtOT(G_BUTTON_JOIN_TXT_ID, (BITMAP*)0, 1 /* Dir num */, 1 /* Step num */, "button_join.tga");
TxtOT G_BUTTON_SPLIT_TXT_TMPL   = TxtOT(G_BUTTON_SPLIT_TXT_ID, (BITMAP*)0, 1 /* Dir num */, 1 /* Step num */, "button_split.tga");
TxtOT G_UITLO_TXT_TMPL          = TxtOT(G_UITLO_TXT_ID, (BITMAP*)0, 1 /* Dir num */, 1 /* Step num */, "ui_tlo.tga");
TxtOT G_BUTTON_JOIN_CC_TXT_TMPL    = TxtOT(G_BUTTON_JOIN_CC_TXT_ID, (BITMAP*)0, 1 /* Dir num */, 1 /* Step num */, "button_join_cc.tga");
TxtOT G_BUTTON_SPLIT_CC_TXT_TMPL   = TxtOT(G_BUTTON_SPLIT_CC_TXT_ID, (BITMAP*)0, 1 /* Dir num */, 1 /* Step num */, "button_split_cc.tga");
TxtOT G_BUTTON_GO_TXT_TMPL    = TxtOT(G_BUTTON_GO_TXT_ID, (BITMAP*)0, 1 /* Dir num */, 1 /* Step num */, "button_go.tga");
TxtOT G_BUTTON_GO_CC_TXT_TMPL    = TxtOT(G_BUTTON_GO_CC_TXT_ID, (BITMAP*)0, 1 /* Dir num */, 1 /* Step num */, "button_go_cc.tga");
TxtOT G_HOUSE_TXT_TMPL           = TxtOT(G_HOUSE_TXT_ID, (BITMAP*)0, 1 /*Dir num*/, 1 /* Step num */, "house.tga");


TxtOT* G_TXT_TMPLS[] = {
	&G_DEF_FLY_TXT_TMPL,
	&G_DEF2_FLY_TXT_TMPL,
	&G_CATAPULT_TXT_TMPL,
	&G_CANNON_TXT_TMPL,
	&G_POWERUP_TXT_TMPL,
	&G_UNIT_TXT_TMPL,
	&G_ENEMY_UNIT_TXT_TMPL,
	&G_TANK_TXT_TMPL,
	&G_ENEMY_TANK_TXT_TMPL,
	&G_CURSOR1_TXT_TMPL,
	&G_CURSOR2_TXT_TMPL,
    &G_BUTTON_JOIN_TXT_TMPL,
    &G_BUTTON_SPLIT_TXT_TMPL,
    &G_UITLO_TXT_TMPL,
    &G_BUTTON_JOIN_CC_TXT_TMPL,
    &G_BUTTON_SPLIT_CC_TXT_TMPL,
	&G_BOMB_TXT_TMPL,
	&G_BUTTON_GO_TXT_TMPL,
	&G_BUTTON_GO_CC_TXT_TMPL,
	&G_BOMB_EXPL_TXT_TMPL,
	&G_HOUSE_TXT_TMPL

};

uint g_numTxtTmpls = (sizeof(G_TXT_TMPLS) / sizeof(G_TXT_TMPLS[0]));

// DEFINICJE template'ow
template int	clamp<int>(int val, const int& minVal, const int& maxVal);
template float	clamp<float>(float val, const float& minVal, const float& maxVal);

bool __DRAW_TG = false;

bool freezeUpdate = false;


void ext_blit(BITMAP* pBmpSrc,
			  BITMAP* pBmpDest, 
			  int source_x,
			  int source_y,
			  int source_width, 
			  int source_height,
			  int dest_x,
			  int dest_y,
			  int dest_width,
			  int dest_height)
{
	if (source_width == dest_width &&
		source_height == dest_height)
	{
		blit(
			pBmpSrc,	 		/** source */
			pBmpDest, 			/** destination */  
			source_x,			/** source_x */
			source_y,			/** source_y */
			dest_x,				/** dest_x */
			dest_y,				/** dest_y */
			source_width,	 	/** width */	
			source_height	 	/** height  */
		);
	}
	else
	{
		stretch_blit(
			pBmpSrc, 			/** source */
			pBmpDest,			/** destination */
			source_x,			/** source_x */
			source_y,			/** source_y */
			source_width,		/** source_width */ 
			source_height,		/** source_height */
			dest_x,				/** dest_x */
			dest_y,				/** dest_y */
			dest_width,			/** dest_width */
			dest_height			/** dest_height */
		);	
	}
}

void ext_masked_blit(BITMAP* pBmpSrc,
					 BITMAP* pBmpDest, 
					 int source_x,
					 int source_y,
					 int source_width, 
					 int source_height,
					 int dest_x,
					 int dest_y,
					 int dest_width,
					 int dest_height)
{
	if (source_width == dest_width &&
		source_height == dest_height)
	{
		masked_blit(
			pBmpSrc,	 		/** source */
			pBmpDest, 			/** destination */  
			source_x,			/** source_x */
			source_y,			/** source_y */
			dest_x,				/** dest_x */
			dest_y,				/** dest_y */
			source_width,	 	/** width */	
			source_height	 	/** height  */
		);
	}
	else
	{
		masked_stretch_blit(
			pBmpSrc, 			/** source */
			pBmpDest,			/** destination */
			source_x,			/** source_x */
			source_y,			/** source_y */
			source_width,		/** source_width */ 
			source_height,		/** source_height */
			dest_x,				/** dest_x */
			dest_y,				/** dest_y */
			dest_width,			/** dest_width */
			dest_height			/** dest_height */
		);	
	}
}

RECTANGLE make_rect(INT_COORDS a_ulVertex, int a_nWidth, int a_nHeight)
{
	RECTANGLE rect;

	rect.ulVertex.first  = a_ulVertex.first;
	rect.ulVertex.second = a_ulVertex.second;
	rect.drVertex.first  = a_ulVertex.first + a_nWidth - 1;
	rect.drVertex.second = a_ulVertex.second + a_nHeight - 1;

	return rect;
}

int sgn(long a_nVal)
{
	return (a_nVal < 0) ? -1 : 1;
}

int is_power2(long a_nVal)
{
	long nTest = 1;
	while (nTest < a_nVal)
	{
		nTest <<= 1;
	}

	if (nTest == a_nVal)
	{
		return 1;
	}

	return 0;
}

int is_on_same_hex_horiz_line(INT_COORDS a_v1, INT_COORDS a_v2)
{
	return (a_v1.first + a_v1.second == a_v2.first + a_v2.second);
}

int is_on_same_hex_vert_line(INT_COORDS a_v1, INT_COORDS a_v2)
{
	return IN_RANGE(a_v1.first - a_v1.second, a_v2.first - a_v2.second, 1);
}

int is_poly_rectangle(vector<INT_COORDS>& vVertices)
{
	return ((4 == vVertices.size()) &&
			(is_on_same_hex_horiz_line(vVertices[0], vVertices[1]) &&
			 is_on_same_hex_horiz_line(vVertices[3], vVertices[2]) &&
			 is_on_same_hex_vert_line(vVertices[0], vVertices[3]) &&
			 is_on_same_hex_vert_line(vVertices[1], vVertices[2])) ||
			(is_on_same_hex_vert_line(vVertices[0], vVertices[1]) &&
			 is_on_same_hex_vert_line(vVertices[3], vVertices[2]) &&
			 is_on_same_hex_horiz_line(vVertices[0], vVertices[3]) &&
			 is_on_same_hex_horiz_line(vVertices[1], vVertices[2]))); 
}

void transform_to_uniform_rect(vector<INT_COORDS>& vVertices)
{
	if (!is_poly_rectangle(vVertices))
	{
		return;
	}

	int nXMin = INT_MAX;
	int nYMin = INT_MAX;
	int nXMax = INT_MIN;
	int nYMax = INT_MIN;

	int nIndexXMin = UNDEFINED;
	int nIndexYMin = UNDEFINED;
	int nIndexXMax = UNDEFINED;
	int nIndexYMax = UNDEFINED;

	int i;
	vector<INT_COORDS>::iterator it;
	for (it = vVertices.begin(), i = 0; it != vVertices.end(); ++it, ++i)
	{
		INT_COORDS gridCoords = *it;
		int nX = gridCoords.first;
		int nY = gridCoords.second;

		if (nX < nXMin)
		{
			nXMin = nX;
			nIndexXMin = i;
		}
		if (nY < nYMin)
		{
			nYMin = nY;
			nIndexYMin = i;
		}
		if (nX > nXMax)
		{
			nXMax = nX;
			nIndexXMax = i;
		}
		if (nY > nYMax)
		{
			nYMax = nY;
			nIndexYMax = i;
		}
	}

	vVertices.push_back(vVertices[nIndexYMin]);
	vVertices.push_back(vVertices[nIndexXMin]);
	vVertices.push_back(vVertices[nIndexYMax]);
	vVertices.push_back(vVertices[nIndexXMax]);

	vVertices.erase(vVertices.begin(), vVertices.begin() + 4);

	// check distance +/- 1
}

int dist(INT_COORDS p1, INT_COORDS p2)
{
	return (int)sqrtf((p1.first - p2.first) * (p1.first - p2.first) + 
					  (p1.second - p2.second) * (p1.second - p2.second));
}

const pt3f getCellCenterCoords(INT_COORDS a_gridElem)
{
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();
	GameView* pView = pConf->m_getGameViewByUserId(DEFAULT_USER_ID);

	const int nGridElemSizeX = pView->m_getGridElemSizeX();
	const int nGridElemSizeY = pView->m_getGridElemSizeY();

	INT_COORDS ovCoords = pView->m_mapGridElementToOverallViewCoords(a_gridElem);

	return pt3f((float)ovCoords.first,
				(float)ovCoords.second,	 
				0.f);
}

const pt3f getCellLUCoords(INT_COORDS a_gridElem)
{
	const uint cellSizeX = (uint)getCellSizeX();
	const uint cellSizeY = (uint)getCellSizeY();

	pt3f coords = getCellCenterCoords(a_gridElem);

	coords -= pt3f((float)(cellSizeX >> 1),
				   (float)(cellSizeY >> 2),
				    0.f);

	return coords;
}

float dist(const pt3f& a_pt1, const pt3f& a_pt2)
{
	return sqrtf((a_pt1.x - a_pt2.x) * (a_pt1.x - a_pt2.x) +
				 (a_pt1.y - a_pt2.y) * (a_pt1.y - a_pt2.y) +
				 (a_pt1.z - a_pt2.z) * (a_pt1.z - a_pt2.z));
}

const float getCellSizeX()
{
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();
	GameView* pView = pConf->m_getGameViewByUserId(DEFAULT_USER_ID);

	return (float)(pView->m_getGridElemSizeX());
}

const float getCellSizeY()
{
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();
	GameView* pView = pConf->m_getGameViewByUserId(DEFAULT_USER_ID);

	return (float)(pView->m_getGridElemSizeY());
}

INT_COORDS getCellForPos(const pt3f& a_pos)
{
	if (a_pos.x < 0.f || a_pos.y < 0.f)
		return INVALID_COORDS;

	assert(a_pos.x >= 0.f && 
		   a_pos.y >= 0.f);

	INT_COORDS cell = INVALID_COORDS;

	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();
	GameView* pView = pConf->m_getGameViewByUserId(DEFAULT_USER_ID);

	const uint nCellSizeX = pView->m_getGridElemSizeX();
	const uint nCellSizeY = pView->m_getGridElemSizeY();
	const uint nNumCellsX = pGrid->m_getNumCellX();
	const uint nNumCellsY = pGrid->m_getNumCellY();
	RECTANGLE gameRect = pView->m_getGameRectangle();

	assert(nNumCellsX == nNumCellsY);

	INT_COORDS ipos = make_pair((int)a_pos.x, (int)a_pos.y); 
	const uint nDivV = (nCellSizeY >> 2) * 3; 

	const uint nRowNum = (uint)(ipos.second / nDivV);        
#ifdef __TRACE_ME_
	cout << "nRowNum: " << nRowNum << endl;  
#endif

	const uint numCellsForThisRow = (nRowNum < (nNumCellsX - 1)) ? 
										(nRowNum + 1) :
										((nNumCellsX - 1) - (nRowNum - nNumCellsX));

	const uint xMid = ((gameRect.ulVertex.first + gameRect.drVertex.first) >> 1);
	const uint numCellsFTRDiv2 = (numCellsForThisRow >> 1);

	uint xMostLeftCellFTR = xMid - numCellsFTRDiv2 * nCellSizeX; 
	if (nRowNum % 2 == 0)
		xMostLeftCellFTR -= (nCellSizeX >> 1);

	if (ipos.first < xMostLeftCellFTR)
		return INVALID_COORDS;

	uint xMostRightCellFTR = xMid + numCellsFTRDiv2 * nCellSizeX;
	if (nRowNum % 2 == 0)
		xMostRightCellFTR += (nCellSizeX >> 1);

	if (ipos.first > xMostRightCellFTR)
		return INVALID_COORDS;

	const uint nNumCellInRowFromL = (ipos.first - xMostLeftCellFTR) / nCellSizeX;

	// ok mamy: nRowNum i nNumCellInRow
	cell.first 	= nRowNum - nNumCellInRowFromL;
	cell.second	= 0 + nNumCellInRowFromL;
	
	return cell;
}

GridElement* getGridElement(INT_COORDS a_coords)
{
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();

	return const_cast<GridElement*>(pGrid->m_getGridElement(a_coords));
}

bool posOutsideGrid(const pt3f& a_pos)
{
    return !posInsideGrid(a_pos);
}

bool posInsideGrid(const pt3f& a_pos)
{
	return (getCellForPos(a_pos) != INVALID_COORDS);
}

ostream& operator<<(ostream& out, const pt3f& pt)
{
	out << " (" << pt.x << " " << pt.y << " " << pt.z << ") ";
	return out;
}

bool operator==(const pt3f& pt1, const pt3f& pt2)
{
	return (pt1.x == pt2.x && pt1.y == pt2.y && pt1.z == pt2.z);
}

bool operator!=(const pt3f& pt1, const pt3f& pt2)
{
	return !(pt1 == pt2);
}

pt3f operator-(const pt3f& pt1, const pt3f& pt2)
{
	pt3f diff;

	diff.x = pt1.x - pt2.x;
	diff.y = pt1.y - pt2.y;
	diff.z = pt1.z - pt2.z;

	return diff;
}

pt3f operator+(const pt3f& a_pt1, const pt3f& a_pt2)
{
	pt3f sum;

	sum.x = a_pt1.x + a_pt2.x;
	sum.y = a_pt1.y + a_pt2.y;
	sum.z = a_pt1.z + a_pt2.z;

	return sum;
}

pt3f operator*(const pt3f& a_pt, const float a_f)
{
	pt3f mult;

	mult.x = a_pt.x * a_f;
	mult.y = a_pt.y * a_f;
	mult.z = a_pt.z * a_f;

	return mult;
}

void drawBmp(const int a_nUserId,
			 BITMAP* a_pDestBmp,
			 BITMAP* a_pSrcBmp,
			 INT_COORDS a_luCell,
			 const vec3f& delta,
			 const uint width,
			 const uint height)
{	
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();
	GameView* pView = pConf->m_getGameViewByUserId(a_nUserId);

	const uint cellSizeX = (uint)getCellSizeX();
	const uint cellSizeY = (uint)getCellSizeY();

    INT_COORDS ovLUCoords = pView->m_mapGridElementToOverallViewCoords(a_luCell);
	if (pView->m_isPointWithingClipRectangle(ovLUCoords))
	{
		INT_COORDS coords = pView->m_mapFromOverallToViewCoords(ovLUCoords);
		ext_masked_blit(
			a_pSrcBmp, 
			a_pDestBmp,
			0,
			0, 	
			a_pSrcBmp->w,
			a_pSrcBmp->h,
			coords.first - (cellSizeX >> 1) + (int)delta.x,
			coords.second - (cellSizeY >> 2) + (int)delta.y,
			width * cellSizeX,
			height * cellSizeY);					
	}

	return;
}

void drawBmp(const int a_nUserId,
	   		 BITMAP* a_pDestBmp,
			 BITMAP* a_pSrcBmp,
			 cpt3f& a_pos,
			 const float width,
			 const float height,
			 bool whInCells,
			 bool mapFromOvToView)
{
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();
	GameView* pView = pConf->m_getGameViewByUserId(a_nUserId);

	const float cellSizeX = getCellSizeX();
	const float cellSizeY = getCellSizeY();

	INT_COORDS coords = INVALID_COORDS;
	if (!mapFromOvToView)
	{
		coords = mp((int)(a_pos.x), (int)(a_pos.y));
	}
	else
	{
		assert((GameView*)0 != pView);

		INT_COORDS ovLUCoords = mp((int)(a_pos.x), (int)(a_pos.y));
		if (!pView->m_isPointWithingClipRectangle(ovLUCoords))
			return;

		coords = pView->m_mapFromOverallToViewCoords(ovLUCoords);
	}

	int widthInPxls 	= (int)(width * ((whInCells) ? cellSizeX : 1.f));
	int heightInPxls 	= (int)(height *((whInCells) ? cellSizeY : 1.f));
	ext_masked_blit(
		a_pSrcBmp, 
		a_pDestBmp,
		0,
		0, 	
		a_pSrcBmp->w,
		a_pSrcBmp->h,
		coords.first - (widthInPxls >> 1),
		coords.second - (heightInPxls >> 1),
		(int)(width * ((whInCells) ? cellSizeX : 1.f)),
		(int)(height * ((whInCells) ? cellSizeY : 1.f)));					

	return;
}

bool makeTextureObj(const int a_txtId, TextureObject& a_txtObj)
{
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();

	TextureObjTmpl txtTmpl;
	if (!pConf->m_getTextureObjById2(a_txtId, txtTmpl))
		return false;

	a_txtObj.m_initFromTemplate(txtTmpl);

	return true;
}

Layer* getDefaultLayer()
{
    return const_cast<Layer*>(Layer::m_getLayerAtLevel(DEFAULT_LAYER_LEVEL));
}

template <class A, class B> pair<A, B> mp(const A& a, const B& b)
{
	return make_pair(a, b);
}

template INT_COORDS mp(const int& a, const int& b);

template <typename T> T clamp(T val, const T& minVal, const T& maxVal)
{
    if (val < minVal) 
		return minVal;

	if (val > maxVal)
		return maxVal;

	return val;
}

pt3f mapOverallToViewCoords(const int& a_nUserId, cpt3f& ovCoords)
{
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();
	GameView* pView = pConf->m_getGameViewByUserId(a_nUserId);

	INT_COORDS iovCoords = make_pair((int)(ovCoords.x), (int)(ovCoords.y));
	if (pView->m_isPointWithingClipRectangle(iovCoords))
	{
		INT_COORDS icoords = pView->m_mapFromOverallToViewCoords(iovCoords);
		return pt3f((float)(icoords.first), (float)(icoords.second), 0.f);
	}

	return INVALID_POS;
}

INT_COORDS mapOverallToViewCoords(const int& a_nUserId, const INT_COORDS& iovCoords)
{
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();
	GameView* pView = pConf->m_getGameViewByUserId(a_nUserId);

    if (pView->m_isPointWithingClipRectangle(iovCoords))
	{
		INT_COORDS icoords = pView->m_mapFromOverallToViewCoords(iovCoords);
		return icoords;
	}

	return INVALID_COORDS;
}

float ranf()
{
	return (rand() / ((float)(RAND_MAX + 1.f)));
}

void draw_line(const int& a_nUserId, BITMAP* a_pDestBmp, cpt3f& objPos , cvec3f& vec, const int& col)
{
	INT_COORDS startCoords = mapOverallToViewCoords(a_nUserId, mp((int)objPos.x, (int)objPos.y));
	INT_COORDS endCoords = mapOverallToViewCoords(
		a_nUserId, mp((int)(objPos.x + vec.x), (int)(objPos.y + vec.y)));

	if (INVALID_COORDS != startCoords && INVALID_COORDS != endCoords)
	{
		line(a_pDestBmp,
			 startCoords.first,
			 startCoords.second,
			 endCoords.first,
			 endCoords.second,
			 col);
	}

	return;
}

void draw_circle(const int& a_nUserId, BITMAP* a_pDestBmp, cpt3f& center, cfloat& rad, const int& col, bool fill)
{
	INT_COORDS coords = mapOverallToViewCoords(a_nUserId, mp((int)(center.x), (int)(center.y)));

	if (INVALID_COORDS != coords)
	{
		if (fill)
		{
			circlefill(a_pDestBmp, 
					   coords.first,
					   coords.second,
					   (int)rad,
					   col);
		}
		else
		{
			circle(a_pDestBmp, 
				   coords.first,
				   coords.second,
				   (int)rad,
				   col);
		}
	}

	return;
}

ostream& operator<<(ostream& os, const RECTANGLE& a_rect)
{
	os << "X1: " << a_rect.x1() << " Y1: " << a_rect.y1() << " X2: " << a_rect.x2() << " Y2: " << a_rect.y2();   
}

GameView* getDefaultView()
{
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();
	GameView* pView = pConf->m_getGameViewByUserId(DEFAULT_USER_ID);

	return pView;
}

float DEG2RAD(cfloat& a_deg)
{
	return (a_deg * M_PI / 280.f);    
}

float RAD2DEG(cfloat& a_rad)
{
	return (a_rad * 180.f / M_PI);
}

float operator%(cvec3f& a_vec1, cvec3f& a_vec2)
{
	return ((a_vec1.x * a_vec2.x) + (a_vec1.y * a_vec2.y) + (a_vec1.z * a_vec2.z));
}

float pt3f::getAngle(cvec3f& a_otherVec, float& a_angle, float& a_sign) const
{
	cvec3f thisN = (*this).normalize();
	cvec3f otherVecN = a_otherVec.normalize();

	cfloat ang2OtherR = acosf(thisN % otherVecN);	
	cfloat ang2OtherD = RAD2DEG(ang2OtherR);

	cvec3f thisOrthoLN = vec3f(-thisN.y, thisN.x, thisN.z);
	cfloat dot = thisOrthoLN % otherVecN;

    cfloat sign = (dot < 0.f) ?
				  -1.f /* docelowy wektor na prawo, trzeba sie obrocic w prawo */ :
				  +1.f /* docelowy wektor na lewo, trzeba sie obrocic w lewo */;

	a_angle = ang2OtherR;
	a_sign  = sign;

	return ang2OtherR;
}

vec3f& pt3f::rotateZ(cfloat& angle)
{
	// (dodatnia wartosc kata oznacza obrot w lewo - ujemna w prawo):

	cfloat oldX = x;
	cfloat oldY = y;

	x = oldX * cosf(angle) - oldY * sinf(angle);
	y = oldX * sinf(angle) + oldY * cosf(angle);

	return *this;
}

vec3f operator-(cvec3f& a_vec)
{
	return vec3f(-a_vec.x, -a_vec.y, -a_vec.z);
}


int getDiffs(INT_COORDS a_cell, uint a_dist, INT_COORDS_VEC& a_nearCells)
{
	assert(a_cell != INVALID_COORDS);

	a_nearCells.clear();

	if (a_dist == 0)
		return 0;

    int i;
	for (i = 1; i < a_dist; ++i)
	{
		INT_COORDS_VEC diffsVec;
		MovementManager::m_getDiffs(i, diffsVec);

		int j;
		for (j = 0; j < diffsVec.size(); ++j)
		{
			INT_COORDS diff = diffsVec[j];

			INT_COORDS cell = mp(a_cell.first + diff.first, a_cell.second + diff.second);
			if (MovementManager::m_isValid(cell))
			{
				a_nearCells.push_back(cell);
			}
		}
	}

	return a_nearCells.size();
}

float TO_360(cfloat& angleD, cfloat& sign)
{
	if (sign == 1.f)
		return angleD;

	assert(sign == -1.f);
	return (360.f - angleD);
}

void placeObj(PlaceableObj* a_pPlObj, cpt3f& a_pos, bool a_place)
{
	assert((PlaceableObj*)0 != a_pPlObj);

	cpt3f destPos = a_pos;
    cfloat widthCC = a_pPlObj->m_getWidth();
	cfloat heightCC = a_pPlObj->m_getHeight();
	assert(INVALID_POS != destPos && ((widthCC > 0.f) && (heightCC > 0.f)));

	INT_COORDS ctrCELL = getCellForPos(destPos);
	assert(ctrCELL != INVALID_COORDS);

	GridElement* pEl = getGridElement(ctrCELL);
	pEl->m_setCovered(a_pPlObj, a_place);

	uint cellRAD = (cuint)((widthCC + heightCC) * .5f);
	cellRAD = (cellRAD == 0) ? 0 : (cellRAD - 1);
	INT_COORDS_VEC covCells;
	getDiffs(ctrCELL, cellRAD, covCells);

	int i;
	for (i = 0; i < covCells.size(); ++i)
	{
		INT_COORDS cell = covCells[i];
		GridElement* pEl = getGridElement(cell);
		pEl->m_setCovered(a_pPlObj, a_place);
	}
		
	return;
}

void placeObj(PlaceableObj* a_pPlObj)
{
	assert((PlaceableObj*)0 != a_pPlObj);
	placeObj(a_pPlObj, a_pPlObj->m_getPos(), true);
}


// >>>>>>> BEHAVIORS <<<<<<<<<<<
pair<ulong, string> tblBehId2Name[] =
{
	pair<ulong, string>(E_BEH_UNDEF, "UNDEF"),
	pair<ulong, string>(E_BEH_RETREAT_FROM_HEAVY_ATTACK, "RETREAT_FROM_HEAVY_ATTACK"),
	pair<ulong, string>(E_BEH_TRACE_ATTACKED_ETG, "TRACE_ATTACKED_ETG"),
	pair<ulong, string>(E_BEH_ATTACK_WEAK_ETGS, "ATTACK_WEAK_ETGS"),
	pair<ulong, string>(E_BEH_ATTACK_STRONG_ETGS, "ATTACK_STRONG_ETGS"),
	pair<ulong, string>(E_BEH_ATTACK_HLP_ATTACKED_FRTGS, "ATTACK_HLP_ATTACKED_FRTGS"),
	pair<ulong, string>(E_BEH_JOIN_NEAR_FRTGS, "JOIN_NEAR_FRTGS"),
	pair<ulong, string>(E_BEH_SPLIT_WHEN_MANY_NEAR_ETGS, "SPLIT_WHEN_MANY_NEAR_ETGS"),
	pair<ulong, string>(E_BEH_SPLIT_WHEN_OVERFORCE, "SPLIT_WHEN_OVERFORCE"),
};

map<ulong, string> mapBehId2Name;

string getBehaviorName(ulong behId)
{
	if (mapBehId2Name.empty())
	{
		const uint nEls = (sizeof(tblBehId2Name) / sizeof(tblBehId2Name[0]));
		mapBehId2Name.insert(tblBehId2Name, tblBehId2Name + nEls);
	}

	return mapBehId2Name[behId];
}

vec3f interpolateVec(cvec3f& a_stVec, cvec3f& a_endVec, cfloat& a_step /* DEG */)
{
	if (UNCOUNT_VEC(a_stVec) || UNCOUNT_VEC(a_endVec))
	{
		return INVALID_VEC;
	}

	cfloat step = clampf(a_step, 1.f, 30.f);

	vec3f resVec = a_stVec;

	float angleR, sign;
	resVec.getAngle(a_endVec, angleR, sign);
	float angleD = RAD2DEG(angleR);
	if (angleD < a_step)
		return a_endVec;

	resVec = resVec.rotateZ(sign * step);

	return resVec;
}

