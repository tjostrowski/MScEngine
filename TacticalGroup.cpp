#include "common.h"
#include "behavior_defs.h"


// Rectangle
Rectangle::Rectangle(cpt3f& a_luPt, cpt3f& a_drPt) 
{
	valid = true;

	assert(a_luPt != INVALID_POS && a_drPt != INVALID_POS);
	assert(a_luPt.x < a_drPt.x && a_luPt.y < a_drPt.y);

	cpt3f ldPt = cpt3f(a_luPt.x, a_drPt.y, 0.f);
	cpt3f urPt = cpt3f(a_drPt.x, a_luPt.y, 0.f);

	if (posOutsideGrid(a_luPt) ||
		posOutsideGrid(a_drPt) ||
		posOutsideGrid(ldPt) ||
		posOutsideGrid(urPt))
	{
		valid = false;
		return;
	}

	luPt = a_luPt;
	drPt = a_drPt;
}

bool Rectangle::ptIsInside(cpt3f& a_pt)
{
	if (!valid)
		return false;

	return (BETWEEN(a_pt.x, luPt.x, drPt.x) &&
			BETWEEN(a_pt.y, luPt.y, drPt.y));
}


// Circle
Circle::Circle(cpt3f& a_ctr, cfloat& a_rad) 
{
	valid = true;

	assert(a_ctr != INVALID_POS && a_rad > 0.f);

	cpt3f pt1 = cpt3f(a_ctr.x - a_rad, a_ctr.y - a_rad, 0.f);
	cpt3f pt2 = cpt3f(a_ctr.x + a_rad, a_ctr.y - a_rad, 0.f);
	cpt3f pt3 = cpt3f(a_ctr.x + a_rad, a_ctr.y + a_rad, 0.f);
	cpt3f pt4 = cpt3f(a_ctr.x - a_rad, a_ctr.y + a_rad, 0.f);

	if (posOutsideGrid(pt1) ||
		posOutsideGrid(pt2) ||
		posOutsideGrid(pt3) ||
		posOutsideGrid(pt4))
	{
		valid = false;
		return;
	}

	ctr = a_ctr;
	rad = a_rad;
}

bool Circle::ptIsInside(cpt3f& a_pt)
{
	if (!valid)
		return false;

	return (dist(a_pt, ctr) <= rad);
}


// Triangle
Triangle::Triangle(cpt3f& a_pt1, cpt3f& a_pt2, cpt3f& a_pt3)
{
	valid = true;

	assert(INVALID_POS != a_pt1 && INVALID_POS != a_pt2 && INVALID_POS != a_pt3);

	if (posOutsideGrid(a_pt1) ||
		posOutsideGrid(a_pt2) ||
		posOutsideGrid(a_pt3) )
	{
		valid = false;
		return;
	}

	pt1 = a_pt1;
	pt2 = a_pt2;
	pt3 = a_pt3;
}

bool Triangle::ptIsInside(cpt3f& a_pt)
{
	if (!valid)
		return false;

	cfloat x = a_pt.x, y = a_pt.y;
	cfloat x1 = pt1.x, y1 = pt1.y;
	cfloat x2 = pt2.x, y2 = pt2.y;
	cfloat x3 = pt3.x, y3 = pt3.y;
  
	cfloat fAB = (y - y1) * (x2 - x1) - (x - x1) * (y2 - y1);
	cfloat fCA = (y - y3) * (x1 - x3) - (x - x3) * (y1 - y3);
	cfloat fBC = (y - y2) * (x3 - x2) - (x - x2) * (y3 - y2);

	if ((fAB * fBC > 0.f) && (fBC * fCA > 0.f)) 
		return true;

	return false;	
}


// TacticalGroup
ulong DEF_TG_BEHAVIOR = (E_BEH_RETREAT_FROM_HEAVY_ATTACK | E_BEH_ATTACK_WEAK_ETGS | E_BEH_TRACE_ATTACKED_ETG | E_BEH_ATTACK_STRONG_ETGS | E_BEH_ATTACK_HLP_ATTACKED_FRTGS | E_BEH_SPLIT_WHEN_OVERFORCE | E_BEH_SPLIT_WHEN_MANY_NEAR_ETGS | E_BEH_JOIN_NEAR_FRTGS | E_BEH_MISC_USE_CANNON | E_BEH_EXP_TG | E_BEH_PCK_TG);

ulong TG::MAX_BEH_TIME = 1000;


TG::TacticalGroup(const uint& a_assign, TGManager* a_pManager, culong& a_beh) :
	assign(a_assign), v_pManager(a_pManager), v_beh(a_beh)
{
	v_grpCenter = INVALID_POS;
	v_grpRadius = 0.f;

	v_vNeighbTGs.clear();

	m_init();
}

TG::TacticalGroup(const uint& a_assign, const UNITS_VEC& a_vec, TGManager* a_pManager, culong& a_beh) :
	assign(a_assign), v_pManager(a_pManager), v_beh(a_beh)
{
	for (int i = 0; i < a_vec.size(); ++i)
		v_vObjs.push_back(a_vec[i]);

	if (a_vec.empty())
	{
		v_grpCenter = INVALID_POS;
		v_grpRadius = 0.f;
	}
	else
	{
		m_recalculateGroupParams();
	}

	m_init();
}

void TG::m_init()
{
	v_shape = E_SHP_NONE;

	v_grpForce = v_grpThreat = 0.f;

    v_jinBeh = E_BEH_UNDEF;

	v_grpDestPt = INVALID_POS;

	strictForm = false;
}

TG::~TacticalGroup()
{
}

