#include "common.h"


uint Idxable::IDRef = 1;


void BulletThrowable::m_throwBullet(cpt3f& a_endPos, BulletClass* a_pClass /*= &G_DEF_BULLET_CLASS*/, bool a_bCollide /* = true */)  
{
	Layer* pDL = getDefaultLayer();
	cpt3f pos = m_getPos();

    Bullet* pBU = new Bullet(pos, a_endPos, a_pClass, a_bCollide);
	assert((Bullet*)0 != pBU);
#ifdef __TRACE_ME
	//cout << *pBU << endl;
#endif

    pDL->m_addBullet(pBU);
}

DrawableObject::DrawableObject() :
	v_bTextured(false), v_bFillAll(false)
{
	v_nColor = m_getDefaultColor();
}

void DrawableObject::m_setColor(int a_nRed, int a_nGreen, int a_nBlue)
{
	v_nColor = makecol(a_nRed, a_nGreen, a_nBlue);
}

int DrawableObject::m_getDefaultColor()
{
	return makecol(0, 0, 0);
}

