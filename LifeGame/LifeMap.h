#pragma once
#include<Siv3D.hpp>

enum class LifeState
{
	DEAD,
	ALIVE,
};

class LifeCell
{

	LifeState m_state;
	LifeState m_nextState;


public:

	LifeCell() = default;

	LifeCell(const LifeState& _state) :m_state(_state),m_nextState(_state) {}

	void setState(const LifeState& _state)
	{
		m_state = _state;
	}

	LifeState getState()const
	{
		return m_state;
	}

	void setNextState(const LifeState& _state)
	{
		m_nextState = _state;
	}

	LifeState getNextState()const
	{
		return m_nextState;
	}

	bool isAlive()const
	{
		return m_state == LifeState::ALIVE;
	}
};


class LifeMap
{
	Grid<LifeCell> m_map;

	int m_gridLength;

	bool checkRange(int _x, int _y)const
	{
		return InRange(_x, 0, m_map.size().x - 1) && InRange(_y, 0, m_map.size().y - 1);
	}

	int countAroundAliveCell(int _x, int _y)const
	{
		if (!checkRange(_x, _y)) { return 0; }

		int count = 0;

		for (int moveY = -1; moveY <= 1; moveY++)
		{
			for (int moveX = -1; moveX <= 1; moveX++)
			{
				if (!Point(moveX,moveY).isZero() &&checkRange(_x + moveX, _y + moveY) && m_map[_y+moveY][_x + moveX].isAlive())
				{
					count++;
				}
			}
		}

		return count;
	}


	void applyRule(int _x, int _y)
	{
		if (!checkRange(_x, _y)) { return; }


	}


public:
	LifeMap(const Size& _size,int _gridLength):m_map(_size),m_gridLength(_gridLength)
	{
		for (int y = 0; y < m_map.size().y; y++)
		{
			for (int x = 0; x < m_map.size().x; x++)
			{
				m_map[y][x].setState(RandomSelect<LifeState>({ LifeState::ALIVE,LifeState::DEAD }));
			}
		}
	}

	void update()
	{
		ClearPrint();

		for (int y = 0; y < m_map.size().y; y++)
		{
			for (int x = 0; x < m_map.size().x; x++)
			{
				Println(Point(x, y), L":", countAroundAliveCell(x, y));
			}
		}
	}

	void draw(const Point& _pos)const
	{
		for (int y = 0; y < m_map.size().y; y++)
		{
			for (int x = 0; x < m_map.size().x; x++)
			{
				Rect(Point(x, y)*m_gridLength + _pos, Size(m_gridLength, m_gridLength)).draw(m_map[y][x].isAlive() ? Palette::Yellowgreen:Palette::Gray);
			}
		}
	}

};