bool TG::m_split(const uint a_nSubs, cbool a_smpl /* = true */)
{
	TGManager* pTGM = TGManager::m_getTGManager();
	assert((TGManager*)0 != pTGM);

	if (a_smpl || !m_isFormed())
	{
        TG* pNewTG1 = pTGM->m_createTG(assign);
		TG* pNewTG2 = pTGM->m_createTG(assign);

		const int numObjs = v_vObjs.size();
		if (numObjs < 3)
			return false;

		int i;
		for (i = 0; i < (numObjs >> 1); ++i)
			pNewTG1->m_addObj(v_vObjs[i]);

		for (i = (numObjs >> 1); i < numObjs; ++i)
			pNewTG2->m_addObj(v_vObjs[i]);

		pTGM->m_removeTG(this->m_getId());

		return true;
	}

    const uint nETGs = v_vEnTGs.size();

	if (nETGs < 2)
	{
#ifdef __TRACE_ME
		cout << "" << endl;
#endif
		return false;
	}

	map<TG*, TG*> mapEnTGs2NewTGs;
    TG* newTGs[nETGs];
	int i;
	for (i = 0; i < nETGs; ++i)
	{
		TG* pNewTG = pTGM->m_createTG(assign);
		newTGs[i] = pNewTG;

		mapEnTGs2NewTGs[ v_vEnTGs[i] ] = newTGs[i]; 
	}

    TG* pDefEnTG = v_vEnTGs[0];
	assert((TG*)0 != pDefEnTG);

    for (i = 0; i < v_vObjs.size(); ++i)
	{
		BasicObject* pObj = v_vObjs[i];
		TG* pEnTG = v_mapObjs2TGEnemTGVs[pObj];

		bool enTGFound = (((TG*)0 != pEnTG) && (v_vEnTGs.end() != find(v_vEnTGs.begin(), v_vEnTGs.end(), pEnTG) ));
		
		if (!enTGFound)
			mapEnTGs2NewTGs[pDefEnTG]->m_addObj(pObj);
		else
			mapEnTGs2NewTGs[pEnTG]->m_addObj(pObj);
    }

	int NN = 0;
	for (i = 0; i < nETGs; ++i)
	{
		TG* pTG = newTGs[i];
		NN += pTG->m_getNumObjs();

		if (0 == pTG->m_getNumObjs())
			pTGM->m_removeTG(pTG->m_getId());
    }
	assert(NN == this->m_getNumObjs());

    pTGM->m_removeTG(this->m_getId());

	return true;
}

void TG::m_getObjs(UNITS_VEC& a_vv)
{
    a_vv = v_vObjs;
}

bool TG::m_join(TacticalGroup*& a_tg)
{
	TGManager* pTGM = TGManager::m_getTGManager();
	
	UNITS_VEC units;
	a_tg->m_getObjs(units);
	this->m_addObjs(units);

	pTGM->m_removeTG(a_tg->m_getId());

	return true;
}

cpt3f TG::m_getNewPosForObj(BasicObject* a_pObj)
{
	assert((BasicObject*)0 != a_pObj);
	assert(v_vObjs.end() != find(v_vObjs.begin(), v_vObjs.end(), a_pObj));

    /*cvec3f& vv = v_mapObjs2Velcs[a_pObj];
	cpt3f& objPos = a_pObj->m_getPos();
	cpt3f newPos = objPos + vv;*/
	Flock* pFl = a_pObj->m_getFlock();
	assert((Flock*)0 != pFl);

	/*if (m_isFormed())
		pFl->m_stopFlock();
	else
		pFl->m_restartFlock();*/

    cpt3f newPos = pFl->m_getNewPosForObj();

	return newPos;
}

void TG::m_form()
{
    if (m_isFormed())
		return;

    cpt3f&  grpCenter 		= v_grpCenter;
	cfloat& grpRadius		= v_grpRadius;
	cfloat& frmdGrpRadius 	= v_frmdGrpRadius;

	m_setDestPt(grpCenter);
	
#undef __TRACE_ME
#ifdef __TRACE_ME
	cout << "grpRadius: " 		<< grpRadius 		<< endl;
	cout << "frmdGrpRadius: "	<< frmdGrpRadius	<< endl;
#endif
	
	int i;
	for (i = 0; i < v_vObjs.size(); ++i)
	{
        BasicObject* pObj = v_vObjs[i];
		cpt3f objPos = pObj->m_getPos();

		Flock* pFl = pObj->m_getFlock();
		assert((Flock*)0 != pFl);

		cfloat dist2GrpCenter = dist(pObj->m_getPos(), grpCenter);
		//if (dist2GrpCenter < .1f * frmdGrpRadius)
			//pFl->m_setTargetPt(INVALID_POS);
	}

	return;
}

bool TG::m_isFormed()
{
	return (v_grpRadius < v_frmdGrpRadius);
}

cfloat& TG::m_computeFrmdGrpRadius()
{
	const uint numObjs = v_vObjs.size();
	cfloat cellSizeX = getCellSizeX();

	cfloat unitArea = 1.f * (M_PI * (.8f * cellSizeX) * (.8f * cellSizeX));
	cfloat grpRad = 1.1f * sqrtf((numObjs * unitArea) / M_PI);

	v_frmdGrpRadius = grpRad;

	return v_frmdGrpRadius;
}

