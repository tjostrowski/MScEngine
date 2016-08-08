#ifndef __GRIDELEMENT_H__
#define __GRIDELEMENT_H__

#include "common_header.h"


struct GridElementListElement
{
	int v_nType;
	int v_nLayerNumber;
	union
	{
		BasicObject* v_pObj;	
	};
};

class equal_layer_number
{
	int v_nLayerNumber;
public: 
	equal_layer_number(const int a_nLayerNumber) :
		v_nLayerNumber(a_nLayerNumber) {}

    bool operator()(GridElementListElement& elem) const
	{
		return elem.v_nLayerNumber == v_nLayerNumber;
	}
};

typedef vector<PlaceableObj*> PLACEABLE_OBJ_VEC;


class GridElement
{
public:
	static const int ELEMENT_GAME_OBJECT;
    
	GridElement();
	virtual ~GridElement() {}

	/**
     * 
     * Adds new basic object to the stack of existing objects
     * 
	 * @param a_pObj
	 * @param a_nLayerLevel
	 */
	void 				m_addLayerObject(BasicObject* a_pObj, int a_nLayerNumber);


	void 				m_removeLayerObject(BasicObject* a_pObj) {};

	BasicObject* 		m_getObjectAtLayer(int a_nLayerNumber);

	void 				removeLayer(int a_nLayerNumber);

	void 				clearLayers();

	// statistics
	int 				m_getStatistics() { return 0; }

	bool				m_isCoveredByObject();
	bool				m_isCoveredByGadget();
	bool				m_isCovered();
    BasicObject* 		m_getCoveringObject();
	Gadget*				m_getCoveringGadget();
	PlaceableObj*		m_getCoveringPlObj();	
	void 				m_setCovered(PlaceableObj* a_pPlObj, bool a_bYes);

	// tests
	inline void			m_setBitmap(BITMAP* a_pBmp) { v_pBmp = a_pBmp; }
    inline BITMAP*		m_getBitmap() { return v_pBmp; }

	inline void 		m_setId(int a_nId) { dbg_nId = a_nId; }
	inline int  		m_getId() { return dbg_nId; }

	inline bool 		m_isNeighbour(GridElement* a_pElem) { return true; }

	inline INT_COORDS	m_getCoords() { return v_coords; } 
	inline void			m_setCoords(INT_COORDS a_coords) { v_coords = a_coords; }
    
	inline void			m_setIndex(int a_nIndex) { v_nIndex = a_nIndex; }
    inline int			m_getIndex() { return v_nIndex; }	
    
protected:
	// tests
	BITMAP* 						v_pBmp;	

	vector<GridElementListElement>	v_vElementLayers;

	PLACEABLE_OBJ_VEC				v_vCovObjs;
    
	int								v_nIndex;

	INT_COORDS						v_coords;

	int								dbg_nId;
};

#endif // __GRIDELEMENT_H__
