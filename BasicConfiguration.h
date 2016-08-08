#ifndef __BASICCONFIGURATION_H__
#define __BASICCONFIGURATION_H__

#include "common_header.h"

#define __TRACE_ME
#define DEFAULT_FPS 20.f

// This class handles basic configuration
// such as grid map, graphics configuration data
// and so on
class BasicConfiguration
{
public:
    virtual					   	~BasicConfiguration();
    
	static BasicConfiguration*	m_getBasicConfiguration();
    
	inline MapGrid*				m_getGridMap() { return v_pMap; }

    /**
     * Returns temporary bitmap that will be used
     * for doing some calculations
     * 
	 * @return BITMAP*
	 */
	BITMAP* 					m_getTmpBitmap(int a_nRectWidth, int a_nRectHeight);

    int 						m_associateTexture(const char* a_strFileName, bool a_bScaleToDef = true);
	BITMAP*						m_getTextureById(int a_nId);
	// returns associated texture id
	int 						m_assocTxt2(const char* a_strFileName,
											const int a_nSelId = UNDEFINED,
											const int a_nDirNum = 1,
											const int a_nStepNum = 1,
											bool a_bScaleToDef = true);
	int							m_assocTxt2(const TextureObjTmpl& a_txtTmpl);
    bool						m_associateBasicTxts2();
	// returns true if texture found
    bool						m_getTextureObjById2(int a_nTxtId, TextureObjTmpl& a_tmpl);
    
    GameView* 					m_createGameView(int a_nUserId);
	GameView* 					m_getGameViewByUserId(int a_nUserId);

	inline void 				m_setColDepth(int a_nDepth) { v_nColDepth = a_nDepth; }
	inline int					m_getColDepth() { return v_nColDepth; }

	inline static void			setFPS(float a_fps) { assert(a_fps > 0.f); fps = a_fps; }
	inline static float			getFPS() { return fps; }

protected:
	static bool					_init;
	static bool					sinit();

protected:
	void 						init();
	bool 						assertIntegrity();

public:
	static inline ulong			m_getFrameNo() { return v_nFrameNo;  }
	static inline void			incFrameNo() { v_nFrameNo++; }
    
protected:
	BasicConfiguration() {}

	static BasicConfiguration*	v_pConfig;	

    MapGrid*					v_pMap;

    BITMAP*						v_pTmpBitmap;

	map<int, GameView*>			v_mapViews;

	map<int, BITMAP*>			v_mapTextures;

	typedef map<int, TextureObjTmpl> TXTS_MAP;
	TXTS_MAP					v_mapTextures2;

	int							v_nColDepth;

	static int					nTextureId;

public:
	static ulong				v_nFrameNo;

protected:
	static float				fps;
};

#endif // __BASICCONFIGURATION_H__
