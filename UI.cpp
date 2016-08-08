#include "common.h"

MouseEventManager  g_mouseEvManager = MouseEventManager();
MouseEventManager* g_pMEM = &g_mouseEvManager;


// RectangularMCUI
void RectangularUIElem::onClick()
{
	cout << endl << typeid(*this).name() << " : " << m_getId() << " clicked " << endl << endl << endl;
}

void RectangularUIElem::onMouseMove()
{
	cout << endl << typeid(*this).name() << " : " << m_getId() << " mouse moved on " << endl << endl << endl;  
}

bool RectangularUIElem::isPtInsideUIElem(const int a_x, const int a_y)
{
	return (BETWEEN(a_x, rect.x1(), rect.x2()) && 
			BETWEEN(a_y, rect.y1(), rect.y2()));
}

bool RectangularUIElem::isMCOverMe(const int a_mousex, const int a_mousey)
{
	return isPtInsideUIElem(a_mousex, a_mousey);
}

bool RectangularUIElem::isCrossing(const RECTANGLE& a_rect) const
{
    return (rect.crossing(a_rect));
}


// MouseEventManager
MouseEventManager::MouseEventManager()
{
	proc_REG_ONCLICK = (REG_ONCLICK)0;
}

void MouseEventManager::addMsg(const MouseMsg& a_msg)
{
	const int& flags = a_msg.flags;
	const int& mousex = a_msg.mousex;
	const int& mousey = a_msg.mousey;

	RegUIs* pUI = RegUIs::m_getRegUIs();
	assert((RegUIs*)0 != pUI);
	UIElem* pElem = pUI->getCrossingUIElem(mousex, mousey);
    if ((UIElem*)0 == pElem) // nie trafilismy w UI
	{
		if ((flags & MOUSE_FLAG_LEFT_DOWN) && (proc_REG_ONCLICK != (REG_ONCLICK)0))
			(*proc_REG_ONCLICK)(mousex, mousey);
		return;
	}

    MCUI* pMCUI = dynamic_cast<MCUI*>(pElem);
	if ((MCUI*)0 == pMCUI) 
		return;

    if (flags & MOUSE_FLAG_MOVE)
	{
		pMCUI->onMouseMove();
	}

	if (flags & MOUSE_FLAG_LEFT_DOWN)
	{
		pMCUI->onClick();
	}

	if (flags & MOUSE_FLAG_LEFT_UP)
	{
		pMCUI->onClickEnd();
	}

	messages.push_back(a_msg);
}


