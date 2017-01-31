#include "stdafx.h"
#include "BasicTypes.h"

DEFINE_META(Rect)
{
	ADD_RE_MEMBER(size);
	ADD_RE_MEMBER(pos);
}

DEFINE_META(Colour)
{
	ADD_RE_MEMBER(r);
	ADD_RE_MEMBER(g);
	ADD_RE_MEMBER(b);
}

DEFINE_META(Point2D)
{
	ADD_RE_MEMBER(x);
	ADD_RE_MEMBER(y);
}

DEFINE_META(Size2D)
{
	ADD_RE_MEMBER(w);
	ADD_RE_MEMBER(h);
}
