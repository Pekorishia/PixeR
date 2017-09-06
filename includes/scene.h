#ifndef _SCENE_H_
#define _SCENE_H_

#include "object.h" 
#include "light.h" 
#include "background.h"

class Scene {

    public:

        Object **list; 
        int list_size;
        Light **lum;
        int lum_size;
        Background *bg;
        rgb ambientLight;
    	    	
        Scene( Object **l_, int n_, Light **lum_, int m_, Background *bg_, rgb al_)
        { 
        	list = l_;
        	list_size = n_;
        	lum = lum_;
            lum_size = m_;
            bg = bg_;
            ambientLight = al_;
        }
};

#endif