// Pane
bool Pane::addUIElem(UIElem* a_pUIEl, const ePlace& place)
{
	assert((UIElem*)0 != a_pUIEl);

	RECTANGLE& paneRECT = rect;
    RECTANGLE cpRECT = paneRECT.ext(border, border, -border, -border); // 'c'lip 'p'ane 'RECT'ect
	RECTANGLE newUIRECT = RECTANGLE(a_pUIEl->getLUCoords(), a_pUIEl->getWidth(), a_pUIEl->getHeight());
	RECTANGLE extUIRECT = newUIRECT.ext(-minElemDist, -minElemDist, minElemDist, minElemDist);

	if (a_pUIEl->isDocced())
	{
		if (!extUIRECT.inside(cpRECT))
			return false;

        int i;
		for (i = 0; i < vUIElems.size(); ++i )
		{
			const UIElem* pUIE = vUIElems[i];
            if (pUIE->isCrossing(extUIRECT))
				return false;
		}

		vUIElems.push_back(a_pUIEl);

		return true;
	}

	if (extUIRECT.getWidth() > cpRECT.getWidth() || extUIRECT.getHeight() > cpRECT.getHeight())
		return false;

	RECTANGLE bfRECT = cpRECT; // 'b'est 'f'it 'RECT'angle
    int i;
	for (i = 0; i < vUIElems.size(); ++i)
	{
		const UIElem* pUIE = vUIElems[i];
		RECTANGLE elRECT = RECTANGLE(pUIE->getLUCoords(), pUIE->getWidth(), pUIE->getHeight());
        RECTANGLE spRECTS[] = { // 's'plitted 'RECT'angles
			RECTANGLE(elRECT.x1(), bfRECT.y1(), elRECT.x2(), elRECT.y1()),
			RECTANGLE(bfRECT.x1(), bfRECT.y1(), elRECT.x1(), bfRECT.y2()),
			RECTANGLE(elRECT.x1(), elRECT.y2(), elRECT.x2(), bfRECT.y2()),
			RECTANGLE(elRECT.x2(), bfRECT.y1(), bfRECT.x2(), bfRECT.y2())
		};
		
		const uint nSPRs = (sizeof(spRECTS) / sizeof(spRECTS[0]));
		int max = 0, j;
		RECTANGLE* bestRECT = (RECTANGLE*)0;
        for (j = 0; j < nSPRs; ++j)
		{
			RECTANGLE& rect = spRECTS[j];
			int mul = (rect.getWidth() * rect.getHeight());
			if (mul > max)
			{
				max = mul;
				bestRECT = &spRECTS[j];
			}
		}

		assert((RECTANGLE*)0 != bestRECT);
		bfRECT = *bestRECT;
	}

#ifdef __TRACE_ME
	cout << "bfRECT: " << bfRECT << endl; 
#endif

	if (extUIRECT.getWidth() > bfRECT.getWidth() || extUIRECT.getHeight() > bfRECT.getHeight())
		return false; 

	a_pUIEl->setLUCoords(mp(bfRECT.x1(), bfRECT.y1()));
	a_pUIEl->setDocced(true);

    vUIElems.push_back(a_pUIEl);

	return true;
}

Pane::~Pane()
{
	int i;
	for (i = 0; i < vUIElems.size(); ++i)
	{
		UIElem* pUIE = vUIElems[i];
		assert((UIElem*)0 != pUIE);
		delete pUIE;
	}

	vUIElems.clear();
}

void Pane::m_draw(int a_nUserId, BITMAP* a_pDestBmp)
{
	bool skipToCol = false;
	if (txtId != (uint)UNDEFINED)
	{
		BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
		TextureObjTmpl txtTmpl;
		bool txtFound =	pConf->m_getTextureObjById2(txtId, txtTmpl);
		if (!txtFound)
		{
			skipToCol = true;
		}
		else
		{
			drawBmp(a_nUserId,
					a_pDestBmp,
					txtTmpl.pBmp,
					cpt3f((float)((rect.x1() + rect.x2()) >> 1), (float)((rect.y1() + rect.y2()) >> 1), 0.f), /* center coords */
					(float)(rect.getWidth()),
					(float)(rect.getHeight()),
					false,
					false);
		}
	}

	if (txtId == (uint)UNDEFINED || skipToCol)
	{
		rectfill(a_pDestBmp, rect.x1(), rect.y1(), rect.x2(), rect.y2(), col);
	}

	// draw outline border
	RECTANGLE& paneRECT = rect;
	RECTANGLE cpRECT = paneRECT.ext(border, border, -border, -border); // 'c'lip 'p'ane 'RECT'ect

	//rect(a_pDestBmp, cpRECT.x1(), cpRECT.y1(), cpRECT.x2(), cpRECT.y2(), borderCol);

	// END

    int i;
	for (i = 0; i < vUIElems.size(); ++i)
	{
		UIElem* pUIE = vUIElems[i];
		pUIE->m_draw(a_nUserId, a_pDestBmp);
	}
}

UIElem* Pane::getCrossingUIElem(const int x, const int y) const 
{
	for (int i = 0; i < vUIElems.size(); ++i)
	{
		UIElem* pElem = vUIElems[i];
		if (vUIElems[i]->isPtInsideUIElem(x, y))
			return pElem;
	}

	return (UIElem*)0;
}


