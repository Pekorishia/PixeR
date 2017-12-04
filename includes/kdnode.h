#ifndef _KDNode_H_
#define _KDNode_H_

#include "triangle_object.h"
//#include "cube.h"

class KDNode{

    public:

        Cube* bbox;
        KDNode* left;
        KDNode* right; 

        vector<Triangle*> triangles;

        KDNode();

        KDNode* build(vector<Triangle*>& tris, int depth)const;

        bool hit(KDNode* node, const Ray & r_, float  t_min_, float  t_max_, HitRecord & ht_ ) const;
};

KDNode::KDNode(){

}

KDNode* KDNode::build(vector<Triangle*>& tris, int depth)const
{
    KDNode* node = new KDNode();
    node->triangles = tris;
    node->left = NULL;
    node->right = NULL;

    point3 midpt = point3(0,0,0);

    for( int i =0; i < tris.size(); i++){
        midpt = midpt + (tris[i]->get_midpoint() * 1.f/tris.size());
    }

    vector<Triangle*> left_tris;
    vector<Triangle*> right_tris;

    for( int i =0; i < tris.size(); i++){
        midpt.x() >=tris[i]->get_midpoint().x() ? right_tris.push_back(tris[i]) : left_tris.push_back(tris[i]);
    }

    if (left_tris.size() == 0 && right_tris.size() > 0)
        left_tris = right_tris;
    if (left_tris.size() > 0 && right_tris.size() == 0)
        right_tris = left_tris;

    float matches =0;

    for (int i = 0; i < left_tris.size(); ++i)
    {
        for (int j = 0; j < right_tris.size(); ++j)
        {
            if(left_tris[i] == right_tris[j])
                matches = matches + 1;
        }
    }

    if (matches/left_tris.size() < 0.5 && matches/right_tris.size() < 0.5)
    {
        node->left = build(left_tris, depth+1);
        node->right = build(right_tris, depth+1);
    }
    else{
        node->left = new KDNode();
        node->right = new KDNode();
        node->left->triangles = std::vector<Triangle*>();
        node->right->triangles = std::vector<Triangle*>();        
    }

    return node;
}

bool KDNode::hit (KDNode* node, const Ray & r_, float  t_min_, float  t_max_, HitRecord & ht_ ) const
{
    bool hit_tri = false;

    if(node->left->triangles.size() > 0 || node->right->triangles.size()){
        bool hitleft = hit(node->left, r_, t_min_, t_max_, ht_);
        bool hitright = hit(node->right, r_, t_min_, t_max_, ht_);
        return hitleft || hitright;
    }
    else{
        for (int i = 0; i < node->triangles.size(); ++i)
        {
            if(node->triangles[i]->hit(r_, t_min_, t_max_, ht_)){
                hit_tri = true;
            }
        }
    }
    return hit_tri;
}

#endif