#include "common.h"

int MovementManager::v_nMipmapGridSize						= 1;
const int MovementManager::MAX_COMPUTE_PATH_GREEDY_STEP_NUM	= 10; 


MovementManager::MovementManager()
{
}

MovementManager::~MovementManager()
{
}

void MovementManager::m_computePath(INT_COORDS a_begin,
									INT_COORDS a_dest,
									set<INT_COORDS>& a_setCoveredGridElems,
									vector<PATHELEM>& a_rvGridElems,
									int a_nRadius,
									bool a_bAssumeSimplePath)
{
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();

    if (a_begin == a_dest)
	{
		return;
	}

	if (a_bAssumeSimplePath)
	{
		// greedy algorithm:
		// take into consideration only the nearest 
		// to dest elements
		m_computePathGreedy(a_begin,
							a_dest,
							1,
							a_setCoveredGridElems,
							a_rvGridElems);

		return;
	}

	int nMipmapGridSize = MovementManager::m_getMipmapGridSize();
	int nDistance = m_getDistance(a_begin, a_dest, 1);
	if (nDistance <= 2 * nMipmapGridSize)
	{
		// simple path computing
		m_computePathExt(a_begin,
						 a_dest,
						 1,
						 a_setCoveredGridElems,
						 a_rvGridElems);

		return;
	}

	// mipmap-based path computing
	GridElement* pElemStart = const_cast<GridElement*>
		(pGrid->m_getGridElement(a_begin, 1, nMipmapGridSize));
	GridElement* pElemEnd	= const_cast<GridElement*>
		(pGrid->m_getGridElement(a_dest, 1, nMipmapGridSize));
	INT_COORDS startCoords	= pElemStart->m_getCoords();
	INT_COORDS destCoords 	= pElemEnd->m_getCoords();
	set<INT_COORDS> setExcluded;

	m_computePathExt(startCoords,
					 destCoords,
					 nMipmapGridSize,
					 setExcluded,
					 a_rvGridElems);

	// since computed path containts 'mipmap'
	// start and dest elements we must remove them
	// and insert 'real' start and dest elements in there
	//a_rvGridElems.pop_back();
	a_rvGridElems[0] = PATHELEM(a_begin, 1);
	a_rvGridElems.push_back(PATHELEM(a_dest, 1));

		// debug
		/*vector<PATHELEM>::iterator it;
		for (it = a_rvGridElems.begin(); it != a_rvGridElems.end(); ++it)
		{
			PATHELEM elem = *it;
			cout << elem.gridElem.first << " " << elem.gridElem.second << " , "
				 << elem.nResolution << " ; ";
		}
		cout << endl;*/
}

void MovementManager::m_computePath(BasicObject* a_pObj, 
									INT_COORDS a_dest,
									set<INT_COORDS>& a_setCoveredGridElems,
									vector<PATHELEM>& a_rvGridElems,
									bool a_bAssumeSimplePath)
{
    INT_COORDS start = a_pObj->m_getPosition();
    m_computePath(start,
				  a_dest,
				  a_setCoveredGridElems,
				  a_rvGridElems,
				  0,
				  a_bAssumeSimplePath);
}

void MovementManager::m_recomputePath(BasicObject* a_pObj,
									  PATHELEM a_elem0,
									  PATHELEM a_elem1,
									  vector<PATHELEM>& a_rvPath)
{
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();

	a_rvPath.clear();

	if (a_elem0.nResolution != 1)
	{
		return;
	}

	if (a_elem1.nResolution == 1)
	{
		set<INT_COORDS> setExcluded;
		MovementManager::m_computePathExt(a_elem0.gridElem, 
										  a_elem1.gridElem,
										  1,
										  setExcluded,
										  a_rvPath);
		return;
	}

	int nMipmapGridSize = MovementManager::m_getMipmapGridSize();
	GridElement* pElem1 = 
		const_cast<GridElement*>(pGrid->m_getGridElement(a_elem1.gridElem, a_elem1.nResolution));
	GridElement* pElem0_parent =
		const_cast<GridElement*>(pGrid->m_getGridElement(a_elem0.gridElem, a_elem0.nResolution,
														 a_elem1.nResolution));
			
	if (!pElem1->m_isNeighbour(pElem0_parent))
	{
		return;
	}

	// WARN!!!: vector size 
	vector<GridElement*> vElem1Children;
	pGrid->m_getGridChildren(a_elem1.gridElem, a_elem1.nResolution, 1, vElem1Children);

    // obtain best child to go to
	GridElement*	pSelectedChild1	= 0;
	int 			nMinDistance	= INT_MAX;
	vector<GridElement*>::iterator it;
	for (it = vElem1Children.begin(); it != vElem1Children.end(); ++it)
	{
		GridElement* pElem = *it;
		if (!pElem->m_isCoveredByObject())
		{
			int nDistance = MovementManager::m_getDistance(a_elem0.gridElem,
														   pElem->m_getCoords(),
														   1);
			
			if (nDistance < nMinDistance)
			{
				pSelectedChild1 = pElem;
				nMinDistance 	= nDistance;
			}
		}	
	}

	set<INT_COORDS> setExcluded;
    MovementManager::m_computePathExt(a_elem0.gridElem, 
									  pSelectedChild1->m_getCoords(),
									  1,
									  setExcluded,
									  a_rvPath);

		// debug
		/*vector<PATHELEM>::iterator itPath;
		for (itPath = a_rvPath.begin(); itPath != a_rvPath.end(); ++itPath)
		{
			PATHELEM elem = *itPath;
			cout << elem.gridElem.first << " " << elem.gridElem.second << " , "
				 << elem.nResolution << " ; ";
		}
		cout << endl;*/
}

