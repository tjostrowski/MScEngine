#include "common.h"


const int AttackBehavior::RUN_ACTIVITY				= 0;
const int AttackBehavior::CLOSE_ATTACK_ACTIVITY		= 1;
const int AttackBehavior::DISTANT_ATTACK_ACTIVITY	= 1 << 1;


AttackBehavior::AttackBehavior(BasicObject* a_pObj) :
	Behavior(a_pObj),
	v_nActivity(UNDEFINED),
	v_pAttackedEnemy(0)
{
	v_nId = ATTACK_BEHAVIOR_ID;
}

AttackBehavior::AttackBehavior(BasicObject* a_pObj, BasicObject* a_pWho) :
	Behavior(a_pObj)
{
	v_vWhos.push_back(a_pWho);
}

AttackBehavior::AttackBehavior(BasicObject* a_pObj, vector<BasicObject*>& a_rvWhos) :
	Behavior(a_pObj)
{
	v_vWhos = a_rvWhos;
}

AttackBehavior::~AttackBehavior()
{
}

void AttackBehavior::m_do()
{
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();
	BasicObjectParamsManager* pParamsManager = v_pObj->m_getParamsManager();

	INT_COORDS objPos			= v_pObj->m_getPosition();
	int nObjVisibility			= 0;//pParamsManager->m_getVisibility();
	int nObjMaxAttackDistance 	= 0;//pParamsManager->m_getMaxAttackDistance();

    v_vEnemiesAtObjVisibility.clear();	
	m_findPotentialEnemiesAtVisibility(v_vEnemiesAtObjVisibility);

		//cout << "OBJ ID: " << v_pObj->getIdx() << " ACTIVITY: " << v_nActivity << endl;
		cout << "nObjVisibility: " << nObjVisibility << endl;
		cout << "nObjMaxAttackDistance: " << nObjMaxAttackDistance << endl;

    /*if (UNDEFINED != v_nActivity)
	{
		// FSM transitions
		// check transition from RUN_ACTIVITY
		if (RUN_ACTIVITY == v_nActivity)
		{
			if (!v_pAttackedEnemy)
			{
				m_planAttack();
				return;
			}

			const int nDistanceToAttackedEnemy = 
				MovementManager::m_getDistance(objPos, v_pAttackedEnemy->m_getPosition(), 1);
			if (1 == nDistanceToAttackedEnemy)
			{
				v_nActivity = CLOSE_ATTACK_ACTIVITY;
			}
			else if (nDistanceToAttackedEnemy <= nObjMaxAttackDistance && 1 < nObjMaxAttackDistance)
			{
				v_nActivity = DISTANT_ATTACK_ACTIVITY;
			}
			else if (nDistanceToAttackedEnemy > nObjVisibility)
			{
				m_planAttack();
				return;
			}
			else
			{
				// do nothing..., preoceed with running 
				// ev. correct path
				// TODO: optimize IT!!!
				INT_COORDS destPos = m_findAdjacentToEnemyPosToGoTo(v_pAttackedEnemy->m_getPosition());
				if (INVALID_COORDS == destPos)
				{
					cerr << " AttackBehavior::m_do() : "
						 << " NO POSITION TO ATTACK FOUND "
						 << endl;
					v_nActivity = UNDEFINED;
					return;
				}
				v_pObj->m_setActualTarget(destPos,
										  true);
			}
			
			return;
		}
		else if (CLOSE_ATTACK_ACTIVITY == v_nActivity)
		{
			if (!v_pAttackedEnemy)
			{
				m_planAttack();
				return;
			}

            int nDistanceToAttackedEnemy = 
				MovementManager::m_getDistance(objPos,
											   v_pAttackedEnemy->m_getPosition(),
											   1);
			
			// if actual enemy not in distance 1
			// find one in distance 1
			if (nDistanceToAttackedEnemy > 1)
			{
				vector<BasicObject*>::iterator itAE;
				for (itAE = v_vEnemiesAtObjVisibility.begin(); itAE != v_vEnemiesAtObjVisibility.end(); ++itAE)
				{
					BasicObject* pEnemy = *itAE;
					INT_COORDS enemyPos = pEnemy->m_getPosition();
					int nDistanceToEnemy = MovementManager::m_getDistance(objPos, enemyPos, 1);
					if (1 == nDistanceToEnemy)
					{
						v_pAttackedEnemy 			= pEnemy;
						nDistanceToAttackedEnemy	= 1;
						break;
					}
				}
			}

            // if no enemy found in distance 1
			// try to reach one
			if (nDistanceToAttackedEnemy > 1)
			{
				if (nDistanceToAttackedEnemy > nObjVisibility)
				{
					m_planAttack();
					return;
				}
				else 
				{
					if (nDistanceToAttackedEnemy <= nObjMaxAttackDistance)
					{	
						v_nActivity = DISTANT_ATTACK_ACTIVITY;
					}
					else
					{
						v_pObj->m_setActualTarget(v_pAttackedEnemy->m_getPosition(),
												  true);
						v_nActivity = RUN_ACTIVITY;
					}

				}
			}

			return;
		}
		else if (DISTANT_ATTACK_ACTIVITY == v_nActivity)
		{
			if (!v_pAttackedEnemy)
			{
				m_planAttack();
				return;
			}

			int nDistanceToAttackedEnemy = 
				MovementManager::m_getDistance(objPos,
											   v_pAttackedEnemy->m_getPosition(),
											   1);
			
			// if actual enemy in distance > nObjMaxAttackDistance 
			// find one in distance <= nObjMaxAttackDistance
			if (nDistanceToAttackedEnemy > nObjMaxAttackDistance)
			{
				vector<BasicObject*>::iterator itAE;
				for (itAE = v_vEnemiesAtObjVisibility.begin(); itAE != v_vEnemiesAtObjVisibility.end(); ++itAE)
				{
					BasicObject* pEnemy = *itAE;
					INT_COORDS enemyPos = pEnemy->m_getPosition();
					int nDistanceToEnemy = MovementManager::m_getDistance(objPos, enemyPos, 1);
					if (nDistanceToEnemy <= nObjMaxAttackDistance)
					{
						v_pAttackedEnemy 			= pEnemy;
						nDistanceToAttackedEnemy 	= nDistanceToEnemy;
						break;
					}
				}
			}

			// .....................
			// if no enemy found in distance <= nObjMinAttackDistance
			// try to reach one
			if (nDistanceToAttackedEnemy > nObjMaxAttackDistance)
			{
				v_pAttackedEnemy = m_findEnemyToAttack(v_vEnemiesAtObjVisibility);
				v_pObj->m_setActualTarget(v_pAttackedEnemy->m_getPosition(), true);

				v_nActivity = RUN_ACTIVITY;
			}
			else
			{ 
				// introduce new bullet into the world
				const Layer* pLayer = Layer::m_getLayerAtLevel(v_pObj->m_getLayerLevel());
				BasicObjectParamsManager* pParamsManager = v_pObj->m_getParamsManager();
				(const_cast<Layer*>(pLayer))->m_addBullet(
					pParamsManager->m_shootBullet(
							v_pAttackedEnemy->m_getPosition()));
			}

			return;
		}
	}*/

	// no state, planning what to do; v_nActivity == UNDEFINED
	m_planAttack();

    return;
}

