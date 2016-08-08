#include "common.h"


BasicObjectWorldInfoFrame::BasicObjectWorldInfoFrame(ulong a_nFrameNo) :
	v_nFrameNo(a_nFrameNo)
{
}

BasicObjectWorldInfoFrame::~BasicObjectWorldInfoFrame()
{
}

void BasicObjectWorldInfoFrame::initAndRecalculate(vector<BasicObject*>& a_rvObjs)
{
	vector<BasicObject*>::iterator itOO;
	for (itOO = a_rvObjs.begin(); itOO != a_rvObjs.end(); ++itOO)
	{
		BasicObject* pObj = *itOO;
		if (pObj->m_isDynamic())
		{
			// TODO: check if enemy
			if (1)
			{
				v_nFriendsNo++;
			}
			else
			{
				v_nEnemiesNo++;
			}
		}
		else
		{
			v_nStaticUnitsNo++;
		}
	}

	v_vObjs = a_rvObjs;
}

