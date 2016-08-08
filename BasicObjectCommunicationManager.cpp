#include "common.h"
#include <iterator>


const int BasicObjectCommunicationManager::MULTICAST_ID	  = -1;
const int BasicObjectCommunicationManager::BROADCAST_ID	  = -2;	

const int BasicObjectCommunicationManager::COMMAND_MSG_TYPE	= 0;
const int BasicObjectCommunicationManager::REPORT_MSG_TYPE	= 1;
const int BasicObjectCommunicationManager::OTHER_MSG_TYPE	= 1 << 1;

const int BasicObjectCommunicationManager::HARD_MAX_INPUT_QUEUE_SIZE	= 100;
const int BasicObjectCommunicationManager::HARD_MAX_OUTPUT_QUEUE_SIZE	= 100;
const int BasicObjectCommunicationManager::HARD_MAX_PROCESSED_INPUT_MSGS_IN_TURN = HARD_MAX_INPUT_QUEUE_SIZE;

const int BasicObjectCommunicationManager::MIN_MSG_PRIORITY = 0;
const int BasicObjectCommunicationManager::MAX_MSG_PRIORITY = 10;

int Message::IDS = 0;

const int CommandMessageData::START_BEHAVIOR	= 0;
const int CommandMessageData::STOP_BEHAVIOR 	= 1;
const int CommandMessageData::RESUME_BEHAVIOR	= 1 << 1;
const int CommandMessageData::HALT_BEHAVIOR		= 1 << 2;
const int CommandMessageData::UPDATE_BEHAVIOR	= 1 << 3;


BasicObjectCommunicationManager::BasicObjectCommunicationManager(BasicObject* a_pObj) :
	v_pObj(a_pObj),
	v_nSentMessages(0), v_nReceivedMessages(0),
	v_nMaxOutputQueueSize(HARD_MAX_OUTPUT_QUEUE_SIZE),
	v_nMaxInputQueueSize(HARD_MAX_INPUT_QUEUE_SIZE),
	v_nMaxProcessedInputMsgsInTurn(HARD_MAX_PROCESSED_INPUT_MSGS_IN_TURN)
{
}

BasicObjectCommunicationManager::~BasicObjectCommunicationManager()
{
}

Message* BasicObjectCommunicationManager::initMessage(int a_nReceiverId,
													  int a_nType,
													  int a_nPriority,
													  MessageData* a_pData)
{
	Message* pMessage		= new Message();
	pMessage->nId			= Message::IDS++;
	pMessage->nSenderId 	= v_pObj->m_getId();
	pMessage->nReceiverId	= a_nReceiverId;
	pMessage->nType			= a_nType;
	pMessage->nPriority		= a_nPriority;

	pMessage->pData			= a_pData;

	pMessage->nUsedCount	= 1;

	return pMessage;
}

Message* BasicObjectCommunicationManager::initCommand(int a_nReceiverId,
													  int a_nBehaviorId,
													  vector<MessageDataParameter>& a_vBehaviorParams)
{
	if (a_vBehaviorParams.size() < 1)
	{
		cerr << " Command no properly initialized "
			 << endl;

		return 0;
	}

	Message* pMessage 		= new Message();
	pMessage->nId			= Message::IDS++;
	pMessage->nSenderId 	= v_pObj->m_getId();
	pMessage->nReceiverId	= a_nReceiverId;
	pMessage->nType			= COMMAND_MSG_TYPE;
	pMessage->nPriority		= MAX_MSG_PRIORITY;

	CommandMessageData* pData = new CommandMessageData();
	pData->nBehaviorId = a_nBehaviorId;
	pData->vParameters = a_vBehaviorParams;

	pMessage->pData = pData;

	return pMessage;
}

int BasicObjectCommunicationManager::m_sendUnicastMessage(BasicObject* a_pToWho, Message* a_pMessage)
{
	BasicObjectCommunicationManager* pReceiverManager = a_pToWho->m_getCommunicationManager();
	pReceiverManager->m_recvMessage(a_pMessage);

	vector<Message*>::iterator itMM = find(v_vOutputMessages.begin(), v_vOutputMessages.end(), a_pMessage);
	if (v_vOutputMessages.end() != itMM)
	{
		v_vOutputMessages.erase(itMM);
	}

	v_nSentMessages++;

	return 1;
}

int BasicObjectCommunicationManager::m_sendMulticastMessage(vector<BasicObject*>& a_rvToWhos, Message* a_pMessage)
{
	a_pMessage->nUsedCount--;

	vector<BasicObject*>::iterator itOO;
	for (itOO = a_rvToWhos.begin(); itOO != a_rvToWhos.end(); ++itOO)
	{
		BasicObject* pReceiver = *itOO;
		BasicObjectCommunicationManager* pReceiverManager = pReceiver->m_getCommunicationManager();
        pReceiverManager->m_recvMessage(a_pMessage);
		a_pMessage->nUsedCount++;
	}

	vector<Message*>::iterator itMM = find(v_vOutputMessages.begin(), v_vOutputMessages.end(), a_pMessage);
	if (v_vOutputMessages.end() != itMM)
	{
		v_vOutputMessages.erase(itMM);
	}

	v_nSentMessages++;

	return 1;
}

