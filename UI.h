#ifndef __UI_H__
#define __UI_H__

#include "common_header.h"
#include <iostream>


class MouseMsg
{
public:
	inline MouseMsg(int a_flags, int a_mousex, int a_mousey) :
		flags(a_flags), mousex(a_mousex), mousey(a_mousey) {}

	inline virtual ~MouseMsg() {}
    
public:
	int flags;
	int mousex;
	int mousey;
};


class MouseControlledUI
{
public:
	virtual void onClick() = 0;
	virtual void onClickEnd() = 0;
	virtual void onMouseMove() = 0;

	virtual bool isMCOverMe(const int a_mousex, const int a_mousey) = 0;
};

typedef MouseControlledUI MCUI;


class UIElem
{
public:
	virtual void m_draw(int a_nUserId, BITMAP* a_pDestBmp) = 0;

	virtual int getWidth() const = 0;
	virtual int getHeight() const = 0;

	virtual INT_COORDS getLUCoords() const = 0;
	virtual void setLUCoords(const INT_COORDS& a_coords) = 0;

	inline bool isDocced() const { return docced;  }
	inline void setDocced(bool a_docced) { docced = a_docced; }

	virtual bool isCrossing(const RECTANGLE& a_rect) const = 0;

	virtual bool isPtInsideUIElem(const int a_x, const int a_y) = 0;

protected:
	bool docced;
};


class RectangularUIElem : public MCUI, public Idxable, public UIElem
{
public:
	inline RectangularUIElem(const int a_x1, const int a_y1, const int a_x2, const int a_y2, bool a_docced = true) :
		rect(a_x1, a_y1, a_x2, a_y2) 
	{
		docced = a_docced;

		assert(rect.x1() >= 0 && rect.x1() < rect.x2());
		assert(rect.y1() >= 0 && rect.y1() < rect.y2());
	}

	inline RectangularUIElem(const RECTANGLE& a_rect, bool a_docced = true) :
		rect(a_rect)
	{
		docced = a_docced;

		assert(rect.x1() >= 0 && rect.x1() < rect.x2());
		assert(rect.y1() >= 0 && rect.y1() < rect.y2());
	} 
	
	inline virtual ~RectangularUIElem() {}
	
	virtual void onClick() = 0;
	virtual void onClickEnd() = 0;
	virtual void onMouseMove() = 0;

	virtual bool isMCOverMe(const int a_mousex, const int a_mousey);

	virtual bool isPtInsideUIElem(const int a_x, const int a_y);

	virtual void m_draw(int a_nUserId, BITMAP* a_pDestBmp) = 0;

	inline int getWidth() const { return rect.getWidth(); }
    inline int getHeight() const { return rect.getHeight(); }

	virtual bool isCrossing(const RECTANGLE& a_rect) const;

	inline virtual INT_COORDS getLUCoords() const
	{
		return mp(rect.x1(), rect.y1());
	}

	inline virtual void setLUCoords(const INT_COORDS& a_coords)
	{
		int rw  = rect.getWidth();
		int rh  = rect.getHeight();
		rect = RECTANGLE(a_coords, rw, rh);
	}

protected:
	RECTANGLE rect;
};


enum ePlace
{
	E_N, E_NE, E_NW, E_E, E_W, E_SE, E_SW, E_S
};


class Pane : public RectangularUIElem
{
public:
	inline Pane(const RECTANGLE& a_rect,
				const uint& a_txtId = (uint)UNDEFINED, 
				const int& a_col = UNDEFINED,
				const uint& a_border = 0,
				const uint& a_minElemDist = 0) :
		RectangularUIElem(a_rect, true),
		txtId(a_txtId),
		col(a_col),
		border(a_border),
		minElemDist(a_minElemDist)
	{
		init();
	}

    virtual ~Pane();
    

	void init()
	{
        if (txtId == UNDEFINED && col == UNDEFINED)
			col = makecol(12, 12, 12);

		borderCol = makecol(0, 255, 0);
	}

	// added?
	bool addUIElem(UIElem* a_pUIEl, const ePlace& place = E_NW);

	UIElem* getCrossingUIElem(const int x, const int y) const;

	virtual void m_draw(int a_nUserId, BITMAP* a_pDestBmp); 

	inline virtual void onClick() {}
	inline virtual void onClickEnd() {}
    inline virtual void onMouseMove() {}

protected:
	vector<UIElem*> vUIElems;
	uint border;
	uint minElemDist;
	uint txtId;
	int col;
	int borderCol;
	// WARN: rect => with borders
};


