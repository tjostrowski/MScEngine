#ifndef __HEXAGONALMAPGRID_H__
#define __HEXAGONALMAPGRID_H__

#include "common_header.h"


class HexagonalMapGrid : public MapGrid
{
public:
	HexagonalMapGrid() {}
	virtual ~HexagonalMapGrid();

	const GridElement*	m_getGridElement(INT_COORDS a_coords,
										int a_nResolution = 1,
										int a_nParentResolution = UNDEFINED);

	void 				m_getGridChildren(INT_COORDS a_parCoords,
										  int a_nParResolution,
										  int a_nChildResolution,
										  vector<GridElement*>& a_rvChildren);

    INT_COORDS			m_mapGridElementToCoords(INT_COORDS a_gridElem);
    INT_COORDS			m_mapGridElementToGameViewCoords(INT_COORDS a_gridElem,
														 int a_nGameViewId = DEFAULT_USER_ID);

	INT_COORDS 			m_mapCoordsToGridElement(INT_COORDS a_coords, 
												 int a_nGameViewId = DEFAULT_USER_ID);

protected:

protected:
	bool 				m_assertConsistency();

	void 				m_generateBasicGridStructure_();

protected:
	vector<GridElement*> v_vGridElems;

public:
	void init(
		int a_nWidth,
		int a_nHeight,
		int a_nNumCellX,
		int a_nNumCellY,
		int a_nGridElemSizeX,
		int a_nGridElemSizeY,
		int a_nNumLayers
	);

	inline void unloadGridArea(GridArea& area) {}
	inline void unloadWholeGrid() {}
	inline const int getDistance(const GridElement& el1, const GridElement& el2) { return 0; } 
    
};

#endif // __HEXAGONALMAPGRID_H__
