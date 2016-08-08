#include "common.h"


#undef __SHOW_OBSTACLE


// SimpleBehavior: !!!
SimpleBehavior::SimpleBehavior(BasicObject* a_pObj, const int a_nTxtId, CtrlObject* a_pCtrlObj) :
	Behavior(a_pObj)
{
	assert((BasicObject*)0 != a_pObj);
	assert(a_nTxtId >= 0);

	v_pCtrlObj = a_pCtrlObj;

	bool initSuccess = m_init(a_nTxtId, a_pCtrlObj);

	if (!initSuccess)
		return;
}

bool SimpleBehavior::m_init(const int a_nTxtId, CtrlObject* a_pCtrlObj)
{
	TextureObject txtObj;
	bool txtFound = makeTextureObj(a_nTxtId, txtObj);
	if (!txtFound)
		return false;

	v_mapActivities.insert(make_pair(0, txtObj));
	
    return true;
}

eDir getDirForAngle(cfloat angleD /* 0.f - 360.f*/)
{
	eDir dir = E_DIR_INVALID;
	cfloat an45Div2 = (45.f / 2.f);
	if (BETWEEN(angleD, an45Div2, 3.f * an45Div2))
		dir = E_DIR_NE;
	else if (BETWEEN(angleD, 3.f * an45Div2, 5.f * an45Div2))
		dir = E_DIR_N;
	else if (BETWEEN(angleD, 5.f * an45Div2, 7.f * an45Div2))
		dir = E_DIR_NW;
	else if (BETWEEN(angleD, 7.f * an45Div2, 9.f * an45Div2))
		dir = E_DIR_W;
	else if (BETWEEN(angleD, 9.f * an45Div2, 11.f * an45Div2))
		dir = E_DIR_SW;
	else if (BETWEEN(angleD, 11.f * an45Div2, 13.f * an45Div2))
		dir = E_DIR_S;
	else if (BETWEEN(angleD, 13.f * an45Div2, 15.f * an45Div2))
		dir = E_DIR_SE;
	else
		dir = E_DIR_E;

	return dir;
}

BITMAP* SimpleBehavior::m_getCurrentAnimationFrame()
{
	assert(v_mapActivities.size() == 1);

	TextureObject& txtObj = v_mapActivities[0];

	cvec3f destDir = v_pObj->getDestDir();
    if (destDir != INVALID_VEC)
	{
		draw_line(DEFAULT_USER_ID, pBuffer, v_pObj->m_getPos(), destDir * 1000.f, makecol(255, 255, 0));

		// mamy sie obrocic do okreslonego kierunku
		vec3f intVec = interpolateVec(MAP2VEC(txtObj.m_getDir()), destDir, 45.f);
		if (intVec == INVALID_VEC)
			return txtObj.m_getActualBitmap();

        cvec3f tstVec(1.f, 0.f, 0.f);
		float NNangleR, NNsign;
		intVec.getAngle(tstVec, NNangleR, NNsign);
		eDir NNdir = getDirForAngle(TO_360(RAD2DEG(NNangleR), NNsign));

        txtObj.resetDir(NNdir);

        return txtObj.m_getActualBitmap();
	}

	// pomagamy sobie flockiem
	Flock* pFL = v_pObj->m_getFlock();
	assert((Flock*)0 != pFL);
	
	cvec3f estVel = pFL->m_getEstVel();
	cvec3f lastVel = pFL->m_getLastVel();
	cvec3f tstVec(1.f, 0.f, 0.f);

	if (estVel == INVALID_VEC || estVel == ZERO_VEC)
		return txtObj.m_getActualBitmap();

	if (lastVel == INVALID_VEC || lastVel == ZERO_VEC)
	{
		float EEangleR, EEsign;
		estVel.getAngle(tstVec, EEangleR, EEsign);
		float EEangleD = TO_360(RAD2DEG(EEangleR), EEsign);
		eDir EEdir = getDirForAngle(EEangleD);
		txtObj.resetDir(EEdir);

		return txtObj.m_getActualBitmap();
	}

	float LLangleR, LLsign;
	lastVel.getAngle(tstVec, LLangleR, LLsign);
	float LLangleD = TO_360(RAD2DEG(LLangleR), LLsign);

	float EEangleR, EEsign;
	estVel.getAngle(tstVec, EEangleR, EEsign);
	float EEangleD = TO_360(RAD2DEG(EEangleR), EEsign);

	eDir LLdir = getDirForAngle(LLangleD);
	eDir EEdir = getDirForAngle(EEangleD);

	if (LLdir == EEdir)
		txtObj.incDirStep();
	else
		txtObj.resetDir(EEdir);

	return txtObj.m_getActualBitmap();
}

cpt3f SimpleBehavior::m_getNewPos()
{
    CtrlObject* pCtrlObj = v_pCtrlObj;
	assert((CtrlObject*)0 != pCtrlObj);
    
    return pCtrlObj->m_getNewPosForObj(v_pObj);
}


