#ifndef __BASICOBJECT_H__
#define __BASICOBJECT_H__

#include "common_header.h"
#include "DrawableObject.h"
#include "BasicObjectParamsManager.h"
#include "Behavior.h"
#include "TacticalGroup.h"


//
//         
// 
// 

extern string 	G_LM_NAME;
extern uint 	G_LM_ANIMATION;


// simple behavior including only one animation
// (only one activity)
class SimpleBehavior : public Behavior
{	
public:
	SimpleBehavior(BasicObject* a_pObj, const int a_nTxtId, CtrlObject* a_pCtrlObj = (CtrlObject*)0);

	inline virtual void	m_do() {}
	
	virtual BITMAP*		m_getCurrentAnimationFrame();

	virtual cpt3f		m_getNewPos();

protected:
	// returns if INIT was successful
	bool 				m_init(const int a_nTxtId, CtrlObject* a_pCtrlObj);

};


// basic class representing obstacle shapes
// now obstacles => circles around objects
class Obstacle
{
public:
	Obstacle(BasicObject* a_pObj);

	inline cpt3f 	getCenter() const { return obsCenter; }
	inline cfloat	getRadius() const { return obsRadius; }

	static bool isColliding(const Obstacle& a_obs1, const Obstacle& a_obs2); 

protected:
	pt3f 	obsCenter;
	float	obsRadius;
};


// basic object representing simple units or buildings
class BasicObject : public DrawableObject, public Idxable, public BulletThrowable, public PlaceableObj
{
public:
	enum DIR {
		DIR_S	= 0,
		DIR_SE	= 1,
		DIR_E	= 2,
		DIR_NE	= 3,
		DIR_N	= 4,
		DIR_NW	= 5,
		DIR_W	= 6,
		DIR_SW	= 7};
    
public:
	BasicObject(const int a_nId);
	// useful constructor for handling objects
	// that cover one grid element
	BasicObject(const int a_nId, INT_COORDS a_pos);
	// usuful constructor for handling objects
	// that cover one cell and 
	// is textured by only one texture object
	BasicObject(const int a_nId, INT_COORDS a_pos, TextureObject& a_textObj);

	// defTxtId -- defualt texture id (used when no behavior specified)
	// ass => 0 == comp, 1 == human player
	BasicObject(cpt3f& a_pos, const uint& a_defTxtId = G_UNIT_TXT_ID, cfloat& a_width = 1.f, cfloat& a_height = 1.f, const uint& a_ass = 0);

	virtual ~BasicObject();

protected:
	void 						m_initDefaultParams();

public:
	virtual void				m_draw(int a_nUserId, BITMAP* a_pDestBmp);
	virtual void				m_applyToGrid(MapGrid* a_pGrid, int a_nLayerLevel);

	/**
     * Checks if element is within specified convex polygon
     * 
	 * @param a_vCoords
	 * @param a_elem
	 * 
	 * @return bool
	 */
	static int 					m_checkElementWithinObject(vector<INT_COORDS>& a_rvObjVertices, INT_COORDS a_gridElem);

	/**
     * Move the object with vector [nX, nY]. Returns the actual
     * vector with which movement >> was done <<.
     * 
	 * @param nX
	 * @param nY
	 * 
	 * @return INT_COORDS
	 */
	INT_COORDS					m_move(int nX, int nY);

	/**
     * Rotate the object
     *
	 */
	inline void					m_rotate() {}
    
	inline void					m_setLayerLevel(int a_nLayerLevel) { v_nLayerLevel = a_nLayerLevel; }
	inline int					m_getLayerLevel() { return v_nLayerLevel;  }
    
	void						m_setVertices(vector<INT_COORDS>& a_vVertices);	
    inline vector<INT_COORDS>&	m_getVertices() { return v_vVertices; }

    vector<INT_COORDS>& 		m_getCoveredElems() { return v_vCoveredElems; }

	void						m_getFOV(float& a_fovRAD, float& a_fovANG);

    /**
     * If object can move
     * 
     */
public:
	inline bool 				m_isDynamic() { return v_bDynamic; }
	inline void 				m_setDynamic(bool a_bDynamic) { v_bDynamic = a_bDynamic; }

	INT_COORDS 					m_getPosition();
    
    inline cfloat				m_getWidth() { return v_width; }
	inline cfloat				m_getHeight() { return v_height; }
    
	/**
     * Get grid position of object at frame number a_nStep counting
     * from 0 (NOW)
     * 
     */
	INT_COORDS 					m_getPosition(int a_nStep);

	inline vector<PATHELEM>&	m_getActualPath() { return v_vActualPath; }
	inline void					m_setActualPath(vector<PATHELEM>& a_rvPath) { v_vActualPath = a_rvPath; }
	static void					m_replacePath(int a_nBeginPos, 
											  int a_nEndPos,
											  vector<PATHELEM>& a_rvPath,
											  vector<PATHELEM>& a_rvActualPath);
	void 						m_replacePath(int a_nBeginPos, int a_nEndPos, vector<PATHELEM>& a_rvPath);	

