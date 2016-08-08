#include "common.h"


ConfigurationWriter::ConfigurationWriter(const char* a_strFileName)
{
	assert((const char*)0 != a_strFileName);
	v_strFileName = a_strFileName;
}

ConfigurationWriter::~ConfigurationWriter()
{	
}

void ConfigurationWriter::m_genConfiguration()
{
	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();
	GameView* pView = pConf->m_getGameViewByUserId(DEFAULT_USER_ID);

	const uint nCellsX = (uint)(pGrid->m_getNumCellX());
	const uint nCellsY = (uint)(pGrid->m_getNumCellY());
	assert(nCellsX > 0 && nCellsY > 0);
	const uint cellSizeX = (uint)(pGrid->m_getGridElementSizeX());
	const uint cellSizeY = (uint)(pGrid->m_getGridElementSizeY());
	assert(cellSizeX > 0 && cellSizeY > 0);

	ofstream os(v_strFileName.c_str(), ios_base::out);
	if (!os)
	{
#ifdef __TRACE_ME
		cerr << "Unable to open  file: " << v_strFileName << endl;
#endif
		return;
	}

	os << nCellsX << " " << nCellsY << endl;
	os << cellSizeX << " " << cellSizeY << endl << endl;

	uint numGenObjs;
	vector<BasicObject*> vGenObjs;
	m_genGameObjects(vGenObjs, numGenObjs);
	int i;
	for (i = 0; i < numGenObjs; ++i)
	{
		BasicObject*& pObj = vGenObjs[i];
		cpt3f objPos = pObj->m_getPos();
		os << "(";
		os << (uint)(objPos.x) << "," << (uint)(objPos.y) << "," << (uint)(objPos.z);
		os << "," << pObj->m_getWidth() << "," << pObj->m_getHeight(); 
		os << "," << pObj->m_getAssign();
		os << "," << pObj->m_getTxtID0();
		os << ")" << endl;
	}

	os.close();

	for (i = 0; i < numGenObjs; ++i)
	{
		BasicObject* pObj = vGenObjs[i];
		delete pObj;
	}
}

void ConfigurationWriter::m_genGameObjects(vector<BasicObject*>& a_rvObjs /* out */, uint& a_numObjs /* out */)
{
	srand(time(0));

	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	MapGrid* pGrid = pConf->m_getGridMap();
	GameView* pView = pConf->m_getGameViewByUserId(DEFAULT_USER_ID);

	const uint nCellsX = (uint)(pGrid->m_getNumCellX());
	const uint nCellsY = (uint)(pGrid->m_getNumCellY());
	assert(nCellsX > 0 && nCellsY > 0);
	const uint cellSizeX = (uint)(pGrid->m_getGridElementSizeX());
	const uint cellSizeY = (uint)(pGrid->m_getGridElementSizeY());
	assert(cellSizeX > 0 && cellSizeY > 0);
	RECTANGLE rect = pGrid->m_getGameRectangle();
	uint rngX = (uint)(rect.drVertex.first - rect.ulVertex.first) * 4;
	uint rngY = (uint)(rect.drVertex.second - rect.ulVertex.second) * 4;

    const uint numGenObjs = (nCellsX * nCellsY) / 10;
	if (0 == numGenObjs)
		return;
	const uint numGenUnits	 	= (uint)((2.f * numGenObjs) / 3.f);
	const uint numGenTanks 		= (uint)(numGenObjs / 6.f);
	const uint numGenBuildings	= numGenObjs - numGenUnits - numGenTanks;

	// UNITS : 
	uint numUnitsAccepted = 0;
	int i;
    for (i = 0; i < numGenUnits * 2; ++i)
	{
        pt3f genPos = pt3f(rngX * ranf(), rngY * ranf(), 0.f);
		if (posOutsideGrid(genPos)) continue;

		const uint assign = (numUnitsAccepted % 2 == 0) ? 0 : 1;
		BasicObject* pObj = new BasicObject(genPos, (assign == 0) ? G_UNIT_TXT_ID : G_ENEMY_UNIT_TXT_ID, 1.f, 1.f, assign);
		Obstacle obs = pObj->m_getObstacle();

		bool isColliding = false;
		int j;
		for (j  = 0; j < a_rvObjs.size(); ++j)
		{
			Obstacle aObs = a_rvObjs[j]->m_getObstacle();
			if (Obstacle::isColliding(obs, aObs))
			{
				isColliding = true;
				break;
			}
		}

		if (isColliding) continue;

        a_rvObjs.push_back(pObj);
		numUnitsAccepted++;

		if (numUnitsAccepted == numGenUnits)
			break;
	}

	// TANKS :
	uint numTanksAccepted = 0;
    for (i = 0; i < numGenTanks * 2; ++i)
	{
        pt3f genPos = pt3f(rngX * ranf(), rngY * ranf(), 0.f);
		if (posOutsideGrid(genPos)) continue;

		const uint assign = (numTanksAccepted % 2 == 0) ? 0 : 1;
		BasicObject* pObj = new BasicObject(genPos, (assign == 0) ? G_TANK_TXT_ID : G_ENEMY_TANK_TXT_ID, 2.f, 2.f, (numTanksAccepted % 2 == 0) ? 0 : 1);
		Obstacle obs = pObj->m_getObstacle();

		bool isColliding = false;
		int j;
		for (j  = 0; j < a_rvObjs.size(); ++j)
		{
			Obstacle aObs = a_rvObjs[j]->m_getObstacle();
			if (Obstacle::isColliding(obs, aObs))
			{
				isColliding = true;
				break;
			}
		}

		if (isColliding) continue;

        a_rvObjs.push_back(pObj);
		numTanksAccepted++;

		if (numTanksAccepted == numGenTanks)
			break;
	}

	// BUILDINGS (common) :
	uint numBuildingsAccepted = 0;
    for (i = 0; i < numGenBuildings * 2; ++i)
	{
        pt3f genPos = pt3f(rngX * ranf(), rngY * ranf(), 0.f);
		if (posOutsideGrid(genPos)) continue;
        
		BasicObject* pObj = new BasicObject(genPos, G_HOUSE_TXT_ID, 3.f, 2.f, (numBuildingsAccepted % 2 == 0) ? 0 : 1);
        Obstacle obs = pObj->m_getObstacle();

		bool isColliding = false;
		int j;
		for (j  = 0; j < a_rvObjs.size(); ++j)
		{
			Obstacle aObs = a_rvObjs[j]->m_getObstacle();
			if (Obstacle::isColliding(obs, aObs))
			{
				isColliding = true;
				break;
			}
		}

		if (isColliding) continue;

        a_rvObjs.push_back(pObj);
		numBuildingsAccepted++;

		if (numBuildingsAccepted == numGenBuildings)
			break;
	}

#ifdef __TRACE_ME
	cout << "numUnitsAccepted:: " 		<< numUnitsAccepted		<< endl;
	cout << "numTanksAccepted:: " 		<< numTanksAccepted 	<< endl;
	cout << "numBuildingsAccepted::" 	<< numBuildingsAccepted << endl << endl;
#endif

	a_numObjs = numUnitsAccepted + numTanksAccepted + numBuildingsAccepted;

	return;
}

