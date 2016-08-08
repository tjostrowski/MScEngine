#include <list>
#include <stack>
#include <vector>
#include <map>
#include <algorithm>
#include <set>
#include <allegro.h>
#include <math.h>
#include <assert.h>
#include <iostream>

using namespace std;

class Layer;
class MapGrid;
class BasicObject;
class DrawableObject;
class GridArea;
class GridElement;
class Position;
class BasicConfiguration;
class HexagonalMapGrid;
class TextureObject;
class TexturePattern;
class GameView;
class ConfigurationReader;
class ConfigurationWriter;
class MovementManager;
class Group;
class Formation;
class Behavior;
class PatrolBehavior;
class TraceBehavior;
class ExcapeBehavior;
class AttackBehavior;
class BasicObjectParamsManager;
class BasicObjectWorldInfoFrame;
class BasicObjectWorldInfoManager;
class BasicObjectCommunicationManager;
class BasicObjectBehaviorManager;
class BasicObjectWorldInfoReport;
class BasicObjectParamsReport;
class BasicObjectDecisionManager;
class Bullet;
class BulletClass;
class TimeFrameScheduledObject;
class Gadget;
class TextureObjTmpl;
class TacticalGroup;
class TGManager;
class CtrlObject;
class Idxable;
class Obstacle;
class MouseMsg;
class MouseEventManager;
class UIElem;
class RectangularUIElem;
class Pane;
class Button;
class Minimap;
class Flock;
class PlaceableObj;

template <class T> class LineMove;

/*struct BITMAP;*/
struct LINE;

typedef pair<int, int> 			INT_COORDS;
typedef pair<double, double>	FLOAT_COORDS;

typedef pair<float, float>		FFLOAT_COORDS;
	
typedef unsigned long			ulong;

typedef unsigned int			uint;

#define BETWEEN(X, A, B) (((X) >= (A)) && ((X) <= (B)))

#ifndef __POINT_STR
#define __POINT_STR

struct pt3f;

extern int dist(INT_COORDS p1, INT_COORDS p2);
extern float dist(const pt3f& pt1, const pt3f& pt2);

struct pt3f;
typedef const pt3f cpt3f;
typedef pt3f vec3f;
typedef const vec3f cvec3f;
typedef const float cfloat;


struct pt3f 
{
	float x, y, z;

	pt3f() :
		x(0.f), y(0.f), z(0.f) {}

	pt3f(const float& a_x, const float& a_y, const float& a_z) :
		x(a_x), y(a_y), z(a_z) {}

	bool operator==(const pt3f& pt)
	{
		return (pt.x == x && pt.y == y && pt.z == z);
	}
	bool operator!=(const pt3f& pt)
	{
		return !(*this == pt);
	}

	pt3f& operator*=(float a_f)
	{
		x *= a_f;
		y *= a_f;
		z *= a_f;

		return (*this);
	}

	friend bool operator==(const pt3f& pt1, const pt3f& pt2);
	friend bool operator!=(const pt3f& pt1, const pt3f& pt2); 
	friend pt3f operator-(const pt3f& pt1, const pt3f& pt2);

	pt3f normalize() const
	{
		float div = sqrtf(x*x + y*y + z*z);
		assert(div != 0.f);

		return pt3f((x/div), (y/div), (z/div));
	}

	pt3f& operator+=(cpt3f& a_pt)
	{
		x += a_pt.x;
		y += a_pt.y;
		z += a_pt.z;

		return *this;	
	}

	pt3f& operator-=(cpt3f& a_pt)
	{
		x -= a_pt.x;
		y -= a_pt.y;
		z -= a_pt.z;

		return *this;	
	} 

	pt3f& operator/=(cfloat a_n)
	{
		assert(a_n != 0.f);

		x /= a_n;
		y /= a_n;
		z /= a_n;

		return *this;
	}

	friend pt3f operator+(const pt3f& a_pt1, const pt3f& a_pt2);

	inline pt3f ortho()
	{
		return orthoL();
	}

	inline pt3f orthoL()
	{
		return pt3f(-y, x, 0.f);
	}

	inline pt3f orthoR()
	{
		return pt3f(y, -x, 0.f);
	}

	inline float length() const
	{
		return dist(*this, pt3f(0.f, 0.f, 0.f));
	}

	friend float operator%(cvec3f& a_vec1, cvec3f& a_vec2);

	/*
	 * a_angle => zwracany kat w radianach (<= M_PI)
	 * a_sign => zwracany kier. obrotu => -1.f (w prawo), +1.f (w lewo)
     *
     */
	float getAngle(cvec3f& a_otherVec, float& a_angle, float& a_sign) const;

