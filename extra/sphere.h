#ifndef SPHERE_H
#define SPHERE_H

#include "../util/hittable.h"
#include "../util/vec3.h"

class sphere : public hittable {
    public:
        sphere() {}
        sphere(vec3 cen, double r, shared_ptr<material> maty) : center(cen), radius(r), mat(maty) {};

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;
		virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

    public:
        vec3 center;
        double radius;
		shared_ptr<material> mat;
};

/*	Determines whether ray hits sphere
*	@r: ray to cast
*	@t_min: the min t value
*	@t_max: the max t value
*	@rec: the hit record struct
*	returns true if sphere intersects ray
*/
bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    rec.n = (rec.p - center) / radius;
	rec.mat_ptr = mat;

	vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    return true;
}

/*	Constructs a bounding box for a sphere.
*	@time0: t0 time interval for moving objects
*	@time1: t1 time interval for moving objects
*	@output_box: output bounding box
*	Returns an AABB.
*/
bool sphere::bounding_box(double time0, double time1, aabb& output_box) const {
    output_box = aabb(
        center - vec3(radius, radius, radius),
        center + vec3(radius, radius, radius));
    return true;
}
#endif