void AttackBehavior::m_planAttack()
{
	BasicConfiguration* pConf 	= BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid 				= pConf->m_getGridMap();
	BasicObjectParamsManager* pParamsManager = v_pObj->m_getParamsManager();

	INT_COORDS objPos			= v_pObj->m_getPosition();
	int nObjVisibility			= 0;//pParamsManager->m_getVisibility();
	int nObjMaxAttackDistance 	= 0;//pParamsManager->m_getMaxAttackDistance();

	v_pAttackedEnemy = m_findEnemyToAttack(v_vEnemiesAtObjVisibility);
	if (!v_pAttackedEnemy)
	{
		cout << " AttackBehavior::m_do() : "
			 << " NO ENEMY TO ATTACK FOUND "
			 << endl;
		v_nActivity = UNDEFINED;
		return;
	}

	int nDistanceToEnemy = MovementManager::m_getDistance(objPos,
														  v_pAttackedEnemy->m_getPosition(),
														  1);
	if (1 == nDistanceToEnemy && 1 == nObjMaxAttackDistance)
	{
		// go to state CLOSE_ATTACK_ACTIVITY
		v_nActivity = CLOSE_ATTACK_ACTIVITY;
		return;
	}
	if (nDistanceToEnemy <= nObjMaxAttackDistance)
	{
		// go to state DISTANT_ATTACK_ACTIVITY
		v_nActivity = DISTANT_ATTACK_ACTIVITY;
		return;
	}

	// find one adjacent position to attacked enemy that is not covered by object
	INT_COORDS attackedEnemyPos = v_pAttackedEnemy->m_getPosition();
	INT_COORDS destPos 			= this->m_findAdjacentToEnemyPosToGoTo(attackedEnemyPos);
	if (INVALID_COORDS == destPos)
	{
		cerr << " AttackBehavior::m_do() : "
			 << " NO POSITION TO ATTACK FOUND "
			 << endl;
		v_nActivity = UNDEFINED;
		return;
	}
		cout << "POSITION: " << v_pObj->m_getPosition().first << " " << v_pObj->m_getPosition().second << endl;
		cout << "ATTACKING OBJ ID: " << v_pObj->m_getId() << " ATTACKS:  " << v_pAttackedEnemy->m_getId() << endl;
		cout << "TO POSITION: " << destPos.first << " " << destPos.second << endl;
	v_pObj->m_setActualTarget(destPos,
							  true);

	v_nActivity = RUN_ACTIVITY;

	return;
}

