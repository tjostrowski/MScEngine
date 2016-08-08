#include "common.h"


const int Group::MAX_NUM_OBJECTS			= 100;
const int Group::MAX_MAX_DIST_TO_LEADER		= 10;
const int Group::MAX_MAX_DIST_TO_PATH		= 5;
const int Group::MAX_MAX_PATH_STEP_BACKWARD	= 20;


Group::Group(int a_nId) :
	v_nId(a_nId)
{
	v_pLeader		= 0;
	v_actualTarget	= INVALID_COORDS; 
}

Group::~Group()
{
}

int Group::m_getObjectsNum()
{
	return v_vObjects.size();
}

void Group::m_addObject(BasicObject* a_pObj)
{
	if (0 == a_pObj)
	{
		return;
	}

	if (v_vObjects.size() > MAX_NUM_OBJECTS)
	{
		return;
	}

	if (0 == v_pLeader)
	{
		v_pLeader = a_pObj;
	}

	int nClass = m_classifyObject(a_pObj);

	v_vObjects.push_back(make_pair(a_pObj,
		GroupObjDescr(GroupObjDescr::RANK_OTHER, nClass)));
}

void Group::m_removeObject(BasicObject* a_pObj)
{
	if (0 == a_pObj)
	{
		return;
	}

	VEC_GROUP::iterator it = find_if(v_vObjects.begin(),
									 v_vObjects.end(),
									 equal_object(a_pObj));
	
	if (v_vObjects.end() == it)
	{
		return;
	}

	v_vObjects.erase(it);

	if (v_pLeader == a_pObj)
	{
		m_setDefaultLeader();
		m_classifyObjects();
	}
}

void Group::m_classifyObjects()
{
	int nSize = m_getObjectsNum();
	for (int i = 0; i < nSize; ++i)
	{
		BasicObject* pObj = v_vObjects[i].first;
		int nClass = m_classifyObject(pObj);
		v_vObjects[i].second.nClass = nClass; 
	}
}

int Group::m_classifyObject(BasicObject* a_pObj)
{
	if (0 == v_pLeader)
	{
		return GroupObjDescr::UNCLASSIFIED;
	}

	int nPathSize = v_vLeaderPath.size();
	if (0 == nPathSize)
	{
		return GroupObjDescr::UNCLASSIFIED;
	}

    int nDistToLeader = MovementManager::m_getDistance(v_pLeader->m_getPosition(),
													   a_pObj->m_getPosition(),
													   1);   
    if (nDistToLeader > MAX_MAX_DIST_TO_LEADER)
	{
		int nPathSize = v_vLeaderPath.size();
		for (int i = nPathSize - 1; i >= 0; --i)
		{
			int nDistToPathElem = MovementManager::m_getDistance(a_pObj->m_getPosition(),
																 v_vLeaderPath[i].gridElem, 
																 1);
            if (nDistToPathElem <= MAX_MAX_DIST_TO_PATH)
			{
				return GroupObjDescr::CLASS_PATH_NOT_GROUP_STEERED;
			}			
		}

		return GroupObjDescr::CLASS_SELF_STEERED;
	}

	for (int i = nPathSize - 1; i >= nPathSize - MAX_MAX_PATH_STEP_BACKWARD; --i)
	{
		int nDistToPathElem = MovementManager::m_getDistance(a_pObj->m_getPosition(),
															 v_vLeaderPath[i].gridElem, 
															 1);
		if (nDistToPathElem <= MAX_MAX_DIST_TO_PATH)
		{
			return GroupObjDescr::CLASS_GROUP_STEERED;
		}
	}

	return GroupObjDescr::CLASS_PARTLY_GROUP_STEERED;
}

void Group::m_setDefaultLeader()
{
	BasicObject* pLeader = 0;//v_vObjects[0];
	m_setLeader(pLeader);
}

void Group::m_setLeader(BasicObject* a_pObj)
{
	if (0 == a_pObj)
	{
		return;
	}

	VEC_GROUP::iterator it = find_if(v_vObjects.begin(),
									 v_vObjects.end(),
									 equal_object(a_pObj));

	if (v_vObjects.end() == it)
	{
		// no such object found in group
		return;
	}

	v_pLeader = a_pObj;
}

