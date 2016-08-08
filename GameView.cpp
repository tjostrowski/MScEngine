#include "common.h"

GameView::GameView(int a_nUserId)
{
	v_nUserId = a_nUserId;
}

bool GameView::m_isObjectVisible(BasicObject* pObj)
{
	BasicConfiguration* pConf	 = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* 			pGrid 	 = pConf->m_getGridMap();
	vector<INT_COORDS> vVertices = pObj->m_getVertices();

	vector<INT_COORDS>::const_iterator it;
	for (it = vVertices.begin(); it != vVertices.end(); ++it)
	{
		INT_COORDS coords = pGrid->m_mapGridElementToCoords(*it);
		if (m_isPointWithingClipRectangle(coords))
		{
			return true;
		}
	}

	return false;
}

void GameView::init(int a_nGridElemSizeX,
					int a_nGridElemSizeY,
					RECTANGLE a_rect)
{
	int nGridElemSizeX = (a_nGridElemSizeX % 4 != 0) ? (a_nGridElemSizeX - (a_nGridElemSizeX % 4)) :
						  a_nGridElemSizeX; 
    int nGridElemSizeY = (a_nGridElemSizeY % 4 != 0) ? (a_nGridElemSizeY - (a_nGridElemSizeY % 4)) :
						  a_nGridElemSizeY;
	v_nGridElemSizeX = nGridElemSizeX;
	v_nGridElemSizeY = nGridElemSizeY;
	v_clippingRectangle = a_rect;

	v_nWidth  = a_rect.drVertex.first - a_rect.ulVertex.first;
	v_nHeight = a_rect.drVertex.second - a_rect.ulVertex.second; 
}

INT_COORDS GameView::m_mapFromOverallToViewCoords(INT_COORDS a_coords)
{
	INT_COORDS resCoords;
	
	resCoords.first 	= a_coords.first - v_clippingRectangle.ulVertex.first;
	resCoords.second	= a_coords.second - v_clippingRectangle.ulVertex.second;

	return resCoords;
}

bool GameView::m_isPointWithingClipRectangle(INT_COORDS a_coords)
{
	return (a_coords.first >= v_clippingRectangle.ulVertex.first &&
			a_coords.first <= v_clippingRectangle.drVertex.first &&
			a_coords.second >= v_clippingRectangle.ulVertex.second &&
			a_coords.second <= v_clippingRectangle.drVertex.second); 
}

bool GameView::m_isRectangleCrossClipRectangle(RECTANGLE a_rect)
{
	int nClipWidthDiv2	= (v_clippingRectangle.getWidth() >> 1);
	int nClipHeightDiv2	= (v_clippingRectangle.getHeight() >> 1);  
	int nWidthDiv2		= (a_rect.getWidth() >> 1);
	int nHeightDiv2 	= (a_rect.getHeight() >> 1);
	INT_COORDS clipMid	= make_pair(v_clippingRectangle.ulVertex.first + nClipWidthDiv2,
									v_clippingRectangle.ulVertex.second + nClipHeightDiv2);
	INT_COORDS mid		= make_pair(a_rect.ulVertex.first + nWidthDiv2,
									a_rect.ulVertex.second + nHeightDiv2);	

	if (abs(clipMid.first - mid.first) < nClipWidthDiv2 + nWidthDiv2 &&
		abs(clipMid.second - mid.second) < nClipHeightDiv2 + nHeightDiv2)
	{
		return true;
	}

	return false;
}

void GameView::m_move(int a_nDx, int a_nDy)
{
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* 			pGrid = pConf->m_getGridMap();
	RECTANGLE 		gameRect  = m_getGameRectangle();	
	
    /*int nDx = (a_nDx > 0) ? min(gameRect.drVertex.first - v_clippingRectangle.drVertex.first, a_nDx) :
							min(v_clippingRectangle.ulVertex.first - gameRect.ulVertex.first, a_nDx);
	int nDy = (a_nDy > 0) ? min(gameRect.drVertex.second - v_clippingRectangle.drVertex.second, a_nDy) :
							min(v_clippingRectangle.ulVertex.second - gameRect.ulVertex.second, a_nDy);
*/

	int nWidth  = v_nWidth;
	int nHeight = v_nHeight; 

	v_clippingRectangle.ulVertex.first 	+= a_nDx;
	v_clippingRectangle.ulVertex.second += a_nDy;
	v_clippingRectangle.drVertex.first 	+= a_nDx;
	v_clippingRectangle.drVertex.second += a_nDy;

	// clipping
	if (v_clippingRectangle.ulVertex.first < gameRect.ulVertex.first)
	{
		v_clippingRectangle.ulVertex.first = gameRect.ulVertex.first;
		v_clippingRectangle.drVertex.first = gameRect.ulVertex.first + nWidth;
	}
	if (v_clippingRectangle.drVertex.first > gameRect.drVertex.first)
	{	
		v_clippingRectangle.drVertex.first = gameRect.drVertex.first;
		v_clippingRectangle.ulVertex.first = gameRect.drVertex.first - nWidth;
	}
	if (v_clippingRectangle.ulVertex.second < gameRect.ulVertex.second)
	{	
		v_clippingRectangle.ulVertex.second = gameRect.ulVertex.second;
		v_clippingRectangle.drVertex.second = gameRect.ulVertex.second + nHeight;
	}
	if (v_clippingRectangle.drVertex.second > gameRect.drVertex.second)
	{	
		v_clippingRectangle.drVertex.second = gameRect.drVertex.second;
		v_clippingRectangle.ulVertex.second = gameRect.drVertex.second - nHeight;
	}

	/*cout << " " << endl;
	cout << "v_clippingRectangle.ulVertex.first "  << v_clippingRectangle.ulVertex.first  << endl;
	cout << "v_clippingRectangle.ulVertex.second " << v_clippingRectangle.ulVertex.second << endl;
	cout << "v_clippingRectangle.drVertex.first "  << v_clippingRectangle.drVertex.first  << endl;
	cout << "v_clippingRectangle.drVertex.second " << v_clippingRectangle.drVertex.second << endl;*/
}