// Obstacle: !!!
Obstacle::Obstacle(BasicObject* a_pObj)
{
	assert((BasicObject*)0 != a_pObj);
	obsCenter = a_pObj->m_getPos();
	cfloat objWidth 	= a_pObj->m_getWidth();
	cfloat objHeight	= a_pObj->m_getHeight();
	cfloat cellSizeX = getCellSizeX();
	obsRadius = 1.2 * (sqrtf(to2(objWidth) + to2(objHeight))) * cellSizeX;
}

bool Obstacle::isColliding(const Obstacle& a_obs1, const Obstacle& a_obs2)
{
	return (dist(a_obs1.getCenter(), a_obs2.getCenter()) 
			<  a_obs1.getRadius() + a_obs2.getRadius());	
}


// BasicObject: !!!
void BasicObject::m_reconfigureToBehavior(Behavior* a_pBehavior)
{
	assert((Behavior*)0 != a_pBehavior);

	SimpleBehavior* pSB = dynamic_cast<SimpleBehavior*>(a_pBehavior);
	if ((SimpleBehavior*)0 != pSB)
	{
		BasicObjectBehaviorManager* pManager = v_pBehaviorManager;
		assert((BasicObjectBehaviorManager*)0 != pManager);
		pManager->m_startNewBehavior(pSB);
	}

	return;
}

BasicObject::BasicObject(const int a_nId)
{
	m_initDefaultParams();
}

BasicObject::BasicObject(const int a_nId, INT_COORDS a_pos)
{
	v_vVertices.clear();
	v_vVertices.push_back(a_pos);

	m_initDefaultParams();
}

BasicObject::BasicObject(const int a_nId, INT_COORDS a_pos, TextureObject& a_txtObj)
{
	v_vVertices.clear();
	v_vVertices.push_back(a_pos);

	v_texturePattern.m_addTextureObj(a_txtObj);

	// covering grid elem
	BasicConfiguration* pConf	= BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid 				= pConf->m_getGridMap();
	GridElement* pEl 			= const_cast<GridElement*>(pGrid->m_getGridElement(a_pos));
	
	m_initDefaultParams();
}

BasicObject::BasicObject(cpt3f& a_pos, const uint& a_txtId /* = G_UNIT_TXT_ID */, cfloat& a_width /* = 1.f*/, cfloat& a_height /* = 1.f */, const uint& a_ass /* = 0*/) :
	assign(a_ass)
{
	assert(a_pos != INVALID_POS);
	assert(a_txtId >= 0);
	assert(a_width > 0.f && a_height > 0.f);

	assert(posInsideGrid(a_pos));

    v_pos 		= a_pos;
	v_width 	= a_width;
	v_height	= a_height;

	TextureObject txtObj;
	bool txtFnd = makeTextureObj(a_txtId, txtObj);
	if (txtFnd)
		v_texturePattern.m_addTextureObj(txtObj);

    m_initDefaultParams();
}

void BasicObject::m_initDefaultParams()
{
	v_bDynamic  			= false;
    v_nFrame 				= 0;
	v_nDir					= DIR_S;
	v_actualTarget			= INVALID_COORDS;

	v_pParamsManager 		= new BasicObjectParamsManager(this);
	v_pParamsManager->m_setFrameGap(5);
	v_pWorldInfoManager		= new BasicObjectWorldInfoManager(this);
	v_pWorldInfoManager->m_setFrameGap(5);
	v_pCommunicationManager	= new BasicObjectCommunicationManager(this);
	v_pCommunicationManager->m_setFrameGap(5);
	v_pBehaviorManager		= new BasicObjectBehaviorManager(this);
	v_pBehaviorManager->m_setFrameGap(5);
	v_pDecisionManager		= new BasicObjectDecisionManager(this);
	v_pDecisionManager->m_setFrameGap(5);

    v_nLayerLevel			= DEFAULT_LAYER_LEVEL;

    v_bTextured				= true;

	GridElement* coveredGE 	= getGridElement(getCellForPos(v_pos));
	
	assert((GridElement*)0 != coveredGE);
	
	//coveredGE->m_setCovered(this, true); // ???????????????

	touched = false;

    v_pFl = new Flock(this);
	v_vel = ZERO_VEC;

	destDir = INVALID_VEC;
	dir = INVALID_VEC;


	isStatic = false;
}

BasicObject::~BasicObject()
{
	delete v_pParamsManager;
	delete v_pWorldInfoManager;
	delete v_pCommunicationManager;
	delete v_pBehaviorManager;
	delete v_pDecisionManager;
	delete v_pFl;
}

