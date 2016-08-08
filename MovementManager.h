#ifndef __MOVEMENTMANAGER_H__
#define __MOVEMENTMANAGER_H__

#include "common_header.h"


class PATHELEM_EXT 
{
public:
	INT_COORDS	gridElem;
	int		  	nResolution;
	int			nFF;
	int			nGG;

	vector<PATHELEM_EXT> vPrevPathElems;	

	PATHELEM_EXT(INT_COORDS a_gridElem, int a_nResolution, int a_nFF, int a_nGG)
		: gridElem(a_gridElem), nResolution(a_nResolution), nFF(a_nFF), nGG(a_nGG) {}

	bool operator<(PATHELEM_EXT& elem)
	{
		// It is reverse, I know...
		return nFF >= elem.nFF;
	}
};

class equal_grid_elem
{
	INT_COORDS v_gridElem;
public: 
	equal_grid_elem(const INT_COORDS a_gridElem) :
		v_gridElem(a_gridElem) {}

    bool operator()(PATHELEM_EXT a_elem) const
	{
		return a_elem.gridElem == v_gridElem;
	}
};

class MovementManager
{
public:
	MovementManager();
	virtual ~MovementManager();


    /**
     * Computes path in grid for object a_pObj
     * to destination and stores result in vector
     * a_rvGridElems. Computed path can have
     * mipmap elements in it.
     * a_dest is on lowest mipmap level.
     *
     */
	static void 		m_computePath(BasicObject* a_pObj, 
									  INT_COORDS a_dest,
									  set<INT_COORDS>& a_setCoveredGridElems,
									  vector<PATHELEM>& a_rvGridElems,
									  bool a_bAssumeSimplePath = false);

	/**
     * 
     * 
	 * @param a_begin
	 * @param a_dest
	 * @param a_setCoveredGridElems
	 * @param a_rvGridElems
     * @param a_nRadius - radius at which there should be no
     *                  surrounding static objects.
	 * @param a_bAssumeSimplePath
	 */
	static void 		m_computePath(INT_COORDS a_begin,
									  INT_COORDS a_dest,
									  set<INT_COORDS>& a_setCoveredGridElems,
									  vector<PATHELEM>& a_rvGridElems,
									  int a_nRadius = 0,
									  bool a_bAssumeSimplePath = false);

	/**
     * Recomputes path from a_elem0 to a_elem1
     * at lowest mipmap level
     * and stores it in a_rvPath.
     * a_elem0 resolution must be 1.
     * 
     */
	static void			m_recomputePath(BasicObject* a_pObj,
										PATHELEM a_elem0,
										PATHELEM a_elem1,
										vector<PATHELEM>& a_rvPath);

	/**
     * Adjust computing paths to actual game load
     *
     */
	void 				m_setLoad() {}

	/**
     * Computes frame gap between making new move
     * on velocity basis.
     * 1 => every frame 2 => every second frame ...
     * 
	 * @param a_nVelocity
	 * 
	 * @return int
	 */
	static int 			m_getFrameGap(int a_nVelocity);

	static void 		m_resolveDynamicMovementConflicts(BasicObject* a_pObj);

	static void 		m_setMipmapGridSize(int a_nSize);
	inline static int	m_getMipmapGridSize() { return v_nMipmapGridSize; }

	static int			m_getDistance(INT_COORDS a_coords1, INT_COORDS a_coords2, 
									  int a_nResolution);

public:
	static void			m_getDiffs(int a_nDist, vector<INT_COORDS>& vDiffs); 	

	static INT_COORDS	findNearestEmptyPos(INT_COORDS a_pos, const uint a_nDeep = 2);

protected:
    /**
     * Computes path at resolution >>a_nResolution<< with A*.
     * 
	 * @param a_pObj
	 * @param a_dest
	 * @param a_setCoveredGridElems
	 * @param a_rvGridElems
	 */
	static void 		m_computePathExt(INT_COORDS a_start,
										 INT_COORDS a_dest,
										 int a_nResolution,
										 set<INT_COORDS>& a_setCoveredGridElems,
										 vector<PATHELEM>& a_rvGridElems);

	/**
     * Computes path at resolution >>a_nResolution<< with greedy
     * algorithm.
     * 
	 * @param a_start
	 * @param a_dest
	 * @param a_nResolution
	 * @param a_setCoveredGridElems
	 * @param a_rvGridElems
	 */
	static int			m_computePathGreedy(INT_COORDS a_start,
											INT_COORDS a_dest,
											int a_nResolution,
											set<INT_COORDS>& a_setCoveredGridElems,
											vector<PATHELEM>& a_rvGridElems);

protected:

	static void			m_getAdjacentGridElems(PATHELEM_EXT a_pathElem, 
											   set<INT_COORDS>& a_setCoveredGridElems,	 
											   vector<PATHELEM_EXT>& a_rvAdjPathElems);

	static int  		m_estimate_hh(PATHELEM_EXT a_vv, PATHELEM_EXT a_dd);

	static int			m_getDistance(PATHELEM_EXT a_v1, PATHELEM_EXT a_v2); 

public:
	static bool 		m_isValid(INT_COORDS gridElem, int a_nRes = 1);

protected:

	/**
     * How many grid elems will be in mipmap movement element
     *
	 */
	static int v_nMipmapGridSize;

public:
	static const int MAX_COMPUTE_PATH_GREEDY_STEP_NUM;

};

#endif // __MOVEMENTMANAGER_H__
