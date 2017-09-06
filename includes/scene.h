#ifndef _SCENE_H_
#define _SCENE_H_

#include "object.h" 
#include "light.h" 

class Scene {

    public:

        Object **list; 
        int list_size;
        Light *luz;
    	    	
        Scene( Object **l_, int n_, Light *luz_)
        { 
        	list = l_;
        	list_size = n_;
        	luz = luz_;
        }
};

#endif