void BasicObject::m_draw(int a_nUserId, BITMAP* a_pDestBmp)
{
    if (m_requiresCoveredElemsRecalculation())
		m_recalculateCoveredElems();
	
    if (m_isTextured())
		m_drawObjectTextured(a_nUserId, a_pDestBmp);
	else
		m_drawObjectSolid(a_nUserId, a_pDestBmp);


	if (touched)
	{
		draw_circle(a_nUserId, a_pDestBmp, m_getPos(), 15.f, makecol(255, 0, 0), false);
		touched = false;
	}

#ifdef __SHOW_OBSTACLE
	Obstacle obs = this->m_getObstacle();
	cpt3f coords = mapOverallToViewCoords(a_nUserId, obs.getCenter());

	if (coords != INVALID_POS)
		circle(a_pDestBmp,
			   (int)(coords.x),
			   (int)(coords.y),
			   (int)(obs.getRadius()),
			   makecol(255, 0, 0));
#endif

	return;
}

INT_COORDS BasicObject::m_move(int nX, int nY)
{
	vector<INT_COORDS>::iterator it;
	for (it = v_vVertices.begin(); it != v_vVertices.end(); ++it)
	{
		INT_COORDS vertex = *it;
		vertex.first  += nX;
		vertex.second += nY;	
		*it = vertex;
	}

    return make_pair(nX, nY);
}

