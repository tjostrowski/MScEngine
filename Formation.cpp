#include "common.h"


Formation::Formation() : 
	v_nStatus(NOT_FORMED),
	v_pos(INVALID_COORDS)
{
	v_nRequiredNumUnits	= UNDEFINED;
	v_nAngle 			= 0;
	v_nFormationState	= NOT_FORMED;
}

Formation::~Formation()
{
}

int Formation::m_getRequiredNumUnits()
{
	if (v_vFormationRows.empty())
	{
		return UNDEFINED;
	}

	if (v_nRequiredNumUnits == UNDEFINED)
	{
		v_nRequiredNumUnits = 0;
		vector<int>::iterator it;
		for (it = v_vFormationRows.begin(); it != v_vFormationRows.end(); ++it)
		{
			int nUnitsInRow = *it;
			v_nRequiredNumUnits += nUnitsInRow;
		}
	}

	return v_nRequiredNumUnits;
}

int Formation::m_addObject(BasicObject* a_pObj)
{
	if (m_getObjectsNum() >= v_nRequiredNumUnits)
	{
		return ERR_RESULT;
	}

	vector<FORMATION_POS> vAvailFormationPoss;
	vector<int>::iterator itRows;
	int	nRow; 	
	for (itRows = v_vFormationRows.begin(), nRow = 0; itRows != v_vFormationRows.end(); ++itRows, ++nRow)
	{
		int nUnitsInRow = *itRows;
		for (int nPosInRow = 0; nPosInRow < nUnitsInRow; ++nPosInRow)
		{
			vAvailFormationPoss.push_back(make_pair(nRow, nPosInRow));
		}
	}

	vector<FormationObjDescr>::iterator itUnits;
	for (itUnits = v_vUnits.begin(); itUnits != v_vUnits.end(); ++itUnits)
	{
		FormationObjDescr descr = *itUnits;
		vector<FORMATION_POS>::iterator it = find(vAvailFormationPoss.begin(),
												  vAvailFormationPoss.end(),
												  descr.m_getFormPos());
		vAvailFormationPoss.erase(it);
	}

	// get free position with minimum distance
	INT_COORDS objPos = a_pObj->m_getPosition();
	FORMATION_POS selFormPos = INVALID_COORDS;
	int nMinDist = INT_MAX;
	vector<FORMATION_POS>::iterator itPoss;
	for (itPoss = vAvailFormationPoss.begin(); itPoss != vAvailFormationPoss.end();  ++itPoss)
	{
		FORMATION_POS formPos = *itPoss;
		int nRow = formPos.first;
		int nPosInRow = formPos.second;
		INT_COORDS pos = (v_vPoss[nRow])[nPosInRow]; 	
		int nDist = MovementManager::m_getDistance(pos, objPos, 1);
		if (nDist < nMinDist)
		{
			selFormPos = formPos;
			nMinDist = nDist;
		}
	}

	v_vUnits.push_back(FormationObjDescr(a_pObj, 
										 selFormPos,
										 m_getGRPos(selFormPos)));

	return 0;
}

void Formation::m_removeObject(BasicObject* a_pObj)
{
	vector<FormationObjDescr>::iterator itUnits = find_if(v_vUnits.begin(),
	  													  v_vUnits.end(),
														  equal_form_object(a_pObj));

	if (v_vUnits.end() != itUnits)
	{
		v_vUnits.erase(itUnits);
	}
}

void Formation::m_setFormation(vector<int>& a_vRowInfo)
{
	v_vFormationRows = a_vRowInfo;
	m_reassignUnitsToFormation();
}

void Formation::m_setAsRectangle(int a_nRows, int a_nUnitsInRow)
{
	for (int i = 0; i < a_nRows; ++i)
	{
		v_vFormationRows.push_back(a_nUnitsInRow);
	}
	m_reassignUnitsToFormation();
}

void Formation::m_setAsTriangle(int a_nRows, int a_nMaxUnitsInRow)
{
	// rows num can be less then a_nRows
	int a_nUnitsInRow = a_nMaxUnitsInRow;
	for (int i = 0; i < a_nRows && a_nUnitsInRow > 0; ++i)
	{
		v_vFormationRows.push_back(a_nUnitsInRow);
        a_nUnitsInRow -= 2;
	}
	m_reassignUnitsToFormation();
}