cpt3f& TG::m_computeGroupCenter()
{
    assert(!v_vObjs.empty());

	const uint numObjs = v_vObjs.size();

	pt3f center(0.f, 0.f, 0.f);

	UNITS_VEC::iterator itUU;
	for (itUU = v_vObjs.begin(); itUU != v_vObjs.end(); ++itUU)
	{
		BasicObject* pObj = (*itUU);
		cpt3f pos = pObj->m_getPos();

		center += pos;
	}

	center /= (float)(numObjs);

	v_grpCenter = center;
	return v_grpCenter;
}

cfloat& TG::m_computeGroupRadius()
{
	const pt3f& grpCenter = v_grpCenter;

	cfloat cellSizeX = getCellSizeX();

	float rad = -1.f;
	UNITS_VEC::iterator itUU;
	for (itUU = v_vObjs.begin(); itUU != v_vObjs.end(); ++itUU)
	{
		BasicObject* pObj = (*itUU);
		cpt3f pos = pObj->m_getPos();

		float dist2CTR = dist(grpCenter, pos) + (.5f * cellSizeX);
		if (dist2CTR > rad)
			rad = dist2CTR;	
	}
	
	v_grpRadius = rad;
	return v_grpRadius;
}

cfloat& TG::m_computeGroupDispersion()
{
	assert(!v_vObjs.empty());

	const pt3f& grpCenter	= v_grpCenter;
	const float	grpRadius	= v_grpRadius;	

	const uint numObjs = v_vObjs.size();

	float dispersion = 0.f;

	UNITS_VEC::iterator itUU;
	for (itUU = v_vObjs.begin(); itUU != v_vObjs.end(); ++itUU)
	{
		BasicObject* pObj = (*itUU);
		pt3f pos = pObj->m_getPos();

		float distToCenter = dist(grpCenter, pos);
		dispersion += (distToCenter * distToCenter); 
	}

	dispersion = (sqrtf(dispersion) / numObjs);
	v_grpDispersion = dispersion;
	return v_grpDispersion;
}

cfloat& TG::m_computeGroupForce()
{
	float force = 0.f;

	cfloat cellSizeX = getCellSizeX();

	cuint nObjs = v_vObjs.size();
	cfloat nOBF = .9f,
		   radF = 1.f - nOBF; 	

	// 20 obiektow => duzo w grupie, im mniejszy promien tym silniejsza, 10.f * cellSizeX - w miare duzy promien
	float grpRadius = max(v_grpRadius, 2.f * cellSizeX); 
	force = nOBF * (nObjs / 20.f) + radF * ((10.f * cellSizeX) / grpRadius);   
	force = clampf(force, 0.f, 1.f);

	v_grpForce = force;	
	return v_grpForce;
}

cfloat& TG::m_computeGroupThreat()
{
	float threat = 0.f;

	// 5 wrogich grup naokolo => duzo
	threat = (v_vEnTGs.size() / 5.f);
	threat = clampf(threat, 0.f, 1.f);

	v_grpThreat = threat;
	return v_grpThreat;
}

int TG::m_getNeighbTGs()
{
	TGManager* pTGM = v_pManager;
	assert((TGManager*)0 != pTGM);
    cpt3f& thisGrpCenter = v_grpCenter;
    cfloat& thisGrpRadius = v_grpRadius;
	cfloat& thisGrpDispersion = v_grpDispersion;

	cfloat cellSizeX = getCellSizeX();
	cfloat cellSizeY = getCellSizeY();

	cfloat fovRAD_ctr = thisGrpRadius + 10.f * cellSizeX;

	TACTICGRP_VEC vNeighbTGs;
    TACTICGRP_VEC& vTGs = pTGM->m_getAvailableTGs();
	int i;
    for (i = 0; i < vTGs.size(); ++i)
	{
		TG* pTG	= vTGs[i];
        cpt3f grpCenter = pTG->m_getGroupCenter();
		cfloat grpRadius = pTG->m_getGroupRadius();	

        if (this != pTG &&
			dist(thisGrpCenter, grpCenter) <= fovRAD_ctr + grpRadius)
			vNeighbTGs.push_back(pTG);
	}

	TACTICGRP_VEC vEnTGs, vFrTGs;
	for (i = 0; i < vNeighbTGs.size(); ++i)
	{	
		TG* pTG = vNeighbTGs[i];
		if (pTG->m_getAssign() == assign)
			vFrTGs.push_back(pTG);
		else
			vEnTGs.push_back(pTG);	
	}

	v_vNeighbTGs = vNeighbTGs;
	v_vEnTGs = vEnTGs;
	v_vFrTGs = vFrTGs;

    return v_vNeighbTGs.size();
}

