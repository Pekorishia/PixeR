#include <iostream>
#include <fstream>
#include <sstream>
#include <string> 

#include "../utility/json.hpp"
#include "../includes/scene.h"
#include "../includes/matted.h"
#include "../includes/sphere.h"
#include "../includes/raytrace.h"
#include "../includes/depth_shader.h"
#include "../includes/lambertian.h"
#include "../includes/difuse_shader.h"
#include "../includes/normal_shader.h"

using json = nlohmann::json;


/*
 * Opens the json file and get all data inside of it
 */
void fileReader(std::string file, int & n_cols, int & n_rows, int & n_samples, int & ray_depth, float & t_min, float & t_max, std::string & codification, Camera* &cam, Shader* &shade, Scene* &world)
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

                //Material *mat = new Matted(kd, ks, ka, a);
                Material *mat = new Lambertian(kd);

                point3 center (j["scene"]["spheres"][i]["center"]["x"],
                j["scene"]["spheres"][i]["center"]["y"],
                j["scene"]["spheres"][i]["center"]["z"]);

                auto radius = j["scene"]["spheres"][i]["radius"];

                list[i] = new Sphere(mat, center, radius);
            }

        world  = new Scene(list, j["scene"]["spheres"].size());

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

    codification = j["image"]["codification"];

    std::string name = j["image"]["name"];

    // Create the image file
    std::ofstream arq;    
    std::string path = "../images/";
    path = path + name;

    // Open the image file
    arq.open (path.c_str(), std::ofstream::trunc);

    // Create a buffer to hold all the image information
    std::stringstream ss;

    ss << "P3\n" << n_cols << " " << n_rows << "\n255\n";
    Raytrace::render (ss, cam, world, shade, n_cols, n_rows, n_samples, ray_depth, t_min, t_max);

    // Put all the buffer inside of the image and closes it
    arq << ss.str();
    arq.close();   

}


int main( int argc, const char * argv[] )
{
    int n_cols;     
    int n_rows;
    int n_samples; 
    int ray_depth;

    float t_min;
    float t_max;

    Camera *cam;
    Scene *world;
    Shader *shade;
    std::string scene; 
    std::string codification; 

    // Verify if one specific scene json file was passed  
    if (argc > 2)
        scene = std::string(argv[1]);
    else
        scene = "scene_file.json";

    // Reads the data from the json
    fileReader(scene, n_cols, n_rows, n_samples, ray_depth, t_min, t_max, codification, cam, shade, world);

    return 0;
}