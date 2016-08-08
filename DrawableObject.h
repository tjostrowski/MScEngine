#ifndef __DRAWABLEOBJECT_H__
#define __DRAWABLEOBJECT_H__

#include "common_header.h"
#include "TexturePattern.h"

// Basic classes inherited by others
// to 'express' common obj properties such as:
// controlling, drawing, ...


class CtrlObject
{
public:
	virtual cpt3f m_getNewPosForObj(BasicObject* a_pObj) = 0;
};

class PlaceableObj
{
public:
	virtual cpt3f m_getPos() =0;

	// width & height in cells
	virtual cfloat m_getWidth() = 0;
	virtual cfloat m_getHeight() = 0;
};

extern BulletClass G_DEF_BULLET_CLASS;

class BulletThrowable
{
public:
	void m_throwBullet(cpt3f& a_endPos, BulletClass* a_pClass = &G_DEF_BULLET_CLASS, bool a_bCollide = true);

	virtual cpt3f m_getPos() = 0;
};


class Idxable
{
public:
	inline Idxable() :
		idx(IDRef++) {}

	inline uint getIdx() const { return idx; }

	inline uint m_getId() const { return getIdx();  }

protected:
	static uint IDRef;
	uint idx;
};


class equal_obj_by_idx
{
	uint idx;
public: 
	equal_obj_by_idx(const uint& a_nIdx) :
		idx(a_nIdx) {}

    bool operator()(Idxable* a_pOtherObj) const
	{
		return a_pOtherObj->getIdx() == idx;
	}
};


class DrawableObject
{
public:
	DrawableObject();
	virtual ~DrawableObject()  {}

	virtual void 			m_draw(int a_nUserId, BITMAP* a_pDestBmp) = 0;

	inline void				m_setTexturePattern(TexturePattern a_texturePattern)
	{
		v_texturePattern = a_texturePattern;
	}
	inline TexturePattern&	m_getTexturePattern() { return v_texturePattern; } 

	inline int				m_getColor() { return v_nColor; } 
    void 					m_setColor(int a_nRed, int a_nGreen, int a_nBlue);

	inline bool 			m_isTextured() { return v_bTextured; }
	inline bool				m_isFillAll() { return v_bFillAll; }
    inline void 			m_setTextParams(bool a_bTextured, bool a_bFillAll)
	{ 
		v_bTextured	= a_bTextured;
		v_bFillAll	= a_bFillAll;
	}

	inline int				m_getTxtID0()
	{
		vector<TextureObject>& txtObjs = v_texturePattern.m_getTextureObjs();
		if (txtObjs.empty())
			return UNDEFINED;

		return txtObjs[0].m_getTextureId();
	}

protected:
	int m_getDefaultColor();

protected:
	// is object texture ot solid color
	bool			v_bTextured;

	// should texture fill all the object (or be repeated?)
	bool			v_bFillAll;

	// texture pattern
	TexturePattern	v_texturePattern;
	
	// or using that color object
	// will be painted
	int				v_nColor;

	// other graphical data...
	bool			v_bIsAlphaBlending;	
};

#endif // __DRAWABLEOBJECT_H__
