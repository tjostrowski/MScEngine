#include "common.h"

void testsInit();
void testTextureLoading();
void testFillingScreenWithTextures();
void testScreenMoving();
void testTimeScreenUpdating();
void testBasicObjectMapping();
void testHexagonalGrid();
void testMovement();
void testDynamicObstaclesAvoidance();
void testNewMovement(int a_nMipmapGridSize);
void testMapCreation();
void testDiffs();
void testFormationRotation();
void testGroupCreation();
void testBulletHandling();
void testBehaviors();
void testLM();
void testShooting();
void testGenOut();
void testGenIn();
int	 makeNewGroup(Group** a_ppGroup, BasicObject*** a_pppObjs, int a_nGroupSize, int a_nTextureId);
void prepareUI();

const int nBmpCount			= 6;
const int nNumCellX			= 64; 
const int nNumCellY			= 64;
const int nGridElemSizeX	= 32;
const int nGridElemSizeY	= 32;
BITMAP* bmpArray[10];
char* bmpNames[] = {
		"pictures/terrain_01.bmp",
		"pictures/walk_01.tga",
		"pictures/bbmerge_rev_one_next.pcx",	// 48 x 48
		"pictures/bbworkspace_new.pcx", 		// 48 x 48
		"pictures/eclipsesearchmark.bmp",		// 22 x 11    => 4th
		"pictures/mysha.pcx",					// 320 x 200 
		"pictures/planet.pcx",					// 49 x 49
		//"pictures/vs.gif",						// 16 x 16
		"pictures/vse64.pcx"					// 64 x 64 
};
static const int nDefTexts = sizeof(bmpNames) / sizeof(bmpNames[0]);
static int tblTextureIds[nDefTexts]; 
BITMAP* pBuffer = NULL;
static int s_nObjId			= 1;	
static int s_nGrpId			= 1;

const uint nUserId = 1;


class ReproducableClass
{
};





int main (int argc, char *argv[])
{
	// INIT Allegro
	allegro_init();

	cout << "Running on ALLEGRO version: "  
		 << ALLEGRO_VERSION      << "."
		 << ALLEGRO_SUB_VERSION  << "."
		 << ALLEGRO_WIP_VERSION
		 << endl;

	cout << "CPU vendor: " << cpu_vendor << endl;

	int nDesktopColDepth;
	int nDesktopWidth, nDesktopHeight;

	if (0 == (nDesktopColDepth = desktop_color_depth()))
	{
		cerr << "Unable to get desktop color" << endl;
		nDesktopColDepth = UNDEFINED;
	}

	if (0 != get_desktop_resolution(&nDesktopWidth, &nDesktopHeight))
	{
		cerr << "Unable to get desktop resolution" << endl;
		nDesktopWidth = nDesktopHeight = UNDEFINED;
	}

	// CPU capabilities
	if (cpu_capabilities & CPU_ID)
	{
		cout << "CPU_ID" << " ";
	}
	if (cpu_capabilities & CPU_FPU)
	{
		cout << "CPU_DPU" << " ";
	}
	/*if (cpu_capabilities & CPU_IA64)
	{
		cout << "CPU_IA64" << " ";
	}
	if (cpu_capabilities & CPU_AMD64)
	{
		cout << "CPU_AMD64" << " ";
	}*/
	if (cpu_capabilities & CPU_MMX)
	{
		cout << "CPU_MMX" << " ";
	}
	if (cpu_capabilities & CPU_MMXPLUS)
	{
		cout << "CPU_MMXPLUS" << " ";
	}
	if (cpu_capabilities & CPU_SSE)
	{
		cout << "CPU_SSE" << " ";
	}
	if (cpu_capabilities & CPU_SSE2)
	{
		cout << "CPU_SSE2" << " ";
	}
	/*if (cpu_capabilities & CPU_SSE3)
	{
		cout << "CPU_SSE3" << " ";
	}*/
	if (cpu_capabilities & CPU_3DNOW)
	{
		cout << "CPU_3DNOW" << " ";
	}
	if (cpu_capabilities & CPU_ENH3DNOW)
	{
		cout << "CPU_ENH3DNOW" << " ";
	}
	if (cpu_capabilities & CPU_CMOV)
	{
		cout << "CPU_CMOV" << " ";
	}

	cout << endl;

	// GRAPHICS handling
	if (UNDEFINED != nDesktopColDepth)
	{
		set_color_depth(nDesktopColDepth);
	}
	bool bSkipToSafe = false;
	if (UNDEFINED != nDesktopWidth && UNDEFINED != nDesktopHeight)
	{
		if	(0 != set_gfx_mode(GFX_AUTODETECT_WINDOWED, nDesktopWidth, nDesktopHeight, 0, 0))
		{
			bSkipToSafe = true;
		}
	}
	if (bSkipToSafe)
	{
		if (0 != set_gfx_mode(GFX_SAFE, 640, 480, 0, 0))
		{
			cerr << "Unable to initialize graphics" << endl;
			return 1;	
		}
	}

	// TIMER handling
	if (0 != install_timer())
	{
		cerr << "Unable to load timer" << endl;
		return 1;
	}

	// MOUSE handling
	if (-1 == install_mouse())
	{	
		cerr << "Unable to install mouse" << endl;
        return 1;
	}

	// enable_hardware_cursor();

	int nMousePooledMode = mouse_needs_poll();
	if (nMousePooledMode)
	{
		cout << "Mouse needs polling" << endl; 
	}

	set_mouse_sprite(NULL);

    if (nMousePooledMode)
	{
		poll_mouse();		
	}

	/*if (gfx_capabilities & GFX_HW_SUPPORT)
	{
		cout << "Mouse hardware cursor enabled" << endl;
	}*/

	// Little test mouse positions
	cout << "Test mouse position: " << mouse_x << " "
									<< mouse_y << " "
									<< mouse_z << endl;

	// KEYBOARD handling
	if (0 != install_keyboard())
	{
		cerr << "Unable to install keyboard" << endl;
		return 1;
	}

	int kbNeedsPoll = keyboard_needs_poll();
	if (TRUE == kbNeedsPoll)
	{
		cout << "Keyboard needs poll" << endl;
	}

	// Little test keyboard handling
	//show_mouse(screen);
	//int nKey = readkey();

	cout << "After keyboard test" << endl;

	// INITIALIZING Game parameters
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pMap = pConf->m_getGridMap();
	pMap->init(
			   SCREEN_W,		// Width 
 			   SCREEN_H,		// Height
			   nNumCellX,  		// Num Cell X,
			   nNumCellY,		// Num Cell Y 
			   nGridElemSizeX,	// Grid Elem Size X,
			   nGridElemSizeY,	// Grid Elem Size Y 
			   10);			// Num Layers
    pMap->m_generateGrid();
	// configure one view
	GameView* pView = pConf->m_createGameView(1);
	RECTANGLE view1Rect = make_rect(make_pair(0, 0), SCREEN_W, SCREEN_H);
	pView->init(nGridElemSizeX, nGridElemSizeY, view1Rect);	

	pConf->m_setColDepth(nDesktopColDepth);

	pConf->m_associateBasicTxts2();

	cout << "SUCCESS!!" << endl;

	cout << "Resolution is : "  
		 << SCREEN_W << " "
		 << SCREEN_H << endl;

	testsInit();
	/*if (posOutsideGrid(cpt3f(288.539f, 1998.95f, 0.f)))
		cout << ">>>>>>>>>> OUTSIDE" << endl;
	else
		cout << ">>>>>>>>>> INSIDE" << endl;*/

	testGenOut();
	testGenIn();
	//testShooting();
	//testTextureLoading();
	//testFormationRotation();

	//testTextureLoading();
	//testHexagonalGrid();
	//testBasicObjectMapping();
	//testsInit();
	//testTextureLoading();
	//testScreenMoving();
	//testTimeScreenUpdating();
	//testMovement();
	//testDynamicObstaclesAvoidance(); 
	//testNewMovement(32);
	//testNewMovement(16);
	//testMapCreation();
	//testGroupCreation();
	//testBulletHandling();
	//testBehaviors();

	cout << "TEST SUCCESS!!" << endl;

	return 0;
}
END_OF_MAIN()


