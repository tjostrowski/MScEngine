#include "common.h"

HexagonalMapGrid::~HexagonalMapGrid()
{
	vector<GridElement*>::iterator itElem;
	for (itElem = v_vGridElems.begin(); itElem != v_vGridElems.end(); ++itElem)
	{
		GridElement* pElem = *itElem;
		delete pElem;
        *itElem = 0;
	}
	v_vGridElems.clear();
}

void HexagonalMapGrid::m_generateBasicGridStructure_()
{
	if (!m_assertConsistency())
	{
		return;
	}

	// special case when v_nNumCellX == v_nNumCellY
    int nVecSize = (int)((v_nNumCellX * v_nNumCellY * 4 - 1) / 3);
	v_vGridElems.resize(nVecSize); 

    // initializing QUAD TREE => 
	// MUST BE INITIALIZED in that order
	for (int i = 0; i < nVecSize; ++i)
	{
		GridElement* pElem = new GridElement();
		pElem->m_setIndex(i);
		if (0 == i)
		{
			pElem->m_setCoords(make_pair(0, 0));
		}
		else
		{
			int nParent 	= ((i - 1) >> 2);
			int nDiv 		=  (i - 1) % 4;
			int nDiffForDiv[][2] = {
				{0, 0},
				{1, 0},
				{1, 1},
				{0, 1}};
			INT_COORDS parCoords = v_vGridElems[nParent]->m_getCoords();
			INT_COORDS coords	 = make_pair((parCoords.first  << 1) + nDiffForDiv[nDiv][0],
											 (parCoords.second << 1) + nDiffForDiv[nDiv][1]);
			pElem->m_setCoords(coords);
		}

		v_vGridElems[i] = pElem;
	}

    // tests, initializing grid bitmaps
	//
	int nGridElemsSize = v_nNumCellX * v_nNumCellY;
	const int nBmpCount = 6;
	BITMAP* bmpArray[10];
	char* bmpNames[] = {
			"pictures/bbmerge_rev_one_next.pcx",	// 48 x 48
			"pictures/bbworkspace_new.pcx", 		// 48 x 48
			"pictures/eclipsesearchmark.bmp",		// 22 x 11
			"pictures/mysha.pcx",					// 320 x 200 
			"pictures/planet.pcx",					// 49 x 49
			//"pictures/vs.gif",						// 16 x 16
			"pictures/vse64.pcx"					// 64 x 64 
	};

	for (int i = 0; i < nBmpCount; ++i)
	{
		const int nGridElemSizeX = 32;
		const int nGridElemSizeY = 32;
		bmpArray[i] = create_bitmap(nGridElemSizeX, nGridElemSizeY);
		BITMAP* pTmpBmp = load_bitmap(bmpNames[i], NULL);
		if (!pTmpBmp)
		{
			cerr << "Unable to load bitmap: " << bmpNames[i] << endl;
			return;
		}
		ext_blit(pTmpBmp, 
				 bmpArray[i],
				 0,
				 0, 
				 pTmpBmp->w,
				 pTmpBmp->h,
				 0,
				 0,
				 nGridElemSizeX,
				 nGridElemSizeY);
	}

	int nGridElemsSizeDiv4 = (nGridElemsSize >> 2);
	int nBase = nVecSize - nGridElemsSize;
    
	for (int i = nBase; i < nBase + nGridElemsSizeDiv4; ++i)
	{
		v_vGridElems[i]->m_setBitmap(bmpArray[0]);
	}

	nBase += nGridElemsSizeDiv4;
	for (int i = nBase; i < nBase + nGridElemsSizeDiv4; ++i)
	{
		v_vGridElems[i]->m_setBitmap(bmpArray[1]);
	}

	nBase += nGridElemsSizeDiv4;
	for (int i = nBase; i < nBase + nGridElemsSizeDiv4; ++i)
	{
		v_vGridElems[i]->m_setBitmap(bmpArray[2]);
	}

	nBase += nGridElemsSizeDiv4;
	for (int i = nBase; i < nBase + nGridElemsSizeDiv4; ++i)
	{
		v_vGridElems[i]->m_setBitmap(bmpArray[3]);
	}
}

