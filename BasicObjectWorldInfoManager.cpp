#include "common.h"

const int BasicObjectWorldInfoManager::DEFAULT_NUM_FRAMES_IN_MEMORY = 10;


BasicObjectWorldInfoManager::BasicObjectWorldInfoManager(BasicObject* a_pObj) :
	v_pObj(a_pObj),
	v_nNumFramesInMemory(DEFAULT_NUM_FRAMES_IN_MEMORY),
	v_nMedianEnemiesNo(UNDEFINED),
	v_nMedianFriendsNo(UNDEFINED),
	v_nMedianStaticUnitsNo(UNDEFINED)
{
}

BasicObjectWorldInfoManager::~BasicObjectWorldInfoManager()
{
}

void BasicObjectWorldInfoManager::m_do()
{
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();

	ulong nFrameIndexNo = pConf->m_getFrameNo();

	BasicObjectParamsManager* pParamsManager = v_pObj->m_getParamsManager();
	int nObjVisibility = 0;//pParamsManager->m_getVisibility();
	INT_COORDS objPos = v_pObj->m_getPosition();

	vector<BasicObject*> vNeighbouringObjs;
    for (int ii = 1; ii < nObjVisibility; ++ii)
	{
		vector<INT_COORDS> vDiffs;
		MovementManager::m_getDiffs(ii, vDiffs);
		vector<INT_COORDS>::iterator itDD;
		for (itDD = vDiffs.begin(); itDD != vDiffs.end(); ++itDD)
		{
			INT_COORDS diff = *itDD;
			INT_COORDS pos = make_pair(objPos.first + diff.first,
									   objPos.second + diff.second);
			GridElement* pElem = const_cast<GridElement*>(pGrid->m_getGridElement(
									pos));
			if (pElem && !pElem->m_isCoveredByObject())
			{
				BasicObject* pObj = pElem->m_getCoveringObject();
				if (vNeighbouringObjs.end() == 
					find(vNeighbouringObjs.begin(), vNeighbouringObjs.end(), pObj))
				{
					vNeighbouringObjs.push_back(pObj);
				}
			}
		} /* end for */
	} /* end for */

	BasicObjectWorldInfoFrame* pNewFrame = new BasicObjectWorldInfoFrame(nFrameIndexNo);
	pNewFrame->initAndRecalculate(vNeighbouringObjs);

	v_vFrames.push_back(pNewFrame);

	int nFrameNo = v_vFrames.size();
	if (nFrameNo > v_nNumFramesInMemory)
	{
		const int nOverfillFrameNo = nFrameNo - v_nNumFramesInMemory;
		for (int ii = 0; ii < nOverfillFrameNo; ++ii)
		{
			BasicObjectWorldInfoFrame* pFrameNN = *(v_vFrames.begin() + ii);
			delete pFrameNN;
		}
		v_vFrames.erase(v_vFrames.begin(), v_vFrames.begin() + nOverfillFrameNo);
	}

	// update statistical values over frames in memory
	v_nMedianFriendsNo 		= 0;
	v_nMedianEnemiesNo 		= 0;
	v_nMedianStaticUnitsNo	= 0;

	vector<BasicObjectWorldInfoFrame*>::iterator itFF;
	for (itFF = v_vFrames.begin(); itFF != v_vFrames.end(); ++itFF)
	{
		BasicObjectWorldInfoFrame* pFrame = *itFF;

		v_nMedianFriendsNo 		+= pFrame->m_getFriendsNo();
		v_nMedianEnemiesNo 		+= pFrame->m_getEnemiesNo();
		v_nMedianStaticUnitsNo	+= pFrame->m_getStaticUnitsNo();
	}

	v_nMedianFriendsNo 		/= v_nNumFramesInMemory;
	v_nMedianEnemiesNo 		/= v_nNumFramesInMemory;
	v_nMedianStaticUnitsNo 	/= v_nNumFramesInMemory;
}

void BasicObjectWorldInfoManager::m_setNumFramesInMemory(int a_nFrameNum)
{
	if (a_nFrameNum > 0)
	{
		v_nNumFramesInMemory = a_nFrameNum;
	}
}

