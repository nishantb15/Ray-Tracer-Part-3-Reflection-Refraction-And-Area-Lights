#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "aabb.h"
#include "util.h"

class material;

struct hit_record {
    vec3 p;
    vec3 n;
    double t;
	vec3 kd;
	vec3 ks;
	int v1i;
	int v2i;
	int v3i;
	shared_ptr<material> mat_ptr;
	bool front_face;
	double u;
	double v;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        n = front_face ? outward_normal :-outward_normal;
    }
};

class hittable {
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
		virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0;
};

#endif