const GridElement* HexagonalMapGrid::m_getGridElement(INT_COORDS a_coords, 
													  int a_nResolution,
													  int a_nParResolution)
{
    int nVecIndex = 0;
	pair<int, int> rangeX = make_pair(0, v_nNumCellX - 1);
	pair<int, int> rangeY = make_pair(0, v_nNumCellY - 1);
	int nSize 		= v_vGridElems.size();
	int nActualResX	= v_nNumCellX;
	int nActualResY = v_nNumCellY;
    
	if (UNDEFINED != a_nParResolution)
	{
		if (a_nParResolution < a_nResolution)
		{
			return 0;
		}
	}
	else
	{
		a_coords.first 	*= a_nResolution;
		a_coords.second	*= a_nResolution;
	}

	while((nVecIndex << 2) + 1 < nSize &&
		  !(UNDEFINED != a_nParResolution && nActualResX == a_nParResolution) &&
		  nActualResX != a_nResolution)
	{
		int nSelectedQuarter;
		int nMidRangeX = rangeX.first + ((rangeX.second - rangeX.first) >> 1);
		int nMidRangeY = rangeY.first + ((rangeY.second - rangeY.first) >> 1);
		if (a_coords.first <= nMidRangeX)
		{
			if (a_coords.second <= nMidRangeY)
			{
				nSelectedQuarter = 0;
			}
			else
			{
				nSelectedQuarter = 3;
			}
		}
		else
		{
			if (a_coords.second <= nMidRangeY)
			{
				nSelectedQuarter = 1;
			}
			else
			{
				nSelectedQuarter = 2;
			}
		}
        
        nVecIndex = (nVecIndex << 2) + nSelectedQuarter + 1;
		if (0 == nSelectedQuarter)
		{
			rangeX 	= make_pair(rangeX.first, nMidRangeX);
			rangeY 	= make_pair(rangeY.first, nMidRangeY);
		}
		else if (1 == nSelectedQuarter)
		{
			rangeX 	= make_pair(nMidRangeX + 1, rangeX.second);
            rangeY 	= make_pair(rangeY.first, nMidRangeY);
		}
		else if (2 == nSelectedQuarter)
		{
			rangeX = make_pair(nMidRangeX + 1, rangeX.second);
			rangeY = make_pair(nMidRangeY + 1, rangeY.second);
		}
		else
		{
			// 3...
			rangeX = make_pair(rangeX.first, nMidRangeX);
			rangeY = make_pair(nMidRangeY + 1, rangeY.second);
		}

		nActualResX >>= 1;
		nActualResY >>= 1;
	}

	return v_vGridElems[nVecIndex];
}

void HexagonalMapGrid::m_getGridChildren(INT_COORDS a_parCoords,
										 int a_nParResolution,
										 int a_nChildResolution,
										 vector<GridElement*>& a_rvChildren)
{
	a_rvChildren.clear();

	if (a_nChildResolution > a_nParResolution)
	{ 
		return;
	}

	GridElement* pElem = const_cast<GridElement*>
		(m_getGridElement(a_parCoords,
						  a_nParResolution));
	if (0 == pElem)
	{
		return;
	}

	int nParIndex	= pElem->m_getIndex();
	int nResolution = a_nParResolution;
	// (, ]
	pair<int, int> vecRange = make_pair(nParIndex, nParIndex + 1);
	while (nResolution > a_nChildResolution)
	{
		nResolution >>= 1;
		vecRange.first 	= (vecRange.first << 2) + 1;
		vecRange.second	= (vecRange.second << 2);
	}

	int nSize = v_vGridElems.size();
	if (vecRange.first >= nSize || vecRange.second >= nSize)
	{
		return;
	}

		/*cout << nParIndex << endl;
		cout << v_vGridElems.size() - v_nNumCellX * v_nNumCellY - 1 << endl;
		cout << a_parCoords.first << " " << a_parCoords.second << endl;
		cout << a_nParResolution << endl;
		cout << a_nChildResolution << endl;
		cout << vecRange.first << " " << vecRange.second << endl;*/

    a_rvChildren.insert(a_rvChildren.end(),
						v_vGridElems.begin() + vecRange.first,
						v_vGridElems.begin() + vecRange.second + 1);
}

