#include "common.h"

string G_CATAPULT	= "catapult";
string G_CANNON		= "cannon";
string G_POWERUP 	= "powerup";


// Gadget
string  Gadget::strClass;
int 	Gadget::numReqOpUnits;
float	Gadget::width;
float 	Gadget::height;
uint	Gadget::gTxtID;


GTmplParams tblTmplParams[] =
{
    {G_CANNON, 2 /* numReqUnits */, 3.5f /* width */, 2.5f /* height */, G_CANNON_TXT_ID}
};

bool Gadget::sinit()
{
	const int nTmplP = (sizeof(tblTmplParams) / sizeof(tblTmplParams[0]));
	GTmplParams* fndP = (GTmplParams*)0; 
    for (int i = 0; i < nTmplP; ++i)
	{
		GTmplParams& par = tblTmplParams[i];
		if (par.strClass == strClass)
		{
			fndP = &par;
			break;
		}
	}

	if ((GTmplParams*)0 == fndP)
	{
#ifdef __TRACE_ME
		cerr << "" << endl;
#endif
        return false;
	}

	numReqOpUnits = fndP->reqUnitsNum;
	width		  = fndP->width;	
	height		  = fndP->height;	
	gTxtID		  = fndP->gTxtID;	

	return true;
}

Gadget::Gadget(cpt3f& a_pos) :
	v_pos(a_pos), v_tbDestroyed(false)
{
    if (!makeTextureObj(gTxtID, v_gTxtObj))
	{
#ifdef __TRACE_ME
		cerr << "" << endl;
#endif
		cout << ">>>>>>>>>>>>> ERROR <<<<<<<<<<<<<<<<< : " << gTxtID << endl;
		v_tbDestroyed = true;
		return;
	}
}

int Gadget::getOpUnitsNum()
{
	int opUnitsNum = 0;

	for (int i = 0; i < v_vAssUnits.size(); ++i)
	{
		BasicObject*& pObj = v_vAssUnits[i];
		if (!objFarOutsideGadget(pObj))
			opUnitsNum++;
	}

	return opUnitsNum;
}

void Gadget::assignUnit(BasicObject* a_pObj)
{
	if (v_vAssUnits.size() >= numReqOpUnits)
		return;

	uint prevASSIGN = (cuint)UNDEFINED;
	if (v_vAssUnits.size() > 0)
	{
		BasicObject* pObj = v_vAssUnits[0];
		prevASSIGN = pObj->m_getAssign();
	}

	if (prevASSIGN != (cuint)UNDEFINED && prevASSIGN != a_pObj->m_getAssign())
	{
#ifdef __TRACE_ME
		cout << "" << endl;
#endif
		return;
	}

    v_vAssUnits.push_back(a_pObj);

	cpt3f gdgPos = m_getPos();

	Flock* pFl = a_pObj->m_getFlock();
	pFl->m_setTargetPt(gdgPos);

    a_pObj->m_reconfigureToBehavior(
		new SimpleBehavior(a_pObj, a_pObj->m_getTxtID0(), this));
}

void Gadget::deassignUnit(BasicObject* a_pObj)
{
	UNITS_VEC::iterator it;

	if (0 == getOpUnitsNum() || 
		v_vAssUnits.end() == (it = find(v_vAssUnits.begin(), v_vAssUnits.end(), a_pObj)))
		return;
    
	v_vAssUnits.erase(it);

	a_pObj->m_reconfigureToBehavior((Behavior*)0);
}

bool Gadget::objFarOutsideGadget(BasicObject* a_pObj)
{
	cfloat cellSizeX = getCellSizeX();
	cfloat cellSizeY = getCellSizeY();

	cpt3f objPos		= a_pObj->m_getPos();
	cfloat objWidth 	= a_pObj->m_getWidth();
	cfloat objHeight	= a_pObj->m_getHeight();

	if (dist(v_pos, objPos) > 
		.75f * cellSizeX * ((sqrtf(width * width + height * height) + sqrtf(objWidth * objWidth + objHeight * objHeight)) / 2.f)) 
		return true;

	return false;
}


// CannonGadget
bool CannonGadget::_sinitOK = sinit();

bool CannonGadget::sinit()
{
    strClass = G_CANNON;
	bool retVal = Gadget::sinit();
	return retVal;
}

CannonGadget::CannonGadget(cpt3f& a_pos, cvec3f& a_dir, cfloat& a_rng) :
	Gadget(a_pos),
	v_canShoot(false),
	dir(a_dir.normalize()),
	rng(a_rng),
	shootDelay(20),
	lastShoot((ulong)UNDEFINED)
{
	destDir = dir;

	assert(rng > 0.f);

	BITMAP* actualBmp = v_gTxtObj.m_getActualBitmap();
	tmpBmp = create_bitmap(actualBmp->w, actualBmp->h);
	clear_to_color(tmpBmp, makecol(255, 0, 255));
	assert(NULL != tmpBmp);

	placeObj(this);
}

