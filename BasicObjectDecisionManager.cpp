#include "common.h"

const int BasicObjectDecisionManager::DEFAULT_FRAME_RATE = 10;


BasicObjectDecisionManager::BasicObjectDecisionManager(BasicObject* a_pObj) :
	v_pObj(a_pObj),
	v_nFrameRate(DEFAULT_FRAME_RATE)
{
}

BasicObjectDecisionManager::~BasicObjectDecisionManager()
{
}

void BasicObjectDecisionManager::m_addWorldInfoReport(BasicObjectWorldInfoReport* a_pReport)
{
    v_vWorldInfoReports.push_back(a_pReport);
}

void BasicObjectDecisionManager::m_addParamsReport(BasicObjectParamsReport* a_pReport)
{
	v_vParamsReport.push_back(a_pReport);
}

void BasicObjectDecisionManager::m_do()
{
	

	return;
}

