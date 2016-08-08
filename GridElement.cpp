#include "common.h"

const int GridElement::ELEMENT_GAME_OBJECT	= 0;


GridElement::GridElement() :
	v_coords(INVALID_COORDS)
{
	v_vCovObjs.clear();
}

void GridElement::m_addLayerObject(BasicObject* a_pObj, int a_nLayerNumber)
{
	GridElementListElement el;
	el.v_pObj 			= a_pObj;
	el.v_nType 			= ELEMENT_GAME_OBJECT;
	el.v_nLayerNumber 	= a_nLayerNumber;
	v_vElementLayers.push_back(el);
}

BasicObject* GridElement::m_getObjectAtLayer(int a_nLayerNumber)
{
    vector<GridElementListElement>::iterator it = 
		find_if(v_vElementLayers.begin(), v_vElementLayers.end(), 
				equal_layer_number(a_nLayerNumber));   
	if (it != v_vElementLayers.end())
	{
		GridElementListElement el = *it;
		return el.v_pObj;
	}

	return 0;
}

void GridElement::removeLayer(int a_nLayerNumber)
{
	vector<GridElementListElement>::iterator it =
		find_if(v_vElementLayers.begin(), v_vElementLayers.end(),
				equal_layer_number(a_nLayerNumber));
	if(it != v_vElementLayers.end())
	{
		v_vElementLayers.erase(it);
	}
}

void GridElement::clearLayers()
{
    v_vElementLayers.clear();
}

bool GridElement::m_isCoveredByObject()
{
	int i;
	for (i = 0; i < v_vCovObjs.size(); ++i)
	{
		PlaceableObj*& pPlObj = v_vCovObjs[i]; 
		BasicObject* pObj = dynamic_cast<BasicObject*>(pPlObj);
		if ((BasicObject*)0 != pObj)
			return true;
	}

	return false;
}

bool GridElement::m_isCoveredByGadget()
{
	int i;
	for (i = 0; i < v_vCovObjs.size(); ++i)
	{
		PlaceableObj*& pPlObj = v_vCovObjs[i]; 
		Gadget* pGdg = dynamic_cast<Gadget*>(pPlObj);
		if ((Gadget*)0 != pGdg)
			return true;
	}

	return false;
}

bool GridElement::m_isCovered()
{
	return (v_vCovObjs.size() > 0);
}

BasicObject* GridElement::m_getCoveringObject()
{
	int i;
	for (i = 0; i < v_vCovObjs.size(); ++i)
	{
		PlaceableObj*& pPlObj = v_vCovObjs[i]; 
		BasicObject* pObj = dynamic_cast<BasicObject*>(pPlObj);
		if ((BasicObject*)0 != pObj)
		{
			return pObj;
		}
	}

	return ((BasicObject*)0);
}

Gadget*	GridElement::m_getCoveringGadget()
{
	int i;
	for (i = 0; i < v_vCovObjs.size(); ++i)
	{
		PlaceableObj*& pPlObj = v_vCovObjs[i]; 
		Gadget* pGdg = dynamic_cast<Gadget*>(pPlObj);
		if ((Gadget*)0 != pGdg)
		{
			return pGdg;
		}
	}

    return ((Gadget*)0);
}

PlaceableObj* GridElement::m_getCoveringPlObj()
{
	if (!v_vCovObjs.empty())
	{
		return v_vCovObjs[0];
	}

    return ((PlaceableObj*)0);
}

void GridElement::m_setCovered(PlaceableObj* a_pPlObj, bool a_bYes)
{
	assert((PlaceableObj*)0 != a_pPlObj);
	PLACEABLE_OBJ_VEC::iterator it = find(v_vCovObjs.begin(), v_vCovObjs.end(), a_pPlObj);
	bool objFound = (it != v_vCovObjs.end());

	if (a_bYes)
	{
		if (!objFound)
			v_vCovObjs.push_back(a_pPlObj);

		return;
	}

	// a_bYes == false
	if (objFound)
		v_vCovObjs.erase(it);

	return;
}

