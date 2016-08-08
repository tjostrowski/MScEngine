#ifndef __LAYER_H__
#define __LAYER_H__

#include "common_header.h"


typedef map<int, Layer*> LAYER_MAP;

class Layer
{
public:
    inline Layer(int a_nLevel = UNDEFINED) :
		v_nLevel(a_nLevel) {}
    virtual ~Layer();

public:
	void								m_applyToGrid(MapGrid* a_pGrid, int a_nLayerLevel);
   
	uint 								m_addObject(BasicObject* pObj);
	BasicObject*						m_getObjWithId(const uint a_nId);
	inline vector<BasicObject*>&		m_getObjects() { return v_vObjects; }

	inline void 						m_getObjsAssigned(uint a_nAssign, vector<BasicObject*>& a_rvObjs)
	{
		// TODO: to fill ???
	}

	uint								m_addBullet(Bullet* a_pBullet);
	Bullet*								m_getBulletWithId(const uint a_nId);
	inline vector<Bullet*>&				m_getBullets() { return v_vBullets; }
    
	uint								m_addGadget(Gadget* a_pGdg);
	Gadget*								m_getGadgetWithId(const uint a_nId);
	inline vector<Gadget*>				m_getGadgets() { return v_vGadgets; }
    
	inline int							m_getLevel() { return v_nLevel; }
    inline void							m_setLevel(int a_nLevel) { v_nLevel = a_nLevel; }

    /**
     * Draws all objects within layer that are visible
     *
	 */
	void								m_draw(int a_nUserId, BITMAP* a_pDestBmp);

	void								m_update();

	static const Layer*					m_getLayerAtLevel(const int a_nLevel);

private:
	int									v_nLevel;
    vector<BasicObject*>				v_vObjects;
	vector<Bullet*>						v_vBullets;
	vector<Gadget*>						v_vGadgets;

	static map<int /* level */, Layer*>	v_mapLevelsToLayers;
};

#endif // __LAYER_H__
