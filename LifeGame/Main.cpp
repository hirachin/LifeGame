
# include <Siv3D.hpp>
# include "LifeMap.h"
# include "BitLifeMap.h"


void Main()
{
	const int scale = 1;
	const Size mapSize(Window::Size()/scale);
	const int cellLength = scale;

	//const Size mapSize(10, 10);
	//const int cellLength = 40;


	BitLifeMap lifeMap(mapSize,cellLength);
	//LifeMap lifeMap2(mapSize, cellLength);

	int count = 0;

	
	while (System::Update())
	{
		ClearPrint();
		Println(Profiler::FPS(), L"fps");
		Println(L"count:", count);

		count++;
		lifeMap.update2();
		lifeMap.draw(Point(0, 0));
		
	}
}