RECTANGLE GameView::m_getGameRectangle()
{
	RECTANGLE rect;
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* 			pMap = pConf->m_getGridMap();
	int nNumCellX = pMap->m_getNumCellX();
	int nNumCellY = pMap->m_getNumCellY();

    rect.ulVertex.first 	= rect.ulVertex.second = 0;
	rect.drVertex.first 	= nNumCellX * v_nGridElemSizeX;
	rect.drVertex.second	= (nNumCellY - 1) * 3 * (v_nGridElemSizeY >> 1) +
							  v_nGridElemSizeY; 

	return rect;
}

// mapping 'center' of grid element
INT_COORDS GameView::m_mapGridElementToOverallViewCoords(INT_COORDS a_gridElem)
{
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
    
	RECTANGLE gameRect = m_getGameRectangle();

	int nGridElemSizeX = m_getGridElemSizeX();
	int nGridElemSizeY = m_getGridElemSizeY();
	INT_COORDS overallCoords;

	overallCoords.first  = ((gameRect.drVertex.first + gameRect.ulVertex.first) >> 1) +
				    (a_gridElem.second - a_gridElem.first) * (nGridElemSizeX >> 1);	
	overallCoords.second = (nGridElemSizeY >> 1) +
					(a_gridElem.second + a_gridElem.first) * (nGridElemSizeY >> 2) * 3;

	return overallCoords;
}

//  mapping 'center' of grid element
INT_COORDS GameView::m_mapGridElementToViewCoords(INT_COORDS a_gridElem)
{
	INT_COORDS overallCoords = m_mapGridElementToOverallViewCoords(a_gridElem);
	return m_mapFromOverallToViewCoords(overallCoords);
}

INT_COORDS GameView::m_mapCoordsToGridElement(INT_COORDS a_coords)
{
	INT_COORDS gridElem			= INVALID_COORDS;					
	RECTANGLE gameRect			= m_getGameRectangle();
	const int nGameRectWidth	= gameRect.getWidth();
	const int nGameRectHeight	= gameRect.getHeight();
	const int nGameRectWidthDiv2 =
        (nGameRectWidth >> 1);
	const int nDeltaY			= (v_nGridElemSizeY >> 2) * 3;
	BasicConfiguration* pConf 	= BasicConfiguration::m_getBasicConfiguration();
	MapGrid* 			pMap 	= pConf->m_getGridMap();
	int nNumCellX 				= pMap->m_getNumCellX();
	// at this moment should be : nNumCellX == nNumCellY 
	int nNumCellY 				= pMap->m_getNumCellY(); 

	int nX = a_coords.first;
	int nY = a_coords.second;

	if (nX < 0 || nX >= nGameRectWidth ||
		nY < 0 || nY >= nGameRectHeight)
	{
		return INVALID_COORDS;
	}

	// 'hard' vertical estimation
    int nVerticalRowNo 		= (nY / nDeltaY);   
	int nHorizontalRowNo	= UNDEFINED;

    const int nGridElemsAtVerticalRow = (nVerticalRowNo <= nNumCellX) ?
										 nVerticalRowNo :
										 (nNumCellX - 
										  (nVerticalRowNo - nNumCellX));
	const int nGridElemsAtVerticalRowDiv2 = (nGridElemsAtVerticalRow >> 1);
	int nHorizontalReference = nGameRectWidthDiv2; 
	if ((nVerticalRowNo % 2) == 0)
	{
		if (nX < nGameRectWidthDiv2)
		{
			nHorizontalReference += (v_nGridElemSizeX >> 1);
		}
		else
		{
			nHorizontalReference -= (v_nGridElemSizeX >> 1);
		}
	}

	const int nHorizontalRowDistanceToMid = abs(nHorizontalReference - nX) / v_nGridElemSizeX;

	if (nX < nGameRectWidthDiv2)
	{
		nHorizontalRowNo = (nGridElemsAtVerticalRowDiv2 - nHorizontalRowDistanceToMid);
	}
	else
	{
		nHorizontalRowNo = (nGridElemsAtVerticalRowDiv2 + nHorizontalRowDistanceToMid);
	}

	// now we have pair (nHorizontalRowNo, nVerticalRowNo)
	// and also 'nGridElemsAtVerticalRow'
	INT_COORDS gridElemBeginningHorizontalRow = 
		make_pair((nVerticalRowNo <= nNumCellX) ? nVerticalRowNo : nNumCellX,
				  (nVerticalRowNo <= nNumCellX) ? 0 : (nVerticalRowNo - nNumCellX));	

	gridElem = make_pair(gridElemBeginningHorizontalRow.first - nHorizontalRowNo,
						 gridElemBeginningHorizontalRow.second + nHorizontalRowNo);

	return gridElem;
}

