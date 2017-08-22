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

        inline bool found( const Ray & r_, float t_min_, float t_max_, HitRecord & ht_ )
        {
        	HitRecord temp_ht;

			bool hit_anything = false;
			double closest_so_far = t_max_;

			for( int i = 0; i < list_size; i++){
				if (list[i]->hit(r_, t_min_, closest_so_far, temp_ht)){
					hit_anything = true;
					closest_so_far = temp_ht.t;
					ht_ = temp_ht;
				}
			}

			return hit_anything;
        }
};

#endif