void TG::m_allocateUnitsToEnemyTGs()
{
	TGManager* pTGM = v_pManager;
	assert((TGManager*)0 != pTGM);

    const uint nEnTGs = v_vEnTGs.size();
	if (0 == nEnTGs)
		return;

#define __TRACE_ME
#ifdef __TRACE_ME
	cout << "nEnTGs: " << nEnTGs << endl;
#endif

	const uint nObjs = v_vObjs.size();
	uint ass2ETGs[nEnTGs];
	memset(ass2ETGs, (int)0, nEnTGs * sizeof(uint));	
	float enTGsINFL[nEnTGs]; // INFL - influence

	float sumINFL = 0.f;
	int i;
	for (i = 0; i < nEnTGs; ++i)
	{
		TG* pTG = v_vEnTGs[i];
		cfloat& grpForce = pTG->m_getGroupForce();
		cpt3f& grpCenter = pTG->m_getGroupCenter();

        cfloat dist2Grp = dist(grpCenter, v_grpCenter);
		assert(dist2Grp > 0.f);

		cfloat frcF = .6f;
		cfloat dstF = 1.f - frcF; 
		float infl = (frcF * grpForce) + (dstF * (100.f / dist2Grp)); 

		enTGsINFL[i] = infl;
		sumINFL += infl;
	}

	assert(sumINFL > 0.f);

	for (i = 0; i < v_vObjs.size(); ++i)
	{
		v_mapObjs2TGEnemTGVs[ v_vObjs[i] ] = (TG*)0;
	}

	int objIdx = 0, j;
	for (i = 0; i < nEnTGs; ++i)
	{
		ass2ETGs[i] = max((uint)((enTGsINFL[i] / sumINFL) * nObjs), (uint)1);
		cout << enTGsINFL[i] << " " << sumINFL << " " << nObjs <<  " ass2ETGs[i]: " << ass2ETGs[i] << endl; // => ZA BARDZO SIE ROZBIJA
		for (j = objIdx; (j < objIdx + ass2ETGs[i]) && (j < nObjs); ++j)
		{
			BasicObject* pObj = v_vObjs[j];
			v_mapObjs2TGEnemTGVs[pObj] = v_vEnTGs[i];
		}
	}

	return;
}

void TG::m_shoot()
{
	int i;
	for (i = 0; i < v_vObjs.size(); ++i)
	{
		BasicObject* pObj = v_vObjs[i];
		TG* pEnTG = v_mapObjs2TGEnemTGVs[pObj];

        if ((TG*)0 == pEnTG) continue;

        cpt3f objPos = pObj->m_getPos();
		cpt3f EEgrpCenter = pEnTG->m_getGroupCenter();
		cfloat EEgrpRadius = pEnTG->m_getGroupRadius();

		// strzelamy w losowy punkt w polu widoku
        cfloat dist2TG = dist(objPos, EEgrpCenter);
		assert(dist2TG > 0.f);
		vec3f shVEC = (EEgrpCenter - objPos).normalize();
		
		cfloat shRAD = atanf(EEgrpRadius / dist2TG);
		shVEC.rotateZ( - shRAD );
		shVEC.rotateZ(ranf() * 2.f * shRAD);

		if (!m_isInFOV(pObj, pEnTG, shVEC)) 
		{
#define __TRACE_ME
#ifdef  __TRACE_ME
			cout << endl << endl << endl << ">>>>>>>> !m_isInFOV <<<<<<<<<" << endl << endl << endl;
#endif

			continue;
		}

		pObj->setDestDir(EEgrpCenter);

		BClass* pCl = (assign == 0) ? &G_DEF_BULLET_CLASS : &G_DEF2_BULLET_CLASS;
		pObj->m_throwBullet(shVEC.normalize() * 10000.f, pCl);

#ifdef  __TRACE_ME
		cout << this->m_getId() << "  : " << pObj->m_getId() << " shooting " << endl;
#endif
	}

	return;
}

bool TG::m_isInFOV(BasicObject* a_pObj, TG* a_pTG, cvec3f& a_bltDir)
{
	assert(v_vObjs.end() != find(v_vObjs.begin(), v_vObjs.end(), a_pObj));

	cpt3f objPos = a_pObj->m_getPos();
	cvec3f bltDir = a_bltDir.normalize();

	cfloat cellSizeX = getCellSizeX();

	cfloat delta = (cellSizeX * 1.f);

    pt3f tstPt = objPos + bltDir * delta;
	while (dist(tstPt, v_grpCenter) < v_grpRadius)
	{
		INT_COORDS cell = getCellForPos(tstPt);
		if (cell == INVALID_COORDS)
			return true;
		
		GridElement* pEl = getGridElement(cell);
		assert((GridElement*)0 != pEl);

		if (pEl->m_isCoveredByObject())
		{
			BasicObject* pCovObj = pEl->m_getCoveringObject();
			if (pCovObj->m_getAssign() == a_pObj->m_getAssign())
			{
				return false;
			}
		}

		tstPt += (bltDir * delta);
	}

	return true;
}

void TG::m_recalculateGroupParams()
{
	int nNTGs = m_getNeighbTGs();

	m_computeFrmdGrpRadius();
	m_computeGroupCenter();
	m_computeGroupRadius();
    m_computeGroupForce();
	m_computeGroupThreat();

	m_allocateUnitsToEnemyTGs();

#define __TRACE_ME
#ifdef __TRACE_ME
	//cout << "grpCenter: " << v_grpCenter << endl;
	//cout << "grpRadius: " << v_grpRadius << endl;
	//cout << "grpDisp:   " << v_grpDispersion << endl;
	cout << "grpForce:  " << v_grpForce		<< endl;
	cout << "grpThreat: " << v_grpThreat	<< endl;
#endif
}

void TacticalGroup::m_update()
{
	if (v_vObjs.empty())
	{
		TGManager* pTGM = v_pManager;
		assert((TGManager*)0 != pTGM);

		pTGM->m_removeTG(this->m_getId());

		return;
	}

	m_recalculateGroupParams();

    if (!strictForm &&
		!m_isFormed() && (v_grpDestPt == INVALID_POS || dist(v_grpCenter, v_grpDestPt) < (16.f)))
	{
#define __TRACE_ME
#ifdef __TRACE_ME
		cout << "FORMING: " << m_getId() << endl;
#endif

		m_form();
		return;
	}

    m_shoot();

	m_handleBehavior();

	//m_computeInteractingGrps();

	//m_allocateUnitsToEnemyTGs();

	//m_move();
}