void MovementManager::m_computePathExt(INT_COORDS a_start,
									   INT_COORDS a_dest,
									   int a_nResolution,
									   set<INT_COORDS>& a_setCoveredGridElems,
									   vector<PATHELEM>& a_rvGridElems)
{
	a_rvGridElems.clear();

	PATHELEM_EXT dd(a_dest, a_nResolution, UNDEFINED, UNDEFINED);

	if (a_start == a_dest)
	{
		return;
	}

    vector<PATHELEM_EXT> vOPEN;
	vector<PATHELEM_EXT> vCLOSED;
	
	vOPEN.push_back(PATHELEM_EXT(a_start, a_nResolution, INT_MAX, 0));
	make_heap(vOPEN.begin(), vOPEN.end());

	int nIter = 0;
    while (!vOPEN.empty())
	{
		++nIter;
        PATHELEM_EXT uu = *(vOPEN.begin());
		pop_heap(vOPEN.begin(), vOPEN.end());
		vOPEN.pop_back();
        
        vCLOSED.push_back(uu);
		push_heap(vCLOSED.begin(), vCLOSED.end());

		//cout << uu.gridElem.first << " " << uu.gridElem.second << " " << uu.nFF << endl;
		
		if (uu.gridElem == a_dest)
		{
			// ok, we found a solution
			// back pointers are in >>CLOSED<< list
			//cout << "SOLUTION FOUND!" << endl;
			a_rvGridElems.push_back(PATHELEM(uu.gridElem, uu.nResolution)); 
			PATHELEM_EXT prev = uu.vPrevPathElems[0];
			while (!(prev.gridElem == a_start))
			{
				vector<PATHELEM_EXT>::iterator itCLOSED = 
					find_if(vCLOSED.begin(), vCLOSED.end(), equal_grid_elem(prev.gridElem));
				uu = *itCLOSED;
				prev = uu.vPrevPathElems[0];
				a_rvGridElems.push_back(PATHELEM(uu.gridElem, uu.nResolution));
			}

			a_rvGridElems.push_back(PATHELEM(prev.gridElem, prev.nResolution));

			reverse(a_rvGridElems.begin(), a_rvGridElems.end());

			/*vector<PATHELEM>::iterator it;
			for (it = a_rvGridElems.begin(); it != a_rvGridElems.end(); ++it)
			{
				PATHELEM elem = *it;
				cout << elem.gridElem.first << " " << elem.gridElem.second << endl;
			}*/

            return;
		}

		vector<PATHELEM_EXT> vAdjGridElems;
		m_getAdjacentGridElems(uu, a_setCoveredGridElems, vAdjGridElems);

		vector<PATHELEM_EXT>::iterator it;
		for (it = vAdjGridElems.begin(); it != vAdjGridElems.end(); ++it)
		{
			PATHELEM_EXT vv = *it;
			vv.nGG = uu.nGG + 
				m_getDistance(uu, vv);
			vv.nFF = vv.nGG +
				m_estimate_hh(vv, dd);
			vector<PATHELEM_EXT>::iterator itOPEN = 
				find_if(vOPEN.begin(), vOPEN.end(), equal_grid_elem(vv.gridElem));
			vector<PATHELEM_EXT>::iterator itCLOSED = 
				find_if(vCLOSED.begin(), vCLOSED.end(), equal_grid_elem(vv.gridElem));
			if (itOPEN != vOPEN.end())
			{
				PATHELEM_EXT foundVV = *itOPEN;
				if (foundVV.nFF <= vv.nFF)
				{
                    continue;
				}
				else
				{
					vOPEN.erase(itOPEN);
					make_heap(vOPEN.begin(), vOPEN.end());
				}
			}
			if (itCLOSED != vCLOSED.end())
			{
				PATHELEM_EXT foundVV = *itCLOSED;
				if (foundVV.nFF <= vv.nFF)
				{
					continue;
				}
				else
				{
					vCLOSED.erase(itCLOSED);
					make_heap(vCLOSED.begin(), vCLOSED.end());
				}
			}

            // prev(vv) = uu
			vv.vPrevPathElems.push_back(uu);
			vOPEN.push_back(vv);
			push_heap(vOPEN.begin(), vOPEN.end());
		}
	}
}