void Formation::m_reassignUnitsToFormation()
{
	/*if (INVALID_COORDS == v_pos)
	{
		int nRow		= 0;
		int	nPosInRow	= 0; 
		vector<FormationObjDescr>::iterator itUnits;
		for (itUnits = v_vUnits.begin(); itUnits != v_vUnits.end(); ++itUnits)
		{
			itUnits->m_setPos(make_pair(nRow, nPosInRow));
			if (nPosInRow >= v_vFormationRows[nRow])
			{
				nRow++;
				nPosInRow = 0;
			}
			else
			{
				nPosInRow++;
			}
		}

		return;
	}

	// assigning basing on object positions
	vector<FORMATION_POS> vBusyFormationPoss;
	vector<FormationObjDescr>::iterator itUnits;
	for (itUnits = v_vUnits.begin(); itUnits != v_vUnits.end(); ++itUnits)
	{
        FormationObjDescr	descr	= *itUnits;
		BasicObject*		pObj	= descr.m_getObj();
		INT_COORDS			objPos	= pObj->m_getPosition();
		
		vector<int>::iterator itRows;




	}*/
}

void Formation::m_reassignUnitToFormation(BasicObject* a_pObj, FormationObjDescr& a_descr)
{
	


}

void Formation::m_setAngle(int a_nAngle)
{
	if (v_nAngle == a_nAngle)
	{
		return;
	}

	if (v_nAngle < 0 || v_nAngle >= 90)
	{
		return;
	}

	v_nAngle = a_nAngle;
	// invalidate grid relative positions of units
	vector<FormationObjDescr>::iterator itUnits;
	for (itUnits = v_vUnits.begin(); itUnits != v_vUnits.end(); ++itUnits)
	{
		itUnits->m_setGridRelPos(INVALID_COORDS);
	}
	v_vPoss.clear();
}

INT_COORDS Formation::m_getGRPos(FORMATION_POS a_formPos)
{
	if (INVALID_COORDS == a_formPos)
	{
		return INVALID_COORDS;
	}
	int nRow = a_formPos.first;
	int nPosInRow = a_formPos.second;
	return (v_vPoss[nRow])[nPosInRow];
}

