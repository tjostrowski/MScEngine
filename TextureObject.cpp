#include "common.h"

vec3f MAP2VEC(int dir)
{
	switch(dir)
	{
	case E_DIR_S:
		return cvec3f(0.f, 1.f, 0.f);	
	case E_DIR_SE:
		return cvec3f(1.f, 1.f, 0.f);
	case E_DIR_E:
		return cvec3f(1.f, 0.f, 0.f);
	case E_DIR_NE:
		return cvec3f(1.f, -1.f, 0.f);
	case E_DIR_N:
		return cvec3f(0.f, -1.f, 0.f);
	case E_DIR_NW:
		return cvec3f(-1.f, -1.f, 0.f);
	case E_DIR_W:
		return cvec3f(-1.f, 0.f, 0.f);
	case E_DIR_SW:	
		return cvec3f(-1.f, 1.f, 0.f);
	default:	
		break;
	}

	return INVALID_VEC;
}

TextureObject::TextureObject(int a_nHorizontalLine,
							 int a_nVerticalLine,
							 int a_nTxtId,
							 bool a_bIsAnimated) :
	v_nHorizontalLine(a_nHorizontalLine),
	v_nVerticalLine(a_nVerticalLine), 
	v_nTextureId(a_nTxtId),
	v_bIsAnimated(a_bIsAnimated)
{
	v_nDirNum				= UNDEFINED;
	v_nStepNum				= UNDEFINED;
	v_pAnimationFrameBmp	= 0;

	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	v_pBmp = pConf->m_getTextureById(v_nTextureId);
}

TextureObject::TextureObject(const TextureObjTmpl& txtTmpl)
{
	m_initFromTemplate(txtTmpl);
}

void TextureObject::m_initFromTemplate(const TextureObjTmpl& txtTmpl)
{
	v_nDirNum 		= txtTmpl.nDirNum;
	v_nStepNum		= txtTmpl.nStepNum;
    v_pBmp			= txtTmpl.pBmp;
	v_nTextureId	= txtTmpl.nTxtId;

	v_nDir	= 0;
	v_nStep = 0;
}

TextureObject::~TextureObject()
{
	if (0 != v_pAnimationFrameBmp)
	{
		destroy_bitmap(v_pAnimationFrameBmp);
	}
}

BITMAP* TextureObject::m_getBitmap()
{
	if(!m_assertValid())
	{
		return 0;
	}

	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	BITMAP* pBmp = pConf->m_getTextureById(v_nTextureId);
	
	return pBmp;
}

bool TextureObject::m_assertValid()
{
	return (UNDEFINED != v_nTextureId &&
			0 != v_pBmp);
}

void TextureObject::m_setAnimatedParams(int a_nDirNum, int a_nStepNum)
{
	if (v_bIsAnimated)
	{
		v_nDirNum	= a_nDirNum;
		v_nStepNum 	= a_nStepNum;
	}
}

int TextureObject::m_getFrameWidth()
{
	if (v_nDirNum > 0 && v_nStepNum > 0)
	{
		return (v_pBmp->w / v_nStepNum);
	}

	return UNDEFINED;
}

int TextureObject::m_getFrameHeight()
{
	if (v_nDirNum > 0 && v_nStepNum > 0)
	{
		return (v_pBmp->h / v_nDirNum);
	}

	return UNDEFINED;
}
/**
 * Uniform order of direction textures must be:
 * => S
 * => SE
 * => E
 * => NE
 * => N
 * => NW
 * => W
 * => SW
 * 
 * @param a_nDir
 * @param a_nStep
 * 
 * @return BITMAP*
 */
BITMAP* TextureObject::m_getAnimSubbitmap(int a_nDir, int a_nStep)
{
    if (v_nDirNum <= 0 || v_nStepNum <= 0)
	{
		return 0;
	}

    int a_nDirIndex		= a_nDir % v_nDirNum;
	int a_nStepIndex 	= a_nStep % v_nStepNum;

	int nFrameWidth		= m_getFrameWidth();
	int nFrameHeight	= m_getFrameHeight();

	if (0 == v_pAnimationFrameBmp)
	{
		v_pAnimationFrameBmp = create_bitmap(nFrameWidth, nFrameHeight);
	}

	int nXStart = a_nStepIndex * nFrameWidth;
	int nYStart = a_nDirIndex * nFrameHeight;

	//clear_to_color(v_pAnimationFrameBmp, makecol(0, 0, 0));
    ext_blit(
		v_pBmp, 
		v_pAnimationFrameBmp,
		nXStart,
		nYStart,
		nFrameWidth,
		nFrameHeight,
		0,
		0,
		nFrameWidth,
		nFrameHeight);

	return v_pAnimationFrameBmp;
}

BITMAP* TextureObject::m_getActualBitmap()
{
	assert((BITMAP*)0 != v_pBmp);
	return m_getAnimSubbitmap(v_nDir, v_nStep);
}