void MovementManager::m_getAdjacentGridElems(PATHELEM_EXT a_pathElem,
											 set<INT_COORDS>& a_setCoveredGridElems,
											 vector<PATHELEM_EXT>& a_rvAdjPathElems)
{
	a_rvAdjPathElems.clear();

	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();

	int nX 		= a_pathElem.gridElem.first;
	int nY 		= a_pathElem.gridElem.second;
	int nRes 	= a_pathElem.nResolution;

    // test: adjacency test
	int tblDiff[][2] = {
		{ 1, -1}, { 1,  0}, { 0,  1}, {-1,  1},
		{-1,  0}, { 0, -1}
	};
	int nTblDiffSize = sizeof(tblDiff) / sizeof(tblDiff[0]);

	for (int i = 0; i < nTblDiffSize; ++i)
	{
		int nDiffX = tblDiff[i][0];
		int nDiffY = tblDiff[i][1];
		INT_COORDS gridElem = make_pair(nX + nDiffX, nY + nDiffY);
		if (m_isValid(gridElem, nRes))
		{
			GridElement* pElem = const_cast<GridElement*>
				(pGrid->m_getGridElement(gridElem, nRes));
			if (a_setCoveredGridElems.end() == a_setCoveredGridElems.find(gridElem) && 
				!pElem->m_isCoveredByObject())
			{
				a_rvAdjPathElems.push_back(PATHELEM_EXT(gridElem, nRes,
														UNDEFINED, UNDEFINED));
			}

		}
	}

	// debug	
		//cout << a_rvAdjPathElems.size() << endl;
	// end debug	
}

int MovementManager::m_estimate_hh(PATHELEM_EXT a_vv, PATHELEM_EXT a_dd)
{
	int nHH = 0;

	nHH = m_getDistance(a_vv, a_dd);

	// debug	
		//cout << nHH << endl;
	// end debug

	return nHH;
}

int MovementManager::m_getDistance(INT_COORDS a_coords1, INT_COORDS a_coords2, 
								   int a_nResolution)
{
	return m_getDistance(PATHELEM_EXT(a_coords1, a_nResolution, UNDEFINED, UNDEFINED),
						 PATHELEM_EXT(a_coords2, a_nResolution, UNDEFINED, UNDEFINED));
}

int MovementManager::m_getDistance(PATHELEM_EXT a_v1, PATHELEM_EXT a_v2)
{
    if (a_v1.nResolution != a_v2.nResolution)
	{
		return UNDEFINED;
	}

	int nDistance = -1;
	INT_COORDS v1Coords = a_v1.gridElem;
	INT_COORDS v2Coords = a_v2.gridElem;

	if (sgn(v1Coords.first - v2Coords.first) == sgn(v1Coords.second - v2Coords.second))
	{
		nDistance = (abs(v1Coords.first  - v2Coords.first) +
					 abs(v1Coords.second - v2Coords.second)) * a_v1.nResolution;
	}
	else
	{
		nDistance = (max(abs(v1Coords.first  - v2Coords.first),
						 abs(v1Coords.second - v2Coords.second))) * a_v1.nResolution;
	}

	return nDistance;
}

int MovementManager::m_getFrameGap(int a_nVelocity)
{
	if (a_nVelocity < VELOCITY_MIN) 
	{
		a_nVelocity = VELOCITY_MIN;
	}
	if (a_nVelocity > VELOCITY_MAX)
	{
		a_nVelocity = VELOCITY_MAX;
	}

	// should also take into consideration actual manager
	// load
    
	return ((VELOCITY_MAX - VELOCITY_MIN + 1) / a_nVelocity); 
}

