
# include <Siv3D.hpp>
# include "LifeMap.h"

void Main()
{
	const Size mapSize(100,100);
	const int cellLength = 4;

	//const Size mapSize(4, 4);
	//const int cellLength = 20;


	LifeMap lifeMap(mapSize,cellLength);

	while (System::Update())
	{
	
		lifeMap.update();

		ClearPrint();
		Println(Profiler::FPS(), L"fps");
		lifeMap.draw(Point(0,0));
	}
}
