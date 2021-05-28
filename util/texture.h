#ifndef TEXTURE_H
#define TEXTURE_H

#include "util.h"
#include "perlin.h"

class texture {
    public:
		/*	returns the texture value at p
		*	@u: u texture coordinate
		*	@v: v texture coordinate
		*	@p: point
		*	retuns the texture value at p
		*/
        virtual vec3 value(double u, double v, const vec3& p) const = 0;
};

class solid_color : public texture {
    public:
        solid_color() {}
        solid_color(vec3 c) : color_value(c) {}

        solid_color(double red, double green, double blue)
          : solid_color(vec3(red,green,blue)) {}

		/*	returns the texture value at p
		*	@u: u texture coordinate
		*	@v: v texture coordinate
		*	@p: point
		*	retuns the texture value at p
		*/
        virtual vec3 value(double u, double v, const vec3& p) const override {
            return color_value;
        }

    private:
        vec3 color_value;
};


class checker_texture : public texture {
    public:
        checker_texture() {}

        checker_texture(shared_ptr<texture> _even, shared_ptr<texture> _odd)
            : even(_even), odd(_odd) {}

        checker_texture(vec3 c1, vec3 c2)
            : even(make_shared<solid_color>(c1)) , odd(make_shared<solid_color>(c2)) {}

		/*	returns the texture value at p
		*	@u: u texture coordinate
		*	@v: v texture coordinate
		*	@p: point
		*	retuns the texture value at p
		*/
        virtual vec3 value(double u, double v, const vec3& msn) const override {
			vec3 p = vec3();
			p[0] = msn[0];
			p[1] = msn[1];
			p[2] = msn[2];
            auto sines = sin(10*p.x())*sin(10*p.y())*sin(10*p.z());
            if (sines < 0)
                return odd->value(u, v, p);
            else
                return even->value(u, v, p);
        }

    public:
        shared_ptr<texture> odd;
        shared_ptr<texture> even;
};

class noise_texture : public texture {
    public:
        noise_texture() {}
        noise_texture(double sc) : scale(sc) {}

		/*	returns the texture value at p
		*	@u: u texture coordinate
		*	@v: v texture coordinate
		*	@p: point
		*	retuns the texture value at p
		*/
        virtual vec3 value(double u, double v, const vec3& msn) const override {
            // return color(1,1,1)*0.5*(1 + noise.turb(scale * p));
            // return color(1,1,1)*noise.turb(scale * p);
			vec3 p = vec3();
			p[0] = msn[0];
			p[1] = msn[1];
			p[2] = msn[2];
            return vec3(1,1,1)*0.5*(1 + sin(scale*p.z() + 10*noise.turb(p)));
        }

    public:
        perlin noise;
        double scale;
};
#endif
