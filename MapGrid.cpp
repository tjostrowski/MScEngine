#include "common.h"


MapGrid::~MapGrid()
{
	vector<Layer*>::iterator it;
	for (it = v_vLayers.begin(); it != v_vLayers.end(); ++it)
	{
		Layer* pLayer = *it;
		delete pLayer;
		*it = 0;
	}
	v_vLayers.clear();
}

void MapGrid::m_generateGrid()
{
	m_generateBasicGridStructure_();
}

void MapGrid::m_addLayerObject(int a_nLayerLevel, BasicObject* a_pObj)
{
	Layer* pLayer = m_getLayer(a_nLayerLevel);
	if (0 != pLayer)
	{
		pLayer->m_addObject(a_pObj);
	}
}

Layer* MapGrid::m_createLayer(int a_nLayerLevel)
{
	if (a_nLayerLevel < v_vLayers.size())
	{
		return v_vLayers[a_nLayerLevel];
	}

	int nLevel = min(v_nNumLayers++, a_nLayerLevel);
    Layer* pLayer = new Layer(nLevel);

	v_vLayers.push_back(pLayer);

	return pLayer;
}

Layer* MapGrid::m_getLayer(int a_nLayerLevel)
{
	if (a_nLayerLevel >= v_vLayers.size())
	{
		return 0;
	}

	return v_vLayers[a_nLayerLevel];
}

void MapGrid::m_createBackgroundLayer(int a_bckTxtID)
{
	return;
}

void MapGrid::m_draw(int a_nUserId, BITMAP* a_pDestBmp)
{
	int nSize = v_vLayers.size();

	for (int i = 0; i < nSize; ++i)
	{
		Layer* pLayer = v_vLayers[i];
		pLayer->m_draw(a_nUserId, a_pDestBmp);
	}
}

bool MapGrid::m_validCoords(INT_COORDS a_coords)
{
	return (a_coords.first >= 0 &&
			a_coords.first < v_nNumCellX &&
			a_coords.second >= 0 &&
			a_coords.second < v_nNumCellY);
}