void TacticalGroup::m_draw(int a_nUserId, BITMAP* a_pDestBmp)
{
	if (v_vObjs.empty())
		return;
    
	cpt3f grpCenter = v_grpCenter;
	if (grpCenter == INVALID_POS)
		return;
	cfloat grpRadius = v_grpRadius;
	assert(grpRadius > 0.f);

	if (__DRAW_TG)
	{
		draw_circle(a_nUserId, a_pDestBmp, grpCenter, 5.f, makecol(255, 0, 0), true);

		draw_circle(a_nUserId, a_pDestBmp, grpCenter, grpRadius, makecol(0, 255, 0), false);
	}
}

void TG::m_addObj(BasicObject* a_pObj)
{
	assert((BasicObject*)0 != a_pObj);
	v_vObjs.push_back(a_pObj);
	v_mapObjs2TGEnemTGVs[a_pObj]	= (TG*)0;
	v_mapObjs2Velcs[a_pObj] 		= ZERO_VEC;

	a_pObj->m_reconfigureToBehavior(
		new SimpleBehavior(a_pObj, a_pObj->m_getTxtID0(), this));

	m_recalculateGroupParams();
}

void TG::m_addObjs(UNITS_VEC& a_vec)
{
	for (int i = 0; i < a_vec.size(); ++i)
		m_addObj(a_vec[i]);
}

void TG::m_removeObj(BasicObject* a_pObj)
{
	assert(a_pObj != (BasicObject*)0);
	UNITS_VEC::iterator it = find(v_vObjs.begin(), v_vObjs.end(), a_pObj);
	bool objFound = (it != v_vObjs.end());

	if (objFound)
	{
		v_vObjs.erase(it);
		m_recalculateGroupParams();
	}

	return;
}

bool TG::delObjRef(BasicObject* a_pObj)
{
	UNITS_VEC::iterator it = find(v_vObjs.begin(), v_vObjs.end(), a_pObj);
	if (it == v_vObjs.end())
		return false;

	v_vObjs.erase(it);

	return true;
}

bool TG::m_isStrictlyFormed()
{
	assert(strictForm);

	cfloat cellSizeX = getCellSizeX();

	int i;
	for (i = 0; i < v_vObjs.size(); ++i)
	{
		BasicObject*& pObj = v_vObjs[i];
		cpt3f objPos = pObj->m_getPos();

		Flock* pFl = pObj->m_getFlock();
		cpt3f trgtPt = pFl->m_getTargetPt();

		if (INVALID_POS == trgtPt || dist(trgtPt, objPos) > (cellSizeX * .5f))
			return false;
	}

    return true;
}

bool TG::m_expand()
{
	cfloat cellSizeX = getCellSizeX();
	cfloat cellSizeY = getCellSizeY();

	v_grpDestPt = v_grpCenter + cvec3f(cellSizeX, cellSizeY, 0.f);
	cpt3f mdstPt = v_grpDestPt;
	
	cuint numObjs = v_vObjs.size();

	if (numObjs < 3)
		return false;

	v_shape = E_SHP_TRIANGLE;

	cfloat maxL = numObjs * (cellSizeX * .5f);
	vec3f tstVEC = cvec3f(1.f, 0.f, 0.f);  
	tstVEC = tstVEC.rotateZ(ranf() * (2.f * M_PI)).normalize() * maxL;
	vec3f tstORTHO = tstVEC.orthoL().normalize() * (cellSizeX);
    Shape* frmShape = new Triangle
						(mdstPt + tstVEC,
						 mdstPt - tstVEC,
						 mdstPt - tstORTHO);

	if (!frmShape->isValid())
	{
#ifdef __TRACE_ME
		cerr << "" << endl;
		return false;
#endif
	}

    int i;
	for (i = 0; i < v_vObjs.size(); ++i)
	{
		BasicObject*& pObj = v_vObjs[i];
		cpt3f objPos = pObj->m_getPos();

		cpt3f stGEN = mdstPt - tstORTHO * .5f - tstVEC;
		cfloat LLtstVEC = 2.f * tstVEC.length();

		pt3f newPos = INVALID_POS;
		bool posFound = false;
		cuint MAX_RETRY = 10;
		int j;
		for (j = 0; j < MAX_RETRY; ++j)
		{
			newPos = stGEN + tstVEC.normalize() * LLtstVEC * ranf();
			if (frmShape->ptIsInside(newPos))
			{
				posFound = true;
                break;
			}
		}

		if (!posFound)
			return false;

		Flock* pFl = pObj->m_getFlock();
		pFl->m_setTargetPt(newPos);
	}

	delete frmShape;

	strictForm = true;

    return true;
}

bool TG::m_pack()
{
	cuint numObjs = v_vObjs.size();
	cfloat cellSizeX = getCellSizeX();
	cfloat cellSizeY = getCellSizeY();

	if (numObjs < 3)
		return false;

	v_grpDestPt = v_grpCenter + cvec3f(cellSizeX, cellSizeY, 0.f);
	cpt3f mdstPt = v_grpDestPt;

    cfloat unitArea = 1.f * (M_PI * (.6f * cellSizeX) * (.6f * cellSizeX));
	cfloat grpRad = sqrtf((numObjs * unitArea) / M_PI);

	Shape* frmShape = new Circle(mdstPt, grpRad);
	if (!frmShape->isValid())
	{
#ifdef __TRACE_ME
		cerr << "" << endl;
		return false;
#endif
	}

	int i;
	for (i = 0; i < numObjs; ++i)
	{
		BasicObject* pObj = v_vObjs[i];
		cpt3f objPos = pObj->m_getPos();

		pt3f newPos = INVALID_POS;
		bool posFound = false;
		cuint MAX_RETRY = 10;
		int j;
		for (j = 0; j < MAX_RETRY; ++j)
		{
			vec3f tstVEC = cvec3f(ranf() * grpRad, 0.f, 0.f);
			tstVEC = tstVEC.rotateZ((2.f * M_PI) * ranf());

			newPos = mdstPt + tstVEC;

			if (frmShape->ptIsInside(newPos))
			{
				posFound = true;
				break;
			}
		}

		if (!posFound)
			return false;

		Flock* pFl = pObj->m_getFlock();
		pFl->m_setTargetPt(newPos);
	}

	delete frmShape;

	strictForm = true;

	return true;
}

