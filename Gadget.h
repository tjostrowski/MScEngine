#ifndef __GADGET_H__
#define __GADGET_H__

#include "common_header.h"


extern string G_CATAPULT;
extern string G_CANNON;
extern string G_POWERUP;

struct GTmplParams
{
	string	strClass;
	int		reqUnitsNum;
	float	width;
    float 	height;

	uint	gTxtID; 	
};

extern GTmplParams tblTmplParams[]; 


class Gadget : public CtrlObject, public Idxable, public BulletThrowable, public PlaceableObj
{
public:
	// width & height in cells
    Gadget(cpt3f& a_pos);
    inline virtual ~Gadget() {}
    
    inline static cstring&	getClass() { return strClass; }
	inline static int 		getReqOpUnitsNum() { return numReqOpUnits; }

	int						getOpUnitsNum();

	inline bool				isAnyAssigned() { return (v_vAssUnits.size() > 0); }
    inline bool				isFullyAssigned() {  return (getReqOpUnitsNum() == v_vAssUnits.size()); }
	inline bool				isOperated() { return (getReqOpUnitsNum() == getOpUnitsNum()); }

	void					assignUnit(BasicObject* a_pObj);
	void					deassignUnit(BasicObject* a_pObj);

	inline int				getAssUnitsNum() { return v_vAssUnits.size(); }
    
	virtual void			m_update() = 0;			

	virtual void 			m_draw(int a_nUserId, BITMAP* a_pDestBmp) = 0;

	bool					objFarOutsideGadget(BasicObject* a_pObj);

	virtual cpt3f 			m_getNewPosForObj(BasicObject* a_pObj) = 0;		

	inline cpt3f			m_getPos() { return v_pos; }
	inline cfloat			m_getWidth() { return width; }
	inline cfloat			m_getHeight() { return height;  }
    
	inline bool 			m_exists() { return (!v_tbDestroyed); }
    
protected:
	static bool				sinit();

	static string 			strClass;
    static int 				numReqOpUnits;
	static float			width;
	static float 			height;
	static uint				gTxtID;

	pt3f					v_pos;
	UNITS_VEC			 	v_vAssUnits;
	TextureObject			v_gTxtObj;
	bool 					v_tbDestroyed;
};


/*class CatapultGadget : public Gadget
{
}; */


// na razie tylko tego FIUTA bedziemy wykorzystywac
class CannonGadget : public Gadget
{
public:
	CannonGadget(cpt3f& a_pos, cvec3f& a_dir, cfloat& a_rng);
	virtual ~CannonGadget();

	void m_update();

	inline void setCanShoot(bool a_can) { v_canShoot = a_can; }
	inline bool canShoot() { return v_canShoot; }

// Tekstury mamy kierunkowo takie:
//   | / -  
//  rotujemy od wektora aktualnego do zadanego
// zakladamy ze kazdorazowo mozna sie obrocic o 1 stopien (/ 360)
	inline void		setDestDir(cvec3f& a_destDir) { destDir = a_destDir.normalize(); }
    
	inline cvec3f&	getDir() { return dir; }

	void 			m_draw(int a_nUserId, BITMAP* a_pDestBmp);

	inline void		setShootDelay(const uint& a_delay) { shootDelay = a_delay; }

	virtual cpt3f 	m_getNewPosForObj(BasicObject* a_pObj);

protected:
	static bool		_sinitOK;
	static bool		sinit();

protected:
	vec3f	dir;
	vec3f	destDir;
    float	rng;

    bool	v_canShoot;

	uint	shootDelay;	
	ulong	lastShoot;

	BITMAP* tmpBmp;
};


/* class PowerupGadget : public Gadget
{	
};*/


#endif // __GADGET_H__