void HexagonalMapGrid::init(
		int a_nWidth,
		int a_nHeight,
		int a_nNumCellX,
		int a_nNumCellY,
		int a_nGridElemSizeX,
		int a_nGridElemSizeY,
		int a_nNumLayers
	)
{
    v_gameRectangle.ulVertex.first 	= 0;
	v_gameRectangle.ulVertex.second	= 0;
	v_gameRectangle.drVertex.first 	= a_nWidth - 1;
	v_gameRectangle.drVertex.second = a_nHeight - 1;

	// a_nNumCellX and a_nNumCellY should be equal
	// and dividable by 4 (or more => be equal to 4^{xxx}?)
	if (a_nNumCellX != a_nNumCellY)
	{
		a_nNumCellY = a_nNumCellX;
	}
	int nNumCellX = (a_nNumCellX % 4 != 0) ? (a_nNumCellX - (a_nNumCellX % 4)) : a_nNumCellX;
	int nNumCellY = (a_nNumCellY % 4 != 0) ? (a_nNumCellY - (a_nNumCellY % 4)) : a_nNumCellY;
	v_nNumCellX = nNumCellX;
	v_nNumCellY = nNumCellY;

	v_nNumLayers = a_nNumLayers;

	// v_nGridElemSizeX and v_nGridElemSizeY
    // should be dividable by 4
	int nGridElemSizeX = (a_nGridElemSizeX % 4 != 0) ? (a_nGridElemSizeX - (a_nGridElemSizeX % 4)) :
						  a_nGridElemSizeX; 
    int nGridElemSizeY = (a_nGridElemSizeY % 4 != 0) ? (a_nGridElemSizeY - (a_nGridElemSizeY % 4)) :
						  a_nGridElemSizeY;
	v_nGridElemSizeX = nGridElemSizeX;
	v_nGridElemSizeY = nGridElemSizeY;

	// initializing layers	
	// DELETED
}

INT_COORDS HexagonalMapGrid::m_mapGridElementToCoords(INT_COORDS a_gridElem)
{
	const int nDefaultUserId = 1; 
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	GameView* pView = pConf->m_getGameViewByUserId(nDefaultUserId);
	if (0 == pView)
	{
		return make_pair(-1, -1);
	}

	return pView->m_mapGridElementToOverallViewCoords(a_gridElem);
}

// mapping 'center' of grid element
INT_COORDS HexagonalMapGrid::m_mapGridElementToGameViewCoords(INT_COORDS a_gridElem, int a_nGameViewId)
{
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	GameView* pView = pConf->m_getGameViewByUserId(a_nGameViewId);
	if (0 == pView)
	{
		return make_pair(-1, -1);
	}

	return pView->m_mapGridElementToViewCoords(a_gridElem);
}

INT_COORDS HexagonalMapGrid::m_mapCoordsToGridElement(INT_COORDS a_coords, int a_nGameViewId)
{
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	GameView* pView = pConf->m_getGameViewByUserId(a_nGameViewId);

	if (0 == pView)
	{
		return INVALID_COORDS;
	}

	return pView->m_mapCoordsToGridElement(a_coords);
}

bool HexagonalMapGrid::m_assertConsistency()
{
	return (v_nNumCellX > 0) &&
		   (v_nNumCellY > 0) &&
		   (is_power2(v_nNumCellX)) &&
           (is_power2(v_nNumCellY)) &&
		   (v_nNumCellX == v_nNumCellY); // thsi should be removed in future
}