	/*
     * a_angle => obrot o kat w radianach (dodatnia wartosc kata to obrot w lewo, ujemna w prawo)
     * 
     */
	vec3f& rotateZ(cfloat& a_angle);

    friend ostream& operator<<(ostream &out, const pt3f& pt);

	friend vec3f operator-(cvec3f& a_vec);
};

#define INVALID_VEC vec3f(0.f, 0.f, -1.f)
#define INVALID_POS pt3f(0.f, 0.f, -1.f)
#define ZERO_VEC	vec3f(0.f, 0.f, 0.f)

extern pt3f operator*(const pt3f& a_pt, const float a_f);

#endif // __POINT_STR

template <typename T> T clamp(T val, const T& minVal, const T& maxVal);
#define clampi clamp<int>
#define clampf clamp<float>

#ifndef __RECTANGLE_STR
#define __RECTANGLE_STR
struct RECTANGLE
{
	INT_COORDS ulVertex;
	INT_COORDS drVertex;

	int x1() const
	{
		return ulVertex.first;
	}

	int y1() const
	{
		return ulVertex.second;
	}

	int x2() const
	{
		return drVertex.first;
	}

	int y2() const
	{
		return drVertex.second;
	}

	void x1(const int a_x1) 
	{
		ulVertex.first = a_x1;
	}

	void y1(const int a_y1)
	{
		ulVertex.second = a_y1;
	}

	void x2(const int a_x2)
	{
		drVertex.first = a_x2;
	}

	void y2(const int a_y2)
	{
		drVertex.second = a_y2;
	}

	int getWidth() const
	{
		return (drVertex.first - ulVertex.first);
	}

	int getHeight() const
	{
		return (drVertex.second - ulVertex.second);
	}

/**************	
	 ________
	|		 |
	|		 | 	
	|		_____________  
	|______|_|			 |
           |			 | 
		   |_____________| 	
 **************/

    bool crossing(const RECTANGLE& a_rect) const
	{
        int xm 	= ((x1() + x2()) >> 1);
		int axm = ((a_rect.x1() + a_rect.x2()) >> 1);

		int widthD2 	= (getWidth() >> 1); 
		int awidthD2 	= (a_rect.getWidth() >> 1);

		int ym 	= ((y1() + y2()) >> 1);
		int aym	= ((a_rect.y1() + a_rect.y2()) >> 1);

		int heightD2 	= (getHeight() >> 1);
		int aheightD2	= (a_rect.getHeight() >> 1);

		bool isCrossing = ((abs(xm - axm) <= widthD2 + awidthD2) &&
						   (abs(ym - aym) <= heightD2 + aheightD2));

		return isCrossing;
	}

	bool inside(const RECTANGLE& a_rect) const
	{
		bool isInside = BETWEEN(a_rect.x1(), x1(), x2()) &&
						BETWEEN(a_rect.x2(), x1(), x2()) &&
						BETWEEN(a_rect.y1(), y1(), y2()) &&
						BETWEEN(a_rect.y2(), y1(), y2());

		return isInside;
	}

	RECTANGLE ext(const int dx1, const int dy1, const int dx2, const int dy2) 
	{
		return RECTANGLE(
				clampi(x1() + dx1, 0, SCREEN_W),
				clampi(y1() + dx2, 0, SCREEN_W),
				clampi(x2() + dy1, 0, SCREEN_H),
				clampi(y2() + dy2, 0, SCREEN_H));
	}

	RECTANGLE() {}

	RECTANGLE(const int a_x1, const int a_y1, const int a_x2, const int a_y2)
	{
		x1(a_x1);
		y1(a_y1);
		x2(a_x2);
		y2(a_y2);	
	}		

	RECTANGLE(INT_COORDS a_ulVertex, int a_nWidth, int a_nHeight) :
		ulVertex(a_ulVertex)
	{
		drVertex = make_pair(a_ulVertex.first  + a_nWidth,
							 a_ulVertex.second + a_nHeight);
	}

	friend ostream& operator<<(ostream &out, const RECTANGLE& pt);
};
#endif // __RECTANGLE_STR

#ifndef __PATHELEM_STR
#define __PATHELEM_STR
class PATHELEM
{
public:
	INT_COORDS	gridElem;
	int		  	nResolution;

	PATHELEM(INT_COORDS a_gridElem, int a_nResolution) :
		gridElem(a_gridElem), nResolution(a_nResolution) {}

	PATHELEM() {}
    
	bool operator==(PATHELEM& a_pathElem)
	{
		return ((gridElem		== a_pathElem.gridElem) &&
				(nResolution	== a_pathElem.nResolution));
	}
};
#endif // __PATHELEM_STR

