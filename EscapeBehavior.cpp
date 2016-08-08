#include "common.h"


const int EscapeBehavior::RUN_ACTIVITY = 0;


EscapeBehavior::EscapeBehavior(BasicObject* a_pObj, BasicObject* a_pWho, INT_COORDS a_pos) :
	Behavior(a_pObj),
	v_pWho(a_pWho),
	v_pos(a_pos)
{
	v_bEscapeFromWho = (0 != v_pWho) ? true : false;
	v_nId = ESCAPE_BEHAVIOR_ID;
}

EscapeBehavior::~EscapeBehavior()
{
}

void EscapeBehavior::m_setWho(BasicObject* a_pWho)
{
	v_pWho = a_pWho;
	v_bEscapeFromWho = (0 != v_pWho) ? true : false;
}

void EscapeBehavior::m_setPos(INT_COORDS a_pos)
{
	v_pos = a_pos;
	v_bEscapeFromWho = (INVALID_COORDS == v_pos) ? true : false;
}

void EscapeBehavior::m_do()
{
	if (!m_assertIntegrity())
	{
		return;
	}

	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();

	INT_COORDS objPos			= v_pObj->m_getPosition();
	INT_COORDS escapeFromPos	= (v_bEscapeFromWho) ?
								  v_pWho->m_getPosition() :
								  v_pos;

	vector<INT_COORDS> vDiffs;
	MovementManager::m_getDiffs(1, vDiffs);
	int nMaxDistance = -INT_MAX;
	INT_COORDS farestFromEPObjPos = INVALID_COORDS;
	vector<INT_COORDS>::const_iterator itDD;
	for (itDD = vDiffs.begin(); itDD != vDiffs.end(); ++itDD)
	{
		INT_COORDS diff	= *itDD;
		INT_COORDS adjObjPos = make_pair(objPos.first + diff.first,
										 objPos.second + diff.second);
		GridElement* pElem = const_cast<GridElement*>(pGrid->m_getGridElement(
										adjObjPos));
		if (pElem && !pElem->m_isCoveredByObject())
		{
			int nDistance = MovementManager::m_getDistance(adjObjPos,
														   escapeFromPos, 
														   1);
			if (nDistance > nMaxDistance)
			{
				nMaxDistance = nDistance;
                farestFromEPObjPos = adjObjPos;
			}
		}
	}

	if (INVALID_COORDS == farestFromEPObjPos)
	{
		// object surrounded
		// stay on your place
		return;
	}

	v_pObj->m_setNextStep(farestFromEPObjPos);
}

BITMAP* EscapeBehavior::m_getCurrentAnimationFrame()
{
	/*ACTIVITY_MAP::iterator itAA = v_mapActivities.find(RUN_ACTIVITY);
		if (itAA == v_mapActivities.end())
		{
			cerr << " Unknown animation "
				 << " for activity: "
				 << " RUN_ACTIVITY " << endl;	
		}
	TextureObject& txtObj = (*itAA).second; 	

	int nDir		= txtObj.m_getDir();
	int nStep		= txtObj.m_getStep();
	int nDirNum 	= txtObj.m_getDirNum();
	int nStepNum	= txtObj.m_getStepNum();

	INT_COORDS objPos		= v_pObj->m_getPosition();
	INT_COORDS nextObjPos	= v_pObj->m_getPosition(1); 
		if (INVALID_COORDS == objPos || INVALID_COORDS == nextObjPos)
		{
			cerr << " Invalid object position "
				 << " or next object position "
				 << objPos.first << " " << objPos.second << " "
				 << nextObjPos.first << " " << nextObjPos.second << " " << endl;
		}
	int nNewDir = BasicObject::m_handleChangeDir(nextObjPos, 
												 objPos);
	if (nNewDir != nDir)
	{
		nStep = 0;
	}
	else
	{
		nStep = (nStep + 1) % nStepNum; 
	}

	BITMAP* pBmp = txtObj.m_getAnimSubbitmap(nNewDir, nStep);

	return pBmp;*/
	return (BITMAP*)0;
}

bool EscapeBehavior::m_assertIntegrity()
{
	return	(0 != v_pWho && v_bEscapeFromWho) || 
			(INVALID_COORDS != v_pos && !v_bEscapeFromWho); 
}

