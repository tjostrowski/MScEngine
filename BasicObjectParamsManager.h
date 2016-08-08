#ifndef __BASICOBJECTPARAMS_H__
#define __BASICOBJECTPARAMS_H__

#include <string>
#include "common_header.h"
#include "TimeFrameScheduledObject.h"
#include "DrawableObject.h"


enum PARAM_TYPES
{
	AMMO_TYPE = 0,
	OTHER_TYPE,
};

enum PARAM_IDS 
{	
	// some common param ids
	VISIBILITY_ID = 0,
	HEALTH_ID,
	STAMINA_ID,
	AMMO_DEFAULT_ID,
	AMMO_OTHER_ID, 
};

#define MIN_FORCE 0.f
#define MAX_FORCE 100.f

struct BulletClass
{
	uint 	id;
    // force of bullet, range 0-100.f.
	float 	force;
	//  % of cell made each turn
	float	speed;
	// max num of cells tb made 
	float	reach;
	// fly texture id
	uint	flyTxtId;
	// zasieg wybuchu
	float 	explReach;
	// tekstura wybuchu
	uint	explTxtId;

	inline BulletClass(uint a_id,
					   float a_force, 
					   float a_speed,
					   float a_reach,
					   uint a_flyTxtId, 
					   float a_explReach,
					   uint a_explTxtId = (uint)UNDEFINED) :
		id(a_id),
		force(a_force), 
		speed(a_speed),
		reach(a_reach), 
		flyTxtId(a_flyTxtId), 
		explReach(a_explReach),
		explTxtId(a_explTxtId) {}
};

extern BulletClass G_DEF_BULLET_CLASS;
extern BulletClass G_DEF2_BULLET_CLASS;
extern BulletClass G_BOMB_BULLET_CLASS;


struct Ammo
{
	// bullet class id
	uint bcId;
	uint num;
};

/**
 * Special class for handling bullets.
 */
class Bullet : public Idxable
{
public:
	Bullet(LineMove<Bullet>* a_pLM,
		   BulletClass* a_pClass = &G_DEF_BULLET_CLASS);

	Bullet(cpt3f& a_startPos,
		   cpt3f& a_endPos, 
		   BulletClass* a_pClass = &G_DEF_BULLET_CLASS,
		   bool a_bCollide = true);

	Bullet(cpt3f& a_startPos,
		   cvec3f& a_dir,
		   bool a_useDir,
		   BulletClass* a_pClass = &G_DEF_BULLET_CLASS);

	inline BulletClass* m_getClass() { return v_pClass; }
	
	void				m_update();

    BITMAP*				m_getCurrentAnimationFrame();	

	virtual void		m_draw(int a_nUserId, BITMAP* a_pDestBmp);

    inline bool			m_exists() { return exists;  }

	cpt3f				m_getPos();

	friend ostream&		operator<<(ostream& os, const Bullet& bu);

protected:
	void				m_initDefaultParams();

protected:

	LineMove<Bullet>*	v_pLM;
	BulletClass*		v_pClass;

protected:
	map<uint /* ACTIVITY_ID */, TextureObject /* texture associated with this activity */>
		v_mapActivities2TxtObjs;
	uint				activity;

protected:
	bool				exists;
	bool				shouldExplode;

public:
	static const uint	NORMAL_FLY_ACTIVITY;
// skipped for now!
	static const uint	EXPLOSION_ACTIVITY;	
};


struct FParam
{
	float	minVal;
	float	maxVal;
	float	val;

	inline FParam(cfloat& a_minVal = 0.f, cfloat& a_maxVal = 100.f, cfloat a_val = 100.f) :
		minVal(a_minVal), maxVal(a_maxVal), val(a_val) {}

};

/**
 * Handling parameters (such as life, ammunition etc.)
 * for specific object.
 * Created because of refactoring class *BasicObject
 *
 */
class BasicObjectParamsManager : public TimeFrameScheduledObject
{
public:
	BasicObjectParamsManager(BasicObject* a_pObj);
	inline virtual ~BasicObjectParamsManager() {}

	inline void					m_addNewParameter(int a_nType, int a_nId, string& a_strDescr, void* a_pValue) {}

	inline void					m_update() {}

	void						m_initDefaultParams();

protected:
	BasicObject*				v_pObj;

public:
	// minimum parameter values are '0'
    FParam						health;

	FParam						stamina;

	FParam						fovRAD;
	FParam						fovANG; // zawsze s STOPNIACH a nie w RADIANACH!!!
};

typedef BasicObjectParamsManager UnitParamsMAN; 

typedef BulletClass BClass;

#endif // __BASICOBJECTPARAMS_H__