// Button
void Button::m_draw(int a_nUserId, BITMAP* a_pDestBmp)
{
	uint txtId = (uint)UNDEFINED;
    switch(state)
	{
	case E_SIMPLE:
		txtId = ssTxtId;
		break;
	case E_CLICKED:
		txtId = ccTxtId;
		break;
	case E_MOUSE_MOVE:
		txtId = mmTxtId;
		break;
	default:
		break;
	}
	assert(txtId != (uint)UNDEFINED);

    BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	TextureObjTmpl txtTmpl;
	bool txtFound =	pConf->m_getTextureObjById2(txtId, txtTmpl);
	if (!txtFound)
		return;

	drawBmp(a_nUserId,
			a_pDestBmp,
			txtTmpl.pBmp,
			cpt3f((float)((rect.x1() + rect.x2()) >> 1), (float)((rect.y1() + rect.y2()) >> 1), 0.f), /* center coords */
			(float)(rect.getWidth()),
			(float)(rect.getHeight()),
			false,
			false);

	return;
}

void Button::forceREDRAW()
{
	scare_mouse();
	this->m_draw(DEFAULT_USER_ID, screen);
	unscare_mouse();
}

void Button::onClick()
{
	state = E_CLICKED;

	forceREDRAW();

    __onClick();
}

void Button::onClickEnd()
{
	state = E_SIMPLE;
}

void Button::onMouseMove()
{
	//state = E_MOUSE_MOVE;

	__onMouseMove();
}


// ButtonJoin
int 			ButtonJoin::numClicked = 0;
vector<TG*>		ButtonJoin::tgsToJoin;


void ButtonJoin::__onClick()
{
	__DRAW_TG = true;

	numClicked = 0;
	g_pMEM->register_REG_ONCLICK(&proc_REG_ONCLICK);
	tgsToJoin.clear();
}

void ButtonJoin::proc_REG_ONCLICK(const int x, const int y)
{
    numClicked++;

    TGManager* pTGM = TGManager::m_getTGManager();
	assert((TGManager*)0 != pTGM);

	GameView* pDV = getDefaultView();
	assert((GameView*)0 != pDV);
	RECTANGLE gvRECT = pDV->m_getClippingRectangle();
	int gvX1 = gvRECT.x1();
	int gvY1 = gvRECT.y1();

	if (numClicked <= 2)
	{
		uint assign = (uint)UNDEFINED;
		if (tgsToJoin.size() >= 1)
			assign = tgsToJoin[0]->m_getAssign();

		TG* tg = pTGM->m_getTGNearestPt(cpt3f((float)(x + gvX1), (float)(y + gvY1), 0.f), assign);
		if ((TG*)0 == tg)
		{
			g_pMEM->register_REG_ONCLICK((REG_ONCLICK)0);
			freezeUpdate = false;
			return;
		}
        else
		{
			tgsToJoin.push_back(tg);
		}

		freezeUpdate = true;
	}


	if (numClicked >= 2)
	{
        assert(tgsToJoin.size() == 2);
		TG* firstTG 	= tgsToJoin[0];
		TG* secondTG	= tgsToJoin[1];

		firstTG->m_join(secondTG);

		g_pMEM->register_REG_ONCLICK((REG_ONCLICK)0);	

		__DRAW_TG = false;	

		freezeUpdate = false;
	}

	return;
}


// ButtonSplit
int ButtonSplit::numClicked = 0;


void ButtonSplit::__onClick()
{
	__DRAW_TG = true;

	numClicked = 0;
	g_pMEM->register_REG_ONCLICK(&proc_REG_ONCLICK);
}

void ButtonSplit::proc_REG_ONCLICK(const int x, const int y)
{
    numClicked++;

    TGManager* pTGM = TGManager::m_getTGManager();
	assert((TGManager*)0 != pTGM);

	GameView* pDV = getDefaultView();
	assert((GameView*)0 != pDV);
	RECTANGLE gvRECT = pDV->m_getClippingRectangle();
	int gvX1 = gvRECT.x1();
	int gvY1 = gvRECT.y1();

	if (numClicked <= 1)
	{
		TG* tg = pTGM->m_getTGNearestPt(cpt3f((float)(x + gvX1), (float)(y + gvY1), 0.f));
		if ((TG*)0 == tg)
		{
			g_pMEM->register_REG_ONCLICK((REG_ONCLICK)0);
			return;
		}
        else
		{
			tg->m_split((uint)UNDEFINED, true);
			__DRAW_TG = false;
		}
	}

	return;
}