void Formation::m_getGRPoses(vector<INT_COORDS>& a_rvPoss)
{
	a_rvPoss.clear();

	// rotation around MID of v_vFormationRows.front()
    int  nMaxElemsInRow = -INT_MAX;

	vector<int>::iterator itRows;
	for (itRows = v_vFormationRows.begin(); itRows != v_vFormationRows.end(); ++itRows)
	{
		int nElemsInRow = *itRows;
		if (nElemsInRow > nMaxElemsInRow)
		{
			nMaxElemsInRow = nElemsInRow;
		}
	}

	const int nUserId			= 1;
	BasicConfiguration* pConf	= BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid				= pConf->m_getGridMap();
	GameView* pView				= pConf->m_getGameViewByUserId(nUserId);

	const int nGridElemSizeX	= pView->m_getGridElemSizeX();
	const int nGridElemSizeY	= pView->m_getGridElemSizeY();

	const double fAngleRad = (v_nAngle * M_PI) / 180.0;

	const int nDeltaX = (int) abs(nGridElemSizeX * cos(fAngleRad));
	const int nDeltaY = (int) abs(nGridElemSizeX * sin(fAngleRad));

		cout << "DEBUG: " << v_nAngle << endl;
		if (nDeltaX <= 0 || nDeltaY <= 0)
		{	
			cout << "ERROR: " << "nDeltaX <= 0 || nDeltaY <= 0" << endl;
			return;
		}

	INT_COORDS ovLeadCoords = pView->m_mapGridElementToOverallViewCoords(v_pos);	

	vector<INT_COORDS> vDiffs1;
	MovementManager::m_getDiffs(1, vDiffs1);

	// left arm of first row
	vector<INT_COORDS> vAssignedPoss;
	vAssignedPoss.push_back(v_pos);
	INT_COORDS actualPos = v_pos; 
	int nXAfterRotation	= ovLeadCoords.first;
	int nYAfterRotation	= ovLeadCoords.second;
	for (int i = -1; i >= -((nMaxElemsInRow >> 1) + 2); --i)
	{
		nXAfterRotation -= nDeltaX;
		nYAfterRotation -= nDeltaY;
		INT_COORDS selectedAdjPos = INVALID_COORDS;
		int nMinExpectedToRealDist = INT_MAX;
		vector<INT_COORDS>::iterator it;
		for (it = vDiffs1.begin(); it != vDiffs1.end(); ++it)
		{
			INT_COORDS diff	= *it;
			INT_COORDS adjPos = make_pair(actualPos.first + diff.first,
								 		  actualPos.second + diff.second);
			vector<INT_COORDS>::iterator itFound = find(vAssignedPoss.begin(),
														vAssignedPoss.end(),
														adjPos);
			if (vAssignedPoss.end() != itFound)
			{
				continue;
			}

			INT_COORDS ovAdjCoords = pView->m_mapGridElementToOverallViewCoords(adjPos);

			int nExpectedToRealDist = 
				(nXAfterRotation - ovAdjCoords.first) * (nXAfterRotation - ovAdjCoords.first) + 
				(nYAfterRotation - ovAdjCoords.second) * (nYAfterRotation - ovAdjCoords.second); 
			if (nExpectedToRealDist < nMinExpectedToRealDist)
			{
				selectedAdjPos = adjPos;
				nMinExpectedToRealDist = nExpectedToRealDist;
			}
		}

		if (INVALID_COORDS == selectedAdjPos)
		{
				cout << "ERROR: " << "INVALID_COORDS == selectedAdjPos" << endl;
				return;
		}

		actualPos = selectedAdjPos;
		vAssignedPoss.push_back(actualPos);
	}

	reverse(vAssignedPoss.begin(), vAssignedPoss.end());

	// right arm of first row	
	actualPos = v_pos;
	nXAfterRotation	= ovLeadCoords.first;
	nYAfterRotation	= ovLeadCoords.second;
	for (int i = 1; i <= ((nMaxElemsInRow >> 1) + 2); ++i)
	{
		nXAfterRotation += nDeltaX;
		nYAfterRotation += nDeltaY;
		INT_COORDS selectedAdjPos = INVALID_COORDS;
		int nMinExpectedToRealDist = INT_MAX;
		vector<INT_COORDS>::iterator it;
		for (it = vDiffs1.begin(); it != vDiffs1.end(); ++it)
		{
			INT_COORDS diff	= *it;
			INT_COORDS adjPos = make_pair(actualPos.first + diff.first,
								 		  actualPos.second + diff.second);
			vector<INT_COORDS>::iterator itFound = find(vAssignedPoss.begin(),
														vAssignedPoss.end(),
														adjPos);
			if (vAssignedPoss.end() != itFound)
			{
				continue;
			}

			INT_COORDS ovAdjCoords = pView->m_mapGridElementToOverallViewCoords(adjPos);

			int nExpectedToRealDist = 
				(nXAfterRotation - ovAdjCoords.first) * (nXAfterRotation - ovAdjCoords.first) + 
				(nYAfterRotation - ovAdjCoords.second) * (nYAfterRotation - ovAdjCoords.second); 
			if (nExpectedToRealDist < nMinExpectedToRealDist)
			{
				selectedAdjPos = adjPos;
				nMinExpectedToRealDist = nExpectedToRealDist;
			}
		}

			if (INVALID_COORDS == selectedAdjPos)
			{
				cout << "ERROR: " << "INVALID_COORDS == selectedAdjPos" << endl;
				return;
			}

		actualPos = selectedAdjPos;
		vAssignedPoss.push_back(actualPos);
	}

    // now refine of 0 + rest of the rows
	vector<INT_COORDS> vPrevRowPoss;
	int nNumRows = v_vFormationRows.size();
    for (int nRow = 0; nRow < 2; ++nRow)
	{
		int nNumUnitsInRow = v_vFormationRows[nRow];
		if (0 == nRow)
		{
			vPrevRowPoss.clear();
			vPrevRowPoss.insert(vPrevRowPoss.end(),
								vAssignedPoss.begin(),
								vAssignedPoss.end());
			a_rvPoss.insert(a_rvPoss.end(),
							vPrevRowPoss.begin() + ((nMaxElemsInRow - nNumUnitsInRow) >> 1),
							vPrevRowPoss.begin() + ((nMaxElemsInRow - nNumUnitsInRow) >> 1) +
							nNumUnitsInRow);
			continue;
		}
		cout << "SIZE: " << vPrevRowPoss.size() << " " << nMaxElemsInRow << endl; 
		INT_COORDS maxLeftPos = vPrevRowPoss[0];
		INT_COORDS tblDiffs[] = {
			pair<int, int>(-1, 0),
			pair<int, int>(-1, 1)
		};
		INT_COORDS newSelPos = INVALID_COORDS;
		int nTblDiffsSize = (sizeof(tblDiffs) / sizeof(tblDiffs[0]));
		for (int i = 0; i < nTblDiffsSize; ++i)
		{
			INT_COORDS pos = make_pair(maxLeftPos.first	+ tblDiffs[i].first,
									   maxLeftPos.second + tblDiffs[i].second);
			vector<INT_COORDS>::iterator it = find(vPrevRowPoss.begin(),
												   vPrevRowPoss.end(),
												   pos);
			if (it == vPrevRowPoss.end())
			{
				newSelPos = pos;
				break;
			}
		}
			if (newSelPos == INVALID_COORDS)
			{
				cout << "ERROR: " << "newSelPos == INVALID_COORDS" << endl;
				return;
			}
		vector<INT_COORDS> vRowPoss;
		vRowPoss.push_back(newSelPos);
		int nPrevRowSize 	= vPrevRowPoss.size();
		int nPrevPos		= 0;
		for (int i = 0; i < nMaxElemsInRow; ++i)
		{
			bool bNextFound	= false;
			for (int j = 0; j < vDiffs1.size() && !bNextFound; ++j)
			{
				INT_COORDS pos = make_pair(newSelPos.first + vDiffs1[j].first,
										   newSelPos.second + vDiffs1[j].second);
				vector<INT_COORDS>::iterator it = find(vPrevRowPoss.begin(),
													   vPrevRowPoss.end(),
													   pos);
				if (it != vPrevRowPoss.end())
				{
					continue;
				}
				for (int k = nPrevPos + 1; k < nPrevRowSize; ++k)
				{
					INT_COORDS prevPos = vPrevRowPoss[k];
					if (MovementManager::m_getDistance(pos, prevPos, 1) == 1)
					{
						nPrevPos	= k;
						newSelPos	= pos;
						bNextFound	= true;
						break;
					}
				}
			}
				if (!bNextFound)
				{
					cout << "ERROR: " << "!bNextFound" << endl;
					return;
				}
				// TODO: check it!
				cout << ">>>" << vPrevRowPoss[0].first << " " << vPrevRowPoss[0].second << endl;
				cout << nPrevPos << endl;
				cout << vRowPoss[1].first << " " << vRowPoss[1].second << endl;
			vRowPoss.push_back(newSelPos);	
		}
		vPrevRowPoss.clear();
		vPrevRowPoss = vRowPoss;
		a_rvPoss.insert(a_rvPoss.end(),
						vPrevRowPoss.begin() + ((nMaxElemsInRow - nNumUnitsInRow) >> 1),
						vPrevRowPoss.begin() + ((nMaxElemsInRow - nNumUnitsInRow) >> 1) +
						nNumUnitsInRow);
	}
}

