#ifndef __BASICOBJECTCOMMUNICATIONMANAGER_H__
#define __BASICOBJECTCOMMUNICATIONMANAGER_H__

#include "common_header.h"
#include "TimeFrameScheduledObject.h"
#include <iostream>


class MessageData
{
public:
	virtual ~MessageData() {} 
	//....
};

class Message
{
public:
	int				nId;

	int				nSenderId; 
	int				nReceiverId;

	int				nType;
	// PRIORITY: 0-10 (10 => HIGHEST)
	int				nPriority;
    MessageData*	pData;

	int				nUsedCount;

	static int		IDS;

	inline virtual ~Message()
	{
		delete pData;
	}
};

class ReportMessageData : public MessageData
{
public:
	BasicObjectWorldInfoReport* pWorldInfoReport;
	BasicObjectParamsReport*	pParamsReport;

	// ...
};

struct MessageDataParameter
{
	inline MessageDataParameter(string a_strParamName, int a_nValue) :
		strParamName(a_strParamName), nValue(a_nValue) {}

	inline MessageDataParameter(string a_strParamName, float a_fValue) :
		strParamName(a_strParamName), fValue(a_fValue) {}

	inline MessageDataParameter(string a_strParamName, BasicObject* a_pObj) :
		strParamName(a_strParamName), pObj(a_pObj) {}

	inline MessageDataParameter(string a_strParamName, void* a_pOther) :
		strParamName(a_strParamName), pOther(a_pOther) {}

	string strParamName;
	union
	{
		int				nValue;	 
		float			fValue;
		BasicObject*	pObj;
		void*			pOther;
	};
};

class CommandMessageData : public MessageData
{
public:
	int								nBehaviorId;
	vector<MessageDataParameter>	vParameters;

	// 1st parameter => what to do with behavior
	static const int START_BEHAVIOR;
	static const int STOP_BEHAVIOR;
	static const int RESUME_BEHAVIOR;
	static const int HALT_BEHAVIOR;
	static const int UPDATE_BEHAVIOR;

};

// helper classes
class equal_or_higher_priority_message
{
	int v_nPriority;
public: 
	equal_or_higher_priority_message(int a_nPriority) :
		v_nPriority(a_nPriority) {}

    bool operator()(Message* a_pMsg) const
	{
		return a_pMsg->nPriority >= v_nPriority;
	}
};

class lower_priority_message
{
	int v_nPriority;
public: 
	lower_priority_message(int a_nPriority) :
		v_nPriority(a_nPriority) {}

    bool operator()(Message* a_pMsg) const
	{
		return a_pMsg->nPriority < v_nPriority;
	}
};


class BasicObjectCommunicationManager : public TimeFrameScheduledObject
{
public:
	BasicObjectCommunicationManager(BasicObject* a_pObj);
	virtual ~BasicObjectCommunicationManager();

	int					m_sendUnicastMessage(BasicObject* a_pToWho, Message* a_pMessage);
	int 				m_sendMulticastMessage(vector<BasicObject*>& a_rvToWhos, Message* a_pMessage);
	int 				m_sendBroadcastMessage(Message* a_pMessage);

	int 				m_recvMessage(Message* a_pMessage);

	inline void 		m_setMaxInputQueueSize(int a_nMaxSize) { v_nMaxInputQueueSize = a_nMaxSize; }
	inline void 		m_setMaxOutputQueueSize(int a_nMaxSize) { v_nMaxOutputQueueSize = a_nMaxSize; }
	inline void 		m_setMaxProcessedInputMsgsInTurn(int a_nMaxMsgs)
	{
		v_nMaxProcessedInputMsgsInTurn = a_nMaxMsgs;
	}

    Message*			initMessage(int a_nReceiverId,
									int a_nType,
									int a_nPriority,
									MessageData* a_pData);

	Message*			initCommand(int a_nReceiverId,
									int a_nBehaviorId,
									vector<MessageDataParameter>& a_vBehaviorParams);

	void				m_do();

protected:
	void 				m_processMessage(Message* a_pMessage);

	void				m_processCommand(Message* a_pMessage);

protected:
	int					v_nSentMessages;
	int					v_nReceivedMessages;

protected:
	BasicObject*		v_pObj;

protected:
	vector<Message*>	v_vOutputMessages;

	vector<Message*>	v_vInputMessages;

	int					v_nMaxOutputQueueSize;
	int					v_nMaxInputQueueSize;

	int 				v_nMaxProcessedInputMsgsInTurn;

public:
	static const int	MULTICAST_ID;
	static const int	BROADCAST_ID;	

public: 
    static const int	COMMAND_MSG_TYPE;
	static const int	REPORT_MSG_TYPE;
	static const int	OTHER_MSG_TYPE;

public:
	static const int	HARD_MAX_OUTPUT_QUEUE_SIZE;
	static const int 	HARD_MAX_INPUT_QUEUE_SIZE;

	static const int	HARD_MAX_PROCESSED_INPUT_MSGS_IN_TURN;

public:
	static const int	MIN_MSG_PRIORITY;
	static const int	MAX_MSG_PRIORITY;

};

#endif // __BASICOBJECTCOMMUNICATIONMANAGER_H__
