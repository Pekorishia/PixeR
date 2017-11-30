#ifndef _JSONIMAGE_H_
#define _JSONIMAGE_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string> 
#include <complex>  
#include <vector>
#include <math.h> 
#include <ios>      // std::left
#include <iomanip> // setw

#include "scene.h"
#include "raytrace.h"
#include "background.h"


#include "light.h"
#include "spot_light.h"
#include "point_light.h"
#include "directional_light.h"

#include "toon_shader.h"
#include "depth_shader.h"
#include "lambertian_shader.h"
#include "normal_shader.h"
#include "blinnphong_shader.h"

#include "toon_material.h"
#include "metal_material.h"
#include "lambertian_material.h"
#include "blinnphong_material.h"
#include "dielectrics_material.h"

#include "perspective_camera.h"
#include "parallel_camera.h"


#include "sphere.h"
#include "plane.h"
#include "ellipsoid.h"
#include "triangle_object.h"

#include "../utility/json.hpp"
// vec3, vec4, ivec4, mat4
#include "../utility/glm/glm.hpp"
// translate, rotate, scale, perspective
#include "../utility/glm/gtc/matrix_transform.hpp"

using json = nlohmann::json;

#define PI 3.14159265

class JsonImage
{
    public:
        static std::string jsonImageHandler (std::stringstream &ss, std::string file, int & n_cols, int & n_rows, int & n_samples, int & ray_depth, float & t_min, float & t_max, Camera* &cam, Shader* &shade, Scene* &world);

		static void imageGenerator (std::stringstream &ss, std::string name);
};

/// Prints a column-major 4x4 matriz.
std::ostream & operator<<( std::ostream& os, const glm::mat4& m )
{
    for (auto i(0) ; i < 4 ; ++i )
    {
        os << "[ ";
        for ( auto j(0) ; j < 4 ; ++j )
            os << std::fixed << std::right << std::setw(6) << std::setprecision(2) << m[j][i] << " ";
        os << "]\n";
    }

    return os;
}

/// Prints a 3D vector.
std::ostream & operator<<( std::ostream& os, const glm::vec4& v )
{
        os << "[ ";
        for ( auto j(0) ; j < 4 ; ++j )
            os << std::right << std::setw(6) << std::setprecision(2) << v[j] << " ";
        os << "]";

    return os;
}

/// User-define literal to convert from degrees to radian.
constexpr long double operator"" _deg ( long double deg )
{
    return deg*3.141592/180;
}

constexpr float deg ( long double deg )
{
    return deg*3.141592/180;
}



/*
 * Opens the json file and generates the image data
 * Return the image file name
 */