int BasicObjectCommunicationManager::m_sendBroadcastMessage(Message* a_pMessage)
{
	// TODO: to fill
	return 1;
}

int BasicObjectCommunicationManager::m_recvMessage(Message* a_pMessage)
{
	if (v_vInputMessages.size() < v_nMaxInputQueueSize)
	{
		v_vInputMessages.push_back(a_pMessage);
		return 1;
	}

	return 0;
}

void BasicObjectCommunicationManager::m_do()
{	
	// receive incoming messages
	const int nHighPriorityBorder	= MAX_MSG_PRIORITY - 2;
	const int nInputMessagesSize	= v_vInputMessages.size(); 
	vector<Message*> vMessagesWithHighestPriority(nInputMessagesSize);

	remove_copy_if(v_vInputMessages.begin(),
				   v_vInputMessages.end(),
				   vMessagesWithHighestPriority.begin(),
				   lower_priority_message(nHighPriorityBorder));
	vector<Message*>::iterator itIM = remove_if(v_vInputMessages.begin(),
												v_vInputMessages.end(),
												equal_or_higher_priority_message(nHighPriorityBorder));
	v_vInputMessages.erase(itIM, v_vInputMessages.end());

	int nProcessedMsgs = 0;
    vector<Message*>::iterator itMM;
	for (itMM = vMessagesWithHighestPriority.begin(); itMM < vMessagesWithHighestPriority.end(); ++itMM)
	{
		Message* pMessage = *itMM;
		m_processMessage(pMessage);
		nProcessedMsgs++;
	}

	if (nProcessedMsgs < v_nMaxProcessedInputMsgsInTurn)
	{
		int nRestMsgsToProcess = min(v_nMaxProcessedInputMsgsInTurn - nProcessedMsgs,
									 (int) v_vInputMessages.size());

		for (itMM = v_vInputMessages.begin(); itMM < v_vInputMessages.begin() + nRestMsgsToProcess; ++itMM)
		{
			Message* pMessage = *itMM;
			m_processMessage(pMessage);
			nProcessedMsgs++;
		}
	}
}

void BasicObjectCommunicationManager::m_processMessage(Message* a_pMessage)
{
	int nMsgType = a_pMessage->nType;

	int nSenderId = a_pMessage->nSenderId;
	int nPriority = a_pMessage->nPriority;

    if (COMMAND_MSG_TYPE == nMsgType)
	{
		m_processCommand(a_pMessage);
		return;
	}

	// normal message, not command
	if (REPORT_MSG_TYPE == nMsgType)
	{
		ReportMessageData* pData = dynamic_cast<ReportMessageData*>
										(a_pMessage->pData);
		BasicObjectWorldInfoReport* pWorldInfoReport =
			pData->pWorldInfoReport;
		BasicObjectParamsReport* pParamsReport =
			pData->pParamsReport;

		BasicObjectDecisionManager* pDecisionManager = 
			v_pObj->m_getDecisionManager();

		pDecisionManager->m_addParamsReport(pParamsReport);
		pDecisionManager->m_addWorldInfoReport(pWorldInfoReport);
	}

}

