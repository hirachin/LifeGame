
# include <Siv3D.hpp>
# include "LifeMap.h"

void Main()
{
	//const Size mapSize(100, 100);
	//const int cellLength = 2;

	const Size mapSize(4, 4);
	const int cellLength = 20;


	LifeMap lifeMap(mapSize,cellLength);

	while (System::Update())
	{
		lifeMap.update();
		lifeMap.draw(Point(100, 100));
	}
}