void TG::m_setDestPt(cpt3f& a_pt)
{
	if (posOutsideGrid(a_pt))
		return;

    cfloat cellSizeX = getCellSizeX();
	cfloat cellSizeY = getCellSizeY();
    
	cuint numObjs = v_vObjs.size();

	cfloat unitArea = 1.f * (M_PI * (.8f * cellSizeX) * (.8f * cellSizeX));
	cfloat grpRad = sqrtf((numObjs * unitArea) / M_PI);   

#ifdef __TRACE_ME
	cout << "grpRad: " << grpRad << endl;
#endif

	cpt3f grpCenter = a_pt;

	Shape* frmShape = (Shape*)0;
	if (v_shape == E_SHP_NONE || v_shape == E_SHP_CIRCLE)
	{
		frmShape = new Circle(grpCenter, grpRad);
	}
	else if (v_shape == E_SHP_TRIANGLE)
	{
		cfloat h23 = grpRad;
		cfloat h13 = grpRad / 2.f;
		cfloat a = (2.f / sqrtf(3.f)) * grpRad;
		cfloat a12 = (a / 2.f);

		frmShape = new Triangle(
						cpt3f(grpCenter.x, grpCenter.y - h23, 0.f),
						cpt3f(grpCenter.x - a12, grpCenter.y + h13, 0.f),
						cpt3f(grpCenter.x + a12, grpCenter.y + h13, 0.f));
	}
	else if (v_shape == E_SHP_RECTANGLE)
	{
		cfloat a12 = (sqrtf(2.f) / 2.f) * grpRad;
		cfloat a = a12 * 2.f;
		frmShape = new Rectangle(
						cpt3f(grpCenter.x - a12, grpCenter.y - a12, 0.f),
						cpt3f(grpCenter.x + a12, grpCenter.y + a12, 0.f));
	}

	if (!frmShape->isValid())
	{
#ifdef __TRACE_ME
		cerr << "" << endl;
		return;
#endif
	}

    int i;
	for (i = 0; i < v_vObjs.size(); ++i)
	{
		BasicObject*& pObj = v_vObjs[i];
		cpt3f objPos = pObj->m_getPos();

		//if (dist(objPos, a_pt) < v_frmdGrpRadius) continue;
        
		cuint MAX_RETRY = 10;
		pt3f newPos = INVALID_POS;
		bool posFound = false;
		int j = 0;
        for (j = 0; j < MAX_RETRY && !posFound; ++j)
		{
			cfloat rndRAD = ranf() * grpRad;
			cfloat rndANG = ranf() * (2.f * M_PI);

			vec3f tstVEC = cvec3f(rndRAD, 0.f, 0.f);  
			tstVEC.rotateZ(rndANG);

			newPos = grpCenter + tstVEC;
			if (frmShape->ptIsInside(newPos))
			{
				posFound = true;
			}
		}

		if (!posFound)
		{
#ifdef __TRACE_ME
			cerr << "" << endl;
			return;
#endif
		}

		draw_circle(DEFAULT_USER_ID, pBuffer, newPos, 10.f, makecol(255, 255, 255), true);

        Flock* pFl = pObj->m_getFlock();
		pFl->m_setTargetPt(newPos);
	}


	delete frmShape;

	v_grpDestPt = a_pt;

	return;
}

int TG::comp_TGsByHlpDemand(const void* a_p1, const void* a_p2)
{
	const TG* pTG1 = static_cast<const TG*>(a_p1);
	const TG* pTG2 = static_cast<const TG*>(a_p2);

	assert(((TG*)0 != pTG1) && ((TG*)0 != pTG2));

	cfloat hlpDemand1 = pTG1->m_getHlpDemand();
	cfloat hlpDemand2 = pTG2->m_getHlpDemand();

    return (hlpDemand1 < hlpDemand2);
}

void TG::m_getSortedEnTGsByHlpDemand(TACTICGRP_VEC& a_vec)
{
	TGManager* pTGM = v_pManager;
	assert((TGManager*)0 != pTGM);

	TACTICGRP_VEC vEnTGs;
	TACTICGRP_VEC& vTGs = pTGM->m_getAvailableTGs();
	int i;
	for (i = 0; i < vTGs.size(); ++i)
	{
		TG* pTG = vTGs[i];
		if (pTG->m_getAssign() != assign)
			vEnTGs.push_back(pTG);
	}

	const uint nEnTGs = vEnTGs.size();
    qsort(&vEnTGs[0], nEnTGs, sizeof(TG*), comp_TGsByHlpDemand);

	a_vec = vEnTGs;

	return;
}

