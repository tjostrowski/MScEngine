#include "common.h"

BulletClass G_DEF_BULLET_CLASS	= BulletClass(1, 50.f /* force */, 5.f /* speed in cells*/, 1000.f /* reach in cells */, G_DEF_FLY_TXT_ID, 0.f); 
BulletClass G_DEF2_BULLET_CLASS	= BulletClass(1, 50.f /* force */, 5.f /* speed in cells */, 1000.f /* reach in cells */, G_DEF2_FLY_TXT_ID, 0.f);
BulletClass G_BOMB_BULLET_CLASS	= BulletClass(3, 100.f /* force */, 10.f /* speed in cells */, 1000.f /* reach in cells */, G_BOMB_TXT_ID, 20.f, G_BOMB_EXPL_TXT_ID); 


const uint Bullet::NORMAL_FLY_ACTIVITY	= 1;
const uint Bullet::EXPLOSION_ACTIVITY	= (1 << 1);


// BasicObjectParamsManager
BasicObjectParamsManager::BasicObjectParamsManager(BasicObject* a_pObj) :
	v_pObj(a_pObj)
{
	m_initDefaultParams();
}

void BasicObjectParamsManager::m_initDefaultParams()
{
	health		= FParam(0.f, 100.f, 100.f);
	stamina		= FParam(0.f, 100.f, 100.f);

	cfloat cellSizeX = getCellSizeX();
	cfloat cellSizeY = getCellSizeY();

	fovRAD = FParam(0.f, (10.f * cellSizeX), (10.f * cellSizeX));		
	fovANG = FParam(0.f, 60.f, 60.f);
}


// Bullet
Bullet::Bullet(LineMove<Bullet>* a_pLM,
			   BulletClass* a_pClass) :
	v_pLM(a_pLM)
{
    assert((BulletClass*)0 != a_pClass);
	v_pClass = a_pClass;

	m_initDefaultParams();
}

Bullet::Bullet(cpt3f& a_startPos,
			   cpt3f& a_endPos, 
			   BulletClass* a_pClass,
			   bool a_bCollide)
{
    assert((BulletClass*)0 != a_pClass);
	v_pClass = a_pClass;

	const float cellSizeX = getCellSizeX();

	vec3f vec2End = (a_endPos - a_startPos);
	const float dist2End = vec2End.length();
	cpt3f endPos = a_startPos + vec2End.normalize() * min(v_pClass->reach * cellSizeX, dist2End);

#undef __TRACE_ME
#ifdef __TRACE_ME
	cout << "START POS: " << a_startPos << endl;
	cout << "END POS: " << endPos << endl;
#endif

    v_pLM = new LineMove<Bullet>(this,
								 a_startPos,
								 endPos, 
								 a_bCollide /* collisions */,
								 v_pClass->speed);

	m_initDefaultParams();
}

Bullet::Bullet(cpt3f& a_startPos,
			   cvec3f& a_dir,
			   bool a_useDir,
			   BulletClass* a_pClass)
{
	assert((BulletClass*)0 != a_pClass);
	v_pClass = a_pClass;

	const float cellSizeX = getCellSizeX();

	assert(v_pClass->reach > 0.f);

	v_pLM = new LineMove<Bullet>(this,
								 a_startPos,
								 a_dir,
								 v_pClass->reach * cellSizeX,
								 true,
								 v_pClass->speed);

	m_initDefaultParams();
}

void Bullet::m_initDefaultParams()
{
	activity = NORMAL_FLY_ACTIVITY;
	exists = true;

	TextureObject txtObj;
	bool txtFound = makeTextureObj(v_pClass->flyTxtId, txtObj);
	if (txtFound)
		v_mapActivities2TxtObjs.insert(make_pair(NORMAL_FLY_ACTIVITY, txtObj));

	shouldExplode = false;
}

void Bullet::m_update()
{
	assert(v_pLM != (LineMove<Bullet>*)0);

	v_pLM->m_update();

#ifdef __TRACE_ME
	cout << *this << " " << m_getPos() << endl;
#endif

	if (v_pLM->ended())
	{
		if (!shouldExplode)
		{
			BulletClass* pClass = v_pClass;
			assert((BulletClass*)0 != pClass);

			cpt3f endLMPos = v_pLM->m_getPos();
			assert(INVALID_POS != endLMPos);
	
			INT_COORDS ctrCELL = getCellForPos(endLMPos);
			assert(INVALID_COORDS != ctrCELL);

			INT_COORDS_VEC cells;
			getDiffs(ctrCELL, (uint)(pClass->explReach), cells);
			cells.push_back(ctrCELL);

			int i;
			for (i = 0; i < cells.size(); ++i)
			{
				INT_COORDS& cell = cells[i];
				GridElement* pEl = getGridElement(cell);
				if (((GridElement*)0 != pEl) && (pEl->m_isCoveredByObject()))
				{
					BasicObject* pObj = pEl->m_getCoveringObject();
					pObj->touch(v_pLM->m_getMovObj());
				}
			}

			shouldExplode = true;
		}
		else
		{
			exists = false;
		}      
	}
}

cpt3f Bullet::m_getPos()
{	
	assert((LineMove<Bullet>*)0 != v_pLM);

	return v_pLM->m_getPos();
}

BITMAP* Bullet::m_getCurrentAnimationFrame()
{
	assert(v_mapActivities2TxtObjs.size() >= 1);

	TextureObject& txtObj = v_mapActivities2TxtObjs[NORMAL_FLY_ACTIVITY];
	return txtObj.m_getActualBitmap();
}

void Bullet::m_draw(int a_nUserId, BITMAP* a_pDestBmp)
{
    BITMAP* pFrame = m_getCurrentAnimationFrame();
	assert((BITMAP*)0 != pFrame);

	drawBmp(a_nUserId,
			a_pDestBmp,
			pFrame,
			m_getPos(), 
			.25f /* width in cells */,
			.25f /* height in cells */,
			true,
			true);

	if (shouldExplode)
	{
		BulletClass* pClass = v_pClass;
		assert((BulletClass*)0 != pClass);

		uint explTxtId = pClass->explTxtId;
		if (explTxtId != (uint)UNDEFINED)
		{
			TextureObject txtObj;
			bool txtMade = makeTextureObj(explTxtId, txtObj);
			if (txtMade)
			{
				BITMAP* bmp = txtObj.m_getActualBitmap();
				drawBmp(a_nUserId,
						a_pDestBmp,
						bmp,
						v_pLM->m_getPos(),
						bmp->w,
						bmp->h,
						false,
						true);
			}
		}
	}

	return;
}

ostream& operator<<(ostream& os, const Bullet& bu)
{
	os << typeid(bu).name() << " , id: " << bu.getIdx();
}

