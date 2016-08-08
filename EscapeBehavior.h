#ifndef __ESCAPEBEHAVIOR_H__
#define __ESCAPEBEHAVIOR_H__

#include "common_header.h"


class EscapeBehavior : public Behavior
{
public:
	inline EscapeBehavior(BasicObject* a_pObj) :
		Behavior(a_pObj) {}
    EscapeBehavior(BasicObject* a_pObj, BasicObject* a_pWho, INT_COORDS a_pos = INVALID_COORDS);
	virtual ~EscapeBehavior();

	void			m_do();

	BITMAP*			m_getCurrentAnimationFrame();	

	void 			m_setWho(BasicObject* a_pWho);
	void			m_setPos(INT_COORDS a_pos);

protected:
	/*inline void		__startBehavior() {}
    inline void		__haltBehavior() {}
	inline void		__stopBehavior() {}
    inline void		__resumeBehavior() {}*/

protected:
	bool			m_assertIntegrity();

protected:
	BasicObject*	v_pWho;
	INT_COORDS		v_pos;

	bool			v_bEscapeFromWho;

public:
	static const int RUN_ACTIVITY;

};

#endif // __ESCAPEBEHAVIOR_H__
