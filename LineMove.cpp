#include "common.h"

#undef __TRACE_ME

template <class T>
	LineMove<T>::LineMove(T* obj,
						  const pt3f& a_posFrom, 
						  const vec3f& a_dir, 
						  const float a_rng,
						  const bool a_bCollisions,
						  const float a_speed) :
	v_posFrom(a_posFrom), v_rng(a_rng),
	v_bCollisions(a_bCollisions), v_speed(a_speed)
{
	assert(INVALID_POS != v_posFrom);
	assert(INVALID_VEC != v_dir);
	assert(v_rng > 0.f);
	assert(v_speed > 0.f);
	assert(obj != (T*)0);

	v_dir = a_dir.normalize();

	this->movObj = obj;

	init();
}

template <class T>
	LineMove<T>::LineMove(T* obj,
						  const pt3f& a_posFrom, 
						  const pt3f& a_posTo, 
						  bool a_bCollisions,
						  const float a_speed) :
	v_posFrom(a_posFrom), 
	v_bCollisions(a_bCollisions), v_speed(a_speed)
{
	assert(INVALID_POS != v_posFrom);
	assert(INVALID_POS != a_posTo);
	assert(v_speed > 0.f);
	assert(obj != (T*)0);

	v_dir = (a_posTo - v_posFrom).normalize();
	v_rng = (a_posTo - v_posFrom).length();
	assert(v_rng > 0.f);

	this->movObj = obj;

	init();
}

template <class T>
	void LineMove<T>::init()
{
	v_chckdCollsPos = INVALID_POS;
	v_pos 			= v_posFrom;

	v_ended			= false;
}

template <class T>
	void LineMove<T>::m_update()
{
	if (v_ended)
		return;

	const float fps = BasicConfiguration::getFPS();
	assert(fps > 0.f);

	cfloat cellSizeX = getCellSizeX();
	cfloat cellSizeY = getCellSizeY();

	float delta = (1.f / fps) * cellSizeX * v_speed;

	cpt3f oldPos = v_pos;
	pt3f newPos = v_pos + v_dir * delta;

#ifdef __TRACE_ME
	cout << "New position: " << newPos << endl;
    cout << "Delta: " << delta << endl; 
#endif

	if (dist(newPos, v_posFrom) > v_rng)
	{
        v_ended = true;
        return;
	}
		
	if (posOutsideGrid(newPos)) // (1154.43, 1554.19, 0)
	{
		v_ended = true;
        return;
	}

    if (!v_bCollisions)
	{
		v_pos = newPos;
		return;
	}

	BasicObject* pTObj = getTouchedObj(oldPos, newPos);
	if ((BasicObject*)0 != pTObj)
	{
		//pTObj->touch(movObj);
		v_pos = pTObj->m_getPos();
		v_ended = true;
		return;
	}

	v_pos = newPos;
	return;
}

template <class T> 
	BasicObject* LineMove<T>::getTouchedObj(cpt3f& a_stPos, cpt3f& a_endPos)
{
	cfloat cellSizeX = getCellSizeX();
	cfloat cellSizeY = getCellSizeY();
	cfloat COLLISION_THRESHOLD = (cellSizeX / 2.f);

	if ((INVALID_POS == a_stPos) || (INVALID_POS == a_endPos))
		return (BasicObject*)0;

	INT_COORDS stCell 	= getCellForPos(a_stPos);
	INT_COORDS endCell	= getCellForPos(a_endPos);

	if ((INVALID_COORDS == stCell) || (INVALID_COORDS == endCell))
		return (BasicObject*)0;

	if (dist(a_stPos,  v_posFrom) < 2.1f * COLLISION_THRESHOLD || 
		dist(a_endPos, v_posFrom) < 2.1f * COLLISION_THRESHOLD)
		return (BasicObject*)0;

	cvec3f vecSt2End 	= (a_endPos - a_stPos);

	cint minX = min(stCell.first, endCell.first);
	cint minY = min(stCell.second, endCell.second);
	cint maxX = max(stCell.first, endCell.first); 
	cint maxY = max(stCell.second, endCell.second);

	BasicObject* movBObj = dynamic_cast<BasicObject*>(movObj);

	vector<BasicObject*> vChosenObjs;
	int x, y;
	for (x = minX; x <= maxX; ++x)
	{
		for (y = minY; y <= maxY; ++y)
		{
			INT_COORDS cell = mp(x, y);
			if (cell == stCell)
			{
				//continue;
			}
 /*             P 
             O /|
			  / |
			 /	| 
			/\__|_________
			S	P1       E   */
			cpt3f cellCC = getCellCenterCoords(cell);
			cvec3f vecSt2P 	= (cellCC - a_stPos);	

			float angle, sign;
			vecSt2End.getAngle(vecSt2P, angle, sign);

			float PP1 = fabsf(vecSt2P.length() * sinf(angle));
			//if (PP1 < 8.f * COLLISION_THRESHOLD)
			{
				GridElement* pElem = getGridElement(cell);
				if (pElem && pElem->m_isCoveredByObject())
				{
					BasicObject* pObj = pElem->m_getCoveringObject();
					if (pObj == movBObj)
					{
						//continue;
					}
                    cpt3f objPos = pObj->m_getPos();

					cvec3f vecSt2O = (objPos - a_stPos);	

					float angle, sign;
					vecSt2End.getAngle(vecSt2O, angle, sign);

					float OO1 = fabsf(vecSt2O.length() * sinf(angle));

					if (OO1 < 2.5f * COLLISION_THRESHOLD)
					{
						vChosenObjs.push_back(pObj);
					}
				}
			}
		}
	} /* for */

	BasicObject* bestChoseObj = (BasicObject*)0;
	float distChose2St = +FLOAT_MAX;
	int i;
	for (i = 0; i < vChosenObjs.size(); ++i)
	{
		BasicObject*& pObj = vChosenObjs[i];
		cfloat dist2St = dist(pObj->m_getPos(), a_stPos);
		if (dist2St < distChose2St)
		{
			distChose2St = dist2St;
			bestChoseObj = pObj;
		}
	}

	return bestChoseObj;
}

template LineMove<BasicObject>;
template LineMove<Bullet>;

