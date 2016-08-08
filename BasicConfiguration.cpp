#include "common.h"


BasicConfiguration*	BasicConfiguration::v_pConfig	= 0;
int					BasicConfiguration::nTextureId	= 0;
ulong				BasicConfiguration::v_nFrameNo	= 0;

float				BasicConfiguration::fps;

bool				BasicConfiguration::_init		= sinit();
		
bool BasicConfiguration::sinit()
{
	fps = DEFAULT_FPS;
}

BasicConfiguration::~BasicConfiguration()
{
	destroy_bitmap(v_pTmpBitmap);
	map<int, BITMAP*>::iterator itTexts;
	for (itTexts = v_mapTextures.begin(); itTexts != v_mapTextures.end(); ++itTexts)
	{
		BITMAP* pBmp = (*itTexts).second;
		destroy_bitmap(pBmp);
		(*itTexts).second = 0;
	}
	v_mapTextures.clear();
	map<int, GameView*>::iterator itViews;
	for (itViews = v_mapViews.begin(); itViews != v_mapViews.end(); ++itViews)
	{
		GameView* pView = (*itViews).second;
		delete pView;
		(*itViews).second = 0;
	}
	v_mapViews.clear();
}

BasicConfiguration* BasicConfiguration::m_getBasicConfiguration()
{
	if (0 == v_pConfig)
	{
		v_pConfig = new BasicConfiguration;
		v_pConfig->init();
	}

	return v_pConfig;
}

void BasicConfiguration::init()
{
	// ...
	v_pTmpBitmap = 0;
	v_pMap = new HexagonalMapGrid();
}

BITMAP* BasicConfiguration::m_getTmpBitmap(int a_nRectWidth, int a_nRectHeight)
{
	if (0 == v_pTmpBitmap)
	{
		v_pTmpBitmap = create_bitmap(a_nRectWidth, a_nRectHeight);
	}
	else
	{
		int nWidth 	= v_pTmpBitmap->w;
		int nHeight	= v_pTmpBitmap->h;

		if (nWidth < a_nRectWidth || nHeight < a_nRectHeight)
		{
			destroy_bitmap(v_pTmpBitmap);
			v_pTmpBitmap = create_bitmap(max(nWidth,  a_nRectWidth),
										 max(nHeight, a_nRectHeight));
		}
	}

	return v_pTmpBitmap;
}

GameView* BasicConfiguration::m_createGameView(int a_nUserId)
{
	GameView* pView = new GameView(a_nUserId);
	v_mapViews.insert(make_pair(a_nUserId, pView));

	return pView;
}

GameView* BasicConfiguration::m_getGameViewByUserId(int a_nUserId)
{
	map<int, GameView*>::iterator it = v_mapViews.find(a_nUserId);
	if (v_mapViews.end() == it)
	{
		return 0;
	}
	return (*it).second;
}

int BasicConfiguration::m_associateTexture(const char* a_strFileName, bool a_bScaleToDef)
{
    PALETTE palette;
	BITMAP* pBmp = load_bitmap(a_strFileName, NULL);
	if (!pBmp)
	{
		cerr << "Unable to load bitmap: " << a_strFileName << endl;
		return UNDEFINED;
	}

	if (!a_bScaleToDef)
	{
		typedef map<int, BITMAP*>::value_type value_type;
		int nId = nTextureId++;
		v_mapTextures.insert(value_type(nId, pBmp));

		return nId;
	}

	const int nGridElemSizeX = v_pMap->m_getGridElementSizeX();
	const int nGridElemSizeY = v_pMap->m_getGridElementSizeY();

	BITMAP* pScaledBmp = create_bitmap(nGridElemSizeX, nGridElemSizeY);

	ext_blit(pBmp, 
			 pScaledBmp,
			 0,
			 0, 
			 pBmp->w,
			 pBmp->h,
			 0,
			 0,
			 nGridElemSizeX,
			 nGridElemSizeY);

	destroy_bitmap(pBmp);

	typedef map<int, BITMAP*>::value_type value_type;
	int nId = nTextureId++;
	v_mapTextures.insert(value_type(nId, pScaledBmp));

	return nId;
}

BITMAP* BasicConfiguration::m_getTextureById(int a_nId)
{
	map<int, BITMAP*>::iterator it = v_mapTextures.find(a_nId);
	if (v_mapTextures.end() == it)
	{
		return (BITMAP*)0;
	}
	return (*it).second;
}

int BasicConfiguration::m_assocTxt2(const char* a_strFileName,
									const int a_nSelId,
									const int a_nDirNum,
									const int a_nStepNum,
									bool a_bScaleToDef)
{
	assert(a_nDirNum > 0);
	assert(a_nStepNum > 0);

	int newId = UNDEFINED;
	if (a_nSelId == UNDEFINED)
		newId = ++nTextureId;
	else
	{
		if (v_mapTextures2.find(a_nSelId) == v_mapTextures2.end())
			newId = a_nSelId;
		else
			newId = ++nTextureId;	
	}

	BITMAP* pBmp = load_bitmap(a_strFileName, NULL);
	if (!pBmp)
	{
#ifdef __TRACE_ME
		cerr << "Unable to load bitmap: " << a_strFileName << endl;
#endif
		return UNDEFINED;
	}

	if (!a_bScaleToDef)
	{
		TextureObjTmpl txtTmpl(newId, pBmp, a_nDirNum, a_nStepNum);
		v_mapTextures2.insert(make_pair(newId, txtTmpl));

		return newId;
	}

	const int cellSizeX = (int)getCellSizeX();
	const int cellSizeY = (int)getCellSizeY();

    BITMAP* pScaledBmp = create_bitmap(cellSizeX, cellSizeY);

	ext_blit(pBmp, 
			 pScaledBmp,
			 0,
			 0, 
			 pBmp->w,
			 pBmp->h,
			 0,
			 0,
			 cellSizeX,
			 cellSizeY);

	destroy_bitmap(pBmp);

	TextureObjTmpl txtTmpl(newId, pScaledBmp, a_nDirNum, a_nStepNum);
	v_mapTextures2.insert(make_pair(newId, txtTmpl));	

	return newId;
}

bool BasicConfiguration::m_getTextureObjById2(int a_nTxtId, TextureObjTmpl& a_tmpl)
{
	TXTS_MAP::const_iterator itTT = v_mapTextures2.find(a_nTxtId);
	if (itTT == v_mapTextures2.end())
		return false;

	a_tmpl = (*itTT).second;

	return true;
}

bool BasicConfiguration::m_associateBasicTxts2()
{
	const string prefix = "/opt/slickedit/projects/RTSEngine/Debug/pictures/";
	//const string suffix = ".tga"; => known
	char buffer[50];
	int i;

    const uint numTxts = g_numTxtTmpls;
#ifdef __TRACE_ME
	cout << "numTxts = " << numTxts << endl;
#endif

	for (i = 0; i < numTxts; ++i)
	{
		TxtOT*& ot = G_TXT_TMPLS[i];
		bool bScaleToDef = false;  //(ot->nDirNum == 1 && ot->nStepNum == 1) ? true : false;
		cout << "Loading: " << ot->strFileName << endl;
		uint loadedTxtId = m_assocTxt2((prefix + ot->strFileName).c_str(),
									   ot->nTxtId,
									   ot->nDirNum,
									   ot->nStepNum,
									   bScaleToDef);
		cout << "End loading: " << ot->strFileName << endl;

		assert(loadedTxtId == ot->nTxtId || loadedTxtId == UNDEFINED);
	}

	return true;
}

bool BasicConfiguration::assertIntegrity()
{
	return (0 != v_pMap);
}