volatile unsigned long nCounter = 0;

void inc_counter(void)
{
	nCounter++;
}
END_OF_FUNCTION(inc_counter)

extern MouseEventManager* g_pMEM;

void mouse_call(int flags)
{
	g_pMEM->addMsg(MouseMsg(flags, mouse_x, mouse_y));
}
END_OF_FUNCTION(mouse_call)


#define MAIN_LOOP_BEGIN \
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration(); \
	MapGrid* pGrid = pConf->m_getGridMap(); \
	GameView* pView = pConf->m_getGameViewByUserId(nUserId); \
								\
	LOCK_VARIABLE(nCounter); \
	LOCK_FUNCTION(inc_counter); \
	LOCK_FUNCTION(mouse_call); \
							\
	install_int(inc_counter, 10); /* every 10 miliseconds */ \
	mouse_callback = mouse_call; \
	ulong fn = (ulong)0; \
						 \
	set_mouse_sprite(NULL); \
	show_mouse(NULL);  \
						 \
	srand(time(NULL)); \
	bool mkUpdate = true; \
	bool mkRedraw = true; \
    do	\
	{ \
		if (mkRedraw && nCounter > 0) \
		{ \
			BasicConfiguration::setFPS((100.f * fn) / nCounter); \
			/* cout << "FPS: " << BasicConfiguration::getFPS() << endl << endl;*/ \
		} \
		if (mkRedraw) \
			clear_bitmap(pBuffer);
		
#define MAIN_LOOP_END \
		if (mkRedraw) \
		{ \
            masked_blit(cursorBmp, pBuffer, 0, 0, mouse_x, mouse_y, cursorBmp->w, cursorBmp->h); \
            blit(pBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H); \
		} \
							\
		if (keypressed()) \
		{ \
			int nKey = readkey(); \
			int nScancode = (nKey >> 8); \
            if (nScancode == KEY_ESC) \
				return; \
			int nDx = 0, nDy = 0; \
			int nIntervalX = 160; \
			int nIntervalY = 160; \
			if (nScancode == KEY_LEFT) \
				nDx -= nIntervalX; \
			if (nScancode == KEY_RIGHT) \
				nDx += nIntervalX; \
			if (nScancode == KEY_UP) \
				nDy -= nIntervalY; \
			if (nScancode == KEY_DOWN) \
				nDy += nIntervalY; \
			if (nScancode == KEY_SPACE) \
			{ \
                if (!mkUpdate) \
					mkRedraw = false; \
										\
				mkUpdate = false;	\
			} \
			if (nScancode == KEY_BACKSPACE) \
				mkUpdate = mkRedraw = true; \
									\
			pView->m_move(nDx, nDy); \
		} \
		++fn; \
		pConf->incFrameNo(); \
	} \
	while (1);

void testsInit()
{
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();
	srand(time(NULL));

	if (!pBuffer)
	{
		pBuffer = create_bitmap(SCREEN_W, SCREEN_H);
	}

	for (int i = 0; i < nDefTexts; ++i)
	{
		tblTextureIds[i] = pConf->m_associateTexture(bmpNames[i], false);
	}
}

void prepareUI()
{
	RegUIs* pUI = RegUIs::m_getRegUIs();
	assert((RegUIs*)0 != pUI);

	// Def. pane => left down side of the screen
	const uint p1W = 400;
	const uint p1H = 150;

    Pane* pP1 = new Pane(RECTANGLE(0, SCREEN_H - p1H, p1W, SCREEN_H),
						 G_UITLO_TXT_ID /* txt id */,
						 UNDEFINED /* col */,
						 0 /*border */,
						 5 /* minElemDist */);

    Button* pBJoin = new ButtonJoin(RECTANGLE(0, 0, 100, 100),
									false /* docced */,
									G_BUTTON_JOIN_TXT_ID,
									G_BUTTON_JOIN_CC_TXT_ID,
									G_BUTTON_JOIN_MM_TXT_ID);
	bool added = pP1->addUIElem(pBJoin);

	Button* pBSplit = new ButtonSplit(RECTANGLE(0, 0, 100, 100),
									  false /* docced */,
									  G_BUTTON_SPLIT_TXT_ID,
									  G_BUTTON_SPLIT_CC_TXT_ID,
									  G_BUTTON_SPLIT_MM_TXT_ID);	
	added = pP1->addUIElem(pBSplit);

	Button* pBGo = new ButtonGo(RECTANGLE(0, 0, 100, 100),
								false /* docced */,
								G_BUTTON_GO_TXT_ID,
								G_BUTTON_GO_CC_TXT_ID,
								G_BUTTON_GO_MM_TXT_ID);
    added = pP1->addUIElem(pBGo);

	added = pUI->addPane(pP1);  
}

void testGenOut()
{
	ConfigurationWriter writer("out_test.txt");

#define __TRACE_ME
	writer.m_genConfiguration();

	return;
}