void TG::m_getSortedFrTGsByHlpDemand(TACTICGRP_VEC& a_vec)
{
	TGManager* pTGM = v_pManager;
	assert((TGManager*)0 != pTGM);

	TACTICGRP_VEC vFrTGs;
	TACTICGRP_VEC& vTGs = pTGM->m_getAvailableTGs();
	int i;
	for (i = 0; i < vTGs.size(); ++i)
	{
		TG* pTG = vTGs[i];
		if (pTG->m_getAssign() == assign && (pTG != this))
			vFrTGs.push_back(pTG);
	}

	const uint nFrTGs = vFrTGs.size();
    qsort(&vFrTGs[0], nFrTGs, sizeof(TG*), comp_TGsByHlpDemand);

	a_vec = vFrTGs;

	return;
}

// >>>> BEHAVIORS <<<<
ulong TG::inOK_AndIfThenStart(culong& a_beh)
{
    /*if (a_beh & E_BEH_RETREAT_FROM_HEAVY_ATTACK)
	{
		bool inOK = (v_grpThreat > .8f);

		if (inOK)
		{
			vec3f vec(1.f, 0.f, 0.f);
			vec.rotateZ(2.f * M_PI * ranf());

			this->m_setDestPt(v_grpCenter + vec * 30.f * v_grpRadius * ranf());

			return E_BEH_RETREAT_FROM_HEAVY_ATTACK;
		}
	}

	if (a_beh & E_BEH_TRACE_ATTACKED_ETG) ;
		
    if (a_beh & E_BEH_ATTACK_WEAK_ETGS)
	{
		bool inOK = (v_grpThreat < .5f);

		if (inOK)
		{
			TACTICGRP_VEC enems;
			m_getSortedEnTGsByHlpDemand(enems);
			assert(enems.size() > 0);
			TG*& pWeakETG = enems.back();
			cfloat enHlpDemand = pWeakETG->m_getHlpDemand();
			if (enHlpDemand > .5f)
			{
				cpt3f weakETGCtr = pWeakETG->m_getGroupCenter();
				cfloat weakETGRad = pWeakETG->m_getGroupRadius();
				cvec3f vec2WeakETG = (weakETGCtr - v_grpCenter).normalize();

				this->m_setDestPt(weakETGCtr + vec2WeakETG * 1.3f * weakETGRad);

				return E_BEH_ATTACK_WEAK_ETGS;
			}
		}
	}

	if (a_beh & E_BEH_ATTACK_STRONG_ETGS)
	{
		bool inOK = (v_grpThreat < .5f);

		if (inOK)
		{
			TACTICGRP_VEC enems;
			m_getSortedEnTGsByHlpDemand(enems);
			assert(enems.size() > 0);
			TG*& pStrongETG = enems.front();
			cfloat enHlpDemand = pStrongETG->m_getHlpDemand();

            if (enHlpDemand < .5f)
			{
				cpt3f strongETGCtr = pStrongETG->m_getGroupCenter();
				cfloat strongETGRad = pStrongETG->m_getGroupRadius();
				cvec3f vec2StrongETG = (strongETGCtr - v_grpCenter).normalize();

				this->m_setDestPt(strongETGCtr + vec2StrongETG * 1.3f * strongETGRad);

				return E_BEH_ATTACK_STRONG_ETGS;
			}
		}
	}

	if (a_beh & E_BEH_ATTACK_HLP_ATTACKED_FRTGS)
	{
		bool inOK = (v_grpThreat < .5f);

		if (inOK)
		{
			TACTICGRP_VEC frnds;
			m_getSortedFrTGsByHlpDemand(frnds);
			if (frnds.size() > 0)
			{
				TG*& pWeakFTG = frnds.back();
				cfloat frHlpDemand = pWeakFTG->m_getHlpDemand();

                if (frHlpDemand > .5f)
				{
					cpt3f weakFTGCtr = pWeakFTG->m_getGroupCenter();
					cfloat weakFTGRad = pWeakFTG->m_getGroupRadius();
					cvec3f vec2WeakFTG = (weakFTGCtr - v_grpCenter).normalize();

					this->m_setDestPt(weakFTGCtr + vec2WeakFTG * 1.3f * weakFTGRad);

					return E_BEH_ATTACK_HLP_ATTACKED_FRTGS;
				}
			}
		}
	}

	if (a_beh & E_BEH_JOIN_NEAR_FRTGS)
	{
		bool inOK = (v_vFrTGs.size() > 0);

		if (inOK)
		{
			this->m_join(v_vFrTGs[0]);

            return E_BEH_JOIN_NEAR_FRTGS;
		}
	}

	if (a_beh & E_BEH_SPLIT_WHEN_MANY_NEAR_ETGS)
	{
		bool inOK = (v_vEnTGs.size() >= 1);

		if (inOK)
		{
			if (this->m_split((uint)UNDEFINED, false))
				return E_BEH_SPLIT_WHEN_MANY_NEAR_ETGS;
		}
	}

    if (a_beh & E_BEH_SPLIT_WHEN_OVERFORCE)
	{
		cfloat hlpDemand = m_getHlpDemand();
		cout << " hlpDemand: " << hlpDemand << endl; 
		bool inOK = (hlpDemand > .0f);

		if (inOK)
		{
			this->m_split((uint)UNDEFINED, true);

			return E_BEH_SPLIT_WHEN_OVERFORCE;
		}
	}

	if (a_beh & E_BEH_MISC_USE_CANNON)
	{
		cpt3f grpCenter = v_grpCenter;
		cfloat grpRad = v_grpRadius;

		INT_COORDS ctrCell = getCellForPos(grpCenter);
		assert(INVALID_COORDS != ctrCell);

		cfloat cellSizeX = getCellSizeX();

		uint cellRad = (uint)((grpRad + 60.f * cellSizeX) / cellSizeX);

		INT_COORDS_VEC surrCells; 
		getDiffs(ctrCell, cellRad, surrCells);

		CannonGadget* pSelCan = (CannonGadget*)0;
		int i;
		for (i = 0; i < surrCells.size(); ++i)
		{
			INT_COORDS cell = surrCells[i];
			GridElement* pEl = getGridElement(cell);
			if (pEl && pEl->m_isCoveredByGadget())
			{
				CannonGadget* pCan = dynamic_cast<CannonGadget*>(pEl->m_getCoveringGadget());
				assert((CannonGadget*)0 != pCan);	

				if (!pCan->isAnyAssigned())
				{
					pSelCan = pCan;	
					break;
				}
			}
		}

		cuint nUnits = v_vObjs.size();
		bool inOK = ((nUnits > CannonGadget::getReqOpUnitsNum()) && (pSelCan != (CannonGadget*)0));

		if (inOK)
		{
			BasicObject *selObj1 = v_vObjs[0],
						*selObj2 = v_vObjs[1];

			this->m_removeObj(selObj1);
			this->m_removeObj(selObj2);
			pSelCan->assignUnit(selObj1);
			pSelCan->assignUnit(selObj2);

			return E_BEH_MISC_USE_CANNON; 
		}
	}*/

	if (a_beh & E_BEH_EXP_TG)
	{
		bool inOK = true;

		if (inOK)
		{
			if (this->m_expand())
			{
				return E_BEH_EXP_TG;
			}
		}
	}

	if (a_beh & E_BEH_PCK_TG)
	{
		bool inOK = true;

		if (inOK)
		{	
			if (this->m_pack())
			{
				return E_BEH_PCK_TG;
			}
		}
	}

    return E_BEH_UNDEF;
};

