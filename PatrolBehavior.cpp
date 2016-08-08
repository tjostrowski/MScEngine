#include "common.h"


const int PatrolBehavior::RUN_ACTIVITY = 0;


PatrolBehavior::PatrolBehavior(BasicObject* a_pObj) :
	Behavior(a_pObj),
	v_nActualPatrolPointIndex(UNDEFINED),
	v_nActivity(UNDEFINED)
{
	v_nId = PATROL_BEHAVIOR_ID;
}

PatrolBehavior::~PatrolBehavior()
{
}

void PatrolBehavior::m_do()
{
	if (!m_assertIntegrity())
	{ 
		return;
	}

	INT_COORDS objPos = v_pObj->m_getPosition();

    if (!v_bReconfiguredToBehavior)
	{
		// case 1: must reconfigure to behavior

		// find nearest patrol point
		int nMinDistance = INT_MAX;
		INT_COORDS nearestPatrolPoint = INVALID_COORDS;
		int nNearestPatrolPointIndex = UNDEFINED;
		int ii = UNDEFINED;
		vector<INT_COORDS>::const_iterator itPP;
		for (ii = 0, itPP = v_vPatrolPoints.begin(); itPP != v_vPatrolPoints.end(); ++itPP, ++ii)
		{
			INT_COORDS patrolPoint = *itPP;
			int nDistance = MovementManager::m_getDistance(objPos,
														   patrolPoint, 
														   1);
			if (nDistance < nMinDistance)
			{
				nMinDistance = nDistance;
				nearestPatrolPoint = patrolPoint;
				nNearestPatrolPointIndex = ii; 
			}
		}

		INT_COORDS objActualTarget = v_pObj->m_getActualTarget();
		if (objActualTarget != nearestPatrolPoint)
		{
			v_pObj->m_setActualTarget(nearestPatrolPoint, true);
		}

		v_nActualPatrolPointIndex 	= nNearestPatrolPointIndex;
		v_nActivity 				= RUN_ACTIVITY;
		v_bReconfiguredToBehavior 	= true;

		return;
	}

	if (UNDEFINED == v_nActualPatrolPointIndex)
	{
		cerr << " PatrolBehavior::m_do " 
			 << " INVALID PATROL POINT INDEX  " 
			 << endl;
		return;
	}

	INT_COORDS nextPatrolPoint = v_vPatrolPoints[v_nActualPatrolPointIndex];
	if (objPos == nextPatrolPoint)
	{
		v_nActualPatrolPointIndex = (v_nActualPatrolPointIndex + 1) % v_vPatrolPoints.size();
		v_pObj->m_setActualTarget(v_vPatrolPoints[v_nActualPatrolPointIndex],
								  true);
	}
}

void PatrolBehavior::m_setPatrolPoints(vector<INT_COORDS>& a_rvPatrolPoints)
{	
	v_vPatrolPoints.clear();
	v_vPatrolPoints = a_rvPatrolPoints;
}

void PatrolBehavior::m_addPatrolPoint(INT_COORDS a_patrolPoint)
{
	vector<INT_COORDS>::const_iterator itPP = find(v_vPatrolPoints.begin(), 
												   v_vPatrolPoints.end(),
												   a_patrolPoint);
	if (v_vPatrolPoints.end() != itPP)
	{
		return;
	}

	v_vPatrolPoints.push_back(a_patrolPoint);
}

bool PatrolBehavior::m_assertIntegrity()
{
	return (v_vPatrolPoints.size() > 0);  
}

BITMAP* PatrolBehavior::m_getCurrentAnimationFrame()
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

