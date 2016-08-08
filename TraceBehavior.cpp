#include "common.h"


const int TraceBehavior::RUN_ACTIVITY = 0;

TraceBehavior::TraceBehavior(BasicObject* a_pObj, BasicObject* a_pWho) :
	Behavior(a_pObj),
	v_pWho(a_pWho)
{
	v_nId = TRACE_BEHAVIOR_ID;
}

TraceBehavior::~TraceBehavior()
{
}

void TraceBehavior::m_do()
{
	if (!m_assertIntegrity())
	{
		return;
	}

	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();

	INT_COORDS objPos = v_pObj->m_getPosition();
	INT_COORDS whoPos = v_pObj->m_getPosition();
		if (INVALID_COORDS == whoPos)
		{
			cerr << "INVALID_COORDS == whoPos" << endl;
			return;
		}

	vector<INT_COORDS> vDiffs;
	MovementManager::m_getDiffs(1, vDiffs); 

	// find grid element with lowest distance to whoPos
	int nMinDistance = INT_MAX;
	INT_COORDS nearestToWhoObjPos = INVALID_COORDS;
	vector<INT_COORDS>::const_iterator itDD;
	for (itDD = vDiffs.begin(); itDD != vDiffs.end(); ++itDD)
	{
		INT_COORDS diff = *itDD;
		INT_COORDS adjObjPos = make_pair(objPos.first + diff.first,
										 objPos.second + diff.second);
		GridElement* pElem = const_cast<GridElement*>(pGrid->m_getGridElement(
										adjObjPos));
		if (pElem && !pElem->m_isCoveredByObject())
		{
			int nDistance = MovementManager::m_getDistance(whoPos, adjObjPos, 1);
			if (nDistance < nMinDistance)
			{
				nMinDistance = nDistance;
				nearestToWhoObjPos = adjObjPos;
			}
		}
	}

	if (INVALID_COORDS == nearestToWhoObjPos)
	{
		// object cannot move
		return;
	}

	//  v_pObj->m_setPosition(nearestToWhoObjPos);
}

BITMAP* TraceBehavior::m_getCurrentAnimationFrame()
{
	/*ACTIVITY_MAP::iterator itAA = v_mapActivities.find(RUN_ACTIVITY);
		if (v_mapActivities.end() == itAA)
		{
			cerr << "Unable to find: " << " proper texture " << endl;
			return 0;
		}
	TextureObject& txtObj = (*itAA).second; 
	int nDirNum		= txtObj.m_getDirNum();
	int nStepNum	= txtObj.m_getStepNum();
	int nDir		= txtObj.m_getDir();
	int nStep 		= txtObj.m_getStep();

	// nDir ???
	nStep = (nStep + 1) % nStepNum;

	BITMAP* pBmp = txtObj.m_getAnimSubbitmap(nDir, nStep);

	return pBmp;*/
	return (BITMAP*)0;
}

bool TraceBehavior::m_assertIntegrity()
{
	return (0 != v_pWho);
}