bool TG::outOK(culong& a_beh)
{
	ulong frNO = BasicConfiguration::m_getFrameNo();

	switch (a_beh)
	{
	case E_BEH_RETREAT_FROM_HEAVY_ATTACK:
	{
		if (destPtReached())
			return true;

		return false;
	}
	break;
	case E_BEH_TRACE_ATTACKED_ETG:
	{}
	break;
	case E_BEH_ATTACK_WEAK_ETGS:
	{
		if (destPtReached())
			return true;

		return false;
	}
	break;
	case E_BEH_ATTACK_STRONG_ETGS:
	{
		if (destPtReached())
			return true;

		return false;

	}
	break;
	case E_BEH_ATTACK_HLP_ATTACKED_FRTGS:
	{
		if (destPtReached())
			return true;

		return false;
	}
	break;
	case E_BEH_JOIN_NEAR_FRTGS:
	{
		return (frNO - v_jinBehST > 50);
	}
	break;
	case E_BEH_SPLIT_WHEN_MANY_NEAR_ETGS:
	{ /* never occurs */ }
	break;
	case E_BEH_SPLIT_WHEN_OVERFORCE:
	{ /* never occurs */ }
	break;
	case E_BEH_EXP_TG:
	{
		return m_isStrictlyFormed();
	}
	break;
	case E_BEH_PCK_TG:
	{
		return m_isStrictlyFormed();
	}
	break;
	default:
		assert(1);
	}

	return false;
}

void TG::proceed(culong& a_beh)
{
	switch (a_beh)
	{
	case E_BEH_RETREAT_FROM_HEAVY_ATTACK:
	{}
	break;
	case E_BEH_TRACE_ATTACKED_ETG:
	{}
	break;
	case E_BEH_ATTACK_WEAK_ETGS:
	{}
	break;
	case E_BEH_ATTACK_STRONG_ETGS:
	{}
	break;
	case E_BEH_ATTACK_HLP_ATTACKED_FRTGS:
	{}
	break;
	case E_BEH_JOIN_NEAR_FRTGS:
	{}
	break;
	case E_BEH_SPLIT_WHEN_MANY_NEAR_ETGS:
	{}
	break;
	case E_BEH_SPLIT_WHEN_OVERFORCE:
	{}
	case E_BEH_MISC_USE_CANNON:
	{}
	case E_BEH_EXP_TG:
	{}
	case E_BEH_PCK_TG:
	{}
	break;
	default:
		assert(1);
	}

	return;
}

void TG::m_handleBehavior()
{
	ulong& jinBeh = v_jinBeh;
	ulong frNO = BasicConfiguration::m_getFrameNo();

	if (jinBeh != E_BEH_UNDEF)
	{
        if ((frNO - v_jinBehST < MAX_BEH_TIME) || !outOK(jinBeh))
		{
			proceed(jinBeh);
			return;	
		}

		jinBeh &= (ulong)0;
	}

	ulong& beh = v_beh;

	jinBeh = inOK_AndIfThenStart(beh);

	if (jinBeh != E_BEH_UNDEF)
	{
#define __TRACE_ME
#ifdef __TRACE_ME
		cout << endl << endl << endl;
		cout << ">>>>>>>>>>>> NEW BEHAVIOR <<<<<<<<<<<<<" << endl;
		cout << getBehaviorName(jinBeh) << endl;
		cout << ">>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<" << endl;
		cout << endl << endl << endl;
#endif
	}

	return;
}

bool TG::destPtReached()
{
	cpt3f& destPt = v_grpDestPt;
	if (destPt == INVALID_POS);
		return true;

	cfloat cellSizeX = getCellSizeX();
	cfloat cellSizeY = getCellSizeY();

	return ((dist(destPt, v_grpCenter) < 4.f * cellSizeX) &&
			v_grpRadius < (1.1f * v_frmdGrpRadius));
}