BITMAP* AttackBehavior::m_getCurrentAnimationFrame()
{
	/*ACTIVITY_MAP::iterator itAA = v_mapActivities.find(v_nActivity);
		if (itAA == v_mapActivities.end())
		{
			cerr << "itAA == v_mapActivities.end()" << endl;
			return 0;
		}
	TextureObject& txtObj = (*itAA).second;
	
	int nDir		= txtObj.m_getDir();
	int nStep		= txtObj.m_getStep();
	int nDirNum 	= txtObj.m_getDirNum();
	int nStepNum	= txtObj.m_getStepNum();

    INT_COORDS objPos = v_pObj->m_getPosition();

	int nNextStepDir = UNDEFINED;
	if (RUN_ACTIVITY == v_nActivity)
	{
		INT_COORDS nextObjPos = v_pObj->m_getPosition(1);
		nNextStepDir = BasicObject::m_handleChangeDir(nextObjPos , 
												 objPos );
	}
	else if (CLOSE_ATTACK_ACTIVITY == v_nActivity)
	{
		INT_COORDS attackedEnemyPos = v_pAttackedEnemy->m_getPosition();
		nNextStepDir = BasicObject::m_handleChangeDir(attackedEnemyPos , 
												 objPos );
	}
	else if (DISTANT_ATTACK_ACTIVITY == v_nActivity)
	{
		// TODO: to fill
	}

	if (nNextStepDir != nDir)
	{
		nStep = 0;
	}
	else
	{
		nStep = (nStep + 1) % nStepNum;
	}

	BITMAP* pBmp = txtObj.m_getAnimSubbitmap(nNextStepDir, nStep);

	return pBmp;*/
	return (BITMAP*)0;
}

