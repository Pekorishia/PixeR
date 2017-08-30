#include <iostream>
#include <fstream>

#include "../includes/scene.h"
#include "../includes/sphere.h"
#include "../includes/raytrace.h"
#include "../includes/difuse_shader.h"
#include "../includes/normal_shader.h"
#include "../includes/depth_shader.h"

int main (){

    int n_cols{ 1000 };     
    int n_rows{ 500 };
    int n_samples{ 100 }; 
    int ray_depth{ 10 };

    float t_min{0.001f};
    float t_max{std::numeric_limits<float>::infinity()};

    Object *list[2];

    list[0] = new Sphere(point3 (0, 0, -1), 0.5);
    list[1] = new Sphere(point3 (0, -100.5, -1), 100);

    Scene *world  = new Scene(list, 2);

    //Shader *shade = new DifuseShader(world);
    Shader *shade = new DepthShader(world, 0.0, 4.0, rgb(1,1,1),  rgb(0,0,0));

    Camera *cam = new Camera();

    Raytrace *raytrace = new Raytrace (cam, world, shade, n_cols, n_rows,n_samples, ray_depth, t_min, t_max);

    raytrace->render();

	return 0;
}