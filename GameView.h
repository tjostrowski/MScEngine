#ifndef __GAMEVIEW_H__
#define __GAMEVIEW_H__

#include "common_header.h"


class GameView
{
public:
	GameView(int a_nUserId);
	virtual ~GameView()  {}

	bool 				m_isObjectVisible(BasicObject* pObj);
    INT_COORDS			m_mapFromOverallToViewCoords(INT_COORDS a_coords);

	void 				m_move(int a_nDx, int a_nDy);

	inline int			m_getGridElemSizeX() { return v_nGridElemSizeX; }		
	inline int			m_getGridElemSizeY() { return v_nGridElemSizeY; }
	inline RECTANGLE	m_getClippingRectangle() { return v_clippingRectangle; }
 
	void 				init(
							int a_nGridElemSizeX,
							int a_nGridElemSizeY,
							RECTANGLE a_rect);

	RECTANGLE 			m_getGameRectangle();

	INT_COORDS			m_mapGridElementToOverallViewCoords(INT_COORDS a_gridElem);
	INT_COORDS			m_mapGridElementToViewCoords(INT_COORDS a_gridElem);

	inline void			m_setFogOfWar(bool a_bFog) { v_bIsFogOfWar = a_bFog; }
    inline bool			m_isFogOfWar() { return v_bIsFogOfWar; }

	bool				m_isPointWithingClipRectangle(INT_COORDS a_coords);
	bool 				m_isRectangleCrossClipRectangle(RECTANGLE a_rect);

	INT_COORDS			m_mapCoordsToGridElement(INT_COORDS a_coords);

protected:
    
	int			v_nUserId;
	// means thar enemy buildings/units
	// will not be displayed normally
	// but will be displayed when if field of view
	// of buildings/units with id : v_nUserId
	bool 		v_bIsFogOfWar;

	RECTANGLE	v_clippingRectangle;

	int			v_nGridElemSizeX;
	int			v_nGridElemSizeY;

	int			v_nWidth;
	int			v_nHeight;
};

#endif // __GAMEVIEW_H__
