#ifndef __MAPTEST_H__
#define __MAPTEST_H__

class MapTest
{
public:
	MapTest();
	virtual ~MapTest()  {}

	void testGridGeneration();
	void testGridUnloading();
	void testGridGenerationWithLimitedLayers();
	void testNonStaticElementsMovement();


};

#endif // __MAPTEST_H__
