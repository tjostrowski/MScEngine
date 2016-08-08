#ifndef __TEXTUREPATTERN_H__
#define __TEXTUREPATTERN_H__

#include "common_header.h"
#include "TextureObject.h"

/**
 * That is texture pattern that can consists of some
 * texture objects => it is forming one BasicObject
 * when it is texturable
 */
class TexturePattern
{
public:
	TexturePattern();
	TexturePattern(TextureObject a_rObj);
	virtual ~TexturePattern()  {}

	void 							m_addTextureObj(TextureObject a_rObj);
	inline vector<TextureObject>&   m_getTextureObjs() { return v_vTextureObjs; }

	int 							m_getPatternWidth();
	int 							m_getPatternHeight();

	inline void						m_masked_blit(BITMAP* a_pBmp, int a_nX, int a_nY) {}
    
protected:
	inline bool						m_checkValid(TextureObject& a_rObj) { return true; }
	inline void						m_spanPattern() {}

protected:
    vector<TextureObject>			v_vTextureObjs;

	int 							v_nWidth;
	int 							v_nHeight;

};

#endif // __TEXTUREPATTERN_H__