void BasicObject::m_drawObjectTextured(int a_nUserId, BITMAP* a_pDestBmp)
{
	// NOW ADDED
	

	Behavior* pBehavior = v_pBehaviorManager->m_getActualBehavior();
	if ((Behavior*)0 != pBehavior)
	{
		BITMAP* pFrame = pBehavior->m_getCurrentAnimationFrame();
		cpt3f& pos = v_pos;

		drawBmp(a_nUserId,
				a_pDestBmp,
				pFrame,
				pos, 
				v_width /* width in cells */,
				v_height /* height in cells */,
				true,
				true);

		return;
	}

	if (v_pos != INVALID_POS)
	{
		assert(v_texturePattern.m_getTextureObjs().size() >= 1);
		BITMAP* pDefTxt = (v_texturePattern.m_getTextureObjs())[0].m_getActualBitmap();

		drawBmp(a_nUserId,
				a_pDestBmp,
				pDefTxt,
				v_pos, 
				v_width /* width in cells */,
				v_height /* height in cells */,
				true,
				true);

		return;
	}

	// END

    BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();
	GameView* pView = pConf->m_getGameViewByUserId(a_nUserId);

	int nGridElemSizeX = pGrid->m_getGridElementSizeX();
	int nGridElemSizeY = pGrid->m_getGridElementSizeY();

    vector<TextureObject>& vTextureObjs = v_texturePattern.m_getTextureObjs();

    if (vTextureObjs.size() == 1)
	{
		TextureObject textureObj = vTextureObjs[0];
		BITMAP* pSrcBmp = 0;
		if (v_bDynamic)
		{
			// TODO: to fix
			pSrcBmp = textureObj.m_getAnimSubbitmap(v_nDir, v_nFrame);
				//pSrcBmp = textureObj.m_getBitmap();
		}
		else
		{
			pSrcBmp = textureObj.m_getBitmap();
		}
		// simple common case 1:
		// texture spans all basic object rectangle
        if (is_poly_rectangle(v_vVertices) &&
			m_isFillAll())
		{
			int nWidth 	= v_vVertices[0].first - v_vVertices[1].first + 1;
			int nHeight	= v_vVertices[3].first - v_vVertices[0].first + 1;

			INT_COORDS gridCoords = v_vVertices[0];
			INT_COORDS ovCoords = pView->m_mapGridElementToOverallViewCoords(gridCoords);
			if (pView->m_isPointWithingClipRectangle(ovCoords))
			{
				INT_COORDS coords = pView->m_mapFromOverallToViewCoords(ovCoords);
				ext_blit(
					pSrcBmp, 
					a_pDestBmp,
					0,
					0, 	
					pSrcBmp->w,
					pSrcBmp->h,
					coords.first - (nGridElemSizeX >> 1),
					coords.second - (nGridElemSizeY >> 2),
					nGridElemSizeX * nWidth,
					((nGridElemSizeY >> 2) * 3) * nHeight);					
			}
		}
		// simple common case 2:
		// 1x1 texture is repeated through the basic object 
		// in every grid cell
		else if (1 == textureObj.m_getHorizontalLineNum() &&
				 1 == textureObj.m_getVerticalLineNum() &&
				 !m_isFillAll())
		{
			vector<INT_COORDS>::const_iterator it;
			for (it = v_vCoveredElems.begin(); it != v_vCoveredElems.end(); ++it)
			{
				INT_COORDS gridCoords = *it;
				INT_COORDS ovCoords = pView->m_mapGridElementToOverallViewCoords(gridCoords);
				if (pView->m_isPointWithingClipRectangle(ovCoords))
				{
					INT_COORDS coords = pView->m_mapFromOverallToViewCoords(ovCoords);
					ext_blit(
						pSrcBmp, 
						a_pDestBmp,
						0,
						0, 	
						pSrcBmp->w,
						pSrcBmp->h,
						coords.first - (nGridElemSizeX >> 1),
						coords.second - (nGridElemSizeY >> 2),
						nGridElemSizeX,
						((nGridElemSizeY >> 2) * 3));					
				}
			}
		}
		// simple common case 3:
		// one NNxMM texture filling for all object
		// texture boundaries must be in v_vVertices
		// (texture could not have rectangular shape)
		else 
		{
            int nWidth	= textureObj.m_getHorizontalLineNum();
			int nHeight	= textureObj.m_getVerticalLineNum();

			if (nWidth <= 0 || nHeight <= 0)
			{
				return;
			}

			int nXMin_Rect = INT_MAX;
			int nYMin_Rect = INT_MAX;
			vector<INT_COORDS>::iterator it;
			for (it = v_vVertices.begin(); it != v_vVertices.end(); ++it)
			{
				INT_COORDS gridCoords = *it;
				INT_COORDS ovCoords = pView->m_mapGridElementToOverallViewCoords(gridCoords);
				int nX_Rect = ovCoords.first;
				int nY_Rect = ovCoords.second;
				nXMin_Rect = min(nXMin_Rect, nX_Rect);
				nYMin_Rect = min(nYMin_Rect, nY_Rect);
			}

			INT_COORDS ovCoords_UL = make_pair(nXMin_Rect, nYMin_Rect);	

			if (pView->m_isRectangleCrossClipRectangle(RECTANGLE(ovCoords_UL, 
																 nWidth * nGridElemSizeX,
																 nHeight * nGridElemSizeY)))
			{
				if (m_isFillAll())
				{
					INT_COORDS coords = pView->m_mapFromOverallToViewCoords(ovCoords_UL);
					ext_masked_blit(
						pSrcBmp, 
						a_pDestBmp,
						0,
						0, 	
						pSrcBmp->w,
						pSrcBmp->h,
						coords.first - (nGridElemSizeX >> 1),
						coords.second - (nGridElemSizeY >> 2),
						nGridElemSizeX * nWidth,
						((nGridElemSizeY >> 2) * 3) * nHeight);
				}
				else
				{
					RECTANGLE clipRect = pView->m_getClippingRectangle();
					int iMin = 
						max(((clipRect.ulVertex.first - ovCoords_UL.first) / nGridElemSizeX) - 1, 0);
					int jMin = 
						max(((clipRect.ulVertex.second - ovCoords_UL.second) / ((nGridElemSizeY >> 2) * 3)) - 1, 0);
					int iCovered = (clipRect.getWidth() / nGridElemSizeX) + 4;
					int jCovered = (clipRect.getHeight() / ((nGridElemSizeY >> 2) * 3)) + 4;
					for (int i = iMin; i < iMin + iCovered; ++i)
					{
						for (int j = jMin; j < jMin + jCovered; ++j)
						{
							INT_COORDS coords = pView->m_mapFromOverallToViewCoords(
								make_pair(ovCoords_UL.first + i * nGridElemSizeX,
										  ovCoords_UL.second + j * ((nGridElemSizeY >> 2) * 3)));
							{
								ext_blit(
									pSrcBmp, 
									a_pDestBmp,
									0,
									0, 	
									pSrcBmp->w,
									pSrcBmp->h,
									coords.first - (nGridElemSizeX >> 1),
									coords.second - (nGridElemSizeY >> 2),
									nGridElemSizeX,
									((nGridElemSizeY >> 2) * 3));
							}
						}
					}
				}
			}
		}
	}
	else
	{
		// most general case:
		int nRectWidth 	= (v_nXMax - v_nXMin) * nGridElemSizeX;
		int nRectHeight = (v_nYMax - v_nYMin) * nGridElemSizeY;

		BITMAP* pTmpBmp = pConf->m_getTmpBitmap(nRectWidth, nRectHeight);
		if (0 == pTmpBmp)
		{
			// cannot allocate temporary bitmap
			return;
		}

		INT_COORDS coords = pGrid->m_mapGridElementToCoords(make_pair(v_nXMin, v_nYMin));
		int nSrcX_c = coords.first;
		int nSrcY_c = coords.second;

		ext_blit(
			screen,
			pTmpBmp,
			nSrcX_c,
			nSrcY_c,
			nRectWidth,
			nRectHeight,
			0,
			0,
			nRectWidth,
			nRectHeight
		);

		int nY_c = nSrcY_c, nX_c;
		while (nY_c < nSrcY_c + (v_nYMax - v_nYMin) * nGridElemSizeY)
		{
			nX_c = nSrcX_c;
			while (nX_c < nSrcX_c + (v_nXMax - v_nXMin) * nGridElemSizeX)
			{
				v_texturePattern.m_masked_blit(screen, nX_c, nY_c);
				nX_c += v_texturePattern.m_getPatternWidth();
			}

			nY_c += v_texturePattern.m_getPatternHeight();
		}

		for (int nX = v_nXMin; nX < v_nXMax; ++nX)
		{
			for (int nY = v_nYMin; nY < v_nYMax; ++nY)
			{
				if (v_vCoveredElems.end() ==
					find(v_vCoveredElems.begin(), v_vCoveredElems.end(), make_pair(nX, nY)))
				{
					INT_COORDS coords = pGrid->m_mapGridElementToCoords(make_pair(nX, nY));
					int nDestX_c = coords.first;
					int nDestY_c = coords.second;
					ext_blit(
						pTmpBmp,
						screen,
						0,
						0,
						nGridElemSizeX,
						nGridElemSizeY,
						nDestX_c,
						nDestY_c,
						nGridElemSizeX,
						nGridElemSizeY
					);		
				}
			}
		} /** end for */

	}
}

