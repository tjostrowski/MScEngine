#ifndef __ATTACKBEHAVIOR_H__
#define __ATTACKBEHAVIOR_H__

#include "common_header.h"


class AttackBehavior : public Behavior
{
public:
	/**
     * Attacks any enemy.
     * 
	 * @param a_pObj
	 */
	AttackBehavior(BasicObject* a_pObj);
	/**
     * Attacks specific enemy object (in first order).
     * 
	 * @param a_pObj
	 * @param a_pWho
	 */
	AttackBehavior(BasicObject* a_pObj, BasicObject* a_pWho);
	/**
     * Attacks enemies from specific set included in vector a_vWhos
     * (in first order).
     * 
	 * @param a_pObj
	 * @param a_vWhos
	 */
	AttackBehavior(BasicObject* a_pObj, vector<BasicObject*>& a_rvWhos);

	virtual ~AttackBehavior();

protected:
	/*inline void				__startBehavior() {}
    inline void				__haltBehavior() {}
    inline void				__stopBehavior() {}
    inline void				__resumeBehavior() {}*/

	void					m_planAttack();

protected:
	INT_COORDS				m_findAdjacentToEnemyPosToGoTo(INT_COORDS a_enemyPos);

public:
	void					m_do();

	BITMAP*					m_getCurrentAnimationFrame();

	inline void				m_setWho(BasicObject* a_pObj)
	{
		v_vWhos.clear();
		v_vWhos.push_back(a_pObj);
	}

	inline void				m_setWhos(vector<BasicObject*>& a_vWhos)
	{
		v_vWhos = a_vWhos;
	}

	inline void				m_addWho(BasicObject* a_pObj)
	{
		v_vWhos.push_back(a_pObj);
	}

	inline vector<BasicObject*> m_getWhos()
	{
		return v_vWhos;
	}

protected:

    /**
     * ...
	 * 
	 * @param a_rvEnemies
	 * 
     * @return int Number of found enemies
	 */
	int 					m_findAllEnemiesAtVisibility(vector<BasicObject*>& a_rvEnemies);

protected:
	/**
     * Find potential enemies in visibility area.
     * 
	 * @param a_rvAssignedWhos
	 */
	void					m_findPotentialEnemiesAtVisibility(vector<BasicObject*>& a_rvAssignedWhos);
	
	/**
     * Find best enemy to attack from given set (also best potential
     * enemy).
     * 
	 * @param a_rvEnemies
	 * 
	 * @return BasicObject*
	 */
	BasicObject*			m_findEnemyToAttack(vector<BasicObject*>& a_rvEnemies);

protected:
	bool					m_assertIntegrity();

	
protected:
	// enemies that are by default set to attack
	// (can be outside visibility area)
	vector<BasicObject*>	v_vWhos;

	int 					v_nActivity;

	// enemies that are in visibility area to attack
	vector<BasicObject*>	v_vEnemiesAtObjVisibility;

	// enemy that is attacked for now
	BasicObject*			v_pAttackedEnemy;

public:
	static const int 		RUN_ACTIVITY;
	static const int 		CLOSE_ATTACK_ACTIVITY;
	static const int 		DISTANT_ATTACK_ACTIVITY;


};

#endif // __ATTACKBEHAVIOR_H__
