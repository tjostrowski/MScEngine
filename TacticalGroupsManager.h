#ifndef __TACTICALGROUPSMANAGER_H__
#define __TACTICALGROUPSMANAGER_H__

#include "common_header.h"
#include "TacticalGroup.h"

enum
{
	E_TG_MIN_TG_SIZE 	= 3,
	E_TG_MAX_TG_SIZE 	= 15,	

	E_TG_MAX_SZ_SCAL_0	= 1,
	E_TG_MAX_SZ_SCAL_1	= 2,
	E_TG_MAX_SZ_SCAL_2	= 4,

	E_TG_FRONT			= 1,
	E_TG_BACK			= 2,
	E_TG_WING			= 3,
};

// 1.f == one cell. multiplied by gridCellSizeX
#define TG_MAX_DIST_BETWEEN_TG_UNITS 10.f

typedef pair<pt3f, float> AREA;


class TGManager
{
public:
	static TGManager*			m_getTGManager();

	TG*							m_createTG(const uint& a_assign);
	bool						m_removeTG(const uint& a_nId);
	TG*							m_getTGWithId(const uint& a_nId);

	void						m_update();

	void						m_draw(int a_nUserId, BITMAP* a_pDestBmp);

	inline TACTICGRP_VEC& 		m_getAvailableTGs() { return v_vTGRps; }

	TG* 						m_getTGNearestPt(cpt3f& a_pt, cuint& a_assign = (uint)UNDEFINED);

	// deleted?
	bool						delObjRef(BasicObject* a_pObj);

protected:
	TGManager();
	virtual ~TGManager();

	bool						m_init();

	void						m_createTacticalGroups(const uint& a_nAssignment);

	void 						m_divideToTGs(const uint a_nAssign, const vector<AREA>& a_vAreas, UNITS_VEC& a_vObjs,
											  float scal);


	void						m_findStruct(const uint& a_nAssignment);
	void						m_findFronts(const uint& a_nAssignment, vector<AREA>& a_vec);
	void						m_findBacks(const uint& a_nAssignment, vector<AREA>& a_vec);
	void						m_findWings(const uint& a_nAssignment, vector<AREA>& a_vec);


protected:
	TACTICGRP_VEC				v_vTGRps;
	map<uint /* front, back, wings */ , vector<AREA> >	
		v_mapAreaDescrs;

	static TGManager*			v_pManager;
};

#endif // __TACTICALGROUPSMANAGER_H__