void BasicObjectCommunicationManager::m_processCommand(Message* a_pMessage)
{
	CommandMessageData* pData = dynamic_cast<CommandMessageData*>
									(a_pMessage->pData);
	if (!pData)
	{
		cerr << " Bad command format: "
			 << " invalid data "
			 << endl;
	}

	int nBehaviorId = pData->nBehaviorId;
	vector<MessageDataParameter>& vParams = pData->vParameters;
	if (vParams.size() < 1)
	{
		cerr << " Bad command format: "
			 << " invalid parameter number "
			 << endl;
	}
	// START, STOP, RESUME... etc.
	const int nCommandParam1 = vParams[0].nValue;
    
	BasicObjectBehaviorManager* pBehaviorManager =
		 v_pObj->m_getBehaviorManager();
	// For debugging purposes and for update behavior use.
	Behavior* pActualBehavior = pBehaviorManager->m_getActualBehavior(); 
	int nActualBehaviorId = UNDEFINED;
	if (pActualBehavior)
	{
		nActualBehaviorId = pActualBehavior->m_getId();
	}

	switch (nCommandParam1)
	{
	case CommandMessageData::START_BEHAVIOR:
		{
            Behavior* pBehavior = pBehaviorManager->m_createPlainNewBehavior(nBehaviorId);  
			// setting parameters for behavior
			if (ATTACK_BEHAVIOR_ID == nBehaviorId)
			{
				AttackBehavior* pAttackBehavior = (AttackBehavior*)pBehavior;
				int nWhoCount = vParams[1].nValue;
				for (int ii = 1 + 1; ii < 1 + 1 + nWhoCount; ++ii)
				{
					BasicObject* pWho = vParams[ii].pObj;
					pAttackBehavior->m_addWho(pWho);
				}
			}
			else if (ESCAPE_BEHAVIOR_ID == nBehaviorId)
			{
				EscapeBehavior* pEscapeBehavior = (EscapeBehavior*)pBehavior;
				bool bIsFromWho = (bool) vParams[1].nValue;
				if (bIsFromWho)
				{
					BasicObject* pWho = vParams[2].pObj;
					pEscapeBehavior->m_setWho(pWho);
				}
				else
				{
					INT_COORDS* pCoords = static_cast<INT_COORDS*>(vParams[2].pOther);
					pEscapeBehavior->m_setPos(*pCoords);
					// TODO: to fix
					//delete pCoords;
				}
			}
			else if (PATROL_BEHAVIOR_ID == nBehaviorId)
			{
				PatrolBehavior* pPatrolBehavior = (PatrolBehavior*)pBehavior;
				int nPatrolPointsCount = vParams[1].nValue;
				for (int ii = 1 + 1; ii < 1 + 1 + nPatrolPointsCount; ++ii)
				{
					INT_COORDS* pPPCoords = static_cast<INT_COORDS*>(vParams[ii].pOther);
					pPatrolBehavior->m_addPatrolPoint(*pPPCoords);
					// TODO: to fix
					//delete pPPCoords;
				}
			}
			else if (TRACE_BEHAVIOR_ID == nBehaviorId)
			{
				TraceBehavior* pTraceBehavior = (TraceBehavior*)pBehavior;
				BasicObject* pWho = vParams[1].pObj;
				pTraceBehavior->m_setWho(pWho);
			}

			pBehaviorManager->m_startNewBehavior(pBehavior);
		}
		break;
	case CommandMessageData::STOP_BEHAVIOR:
		{
			bool bIsStopped = pBehaviorManager->m_stopActualBehavior();
		}
		break;
	case CommandMessageData::RESUME_BEHAVIOR:
		{
			bool bIsResumed = pBehaviorManager->m_resumeActualBehavior();
		}
		break;
	case CommandMessageData::HALT_BEHAVIOR:
		{
			bool bIsHalted 	= pBehaviorManager->m_haltActualBehavior();
		}
		break;
	case CommandMessageData::UPDATE_BEHAVIOR:
		{
			if (UNDEFINED == nActualBehaviorId ||
				nBehaviorId != nActualBehaviorId)
			{
				cerr << " Command can not be run on actual behavior "
					 << endl;
				break;
			}

			if (ATTACK_BEHAVIOR_ID == nBehaviorId)
			{
				AttackBehavior* pAttackBehavior = (AttackBehavior*)pActualBehavior;
				int nWhoCount = vParams[1].nValue;
				vector<BasicObject*> vWhos;
				for (int ii = 1 + 1; ii < 1 + 1 + nWhoCount; ++ii)
				{
					BasicObject* pWho = vParams[ii].pObj;
					vWhos.push_back(pWho);
				}
				pAttackBehavior->m_setWhos(vWhos);
			}
			else if (ESCAPE_BEHAVIOR_ID == nBehaviorId)
			{
				EscapeBehavior* pEscapeBehavior = (EscapeBehavior*)pActualBehavior;
				bool bIsFromWho = (bool) vParams[1].nValue;
				if (bIsFromWho)
				{
					BasicObject* pWho = vParams[2].pObj;
					pEscapeBehavior->m_setWho(pWho);
				}
				else
				{
					INT_COORDS* pCoords = static_cast<INT_COORDS*>(vParams[2].pOther);
					pEscapeBehavior->m_setPos(*pCoords);
					// TODO: to fix
					//delete pCoords;
				}
			}
			else if (PATROL_BEHAVIOR_ID == nBehaviorId)
			{
				PatrolBehavior* pPatrolBehavior = (PatrolBehavior*)pActualBehavior;
				vector<INT_COORDS> vPatrolPoints;
				int nPatrolPointsCount = vParams[1].nValue;
				for (int ii = 1 + 1; ii < 1 + 1 + nPatrolPointsCount; ++ii)
				{
					INT_COORDS* pPPCoords = static_cast<INT_COORDS*>(vParams[ii].pOther);
					vPatrolPoints.push_back(*pPPCoords);
					// TODO: to fix
					//delete pPPCoords;
				}
				pPatrolBehavior->m_setPatrolPoints(vPatrolPoints);
			}
			else if (TRACE_BEHAVIOR_ID == nBehaviorId)
			{
				TraceBehavior* pTraceBehavior = (TraceBehavior*)pActualBehavior;
				BasicObject* pWho = vParams[1].pObj;
				pTraceBehavior->m_setWho(pWho);
			}

			pActualBehavior->m_reconfigureToNewParams();
		}
		break;
	}
}