void testGenIn()
{
	ConfigurationReader reader("out_test.txt");

#define __TRACE_ME
	
	vector<BasicObject*> vObjs;
	reader.m_readConfiguration(vObjs);

	// UI prep
    prepareUI(); 
	RegUIs* pUI = RegUIs::m_getRegUIs();
	assert((RegUIs*)0 != pUI);
	// END

	// BACKGROUND
	BITMAP* pBckBMP = load_bitmap("pictures/terrain_01.bmp", NULL);
	assert((BITMAP*)0 != pBckBMP);
	// END

	Layer* pDL = getDefaultLayer();
	assert((Layer*)0 != pDL );

	TGManager* pTM = TGManager::m_getTGManager();
	assert((TGManager*)0 != pTM);
    TacticalGroup* tg0 = pTM->m_createTG(0);
    const uint numETGs = 6;
	TG* etgs[numETGs];
	int i;
	for (i = 0; i < numETGs; ++i)
	{
		etgs[i] = pTM->m_createTG(1);
		etgs[i]->m_setFormShape(E_SHP_RECTANGLE);
	}

	uint gdgId = pDL->m_addGadget(new CannonGadget(getCellCenterCoords(mp(10, 10)), cvec3f(0.f, -1.f, 0.f), 500.f));
	CannonGadget* pGdg = dynamic_cast<CannonGadget*>(pDL->m_getGadgetWithId(gdgId));
	assert((CannonGadget*)0 != pGdg);
	pGdg->setDestDir(cvec3f(0.f, 1.f, 0.f));
	pGdg->setCanShoot(true);

	for (i = 0; i < vObjs.size(); ++i)
	{
		BasicObject*& pObj = vObjs[i];
		pDL->m_addObject(pObj);
		/*if (!pGdg->isFullyAssigned())
		{
			cfloat width 	= pObj->m_getWidth();
			cfloat height	= pObj->m_getHeight();
			cint txtID = pObj->m_getTxtID0();

			if (width == 1.f && height == 1.f &&
				(txtID == G_UNIT_TXT_ID || txtID == G_ENEMY_UNIT_TXT_ID))
			{
				pGdg->assignUnit(pObj);
				continue;
			}
		}*/

		if (!pObj->_isStatic())
		{
			if (pObj->m_getAssign() == 0)
				tg0->m_addObj(pObj);
			else if (pObj->m_getAssign() == 1)
				etgs[i % numETGs]->m_addObj(pObj);
		}
	}

	for (i = 0; i < numETGs; ++i)
	{
		const uint numObjs = etgs[i]->m_getNumObjs();
		if (numObjs == 0)
		{
			pTM->m_removeTG(etgs[i]->m_getId());
			etgs[i] = (TacticalGroup*)0;	
		}
	}

	TextureObjTmpl 	cursor1TxtTmpl,
					cursor2TxtTmpl;
	BITMAP*			cursorBmp = (BITMAP*)0;

	BasicConfiguration* pConfT = BasicConfiguration::m_getBasicConfiguration();
	assert((BasicConfiguration*)0 != pConfT);
	if (pConfT->m_getTextureObjById2(G_CURSOR1_TXT_ID, cursor1TxtTmpl))
		cursorBmp = cursor1TxtTmpl.pBmp;

    MAIN_LOOP_BEGIN
		// UPDATE section
		if (mkUpdate)
		{
			// SHOOTING
			UNITS_VEC& newObjs = pDL->m_getObjects();
			ulong frameNo = BasicConfiguration::m_getFrameNo();
			assert(newObjs.size() > 6);
			/*BasicObject* pTrgt = newObjs[5];
			cpt3f trgtPos = pTrgt->m_getPos();
			draw_circle(nUserId, pBuffer, trgtPos, 25.f, makecol(255, 255, 0), false);
			if (frameNo % 10 == 0)
			{
                for (i = 0; (i < newObjs.size()) && (i != 5); ++i)
				{
					BasicObject*& pObj = newObjs[i];
					assert(pObj != (BasicObject*)0);
					pObj->m_throwBullet(trgtPos, &G_DEF_BULLET_CLASS);
				}
			}*/
			// END

			if (!freezeUpdate)
			{
				pDL->m_update();
				pTM->m_update();
			}
		}

		// REDRAW section
		if (mkRedraw)
		{
			// BACKGROUND
			GameView* pDefVIEW = getDefaultView();
			assert((GameView*)0 != pDefVIEW);
			RECTANGLE gR = pDefVIEW->m_getGameRectangle();
			RECTANGLE cR = pDefVIEW->m_getClippingRectangle();
			cint bW = pBckBMP->w;
			cint bH = pBckBMP->h;
			cfloat scX = ((float)bW / gR.getWidth());
			cfloat scY = ((float)bH / gR.getHeight());

			stretch_blit(
				pBckBMP,
				pBuffer,
				(int)(scX * cR.x1()),
				(int)(scY * cR.y1()),
				(int)(scX * cR.getWidth()),
				(int)(scY * cR.getHeight()),
				0,
				0,
				SCREEN_W,
				SCREEN_H);

			// END

			pDL->m_draw(nUserId, pBuffer);
			pTM->m_draw(nUserId, pBuffer);
			pUI->m_draw(nUserId, pBuffer);
		}

	MAIN_LOOP_END

	return;
}

void testShooting()
{
	/*Layer* pDL = getDefaultLayer();
	assert((Layer*)0 != pDL );

	INT_COORDS cells[] = {
		mp(0, 0), mp(20, 20), mp(10, 15), mp(10, 10)};

	INT_COORDS enemyCells[] = { 	
		mp(1, 1), mp(12, 20), mp(14, 14), mp(15, 18), mp(16, 18)};
	
	const uint numObjs = (sizeof(cells) / sizeof(cells[0]));
	assert(numObjs > 0);
	uint objIDs[numObjs];

	TacticalGroup* tg	= new TacticalGroup(0);
	TacticalGroup* etg	= new TacticalGroup(1); 

	int i;
	for (i = 0; i < numObjs; ++i)
	{
		uint id = pDL->m_addObject(new BasicObject(getCellCenterCoords(cells[i])));
		objIDs[i] = id;
		tg->m_addObj(pDL->m_getObjWithId(id));
	}

	const uint numEnemyObjs = (sizeof(enemyCells) / sizeof(enemyCells[0]));
	assert(numEnemyObjs > 0);
	uint enemyObjIDs[numEnemyObjs];

    for (i = 0; i < numEnemyObjs; ++i)
	{
		uint id = pDL->m_addObject(new BasicObject(getCellCenterCoords(enemyCells[i]), G_ENEMY_UNIT_TXT_ID));
		enemyObjIDs[i] = id;
		etg->m_addObj(pDL->m_getObjWithId(id));
	}

	INT_COORDS tankCell = mp(13, 13);
	pDL->m_addObject(new BasicObject(getCellCenterCoords(tankCell), G_TANK_TXT_ID, 2.f, 2.f));

#define __TRACE_ME*/

	/*BasicObject* shooter = pDL->m_getObjWithId(objIDs[0]);
	assert((BasicObject*)0 != shooter);
	BasicObject* target = pDL->m_getObjWithId(objIDs[1]);
	assert((BasicObject*)0 != target);

	shooter->m_throwBullet(target->m_getPos());*/

#undef __TRACE_ME
    
	/*MAIN_LOOP_BEGIN
		// UPDATE section
		pDL->m_update();
		tg->m_update();
		etg->m_update();

		// DRAW section
		pDL->m_draw(nUserId, pBuffer);
		tg->m_draw(nUserId, pBuffer);
		etg->m_draw(nUserId, pBuffer);
	MAIN_LOOP_END*/	

	return;	
}

