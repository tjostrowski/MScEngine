#include "common.h"

#define IN_AREA(__pos, __area) (dist(__pos, __area.first) < __area.second) 

TGManager* TGManager::v_pManager = 0;


TGManager* TGManager::m_getTGManager()
{
	if ((TGManager*)0 == v_pManager)
		v_pManager = new TGManager();

	return v_pManager;
}

TGManager::TGManager()
{
	m_init();
}

TGManager::~TGManager()
{


}

bool TGManager::m_init()
{
	return true;
}

TacticalGroup* TGManager::m_getTGWithId(const uint& a_nId)
{
	TACTICGRP_VEC::const_iterator itTG = find_if(v_vTGRps.begin(),
												 v_vTGRps.end(),
												 equal_obj_by_idx(a_nId));

	if (v_vTGRps.end() != itTG)
		return *itTG;

	return (TacticalGroup*)0;
}

TG* TGManager::m_createTG(const uint& a_assign)
{
	TG* pTG = new TG(a_assign, this);
	v_vTGRps.push_back(pTG);

#ifdef __TRACE_ME
	cout << "TGs num: " << v_vTGRps.size() << endl;
#endif

	return pTG;
}

bool TGManager::m_removeTG(const uint& a_nId)
{
	TacticalGroup* pTG = m_getTGWithId(a_nId);
	bool fndTG = ((TacticalGroup*)0 != pTG);

	if (fndTG)
	{	
		TACTICGRP_VEC::iterator it = find(v_vTGRps.begin(),
										  v_vTGRps.end(),
										  pTG);
		v_vTGRps.erase(it);
		delete pTG;

		return true;
	}

	return false;
}

void TGManager::m_update()
{
	int i;
	for (i = 0; i < v_vTGRps.size(); ++i)
	{
		TacticalGroup*& tg = v_vTGRps[i];
		tg->m_update();
	}
}

void TGManager::m_draw(int a_nUserId, BITMAP* a_pDestBmp)
{
	int i;
	for (i = 0; i < v_vTGRps.size(); ++i)
	{
		TacticalGroup*& tg = v_vTGRps[i];
		tg->m_draw(a_nUserId, a_pDestBmp);
	}
}

void TGManager::m_createTacticalGroups(const uint& a_nAssign)
{
    for (int i = 0; i < v_vTGRps.size(); ++i)
	{
		v_vTGRps[i] = (TacticalGroup*)0;
	}
	v_vTGRps.empty();

	Layer* pLayer = 
		const_cast<Layer*>(Layer::m_getLayerAtLevel(DEFAULT_LAYER_LEVEL));
	UNITS_VEC vObjs;
    pLayer->m_getObjsAssigned(a_nAssign, vObjs);

	// fronts :
	m_divideToTGs(a_nAssign, v_mapAreaDescrs[(uint)E_TG_FRONT], vObjs, E_TG_MAX_SZ_SCAL_2);
	// wings :
	m_divideToTGs(a_nAssign, v_mapAreaDescrs[(uint)E_TG_WING], vObjs, E_TG_MAX_SZ_SCAL_1);
	// backs :
	m_divideToTGs(a_nAssign, v_mapAreaDescrs[(uint)E_TG_BACK], vObjs, E_TG_MAX_SZ_SCAL_0);

	return;
}

