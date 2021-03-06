#pragma once
#include <cmath>
#include "../../../MetaSystem/MetaSystem.h"

//Define some basic types needed for 2D graphics
class Point2D 
{
public:
	META_DATA(Point2D);
	float x, y;
	Point2D(float _x = 0, float _y = 0) :x(_x), y(_y) { };
	float length() { return (float)std::sqrt(x*x + y*y); };

	Point2D& operator*(float scale)
	{
		return Point2D(this->x * scale, this->y * scale);
	}

	Point2D& operator/(float scale)
	{
		return Point2D(this->x / scale, this->y / scale);
	}

	inline bool operator==(const Point2D& p2)
	{
		return (this->x == p2.x && this->y == p2.y);
	}
};

class Size2D {
public:
	META_DATA(Size2D);
	float w, h;
	Size2D(float _w = 0, float  _h = 0) :w(_w), h(_h) {};
};

class Colour {
public:
	META_DATA(Colour);
	int r, g, b, a;
	Colour(int _r = 255, int  _g = 255, int _b = 255, int  _a = 255) :r(_r), g(_g), b(_b), a(_a) {};
};

class Rect {
public:
	META_DATA(Rect);
	Point2D pos;
	Size2D size;
	Rect(Point2D p, Size2D s) :pos(p), size(s) {};
	Rect(float x = 0, float y = 0, float w = 1, float h = 1) :pos(x, y), size(w, h) {};
	Rect& operator*(float scale)
	{
		return Rect(this->pos.x * scale, this->pos.y * scale, this->size.w * scale, this->size.h * scale);
	}

	Rect& operator/(float scale)
	{
		return Rect(this->pos.x / scale, this->pos.y / scale, this->size.w / scale, this->size.h / scale);
	}

	Point2D getCentreCopy()
	{
		return  Point2D(this->pos.x + this->size.w / 2, this->pos.y + this->size.h / 2);
	}
};