void testLM()
{
	const INT_COORDS startCell = make_pair(0, 0);
	const pt3f startPos = getCellCenterCoords(startCell);

	const INT_COORDS endCell = make_pair(15, 15);
	const pt3f endPos = getCellCenterCoords(endCell);

#ifdef __TRACE_ME
	cout << "Start pos: " << startPos << endl
		 << "End pos: " << endPos << endl
		 << endl << endl;
#endif

	/*LineMove* lm = new LineMove(startPos, 
								endPos,
								false, 
								10.f  speed );

#undef __TRACE_ME

	MAIN_LOOP_BEGIN
		// do loop
		lm->m_update();
		const pt3f newPos = lm->m_getPos();
		INT_COORDS cell = getCellForPos(newPos);
#ifdef __TRACE_ME
		cout << "New pos: " 	<< newPos << endl;
		cout << "Cell for pos:" << cell.first << " " << cell.second << endl << endl;
#endif
		// end do loop
	MAIN_LOOP_END*/

#define __TRACE_ME

	return;	
}

void testGadgets()
{

	//MAIN_LOOP_BEGIN
		// do loop


		// end do loop
	//MAIN_LOOP_END

	return;	
}

void testTextureLoading()
{
    for (int i = 0; i < nBmpCount; ++i)
	{
		const int nGridElemSizeX = 32;
		const int nGridElemSizeY = 32;
		bmpArray[i] = create_bitmap(nGridElemSizeX, nGridElemSizeY);
		BITMAP* pTmpBmp = load_bitmap(bmpNames[i], NULL);
		if (!pTmpBmp)
		{
			cerr << "Unable to load bitmap: " << bmpNames[i] << endl;
			return;
		}
		ext_blit(pTmpBmp, 
				 bmpArray[i],
				 0,
				 0, 
				 pTmpBmp->w,
				 pTmpBmp->h,
				 0,
				 0,
				 nGridElemSizeX,
				 nGridElemSizeY);
	}

	cout << "Textures successfully loaded" << endl;
}