int Formation::m_nextStep()
{
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();

	if (!m_isValid())
	{
		return ERR_RESULT;
	}

	if (m_pathToDestNeedsRecompute())
	{
		m_recomputePathToDest();
	}

	if (FORMING == v_nFormationState)
	{
		int nNumUnitsFormed = 0;
		vector<FormationObjDescr>::iterator itUnits;
		for (itUnits = v_vUnits.begin(); itUnits != v_vUnits.end(); ++itUnits)
		{
			FormationObjDescr descr = *itUnits;
			BasicObject* pObj = descr.m_getObj();
			FORMATION_POS formPos = descr.m_getFormPos();
			INT_COORDS grPos = descr.m_getGridRelPos();
			INT_COORDS pos = make_pair(v_pos.first + grPos.first,
									   v_pos.second + grPos.second);
					
			pObj->m_nextStep();
			INT_COORDS actualPos = pObj->m_getPosition();
			if (actualPos == pos)
			{
				nNumUnitsFormed++;
			}
		}

		if (v_vUnits.size() == nNumUnitsFormed)
		{
				cout << "INFO: " << "Formation is formed NOW" << endl;
			v_nFormationState = FORMED;
		}
		
		return 0;
	}
	else if (NOT_FORMED == v_nFormationState)
	{
		if (v_vPoss.empty())
		{
			// m_getGRPoses(v_vPoss);
			vector<FormationObjDescr>::iterator itUnits;
			for (itUnits = v_vUnits.begin(); itUnits != v_vUnits.end(); ++itUnits)
			{
				FormationObjDescr descr = *itUnits;
				FORMATION_POS formPos = descr.m_getFormPos();
				itUnits->m_setGridRelPos(m_getGRPos(formPos));
			}
		}

		vector<FormationObjDescr>::iterator itUnits;
		for (itUnits = v_vUnits.begin(); itUnits != v_vUnits.end(); ++itUnits)
		{
			FormationObjDescr descr = *itUnits;
			BasicObject* pObj = descr.m_getObj();
			FORMATION_POS formPos = descr.m_getFormPos();
			INT_COORDS grPos = descr.m_getGridRelPos();
			INT_COORDS pos = make_pair(v_pos.first + grPos.first,
									   v_pos.second + grPos.second);
			if (!MovementManager::m_isValid(pos, 1))
			{
					cout << "ERROR: " << "!MovementManager::m_isValid(pos, 1)" << endl;
				return ERR_RESULT;
			}
			GridElement* pElem = const_cast<GridElement*>(pGrid->m_getGridElement(pos));
			if (0 == pElem)
			{
					cout << "ERROR: " << "0 == pElem" << endl;
				return ERR_RESULT;	
			}
			//if (pElem->m_isCoveredByStaticObject())
			{
			}

			set<INT_COORDS> setCovered;
			vector<PATHELEM> vPath;
			MovementManager::m_computePath(
				pObj,
				pos, 
				setCovered,
				vPath,
				true);
			pObj->m_setActualPath(vPath);
		}

		v_nFormationState = FORMING;

		return 0;
	}
	else if (FORMED == v_nFormationState)
	{
		PATHELEM pathElem0 = v_vPathToDest[0];
		if (1 != pathElem0.nResolution)
		{
				cout << "ERROR: " << "1 != pathElem0.nResolution" << endl;
			return ERR_RESULT;	
		}
		INT_COORDS pos0 = pathElem0.gridElem;
		bool bFoundGoodPosForFormation = m_checkIfGoodPositionForFormation(pos0);
		if (!bFoundGoodPosForFormation)
		{				
			// try some adjacent positions
			// 1) first from computed path 
			int nPathSize = v_vPathToDest.size(); 
			const int nMaxNextFromComputedPath = 5;
			for (int i = 1; i < nMaxNextFromComputedPath && i < nPathSize; ++i)
			{
				PATHELEM pathElem = v_vPathToDest[i];
				if (1 == pathElem.nResolution)
				{
					INT_COORDS pos = pathElem.gridElem;
					if (m_checkIfGoodPositionForFormation(pos))
					{
						v_vPathToDest.erase(v_vPathToDest.begin(),
											v_vPathToDest.begin() + i);
						bFoundGoodPosForFormation = true;
                        break;
					}
				}
				if (pathElem.nResolution > 1)
				{
					break;
				}
			}
			// try to recompute path
			if (!bFoundGoodPosForFormation)
			{
				int nMaxRadius = -INT_MAX;
				int nIndex = 0;
				vector<int>::iterator itRows;
				for (itRows = v_vFormationRows.begin(), nIndex = 0; itRows != v_vFormationRows.end(); ++itRows, ++nIndex)
				{
					int nNumUnitsInRow = *itRows;
					FORMATION_POS formPos = make_pair(nIndex, (nNumUnitsInRow >> 1));	
					INT_COORDS grPos0 = make_pair(0, 0);
					INT_COORDS grPos = m_getGRPos(formPos); 
					int nRadius = MovementManager::m_getDistance(grPos0, grPos, 1) + 1;
					if (nRadius > nMaxRadius)
					{
						nMaxRadius = nRadius;
					}
				}
					cout << "INFO: " << "nMaxRadius = " << nMaxRadius << endl;
				set<INT_COORDS> setCovered;
				vector<PATHELEM> vPath;
				MovementManager::m_computePath(v_pos,
											   v_destPos,
											   setCovered,
											   vPath,
											   nMaxRadius);
				if (vPath.empty())
				{
						cout << "ERROR: " << "Cannot make a formation" << endl;
					return ERR_RESULT;	
				}
				v_vPathToDest = vPath;
			}
		}
		
		pathElem0 = v_vPathToDest[0];
		if (1 != pathElem0.nResolution)
		{
				cout << "ERROR: " << "1 != pathElem0.nResolution" << endl;
			return ERR_RESULT;	
		}
		v_vPathToDest.erase(v_vPathToDest.begin()); // BAD!!
		PATHELEM pathElem1 = v_vPathToDest[0];
		if (1 < pathElem1.nResolution ||
			MovementManager::m_getDistance(pathElem0.gridElem, pathElem1.gridElem, 1) > 1)
		{
			// we have mipmaps or last element in mipmap chain
			vector<PATHELEM> vPath;
			MovementManager::m_recomputePath(0, pathElem0, pathElem1, vPath);
			
			vPath.erase(vPath.begin()); // BAD!!!

			BasicObject::m_replacePath(UNDEFINED, 0, vPath, v_vPathToDest);

			pathElem1 = vPath[0];			
		}
		v_nFormationState = NOT_FORMED;
		return 0;
	}

			cout << "INVALID STATE: " << v_nFormationState << endl;
	return ERR_RESULT;	
}

