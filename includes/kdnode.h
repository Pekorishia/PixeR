#ifndef _KDNode_H_
#define _KDNode_H_

#include <algorithm>

#include "triangle_object.h"

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

KDNode::KDNode(){ }

bool ordenarX(Triangle* i, Triangle* j){
    return i->get_midpoint().x() < j->get_midpoint().x();
}

bool ordenarY(Triangle* i, Triangle* j){
    return i->get_midpoint().y() < j->get_midpoint().y();
}

bool ordenarZ(Triangle* i, Triangle* j){
    return i->get_midpoint().z() < j->get_midpoint().z();
}

KDNode* KDNode::build(vector<Triangle*>& tris, int depth)const
{
    KDNode* node = new KDNode();
    node->triangles = tris;
    node->left = NULL;
    node->right = NULL;
    Cube* cube_;
    node->bbox = NULL;

    if(tris.size() == 0)
        return node;

    if (tris.size() == 1)
    {
        node->bbox = tris[0]->bbox;
        node->left = new KDNode();
        node->right = new KDNode();
        node->left->triangles = std::vector<Triangle*> ();
        node->right->triangles = std::vector<Triangle*> ();
        return node;
    }

    vector<Triangle*> left_tris = std::vector<Triangle*>();
    vector<Triangle*> right_tris = std::vector<Triangle*>();

    int axis = depth%3;
    switch(axis){
        case 0:
            std::sort(tris.begin(), tris.end(), ordenarX);
            break;
        case 1:
            std::sort(tris.begin(), tris.end(), ordenarY);
            break;
        case 2:
            std::sort(tris.begin(), tris.end(), ordenarZ);            
            break;
        }

    node->bbox = tris[0]->bbox;

    for (int i = 1; i < tris.size(); ++i)
    {
        node->bbox = node->bbox->wrap(node->bbox, tris[i]->bbox);
    }    
    
    int contAux = tris.size()/2;

    for (int i = 0; i < contAux; ++i)
    {
        left_tris.push_back(tris[i]);
    }

    for (int i = contAux; i < tris.size(); ++i)
    {
        right_tris.push_back(tris[i]);
    }
    
    node->left = build(left_tris, depth+1);
    node->right = build(right_tris, depth+1);
    
    return node;
}
bool KDNode::hit (KDNode* node, const Ray & r_, float  t_min_, float  t_max_, HitRecord & ht_ ) const
{
    if (node->bbox->hit(r_, t_min_, t_max_, ht_)){
        HitRecord left_ht, right_ht;
        if(node->left->triangles.size() > 0 || node->right->triangles.size()>0){
            bool hitleft = hit(node->left, r_, t_min_, t_max_, left_ht);
            bool hitright = hit(node->right, r_, t_min_, t_max_, right_ht);
            if(hitleft && hitright){
                if(left_ht.t < right_ht.t)
                    ht_ = left_ht;
                else
                    ht_ = right_ht;
                return true;
            }
            else if (hitleft)
            {
                 ht_ = left_ht;
                 return true;
            }
            else if (hitright)
            {
                 ht_ = right_ht;
                 return true;
            }
            else
                return false;
        }
        else{
            for (int i = 0; i < node->triangles.size(); ++i)
            {
                if(node->triangles[i]->hit(r_, t_min_, t_max_, ht_)){
                   return true;
                }
            }
            return false;
        }
    }
    else
        return false;
}
#endif