void testFillingScreenWithTextures()
{
    const int nGridElemSizeX = 32;
	const int nGridElemSizeY = 32;
	const int nBackgroundTextNum = 4;
	const int nNumCellX = (SCREEN_W / nGridElemSizeX);
	const int nNumCellY = (SCREEN_H / nGridElemSizeY);

    // Initialize basic screen
	//srand(time(NULL));
	for (int i = 0; i < nNumCellX; ++i)
	{
		for (int j = 0; j < nNumCellY; ++j)
		{
			int nNum = rand() % nBmpCount;
			ext_blit(bmpArray[nBackgroundTextNum], 
					 pBuffer,
					 0,
					 0, 
					 bmpArray[nBackgroundTextNum]->w,
					 bmpArray[nBackgroundTextNum]->h,
					 i * nGridElemSizeX,
					 j * nGridElemSizeY,
					 nGridElemSizeX,
					 nGridElemSizeY);
		}
	}

	// Add 2 layers to this
	for (int i = 0; i < nNumCellX; ++i)
	{
		for (int j = 0; j < nNumCellY; ++j)
		{
			if ((i + j) % 2 == 0)
			{
				for (int k = 0; k < 2; ++k)
				{
					int nNum = rand() % nBmpCount;
					ext_masked_blit(bmpArray[nNum], 
									pBuffer,
									0,
									0, 
									bmpArray[nNum]->w,
									bmpArray[nNum]->h,
									i * nGridElemSizeX,
									j * nGridElemSizeY,
									nGridElemSizeX,
									nGridElemSizeY);
				}
			}           
		}
	}

	show_mouse(NULL);
	blit(pBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
	show_mouse(screen);
	clear_bitmap(pBuffer);
}

void testScreenMoving()
{
	do
	{
		int nKey = readkey();
		int nScancode = (nKey >> 8);
		if (nScancode == KEY_ESC)
		{
			return;
		}
		if (nScancode == KEY_LEFT || 
			nScancode == KEY_RIGHT ||
			nScancode == KEY_UP ||
			nScancode == KEY_DOWN)
		{
			testFillingScreenWithTextures();
		}
	}
	while (1);
}

void testTimeScreenUpdating()
{
	const int nBackgroundTextNum = 4;
	const int nGridElemSizeX = 32;
	const int nGridElemSizeY = 32;
	const int nNumCellX = (SCREEN_W / nGridElemSizeX);
	const int nNumCellY = (SCREEN_H / nGridElemSizeY);

	LOCK_VARIABLE(nCounter);
	LOCK_FUNCTION(inc_counter);

	install_int(inc_counter, 10); // every 10 miliseconds
	unsigned long nFrameNum = 0;

    cout << nNumCellX << " " << nNumCellY << endl;

	srand(time(NULL));
	do
	{
		if (nCounter > 0)
		{
			//textout_ex(screen, font, (SCREEN_W / 2), 8, makecol(0, 0, 0), 
			//					 "Frame rate: %d ", ((100 * nFrameNum) / nCounter));
			cout << "Frame rate: " << ((100 * nFrameNum) / nCounter) << endl; 
		}

		if (key[KEY_ESC])
		{
			return;
		}
		// background...
		for (int i = 0; i < nNumCellX; ++i)
		{
			for (int j = 0; j < nNumCellY; ++j)
			{
				ext_blit(bmpArray[nBackgroundTextNum], 
						 pBuffer,
						 0,
						 0, 
						 bmpArray[nBackgroundTextNum]->w,
						 bmpArray[nBackgroundTextNum]->h,
						 i * nGridElemSizeX,
						 j * nGridElemSizeY,
						 nGridElemSizeX,
						 nGridElemSizeY);
			}
		}
		// layers...
		//srand(time(NULL));
		for (int i = 0; i < nNumCellX; ++i)
		{
			for (int j = 0; j < nNumCellY; ++j)
			{
				if ((i + j) % 2 == 0)
				{
					for (int k = 0; k < 2; ++k)
					{
						int nNum = rand() % nBmpCount;
						ext_masked_blit(bmpArray[nNum], 
										pBuffer,
										0,
										0, 
										bmpArray[nNum]->w,
										bmpArray[nNum]->h,
										i * nGridElemSizeX,
										j * nGridElemSizeY,
										nGridElemSizeX,
										nGridElemSizeY);
					}
				}           
			}
		}

		show_mouse(NULL);
		blit(pBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
		show_mouse(screen);
		clear_bitmap(pBuffer);

		++nFrameNum;
	}
	while (1);
}

void testBasicObjectMapping()
{
	const int nUserId = 1;
	static int nId = 0;
	static int nTextureId = 0;
	BasicObject* pObj = new BasicObject(++nId);
	
	INT_COORDS tblVertices[] = {
        pair<int, int>(2,   1),
		pair<int, int>(0,   128),
		pair<int, int>(220, 222),
		pair<int, int>(120, 128), 
		pair<int, int>(220, 2)
	};
	vector<INT_COORDS> vVertices;
	vVertices.insert(vVertices.end(),
					 tblVertices, 
					 tblVertices + sizeof(tblVertices) / sizeof(tblVertices[0]));

	pObj->m_setVertices(vVertices);
	pObj->m_setTextParams(true, true);

	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();
	GameView* pView = pConf->m_getGameViewByUserId(nUserId);
	int nGridElemSizeX = pView->m_getGridElemSizeX();
	int nGridElemSizeY = pView->m_getGridElemSizeY();

	nTextureId = pConf->m_associateTexture(bmpNames[3]);
	TexturePattern texturePattern;
	TextureObject textureObj(32, 32, nTextureId);
	texturePattern.m_addTextureObj(textureObj);

	pObj->m_setTexturePattern(texturePattern);

    do
	{
        clear_bitmap(pBuffer);

		pObj->m_draw(nUserId, pBuffer);

		show_mouse(NULL);
		blit(pBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
		show_mouse(screen);
        
		int nKey = readkey();
		int nScancode = (nKey >> 8);
		if (nScancode == KEY_ESC)
		{
			return;
		}
		int nDx = 0, nDy = 0; 
		int nIntervalX = 160;
		int nIntervalY = 160;
		if (nScancode == KEY_LEFT)
		{
			nDx -= nIntervalX;
		}
		if (nScancode == KEY_RIGHT)
		{
			nDx += nIntervalX;
		}
        if (nScancode == KEY_UP)
		{
			nDy -= nIntervalY;
		}
		if (nScancode == KEY_DOWN)
		{
			nDy += nIntervalY;
		}

		pView->m_move(nDx, nDy);
	}
	while (1);
    
	delete pObj;
}

void testHexagonalGrid()
{
	int nUserId = 1;
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();
	GameView* pView = pConf->m_getGameViewByUserId(nUserId);
	if (0 == pView)
	{
		cerr << "Simulation failed, view with for user: " << nUserId << "not found" << endl;
		return;
	}

	LOCK_VARIABLE(nCounter);
	LOCK_FUNCTION(inc_counter);

	install_int(inc_counter, 10); // every 10 miliseconds
	unsigned long nFrameNum = 0;

	int nNumCellX = pGrid->m_getNumCellX();
	int nNumCellY = pGrid->m_getNumCellY();
	int nGridElemSizeX = pView->m_getGridElemSizeX();
	int nGridElemSizeY = pView->m_getGridElemSizeY();

	const int nDefaultTextNum = 3; // mysha

	do
	{
		clear_bitmap(pBuffer);

		// clip rectangle Y constraint
		// for X we will have difficulty

		// draw loop
		for (int i = 0; i < nNumCellX; ++i)
		{
			for (int j = 0; j < nNumCellY; ++j)
			{
				INT_COORDS gridCoords = make_pair(i, j);
				INT_COORDS ovCoords = pView->m_mapGridElementToOverallViewCoords(gridCoords);
				if (pView->m_isPointWithingClipRectangle(ovCoords))
				{
					// coords are 'center'
					INT_COORDS coords = pView->m_mapFromOverallToViewCoords(ovCoords);
					/*rect(pBuffer, 
						 coords.first - (nGridElemSizeX >> 2),
						 coords.second - (nGridElemSizeY >> 2),
						 coords.first + (nGridElemSizeX >> 2),
						 coords.second + (nGridElemSizeY >> 2),
						 makecol(255, 255, 255));*/
					GridElement* pElem = const_cast<GridElement*>(pGrid->m_getGridElement(gridCoords));
					BITMAP* pBmp = pElem->m_getBitmap();
                    ext_blit(
						 pBmp, 
						 pBuffer,
						 0,
						 0, 	
						 pBmp->w,
						 pBmp->h,
						 coords.first - (nGridElemSizeX >> 1),
						 coords.second - (nGridElemSizeY >> 2),
						 nGridElemSizeX,
						 (nGridElemSizeY >> 2) * 3);					
				}
			}
		}

		show_mouse(NULL);
		blit(pBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
		show_mouse(screen);
        
		int nKey = readkey();
		int nScancode = (nKey >> 8);
		if (nScancode == KEY_ESC)
		{
			return;
		}
		int nDx = 0, nDy = 0; 
		int nIntervalX = 160;
		int nIntervalY = 160;
		if (nScancode == KEY_LEFT)
		{
			nDx -= nIntervalX;
		}
		if (nScancode == KEY_RIGHT)
		{
			nDx += nIntervalX;
		}
        if (nScancode == KEY_UP)
		{
			nDy -= nIntervalY;
		}
		if (nScancode == KEY_DOWN)
		{
			nDy += nIntervalY;
		}

		pView->m_move(nDx, nDy);

		++nFrameNum;
	}
	while (1);    
}

void testMovement()
{
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();
	const int nUserId = 1;
	GameView* pView = pConf->m_getGameViewByUserId(nUserId);
	if (0 == pView)
	{
		cerr << "Simulation failed, view with for user: " << nUserId << "not found" << endl;
		return;
	}

	int nBackTextId	= pConf->m_associateTexture(bmpNames[0], false);	
    pGrid->m_createBackgroundLayer(nBackTextId);

	BasicObject* pObj = new BasicObject(1);
	INT_COORDS tblVertices[] = {
		pair<int, int>(1,   1)
	};
	vector<INT_COORDS> vVertices;
	vVertices.insert(vVertices.end(),
					 tblVertices, 
					 tblVertices + sizeof(tblVertices) / sizeof(tblVertices[0]));

	pObj->m_setVertices(vVertices);
	pObj->m_setDynamic(true);

	int nTextureId = pConf->m_associateTexture(bmpNames[1], false);
	TexturePattern texturePattern;
	TextureObject textureObj(1, 1, nTextureId, true);
	textureObj.m_setAnimatedParams(8, 10);
	texturePattern.m_addTextureObj(textureObj);
	pObj->m_setTextParams(true, true);
	pObj->m_setTexturePattern(texturePattern);

	vector<PATHELEM>	vPathElems; 
	set<INT_COORDS>		setCoveredGridElems;
	setCoveredGridElems.insert(make_pair(2, 1));
	time_t startTime = time(NULL);
	MovementManager::m_computePath(pObj, make_pair(10, 10),
								   setCoveredGridElems,
								   vPathElems);
	time_t elapsedTime = time(NULL) - startTime;
	cout << "Second(s) elapsed: " << elapsedTime << endl;

	pObj->m_setActualPath(vPathElems);

    do
	{
		clear_bitmap(pBuffer);

		// clip rectangle Y constraint
		// for X we will have difficulty

		// draw loop
		pGrid->m_draw(nUserId, pBuffer);
		pObj->m_draw(nUserId, pBuffer);

		show_mouse(NULL);
		blit(pBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
		show_mouse(screen);
        
		int nKey = readkey();
		int nScancode = (nKey >> 8);
		if (nScancode == KEY_ESC)
		{
			return;
		}
		int nDx = 0, nDy = 0; 
		int nIntervalX = 160;
		int nIntervalY = 160;
		if (nScancode == KEY_LEFT)
		{
			nDx -= nIntervalX;
		}
		else if (nScancode == KEY_RIGHT)
		{
			nDx += nIntervalX;
		}
        else if (nScancode == KEY_UP)
		{
			nDy -= nIntervalY;
		}
		else if (nScancode == KEY_DOWN)
		{
			nDy += nIntervalY;
		}
		else if (nScancode == KEY_PLUS_PAD)
		{
			pObj->m_nextStep();
		}

		pView->m_move(nDx, nDy);

		//++nFrameNum;
	}
	while (1);

	return;
}

void testDynamicObstaclesAvoidance()
{
	/*const int nUserId = 1;
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();
	GameView* pView = pConf->m_getGameViewByUserId(nUserId);
	if (0 == pView)
	{
		cerr << "Simulation failed, view with for user: " << nUserId << "not found" << endl;
		return;
	}

	BasicObject* pObj1 = new BasicObject(1);
	BasicObject* pObj2 = new BasicObject(2);
	BasicObject* pObj3 = new BasicObject(3);

	INT_COORDS tblVertices1[] = {
		pair<int, int>(1, 1)
	};
	INT_COORDS tblVertices2[] = {
		pair<int, int>(0, 2)
	};
	INT_COORDS tblVertices3[] = {
		pair<int, int>(2, 0)
	};
	vector<INT_COORDS> vVertices1;
	vVertices1.insert(vVertices1.end(),
					  tblVertices1, 
					  tblVertices1 + sizeof(tblVertices1) / sizeof(tblVertices1[0]));
	vector<INT_COORDS> vVertices2;
	vVertices2.insert(vVertices2.end(),
					  tblVertices2, 
					  tblVertices2 + sizeof(tblVertices2) / sizeof(tblVertices2[0]));
	vector<INT_COORDS> vVertices3;
	vVertices3.insert(vVertices3.end(),
					  tblVertices3, 
					  tblVertices3 + sizeof(tblVertices3) / sizeof(tblVertices3[0]));
	pObj1->m_setVertices(vVertices1);
	pObj1->m_setDynamic(true);
	pObj2->m_setVertices(vVertices2);
	pObj2->m_setDynamic(true);
    pObj3->m_setVertices(vVertices3);
	pObj3->m_setDynamic(true);

	int nTextureId = pConf->m_associateTexture(bmpNames[1], false);
	TexturePattern texturePattern;
	TextureObject textureObj(1, 1, nTextureId, true);
	textureObj.m_setAnimatedParams(8, 10);
	texturePattern.m_addTextureObj(textureObj);

	pObj1->m_setTextParams(true, true);
	pObj1->m_setTexturePattern(texturePattern);
	pObj2->m_setTextParams(true, true);
	pObj2->m_setTexturePattern(texturePattern);
	pObj3->m_setTextParams(true, true);
	pObj3->m_setTexturePattern(texturePattern);

	vector<PATHELEM>	vPathElems; 
	set<INT_COORDS>		setCoveredGridElems;
	setCoveredGridElems.insert(make_pair(2, 1));
	time_t startTime = time(NULL);
	MovementManager::m_computePath(pObj1, make_pair(5, 12),
								   setCoveredGridElems,
								   vPathElems);
	time_t elapsedTime = time(NULL) - startTime;
	cout << "Second(s) elapsed: " << elapsedTime << endl;
	pObj1->m_replacePath(UNDEFINED, UNDEFINED, vPathElems);

	startTime = time(NULL);
	MovementManager::m_computePath(pObj2, make_pair(2, 2),
								   setCoveredGridElems,
								   vPathElems);
	elapsedTime = time(NULL) - startTime;
	cout << "Second(s) elapsed: " << elapsedTime << endl;
	pObj2->m_replacePath(UNDEFINED, UNDEFINED, vPathElems);

	startTime = time(NULL);
	MovementManager::m_computePath(pObj3, make_pair(2, 5),
								   setCoveredGridElems,
								   vPathElems);
	elapsedTime = time(NULL) - startTime;
	cout << "Second(s) elapsed: " << elapsedTime << endl;
	pObj3->m_replacePath(UNDEFINED, UNDEFINED, vPathElems);

    INT_COORDS gridCoords1 = pObj1->m_getPosition();
	INT_COORDS gridCoords2 = pObj2->m_getPosition();
	INT_COORDS gridCoords3 = pObj3->m_getPosition();
	GridElement* pElem1 = const_cast<GridElement*>(pGrid->m_getGridElement(gridCoords1));
	GridElement* pElem2 = const_cast<GridElement*>(pGrid->m_getGridElement(gridCoords2));
	GridElement* pElem3 = const_cast<GridElement*>(pGrid->m_getGridElement(gridCoords3));
	pElem1->m_setCovered(pObj1, true);
	pElem2->m_setCovered(pObj2, true);
	pElem3->m_setCovered(pObj2, true);
	MovementManager::m_resolveDynamicMovementConflicts(pObj1);
	MovementManager::m_resolveDynamicMovementConflicts(pObj2);
	MovementManager::m_resolveDynamicMovementConflicts(pObj3);

	do
	{
		clear_bitmap(pBuffer);

		// draw loop
		pGrid->m_draw(nUserId, pBuffer);
		pObj1->m_draw(nUserId, pBuffer);
		pObj2->m_draw(nUserId, pBuffer);
		pObj3->m_draw(nUserId, pBuffer);

		show_mouse(NULL);
		blit(pBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
		show_mouse(screen);
        
		int nKey = readkey();
		int nScancode = (nKey >> 8);
		if (nScancode == KEY_ESC)
		{
			return;
		}
		int nDx = 0, nDy = 0; 
		int nIntervalX = 160;
		int nIntervalY = 160;
		if (nScancode == KEY_LEFT)
		{
			nDx -= nIntervalX;
		}
		if (nScancode == KEY_RIGHT)
		{
			nDx += nIntervalX;
		}
        if (nScancode == KEY_UP)
		{
			nDy -= nIntervalY;
		}
		if (nScancode == KEY_DOWN)
		{
			nDy += nIntervalY;
		}
		if (nScancode == KEY_PLUS_PAD)
		{
            pElem1->m_setCovered(0);
			pElem2->m_setCovered(0);
			pElem3->m_setCovered(0);
			pObj1->m_nextStep();
			pObj2->m_nextStep();
			pObj3->m_nextStep();
			gridCoords1 = pObj1->m_getPosition();
			gridCoords2 = pObj2->m_getPosition();
			gridCoords3 = pObj3->m_getPosition();
			pElem1 = const_cast<GridElement*>(pGrid->m_getGridElement(gridCoords1));
			pElem2 = const_cast<GridElement*>(pGrid->m_getGridElement(gridCoords2));
			pElem3 = const_cast<GridElement*>(pGrid->m_getGridElement(gridCoords3));
			pElem1->m_setCovered(pObj1);
			pElem2->m_setCovered(pObj2);
			pElem3->m_setCovered(pObj3);
			MovementManager::m_resolveDynamicMovementConflicts(pObj1);
			MovementManager::m_resolveDynamicMovementConflicts(pObj2);
			MovementManager::m_resolveDynamicMovementConflicts(pObj3);
		}

		pView->m_move(nDx, nDy);
	}
	while (1);*/

	return;
}

void testNewMovement(int a_nMipmapGridSize)
{
	const int nUserId = 1;
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();
	GameView* pView = pConf->m_getGameViewByUserId(nUserId);
	if (0 == pView)
	{
		cerr << "Simulation failed, view with for user: " << nUserId << "not found" << endl;
		return;
	}
	MovementManager::m_setMipmapGridSize(a_nMipmapGridSize);

	BasicObject* pObj1 = new BasicObject(1);

	INT_COORDS tblVertices1[] = {
		pair<int, int>(1, 1)
	};
	vector<INT_COORDS> vVertices1;
	vVertices1.insert(vVertices1.end(),
					  tblVertices1, 
					  tblVertices1 + sizeof(tblVertices1) / sizeof(tblVertices1[0]));
	pObj1->m_setVertices(vVertices1);
	pObj1->m_setDynamic(true);

	int nTextureId = pConf->m_associateTexture(bmpNames[1], false);
	TexturePattern texturePattern;
	TextureObject textureObj(1, 1, nTextureId, true);
	textureObj.m_setAnimatedParams(8, 10);
	texturePattern.m_addTextureObj(textureObj);

	pObj1->m_setTextParams(true, true);
	pObj1->m_setTexturePattern(texturePattern);

	vector<PATHELEM>	vPathElems; 
	set<INT_COORDS>		setCoveredGridElems;
	setCoveredGridElems.insert(make_pair(2, 1));
	time_t startTime = time(NULL);
	MovementManager::m_computePath(pObj1, make_pair(62, 62),
								   setCoveredGridElems,
								   vPathElems);
	time_t elapsedTime = time(NULL) - startTime;
	cout << "Second(s) elapsed: " << elapsedTime << endl;

	pObj1->m_setActualPath(vPathElems);

    do
	{
		clear_bitmap(pBuffer);

		// clip rectangle Y constraint
		// for X we will have difficulty

		// draw loop
		pGrid->m_draw(nUserId, pBuffer);
		pObj1->m_draw(nUserId, pBuffer);

		show_mouse(NULL);
		blit(pBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
		show_mouse(screen);
        
		int nKey = readkey();
		int nScancode = (nKey >> 8);
		if (nScancode == KEY_ESC)
		{
			return;
		}
		int nDx = 0, nDy = 0; 
		int nIntervalX = 160;
		int nIntervalY = 160;
		if (nScancode == KEY_LEFT)
		{
			nDx -= nIntervalX;
		}
		if (nScancode == KEY_RIGHT)
		{
			nDx += nIntervalX;
		}
        if (nScancode == KEY_UP)
		{
			nDy -= nIntervalY;
		}
		if (nScancode == KEY_DOWN)
		{
			nDy += nIntervalY;
		}
		if (nScancode == KEY_PLUS_PAD)
		{
			pObj1->m_nextStep();
		}

		pView->m_move(nDx, nDy);

		//++nFrameNum;
	}
	while (1);

	return;
}

void testMapCreation()
{
	// create layers
	const int nUserId		= 1;
    
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();
	GameView* pView = pConf->m_getGameViewByUserId(nUserId);

	int nBackTextId	= pConf->m_associateTexture(bmpNames[0], false);
	
    pGrid->m_createBackgroundLayer(nBackTextId);
	
	do
	{
		clear_bitmap(pBuffer);

		pGrid->m_draw(nUserId, pBuffer);

		show_mouse(NULL);
		blit(pBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
		show_mouse(screen);

		int nKey = readkey();
		int nScancode = (nKey >> 8);
		if (nScancode == KEY_ESC)
		{
			return;
		}
		int nDx = 0, nDy = 0; 
		int nIntervalX = 160;
		int nIntervalY = 160;
		if (nScancode == KEY_LEFT)
		{
			nDx -= nIntervalX;
		}
		if (nScancode == KEY_RIGHT)
		{
			nDx += nIntervalX;
		}
		if (nScancode == KEY_UP)
		{
			nDy -= nIntervalY;
		}
		if (nScancode == KEY_DOWN)
		{
			nDy += nIntervalY;
		}

        pView->m_move(nDx, nDy);
	}
	while (1);
	
	return;
}

void testDiffs()
{
	const int nUserId		= 1;
    
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();
	GameView* pView = pConf->m_getGameViewByUserId(nUserId);

	int nGridElemSizeX = pView->m_getGridElemSizeX();
	int nGridElemSizeY = pView->m_getGridElemSizeY();

	const int nBmpIndex = 3;

	int nDiffRes = 1;

	do
	{
		clear_bitmap(pBuffer);

		vector<INT_COORDS> vDiffs;
		MovementManager::m_getDiffs(nDiffRes, vDiffs);
		vDiffs.push_back(make_pair(0, 0));

		INT_COORDS basic = make_pair(20, 20);

		for (int i = 0; i < vDiffs.size(); ++i)
		{
			INT_COORDS gridCoords = make_pair(
				basic.first + vDiffs[i].first, basic.second + vDiffs[i].second);
			if (pGrid->m_validCoords(gridCoords))
			{
				INT_COORDS ovCoords = pView->m_mapGridElementToOverallViewCoords(
					gridCoords);
				if (pView->m_isPointWithingClipRectangle(ovCoords))
				{
					// coords are 'center'
					INT_COORDS coords = pView->m_mapFromOverallToViewCoords(ovCoords);
                    BITMAP* pBmp = bmpArray[nBmpIndex];
                    ext_blit(
						 pBmp, 
						 pBuffer,
						 0,
						 0, 	
						 pBmp->w,
						 pBmp->h,
						 coords.first - (nGridElemSizeX >> 1),
						 coords.second - (nGridElemSizeY >> 2),
						 nGridElemSizeX,
						 (nGridElemSizeY >> 2) * 3);					
				}
			}
		}

		show_mouse(NULL);
		blit(pBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
		show_mouse(screen);

		int nKey = readkey();
		int nScancode = (nKey >> 8);
		if (nScancode == KEY_ESC)
		{
			return;
		}
		int nDx = 0, nDy = 0; 
		int nIntervalX = 160;
		int nIntervalY = 160;
		if (nScancode == KEY_LEFT)
		{
			nDx -= nIntervalX;
		}
		if (nScancode == KEY_RIGHT)
		{
			nDx += nIntervalX;
		}
		if (nScancode == KEY_UP)
		{
			nDy -= nIntervalY;
		}
		if (nScancode == KEY_DOWN)
		{
			nDy += nIntervalY;
		}
		if (nScancode == KEY_PLUS_PAD)
		{
			nDiffRes++;
		}
		if (nScancode == KEY_MINUS_PAD)
		{
			nDiffRes--;
		}

        pView->m_move(nDx, nDy);
	}
	while (1);
	
	return;
}

void testFormationRotation()
{
	const int nUserId = 1;
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();
	GameView* pView = pConf->m_getGameViewByUserId(nUserId);
	const int nBmpIndex = 3;

	vector<int> formationRows;

	for (int i = 15; i >= 1; i-=2)
	{
		formationRows.push_back(i);
	}

	Formation newForm;
	newForm.m_setFormation(formationRows);
	int nAngle = 1;
	newForm.m_setAngle(nAngle);
	newForm.m_setPos(make_pair(50, 50));
    
	vector<INT_COORDS> vNewPoss;
	newForm.m_getGRPoses(vNewPoss);

	do
	{
		clear_bitmap(pBuffer);

		for (int i = 0; i < vNewPoss.size(); ++i)
		{
			INT_COORDS gridCoords = vNewPoss[i];
			if (pGrid->m_validCoords(gridCoords))
			{
				INT_COORDS ovCoords = pView->m_mapGridElementToOverallViewCoords(
					gridCoords);
				if (pView->m_isPointWithingClipRectangle(ovCoords))
				{
					// coords are 'center'
					INT_COORDS coords = pView->m_mapFromOverallToViewCoords(ovCoords);
                    BITMAP* pBmp = bmpArray[nBmpIndex];
                    ext_blit(
						 pBmp, 
						 pBuffer,
						 0,
						 0, 	
						 pBmp->w,
						 pBmp->h,
						 coords.first - (nGridElemSizeX >> 1),
						 coords.second - (nGridElemSizeY >> 2),
						 nGridElemSizeX,
						 (nGridElemSizeY >> 2) * 3);					
				}
			}
		}

		show_mouse(NULL);
		blit(pBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
		show_mouse(screen);

		int nKey = readkey();
		int nScancode = (nKey >> 8);
		if (nScancode == KEY_ESC)
		{
			return;
		}
		int nDx = 0, nDy = 0; 
		int nIntervalX = 160;
		int nIntervalY = 160;
		if (nScancode == KEY_LEFT)
		{
			nDx -= nIntervalX;
		}
		if (nScancode == KEY_RIGHT)
		{
			nDx += nIntervalX;
		}
		if (nScancode == KEY_UP)
		{
			nDy -= nIntervalY;
		}
		if (nScancode == KEY_DOWN)
		{
			nDy += nIntervalY;
		}
		if (nScancode == KEY_PLUS_PAD)
		{
			nAngle++;
			newForm.m_setAngle(nAngle);
			newForm.m_getGRPoses(vNewPoss);
		}
		if (nScancode == KEY_MINUS_PAD)
		{
			nAngle--;
			newForm.m_setAngle(nAngle);
			newForm.m_getGRPoses(vNewPoss);
		}

			/*for (int i = 0; i < vNewPoss.size(); ++i)
			{
				cout << i << ": " << vNewPoss[i].first << "\t" << vNewPoss[i].second << endl;
			}
			cout << endl << endl;*/


        pView->m_move(nDx, nDy);
	}
	while (1);
}

void testGroupCreation()
{
	// tests when group will be created
	// from randomly situated objects on the map
	const int		nUserId			= 1; 
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();
	GameView* pView = pConf->m_getGameViewByUserId(nUserId);
	Group* 			pGroup			= 0;
	BasicObject**	ppObjs			= 0;
	int 	  		nGroupSize		= 10;
	const int 		nDeltaUnits		= 5; 
	const int		nTextureIndex 	= 3;	

	srand(time(NULL));

	int nTextureId = tblTextureIds[1];

	int nRes = makeNewGroup(&pGroup, &ppObjs, nGroupSize, nTextureId);
	if (0 != nRes)
	{
		cout << "ERROR: " << "Creating new group" << endl;
		return;
	}
	pGroup->m_setActualTarget(make_pair(32, 32));

	do
	{
		clear_bitmap(pBuffer);

		if (0 != ppObjs)
		{
			for (int i = 0; i < nGroupSize; ++i)
			{
				BasicObject* pObj = ppObjs[i];
				if (0 != pObj)
				{
                    pObj->m_draw(nUserId, pBuffer);
				}
			}
		}

		show_mouse(NULL);
		blit(pBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
		show_mouse(screen);

		int nKey = readkey();
		int nScancode = (nKey >> 8);
		if (nScancode == KEY_ESC)
		{
			return;
		}
		if (nScancode == KEY_PLUS_PAD)
		{
			pGroup->m_nextStep();
		}
		if (nScancode == KEY_N)
		{
			for (int i = 0; i < nGroupSize; ++i)
			{
				delete ppObjs[i];
				ppObjs[i] = 0;
			}
			delete [] ppObjs;
			ppObjs = 0;
			delete pGroup;
			pGroup = 0;

			nGroupSize += nDeltaUnits;

			nRes = makeNewGroup(&pGroup, &ppObjs, nGroupSize, nTextureId);
			if (0 != nRes)
			{
				cout << "ERROR: " << "Creating new group" << endl;
				return;
			}
			pGroup->m_setActualTarget(make_pair(32, 32));
		}
		int nDx = 0, nDy = 0; 
		int nIntervalX = 160;
		int nIntervalY = 160;
		if (nScancode == KEY_LEFT)
		{
			nDx -= nIntervalX;
		}
		if (nScancode == KEY_RIGHT)
		{
			nDx += nIntervalX;
		}
		if (nScancode == KEY_UP)
		{
			nDy -= nIntervalY;
		}
		if (nScancode == KEY_DOWN)
		{
			nDy += nIntervalY;
		}

		pView->m_move(nDx, nDy);
	}
	while (1);
}

int makeNewGroup(Group** a_ppGroup, BasicObject*** a_pppObjs, int a_nGroupSize, int a_nTextureId)
{
	if (a_nGroupSize <= 0)
	{
		return ERR_RESULT;
	}

	TexturePattern texturePattern;
	TextureObject textureObj(1, 1, a_nTextureId, true);
	textureObj.m_setAnimatedParams(8, 10);
	texturePattern.m_addTextureObj(textureObj);
    
	*a_ppGroup = new Group(s_nGrpId++);
	*a_pppObjs = new BasicObject*[a_nGroupSize];
	for (int i = 0; i < a_nGroupSize; ++i)
	{
		BasicObject* pObj = new BasicObject(s_nObjId++);
		int nX = rand() % nNumCellX;
		int nY = rand() % nNumCellY;
		vector<INT_COORDS> vVertices;
		vVertices.push_back(make_pair(nX, nY));
		pObj->m_setVertices(vVertices);
		pObj->m_setDynamic(true);

		pObj->m_setTextParams(true, true);
		pObj->m_setTexturePattern(texturePattern);

		(*a_pppObjs)[i] = pObj;

        (*a_ppGroup)->m_addObject((*a_pppObjs)[i]);
	}

	return 0;
}

void testBulletHandling()
{	
}

void testBehaviors()
{	
}

