#pragma once

#include <string>

namespace NUM_TYPE
{
	const unsigned char NORMAL_NUMBER = 0;
	const unsigned char TILDE_NUMBER = 1;
	const unsigned char CARET_NUMBER = 2;
}

class MCPosition
{
public:
	
	struct PositionType
	{
		/*
			0 = Normal number "51.233333"
			1 = Tilde number "~13"
			2 = Caret number "^13"
		*/
		unsigned char Type = 0;
		float Value = 0;

		PositionType(unsigned char t, float v)
		{
			Type = t;
			Value = v;
		}

		PositionType()
		{
			Type = 0;
			Value = 0.0f;
		}
	};

	PositionType x;
	PositionType y;
	PositionType z;

	MCPosition(PositionType a, PositionType b, PositionType c)
	{
		x = a;
		y = b;
		z = c;
	}

	MCPosition()
	{
	}

	std::string toString()
	{
		std::string xVal;
		if (x.Type == 1)
			xVal += '~';
		else if (x.Type == 2)
			xVal += '^';
		xVal += std::to_string(x.Value);
		std::string yVal;
		if (y.Type == 1)
			yVal += '~';
		else if (x.Type == 2)
			yVal += '^';
		yVal += std::to_string(y.Value);
		std::string zVal;
		if (z.Type == 1)
			zVal += '~';
		else if (z.Type == 2)
			zVal += '^';
		zVal += std::to_string(z.Value);

		return xVal + ' ' + yVal + ' ' + zVal;
	}

};