void BasicObject::m_drawObjectSolid(int a_nUserId, BITMAP* a_pDestBmp)
{
    int screenMaskColor = bitmap_mask_color(screen);
	if (screenMaskColor == v_nColor)
	{
		return;
	}

	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();
	int nGridElemSizeX = pGrid->m_getGridElementSizeX();
	int nGridElemSizeY = pGrid->m_getGridElementSizeY();

	BITMAP* pTmpBmp = pConf->m_getTmpBitmap(nGridElemSizeX, nGridElemSizeY);
	clear_to_color(pTmpBmp, v_nColor);

    vector<INT_COORDS>::const_iterator it;
	for (it = v_vCoveredElems.begin(); it != v_vCoveredElems.end(); ++it)
	{
		INT_COORDS coords = pGrid->m_mapGridElementToCoords(*it);
		int destX = coords.first;
		int destY = coords.second;

        if (0 != pTmpBmp)
		{
			ext_masked_blit(
				pTmpBmp, 
				screen, 
				0,
				0,
				nGridElemSizeX,
				nGridElemSizeY,
				destX,
				destY,
				nGridElemSizeX,
				nGridElemSizeY
			);	
		}
		else
		{
			rectfill(
				screen,					
				destX,							          
				destY,						
				destX + nGridElemSizeX,	
				destY + nGridElemSizeY,	
				v_nColor
			);							
		}
	}
}

bool BasicObject::m_requiresCoveredElemsRecalculation()
{
	return v_vCoveredElems.empty();
}

void BasicObject::m_recalculateCoveredElems()
{
	m_calculateBoundingRectangle();

	// find proper grid elements within this rectangular area
	for (int nX = v_nXMin; nX <= v_nXMax; ++nX)
	{
		for (int nY = v_nYMin; nY <= v_nYMax; ++nY)
		{
			INT_COORDS gridElem = make_pair(nX, nY);
			if (m_checkElementWithinObject(v_vVertices, gridElem))
			{
				v_vCoveredElems.push_back(gridElem);
			}
		}
	}
}

void BasicObject::m_applyToGrid(MapGrid* a_pGrid, int a_nLayerLevel)
{
	if (m_requiresCoveredElemsRecalculation())
	{
		m_recalculateCoveredElems();
	}

	vector<INT_COORDS>::const_iterator it;
	for (it = v_vCoveredElems.begin(); it != v_vCoveredElems.end(); ++it)
	{
		INT_COORDS gridElem = *it;
		GridElement* pElem = const_cast<GridElement*>(a_pGrid->m_getGridElement(gridElem));
		pElem->m_addLayerObject(this, a_nLayerLevel);
	}
}

void BasicObject::m_calculateBoundingRectangle()
{
	// get rectangular that will be potentially covered
	// by this basic object
	v_nXMin = INT_MAX,
	v_nXMax = INT_MIN,
	v_nYMin = INT_MAX,
	v_nYMax = INT_MIN;

	vector<INT_COORDS>::const_iterator it;
	for (it = v_vVertices.begin(); it != v_vVertices.end(); ++it)
	{
		INT_COORDS vertex = *it;
		int nX = vertex.first,
			nY = vertex.second;
		v_nXMin = min(nX, v_nXMin);
		v_nXMax = max(nX, v_nXMax);
		v_nYMin = min(nY, v_nYMin);
		v_nYMax = max(nY, v_nYMax);
	}
}