void TGManager::m_divideToTGs(const uint a_nAssign, const vector<AREA>& a_vAreas, UNITS_VEC& a_vObjs, float scal)
{
	/*const vector<AREA>&	vAreas	= a_vAreas; 
	UNITS_VEC&		vObjs 		= a_vObjs;

	const float cellSizeX = getCellSizeX();
	const float cellSizeY = getCellSizeY();
    const float maxDistForTGUs = TG_MAX_DIST_BETWEEN_TG_UNITS * cellSizeX;

    vector<AREA>::const_iterator itFF;
	for (int i = 0; i < vAreas.size(); ++i)
	{
		const AREA& area = vAreas[i];
		UNITS_VEC vObjsInThisArea;
		UNITS_VEC::iterator itUU;
		for (itUU = vObjs.begin(); itUU != vObjs.end(); )
		{
			BasicObject* pObj = (*itUU);
			assert((BasicObject*)0 != pObj);
			pt3f pos = getCellCenterCoords(pObj->m_getPosition());
			if (IN_AREA(pos, area))
			{
				vObjsInThisArea.push_back(pObj);
				// UWAGA: !!!!!
				itUU = vObjs.erase(itUU);	
			}
			else
				++itUU;
		}
		assert(vObjsInThisArea.size() >= 1);
#ifdef __TRACE_ME
		cout << " Objs in area: " << endl  
			 << " X: " << area.first.x << " Y: " << area.first.y << " Z: " << area.first.z << endl
			 << " R: " << area.second << endl 
			 << vObjsInThisArea.size() << endl << endl;
#endif
        vector<BasicObject*> vUnassignedObjs = vObjsInThisArea; 
		uint seedIdx = 0;
		while (!vUnassignedObjs.empty() && seedIdx < vUnassignedObjs.size())
		{
			BasicObject* seedObj = vUnassignedObjs[seedIdx];
			pt3f seedPos = getCellCenterCoords(seedObj->m_getPosition());
			UNITS_VEC vPotentTG;
			vPotentTG.push_back(seedObj);
			for (int j = 0; j < vUnassignedObjs.size(); ++j)
			{
				BasicObject* pObj = vUnassignedObjs[j];
				if (pObj == seedObj) continue;
				pt3f pos = getCellCenterCoords(pObj->m_getPosition());
				if (IN_AREA(pos, make_pair(seedPos, maxDistForTGUs)))
					vPotentTG.push_back(pObj);
			}

			const uint potTGSz = vPotentTG.size();
			if (potTGSz >= (E_TG_MIN_TG_SIZE * scal) &&
				potTGSz <= (E_TG_MAX_TG_SIZE * scal))
			{
				UNITS_VEC::iterator itUU;
				for (itUU = vUnassignedObjs.begin(); itUU != vUnassignedObjs.end(); )
				{
					BasicObject* pObj = (*itUU);
					if (vec_contains<BasicObject*>(vPotentTG, pObj))
						itUU = vUnassignedObjs.erase(itUU);
					else
						++itUU;
				}

				TacticalGroup* newGrp = new TacticalGroup(a_nAssign, vPotentTG);
				v_vTGRps.push_back(newGrp);

#ifdef __TRACE_ME
				cout << "New tactical group: " << endl
					 << vPotentTG.size() << endl;
#endif
				seedIdx = 0;
			}
			else
				seedIdx++;
		}  // end while
	}*/
}

void TGManager::m_findStruct(const uint& a_nAssignment)
{
	vector<AREA> vFronts;
	m_findFronts(a_nAssignment, vFronts);
	vector<AREA> vBacks;
	m_findFronts(a_nAssignment, vBacks);
	vector<AREA> vWings;
	m_findWings(a_nAssignment, vWings);
	
	v_mapAreaDescrs.insert(make_pair((uint)E_TG_FRONT, vFronts));
	v_mapAreaDescrs.insert(make_pair((uint)E_TG_BACK, vBacks));
	v_mapAreaDescrs.insert(make_pair((uint)E_TG_WING, vWings));
}

void TGManager::m_findFronts(const uint& a_nAssignment, vector<AREA>& a_vec)
{
	// TODO:

}

void TGManager::m_findBacks(const uint& a_nAssignment, vector<AREA>& a_vec)
{
	// TODO:

}

void TGManager::m_findWings(const uint& a_nAssignment, vector<AREA>& a_vec)
{
	// TODO:

}

TG* TGManager::m_getTGNearestPt(cpt3f& a_pt, cuint& a_assign)
{
	TG* foundTG = (TG*)0;

	cfloat cellSizeX = getCellSizeX();
	cfloat tolerance = (cellSizeX / 2.f);

	for (int i = 0; i < v_vTGRps.size(); ++i)
	{
		TG*& pTG = v_vTGRps[i];
		cpt3f grpCenter = pTG->m_getGroupCenter(); 
		cfloat grpRadius = pTG->m_getGroupRadius();

        if ((pTG->m_getAssign() == a_assign || a_assign == (uint)UNDEFINED)
			 && dist(grpCenter, a_pt) <= (grpRadius + tolerance))
		{
            foundTG = pTG;
			break;
		}
	}

	return foundTG;
}

bool TGManager::delObjRef(BasicObject* a_pObj)
{
	int i;
	for (i = 0; i < v_vTGRps.size(); ++i)
	{
		TG* pTG = v_vTGRps[i];
		if (pTG->delObjRef(a_pObj))
		{
			return true;
		}
	}

	return false;
}