void Group::m_nextStep()
{
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();

	if (!m_isConsistent())
	{
		m_makeConsistent();
	}

	m_classifyObjects();

    INT_COORDS leadStep = v_pLeader->m_nextStep();
	VEC_GROUP vGroupSteered;
	VEC_GROUP::iterator it;
	for (it = v_vObjects.begin(); it != v_vObjects.end(); ++it)
	{
		GROUP_OBJ		obj			= *it;
		BasicObject*	pObj		= obj.first;
		GroupObjDescr 	objDescr	= obj.second;

        INT_COORDS pos = pObj->m_getPosition(); 

			cout << "LEADER POS: " << v_pLeader->m_getPosition().first << " " << v_pLeader->m_getPosition().second << endl;  
			cout << "CLASS: " << objDescr.nClass << endl;

		//if (objDescr.nClass == GroupObjDescr::UNCLASSIFIED)
		{
			// trivial case: no need to move
			pObj->m_nextStep();
		}
        /*else if (objDescr.nClass == GroupObjDescr::CLASS_SELF_STEERED)
		{
			INT_COORDS target = pObj->m_getActualTarget();
			if (INVALID_COORDS == target)
			{
				pObj->m_setActualTarget(
					m_predictLeaderPos(
						MovementManager::m_getDistance(pos, target, 1)),
					true);
			}

			pObj->m_nextStep();

            m_classifyObject(pObj);
		}
		else if (objDescr.nClass == GroupObjDescr::CLASS_PATH_NOT_GROUP_STEERED)
		{
			int nLPathIndex	= objDescr.nLPathIndex;
			if (UNDEFINED == nLPathIndex)
			{
				cerr << "UNDEFINED == nLPathIndex" << endl;
				return;
			}
			int nPathDistance = MovementManager::m_getDistance(
				pos, v_vLeaderPath[nLPathIndex].gridElem, 1);
			if (nLPathIndex == v_vLeaderPath.size() - 1)
			{
				cerr << "nLPathIndex == v_vLeaderPath.size() - 1" << endl;
				return;
			}
			vector<INT_COORDS> vPotentialElems2;
			vector<INT_COORDS> vDiffs;
			MovementManager::m_getDiffs(1, vDiffs);
			bool bNextStepAssigned = false;	
			vector<INT_COORDS>::iterator itDiff;
			for (itDiff = vDiffs.begin(); itDiff != vDiffs.end(); ++itDiff)
			{
				INT_COORDS diff = *itDiff;
				INT_COORDS nextPos = 
					make_pair(pos.first + diff.first, pos.second + diff.second);
				GridElement* pElem = const_cast<GridElement*>(pGrid->m_getGridElement(
										nextPos));
				if (pElem && !pElem->m_isCoveredByObject())
				{
					int nNextPathDistance = MovementManager::m_getDistance(
						nextPos, v_vLeaderPath[nLPathIndex + 1].gridElem, 1);
					if (nNextPathDistance <= nPathDistance)
					{
						// this grid element is OK
						pObj->m_setNextStep(nextPos);
						bNextStepAssigned = true;
						break;
					}
					else
					{
						vPotentialElems2.push_back(nextPos);
					}
				}
			}
			if (!bNextStepAssigned)
			{
				if (vPotentialElems2.empty())
				{
					// stay on actual position
					pObj->m_setNextStep(pos);	
				}
				else
				{
					pObj->m_setNextStep(vPotentialElems2[0]);
				}
			}

			m_classifyObject(pObj);
		}
		else if (objDescr.nClass == GroupObjDescr::CLASS_PARTLY_GROUP_STEERED)
		{
			vGroupSteered.push_back(obj);
		}
		else
		{
			// GroupObjDescr::CLASS_GROUP_STEERED
			vGroupSteered.push_back(obj);	
		}*/
	}

	if (!vGroupSteered.empty())
	{
		vector<INT_COORDS> vDiffs;
		for (int i = 1; i <= v_nMaxGroupDist + 1; ++i)
		{
			vector<INT_COORDS> vDiffsTmp;
			MovementManager::m_getDiffs(i, vDiffsTmp);
			vDiffs.insert(
				vDiffs.end(),
				vDiffsTmp.begin(),
				vDiffsTmp.end());
		}
		vector<INT_COORDS> vDiffs1;
		MovementManager::m_getDiffs(1, vDiffs1);

		VEC_GROUP::iterator it;
		for (it = vGroupSteered.begin(); it != vGroupSteered.end(); ++it)
		{
			GROUP_OBJ		obj			= *it;
			BasicObject*	pObj		= obj.first;
			GroupObjDescr 	objDescr	= obj.second;

			int nLPathIndex	= objDescr.nLPathIndex;
			INT_COORDS pos	= pObj->m_getPosition();

			int nLeadDistance = MovementManager::m_getDistance(
				pos, v_pLeader->m_getPosition(), 1);
			int nPathDistance = MovementManager::m_getDistance(
				pos, v_vLeaderPath[nLPathIndex].gridElem, 1);

			vector<BasicObject*> vNeighbObjs;
			vector<INT_COORDS>::iterator itDiff;
			for (itDiff = vDiffs.begin(); itDiff != vDiffs.end(); ++itDiff)
			{
				INT_COORDS diff = *itDiff;
				INT_COORDS neighbPos = 
					make_pair(pos.first + diff.first, pos.second + diff.second);
				GridElement* pElem = const_cast<GridElement*>(pGrid->m_getGridElement(
										neighbPos));
				if (pElem && pElem->m_isCoveredByObject())
				{
					// TODO: add constraint only group objects
					vNeighbObjs.push_back(pElem->m_getCoveringObject());
				}
			}

			vector<INT_COORDS> vPotentialElems2;
			bool bNextStepAssigned = false;
			vector<INT_COORDS>::iterator itDiff1;
			for (itDiff1 = vDiffs1.begin(); itDiff1 != vDiffs1.end(); ++itDiff1)
			{
				INT_COORDS diff1 = *itDiff1;
				INT_COORDS nextPos =
					make_pair(pos.first + diff1.first, pos.second + diff1.second);

				GridElement* pElem = const_cast<GridElement*>(pGrid->m_getGridElement(
										nextPos));
				if (pElem && pElem->m_isCoveredByObject())
				{
					// invalid element
					continue;
				}

				int nNextLeadDistance = MovementManager::m_getDistance(
					nextPos, v_pLeader->m_getPosition(), 1);
				int nNextPathDistance = MovementManager::m_getDistance(
					nextPos, v_vLeaderPath[nLPathIndex + 1].gridElem, 1);

				bool bObjFoundAtLessMinGroupDist = false,
					 bObjFoundAtMostMaxGroupDist = false;
				vector<BasicObject*>::iterator itObj; 
				for (itObj = vNeighbObjs.begin(); itObj != vNeighbObjs.end(); ++itObj)
				{
					BasicObject* pObj = *itObj;

					int nDistance = MovementManager::m_getDistance(
						nextPos, pObj->m_getPosition(), 1);

					if (nDistance < v_nMinGroupDist)
					{
						// TODO: take into consider. only already moved group elems
						bObjFoundAtLessMinGroupDist = true;
						break;
					}

					if (nDistance <= v_nMaxGroupDist)
					{
						bObjFoundAtMostMaxGroupDist = true;
					}
				}

				bool bValid = !bObjFoundAtLessMinGroupDist && bObjFoundAtMostMaxGroupDist; 

				if (bValid && 1 /**/)
				{
					//pObj->m_setNextStep(nextPos);
					bNextStepAssigned = true;
					break;
				}

				if (bValid)
				{
					vPotentialElems2.push_back(nextPos);
				}
			}
			if (!bNextStepAssigned)
			{
				if (vPotentialElems2.empty())
				{
					// stay on actual position
					pObj->m_setNextStep(pos);	
				}
				else
				{
					pObj->m_setNextStep(vPotentialElems2[0]);
				}
			}

			m_classifyObject(pObj);
		}

	}

		cout << "SZ: " << v_vLeaderPath.size() << endl;

	if (!(v_pLeader->m_isStandingStill()))
	{
		v_vLeaderPath.push_back(PATHELEM(leadStep, 1));
	}
}

