#pragma once
#include<Siv3D.hpp>

class BitLifeMap
{
	Array<uint8> m_buffer;

	Size m_size;

	int m_gridLength;

	int getOffset(int _x, int _y)const
	{
		return (_x + 1) / 8 + (_y + 1)*(m_size.x + 1);
	}

	int getMask(int _x)const
	{
		return 0x80 >> (_x & 7);
	}

	template<typename T>
	T nextGeneration(uint8 UL, T U, uint8 UR, uint8 L, T bd, uint8 R, uint8 DL, T D, uint8 DR)
	{
		const int MSB = 0x80 << (sizeof(T) * 8 - 1);
		T a = (U >> 1) | (!(UL & 0x01) ? 0 : MSB);
		T b = U;
		T c = (U << 1) | (!(UL & 0x80) ? 0 : 1);
		T d = (bd >> 1) | (!(L & 0x01) ? 0 : MSB);
		T e = (bd << 1) | (!(L & 0x80) ? 0 : 1);
		T f = (D >> 1) | (!(DL & 0x01) ? 0 : MSB);
		T g = D;
		T h = (D << 1) | (!(DL & 0x80) ? 0 : 1);

		T s0, s1, s2, s3;
		//a`h ‚©‚ç s2, s3 ‚ðŒvŽZ;
		s2 = a & b;
		s1 = a ^ b;
		s0 = ~(a | b);  //  a ‚Æ b ‚Ì‰ÁŽZA˜_—‰‰ŽZ‰ñ”F‚S
		s3 = s2 & c;
		s2 = (s2 & ~c) | (s1 & c);
		s1 = (s1 & ~c) | (s0 & c);
		s0 &= ~c;       //  c ‚ð‰ÁŽZA˜_—‰‰ŽZ‰ñ”F‚P‚Pi‚Xj
		s3 = (s3 & ~d) | (s2 & d);
		s2 = (s2 & ~d) | (s1 & d);
		s1 = (s1 & ~d) | (s0 & d);
		s0 &= ~d;       //  d ‚ð‰ÁŽZA˜_—‰‰ŽZ‰ñ”F‚P‚Si‚P‚Pj
		s3 = (s3 & ~e) | (s2 & e);
		s2 = (s2 & ~e) | (s1 & e);
		s1 = (s1 & ~e) | (s0 & e);
		s0 &= ~e;       //  e ‚ð‰ÁŽZA˜_—‰‰ŽZ‰ñ”F‚P‚Si‚P‚Pj
		s3 = (s3 & ~f) | (s2 & f);
		s2 = (s2 & ~f) | (s1 & f);
		s1 = (s1 & ~f) | (s0 & f);
		s0 &= ~f;       //  f ‚ð‰ÁŽZA˜_—‰‰ŽZ‰ñ”F‚P‚Si‚P‚Pj
		s3 = (s3 & ~g) | (s2 & g);
		s2 = (s2 & ~g) | (s1 & g);
		s1 = (s1 & ~g) | (s0 & g);
		//s0 &= ~g;     //  g ‚ð‰ÁŽZA˜_—‰‰ŽZ‰ñ”F‚P‚Qi‚P‚Oj
		s3 = (s3 & ~h) | (s2 & h);
		s2 = (s2 & ~h) | (s1 & h);


		return (bd & s2) | s3;
	}

public:

	BitLifeMap() {}

	BitLifeMap(const Size& _size,int _gridLength) :m_size(_size),m_gridLength(_gridLength)
	{
		m_buffer.resize(bufferSize());

		for (int y = 0; y < m_size.y; y++)
		{
			for (int x = 0; x < m_size.x; x++)
			{
				setPixel(m_buffer, x, y, RandomBool());
			}
		}
	}
	
	bool pixel(int _x, int _y)const
	{
		return (m_buffer[getOffset(_x, _y)] & getMask(_x)) != 0;
	}

	int bufferSize()const
	{
		const int ySize = m_size.y + 2;
		const int xSize = static_cast<int>(ceil(m_size.x + 2/8.0));

		return xSize*ySize;
	}

	void setPixel(Array<uint8>& _buffer, int _x, int _y, bool _b)
	{
		if (_b)
		{
			_buffer[getOffset(_x,_y)] |= getMask(_x);
		}
		else
		{
			_buffer[getOffset(_x, _y)] &= ~getMask(_x);
		}
	}


	void update()
	{
		Array<uint8> buffer2;
		buffer2.resize(bufferSize());

		for (int y = 0; y < m_size.y; y++)
		{
			for (int x = 0; x < m_size.x; x++)
			{
				const int count =
					(pixel(x - 1, y - 1) ? 1 : 0) +
					(pixel(x, y - 1) ? 1 : 0) +
					(pixel(x + 1, y - 1) ? 1 : 0) +
					(pixel(x - 1, y) ? 1 : 0) +
					(pixel(x + 1, y) ? 1 : 0) +
					(pixel(x - 1, y + 1) ? 1 : 0) +
					(pixel(x, y + 1) ? 1 : 0) +
					(pixel(x + 1, y + 1) ? 1 : 0);

				const bool b = count == 3 || count == 2 && pixel(x, y);
				setPixel(buffer2, x, y, b);
			}
		}

		m_buffer.swap(buffer2);
	}

	void update2()
	{
		Array<uint8> buffer2;
		buffer2.resize(bufferSize());

		for (int y = 0; y < m_size.y; y++)
		{
			for (int x = 0; x < m_size.x; x+=8)
			{
				const bool isLeft = x == 0;
				const bool isRight = x / 8 == static_cast<int>(ceil(m_size.x + 2 / 8.0)) - 1;
				const bool isTop = y == 0;
				const bool isBottom = y == m_size.y - 1;

				const uint8 UL = (isLeft || isTop) ? 0 : m_buffer[getOffset(x - 1, y - 1)];
				const uint8 U = (isTop) ? 0 : m_buffer[getOffset(x, y - 1)];
				const uint8 UR = (isRight || isTop) ? 0 : m_buffer[getOffset(x + 1, y - 1)];
				
				const uint8 L = (isLeft) ? 0 : m_buffer[getOffset(x - 1, y)];
				const uint8 bd = m_buffer[getOffset(x, y)];
				const uint8 R = (isRight) ? 0 : m_buffer[getOffset(x + 1, y)];
				
				const uint8 DL = (isLeft || isBottom) ? 0 : m_buffer[getOffset(x - 1, y + 1)];
				const uint8 D = (isBottom) ? 0 : m_buffer[getOffset(x, y + 1)];
				const uint8 DR = (isRight || isBottom) ? 0 : m_buffer[getOffset(x + 1, y + 1)];

				buffer2[getOffset(x, y)] = nextGeneration(UL, U, UR, L, bd, R, DL, D, DR);
			}
		}

		m_buffer.swap(buffer2);
	}

	void draw(const Point& _pos)const
	{
		for (int y = 0; y < m_size.y; y++)
		{
			for (int x = 0; x < m_size.x; x++)
			{
				const Color color = pixel(x,y) ? Palette::Yellowgreen : Palette::Gray;
				Rect(Point(x, y)*m_gridLength + _pos, Size(m_gridLength, m_gridLength)).draw(color);
			}
		}
	}
};