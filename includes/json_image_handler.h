#ifndef _JSONIMAGE_H_
#define _JSONIMAGE_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string> 

#include "scene.h"
#include "sphere.h"
#include "light.h"
#include "raytrace.h"
#include "background.h"

#include "depth_shader.h"
#include "difuse_shader.h"
#include "normal_shader.h"
#include "blinnphong_shader.h"

#include "matted_material.h"
#include "blinnphong_material.h"

#include "../utility/json.hpp"

using json = nlohmann::json;

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
        Object *list[ j["scene"]["spheres"].size() ];

        // Spheres creation
            for(int i=0; i<j["scene"]["spheres"].size(); i++){

                // Material creation
                    rgb kd (j["scene"]["spheres"][i]["material"]["albedo"]["r"],
                    j["scene"]["spheres"][i]["material"]["albedo"]["g"],
                    j["scene"]["spheres"][i]["material"]["albedo"]["b"]);

                    rgb ks (j["scene"]["spheres"][i]["material"]["specular"]["r"],
                    j["scene"]["spheres"][i]["material"]["specular"]["g"],
                    j["scene"]["spheres"][i]["material"]["specular"]["b"]);

                    rgb ka (j["scene"]["spheres"][i]["material"]["ambient"]["r"],
                    j["scene"]["spheres"][i]["material"]["ambient"]["g"],
                    j["scene"]["spheres"][i]["material"]["ambient"]["b"]);

                    auto a = j["scene"]["spheres"][i]["material"]["alpha"];

                    Material *mat;

                    if (j["scene"]["spheres"][i]["material"]["type"] == "matted"){
                        mat = new Matted(kd, ks, ka, a);
                    }
                    else if (j["scene"]["spheres"][i]["material"]["type"] == "blinnphong"){
                        mat = new BlinnPhong(kd, ks, ka, a);
                    }

                point3 center (j["scene"]["spheres"][i]["center"]["x"],
                j["scene"]["spheres"][i]["center"]["y"],
                j["scene"]["spheres"][i]["center"]["z"]);

                auto radius = j["scene"]["spheres"][i]["radius"];

                list[i] = new Sphere(mat, center, radius);
            }

        // Light creation
            Light *lum[ j["scene"]["light"].size() ];

            for(int i=0; i<j["scene"]["light"].size(); i++){
                vec3 direction (j["scene"]["light"][i]["direction"]["x"],
                                j["scene"]["light"][i]["direction"]["y"],
                                j["scene"]["light"][i]["direction"]["z"]);

                rgb intensity (j["scene"]["light"][i]["intensity"]["r"],
                               j["scene"]["light"][i]["intensity"]["g"],
                               j["scene"]["light"][i]["intensity"]["b"]);

                lum[i] = new Light( direction, intensity);
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

        world  = new Scene(list, j["scene"]["spheres"].size(), lum, j["scene"]["light"].size(), bg, al);

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
        else if (j["shader"]["type"] == "difuse"){
            shade = new DifuseShader(world);
        }
        else if (j["shader"]["type"] == "normal"){
            shade = new NormalShader(world);
        }
        else if (j["shader"]["type"] == "blinnphong"){
            shade = new BlinnphongShader(world);
        }
    // Camera creation
        point3 llc (j["camera"]["lower_left_corner"]["x"],
                    j["camera"]["lower_left_corner"]["y"],
                    j["camera"]["lower_left_corner"]["z"]);

        vec3 horizontal (j["camera"]["horizontal"]["x"],
                         j["camera"]["horizontal"]["y"],
                         j["camera"]["horizontal"]["z"]);

        vec3 vertical (j["camera"]["vertical"]["x"],
                       j["camera"]["vertical"]["y"],
                       j["camera"]["vertical"]["z"]);

        point3 origin (j["camera"]["origin"]["x"],
                       j["camera"]["origin"]["y"],
                       j["camera"]["origin"]["z"]);

        cam = new Camera(llc, horizontal, vertical, origin);

	
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