// assumption: a_rvObjVertices will be orderer properly
// 			   to form convex polygon 
int BasicObject::m_checkElementWithinObject(vector<INT_COORDS>& a_rvObjVertices,
											 INT_COORDS a_gridElem)
{
	// inclusion in a polygon algorithm
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();
    INT_COORDS centerCoords = pGrid->m_mapGridElementToCoords(a_gridElem);
	int	xt = centerCoords.first;			/*   x (horizontal) of target point     */
	int yt = centerCoords.second;			/*   y (vertical) of target point       */
	int npoints = a_rvObjVertices.size();	/*   number of points in polygon        */
	int* poly = new int[npoints << 1];		/*   polygon points 					*/

	for (int i = 0; i < npoints; ++i)
	{
		centerCoords = pGrid->m_mapGridElementToCoords(a_rvObjVertices[i]);
		poly[(i << 1)]		= centerCoords.first;
		poly[(i << 1) + 1]	= centerCoords.second;
	}

	int xnew,ynew;
    int xold,yold;
    int x1,y1;
    int x2,y2;
    int i;
    int inside=0;

    if (npoints < 3)
	{
		return 0;
    }
    xold=poly[(npoints - 1) << 1];
    yold=poly[((npoints - 1) << 1) + 1];
    for (i = 0; i < npoints; ++i)
	{
		xnew=poly[i << 1];
        ynew=poly[(i << 1) + 1];
        if (xnew > xold)
		{
			x1=xold;
            x2=xnew;
            y1=yold;
            y2=ynew;
        }
        else
		{
			x1=xnew;
            x2=xold;
            y1=ynew;
            y2=yold;
		}
        if ((xnew < xt) == (xt <= xold)          /* edge "open" at one end */
			&& ((long)yt-(long)y1)*(long)(x2-x1)
			< ((long)y2-(long)y1)*(long)(xt-x1))
		{
			inside=!inside;
        }
        xold=xnew;
		yold=ynew;
    }

	delete [] poly;

    return (inside);
}

INT_COORDS BasicObject::m_getPosition()
{
    return m_getPosition(0);
}

// taking now only into consideration
// dynamic objects that consist of 'single' grid element
// taking into consideration objects 'multiple' grid elements
// not provided by now
INT_COORDS BasicObject::m_getPosition(int a_nStep)
{
	if (!v_bDynamic ||
		v_vVertices.size() != 1)
	{
		return INVALID_COORDS;
	}

	if (0 == a_nStep)
	{
		if (v_vActualPath.size() == 0)
		{
			return v_vVertices[0];
		}
		return v_vActualPath[0].gridElem;
	}

	if (a_nStep < 0)
	{
		// do not provide position
		// backwards
		return INVALID_COORDS;
	}

	int nFrameGap = 0;//MovementManager::m_getFrameGap(v_nVelocity);
	int nPathStep = (a_nStep / nFrameGap);

	if (nPathStep > v_vActualPath.size())
	{
		return INVALID_COORDS;
	}

	INT_COORDS pathElem = v_vActualPath[nPathStep].gridElem; 
    return pathElem;
}

void BasicObject::m_replacePath(int a_nBeginPos,
								int a_nEndPos,
								vector<PATHELEM>& a_rvPath)
{
	m_replacePath(a_nBeginPos,
				  a_nEndPos,
				  a_rvPath,
				  v_vActualPath);
}

/**
 * Replaces actual path from position a_nBeginPos to position
 * a_nEndPos with path a_rvPath.
 * a_rvPath includes begin and end of replaced path.
 * If a_nBeginPos == -1 then actual path is replaced from the
 * beginning.
 * If a_nEndPos == -1 then actual path is replaced up to the the
 * end.
 * 
 * @param a_nBeginPos
 * @param a_nEndPos
 * @param a_rvPath
 * @param a_rvActualPath
 */
void BasicObject::m_replacePath(int a_nBeginPos,
								int a_nEndPos,
								vector<PATHELEM>& a_rvPath,
								vector<PATHELEM>& a_rvActualPath)
{
	int nSize = a_rvActualPath.size();

	if (nSize <= a_nBeginPos)
	{
		return;
	}

	if (a_nBeginPos == UNDEFINED)
	{
		a_nBeginPos = 0;
	}

	if (a_nEndPos == UNDEFINED ||
		nSize <= a_nEndPos)
	{
		a_nEndPos = nSize; 
	}
	else
	{
		a_nEndPos++;
	}

	vector<PATHELEM>::iterator itBegin	= a_rvActualPath.begin() + a_nBeginPos;
	vector<PATHELEM>::iterator itEnd  	= a_rvActualPath.begin() + a_nEndPos;

	vector<PATHELEM>::iterator itAfterEnd = a_rvActualPath.erase(itBegin, itEnd);

	a_rvActualPath.insert(itAfterEnd,
						 a_rvPath.begin(), a_rvPath.end());

}

/**
 * Computes at which element object will be in next step
 * 
 * @return INT_COORDS
 */
