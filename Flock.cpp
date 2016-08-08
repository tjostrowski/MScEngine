#include "common.h"

Flock::Flock(BasicObject* a_pObj, cpt3f& a_trgtPos) :
	v_trgtPt(a_trgtPos)
{
	assert((BasicObject*)0 != a_pObj);

	v_pObj = a_pObj;

	v_velHistory.clear();

	m_init();
}

Flock::~Flock()
{
}

void Flock::m_init()
{
	v_estVel = INVALID_VEC;

	v_stopped = false;
}

void Flock::reset()
{
	v_velHistory.clear();
}

cpt3f Flock::m_getNewPosForObj()
{
	vec3f vec = v_pObj->m_getVel(); 
	v_velHistory.push_back(vec);

	cfloat cellSizeX = getCellSizeX();
	cfloat cellSizeY = getCellSizeY();

	cfloat THRESHOLD = (cellSizeX / 2.f);
	cpt3f objPos = v_pObj->m_getPos();
	
	if (INVALID_POS == v_trgtPt || dist(objPos, v_trgtPt) < THRESHOLD)
	{
		v_estVel = ZERO_VEC;
		return objPos;
	}

	TG* ctrlTG;
	bool ctrledByTG = v_pObj->m_isControlledByTG(&ctrlTG);

	UNITS_VEC friendUnits; // other as obstacles
	if (ctrledByTG)
	{
		assert((TG*)0 != ctrlTG);
		ctrlTG->m_getObjs(friendUnits);
	}
	
	PL_OBJS_VEC nearPlObjs;
	int numNear = m_findNearPlObjs(nearPlObjs);

    if (numNear == 0)
	{
		cvec3f vec = m_getTrgtVec();
		cpt3f newPos = objPos + vec;

		v_estVel = newPos - objPos;

		return newPos;
	}

	UNITS_VEC nearFriends;
	UNITS_VEC nearEnems;
	int i;
	for (i = 0; i < numNear; ++i)
	{
		PlaceableObj*& pPlObj = nearPlObjs[i];
		BasicObject* pObj = dynamic_cast<BasicObject*>(pPlObj);
		if ((BasicObject*)0 == pObj) continue;
		bool isFriend = (friendUnits.end() != find(friendUnits.begin(), friendUnits.end(), pObj));
		if (isFriend)
		{
			nearFriends.push_back(pObj);
		}
		else
		{
			nearEnems.push_back(pObj);
		}
	}

	cout << ">>>> " << nearFriends.size() << " " << nearEnems.size() << endl;

	cfloat trgtF = .8f;
	vec3f trgtVec = m_getTrgtVec();

	// 1-sza regula : predkosc dostosowujemy do predkosci sasiednich boidow
	vec3f nearVelVec = ZERO_VEC;
	m_getNearVelVec(nearFriends, vec, nearVelVec);
	cfloat nvvF = .1f;
	cout << ">>>>>>>>>>>>> NVV: " << nearVelVec << endl;

	// 2-ga regula : el. w srodku grupy sasiednich boidow
	vec3f nearMidVec = ZERO_VEC;
	m_getNearMidVec(nearFriends, vec, nearMidVec);
	cfloat nmvF = .1f;
	cout << ">>>>>>>>>>>>> NMV: " << nearVelVec << endl;

	// 3 regula : odpychanie od wszystkich
	vec3f repVec = ZERO_VEC;
	m_getRepVec(nearPlObjs, vec, repVec);
	cfloat repF = .2f;
	cout << ">>>>>>>>>>>>> RPV: " << repVec << endl;

	cout << "<<<<" << endl;

	if (v_stopped)
	{
		trgtVec = nearVelVec = nearMidVec = ZERO_VEC;
		repVec = ZERO_VEC; 
	}

	draw_line(DEFAULT_USER_ID, pBuffer, objPos, trgtVec, makecol(255, 0, 0));
	draw_line(DEFAULT_USER_ID, pBuffer, objPos, nearVelVec, makecol(0, 255, 0));
	draw_line(DEFAULT_USER_ID, pBuffer, objPos, nearMidVec, makecol(0, 0, 255));
	draw_line(DEFAULT_USER_ID, pBuffer, objPos, repVec, makecol(0, 255, 255));

	vec = (trgtVec * trgtF) + (nearVelVec * nvvF) + (nearMidVec * nmvF) + (repVec * repF); 

	cpt3f newPos = objPos + vec;

	v_estVel = newPos - objPos;

	return newPos;
}

