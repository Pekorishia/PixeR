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
#include "../includes/difuse_shader.h"
#include "../includes/normal_shader.h"

using json = nlohmann::json;

std::string fileReader(std::string file, int & n_cols, int & n_rows, int & n_samples, int & ray_depth, float & t_min, float & t_max, std::string & codification, Camera *cam, Shader *shade, Scene *world)
{

    // read the json file
    std::ifstream i("../scenes_files/" + file);
    json j;
    i >> j;

    n_cols = j["image"]["width"];
    n_rows = j["image"]["height"];
    //n_rows = 1;
    n_samples = j["image"]["samples"];
    ray_depth = j["image"]["ray_depth"];
    t_min = j["image"]["t_min"];

    if (j["image"]["t_max"] == "infinity")
        t_max = std::numeric_limits<float>::infinity();
    else
        t_max = j["image"]["t_max"];


    Object *list[j["scene"]["spheres"].size()];

    //Spheres creation
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

        Material *mat = new Matted(kd, ks, ka, a);

        point3 center (j["scene"]["spheres"][i]["center"]["x"],
        j["scene"]["spheres"][i]["center"]["y"],
        j["scene"]["spheres"][i]["center"]["z"]);

        auto radius = j["scene"]["spheres"][i]["radius"];

        list[1] = new Sphere(mat, center, radius);
    }

    world  = new Scene(list, 2);

    //Shader creation
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
    else{
        shade = new DifuseShader(world);
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

    return j["image"]["name"];
}

void fileWriter( Raytrace *raytrace, std::string name, int n_cols, int n_rows)
{

    std::ofstream arq;
    
    std::string path = "../images/";
    path = path + name;

    arq.open (path.c_str(), std::ofstream::trunc);

    std::stringstream ss;
    ss << "P3\n" << n_cols << " " << n_rows << "\n255\n";

    raytrace->render(ss);

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
    std::string file;   
    std::string codification; 

    if(argc >1)
        file = std::string(argv[1]);
    else
        file = "scene_file.json";

    std::string name = fileReader(file, n_cols, n_rows, n_samples, ray_depth, t_min, t_max, codification, cam, shade, world);

    Raytrace *raytrace = new Raytrace (cam, world, shade, n_cols, n_rows,n_samples, ray_depth, t_min, t_max);

    fileWriter(raytrace, name, n_cols, n_rows);

	return 0;
}