#ifndef MATERIAL_H
#define MATERIAL_H

#include "util.h"
#include "texture.h"

struct hit_record;

class material {
    public:
		/*	Returns an emitted color at point p
		*	@u: u texture coordinate
		*	@v: v texture coordinate
		*	@p: vec3 point
		*	returns a color at that point
		*/ 
		virtual vec3 emitted(double u, double v, const vec3& p) const {
            return vec3(0,0,0);
        }

		/* scatters light ray according to material
		*	@r_in: ray to scatter
		*	@rec: hit record struct
		*	@attenuation: how much the light should be attenuated by
		*	@scattered: the scattered ray
		*	returns true if scattering occurs
		*/
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered
        ) const = 0;
};

class lambertian : public material {
    public:
        lambertian(const vec3& a) : albedo(make_shared<solid_color>(a)) {}
        lambertian(shared_ptr<texture> a) : albedo(a) {}

		/* scatters light ray according to material
		*	@r_in: ray to scatter
		*	@rec: hit record struct
		*	@attenuation: how much the light should be attenuated by
		*	@scattered: the scattered ray
		*	returns true if scattering occurs
		*/
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered
        ) const override {
            auto scatter_direction = rec.n + random_unit_vector();

            // Catch degenerate scatter direction
            if (scatter_direction.near_zero())
                scatter_direction = rec.n;

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo->value(rec.u, rec.v, rec.p);
            return true;
        }

    public:
        shared_ptr<texture> albedo;
};

class metal : public material {
    public:
        metal(const vec3& a) : albedo(a) {}

		/* scatters light ray according to material
		*	@r_in: ray to scatter
		*	@rec: hit record struct
		*	@attenuation: how much the light should be attenuated by
		*	@scattered: the scattered ray
		*	returns true if scattering occurs
		*/
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered
        ) const override {
            vec3 reflected = reflect(normalize(r_in.direction()), rec.n);
			double eps = 1e-5;
            scattered = ray(rec.p + vec3(eps,eps,eps)*normalize(reflected), normalize(reflected));
            attenuation = albedo;
            return (dot(scattered.direction(), rec.n) > 0);
        }

    public:
        vec3 albedo;
};

class dielectric : public material {
    public:
        dielectric(double index_of_refraction) : ir(index_of_refraction) {}

		/* scatters light ray according to material
		*	@r_in: ray to scatter
		*	@rec: hit record struct
		*	@attenuation: how much the light should be attenuated by
		*	@scattered: the scattered ray
		*	returns true if scattering occurs
		*/
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered
        ) const override {
            attenuation = vec3(1.0, 1.0, 1.0);
            double refraction_ratio = rec.front_face ? (1.0/ir) : ir;

            vec3 unit_direction = normalize(r_in.direction());
            double cos_theta = fmin(dot(-unit_direction, rec.n), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            vec3 direction;

            if (cannot_refract || reflectance(cos_theta, refraction_ratio))
                direction = reflect(unit_direction, rec.n);
            else
                direction = refract(unit_direction, rec.n, refraction_ratio);

            scattered = ray(rec.p, direction);
            return true;
        }

    public:
        double ir; // Index of Refraction

    private:
		/*	Uses Schlick's approximation to determine if we should reflect
		*	@cosine: cos value of ray to normal
		*	@ref_idx: refraction ratio
		*	returns a boolean to determine if we should reflect
		*/
        static bool reflectance(double cosine, double ref_idx) {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1-ref_idx) / (1+ref_idx);
            r0 = r0*r0;
            return r0 + (1-r0)*pow((1 - cosine),5) > random_double();
        }
};

class diffuse_light : public material  {
    public:
        diffuse_light(shared_ptr<texture> a) : emit(a) {}
        diffuse_light(vec3 c) : emit(make_shared<solid_color>(c)) {}

		/* scatters light ray according to material
		*	@r_in: ray to scatter
		*	@rec: hit record struct
		*	@attenuation: how much the light should be attenuated by
		*	@scattered: the scattered ray
		*	returns true if scattering occurs
		*/
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered
        ) const override {
            return false;
        }

		/*	Returns an emitted color at point p
		*	@u: u texture coordinate
		*	@v: v texture coordinate
		*	@p: vec3 point
		*	returns a color at that point
		*/ 
        virtual vec3 emitted(double u, double v, const vec3& p) const override {
            return emit->value(u, v, p);
        }

    public:
        shared_ptr<texture> emit;
};

#endif