bool Formation::m_isValid()
{
	if (INVALID_COORDS == v_pos)
	{
		return false;
	}
	if (v_vFormationRows.size() == 0)
	{
		return false;
	}
	int nRequiredNumUnits = m_getRequiredNumUnits();
	if (0 == nRequiredNumUnits || v_vUnits.size() == 0)
	{
		return false;
	}

	return true;
}

bool Formation::m_checkIfGoodPositionForFormation(INT_COORDS a_leadPos)
{
	if (INVALID_COORDS == a_leadPos)
	{
		return false;
	}
	if (v_vPoss.empty())
	{
		return false;
	}

	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();

	vector<vector<INT_COORDS> > vAvailPoss = v_vPoss;

	int nCellsCovered = 0;
	vector<vector<INT_COORDS> >::iterator itPoss;
	for (itPoss = vAvailPoss.begin(); itPoss != vAvailPoss.end(); ++itPoss)
	{
		vector<INT_COORDS>& vRow = *itPoss;
		vector<INT_COORDS>::iterator itPoss2;
		for (itPoss2 = vRow.begin(); itPoss2 != vRow.end(); )
		{
			INT_COORDS grPos = *itPoss2;
			INT_COORDS pos = make_pair(a_leadPos.first + grPos.first,
									   a_leadPos.second + grPos.second);
			GridElement* pElem = const_cast<GridElement*>(pGrid->m_getGridElement(pos));
			if (pElem && pElem->m_isCoveredByObject())
			{
				itPoss2 = vRow.erase(itPoss2);
				nCellsCovered++;
			}
			else
			{
				++itPoss2;
			}
		}
	}

	vector<FormationObjDescr> vUnitsNotPlaced;

	vector<FormationObjDescr>::iterator itUnits;
	for (itUnits = v_vUnits.begin(); itUnits != v_vUnits.end(); ++itUnits)
	{
		FormationObjDescr descr = *itUnits;
		FORMATION_POS formPos = descr.m_getFormPos();
		int nRow = formPos.first;
		int nPosInRow = formPos.second;
		INT_COORDS grPos = m_getGRPos(formPos);
		INT_COORDS pos = make_pair(a_leadPos.first + grPos.first,
								   a_leadPos.second + grPos.second);
		GridElement* pElem = const_cast<GridElement*>(pGrid->m_getGridElement(pos));
		if (pElem && !pElem->m_isCoveredByObject())
		{
			vector<INT_COORDS>& vRow = vAvailPoss[nRow];
			vector<INT_COORDS>::iterator itPoss2 = find(vRow.begin(),
														vRow.end(),
														grPos);
			if (vRow.end() != itPoss2)
			{
				vRow.erase(itPoss2);
			}
		}
		if (pElem && pElem->m_isCoveredByObject()  )
		{
			vUnitsNotPlaced.push_back(descr);
		}
	}

	// try to rearrange...

	return true;
}

