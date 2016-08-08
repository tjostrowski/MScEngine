#ifndef __FORMATION_H__
#define __FORMATION_H__

#include "common_header.h"

// <ROW, POS_IN_ROW>
#define FORMATION_POS pair<int, int>

class FormationObjDescr
{
protected:
	BasicObject*	v_pObj;
	FORMATION_POS	v_formPos;

	// grid position relative to 'formation  position' as defined
	// in Formation class
	INT_COORDS		v_grPos;

public:
	FormationObjDescr(BasicObject* a_pObj, FORMATION_POS a_formPos, 
					  INT_COORDS a_gridRelativePos = INVALID_COORDS) :
		v_pObj(a_pObj), v_formPos(a_formPos), v_grPos(a_gridRelativePos) {}

	inline BasicObject* 	m_getObj() 		{ return v_pObj;  }
	inline FORMATION_POS	m_getFormPos()	{ return v_formPos; }
    inline int				m_getRow() 		{ return v_formPos.first; }
	inline int				m_getPosInRow()	{ return v_formPos.second; }

	inline void				m_setFormPos(FORMATION_POS a_formPos) { v_formPos = a_formPos; }

	inline void				m_setGridRelPos(INT_COORDS a_grPos) { v_grPos = a_grPos; }
	inline INT_COORDS		m_getGridRelPos() { return v_grPos; }
};

class equal_form_object
{
	BasicObject* v_pObj;
public: 
	equal_form_object(BasicObject*& a_pObj) :
		v_pObj(a_pObj) {}

    bool operator()(FormationObjDescr a_descr) const
	{
		return a_descr.m_getObj() == v_pObj;
	}
};

class Formation
{
public:
	enum FORMATION_STATE
	{
		NOT_FORMED,
		FORMING,
		FORMED
	};

	Formation();
	virtual ~Formation();

	// set formation as rectangle
	void		m_setAsRectangle(int a_nRows, int a_nUnitsInRow);
	// set formation as traingle with max units in row = nMaxUnitsInRow
	// and min units in row = 1
	void		m_setAsTriangle(int a_nRows, int a_nMaxUnitsInRow);
	// set formation as circle with diameter nMaxUnitsInRows
	inline void m_setAsCircle(int a_nMaxUnitsInRow) {}

	void		m_setFormation(vector<int>& a_vRowInfo);


	inline int	m_getObjectsNum() { return v_vUnits.size(); } 

	int			m_getRequiredNumUnits();

	inline int	m_getStatus() { return v_nStatus; } 
    

	int	 		m_addObject(BasicObject* a_pObj);

	void		m_removeObject(BasicObject* a_pObj);

	// angle of formation in real degrees
	inline int	m_getAngle() { return v_nAngle;  }	
	void		m_setAngle(int a_nAngle);	

	inline void	m_setPos(INT_COORDS a_pos) { v_pos = a_pos; }

public:
	void		m_getGRPoses(vector<INT_COORDS>& a_rvPoss);

public:
	/**
     * Form the formation to actual positions.
	 * 
     * @return int => If forming was successful.
	 */
	int			m_nextStep();

protected:
	void		m_reassignUnitsToFormation();

	void		m_reassignUnitToFormation(BasicObject* a_pObj, FormationObjDescr& a_descr);

	INT_COORDS	m_getGRPos(FORMATION_POS a_formPos);

protected:
	bool		m_isValid();	

protected:
	inline bool	m_pathToDestNeedsRecompute() { return false; }
    inline void	m_recomputePathToDest() {}
    
protected:
	bool		m_checkIfGoodPositionForFormation(INT_COORDS a_leadPos);

protected:
	int							v_nRequiredNumUnits;

	// formation must have at least one symmetry axis 
	vector<int>					v_vFormationRows;

	int							v_nStatus;

	vector<FormationObjDescr>	v_vUnits;

	// formation position: 'middle' of front row of the formation => MID OF v_vFormationRows.front()
	INT_COORDS					v_pos;

    // vector of unit positions. Each entry contains unit positions for one row.
	vector<vector<INT_COORDS> >	v_vPoss;

	// angle under which formation rows are to horizontal line : can be 0-360
	int							v_nAngle;

	// state in which formation is: one of FORMATION_STATE
	int							v_nFormationState;

	// destination to which formation is going
	INT_COORDS					v_destPos;

	// path to destination for pseudo leader (MID OF v_vFormationRows.front()) element
	vector<PATHELEM>			v_vPathToDest;
};

#endif // __FORMATION_H__
