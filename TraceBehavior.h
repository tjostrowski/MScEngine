#ifndef __TRACEBEHAVIOR_H__
#define __TRACEBEHAVIOR_H__

#include "common_header.h"


class TraceBehavior : public Behavior
{
public:
	inline TraceBehavior(BasicObject* a_pObj) :
		Behavior(a_pObj) {}
	TraceBehavior(BasicObject* a_pObj, BasicObject* a_pWho);
	virtual ~TraceBehavior();

	void				m_do();

	BITMAP*				m_getCurrentAnimationFrame();

	inline BasicObject*	m_getWho() { return v_pWho; } 
	inline void			m_setWho(BasicObject* a_pWho) { v_pWho = a_pWho; }

protected:
	/*inline void			__startBehavior() {}
    inline void			__haltBehavior() {}
	inline void			__stopBehavior() {}
    inline void			__resumeBehavior() {}*/

protected:
	bool				m_assertIntegrity();

protected:
	BasicObject*		v_pWho;

	static const int	RUN_ACTIVITY;

};

#endif // __TRACEBEHAVIOR_H__