// C: 2*v_vObjects.size() + C(A*(MAX_MAX_DIST_TO_LEADER)) + C(m_classifyObjects) + gluepath.size() 
// or 3*v_vObjects.size() + C(m_classifyObjects) 
void Group::m_makeConsistent()
{
	if (0 == v_vObjects.size())
	{
		return;
	}

	// try to assign one object 
	// with class CLASS_GROUP_STEERED as leader
	VEC_GROUP::iterator it = find_if(v_vObjects.begin(),
									 v_vObjects.end(),
									 equal_class(GroupObjDescr::CLASS_GROUP_STEERED));

	if (v_vObjects.end() != it)
	{
		BasicObject* pObj = (*it).first;
		v_pLeader = pObj;

		// refine leader path
		if (!v_vLeaderPath.empty())
		{
			PATHELEM prevLeaderPos	= v_vLeaderPath.back();
			PATHELEM newLeaderPos(pObj->m_getPosition(), 1);
			vector<PATHELEM> vGluePath;
			MovementManager::m_recomputePath(0,
											 prevLeaderPos,
											 newLeaderPos,
											 vGluePath);
			v_vLeaderPath.insert(v_vLeaderPath.end(),
								 vGluePath.begin(),
								 vGluePath.end());
		}
	}
	else
	{
		v_pLeader = v_vObjects[0].first;
		v_vLeaderPath.erase(v_vLeaderPath.begin(), v_vLeaderPath.end());
	}
}

void Group::m_setActualTarget(INT_COORDS a_coords)
{
	if (INVALID_COORDS == a_coords)
	{
		return;
	}

	if (0 == v_pLeader)
	{
		return;
	}

	// recompute path for leader
	v_pLeader->m_setActualTarget(a_coords, true);
	
	v_actualTarget = a_coords;
}

INT_COORDS Group::m_predictLeaderPos(int a_nStep)
{
	// TODO: to fill
	return INVALID_COORDS;
}

bool Group::m_isConsistent()
{
	return (0 != v_pLeader);
}



