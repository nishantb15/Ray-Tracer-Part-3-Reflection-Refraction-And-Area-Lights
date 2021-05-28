#ifndef AARECT_H
#define AARECT_H

#include "util.h"

#include "hittable.h"

class xy_rect : public hittable {
    public:
        xy_rect() {}

        xy_rect(double _x0, double _x1, double _y0, double _y1, double _k, 
            shared_ptr<material> mat)
            : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};

		/*	Determines whether ray hits the rect
		*	@r: ray to cast
		*	@t_min: the min t value
		*	@t_max: the max t value
		*	@rec: the hit record struct
		*	returns true if sphere intersects ray
		*/
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

		/*	Constructs a bounding box for a sphere.
		*	@time0: t0 time interval for moving objects
		*	@time1: t1 time interval for moving objects
		*	@output_box: output bounding box
		*	Returns an AABB.
		*/
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            // The bounding box must have non-zero width in each dimension, so pad the Z
            // dimension a small amount.
            output_box = aabb(vec3(x0,y0, k-0.0001), vec3(x1, y1, k+0.0001));
            return true;
        }

    public:
        shared_ptr<material> mp;
        double x0, x1, y0, y1, k;
};

/*	Hit function for xy_rect
*	@r: the ray to cast
*	@t_min: min t value
*	@t_max: max t value
*	@rec: hit record of point
*	returns true if hit, false otherwise
*/
bool xy_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    auto t = (k-r.origin().z()) / r.direction().z();
    if (t < t_min || t > t_max)
        return false;
    auto x = r.origin().x() + t*r.direction().x();
    auto y = r.origin().y() + t*r.direction().y();
    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;
    rec.u = (x-x0)/(x1-x0);
    rec.v = (y-y0)/(y1-y0);
    rec.t = t;
    auto outward_normal = vec3(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);
    return true;
}

#endif
