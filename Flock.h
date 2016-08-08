#ifndef __FLOCK_H__
#define __FLOCK_H__

#include "common_header.h"


typedef vector<PlaceableObj*> PL_OBJS_VEC;


class Flock
{
public:
	Flock(BasicObject* a_pObj, cpt3f& a_trgtPos = INVALID_POS);
	virtual ~Flock();

	inline void		m_setTargetPt(cpt3f& a_pt) { v_trgtPt = a_pt; }
	inline cpt3f	m_getTargetPt() { return v_trgtPt; }	

	cpt3f			m_getNewPosForObj();

	void			reset();

	vec3f			m_getLastVel();
	vec3f 			m_getAveVel();

	inline vec3f	m_getEstVel() { return v_estVel; }

	inline bool		m_isStopped() { return v_stopped; }
    inline void		m_stopFlock() { v_stopped = true; }
    void			m_restartFlock() { v_stopped = false; } 
    
protected:
	int				m_findNearPlObjs(PL_OBJS_VEC& a_vNearPlObjs);

	void			m_init();

	cvec3f			m_getTrgtVec();
	cvec3f 			m_getNearVelVec(UNITS_VEC& a_units, cvec3f& a_vel, vec3f& nearVelVec);
	cvec3f			m_getNearMidVec(UNITS_VEC& a_units, cvec3f& a_vel, vec3f& nearMidVec);
	cvec3f			m_getRepVec(PL_OBJS_VEC& a_vPlObjs, cvec3f& a_vel, vec3f& repVec);
    
protected:
	BasicObject*	v_pObj;
	

	pt3f			v_trgtPt;

	vector<vec3f>	v_velHistory;
	vec3f			v_estVel; // 'est'imated velocity 	

	bool			v_stopped;
};

#endif // __FLOCK_H__