void MovementManager::m_resolveDynamicMovementConflicts(BasicObject* a_pObj)
{
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();

	INT_COORDS pos_Frame0 = a_pObj->m_getPosition();
	INT_COORDS pos_Frame1 = a_pObj->m_getPosition(1);
	INT_COORDS pos_Frame2 = a_pObj->m_getPosition(2);
	int nX0 = pos_Frame0.first;
	int nY0 = pos_Frame0.second;

	if (INVALID_COORDS == pos_Frame1 || 
		pos_Frame0 == pos_Frame1)
	{
		// stand still
		return;
	}

	// movement
	int tblDiff[][2] = {
		{-1, -1}, { 0, -2}, { 1, -2}, { 2, -2},
		{ 2, -1}, { 2,  0}, { 1,  1}, { 0,  2},
		{-1,  2}, {-2,  2}, {-2,  1}, {-2,  0},
		{ 1, -1}, { 1,  0}, { 0,  1}, {-1,  1},
		{-1,  0}, { 0, -1}
	};
	int nTblDiffSize = sizeof(tblDiff) / sizeof(tblDiff[0]);
	vector<INT_COORDS> vPredPosForNeighbObject;

    for (int i = 0; i < nTblDiffSize; ++i)
	{
		int nDiffX = tblDiff[i][0];
		int nDiffY = tblDiff[i][1];
		INT_COORDS gridElem = make_pair(nX0 + nDiffX, nY0 + nDiffY);
		if (m_isValid(gridElem))
		{
			GridElement* pElem = const_cast<GridElement*>(pGrid->m_getGridElement(
				gridElem));
            if (pElem->m_isCoveredByObject())
			{
				BasicObject* pNeighbObj = pElem->m_getCoveringObject();
				if (pNeighbObj != a_pObj)
				{
					INT_COORDS neighbPos_Frame1 = pNeighbObj->m_getPosition(1);
					if (INVALID_COORDS != neighbPos_Frame1)
					{
						vPredPosForNeighbObject.push_back(neighbPos_Frame1);
					}
				}
			}
		}
	}

    set<INT_COORDS> setCoveredGridElems(vPredPosForNeighbObject.begin(),
										vPredPosForNeighbObject.end());
	
	vector<INT_COORDS>::iterator it;
	for (it = vPredPosForNeighbObject.begin(); it != vPredPosForNeighbObject.end(); ++it)
	{
		INT_COORDS predPos = (*it);
        if (predPos == pos_Frame1)
		{
			// conflict found!!!
			if (pos_Frame2 == INVALID_COORDS)
			{
				// bad +1 position,stand still
				vector<PATHELEM> vPathElems;
				vPathElems.push_back(PATHELEM(pos_Frame0, 1));
				vPathElems.push_back(PATHELEM(pos_Frame1, 1));
				a_pObj->m_replacePath(0, UNDEFINED, vPathElems);
				break;
			}
			else
			{
                vector<PATHELEM> vPathElems;
				m_computePathExt(a_pObj->m_getPosition(),
								 pos_Frame2,
								 1,
								 setCoveredGridElems, 
								 vPathElems);
				a_pObj->m_replacePath(UNDEFINED, 2, vPathElems);
				break;
			}
		}
	}

}

bool MovementManager::m_isValid(INT_COORDS gridElem, int a_nRes)
{
	if (0 == a_nRes)
	{
		a_nRes = 1;
	}

	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();

	int nNumCellX	= pGrid->m_getNumCellX();
	int nNumCellY	= pGrid->m_getNumCellY();
	int nMaxX 		= (nNumCellX / a_nRes);
	int nMaxY		= (nNumCellY / a_nRes);

	return (gridElem.first >= 0 &&
			gridElem.first <  nMaxX &&
			gridElem.second >= 0 &&
			gridElem.second <  nMaxY); 
}

void MovementManager::m_setMipmapGridSize(int a_nSize)
{
	if (!is_power2(a_nSize))
	{
		return;
	}

	v_nMipmapGridSize = a_nSize;
}