class Button : public RectangularUIElem
{
public:
	inline Button(const RECTANGLE& a_rect, 
				  bool a_docced, 
				  const uint& a_ssTxtId = UNDEFINED, 
				  const uint& a_ccTxtId = UNDEFINED,
				  const uint& a_mmTxtId = UNDEFINED) :
		RectangularUIElem(a_rect, a_docced),
		ssTxtId(a_ssTxtId), ccTxtId(a_ccTxtId), mmTxtId(a_mmTxtId) 
	{
		init();
	}

	void init() 
	{
		state = E_SIMPLE;
	}

	virtual void m_draw(int a_nUserId, BITMAP* a_pDestBmp);

	void forceREDRAW();

private:
	virtual void onClick();
	virtual void onClickEnd();
	virtual void onMouseMove();

protected:
	inline virtual void __onClick() {}
    inline virtual void __onMouseMove() {}
    
protected:
	uint ssTxtId; // 's''imple txt id
	uint ccTxtId; // 'c'li'c'ked txt id
	uint mmTxtId; // 'm'ouse 'm'oved txt id

	enum eButtonState
	{
		E_SIMPLE = 0,
		E_CLICKED,
		E_MOUSE_MOVE
	};

	uint state;
};


class ButtonJoin : public Button
{
	friend class MouseEventManager;

public:
	inline ButtonJoin(const RECTANGLE& a_rect, 
					  bool a_docced, 
					  const uint& a_ssTxtId = UNDEFINED, 
					  const uint& a_ccTxtId = UNDEFINED,
					  const uint& a_mmTxtId = UNDEFINED) :
		Button(a_rect, a_docced, a_ssTxtId, a_ccTxtId, a_mmTxtId) {}

protected:
	static void proc_REG_ONCLICK(const int x, const int y);

	static int numClicked;

	static vector<TG*> tgsToJoin;

protected:
	virtual void __onClick(); 
};


class ButtonSplit : public Button
{
public:
	inline ButtonSplit(const RECTANGLE& a_rect, 
					   bool a_docced, 
					   const uint& a_ssTxtId = UNDEFINED, 
					   const uint& a_ccTxtId = UNDEFINED,
					   const uint& a_mmTxtId = UNDEFINED) :
		Button(a_rect, a_docced, a_ssTxtId, a_ccTxtId, a_mmTxtId) {}

protected:
	static void proc_REG_ONCLICK(const int x, const int y);
	static int numClicked;

protected:
	virtual void __onClick();
};


class ButtonGo : public Button
{
public:
	inline ButtonGo(const RECTANGLE& a_rect, 
					bool a_docced, 
					const uint& a_ssTxtId = UNDEFINED, 
					const uint& a_ccTxtId = UNDEFINED,
					const uint& a_mmTxtId = UNDEFINED) :
		Button(a_rect, a_docced, a_ssTxtId, a_ccTxtId, a_mmTxtId) {}

protected:
	static void proc_REG_ONCLICK(const int x, const int y);
	static int numClicked;	
	static TG* tgToMove;

protected:
	virtual void __onClick();
};


class Minimap
{
};


typedef void (*REG_ONCLICK)(const int x, const int y);

class MouseEventManager
{
public:
	MouseEventManager();
	inline virtual ~MouseEventManager() {}
    
	void addMsg(const MouseMsg& a_msg);

	inline void register_REG_ONCLICK(REG_ONCLICK a_proc)
	{
		proc_REG_ONCLICK = a_proc;
	}

protected:
	vector<MouseMsg> messages;

	REG_ONCLICK proc_REG_ONCLICK;
};

extern MouseEventManager  g_mouseEvManager;
extern MouseEventManager* g_pMEM;


class RegUIs
{
public:
	inline static RegUIs* m_getRegUIs()
	{
		if ((RegUIs*)0 == v_pUIs)
			v_pUIs = new RegUIs();
        
		return v_pUIs;
	}

	// added?
	bool addPane(Pane* a_pP);

	bool isPtInsideUI(cpt3f& a_pt);
	bool isPtInsideUI(const int x, const int y);

	Pane* getCrossingPane(const int x, const int y) const;

	UIElem* getCrossingUIElem(const int x, const int y) const;

	void m_draw(int a_nUserId, BITMAP* a_pDestBmp);

protected:
	inline RegUIs() {} 
	virtual ~RegUIs();

protected:
	static RegUIs* v_pUIs;

	vector<Pane*>  v_vPanes;
};

#endif // __UI_H__

