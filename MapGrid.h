#ifndef __MAPGRID_H__
#define __MAPGRID_H__

#include "common_header.h"


/**
 * 
 * This is basic grid class.
 * It can handle different grid cells (quads, hexagons)
 * 
 */
class MapGrid
{
public:
	MapGrid() {}
    virtual ~MapGrid();

	/**
     *  This function generated basic grid.
     *  Allocated memory, etc.
     * 
     */
	void						m_generateGrid();
	/**
     * This function unallocates specific grid area
     *
	 */
    virtual void				unloadGridArea(GridArea& area) = 0;
	/**
     * This function unallocates whole grid
     *  
     */
	virtual void				unloadWholeGrid() = 0;

    /**
     * This function returns specific grid element
     * at specified position
     * with grid resolution a_nResolution.
     * If a_nParentResolution != UNDEFINED
     * then finds parent grid element (with resolution
     * a_nParentResolution) to element with coords a_coords
     * and resolution a_nResolution.
     * 
     */
	virtual const GridElement*	m_getGridElement(INT_COORDS a_coords,
												int a_nResolution = 1,
												int a_nParentResolution = UNDEFINED) = 0; 

	/**
     * Obtains children with resolution a_nChildResolution of
     * element a_parCoords with resolution a_nParResolution
     * and put it into a_rvChildren vector.
     * 
     */
	virtual void				m_getGridChildren(INT_COORDS a_parCoords,
												  int a_nParResolution,
												  int a_nChildResolution,
												  vector<GridElement*>& a_rvChildren) = 0;

	/**
     *  This function calculates distance
     *  between two grid elements
     * 
     */
	virtual const int			getDistance(const GridElement& e1, const GridElement& e2) = 0;

	/**
     * Maps pair of (X, Y) grid element number coordinates to
	 * coordinates of left upper edge of smallest rectangle
	 * containing this grid element
	 * 
	 * @param a_gridElem
	 * 
	 * @return INT_COORDS
	 */
	virtual INT_COORDS 			m_mapGridElementToCoords(INT_COORDS a_gridElem) = 0;

	/**
     * Maps coordinates in real world to grid element coordinates.
     * 
	 * @param a_coords
	 * 
	 * @return INT_COORDS
	 */
	virtual INT_COORDS 			m_mapCoordsToGridElement(INT_COORDS a_coords, 
														 int a_nGameViewId = DEFAULT_USER_ID) = 0;

	/**
     * Maps pair of (X, Y) grid element number coordinates to
     * coordinates of ledt upper edge of smallest rectangle
     * containing this grid element in specific game view
     * 
     * @param a_nGameViewId
	 * @param a_gridElem
	 * 
	 * @return INT_COORDS
	 */
	virtual INT_COORDS 			m_mapGridElementToGameViewCoords(INT_COORDS a_gridElem, 
																 int a_nGameViewId = DEFAULT_USER_ID) = 0;

	Layer*						m_createLayer(int a_nLevel);
	Layer*						m_getLayer(int a_nLevel);
	void						m_addLayerObject(int a_nLevel, BasicObject* a_pObj);
	void						m_createBackgroundLayer(int a_nBackTextureId);

	inline const int			getNumLayers() { return v_nNumLayers; }
	inline void					setNumLayers(int a_nNumLayers) { v_nNumLayers = a_nNumLayers; }

	// TODO: to fill
	void						m_introduceNewObjectIntoWorld(BasicObject* a_pObj) {}
    
public:
	void						m_draw(int a_nUserId, BITMAP* a_pDestBmp);

	/**
     * Get X (width) and Y (height) of smallest rectangle
     * containing this grid element
     * 
	 * @return int
	 */
	inline int					m_getGridElementSizeX() { return v_nGridElemSizeX; }
	inline int					m_getGridElementSizeY() { return v_nGridElemSizeY; }

    inline RECTANGLE			m_getGameRectangle() { return v_gameRectangle; };

	inline int					m_getNumCellX() { return v_nNumCellX; }
	inline void					m_setNumCellX(int a_nNumCellX) { v_nNumCellX = a_nNumCellX; }
	inline int					m_getNumCellY() { return v_nNumCellY; }
	inline void					m_setNumCellY(int a_nNumCellY) { v_nNumCellY = a_nNumCellY; }

	bool						m_validCoords(INT_COORDS a_coords);

	virtual void 				init(
									int a_nWidth,
									int a_nHeight,
									int a_nNumCellX,
									int a_nNumCellY,
									int a_nGridElemSizeX,
									int a_nGridElemSizeY,
									int a_nNumLayers) = 0;

protected:
	virtual void				m_generateBasicGridStructure_() = 0;

protected:
    int 						v_nNumCellX;
	int 						v_nNumCellY;
    int 						v_nGridElemSizeX;
	int							v_nGridElemSizeY;

	int							v_nNumLayers;

	RECTANGLE					v_gameRectangle;

	vector<Layer*>				v_vLayers;
};

#endif // __MAPGRID_H__