int MovementManager::m_computePathGreedy(INT_COORDS a_start,
										 INT_COORDS a_dest,
										 int a_nResolution,
										 set<INT_COORDS>& a_setCoveredGridElems,
										 vector<PATHELEM>& a_rvGridElems)
{
	a_rvGridElems.clear();

	if (a_start == a_dest)
	{
		return 0;
	}

	PATHELEM firstPathElem(a_start, a_nResolution);

	int nStep = 0;
	INT_COORDS actualGridElem = a_start;
	while (actualGridElem != a_dest)
	{
		if (nStep++ > MAX_COMPUTE_PATH_GREEDY_STEP_NUM)
		{
			return ERR_RESULT;
		}

		PATHELEM actualPathElem(actualGridElem, a_nResolution);
		a_rvGridElems.push_back(actualPathElem);

		vector<PATHELEM_EXT> vAdjGridElems;
		m_getAdjacentGridElems(PATHELEM_EXT(actualGridElem, a_nResolution, UNDEFINED, UNDEFINED),
							   a_setCoveredGridElems,
							   vAdjGridElems);	

		if (vAdjGridElems.empty())
		{
			return ERR_RESULT;
		}

		int nHHMin = INT_MAX; 
		PATHELEM bestAdjPathElem;
		vector<PATHELEM_EXT>::iterator it;
		for (it = vAdjGridElems.begin(); it != vAdjGridElems.end(); ++it)
		{
			PATHELEM_EXT pathElemExt = *it;
			PATHELEM pathElem(pathElemExt.gridElem, pathElemExt.nResolution);
			if (pathElem.gridElem == a_dest)
			{
				bestAdjPathElem = pathElem;
				break;
			}
			if (pathElem == firstPathElem)
			{
				continue;
			}
			int nHH = m_estimate_hh(pathElemExt, 
									PATHELEM_EXT(a_dest, a_nResolution, UNDEFINED, UNDEFINED));
			if (nHH < nHHMin)
			{
				nHHMin			= nHH;
				bestAdjPathElem	= pathElem;
			}
		}

		actualGridElem = bestAdjPathElem.gridElem;
	}

	PATHELEM destPathElem(a_dest, a_nResolution);
	a_rvGridElems.push_back(destPathElem);

	return 0;
}

void MovementManager::m_getDiffs(int a_nDist, vector<INT_COORDS>& vDiffs)
{
	vDiffs.clear();

	int nXDiff, nYDiff;

	// X(+), Y(+)
	for (nXDiff = 0; nXDiff <= a_nDist; ++nXDiff)
	{
		nYDiff = a_nDist - nXDiff;
		vDiffs.push_back(make_pair(nXDiff, nYDiff));
	}

	// X(+), Y(-) (1)
	for (nXDiff = 0; nXDiff <= a_nDist - 1; ++nXDiff)
	{
		nYDiff = -a_nDist;
		vDiffs.push_back(make_pair(nXDiff, nYDiff));
	}

	// X(+), Y(-) (2)
	for (nYDiff = -1; nYDiff >= -a_nDist; --nYDiff)
	{
		nXDiff = a_nDist;
		vDiffs.push_back(make_pair(nXDiff, nYDiff));
	}

	// X(-), Y(-)
	for (nXDiff = -1; nXDiff >= -a_nDist; --nXDiff)
	{
		nYDiff = -(a_nDist - abs(nXDiff));
		vDiffs.push_back(make_pair(nXDiff, nYDiff));
	}

	// X(-), Y(+) (1)
	for (nYDiff = 1; nYDiff <= a_nDist - 1; ++nYDiff)
	{
		nXDiff = -a_nDist;
		vDiffs.push_back(make_pair(nXDiff, nYDiff));
	}

	// X(-), Y(+) (2)
	for (nXDiff = -1; nXDiff >= -a_nDist; --nXDiff)
	{
		nYDiff = a_nDist;
		vDiffs.push_back(make_pair(nXDiff, nYDiff));
	}
}

INT_COORDS MovementManager::findNearestEmptyPos(INT_COORDS a_pos, uint a_nDeep)
{
	INT_COORDS selPos = INVALID_COORDS;

	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();

	vector<INT_COORDS> vDiffs;
	for (int ii = 0; ii < a_nDeep; ++ii)
	{
		MovementManager::m_getDiffs(ii, vDiffs);	
		vector<INT_COORDS>::iterator itCC;
		for (itCC = vDiffs.begin(); itCC != vDiffs.end(); ++itCC)
		{
			INT_COORDS diff = *itCC;
			INT_COORDS checkedPos = make_pair(a_pos.first + diff.first, a_pos.second + diff.second);
			if (MovementManager::m_isValid(checkedPos))
			{
				GridElement* pElem = const_cast<GridElement*>(pGrid->m_getGridElement(
										checkedPos));
				if (pElem && !pElem->m_isCoveredByObject())
				{
					selPos = checkedPos;
					return selPos;
				}
			}
		}
	}

	return INVALID_COORDS;
}