CannonGadget::~CannonGadget()
{
    destroy_bitmap(tmpBmp);
}

cpt3f CannonGadget::m_getNewPosForObj(BasicObject* a_pObj)
{
	assert((BasicObject*)0 != a_pObj);

    UNITS_VEC::iterator it = find(v_vAssUnits.begin(), v_vAssUnits.end(), a_pObj);
	assert(it != v_vAssUnits.end());

	cpt3f objPos = a_pObj->m_getPos();
	cpt3f gdgPos = m_getPos();

	if (objFarOutsideGadget(a_pObj))
	{
		Flock* pFl = a_pObj->m_getFlock();
		cpt3f newPos = pFl->m_getNewPosForObj();

		return newPos;
	}

	// obiekty operuja na gadgecie
	cfloat cellSizeX = getCellSizeX();
	
	cfloat widthInPxls = width * cellSizeX;
	cfloat hghtInPxls  = height * cellSizeX;
	assert(hghtInPxls > 0.f);

	cfloat diag = sqrtf((widthInPxls * widthInPxls) + (hghtInPxls * hghtInPxls)) * .9f;
	assert(diag > 0.f);

/*
	\
	 \
 |	  \ 	
 W     X---->
 | 	  /	
	 /	
	/  -H- 
*/

	cint idx = ((a_pObj == v_vAssUnits[0]) ? 0 : 1);

    vec3f vec = (-dir);
	cfloat angle = atanf(widthInPxls / hghtInPxls);

	pt3f newPos = INVALID_POS;
	if (0 == idx)
	{
		vec.rotateZ(angle);
		newPos = gdgPos + (vec.normalize() * (.5f * diag));
	}
	else
	{
		vec.rotateZ(-angle);
		newPos = gdgPos + (vec.normalize() * (.5f * diag));
	}

	return newPos;
}

void CannonGadget::m_draw(int a_nUserId, BITMAP* a_pDestBmp)
{
	TextureObject& txtObj = v_gTxtObj;
	BITMAP* actualBmp = txtObj.m_getActualBitmap();

	if ((BITMAP*)0 == tmpBmp)
	{
		cout << "WH: " << actualBmp->w << " " << actualBmp->h << endl;
        //tmpBmp = create_bitmap(     actualBmp->w, actualBmp->h);
		//assert((BITMAP*)0 != tmpBmp);
	}

	cvec3f refVec(0.f, -1.f, 0.f);
	float angleR, angleD, sign;
	dir.getAngle(refVec, angleR, sign);
	angleD = RAD2DEG(angleR);
	clear_to_color(tmpBmp, makecol(255, 0, 255));
    rotate_sprite(tmpBmp, actualBmp, 0, 0, itofix((int)(angleD * 256.f / 360.f)));
    
    drawBmp(a_nUserId,
			a_pDestBmp,
			tmpBmp,
			m_getPos(),
			width,
			height,
			true,
			true);

	draw_line(a_nUserId, a_pDestBmp, m_getPos() , dir * 100.f, makecol(255, 255, 0));
}

void CannonGadget::m_update()
{
	assert(dir != INVALID_VEC && destDir != INVALID_VEC);

	if (!isOperated())
	{
#ifdef __TRACE_ME
		cout << "CannonGadget not operated" << endl;
#endif
		return;
	}

	float ang2DestR, ang2DestD;
	float sgn2Dest;

	dir.getAngle(destDir, ang2DestR, sgn2Dest);
	ang2DestD = RAD2DEG(ang2DestR);

    // obrot
	if (ang2DestD > 1.f)
	{
		dir.rotateZ(DEG2RAD(sgn2Dest * 2.f));
		return;
	}

	// strzelanie
	if (canShoot())
	{
		ulong frameNo = BasicConfiguration::m_getFrameNo();
		if (lastShoot == (ulong)UNDEFINED || (frameNo - lastShoot >= shootDelay))
		{
            m_throwBullet(m_getPos() + dir.normalize() * rng * 2.5f, &G_BOMB_BULLET_CLASS, false);
			lastShoot = frameNo;

			uint assign = v_vAssUnits[0]->m_getAssign();
			TGManager* pTGM = TGManager::m_getTGManager();
			assert((TGManager*)0 != pTGM);

			TACTICGRP_VEC& vec  = pTGM->m_getAvailableTGs();
			int i;
			for (i = 0; i < vec.size(); ++i)
			{
				TG* pTG = vec[i];
				if (pTG->m_getAssign() != assign)
				{
					this->setDestDir(cvec3f(pTG->m_getGroupCenter() - m_getPos()).normalize());		
				}
			}
		}
	}

	return;
}