void AttackBehavior::m_findPotentialEnemiesAtVisibility(vector<BasicObject*>& a_rvEnemies)
{
	/*BasicConfiguration* pConf 					= BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid 								= pConf->m_getGridMap();
	BasicObjectParamsManager* pParamsManager 	= v_pObj->m_getParamsManager();

	INT_COORDS objPos			= v_pObj->m_getPosition();
	int nObjVisibility			= pParamsManager->m_getVisibility();
	int nObjMaxAttackDistance 	= pParamsManager->m_getMaxAttackDistance();

	if (v_vWhos.empty())
	{
		// check enemies in visibility area
		int nFoundEnemies = m_findAllEnemiesAtVisibility(a_rvEnemies);
	}
	else
	{
		// check if set enemies are in visibility area
		vector<BasicObject*>::iterator itEE;
		for (itEE = v_vWhos.begin(); itEE != v_vWhos.end(); ++itEE)
		{
			BasicObject* pEnemy = *itEE;
			int nDistanceToEnemy = MovementManager::m_getDistance(objPos,
																  pEnemy->m_getPosition(),
																  1);
			if (nDistanceToEnemy <= nObjVisibility)
			{
				a_rvEnemies.push_back(pEnemy);
			}
		}
		if (v_vEnemiesAtObjVisibility.empty())
		{
			int nFoundEnemies = m_findAllEnemiesAtVisibility(a_rvEnemies);
				cout << "FOUND ENEMIES: " << nFoundEnemies << endl;
		}
	}*/
}

int AttackBehavior::m_findAllEnemiesAtVisibility(vector<BasicObject*>& a_rvEnemies)
{
	BasicConfiguration* pConf					= BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid								= pConf->m_getGridMap();
	BasicObjectParamsManager* pParamsManager 	= v_pObj->m_getParamsManager();

	INT_COORDS objPos			= v_pObj->m_getPosition();
	int nObjVisibility			= 0;//pParamsManager->m_getVisibility();
	int nObjMaxAttackDistance 	= 0;//pParamsManager->m_getMaxAttackDistance();

	// check enemies in visibility area
	for (int ii = 1; ii < nObjVisibility; ++ii)
	{
		vector<INT_COORDS> vDiffs;
		MovementManager::m_getDiffs(ii, vDiffs);
		vector<INT_COORDS>::iterator itDD;
		for (itDD = vDiffs.begin(); itDD != vDiffs.end(); ++itDD)
		{
			INT_COORDS diff = *itDD;
			INT_COORDS pos = make_pair(objPos.first + diff.first,
									   objPos.second + diff.second);
			if (!MovementManager::m_isValid(pos, 1))
			{
				continue;
			}
			GridElement* pElem = const_cast<GridElement*>(pGrid->m_getGridElement(
									pos));
			if (pElem && pElem->m_isCoveredByObject())
			{
				BasicObject* pObj = pElem->m_getCoveringObject();
				// TODO: check if is enemy
				if (a_rvEnemies.end() == find(a_rvEnemies.begin(), a_rvEnemies.end(), pObj) &&
					1 /* check if enemy */)
				{
						a_rvEnemies.push_back(pObj);
				}
			}
		} /* end for */
	}

	return a_rvEnemies.size();
}

BasicObject* AttackBehavior::m_findEnemyToAttack(vector<BasicObject*>& a_rvEnemies)
{
	if (!a_rvEnemies.empty())
	{
		return a_rvEnemies[0];
	}

	return 0;
}

INT_COORDS	AttackBehavior::m_findAdjacentToEnemyPosToGoTo(INT_COORDS a_enemyPos)
{
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();

	INT_COORDS destPos = INVALID_COORDS;

    vector<INT_COORDS> vDiffs1;
	MovementManager::m_getDiffs(1, vDiffs1);
	vector<INT_COORDS>::iterator itDD;
	for (itDD = vDiffs1.begin(); itDD != vDiffs1.end(); ++itDD)
	{
		INT_COORDS diff = *itDD;
		INT_COORDS pos = make_pair(a_enemyPos.first + diff.first,
								   a_enemyPos.second + diff.second);
		if (!MovementManager::m_isValid(pos, 1))
		{
			continue;
		}
		GridElement* pElem = const_cast<GridElement*>(pGrid->m_getGridElement(pos));
		if (pElem && !pElem->m_isCoveredByObject())
		{
			destPos = pos;
			break;
		}
	}
	
	return destPos;
}

bool AttackBehavior::m_assertIntegrity()
{
	return true;
}


