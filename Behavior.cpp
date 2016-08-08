#include "common.h"


Behavior::Behavior(BasicObject* a_pObj) :
	v_pObj(a_pObj),
	v_bReconfiguredToBehavior(false),
	v_bIsStopped(false),
	v_bIsHalted(false),
	v_bIsAssignedToStop(false),
	v_bIsAssignedToHalt(false)
{
}

Behavior::~Behavior()
{
}

void Behavior::m_assocActivity(int a_nActivityId, TextureObject& a_rObj)
{
	v_mapActivities.insert(make_pair(a_nActivityId, a_rObj));
}

bool Behavior::m_getTextureObj(TextureObject& a_txtObj, const int a_nActivity)
{
	assert(a_nActivity >= 0);

	ACTIV2TXT_MAP::const_iterator itA2T = v_mapActivities.find(a_nActivity);
	if (itA2T == v_mapActivities.end())
		return false;

	a_txtObj = v_mapActivities[a_nActivity];

	return true;
}

