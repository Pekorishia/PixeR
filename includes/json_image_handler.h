#ifndef _JSONIMAGE_H_
#define _JSONIMAGE_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string> 
#include <vector>
#include <math.h> 

#include "scene.h"
#include "sphere.h"
#include "light.h"
#include "raytrace.h"
#include "background.h"

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


#include "perspective_camera.h"
#include "parallel_camera.h"

#include "triangle_object.h"

#include "../utility/json.hpp"

using json = nlohmann::json;

#define PI 3.14159265

class JsonImage
{
    public:
        static std::string jsonImageHandler (std::stringstream &ss, std::string file, int & n_cols, int & n_rows, int & n_samples, int & ray_depth, float & t_min, float & t_max, Camera* &cam, Shader* &shade, Scene* &world);

		static void imageGenerator (std::stringstream &ss, std::string name);
};

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

    // Nummber of columns and rows
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
        Object *list[qtd_triangle + qtd_sphere];

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
                    else if(j["scene"]["objects"]["spheres"][i]["material"]["type"] == "toon"){
                        std::vector<rgb> gradient;
                        rgb gradient_;

                        std::vector<float> angles;
                        float angles_;

                        for(int k=0; k<j["scene"]["objects"]["spheres"][i]["material"]["gradient"].size(); k++){
                            gradient_=rgb(j["scene"]["objects"]["spheres"][i]["material"]["gradient"][k]["r"],
                                            j["scene"]["objects"]["spheres"][i]["material"]["gradient"][k]["g"],
                                            j["scene"]["objects"]["spheres"][i]["material"]["gradient"][k]["b"]);
                            gradient.push_back(gradient_);

                            angles_=j["scene"]["objects"]["spheres"][i]["material"]["angles"][k]["a"];
                            
                            angles_ = cos(angles_* PI / 180.0);
                            angles.push_back(angles_);
                        }
                        mat = new Toon(gradient, angles);
                    }
                    
                point3 center (j["scene"]["objects"]["spheres"][i]["center"]["x"],
                j["scene"]["objects"]["spheres"][i]["center"]["y"],
                j["scene"]["objects"]["spheres"][i]["center"]["z"]);

                auto radius = j["scene"]["objects"]["spheres"][i]["radius"];
                
                list[i] = new Sphere(mat, center, radius);
                
            }

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
                    else if(j["scene"]["objects"]["triangles"][i]["material"]["type"] == "toon"){
                        std::vector<rgb> gradient;
                        rgb gradient_;

                        std::vector<float> angles;
                        float angles_;

                        for(int k=0; k<j["scene"]["objects"]["triangles"][i]["material"]["gradient"].size(); k++){
                            gradient_=rgb(j["scene"]["objects"]["triangles"][i]["material"]["gradient"][k]["r"],
                                            j["scene"]["objects"]["triangles"][i]["material"]["gradient"][k]["g"],
                                            j["scene"]["objects"]["triangles"][i]["material"]["gradient"][k]["b"]);
                            gradient.push_back(gradient_);

                            angles_=j["scene"]["objects"]["triangles"][i]["material"]["angles"][k]["a"];
                            
                            angles_ = cos(angles_* PI / 180.0);
                            angles.push_back(angles_);
                        }
                        mat = new Toon(gradient, angles);
                    }
                    
                point3 v0 (j["scene"]["objects"]["triangles"][i]["v0"]["x"],
                j["scene"]["objects"]["triangles"][i]["v0"]["y"],
                j["scene"]["objects"]["triangles"][i]["v0"]["z"]);

                point3 v1 (j["scene"]["objects"]["triangles"][i]["v1"]["x"],
                j["scene"]["objects"]["triangles"][i]["v1"]["y"],
                j["scene"]["objects"]["triangles"][i]["v1"]["z"]);

                point3 v2 (j["scene"]["objects"]["triangles"][i]["v2"]["x"],
                j["scene"]["objects"]["triangles"][i]["v2"]["y"],
                j["scene"]["objects"]["triangles"][i]["v2"]["z"]);

                list[i + qtd_sphere] = new Triangle(mat, v0, v1, v2);
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

        world  = new Scene(list, qtd_triangle + qtd_sphere , lum, j["scene"]["light"].size(), bg, al);

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

            float aspect = j["camera"]["aspect"];

            float aperture = j["camera"]["aperture"];

            float focus_distance = j["camera"]["focus_distance"];

            cam = new PerspectiveCamera(origin2, lookAt, upVector, fov, aspect, aperture, focus_distance );
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