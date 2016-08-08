#include "common.h"


ConfigurationReader::ConfigurationReader(const char* a_strFileName)
{
	assert((const char*)0 != a_strFileName);
	v_strFileName = a_strFileName;
}

ConfigurationReader::~ConfigurationReader()
{
}

void ConfigurationReader::m_readConfiguration(vector<BasicObject*>& a_rvObjs)
{
	ifstream is(v_strFileName.c_str(), ios_base::in);
	if (!is)
	{
#ifdef __TRACE_ME
		cerr << "Unable to open  file: " << v_strFileName << endl;
#endif
		return;
	}

	const uint maxBuff = 256;
	char buffer[maxBuff];

	// nCellsX nCellsY
	is.getline(buffer, maxBuff);
	if (is.fail()) return;

	// cellSizeX cellSizeY
	is.getline(buffer, maxBuff);
	if (is.fail()) return;

	is.getline(buffer, maxBuff);

	while (!is.eof())
	{
		is.getline(buffer, maxBuff);
		if (is.fail()) { is.close(); return;}

		string line = buffer;

		pt3f pos = INVALID_POS;

		size_t startPos, endPos;

        if (string::npos == (startPos = line.find_first_not_of("( \r\t", 0))) return;
        if (string::npos == (endPos	= line.find_first_of(",", startPos))) return;
		pos.x = (float)(atoi(line.substr(startPos, endPos - startPos).c_str()));	

		if (string::npos == (startPos = line.find_first_not_of(" \r\t", endPos + 1))) return;
        if (string::npos == (endPos	= line.find_first_of(",", startPos))) return;
		pos.y = (float)(atoi(line.substr(startPos, endPos - startPos).c_str()));

		if (string::npos == (startPos = line.find_first_not_of(" \r\t", endPos + 1))) return;
        if (string::npos == (endPos	= line.find_first_of(",", startPos))) return;
		pos.z = (float)(atoi(line.substr(startPos, endPos - startPos).c_str()));

		float width, height;

		if (string::npos == (startPos = line.find_first_not_of(" \r\t", endPos + 1))) return;
        if (string::npos == (endPos	= line.find_first_of(",", startPos))) return;
		width = (float)(atoi(line.substr(startPos, endPos - startPos).c_str()));

		if (string::npos == (startPos = line.find_first_not_of(" \r\t", endPos + 1))) return;
        if (string::npos == (endPos	= line.find_first_of(",", startPos))) return;
		height = (float)(atoi(line.substr(startPos, endPos - startPos).c_str()));

		uint assign;

		if (string::npos == (startPos = line.find_first_not_of(" \r\t", endPos + 1))) return;
        if (string::npos == (endPos	= line.find_first_of(",", startPos))) return;
		assign = atoi(line.substr(startPos, endPos - startPos).c_str());

		uint txtID;

		if (string::npos == (startPos = line.find_first_not_of(" \r\t", endPos + 1))) return;
        if (string::npos == (endPos	= line.find_first_of(")", startPos))) return;
		txtID = atoi(line.substr(startPos, endPos - startPos).c_str());
		
#ifdef __TRACE_ME
		cout << "pos: " 	<< pos 		<< endl;
		cout << "width: "	<< width	<< endl;
		cout << "height: "	<< height	<< endl;
		cout << "assign: "	<< assign	<< endl;
		cout << "txtID: "	<< txtID	<< endl;
#endif

		BasicObject* pObj = new BasicObject(pos, txtID, width, height, assign);
		if (txtID == G_HOUSE_TXT_ID)
			pObj->setStatic();

		a_rvObjs.push_back(pObj);
	}


	is.close();
}

