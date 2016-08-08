#include "common.h"


BasicObjectBehaviorManager::BasicObjectBehaviorManager(BasicObject* a_pObj) :
	v_pObj(a_pObj),
	v_pActualBehavior(0)
{
}

BasicObjectBehaviorManager::~BasicObjectBehaviorManager()
{
}

Behavior* BasicObjectBehaviorManager::m_createPlainNewBehavior(const int a_nBehaviorId)
{
	switch(a_nBehaviorId)
	{
	case ATTACK_BEHAVIOR_ID:
		return new AttackBehavior(v_pObj);
	case PATROL_BEHAVIOR_ID:
		return new PatrolBehavior(v_pObj);
	case ESCAPE_BEHAVIOR_ID:
		return new EscapeBehavior(v_pObj);
	case TRACE_BEHAVIOR_ID:
		return new TraceBehavior(v_pObj);
	default:
		break;	
	}

	return 0;
}

bool BasicObjectBehaviorManager::m_startNewBehavior(Behavior* a_pBehavior)
{
	/*if (!a_pBehavior)
	{
		cerr << " Not properly initialized input behavior: "
			 << endl;
		return false;
	}

	if (v_pActualBehavior && v_pActualBehavior->m_isActive())
	{
		v_pActualBehavior->m_haltBehavior();
	}
	if (!v_pActualBehavior ||
		(v_pActualBehavior && v_pActualBehavior->m_isHalted()))
	{
        delete v_pActualBehavior;
		v_pActualBehavior = a_pBehavior;

		v_pActualBehavior->m_startBehavior();

		return true;
	}
	if (v_pActualBehavior && !v_pActualBehavior->m_isHalted())
	{
		v_vBehaviorsToStart.push_back(a_pBehavior);
	}

	return false;*/
	delete v_pActualBehavior;
	v_pActualBehavior = a_pBehavior;

	return true;
}

bool BasicObjectBehaviorManager::m_stopActualBehavior()
{
	if (!v_pActualBehavior)
	{
		return false;
	}

	if (v_pActualBehavior->m_isActive())
	{
		v_pActualBehavior->m_stopBehavior();
	}
	if (v_pActualBehavior->m_isStopped())
	{
		return true;
	}

	return false;
}

bool BasicObjectBehaviorManager::m_resumeActualBehavior()
{
	if (!v_pActualBehavior)
	{
		return false;
	}

	if ((v_pActualBehavior->m_isStopped() || v_pActualBehavior->m_isAssignedToStop()))
	{
		v_pActualBehavior->m_resumeBehavior();
		return true;
	}

	return false;
}

bool BasicObjectBehaviorManager::m_haltActualBehavior()
{
	if (!v_pActualBehavior)
	{
		return false;
	}

	if (v_pActualBehavior->m_isActive())
	{
		v_pActualBehavior->m_haltBehavior();
	}
	if (v_pActualBehavior->m_isHalted())
	{
		return true;
	}
	
	return false;
}

void BasicObjectBehaviorManager::m_do()
{
	if (v_pActualBehavior)
	{
		// TODO: HERE BUG in AttackBehavior
		v_pActualBehavior->m_do();

		if (v_pActualBehavior->m_isHalted())
		{
			delete v_pActualBehavior;
			if (!v_vBehaviorsToStart.empty())
			{
				Behavior* pBehaviorToStart = v_vBehaviorsToStart[0];
				v_pActualBehavior = pBehaviorToStart;
				v_vBehaviorsToStart.clear();

				v_pActualBehavior->m_startBehavior();
			}
			else
			{
				v_pActualBehavior = 0;
			}
		}
	}

	return;
}