INT_COORDS BasicObject::m_nextStep()
{
    BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();
	int nSize = v_vActualPath.size(); 

    if (0 == nSize)
	{
		INT_COORDS actualPos = m_getPosition();
        return actualPos;
	}

	// v_vActualPath is always filled with:
	// 1) actual position
	// 2) destination
	// (even if it contains mipmap elements)

	if (1 == nSize)
	{
		return v_vActualPath[0].gridElem;
	}

	PATHELEM elem0 = v_vActualPath[0];
	if (1 != elem0.nResolution)
	{
		return INVALID_COORDS;
	}

    v_vActualPath.erase(v_vActualPath.begin());

	PATHELEM elem1 = v_vActualPath[0];
	if (1 < elem1.nResolution ||
		MovementManager::m_getDistance(elem0.gridElem, elem1.gridElem, 1) > 1)
	{
		// we have mipmaps or last element in mipmap chain
		vector<PATHELEM> vPath;
		MovementManager::m_recomputePath(this, elem0, elem1, vPath);
			
		vPath.erase(vPath.begin()); // BAD!!!

		m_replacePath(UNDEFINED, 0, vPath);

		elem1 = vPath[0];			
	}
    
	// move object
	if (v_bDynamic)
	{
		v_nFrame++;
		m_move(elem1.gridElem.first - elem0.gridElem.first,
			   elem1.gridElem.second - elem0.gridElem.second);
		v_nDir = m_handleChangeDir(elem1.gridElem, elem0.gridElem);
	}

	return elem0.gridElem;
}

void BasicObject::m_setVertices(vector<INT_COORDS>& a_vVertices)
{
	v_vVertices = a_vVertices;

	if (is_poly_rectangle(v_vVertices))
	{
		transform_to_uniform_rect(v_vVertices);
	}
}

int BasicObject::m_handleChangeDir(INT_COORDS a_dest, INT_COORDS a_src)
{
	int nDx 	= a_dest.first - a_src.first;
	int nDy		= a_dest.second - a_src.second;
	int nDir 	= UNDEFINED;

	if (abs(nDx) > 1 || abs(nDy) > 1)
	{
		return UNDEFINED;
	}

	nDir = (nDx == 0 && nDy == 1)	? DIR_SE	: nDir;
    nDir = (nDx == -1 && nDy == 1)	? DIR_E		: nDir; 
	nDir = (nDx == -1 && nDy == 0)	? DIR_NE 	: nDir;
	nDir = (nDx == 0 && nDy == -1)	? DIR_NW	: nDir;
	nDir = (nDx == 1 && nDy == -1)	? DIR_W		: nDir;
	nDir = (nDx == 1 && nDy == 0)	? DIR_SW	: nDir;

	return nDir;
}

void BasicObject::m_setActualTarget(INT_COORDS a_coords, bool a_bRecomputePath)
{
	if (a_bRecomputePath)
	{
		set<INT_COORDS>	setCovered;
		vector<PATHELEM> vPathToDest; 
		MovementManager::m_computePath(this,
									   a_coords,
									   setCovered,
									   vPathToDest);
		if (vPathToDest.empty())
		{
			return;
		}
		m_setActualPath(vPathToDest);
	}
	v_actualTarget = a_coords;
}

INT_COORDS BasicObject::m_getActualTarget()
{
    return v_actualTarget;
}

void BasicObject::m_setNextStep(INT_COORDS a_newObjPos, bool a_bCheckValid)
{
	INT_COORDS objPos			= m_getPosition();
	BasicConfiguration* pConf	= BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid 				= pConf->m_getGridMap();

	if (a_bCheckValid)
	{
		// check if a_coords is on adjacent position
		// and is not covered by any other object
		if (1 != MovementManager::m_getDistance(objPos, a_newObjPos, 1))
		{
			cerr << " Next step not adjacent "
				 << endl;
			return;
		}
		GridElement* pElem = const_cast<GridElement*>(pGrid->m_getGridElement(a_newObjPos));
		if (!(pElem && pElem->m_isCoveredByObject()))
		{
			cerr << " Not empty cell for next step "
				 << endl;
			return;
		}
	}
	
	int nActualPathSize = v_vActualPath.size();
	if (0 == nActualPathSize)
	{
		v_vActualPath.push_back(PATHELEM(objPos, 1));
		v_vActualPath.push_back(PATHELEM(a_newObjPos, 1));
	}
	else if (1 == nActualPathSize)
	{
		// 0 EL => ACTUAL POSITION
		v_vActualPath.push_back(PATHELEM(a_newObjPos, 1));
	}
	else
	{
		// 1 < nActualPathSize
		PATHELEM elem1 = v_vActualPath[1];
		if (a_newObjPos == elem1.gridElem &&
			1 == elem1.nResolution)
		{
			; // do nothing
		}
		else
		{
			v_vActualPath.erase(v_vActualPath.begin() + 1, v_vActualPath.end());
			v_vActualPath.push_back(PATHELEM(a_newObjPos, 1));
		}
	}
}

bool BasicObject::m_isStandingStill()
{
	return v_vActualPath.empty();
}