/*#ifndef __RGB_STR
#define __RGB_STR
typedef struct RGB_str
{
   unsigned char r, g, b;
} RGB;
#endif // __RGB_STR

#define PAL_SIZE 256

typedef RGB PALETTE[PAL_SIZE];*/

#define UNDEFINED -1

#define VELOCITY_MIN 1
#define VELOCITY_MAX 100

#define INVALID_COORDS (make_pair(-1, -1))

#define ERR_RESULT -1

#ifndef M_PI
#define M_PI 3.1415926535897932385
#endif

#define UNDEFINED_STR ""

extern const pt3f getCellCenterCoords(INT_COORDS a_gridElem);
extern const pt3f getCellLUCoords(INT_COORDS a_gridElem); 
extern const float getCellSizeX();
extern const float getCellSizeY();
extern GridElement* getGridElement(INT_COORDS a_coords);
extern bool posOutsideGrid(const pt3f& a_pos);
extern bool posInsideGrid(const pt3f& a_pos);

extern INT_COORDS getCellForPos(const pt3f& a_pos);

extern void drawBmp(const int a_nUserId,
					BITMAP* a_pDestBmp,
					BITMAP* a_pSrcBmp,
					INT_COORDS a_luCell,
					const vec3f& delta,
					const uint width,
					const uint height);

extern void drawBmp(const int a_nUserId,
					BITMAP* a_pDestBmp,
					BITMAP* a_pSrcBmp,
					cpt3f& a_pos, /* center pos*/
					const float width,
					const float height,
					bool whInCells,
					bool mapFromOvToView);
#ifndef __TRIPLE_H
#define __TRIPLE_H

template <class T1, class T2, class T3> 
	class triple;

template <class T1, class T2, class T3> bool
	operator==(const triple<T1, T2, T3>& a_t1, const triple<T1, T2, T3>& a_t2); 

template <class T1, class T2, class T3> bool
	operator!=(const triple<T1, T2, T3>& a_t1, const triple<T1, T2, T3>& a_t2);

template <class T1, class T2, class T3> 
	class triple
{
public:
	inline triple(T1 a, T2 b, T3 c) :
		first(a), second(b), third(c) {} 

	friend bool operator== <T1, T2, T3> (const triple<T1, T2, T3>& a_t1, const triple<T1, T2, T3>& a_t2);
	friend bool operator!= <T1, T2, T3> (const triple<T1, T2, T3>& a_t1, const triple<T1, T2, T3>& a_t2);

public:
	T1 first;
	T2 second;
	T3 third;
};

template<class T1, class T2, class T3> triple<T1, T2, T3> make_triple(T1 a, T2 b, T3 c)
{
	return triple<T1, T2, T3>(a, b, c);
}

template <class T1, class T2, class T3> bool operator==(const triple<T1, T2, T3>& a_t1, const triple<T1, T2, T3>& a_t2)
{
	return (a_t1.varFirst == a_t2.varFirst &&
			a_t1.varSecond == a_t2.varSecond &&
			a_t1.varThird == a_t2.varThird);
}

template <class T1, class T2, class T3> bool operator!=(const triple<T1, T2, T3>& a_t1, const triple<T1, T2, T3>& a_t2)
{
	return !(a_t1 == a_t2);
}

#endif // __TRIPLE_H

#ifndef __HELPER_
#define __HELPER_
// nie wiadomo dlaczego tego nie mozna dodac do common.h?
template <class T> bool vec_contains(const vector<T>& a_vec, const T& a_elem)
{
	typename vector<T>::const_iterator it;
    return (a_vec.end() != find(a_vec.begin(), a_vec.end(), a_elem));
}

#endif // __HELPER_

#define DEFAULT_LAYER_LEVEL 1

Layer* getDefaultLayer();

#ifndef __TEXTURE_OBJ_TMPL
#define __TEXTURE_OBJ_TMPL
struct TextureObjTmpl
{
	int		nTxtId;
	BITMAP*	pBmp;
    int 	nDirNum;	// vertical 
	int 	nStepNum;	// horizontal
	string 	strFileName;

	inline TextureObjTmpl() {}
	
	inline TextureObjTmpl(int a_nTxtId, BITMAP* a_pBmp, int a_nDN = 1, int a_nSN = 1, const char* a_strFileName = "") :
		nTxtId(a_nTxtId), pBmp(a_pBmp), nDirNum(a_nDN), nStepNum(a_nSN), strFileName(a_strFileName) {}
};

typedef TextureObjTmpl TxtOT;
#endif