// ButtonGo
int ButtonGo::numClicked = 0;
TG* ButtonGo::tgToMove = (TG*)0;


void ButtonGo::__onClick()
{
	__DRAW_TG = true;

	numClicked = 0;
	g_pMEM->register_REG_ONCLICK(&proc_REG_ONCLICK);

	tgToMove = (TG*)0;
}

void ButtonGo::proc_REG_ONCLICK(const int x, const int y)
{
    numClicked++;

	TGManager* pTGM = TGManager::m_getTGManager();
	assert((TGManager*)0 != pTGM);

	GameView* pDV = getDefaultView();
	assert((GameView*)0 != pDV);
	RECTANGLE gvRECT = pDV->m_getClippingRectangle();
	int gvX1 = gvRECT.x1();
	int gvY1 = gvRECT.y1();

	if (numClicked == 1)
	{
		TG* tg = pTGM->m_getTGNearestPt(cpt3f((float)(x + gvX1), (float)(y + gvY1), 0.f));

        if ((TG*)0 == tg)
		{
			g_pMEM->register_REG_ONCLICK((REG_ONCLICK)0);
			return;
		}
		else
		{
			tgToMove = tg;
		}

		freezeUpdate = true;

		__DRAW_TG = false;
	}

	if (numClicked >= 2)
	{
		assert((TG*)0 != tgToMove);
		cpt3f destPt = cpt3f((float)(x + gvX1), (float)(y + gvY1), 0.f);

        tgToMove->m_setDestPt(destPt);

		g_pMEM->register_REG_ONCLICK((REG_ONCLICK)0);

		freezeUpdate = false;
	}

	return;
}


// RegUIs
RegUIs* RegUIs::v_pUIs = (RegUIs*)0;

RegUIs::~RegUIs()
{
	for (int i = 0; i < v_vPanes.size(); ++i)
	{
		Pane*& pP = v_vPanes[i]; 
		delete pP;
	}

	v_vPanes.clear();
}

bool RegUIs::isPtInsideUI(cpt3f& a_pt)
{
	return isPtInsideUI((int)(a_pt.x), (int)(a_pt.y));
}

bool RegUIs::isPtInsideUI(const int x, const int y)
{
    return (getCrossingPane(x, y) != (Pane*)0);
}

Pane* RegUIs::getCrossingPane(const int x, const int y) const
{
    for (int i = 0; i < v_vPanes.size(); ++i)
	{
		Pane* pP = v_vPanes[i];
		if (pP->isPtInsideUIElem(x, y))
			return pP;
	}

	return (Pane*)0;
}

UIElem* RegUIs::getCrossingUIElem(const int x, const int y) const
{
	Pane* pP = getCrossingPane(x, y); 
	if (pP != (Pane*)0)
		return pP->getCrossingUIElem(x, y);

	return (UIElem*)0;	
}

void RegUIs::m_draw(int a_nUserId, BITMAP* a_pDestBmp)
{
    for (int i = 0; i < v_vPanes.size(); ++i)
	{
		Pane*& pP = v_vPanes[i];
		pP->m_draw(a_nUserId, a_pDestBmp);
	}

	return;
}

bool RegUIs::addPane(Pane* a_pP)
{
	assert((Pane*)0 != a_pP);

	RECTANGLE apRECT = RECTANGLE(a_pP->getLUCoords(), a_pP->getWidth(), a_pP->getHeight());

	bool crossingOtherPane = false;
	for (int i = 0; i < v_vPanes.size(); ++i)
	{
		Pane*& pP = v_vPanes[i];

		if (pP->isCrossing(apRECT))
		{
			crossingOtherPane = true;
			break;	
		}
	}

	if (!crossingOtherPane)
	{
		v_vPanes.push_back(a_pP);
		return true;
	}

	return false;
}

