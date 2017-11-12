#include <iostream>
#include <fstream>
#include <sstream>
#include <string> 
#include <chrono>

#include "../includes/scene.h"
#include "../includes/camera.h"
#include "../includes/shader.h"
#include "../includes/json_image_handler.h"

using namespace std;


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

    // Create a buffer to hold all the image information
    std::stringstream ss;

    // Verify if one specific scene json file was passed  
    if (argc > 1)
        scene = std::string(argv[1]);
    else
        scene = "scene_file.json";

     chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();

    // Reads the data from the json and return the image name
    std::string name = JsonImage::jsonImageHandler(ss, scene, n_cols, n_rows, n_samples, ray_depth, t_min, t_max, cam, shade, world);

    // Generates the image
    JsonImage::imageGenerator(ss, name);

    chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = t2 - t1;
    std::cout << "\nRendering time: " << duration.count() << " seconds." << std::endl;

    return 0;
}