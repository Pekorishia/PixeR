#ifndef _Box_H_
#define _Box_H_

#include "object.h"
#include "triangle_object.h"


class Box : public Object {
    public:

        vector<point3> vertices;
        vector<Triangle*> triangles;

        Box();


        Box(Material *material_, point3 p0, point3 p7){

            point3 p1 = point3(p7.x(), p0.y(), p0.z());
            point3 p2 = point3(p7.x(), p7.y(), p0.z());
            point3 p3 = point3(p0.x(), p7.y(), p0.z());
            point3 p4 = point3(p0.x(), p7.y(), p7.z());
            point3 p5 = point3(p0.x(), p0.y(), p7.z());
            point3 p6 = point3(p7.x(), p0.y(), p7.z());

            vertices.push_back(p0);
            vertices.push_back(p1);
            vertices.push_back(p2);
            vertices.push_back(p3);
            vertices.push_back(p4);
            vertices.push_back(p5);
            vertices.push_back(p6);
            vertices.push_back(p7);

            triangles.push_back(new Triangle(material_, p0, p1, p2)); //FRENTE
            triangles.push_back(new Triangle(material_, p0, p2, p3)); //FRENTE
            triangles.push_back(new Triangle(material_, p5, p6, p1)); //CIMA
            triangles.push_back(new Triangle(material_, p1, p0, p5)); //CIMA
            triangles.push_back(new Triangle(material_, p0, p3, p4)); //DIREITA
            triangles.push_back(new Triangle(material_, p0, p4, p5)); //DIREITA
            triangles.push_back(new Triangle(material_, p3, p2, p4)); //BAIXO
            triangles.push_back(new Triangle(material_, p4, p2, p7)); //BAIXO
            triangles.push_back(new Triangle(material_, p1, p7, p2)); //ESQUERDA
            triangles.push_back(new Triangle(material_, p7, p1, p6)); //ESQUERDA
            triangles.push_back(new Triangle(material_, p5, p4, p7)); //FUNDO
            triangles.push_back(new Triangle(material_, p6, p5, p7)); //FUNDO

            Object::material = material_;
        }

        virtual bool hit( const Ray & r_, float  t_min_, float  t_max_, HitRecord & ht_ ) const;
        
        Box* wrap(Box* node, Box* bbox_tri)const;
};


bool Box::hit( const Ray & r_, float  t_min_, float  t_max_, HitRecord & ht_ ) const{
    for(unsigned int i = 0; i < triangles.size(); i++){
        if(triangles[i]->hit(r_, t_min_, t_max_, ht_)){
            return true;
        }
    }
    return false;
}

Box* Box::wrap(Box* node, Box* bbox_tri)const{

    if (node->vertices[7].x() > bbox_tri->vertices[7].x())
     {
                node->vertices[7][0] = bbox_tri->vertices[7].x();
            }
            if (node->vertices[7].y() > bbox_tri->vertices[7].y())
            {
                node->vertices[7][1] = bbox_tri->vertices[7].y();
            }
            if (node->vertices[7].z() > bbox_tri->vertices[7].z())
            {
                node->vertices[7][2] = bbox_tri->vertices[7].z();
            }

            if (node->vertices[0].x() < bbox_tri->vertices[0].x())
            {
                node->vertices[0][0] = bbox_tri->vertices[0].x();
            }
            if (node->vertices[0].y() < bbox_tri->vertices[0].y())
            {
                node->vertices[0][1] = bbox_tri->vertices[0].y();
            }
            if (node->vertices[0].z() < bbox_tri->vertices[0].z())
            {
                node->vertices[0][2] = bbox_tri->vertices[0].z();
            }

    Box* result = new Box(Object::material, node->vertices[0], node->vertices[7]);
    return result;
}

#endif