// some default texture ids
extern uint G_DEF_FLY_TXT_ID;
extern uint G_DEF2_FLY_TXT_ID;
extern uint G_CATAPULT_TXT_ID;
extern uint BU_CATAPULT_TXT_IDS[];
extern uint G_CANNON_TXT_ID;
extern uint BU_CANNON_TXT_IDS[];
extern uint G_POWERUP_TXT_ID;
extern uint BU_POWERUP_TXT_IDS[];
extern uint G_UNIT_TXT_ID;
extern uint G_ENEMY_UNIT_TXT_ID;
extern uint G_TANK_TXT_ID;
extern uint G_ENEMY_TANK_TXT_ID;
extern uint G_BUTTON_JOIN_TXT_ID;
extern uint G_BUTTON_JOIN_CC_TXT_ID;
extern uint G_BUTTON_JOIN_MM_TXT_ID;
extern uint G_BUTTON_SPLIT_TXT_ID;
extern uint G_BUTTON_SPLIT_CC_TXT_ID;
extern uint G_BUTTON_SPLIT_MM_TXT_ID;
extern uint G_BUTTON_GO_TXT_ID;
extern uint G_BUTTON_GO_CC_TXT_ID;
extern uint G_BUTTON_GO_MM_TXT_ID;
extern uint G_UITLO_TXT_ID;
extern uint G_BOMB_TXT_ID;
extern uint G_BOMB_EXPL_TXT_ID;
extern uint G_HOUSE_TXT_ID;

// cursors
extern uint G_CURSOR1_TXT_ID;
extern uint G_CURSOR2_TXT_ID;

extern uint G_TXT_IDS[];

// => and texture templates
extern TxtOT G_DEF_FLY_TXT_TMPL;
extern TxtOT G_TEST_FLY_TXT_TMPL;
extern TxtOT G_CATAPULT_TXT_TMPL;
extern TxtOT BU_CATAPULT_TXT_TMPLS[];
extern TxtOT G_CANNON_TXT_TMPL;
extern TxtOT BU_CANNON_TXT_TMPLS[];
extern TxtOT G_POWERUP_TXT_TMPL;
extern TxtOT BU_POWERUP_TXT_TMPLS[];
extern TxtOT G_UNIT_TXT_TMPL;
extern TxtOT G_ENEMY_UNIT_TXT_TMPL;
extern TxtOT G_TANK_TXT_TMPL;

// cursors
extern TxtOT G_CURSOR1_TXT_TMPL;
extern TxtOT G_CURSOR2_TXT_TMPL;

extern TxtOT* G_TXT_TMPLS[];
extern uint g_numTxtTmpls;


template <class A, class B> pair<A, B> mp(const A& a, const B& b);

#define to2(__x) (__x * __x)

#define __TRACE_DRAW

extern pt3f mapOverallToViewCoords(const int& a_nUserId, cpt3f& ovCoords);
extern INT_COORDS mapOverallToViewCoords(const int& a_nUserId, const INT_COORDS& ovCoords);

#define DEFAULT_USER_ID 1

extern float ranf();

extern void draw_line(const int& a_nUserId, BITMAP* a_pDestBmp, cpt3f& objPos , cvec3f& vec, const int& col);
extern void draw_circle(const int& a_nUserId, BITMAP* a_pDestBmp, cpt3f& center, cfloat& rad, const int& col, bool fill );

#ifndef FLOAT_MAX
#define FLOAT_MAX (1.2E+38F)
#endif

extern bool __DRAW_TG;

extern GameView* getDefaultView();

extern float DEG2RAD(cfloat& a_deg);
extern float RAD2DEG(cfloat& a_rad); 

typedef const int cint;

typedef vector<INT_COORDS> INT_COORDS_VEC;

// zwraca wszystkie near celle o odleglosci od 'a_cell' w zakresie <1, a_dist>
extern int getDiffs(INT_COORDS a_cell, uint a_dist, INT_COORDS_VEC& a_nearCells);

extern BITMAP* pBuffer;

typedef const bool cbool;

typedef const ulong culong;
typedef const uint cuint;

typedef const int cint;

extern float TO_360(cfloat& angleD, cfloat& sign);

typedef const string cstring;

extern void placeObj(PlaceableObj* a_pPlObj);

extern void placeObj(PlaceableObj* a_pPlObj, cpt3f& a_pos, bool a_place);

extern bool freezeUpdate;

extern vec3f interpolateVec(cvec3f& a_stVec, cvec3f& a_endVec, cfloat& a_step);

#define UNCOUNT_VEC(__v) ((__v == ZERO_VEC) || (__v == INVALID_VEC))

