/*
*	@author: Nishant Bhushan
*		bhushan3
*/
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "util/hittable.h"
#include "util/ray.cpp"
#include "util/hittable_list.cpp"
#include "util/util.h"
#include "util/material.h"
#include "util/aarect.h"

#include "extra/camera.h"
#include "extra/sphere.h"

#include <chrono> 
using namespace std::chrono; 

using namespace std;
using std::string;

float gam = 2.0;
unsigned long num_rays = 0;

/* Writes the color to the output stream
*	@out: The output stream to write the color to
*	@pixel_color: The color to write
*	@samples_per_pixel: samples per pixel
*/
void write_color(std::ostream &out, vec3 pixel_color, int samples_per_pixel) {
    // Write the translated [0,255] value of each color component.
	auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples.
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;
	r = pow(r,1.0/gam);
	g = pow(g,1.0/gam);
	b = pow(b,1.0/gam);

    out << static_cast<int>(255.999 * r) << ' '
        << static_cast<int>(255.999 * g) << ' '
        << static_cast<int>(255.999 * b) << '\n';
}

/* Clamps the value of components of color.
*	to the interval [0,1]
*	@color: The vec3 to clamp
*	a vec3 with components clamped to [0,1].
*/
vec3 clamp(vec3 color) {
	color[0] = (color[0] > 1) ? 1 : color[0];
	color[1] = (color[1] > 1) ? 1 : color[1];
	color[2] = (color[2] > 1) ? 1 : color[2];
	color[0] = (color[0] < 0) ? 0 : color[0];
	color[1] = (color[1] < 0) ? 0 : color[1];
	color[2] = (color[2] < 0) ? 0 : color[2];
	return color;
}

/* Casts a ray to determine if it hits any objects in the scene. Uses Material Shading.
*	@r: The ray to cast.
*	@world: The list of hittable objects to test ray intersection with
*	@depth: The max amount of depth of recursion
*/
/*vec3 ray_color(const ray& r, vec3 background, const hittable& world, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return vec3(0,0,0);

    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        vec3 attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, background, world, depth-1);
        return vec3(0,0,0);
    }

    vec3 unit_direction = normalize(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
}*/
vec3 ray_color(const ray& r, const vec3& background, const hittable& world, int depth) {
	// we have just cast a new ray
	num_rays += 1;
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return vec3(0,0,0);

    // If the ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    ray scattered;
    vec3 attenuation;
    vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_color(scattered, background, world, depth-1);
}

/*	Generates a scene to demonstrate area lighting
*	returns a hittable list of objects in the scene
*/
hittable_list area_light() {
    hittable_list objects;

    auto pertext = make_shared<noise_texture>(4);
    //objects.add(make_shared<sphere>(vec3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    //objects.add(make_shared<sphere>(vec3(0,2,0), 2, make_shared<lambertian>(pertext)));
	objects.add(make_shared<sphere>(vec3(0,-1000,0), 1000, make_shared<lambertian>(vec3(0.5,0.3,0.9))));
	objects.add(make_shared<sphere>(vec3(0,2,0), 2, make_shared<metal>(vec3(0,0.2,0.9))));
	//objects.add(make_shared<sphere>(vec3(-20,4.5,5), 5, make_shared<metal>(vec3(0.8,0.2,0.2))));
	objects.add(make_shared<sphere>(vec3(7,2,5), 2, make_shared<metal>(vec3(0.8,0.8,0.2))));

    auto difflight = make_shared<diffuse_light>(vec3(4,4,4));
    objects.add(make_shared<sphere>(vec3(0,7,0), 2, difflight));
    objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));

    return objects;
}

/* The main method to run everything.
*	compile using: g++ mp1.cpp -std=c++11 -o mp1
*	./mp2 0 400 1.7 > output.ppm
*	@argc: The size of args array
*	@args: The arguments provided by the command line
*/
int main() {
	auto program_start = high_resolution_clock::now();

    // Image

    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 700;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    int samples_per_pixel = 100;
	const int max_depth = 50;

	vec3 background(0,0,0);
	const vec3 vup(0,1,0);
    const auto dist_to_focus = 10.0;
	cameraDefault cam;

    // World
	hittable_list world;

    auto material_ground = make_shared<lambertian>(vec3(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(vec3(0.7, 0.3, 0.3));
    //auto material_left   = make_shared<metal>(vec3(0.8, 0.8, 0.8));
	auto material_left   = make_shared<dielectric>(1.5);
    auto material_right  = make_shared<metal>(vec3(0.8, 0.6, 0.2));
	auto metal1  = make_shared<metal>(vec3(0.8, 0.3, 0.7));
	auto metal2  = make_shared<metal>(vec3(0.2, 0.3, 0.8));
	auto metal3  = make_shared<metal>(vec3(0.2, 0.9, 0.8));
	auto glass1   = make_shared<dielectric>(1.2);

    world.add(make_shared<sphere>(vec3( 0.0, -100.5, -1.5), 100.0, material_ground));
    world.add(make_shared<sphere>(vec3( 0.8,    0.8, -3),   1.0, material_center));	// lambertian center
    world.add(make_shared<sphere>(vec3(-2.0,    0.0, -1.5),   0.5, material_left));
    world.add(make_shared<sphere>(vec3( 2.0,    0.0, -1.5),   0.5, material_right));

	/*world.add(make_shared<sphere>(vec3(-2.0,    2, -1.5),   0.5, metal1));
	world.add(make_shared<sphere>(vec3( 0,      0, -1.5),   0.5, glass1));
	world.add(make_shared<sphere>(vec3( 4.0,    0.0, -1.5),   0.5, metal2));
	world.add(make_shared<sphere>(vec3(-4.0,    0.0, -1.5),   0.5, metal3));*/

	// Image

    /*auto aspect_ratio = 16.0 / 9.0;
    int image_width = 700;
    int samples_per_pixel = 100; // 100
    int max_depth = 50;

    // World
	hittable_list world;
    vec3 lookfrom;
    vec3 lookat;
    auto vfov = 40.0;
    auto aperture = 0.0;
    vec3 background(0,0,0);

    // Camera
    //AREA LIGHTS
	world = area_light();
    samples_per_pixel = 400; // 400
    lookfrom = vec3(26,3,6);
    lookat = vec3(0,2,0);
    vfov = 20.0;

	// Camera

    const vec3 vup(0,1,0);
    const auto dist_to_focus = 10.0;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);*/

    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        //std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            vec3 pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, background, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

	// Get Time of program and number of rays sent into the scene. 
	// Comment out write_color above and uncomment below.
	/*
	auto program_stop = high_resolution_clock::now();
	// Subtract stop and start timepoints and 
	// cast it to required unit. Predefined units 
	// are nanoseconds, microseconds, milliseconds, 
	// seconds, minutes, hours. Use duration_cast() 
	// function. 
	auto duration_s = duration_cast<seconds>(program_stop - program_start); 
	auto duration_ms = duration_cast<milliseconds>(program_stop - program_start); 
  
	// To get the value of duration use the count() 
	// member function on the duration object 
	cout << "Program duration(s): " << duration_s.count() << " s" << endl;
	cout << "Program duration(ms): " << duration_ms.count() << " ms" << endl;
	cout << "num rays = " << num_rays << endl;*/
}