int BasicObject::m_handleChangeDir(double a_fAngle)
{
	int nDir = UNDEFINED;

	const double M_PI_DIV8 = (M_PI / 8.0);

	nDir = (a_fAngle >= M_PI_DIV8 && a_fAngle < 3 * M_PI_DIV8) ? DIR_NE : nDir; 
	nDir = (a_fAngle >= 3 * M_PI_DIV8 && a_fAngle < 5 * M_PI_DIV8) ? DIR_N : nDir; 
	nDir = (a_fAngle >= 5 * M_PI_DIV8 && a_fAngle < 7 * M_PI_DIV8) ? DIR_NW : nDir;
	nDir = (a_fAngle >= 7 * M_PI_DIV8 && a_fAngle < 9 * M_PI_DIV8) ? DIR_W : nDir;
	nDir = (a_fAngle >= 9 * M_PI_DIV8 && a_fAngle < 11 * M_PI_DIV8) ? DIR_SW : nDir;
	nDir = (a_fAngle >= 11 * M_PI_DIV8 && a_fAngle < 13 * M_PI_DIV8) ? DIR_S : nDir;
	nDir = (a_fAngle >= 13 * M_PI_DIV8 && a_fAngle < 15 * M_PI_DIV8) ? DIR_SE : nDir;
	nDir = ((a_fAngle >= 15 * M_PI_DIV8 && a_fAngle <= 2 * M_PI) ||
			(a_fAngle >= 0 && a_fAngle < M_PI_DIV8)) ? DIR_E : nDir;

	return nDir;
}

void BasicObject::m_update()
{
	assert(INVALID_POS != v_pos);
	cpt3f oldPos = v_pos;
	
    // managers state update:
	if (v_pParamsManager->m_tryToScheduledObjAtTimeFrame())
		v_pParamsManager->m_update();
	if (v_pParamsManager->m_tryToScheduledObjAtTimeFrame())
		//v_pWorldInfoManager->m_do();
	if (v_pParamsManager->m_tryToScheduledObjAtTimeFrame())
		v_pCommunicationManager->m_do();
	if (v_pParamsManager->m_tryToScheduledObjAtTimeFrame())
		v_pDecisionManager->m_do();
	if (v_pParamsManager->m_tryToScheduledObjAtTimeFrame())
		v_pBehaviorManager->m_do();

	if (!isStatic)
	{
        Behavior* pBehavior = v_pBehaviorManager->m_getActualBehavior();
		if ((Behavior*)0 != pBehavior)
		{
			cpt3f newPos = pBehavior->m_getNewPos();
			v_pos = newPos;
		}
		else
		{
			assert((Flock*)0 != v_pFl);
			cpt3f newPos = v_pFl->m_getNewPosForObj();
			v_pos = newPos;
		}


		placeObj(this, oldPos, false);
		placeObj(this, v_pos, true);
	}

    v_vel = v_pos - oldPos;

	if (v_vel != ZERO_VEC)
	{
		dir = v_vel;
	}

    return;
}

ostream& operator<<(ostream& os, const BasicObject& obj)
{
	os << typeid(obj).name() << " , id: " << obj.getIdx();
}

void BasicObject::touch(Bullet* a_pBullet)
{
#ifdef __TRACE_ME
	cout << *this << " " << "TOUCHED! " << endl;
#endif 

	BulletClass* pClass = a_pBullet->m_getClass();
	assert((BulletClass*)0 != pClass);

	float& force = pClass->force;
	assert(force >= MIN_FORCE && force <= MAX_FORCE);

	float& health = v_pParamsManager->health.val;
	cfloat& minHealth = v_pParamsManager->health.minVal;
	cfloat& maxHealth = v_pParamsManager->health.maxVal;

	// TODO: enhance
	health -= force / (MAX_FORCE - MIN_FORCE);

#ifdef __TRACE_ME
	cout << health << endl << endl;
#endif

	touched = true;

	return;	
}

bool BasicObject::m_exists()
{
	return (v_pParamsManager->health.val > v_pParamsManager->health.minVal);
}

void BasicObject::delObjRef()
{
	TGManager* pTGM = TGManager::m_getTGManager();
	assert((TGManager*)0 != pTGM);
	pTGM->delObjRef(this);

	INT_COORDS cell = getCellForPos(v_pos);
	GridElement* pGE = getGridElement(cell);
	assert((GridElement*)0 != pGE);

	pGE->m_setCovered(this, false);
}

bool BasicObject::m_isControlledByTG(TG** a_tg)
{
	UnitBehaviorMAN* pMAN = v_pBehaviorManager;

	Behavior* pB = pMAN->m_getActualBehavior();
	if ((Behavior*)0 != pB)
	{
		CtrlObject* pCtrlObj = pB->m_getCtrlObj();
		if ((CtrlObject*)0 != pCtrlObj)
		{
			TG* pTG = dynamic_cast<TG*>(pCtrlObj);
			if ((TG*)0 != pTG)
			{
				*a_tg = pTG;
                return true;
			}
		}
	}
    
	return false;
}

void BasicObject::m_getFOV(float& a_fovRAD, float& a_fovANG)
{
	UnitParamsMAN* pMAN = v_pParamsManager;

    a_fovRAD = (pMAN->fovRAD).val;
	a_fovANG = (pMAN->fovANG).val;

    return;
}

