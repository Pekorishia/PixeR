#ifndef _BG_H_
#define _BG_H_

#include "../utility/vec3.h"

class Background
{
public:
	rgb lower_left;
	rgb lower_right;
	rgb top_left;
	rgb top_right;

	Background( rgb ll_, rgb lr_, rgb tl_, rgb tr_)
	{
		lower_left = ll_;
		lower_right = lr_;
		top_left = tl_;
		top_right = tr_;
	}
	
};

#endif
