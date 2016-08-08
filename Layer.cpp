#include "common.h"

map<int /* level */, Layer*> Layer::v_mapLevelsToLayers;


Layer::~Layer()
{
	vector<BasicObject*>::iterator itOO;
	for (itOO = v_vObjects.begin(); itOO != v_vObjects.end(); ++itOO)
	{
		BasicObject* pObj = *itOO;
		delete pObj;
		*itOO = 0;
	}
	v_vObjects.clear();
	vector<Bullet*>::iterator itBB;
	for (itBB = v_vBullets.begin(); itBB != v_vBullets.end(); ++itBB)
	{
		Bullet* pBullet = *itBB;
		delete pBullet;
		*itBB = 0;
	}
	v_vBullets.clear();
	vector<Gadget*>::iterator itGG;
	for (itGG = v_vGadgets.begin(); itGG != v_vGadgets.end(); ++itGG)
	{
		Gadget* pGdg = *itGG;
		delete pGdg;
		*itGG = 0;
	}
}

uint Layer::m_addObject(BasicObject* a_pObj)
{
	v_vObjects.push_back(a_pObj);
	return a_pObj->getIdx(); 
}

BasicObject* Layer::m_getObjWithId(const uint a_nId)
{
	UNITS_VEC::const_iterator itUU = find_if(v_vObjects.begin(),
											 v_vObjects.end(),
											 equal_obj_by_idx(a_nId));

	if (itUU != v_vObjects.end())
		return *itUU;
	
	return (BasicObject*)0;
}

uint Layer::m_addBullet(Bullet* a_pBullet)
{
	v_vBullets.push_back(a_pBullet);
    return a_pBullet->getIdx();
}

Bullet* Layer::m_getBulletWithId(const uint a_nId)
{
	vector<Bullet*>::const_iterator itBB = find_if(v_vBullets.begin(),
												   v_vBullets.end(),
												   equal_obj_by_idx(a_nId));

	if (itBB != v_vBullets.end())
		return *itBB;

	return (Bullet*)0;
}

uint Layer::m_addGadget(Gadget* a_pGdg)
{
	v_vGadgets.push_back(a_pGdg);
	return a_pGdg->getIdx();
}

Gadget* Layer::m_getGadgetWithId(const uint a_nId)
{
	vector<Gadget*>::const_iterator itGG = find_if(v_vGadgets.begin(),
												   v_vGadgets.end(),
												   equal_obj_by_idx(a_nId));

	if (itGG != v_vGadgets.end())
		return *itGG;

	return (Gadget*)0;
}

void Layer::m_applyToGrid(MapGrid* a_pGrid, int a_nLayerLevel)
{
	vector<BasicObject*>::iterator it;
	for (it = v_vObjects.begin(); it != v_vObjects.end(); ++it)
	{
		BasicObject* pObj = *it;
		pObj->m_setLayerLevel(a_nLayerLevel);
		pObj->m_applyToGrid(a_pGrid, a_nLayerLevel);
	}
}

void Layer::m_draw(int a_nUserId, BITMAP* a_pDestBmp)
{
    vector<Gadget*>::iterator itGG; // ?????????????
	for (itGG = v_vGadgets.begin(); itGG != v_vGadgets.end(); ++itGG)
	{
		Gadget* pGdg = *itGG;
		pGdg->m_draw(a_nUserId, a_pDestBmp);
	}

	vector<BasicObject*>::iterator itOO;
	for (itOO = v_vObjects.begin(); itOO != v_vObjects.end(); ++itOO)
	{
		BasicObject* pObj = *itOO;
		pObj->m_draw(a_nUserId, a_pDestBmp);
	}

	vector<Bullet*>::iterator itBB;
	for (itBB = v_vBullets.begin(); itBB != v_vBullets.end(); ++itBB)
	{
		Bullet* pBullet = *itBB;
		pBullet->m_draw(a_nUserId, a_pDestBmp);
	}
}

void Layer::m_update()
{
	UNITS_VEC::iterator itOO;
	for (itOO = v_vObjects.begin(); itOO != v_vObjects.end();)
	{
		BasicObject* pObj = *itOO;
		pObj->m_update();
        if (!pObj->m_exists())
		{
            itOO = v_vObjects.erase(itOO);
			pObj->delObjRef();
			delete pObj;
		}
		else
			++itOO;
	}

	vector<Bullet*>::iterator itBB;
	for (itBB = v_vBullets.begin(); itBB != v_vBullets.end();)
	{
		Bullet* pBullet = *itBB;
		pBullet->m_update();
		if (!pBullet->m_exists())
		{
			delete pBullet;
            itBB = v_vBullets.erase(itBB);
		}
		else
			itBB++;
	}

	vector<Gadget*>::iterator itGG;
	for (itGG = v_vGadgets.begin(); itGG != v_vGadgets.end();)
	{
		Gadget* pGdg = *itGG;
		pGdg->m_update();
		if (!pGdg->m_exists())
		{
			delete pGdg;
            itGG = v_vGadgets.erase(itGG);
		}
		else
			itGG++;
	}
}

const Layer* Layer::m_getLayerAtLevel(const int a_nLevel)
{
	LAYER_MAP::iterator itLL = v_mapLevelsToLayers.find(a_nLevel);
	if (v_mapLevelsToLayers.end() == itLL)
	{
		Layer* pLayer = new Layer(a_nLevel);
		v_mapLevelsToLayers[a_nLevel] = pLayer;
		return pLayer;
	}

	return (*itLL).second;
}

