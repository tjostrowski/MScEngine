#ifndef __LINEMOVE_H__
#define __LINEMOVE_H__

#include "common_header.h"

export template <class T>
class LineMove
{
public:
    LineMove(T* obj,
			 const pt3f& a_posFrom,
			 const vec3f& a_dir, 
			 const float a_rng,
			 const bool a_bCollisions = true,
			 const float a_speed = 1.f);
	LineMove(T* obj,
			 const pt3f& a_posFrom, 
			 const pt3f& a_posTo, 
			 bool a_bCollisions = true,
			 const float a_speed = 1.f);

	inline virtual 		~LineMove() {}
	
	inline cpt3f		m_getPos() const { return v_pos; }
    
	void 				m_update();

	void				init();

	inline bool			ended() { return v_ended; }

	inline T*			m_getMovObj() { return movObj; }
    
protected:
	BasicObject*		getTouchedObj(cpt3f& a_stPos, cpt3f& a_endPos);

protected:
	T*					movObj;

	pt3f				v_posFrom;
	vec3f				v_dir;
	float				v_rng;

	// 1.f => 
	// every second cellSizeX distance
	float				v_speed;

	float				v_bCollisions;

	pt3f				v_pos;

	// last checked collisions pos
	pt3f				v_chckdCollsPos;

	bool				v_ended;
};

#endif // __LINEMOVE_H__
