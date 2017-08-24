#ifndef _SCENE_H_
#define _SCENE_H_

#include "object.h" 

class Scene {

    public:

        Object **list; 
        int list_size;
    	    	
        //=== Special members
        Scene() {/* empty */}

        Scene( Object **l_, int n_)
        { 
        	list = l_;
        	list_size = n_;
        }

};

#endif