std::string JsonImage::jsonImageHandler(std::stringstream &ss, std::string file, int & n_cols, int & n_rows, int & n_samples, int & ray_depth, float & t_min, float & t_max, Camera* &cam, Shader* &shade, Scene* &world)
{

    // read the json file and put it inside 'j'
    std::ifstream i("../scenes_files/" + file);
    json j;
    i >> j;

    // Number of columns and rows
    n_cols = j["image"]["width"];
    n_rows = j["image"]["height"];
    // Number of rays that will be fired inside one pixel
    n_samples = j["image"]["samples"];
    // Number of times allowed to one ray bounce
    ray_depth = j["image"]["ray_depth"];

    // The minimum depth allowed
    t_min = j["image"]["t_min"];

    // The maximum depth allowed
    if (j["image"]["t_max"] == "infinity")
        t_max = std::numeric_limits<float>::infinity();
    else
        t_max = j["image"]["t_max"];


    // Scene Creation
    int qtd_sphere = j["scene"]["objects"]["spheres"].size();
    int qtd_triangle = j["scene"]["objects"]["triangles"].size();
    int qtd_plane = j["scene"]["objects"]["plane"].size();
    int qtd_ellipsoid = j["scene"]["objects"]["ellipsoid"].size();

    int qtd_obj = qtd_triangle + qtd_sphere + qtd_plane;// + qtd_ellipsoid;

    Object *list[qtd_obj];

        // Spheres creation
            for(int i=0; i<j["scene"]["objects"]["spheres"].size(); i++) {

                // Material creation
                    Material *mat;

                    if (j["scene"]["objects"]["spheres"][i]["material"]["type"] == "lambertian"){
                        rgb kd (j["scene"]["objects"]["spheres"][i]["material"]["albedo"]["r"],
                                j["scene"]["objects"]["spheres"][i]["material"]["albedo"]["g"],
                                j["scene"]["objects"]["spheres"][i]["material"]["albedo"]["b"]);
                        
                        mat = new Lambertian(kd);
                    }
                    else if (j["scene"]["objects"]["spheres"][i]["material"]["type"] == "blinnphong"){
                        rgb kd (j["scene"]["objects"]["spheres"][i]["material"]["albedo"]["r"],
                                j["scene"]["objects"]["spheres"][i]["material"]["albedo"]["g"],
                                j["scene"]["objects"]["spheres"][i]["material"]["albedo"]["b"]);
                        
                        rgb ks (j["scene"]["objects"]["spheres"][i]["material"]["specular"]["r"],
        	                    j["scene"]["objects"]["spheres"][i]["material"]["specular"]["g"],
        	                    j["scene"]["objects"]["spheres"][i]["material"]["specular"]["b"]);

	                    rgb ka (j["scene"]["objects"]["spheres"][i]["material"]["ambient"]["r"],
        	                    j["scene"]["objects"]["spheres"][i]["material"]["ambient"]["g"],
        	                    j["scene"]["objects"]["spheres"][i]["material"]["ambient"]["b"]);

                        rgb km (j["scene"]["objects"]["spheres"][i]["material"]["mirrow"]["r"],
                                j["scene"]["objects"]["spheres"][i]["material"]["mirrow"]["g"],
                                j["scene"]["objects"]["spheres"][i]["material"]["mirrow"]["b"]);

	                    auto a = j["scene"]["objects"]["spheres"][i]["material"]["alpha"];

                        mat = new BlinnPhong(kd, ks, km, ka, a);
                    }
                    else if (j["scene"]["objects"]["spheres"][i]["material"]["type"] == "metal"){
                        rgb kd (j["scene"]["objects"]["spheres"][i]["material"]["albedo"]["r"],
                                j["scene"]["objects"]["spheres"][i]["material"]["albedo"]["g"],
                                j["scene"]["objects"]["spheres"][i]["material"]["albedo"]["b"]);                        
                        
                        auto fuzz = j["scene"]["objects"]["spheres"][i]["material"]["fuzz"];
                        mat = new Metal(kd, fuzz);
                    }
                    else if (j["scene"]["objects"]["spheres"][i]["material"]["type"] == "dielectrics"){
                        rgb kd (j["scene"]["objects"]["spheres"][i]["material"]["albedo"]["r"],
                                j["scene"]["objects"]["spheres"][i]["material"]["albedo"]["g"],
                                j["scene"]["objects"]["spheres"][i]["material"]["albedo"]["b"]);                        
                        
                        auto ref_idx = j["scene"]["objects"]["spheres"][i]["material"]["ref_idx"];
                        mat = new Dielectrics(kd, ref_idx);
                    }
                    else if(j["scene"]["objects"]["spheres"][i]["material"]["type"] == "toon"){
                        std::vector<rgb> gradient;
                        rgb gradient_;

                        std::vector<float> angles;
                        float angles_;

                        for(int k=0; k<j["scene"]["objects"]["spheres"][i]["material"]["gradient"].size(); k++){
                            gradient_ = rgb(j["scene"]["objects"]["spheres"][i]["material"]["gradient"][k]["r"],
                                            j["scene"]["objects"]["spheres"][i]["material"]["gradient"][k]["g"],
                                            j["scene"]["objects"]["spheres"][i]["material"]["gradient"][k]["b"]);
                            gradient.push_back(gradient_);

                            angles_=j["scene"]["objects"]["spheres"][i]["material"]["angles"][k]["a"];
                            
                            angles_ = cos(deg(angles_));
                            angles.push_back(angles_);
                        }
                        mat = new Toon(gradient, angles);
                    }
                    
                glm::vec4 center (j["scene"]["objects"]["spheres"][i]["center"]["x"],
                                j["scene"]["objects"]["spheres"][i]["center"]["y"],
                                j["scene"]["objects"]["spheres"][i]["center"]["z"], 
                                j["scene"]["objects"]["spheres"][i]["center"]["homogeneous"]);

                float radius = j["scene"]["objects"]["spheres"][i]["radius"];

                glm::mat4 transformations = glm::mat4(1.0f);

                // Transformation Creation
                    for(int k=0; k< j["scene"]["objects"]["spheres"][i]["transformation"].size(); k++) 
                    {
                        if (j["scene"]["objects"]["spheres"][i]["transformation"][k]["type"] == "translate")
                        {
                            glm::vec3 translate_factor ( j["scene"]["objects"]["spheres"][i]["transformation"][k]["change"]["x"],
                                                         j["scene"]["objects"]["spheres"][i]["transformation"][k]["change"]["y"],
                                                         j["scene"]["objects"]["spheres"][i]["transformation"][k]["change"]["z"]);
                            glm::mat4 translate = glm::translate(
                                    glm::mat4(1.0f), // Identity.
                                    translate_factor);
                           
                            transformations  = translate * transformations;
                        }
                        else if(j["scene"]["objects"]["spheres"][i]["transformation"][k]["type"] == "rotate")
                        {
                            //glm::vec3 center_ = -center;
                            glm::mat4 translateA = glm::translate(glm::mat4(1.0f), glm::vec3(-center));

                            glm::vec3 rotate_factor ( (j["scene"]["objects"]["spheres"][i]["transformation"][k]["change"]["x"]),
                                                      (j["scene"]["objects"]["spheres"][i]["transformation"][k]["change"]["y"]),
                                                      (j["scene"]["objects"]["spheres"][i]["transformation"][k]["change"]["z"]));
 
                            glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f), deg(rotate_factor.z) , glm::vec3 (0.0f,0.0f,1.0f) );
                            glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f), deg(rotate_factor.y) , glm::vec3 (0.0f,1.0f,0.0f) );
                            glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f), deg(rotate_factor.x) , glm::vec3 (1.0f,0.0f,0.0f) );

                            glm::mat4 translate_ = glm::translate(glm::mat4(1.0f), glm::vec3(center));

                            transformations = translateA * transformations;
                            transformations = rotateX * rotateY * rotateZ * transformations;
                            transformations = translate_ * transformations;
                        }
                        else if(j["scene"]["objects"]["spheres"][i]["transformation"][k]["type"] == "scale")
                        {
                            float scale_factor = j["scene"]["objects"]["spheres"][i]["transformation"][k]["change"];

                            radius  = scale_factor * radius;
                        }
                    }
                
                center = transformations * center;

                list[i+ qtd_triangle] = new Sphere(mat, point3 (center[0],center[1],center[2]), radius);
                
            }

        // Triangle creation
            for(int i=0; i<j["scene"]["objects"]["triangles"].size(); i++){

                // Material creation
                    Material *mat;

                    if (j["scene"]["objects"]["triangles"][i]["material"]["type"] == "lambertian"){
                        rgb kd (j["scene"]["objects"]["triangles"][i]["material"]["albedo"]["r"],
                                j["scene"]["objects"]["triangles"][i]["material"]["albedo"]["g"],
                                j["scene"]["objects"]["triangles"][i]["material"]["albedo"]["b"]);
                        
                        mat = new Lambertian(kd);
                    }
                    else if (j["scene"]["objects"]["triangles"][i]["material"]["type"] == "blinnphong"){
                        rgb kd (j["scene"]["objects"]["triangles"][i]["material"]["albedo"]["r"],
                                j["scene"]["objects"]["triangles"][i]["material"]["albedo"]["g"],
                                j["scene"]["objects"]["triangles"][i]["material"]["albedo"]["b"]);
                        
                        rgb ks (j["scene"]["objects"]["triangles"][i]["material"]["specular"]["r"],
                                j["scene"]["objects"]["triangles"][i]["material"]["specular"]["g"],
                                j["scene"]["objects"]["triangles"][i]["material"]["specular"]["b"]);

                        rgb ka (j["scene"]["objects"]["triangles"][i]["material"]["ambient"]["r"],
                                j["scene"]["objects"]["triangles"][i]["material"]["ambient"]["g"],
                                j["scene"]["objects"]["triangles"][i]["material"]["ambient"]["b"]);

                        rgb km (j["scene"]["objects"]["triangles"][i]["material"]["mirrow"]["r"],
                                j["scene"]["objects"]["triangles"][i]["material"]["mirrow"]["g"],
                                j["scene"]["objects"]["triangles"][i]["material"]["mirrow"]["b"]);

                        auto a = j["scene"]["objects"]["triangles"][i]["material"]["alpha"];

                        mat = new BlinnPhong(kd, ks, km, ka, a);
                    }
                    else if (j["scene"]["objects"]["triangles"][i]["material"]["type"] == "metal"){
                        rgb kd (j["scene"]["objects"]["triangles"][i]["material"]["albedo"]["r"],
                                j["scene"]["objects"]["triangles"][i]["material"]["albedo"]["g"],
                                j["scene"]["objects"]["triangles"][i]["material"]["albedo"]["b"]);                        
                        
                        auto fuzz = j["scene"]["objects"]["triangles"][i]["material"]["fuzz"];
                        mat = new Metal(kd, fuzz);
                    }
                    else if (j["scene"]["objects"]["triangles"][i]["material"]["type"] == "dielectrics"){
                        rgb kd (j["scene"]["objects"]["triangles"][i]["material"]["albedo"]["r"],
                                j["scene"]["objects"]["triangles"][i]["material"]["albedo"]["g"],
                                j["scene"]["objects"]["triangles"][i]["material"]["albedo"]["b"]);                        
                        
                        auto ref_idx = j["scene"]["objects"]["triangles"][i]["material"]["ref_idx"];
                        mat = new Dielectrics(kd, ref_idx);
                    }
                    else if(j["scene"]["objects"]["triangles"][i]["material"]["type"] == "toon"){
                        std::vector<rgb> gradient;
                        rgb gradient_;

                        std::vector<float> angles;
                        float angles_;

                        for(int k=0; k<j["scene"]["objects"]["triangles"][i]["material"]["gradient"].size(); k++){
                            gradient_ = rgb(j["scene"]["objects"]["triangles"][i]["material"]["gradient"][k]["r"],
                                            j["scene"]["objects"]["triangles"][i]["material"]["gradient"][k]["g"],
                                            j["scene"]["objects"]["triangles"][i]["material"]["gradient"][k]["b"]);
                            gradient.push_back(gradient_);

                            angles_=j["scene"]["objects"]["triangles"][i]["material"]["angles"][k]["a"];
                            
                            angles_ = cos(angles_* PI / 180.0);
                            angles.push_back(angles_);
                        }
                        mat = new Toon(gradient, angles);
                    }
                    
                glm::vec4 v0 (j["scene"]["objects"]["triangles"][i]["v0"]["x"],
                            j["scene"]["objects"]["triangles"][i]["v0"]["y"],
                            j["scene"]["objects"]["triangles"][i]["v0"]["z"],
                            j["scene"]["objects"]["triangles"][i]["v0"]["homogeneous"]);

                glm::vec4 v1 (j["scene"]["objects"]["triangles"][i]["v1"]["x"],
                            j["scene"]["objects"]["triangles"][i]["v1"]["y"],
                            j["scene"]["objects"]["triangles"][i]["v1"]["z"],
                            j["scene"]["objects"]["triangles"][i]["v1"]["homogeneous"]);

                glm::vec4 v2 (j["scene"]["objects"]["triangles"][i]["v2"]["x"],
                            j["scene"]["objects"]["triangles"][i]["v2"]["y"],
                            j["scene"]["objects"]["triangles"][i]["v2"]["z"],
                            j["scene"]["objects"]["triangles"][i]["v2"]["homogeneous"]);

                glm::mat4 transformations = glm::mat4(1.0f);

                // Transformation Creation
                    for(int k=0; k< j["scene"]["objects"]["triangles"][i]["transformation"].size(); k++) 
                    {
                        if (j["scene"]["objects"]["triangles"][i]["transformation"][k]["type"] == "translate")
                        {
                            glm::vec3 translate_factor ( j["scene"]["objects"]["triangles"][i]["transformation"][k]["change"]["x"],
                                                         j["scene"]["objects"]["triangles"][i]["transformation"][k]["change"]["y"],
                                                         j["scene"]["objects"]["triangles"][i]["transformation"][k]["change"]["z"]);
                            glm::mat4 translate = glm::translate(
                                    glm::mat4(1.0f), // Identity.
                                    translate_factor);
                           
                            transformations  = translate * transformations;
                        }
                        else if (j["scene"]["objects"]["triangles"][i]["transformation"][k]["type"] == "rotate")
                        {
                            glm::mat4 translate = glm::translate( glm::mat4(1.0f), glm::vec3(-v0.x, -v0.y,-v0.z) ); 

                            // The rotation angles for each axis.
                            glm::vec3 rotate_factor ( (j["scene"]["objects"]["triangles"][i]["transformation"][k]["change"]["x"]),
                                                      (j["scene"]["objects"]["triangles"][i]["transformation"][k]["change"]["y"]),
                                                      (j["scene"]["objects"]["triangles"][i]["transformation"][k]["change"]["z"]));
 
                            glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f), deg(rotate_factor.z) , glm::vec3 (0.0f,0.0f,1.0f) );
                            glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f), deg(rotate_factor.y) , glm::vec3 (0.0f,1.0f,0.0f) );
                            glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f), deg(rotate_factor.x) , glm::vec3 (1.0f,0.0f,0.0f) );

                            glm::mat4 translate_ = glm::translate( glm::mat4(1.0f), glm::vec3(v0.x, v0.y,v0.z) ); 

                            transformations = translate * transformations;
                            transformations = rotateX * rotateY * rotateZ * transformations;
                            transformations = translate_ * transformations;
                        }
                        else if (j["scene"]["objects"]["triangles"][i]["transformation"][k]["type"] == "scale")
                        {                           
                            // The rotation angles for each axis.
                            float scale_factor = j["scene"]["objects"]["triangles"][i]["transformation"][k]["change"];
 
                            glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3 (scale_factor,scale_factor,scale_factor) );

                            transformations  = scale * transformations;
                        }
                    }
                
                v0 = transformations * v0;
                v1 = transformations * v1;
                v2 = transformations * v2;
                
                list[i] = new Triangle(mat, point3 (v0[0], v0[1], v0[2]), point3 (v1[0], v1[1], v1[2]), point3 (v2[0], v2[1], v2[2]));
            }

            // Plane creation
            for(int i=0; i<j["scene"]["objects"]["plane"].size(); i++){

                // Material creation
                    Material *mat;

                    if (j["scene"]["objects"]["plane"][i]["material"]["type"] == "blinnphong"){
                        rgb kd (j["scene"]["objects"]["plane"][i]["material"]["albedo"]["r"],
                                j["scene"]["objects"]["plane"][i]["material"]["albedo"]["g"],
                                j["scene"]["objects"]["plane"][i]["material"]["albedo"]["b"]);
                        
                        rgb ks (j["scene"]["objects"]["plane"][i]["material"]["specular"]["r"],
                                j["scene"]["objects"]["plane"][i]["material"]["specular"]["g"],
                                j["scene"]["objects"]["plane"][i]["material"]["specular"]["b"]);

                        rgb ka (j["scene"]["objects"]["plane"][i]["material"]["ambient"]["r"],
                                j["scene"]["objects"]["plane"][i]["material"]["ambient"]["g"],
                                j["scene"]["objects"]["plane"][i]["material"]["ambient"]["b"]);

                        rgb km (j["scene"]["objects"]["plane"][i]["material"]["mirrow"]["r"],
                                j["scene"]["objects"]["plane"][i]["material"]["mirrow"]["g"],
                                j["scene"]["objects"]["plane"][i]["material"]["mirrow"]["b"]);

                        auto a = j["scene"]["objects"]["plane"][i]["material"]["alpha"];

                        mat = new BlinnPhong(kd, ks, km, ka, a);
                    }
                    else if (j["scene"]["objects"]["plane"][i]["material"]["type"] == "lambertian"){
                        rgb kd (j["scene"]["objects"]["plane"][i]["material"]["albedo"]["r"],
                                j["scene"]["objects"]["plane"][i]["material"]["albedo"]["g"],
                                j["scene"]["objects"]["plane"][i]["material"]["albedo"]["b"]);
                        
                        mat = new Lambertian(kd);
                    }
                    else if (j["scene"]["objects"]["plane"][i]["material"]["type"] == "metal"){
                        rgb kd (j["scene"]["objects"]["plane"][i]["material"]["albedo"]["r"],
                                j["scene"]["objects"]["plane"][i]["material"]["albedo"]["g"],
                                j["scene"]["objects"]["plane"][i]["material"]["albedo"]["b"]);                        
                        
                        auto fuzz = j["scene"]["objects"]["triangles"][i]["material"]["fuzz"];
                        mat = new Metal(kd, fuzz);
                    }
                    else if (j["scene"]["objects"]["plane"][i]["material"]["type"] == "dielectrics"){
                        rgb kd (j["scene"]["objects"]["plane"][i]["material"]["albedo"]["r"],
                                j["scene"]["objects"]["plane"][i]["material"]["albedo"]["g"],
                                j["scene"]["objects"]["plane"][i]["material"]["albedo"]["b"]);                        
                        
                        auto ref_idx = j["scene"]["objects"]["plane"][i]["material"]["ref_idx"];
                        mat = new Dielectrics(kd, ref_idx);
                    }
                    else if(j["scene"]["objects"]["plane"][i]["material"]["type"] == "toon"){
                        std::vector<rgb> gradient;
                        rgb gradient_;

                        std::vector<float> angles;
                        float angles_;

                        for(int k=0; k<j["scene"]["objects"]["plane"][i]["material"]["gradient"].size(); k++){
                            gradient_ = rgb(j["scene"]["objects"]["plane"][i]["material"]["gradient"][k]["r"],
                                            j["scene"]["objects"]["plane"][i]["material"]["gradient"][k]["g"],
                                            j["scene"]["objects"]["plane"][i]["material"]["gradient"][k]["b"]);
                            gradient.push_back(gradient_);

                            angles_=j["scene"]["objects"]["plane"][i]["material"]["angles"][k]["a"];
                            
                            angles_ = cos(angles_* PI / 180.0);
                            angles.push_back(angles_);
                        }
                        mat = new Toon(gradient, angles);
                    }
                    
                    
                    
                glm::vec4 v0 (j["scene"]["objects"]["plane"][i]["v0"]["x"],
                            j["scene"]["objects"]["plane"][i]["v0"]["y"],
                            j["scene"]["objects"]["plane"][i]["v0"]["z"],
                            j["scene"]["objects"]["plane"][i]["v0"]["homogeneous"]);

                glm::vec4 v1 (j["scene"]["objects"]["plane"][i]["v1"]["x"],
                            j["scene"]["objects"]["plane"][i]["v1"]["y"],
                            j["scene"]["objects"]["plane"][i]["v1"]["z"],
                            j["scene"]["objects"]["plane"][i]["v1"]["homogeneous"]);

                glm::vec4 v2 (j["scene"]["objects"]["plane"][i]["v2"]["x"],
                            j["scene"]["objects"]["plane"][i]["v2"]["y"],
                            j["scene"]["objects"]["plane"][i]["v2"]["z"],
                            j["scene"]["objects"]["plane"][i]["v2"]["homogeneous"]);

                glm::vec4 v3 (j["scene"]["objects"]["plane"][i]["v3"]["x"],
                            j["scene"]["objects"]["plane"][i]["v3"]["y"],
                            j["scene"]["objects"]["plane"][i]["v3"]["z"],
                            j["scene"]["objects"]["plane"][i]["v3"]["homogeneous"]);


                
                list[i + qtd_triangle + qtd_sphere] = new Plane(mat, point3 (v0[0], v0[1], v0[2]), point3 (v1[0], v1[1], v1[2]), point3 (v2[0], v2[1], v2[2]) , point3(v3[0], v3[1], v3[2]));
            }

            // Ellipsoid creation
            for(int i=0; i<j["scene"]["objects"]["ellipsoid"].size(); i++){

                // Material creation
                    Material *mat;

                    if (j["scene"]["objects"]["ellipsoid"][i]["material"]["type"] == "blinnphong"){
                        rgb kd (j["scene"]["objects"]["ellipsoid"][i]["material"]["albedo"]["r"],
                                j["scene"]["objects"]["ellipsoid"][i]["material"]["albedo"]["g"],
                                j["scene"]["objects"]["ellipsoid"][i]["material"]["albedo"]["b"]);
                        
                        rgb ks (j["scene"]["objects"]["ellipsoid"][i]["material"]["specular"]["r"],
                                j["scene"]["objects"]["ellipsoid"][i]["material"]["specular"]["g"],
                                j["scene"]["objects"]["ellipsoid"][i]["material"]["specular"]["b"]);

                        rgb ka (j["scene"]["objects"]["ellipsoid"][i]["material"]["ambient"]["r"],
                                j["scene"]["objects"]["ellipsoid"][i]["material"]["ambient"]["g"],
                                j["scene"]["objects"]["ellipsoid"][i]["material"]["ambient"]["b"]);

                        rgb km (j["scene"]["objects"]["ellipsoid"][i]["material"]["mirrow"]["r"],
                                j["scene"]["objects"]["ellipsoid"][i]["material"]["mirrow"]["g"],
                                j["scene"]["objects"]["ellipsoid"][i]["material"]["mirrow"]["b"]);

                        auto a = j["scene"]["objects"]["ellipsoid"][i]["material"]["alpha"];

                        mat = new BlinnPhong(kd, ks, km, ka, a);
                    }
                    
                    
                    
                glm::vec4 center (j["scene"]["objects"]["ellipsoid"][i]["center"]["x"],
                            j["scene"]["objects"]["ellipsoid"][i]["center"]["y"],
                            j["scene"]["objects"]["ellipsoid"][i]["center"]["z"],
                            j["scene"]["objects"]["ellipsoid"][i]["center"]["homogeneous"]);

                glm::vec4 size (j["scene"]["objects"]["ellipsoid"][i]["size"]["x"],
                            j["scene"]["objects"]["ellipsoid"][i]["size"]["y"],
                            j["scene"]["objects"]["ellipsoid"][i]["size"]["z"],
                            j["scene"]["objects"]["ellipsoid"][i]["size"]["homogeneous"]);


                
                list[i + qtd_triangle + qtd_sphere + qtd_ellipsoid] = new Ellipsoid(mat, point3 (center[0], center[1], center[2]) , point3(size[0], size[1], size[2]));
            }


        // Light creation
            Light *lum[ j["scene"]["light"].size() ];

            for(int i=0; i<j["scene"]["light"].size(); i++)
            {                
                rgb intensity (j["scene"]["light"][i]["intensity"]["r"],
                               j["scene"]["light"][i]["intensity"]["g"],
                               j["scene"]["light"][i]["intensity"]["b"]);

                if(j["scene"]["light"][i]["type"] == "point")
                {                    
                    point3 origin1 (j["scene"]["light"][i]["origin"]["x"],
                                   j["scene"]["light"][i]["origin"]["y"],
                                   j["scene"]["light"][i]["origin"]["z"]);

                    lum[i] = new PointLight( origin1, intensity);
                }
                else if (j["scene"]["light"][i]["type"] == "spot") 
                {
                    point3 origin1 (j["scene"]["light"][i]["origin"]["x"],
                                   j["scene"]["light"][i]["origin"]["y"],
                                   j["scene"]["light"][i]["origin"]["z"]);
                    
                    vec3 direction (j["scene"]["light"][i]["direction"]["x"],
                                    j["scene"]["light"][i]["direction"]["y"],
                                    j["scene"]["light"][i]["direction"]["z"]);

                    float angle = j["scene"]["light"][i]["angle"];
                    angle = fabs(cos(angle * PI / 180.0));

                    lum[i] = new SpotLight( origin1, direction, intensity, angle);
                }
                else 
                {
                    vec3 direction (j["scene"]["light"][i]["direction"]["x"],
                                    j["scene"]["light"][i]["direction"]["y"],
                                    j["scene"]["light"][i]["direction"]["z"]);

                    lum[i] = new DirectionLight( direction, intensity);
                }

                
            }

        // Background creation
            rgb tl (j["scene"]["background"]["upper_left"]["r"],
                   j["scene"]["background"]["upper_left"]["g"],
                   j["scene"]["background"]["upper_left"]["b"]);

            rgb tr (j["scene"]["background"]["upper_right"]["r"],
                   j["scene"]["background"]["upper_right"]["g"],
                   j["scene"]["background"]["upper_right"]["b"]);

            rgb ll (j["scene"]["background"]["lower_left"]["r"],
                   j["scene"]["background"]["lower_left"]["g"],
                   j["scene"]["background"]["lower_left"]["b"]);

            rgb lr (j["scene"]["background"]["lower_right"]["r"],
                   j["scene"]["background"]["lower_right"]["g"],
                   j["scene"]["background"]["lower_right"]["b"]);

            Background *bg = new Background(ll, lr, tl, tr);

        // Ambient light creation
            rgb al (j["scene"]["ambient_light"]["r"],
                    j["scene"]["ambient_light"]["g"],
                    j["scene"]["ambient_light"]["b"]);

        world  = new Scene(list, qtd_obj, lum, j["scene"]["light"].size(), bg, al);

    // Shader creation
        if (j["shader"]["type"] == "depth"){

            auto min_depth = j["shader"]["min_depth"];
            auto max_depth = j["shader"]["max_depth"];

            rgb foreground (j["shader"]["foreground"]["r"],
                            j["shader"]["foreground"]["g"],
                            j["shader"]["foreground"]["b"]);

            rgb background (j["shader"]["background"]["r"],
                            j["shader"]["background"]["g"],
                            j["shader"]["background"]["b"]);

            shade = new DepthShader(world, min_depth, max_depth, foreground, background);
        }
        else if (j["shader"]["type"] == "lambertian"){
            shade = new LambertianShader(world);
        }
        else if (j["shader"]["type"] == "normal"){
            shade = new NormalShader(world);
        }
        else if (j["shader"]["type"] == "blinnphong"){
            shade = new BlinnphongShader(world);
        }
        else if (j["shader"]["type"] == "toon"){
            shade = new ToonShader(world);
        }
    // Camera creation
        vec3 origin2 (j["camera"]["origin"]["x"],
                     j["camera"]["origin"]["y"],
                     j["camera"]["origin"]["z"]);

        vec3 lookAt (j["camera"]["lookAt"]["x"],
                     j["camera"]["lookAt"]["y"],
                     j["camera"]["lookAt"]["z"]);

        point3 upVector (j["camera"]["upVector"]["x"],
                         j["camera"]["upVector"]["y"],
                         j["camera"]["upVector"]["z"]);

        if (j["camera"]["type"] == "parallel")
        {
            float left = j["camera"]["left"];

            float right = j["camera"]["right"];

            float bottom = j["camera"]["bottom"];

            float top = j["camera"]["top"];

            cam = new ParallelCamera(origin2, lookAt, upVector, left, right, bottom, top);
        }
        else{

            float fov = j["camera"]["fov"];

            float aperture = j["camera"]["aperture"];

            float focus_distance = j["camera"]["focus_distance"];

            cam = new PerspectiveCamera(origin2, lookAt, upVector, fov, n_cols/n_rows, aperture, focus_distance );
        }
	
	// Image Codification

	if (j["image"]["codification"] == "ascii"){
    	ss << "P3\n" << n_cols << " " << n_rows << "\n255\n";
	}

    Raytrace::render (ss, cam, world, shade, n_cols, n_rows, n_samples, ray_depth, t_min, t_max);


    return j["image"]["name"];
}

void JsonImage::imageGenerator (std::stringstream &ss, std::string name)
{
    // Create the image file
    std::ofstream arq;  
    std::string path = "../images/";
    path = path + name;

    // Open the image file
    arq.open (path.c_str(), std::ofstream::trunc);

    // Put all the buffer inside of the image and closes it
    arq << ss.str();
    arq.close();  
}


#endif