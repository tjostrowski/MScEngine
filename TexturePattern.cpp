#include "common.h"

TexturePattern::TexturePattern() :
	v_nWidth(0), v_nHeight(0)
{
}

TexturePattern::TexturePattern(TextureObject a_rObj) :
	v_nWidth(0), v_nHeight(0)
{
	m_addTextureObj(a_rObj);
}

void TexturePattern::m_addTextureObj(TextureObject a_rObj)
{
	if (m_checkValid(a_rObj))
	{
		v_vTextureObjs.push_back(a_rObj);
		v_nWidth += 0;
	}
}

int TexturePattern::m_getPatternWidth()
{
	int nWidth = 0;
	return nWidth;
}

int TexturePattern::m_getPatternHeight()
{
	int nHeight = 0;
	return nHeight;
}
