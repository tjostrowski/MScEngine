#ifndef __GROUP_H__
#define __GROUP_H__

#include "common_header.h"

struct GroupObjDescr
{
	int nRank;
	int nClass;

	// index to which index in leader path this object is nearest
	// when class is CLASS_PATH_NOT_GROUP_STEERED
	int nLPathIndex;

	GroupObjDescr(int a_nRank, int a_nClass, int a_nLPathIndex = UNDEFINED)
		: nRank(a_nRank), nClass(a_nClass), nLPathIndex(a_nLPathIndex) {} 
    
	enum RANK {
		RANK_LEADER = 0,
		RANK_OTHER
	};

	enum CLASSIFICATION {
		UNCLASSIFIED = -1,
		// steered by leader
		CLASS_GROUP_STEERED = 0,
		// steered by previous leader path
		CLASS_PATH_NOT_GROUP_STEERED,
		// steered by leader approaching to path
		CLASS_PARTLY_GROUP_STEERED,
		// self steering
		CLASS_SELF_STEERED
	};
};

typedef pair<BasicObject*, GroupObjDescr> GROUP_OBJ;
typedef vector<GROUP_OBJ> VEC_GROUP;

class equal_object
{
	BasicObject* v_pObj;
public: 
	equal_object(BasicObject*& a_pObj) :
		v_pObj(a_pObj) {}

    bool operator()(GROUP_OBJ a_obj) const
	{
		return a_obj.first == v_pObj;
	}
};

class equal_class
{
	int v_nClass;

public:
	equal_class(int a_nClass) :
		v_nClass(a_nClass) {}

	bool operator()(GROUP_OBJ a_obj) const
	{
		return a_obj.second.nClass = v_nClass;
	}
};

class Group
{
public:
	Group(int a_nId);
	virtual ~Group();

	inline int			m_getId() { return v_nId; }

	int					m_getObjectsNum();

	void				m_addObject(BasicObject* a_pObj);

	void				m_removeObject(BasicObject* a_pObj);

	void				m_setDefaultLeader();
	void				m_setLeader(BasicObject* a_pObj);
	inline BasicObject*	m_getLeader() { return v_pLeader; }
    
	void				m_nextStep();

	void				m_setActualTarget(INT_COORDS a_coords);

protected:
	void				m_classifyObjects();
	int					m_classifyObject(BasicObject* a_pObj);

protected:	
	bool				m_isConsistent();
	void				m_makeConsistent();

protected:
    /**
     * Predicts leader position +a_nStep forwards from here.
	 * 
	 * @param a_nStep
	 * 
	 * @return INT_COORDS
	 */
	INT_COORDS			m_predictLeaderPos(int a_nStep);	

	// minimum distance between objects in 'real' group 
	// (any object must have AT LEAST this distance to any other distance) => if possible
	inline void			m_setMinGroupDist(int a_nDist) { v_nMinGroupDist = a_nDist; }
	inline int			m_getMinGroupDist() { return v_nMinGroupDist; }

	// maximum distance between objects in 'real' group
	// (any object must have AT LEAST one object with at most this distance) => if possible
	inline void			m_setMaxGroupDist(int a_nDist) { v_nMaxGroupDist = a_nDist; }
	inline int			m_getMaxGroupDist() { return v_nMaxGroupDist; }

public:
	static const int								MAX_NUM_OBJECTS;
	static const int								MAX_MAX_DIST_TO_LEADER;
	static const int								MAX_MAX_DIST_TO_PATH;
	static const int								MAX_MAX_PATH_STEP_BACKWARD;	

protected:
	vector<GROUP_OBJ>								v_vObjects;
	
	int												v_nId;

protected:
	BasicObject*									v_pLeader;

protected:
	vector<PATHELEM>								v_vLeaderPath;
	
	int												v_nMaxDistToLeader;

	int												v_nMinGroupDist;
	int												v_nMaxGroupDist;

protected:
	INT_COORDS										v_actualTarget;
};

#endif // __GROUP_H__
