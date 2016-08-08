#include "MapTest.h"

MapTest::MapTest()
{
	// ...
}

void MapTest::testGridGeneration()
{
	/*SaveGameFile file("filegame.sav");
	const vector<Layer>& vLayers = file.getLayers();
	int nSizeX = file.nSizeX;
	int nSizeY = file.nSizeY;

	MapGrid grid(nSizeX, nSizeY);
	// timer starts
	grid.generateGrid(vLayers);
	// timer ends...*/
}

void MapTest::testGridUnloading()
{
	/*SaveGameFile file("filegame.sav");
	const vector<Layer>& vLayers = file.getLayers();
	int nSizeX = file.nSizeX;
	int nSizeY = file.nSizeY;

	MapGrid grid(nSizeX, nSizeY);
    grid.generateGrid(vLayers);
    //timer starts
	grid.unloadWholeGrid();
	// timer ends*/
}

void MapTest::testGridGenerationWithLimitedLayers()
{
	/*SaveGameFile file("filegame.sav");
	int nNumLayers = file.getNumLayers();
	for (int i = 0; i < nNumLayers; ++i)
	{
		const vector<Layer>& vLayers = file.getLayers(i);
		int nSizeX = file.nSizeX;
		int nSizeY = file.nSizeY;

		MapGrid grid(nSizeX, nSizeY);
		// timer starts
		grid.generateGrid(vLayers);
		// timer ends...
	}*/
}

void MapTest::testNonStaticElementsMovement()
{
	/*SaveGameFile file("filegame.sav");
	const vector<Layer>& vLayers = file.getLayers();
	int nSizeX = file.nSizeX;
	int nSizeY = file.nSizeY;

	MapGrid grid(nSizeX, nSizeY);
	// timer starts...
	grid.generateGrid(vLayers);
	// timer ends...*/
}