int Flock::m_findNearPlObjs(PL_OBJS_VEC& a_vNearPlObjs)
{
	a_vNearPlObjs.clear();

	cpt3f objPos = v_pObj->m_getPos();
	INT_COORDS objCell = getCellForPos(objPos);

	//assert(objCell != INVALID_COORDS);
	if (objCell == INVALID_COORDS)
	{
		return 0;
	}

	vec3f vec = v_pObj->m_getVel();
    
	if (ZERO_VEC == vec || INVALID_VEC == vec)
	{
		if (INVALID_POS == v_trgtPt)
			vec = cvec3f(1.f, 0.f, 0.f);
		else
			vec = (v_trgtPt - objPos);
	}

	cvec3f dir = vec.normalize();

	cfloat cellSizeX = getCellSizeX();
	cfloat cellSizeY = getCellSizeY();

	float fovRAD, fovANG_D;
	v_pObj->m_getFOV(fovRAD, fovANG_D);

	assert(fovRAD > 0.f && fovANG_D > 0.f);

	const uint numDiffs = max((uint)(fovRAD / cellSizeX), (uint)1);
	INT_COORDS_VEC nearCells;
	getDiffs(objCell, numDiffs, nearCells);

	vector<PlaceableObj*> vNearPlObjs;
	int i;
	for (i = 0; i < nearCells.size(); ++i)
	{
		INT_COORDS cell = nearCells[i];
		cpt3f ccPos = getCellCenterCoords(cell); // 'c'ell 'c'enter 'pos'
		cvec3f dir2Pos = (ccPos - objPos).normalize();

		float angleR, sign;
		dir2Pos.getAngle(dir, angleR, sign);

		if (angleR < DEG2RAD(fovANG_D))
		{
			GridElement* pGE = getGridElement(cell);
			if (pGE->m_isCovered())
			{
				PlaceableObj* pPlObj = pGE->m_getCoveringPlObj();
				bool fnd = ((v_pObj != pPlObj) && (vNearPlObjs.end() == find(vNearPlObjs.begin(), vNearPlObjs.end(), pPlObj)));

				if (fnd)
				{
					vNearPlObjs.push_back(pPlObj);
				}
			}
		}
	} /* for */

	a_vNearPlObjs = vNearPlObjs;

	return a_vNearPlObjs.size();
}

cvec3f Flock::m_getTrgtVec()
{
	cpt3f objPos = v_pObj->m_getPos();
	cfloat refSpeed = v_pObj->getRefSpeed();

	if (INVALID_POS == v_trgtPt)
		return ZERO_VEC;

	cvec3f trgtVec = (v_trgtPt - objPos).normalize();

	return (trgtVec * refSpeed);
}

cvec3f Flock::m_getNearVelVec(UNITS_VEC& a_units, cvec3f& a_vel, vec3f& nearVelVec)
{
	if (a_units.size() == 0)
		return ZERO_VEC;

	vec3f vec = ZERO_VEC;

	int i;
	for (i = 0; i < a_units.size(); ++i)
	{
		BasicObject* pObj = a_units[i];
		cvec3f objVel = pObj->m_getVel();

		vec += objVel;
	}

	vec /= (float)(a_units.size());
	nearVelVec = vec;

	return vec;
}

cvec3f Flock::m_getNearMidVec(UNITS_VEC& a_units, cvec3f& a_vel, vec3f& nearMidVec)
{
	if (a_units.size() == 0)
		return ZERO_VEC;

	cpt3f thisPos = v_pObj->m_getPos();
	cfloat refSpeed = v_pObj->getRefSpeed();
	float medDist = 0.f;

	int i;
	for (i = 0; i < a_units.size(); ++i)
	{
		BasicObject* pObj = a_units[i];
		cpt3f objPos = pObj->m_getPos();
		cfloat dist2Obj = dist(thisPos, objPos);

		medDist += dist2Obj;
	}

	medDist /= a_units.size();
	assert(medDist > 0.f);

	vec3f vec = ZERO_VEC;

	for (i = 0; i < a_units.size(); ++i)
	{
		BasicObject* pObj = a_units[i];
		cpt3f objPos = pObj->m_getPos();
		cfloat dist2Obj = dist(thisPos, objPos);

		cvec3f vec2Obj = (objPos - thisPos).normalize();

		vec += vec2Obj * ((dist2Obj - medDist) / medDist) * (refSpeed / 2.f);
	}

	nearMidVec = vec;

	return vec;
}

cvec3f Flock::m_getRepVec(PL_OBJS_VEC& a_vPlObjs, cvec3f& a_vel, vec3f& repVec)
{
	if (a_vPlObjs.size() == 0)
		return ZERO_VEC;

	cfloat cellSizeX = getCellSizeX();
	cfloat cellSizeY = getCellSizeY();

	cfloat REP_THRESHOLD = 4.f * cellSizeX;
	cpt3f thisPos = v_pObj->m_getPos();
	cfloat refSpeed = v_pObj->getRefSpeed();

	vec3f vec = ZERO_VEC;

	int i;
	for (i = 0; i < a_vPlObjs.size(); ++i)
	{
		PlaceableObj* pPlObj = a_vPlObjs[i];
		cpt3f objPos = pPlObj->m_getPos();
		cfloat dist2Obj = dist(objPos, thisPos);
		assert(dist2Obj > 0.f);

		cvec3f vec2Obj = (objPos - thisPos).normalize();

		if (dist2Obj < REP_THRESHOLD)
		{
			cfloat objWidth		= pPlObj->m_getWidth();
			cfloat objHeight 	= pPlObj->m_getHeight();
			vec -= vec2Obj * clampf((REP_THRESHOLD / dist2Obj) * (objWidth + objHeight) * .5f, 1.f, 4.f) * (refSpeed * .5f); // '-' => bo odbijanie
		}
	}

	repVec = vec;

	return vec;
}

vec3f Flock::m_getLastVel()
{
	if (v_velHistory.empty())
		return INVALID_VEC;

	return v_velHistory.back();
}

vec3f Flock::m_getAveVel()
{
	cuint szHistory = v_velHistory.size();
	if (szHistory == 0)
		return INVALID_VEC;

	vec3f aveVel = ZERO_VEC;

	int i;
	for (i = 0; i < szHistory; ++i)
	{
		aveVel += v_velHistory[i];
	}

	aveVel /= ((float)szHistory);

	return aveVel; 
}

