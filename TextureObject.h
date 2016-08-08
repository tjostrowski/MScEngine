#ifndef __TEXTUREOBJECT_H__
#define __TEXTUREOBJECT_H__

#include "common_header.h"


// w takiej kolejnosci wystepuja kierunki ruchu postaci w 'walk_01.tga'
enum eDir
{
	E_DIR_INVALID = -1,
	E_DIR_S = 0,
	E_DIR_SE,
	E_DIR_E,
	E_DIR_NE,
	E_DIR_N,
	E_DIR_NW,
	E_DIR_W,
	E_DIR_SW,
};

extern vec3f MAP2VEC(int dir);

/**
 * That is texture object that consists of texture
 * that can span some horizontal/vertical layers of the grid
 * (here: hexagonal)
 */
class TextureObject
{
public:
	TextureObject(int	a_nHorizontalLine = UNDEFINED, 
				  int	a_nVerticalLine = UNDEFINED, 
				  int	a_nTxtId = -1,
				  bool 	a_bIsAnimated = false);
	TextureObject(const TextureObjTmpl& txtTmpl);

	void m_initFromTemplate(const TextureObjTmpl& txtTmpl);

	virtual ~TextureObject();

	// number of cells in X direction
	inline int	m_getHorizontalLineNum() { return v_nHorizontalLine; }
	// number of cells in Y direction
	inline int	m_getVerticalLineNum() { return v_nVerticalLine; }

	inline int	m_getTextureId() { return v_nTextureId; }
    
	BITMAP*		m_getBitmap();

	BITMAP*		m_getActualBitmap();

	bool		m_assertValid();

	inline bool m_isAnimated() { return v_bIsAnimated; }
    
	void 		m_setAnimatedParams(int a_nDirNum, int a_nStepNum);

	inline int	m_getDirNum()	{ return v_nDirNum; }
	inline int	m_getStepNum()	{ return v_nStepNum; }

	int			m_getFrameWidth();
	int			m_getFrameHeight();

	BITMAP*		m_getAnimSubbitmap(int a_nDir, int a_nStep);

	inline int	m_getDir() { return v_nDir; }
    inline int	m_getStep() { return v_nStep; }

	inline void	m_setDir(int a_nDir) { v_nDir = a_nDir; }
	inline void m_setStep(int a_nStep) { v_nStep = a_nStep;  } 

	inline void	incDirStep() { v_nStep = ((v_nStep + 1) % v_nStepNum); }

	inline void resetDir(const eDir& a_dir)
	{
		v_nDir = a_dir;
		v_nStep = 0;
	}

protected:
	int			v_nHorizontalLine;
	int 		v_nVerticalLine;

	int			v_nTextureId;

	bool		v_bIsAnimated;

	int 		v_nDirNum;
	int			v_nStepNum;

	int			v_nDir;
	int			v_nStep;

	BITMAP*		v_pBmp;
	BITMAP*		v_pAnimationFrameBmp;
};

#endif // __TEXTUREOBJECT_H__