	INT_COORDS					m_nextStep();

	INT_COORDS					m_getActualTarget();
	void						m_setActualTarget(INT_COORDS a_coords, bool a_bRecomputePath = true);

	inline virtual cpt3f		m_getPos() { return v_pos;  }
    inline void					m_setPos(cpt3f& a_pos) { v_pos = a_pos; }
	
    /**
     * Set coords for next object step.
     * 
	 * @param a_coords
	 */
	void						m_setNextStep(INT_COORDS a_newObjPos, bool a_bCheckValid = false);

	bool						m_isControlledByTG(TG** a_tg);

	inline void					setDestDir(cvec3f& a_destDir) { destDir = a_destDir; }
    inline cvec3f				getDestDir() { return destDir; }
	inline cvec3f				getDir() { return dir; }

public:
	// BASIC METHOD for object management
	void						m_update();

protected:
	void						m_drawObjectLM(int a_nUserId, BITMAP* a_pDestBmp);
	void						m_drawObjectTextured(int a_nUserId, BITMAP* a_pDestBmp);
	void						m_drawObjectSolid(int a_nUserId, BITMAP* a_pDestBmp);

    bool						m_requiresCoveredElemsRecalculation();
	void						m_recalculateCoveredElems();

	void						m_calculateBoundingRectangle();

public:
	static int					m_handleChangeDir(INT_COORDS a_dest, INT_COORDS a_src);

	static int					m_handleChangeDir(double a_fAngle);
    
public:
	bool						m_isStandingStill();

	void						switchToBehavior(const string& behaviorName, void* pData);

	inline Obstacle 			m_getObstacle() { return Obstacle(this); }

	inline static cfloat		getRefSpeed() { return (getCellSizeX() / 2.f); }
    inline cfloat				m_getSpeed() { return v_vel.length(); }
	inline Flock*				m_getFlock() { return v_pFl; }	
	inline cvec3f				m_getVel() { return v_vel;  }
    
	void						delObjRef();

	void						setStatic() { isStatic = true;  }
	inline bool					_isStatic() { return isStatic;  }
    
public:
	// managers:
	inline BasicObjectParamsManager* m_getParamsManager() { return v_pParamsManager; }
    inline BasicObjectCommunicationManager* m_getCommunicationManager() { return v_pCommunicationManager; }
	inline BasicObjectWorldInfoManager* m_getWorldInfoManager() { return v_pWorldInfoManager; }
    inline BasicObjectBehaviorManager* m_getBehaviorManager() { return v_pBehaviorManager; }
	inline BasicObjectDecisionManager* m_getDecisionManager() { return v_pDecisionManager; }

public:
	bool m_exists();
    
protected:
    
    // These are vertice edges that form
	// specific basic object
	// starting from vertex with coordinates (0,0)
	// then (x1, y1), (x2, y2), ... .
	// When  moving basic object that spans multiple vertices
	// taking into account vertex (0, 0).
	// !!!: those vertices are in clockwise direction
	vector<INT_COORDS>					v_vVertices;

	// These are grid elements that are covered 
	// by this basic object
	vector<INT_COORDS>					v_vCoveredElems; 

    // actual path to follow in next steps
	// (not counting actual position)
	vector<PATHELEM>					v_vActualPath;	 

	int									v_nLayerLevel;

	int									v_nXMin;
	int									v_nXMax;
	int									v_nYMin;
	int									v_nYMax;

	bool								v_bDynamic;

	int									v_nDir;
	int									v_nFrame;

	INT_COORDS							v_actualTarget;

public:
	inline uint							m_getAssign() { return assign; }

protected:
	// managers: PO CHUJ!
	BasicObjectParamsManager*			v_pParamsManager;
	BasicObjectWorldInfoManager*		v_pWorldInfoManager;
	BasicObjectCommunicationManager*	v_pCommunicationManager;
	BasicObjectBehaviorManager* 		v_pBehaviorManager;
	BasicObjectDecisionManager*			v_pDecisionManager;

public:
	friend ostream&						operator<<(ostream& os, const BasicObject& obj);

	inline void							touch(BasicObject* a_pObj) {}
    void								touch(Bullet* a_pBullet);
	
	void								m_reconfigureToBehavior(Behavior* a_pBehavior);

	Behavior*							v_pBehavior;

	// v_pos => center of object
	pt3f								v_pos;

    // width & height in cells
	float								v_width;
	float								v_height;

	uint								assign;

	bool								touched;

	Flock*								v_pFl;
	vec3f								v_vel;

	vec3f								destDir;
	vec3f								dir;

	bool								isStatic;
};

typedef BasicObject Unit;

#endif // __BASICOBJECT_H__
