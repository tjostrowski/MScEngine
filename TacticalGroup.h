#ifndef __TACTICALGROUP_H__
#define __TACTICALGROUP_H__

#include "common_header.h"



class Shape
{
public:
	virtual bool ptIsInside(cpt3f& a_pt) = 0;

	inline  bool isValid() { return valid; }

protected:
	bool valid;

};


class Rectangle : public Shape
{
public:
	Rectangle(cpt3f& a_luPt, cpt3f& a_drPt);
	virtual bool ptIsInside(cpt3f& a_pt);

protected:
	pt3f luPt;
	pt3f drPt; 
};


class Circle : public Shape
{
public:
	Circle(cpt3f& a_ctr, cfloat& a_rad);
	virtual bool ptIsInside(cpt3f& a_pt);

protected:
	pt3f	ctr;
	float	rad;
};


class Triangle : public Shape
{
public:
	Triangle(cpt3f& a_pt1, cpt3f& a_pt2, cpt3f& a_pt3);
	virtual bool ptIsInside(cpt3f& a_pt);

protected:
	pt3f pt1;
	pt3f pt2;
	pt3f pt3;
};


typedef vector<TacticalGroup*> 				TACTICGRP_VEC;
typedef vector<BasicObject*> 				UNITS_VEC;

enum eFormShape
{
	E_SHP_NONE,
	E_SHP_RECTANGLE,
	E_SHP_CIRCLE,
	E_SHP_TRIANGLE,
};


class TacticalGroup;
typedef TacticalGroup TG;

extern ulong DEF_TG_BEHAVIOR;


class TacticalGroup : public CtrlObject, public DrawableObject, public Idxable
{
public:
    TacticalGroup(const uint& a_assign, TGManager* a_pManager, culong& a_beh = DEF_TG_BEHAVIOR);
	TacticalGroup(const uint& a_assign, const UNITS_VEC& unitsVec, TGManager* a_pManager, culong& a_beh = DEF_TG_BEHAVIOR);
	inline TacticalGroup() {}
	virtual ~TacticalGroup();

	void					m_init();

	void 					m_addObj(BasicObject* a_pObj);
	void					m_addObjs(UNITS_VEC& a_vec);
	void					m_removeObj(BasicObject* a_pObj);
	// is SPLITTING succesful?
	bool					m_split(const uint a_nSubs = (uint)UNDEFINED, cbool a_smpl = true);
	// is JOINING succesful?
	bool					m_join(TacticalGroup*& a_tg);

	void					m_update();

	inline uint				m_getNumObjs() { return v_vObjs.size();  }

	void					m_getObjs(UNITS_VEC& a_vv);

	// deleted?
	bool					delObjRef(BasicObject* a_pObj);

	// who directs this group
	inline const uint&		m_getAssign() { return assign; }

	inline pt3f 			m_getGroupCenter() const { return v_grpCenter; }
	inline float			m_getGroupRadius() const { return v_grpRadius; }
	inline float			m_getGroupDispersion() const { return v_grpDispersion; }
	inline float			m_getGroupForce() const { return v_grpForce;  }
	inline float 			m_getGroupThreat() const { return v_grpThreat; }

	static int				comp_TGsByHlpDemand(const void* a_p1, const void* a_p2);

	// <0.f, 1.f>
	inline float			m_getHlpDemand() const
	{
		cfloat frcF = .6f;
		cfloat thrtF = 1.f - frcF;

		float hlpDemand = (frcF * (1.f - v_grpForce)) + (thrtF * v_grpThreat);

        return clampf(hlpDemand, 0.f, 1.f);  
	}

	void					m_form();
	
	bool					m_isFormed();

    bool					m_isInFOV(BasicObject* a_pObj, TG* a_pTG, cvec3f& a_bltDir);

	virtual cpt3f 			m_getNewPosForObj(BasicObject* a_pObj);

	void					m_draw(int a_nUserId, BITMAP* a_pDestBmp);

	void 					m_setDestPt(cpt3f& a_pt);

	inline void 			m_setFormShape(eFormShape a_fs) { v_shape = a_fs; }

	void					m_getSortedEnTGsByHlpDemand(TACTICGRP_VEC& a_vec); // 'En'emy 'TG's
	void					m_getSortedFrTGsByHlpDemand(TACTICGRP_VEC& a_vec); // 'Fr'iend 'TG's

    inline void				m_setBehavior(ulong a_beh) { v_beh = a_beh; }
    
protected:
	cpt3f&		  			m_computeGroupCenter();
	cfloat&					m_computeGroupRadius();
	cfloat&					m_computeGroupDispersion();
    cfloat&					m_computeFrmdGrpRadius();	
	cfloat&					m_computeGroupForce();
	cfloat&					m_computeGroupThreat();

	int						m_getNeighbTGs();
    void					m_allocateUnitsToEnemyTGs();
	void					m_recalculateGroupParams();
	void					m_shoot();

	bool					destPtReached();

	bool					m_isStrictlyFormed();
	bool					m_expand();
	bool					m_pack();

	ulong					inOK_AndIfThenStart(culong& a_beh);
	bool					outOK(culong& a_beh);
	void					proceed(culong& a_beh);
	void					m_handleBehavior();

protected:
    UNITS_VEC				v_vObjs;
	map<BasicObject*, TacticalGroup*>
			v_mapObjs2TGEnemTGVs;
	map<BasicObject*, vec3f>
			v_mapObjs2Velcs;
	
    TACTICGRP_VEC			v_vNeighbTGs;
	TACTICGRP_VEC			v_vEnTGs;
	TACTICGRP_VEC			v_vFrTGs;

	TGManager*				v_pManager;

	uint					assign; 	

	pt3f					v_grpCenter;
	float					v_grpRadius;
	float					v_frmdGrpRadius;
	float					v_grpDispersion;
	// <0.f, 1.f>
	float					v_grpForce;
	// <0.f, 1.f>
	float					v_grpThreat;

	pt3f					v_grpDestPt;

	eFormShape				v_shape;

	ulong					v_beh;
	ulong					v_jinBeh; 	// 'j'ust 'i'n 'n'ow behavior
	ulong					v_jinBehST; // 'j'ust 'i'n 'n'ow behavior 'S'tart 'T'ime

	static ulong 			MAX_BEH_TIME;

	bool					strictForm;

};

#endif // __TACTICALGROUP_H__
