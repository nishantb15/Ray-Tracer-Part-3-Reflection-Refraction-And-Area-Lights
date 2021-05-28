#include "vec3.h"
#include "util.h"

#include <cmath>
#include <iostream>

using std::sqrt;

/* Empty Constructor for vec3
*/
vec3::vec3() {
	v[0] = 0;
	v[1] = 0;
	v[2] = 0;
}

/* Constructor for vec3
	@v1: x component
	@v2: y component
	@v3: z component
*/
vec3::vec3(double v1, double v2, double v3) {
	v[0] = v1;
	v[1] = v2;
	v[2] = v3;
}

/*vec3(vec3& v2) {
	v[0] = v2[0];
	v[1] = v2[1];
	v[2] = v2[2];
}*/

/* Returns the x-component of the vector.
*/
double vec3::x() {
	return v[0];
}

/* Returns the y-component of the vector.
*/
double vec3::y() {
	return v[1];
}

/* Returns the z-component of the vector.
*/
double vec3::z() {
	return v[2];
}

/* Returns the length of the vector (equlidian 2-norm).
*/
double vec3::length() {
	return sqrt((v[0]*v[0]) + (v[1]*v[1]) + (v[2]*v[2]));
}

/* Returns the square of the length of the vector (euclidian 2-norm).
*/
double vec3::length_squared() {
	return length()*length();
}

/* Returns a vector with all its components negated.
*/
vec3 vec3::operator-() const {
	return vec3(-v[0],-v[1],-v[2]);
}

/* Used to return the ith element of v. 
*/
double vec3::operator[](int i) const {
	return v[i];
}

/* Used to return the ith element of v. 
*/
double& vec3::operator[](int i) {
	return v[i];
}

/* This operator does a component wise addition
*	of v and v1.
*	@v1: the vector whose components will be added to v
*	returns: A reference to itself. 
*/
vec3& vec3::operator+=(const vec3 &v1) {
	v[0] += v1.v[0];
	v[1] += v1.v[1];
	v[2] += v1.v[2];
	return *this;
}

/* This operator does a scalar multiplication
*	of v and t.
*	@t: the scalar value.
*	returns: A reference to itself. 
*/
vec3& vec3::operator*=(const double t) {
	v[0] *= t;
	v[1] *= t;
	v[2] *= t;
	return *this;
}

/* This operator does a scalar multiplication with v and
*	the reciprocal of t.
*	@v1: the scalar value.
*	returns: A reference to itself. 
*/
vec3& vec3::operator/=(const double t) {
	v[0] /= t;
	v[1] /= t;
	v[2] /= t;
	return *this;
}

/* Returns true if all components of the vector
*	are very small (close to 0)
*/
bool vec3::near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        const auto s = 1e-8; // 1e-8
        return (fabs(v[0]) < s) && (fabs(v[1]) < s) && (fabs(v[2]) < s);
}

inline vec3 vec3::random() {
	return vec3(random_double(), random_double(), random_double());
}

inline vec3 vec3::random(double min, double max) {
	return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
}

/* Allows us to easily print out the contents of a vec3.
*	@out: The ouput stream to write to.
*	@v1: the vector whose components will be added to v
*	returns: A reference to itself. 
*/
std::ostream& operator<<(std::ostream &out, const vec3 &v1) {
    return out << v1[0] << ' ' << v1[1] << ' ' << v1[2];
}

/* Performs a component-wise addition ofS two vectors.
*	@u1: the first vector
*	@u2: the second vector
*	returns: A new vec3 whose components are the sum of 
*	the components of u1 and u2.
*/
vec3 operator+(const vec3 &u1, const vec3 &u2) {
    return vec3(u1[0] + u2[0], u1[1] + u2[1], u1[2] + u2[2]);
}

/* Performs a component-wise subtraction of two vectors.
*	@u1: the first vector
*	@u2: the second vector
*	returns: A new vec3 whose components are the difference of 
*	the components of u1 and u2.
*/
vec3 operator-(const vec3 &u1, const vec3 &u2) {
    return vec3(u1[0] - u2[0], u1[1] - u2[1], u1[2] - u2[2]);
}

/* Performs a component-wise multiplication of two vec3 objects.
*	@u1: the first vector
*	@u2: the second vector
*	returns: A new vec3 whose components are the product of 
*	the components of u1 and u2.
*/
vec3 operator*(const vec3 &u1, const vec3 &u2) {
    return vec3(u1[0] * u2[0], u1[1] * u2[1], u1[2] * u2[2]);
}

/* Performs a scalar multplication.
*	@t: scalar value
*	@v1: the vec3 to multiply with
*	returns: A new vec3 whose components are multiplied
*	by t
*/
vec3 operator*(double t, const vec3 &v1) {
    return vec3(t*v1[0], t*v1[1], t*v1[2]);
}

/* Performs a scalar multplication.
*	@v: the vec3 to multiply with
*	@t: scalar value
*	returns: A new vec3 whose components are multiplied
*	by t.
*/
vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

/* Performs a scalar multplication.
*	@v: the vec3 to multiply with
*	@t: reciprocal of scalar value
*	returns: A new vec3 whose components are multiplied
*	by 1/t.
*/
vec3 operator/(vec3 v, double t) {
    return (1/t) * v;
}

/* Performs a dot product
*	@u1: First vec3
*	@u2: Second vec3
*	returns the dot product of u1 and u2.
*/
double dot(const vec3 &u1, const vec3 &u2) {
    return u1[0] * u2[0]
         + u1[1] * u2[1]
         + u1[2] * u2[2];
}

/* Performs a cross product
*	@u1: First vec3
*	@u2: Second vec3
*	returns the cross product of u1 and u2.
*/
vec3 cross(const vec3 &u1, const vec3 &u2) {
    return vec3(u1[1] * u2[2] - u1[2] * u2[1],
                u1[2] * u2[0] - u1[0] * u2[2],
                u1[0] * u2[1] - u1[1] * u2[0]);
}

/* normalizes the vec3 v
*	@v: Vec3 to normalize.
*	returns the normalized vec3.
*/
vec3 normalize(vec3 v) {
    return v / v.length();
}

/*	Translates the point
*	@point: point to translate
*	@dx: the amount in x
*	@dy: the amount in y
*	@dz: the amount in z
*	returns the translated vec3
*/
vec3 translate(vec3 point, double dx, double dy, double dz) {
	vec3 ret;
	ret[0] = point[0] + dx;
	ret[1] = point[1] + dy;
	ret[2] = point[2] + dx;
	return ret;
}

/*	Translates the point uniformly in x,y,z
*	@point: point to translate
*	@d: amount in x,y,z
*	returns the translated vec3
*/
vec3 uniformTranslate(vec3 point, double d) {
	vec3 ret;
	ret[0] = point[0] + d;
	ret[1] = point[1] + d;
	ret[2] = point[2] + d;
	return ret;
}

/*	Scales the point
*	@point: point to scale
*	@a: scale in x
*	@b: scale in y
*	@c: scale in z
*	returns the scaled point
*/
vec3 scale(vec3 point, double a, double b, double c) {
	vec3 ret;
	ret[0] = point[0] * a;
	ret[1] = point[1] * b;
	ret[2] = point[2] * c;
	return ret;
}

/*	Scales the point uniformly in x,y,z
*	@point: point to scale
*	@s: the scale in x,y,z
*	returns the scaled point
*/
vec3 uniformScale(vec3 point, double s) {
	vec3 ret;
	ret[0] = point[0] * s;
	ret[1] = point[1] * s;
	ret[2] = point[2] * s;
	return ret;
}

/*	Rotates the point counter-clockwise by theta in x axis
*	@point: point to rotate
*	@theta: angle (in deg)
*	returns the rotated point
*/
vec3 rotateX(vec3 point, double theta) {
	vec3 ret;
	double x = point[0];
	double y = point[1];
	double z = point[2];
	double angle = degToRad(theta);
	ret[0] = point[0];
	ret[1] = (y*cos(angle)) - (z*sin(angle));
	ret[2] = (y*sin(angle)) + (z*cos(angle));
	return ret;
}

/*	Rotates the point counter-clockwise by theta in y axis
*	@point: point to rotate
*	@theta: angle (in deg)
*	returns the rotated point
*/
vec3 rotateY(vec3 point, double theta) {
	vec3 ret;
	double x = point[0];
	double y = point[1];
	double z = point[2];
	double angle = degToRad(theta);
	ret[0] = (x*cos(angle)) + (z*sin(angle));
	ret[1] = point[1];
	ret[2] = ((-x)*sin(angle)) + (z*cos(angle));
	return ret;
}

/*	Rotates the point counter-clockwise by theta in z axis
*	@point: point to rotate
*	@theta: angle (in deg)
*	returns the rotated point
*/
vec3 rotateZ(vec3 point, double theta) {
	vec3 ret;
	double x = point[0];
	double y = point[1];
	double z = point[2];
	double angle = degToRad(theta);
	ret[0] = (x*cos(angle)) - (y*sin(angle));
	ret[1] = ((x)*sin(angle)) + (y*cos(angle));
	ret[2] = point[2];
	return ret;
}

/*	Rotates the point counter-clockwise by theta in axis
*	@point: point to rotate
*	@theta: angle (in deg)
*	@axis: axis to rotate (1=X, 2=Y, 3=Z)
*	returns the rotated point
*/
vec3 rotate(vec3 point, double theta, int axis) {
	double x = point[0];
	double y = point[1];
	double z = point[2];
	if (axis == 0) return rotateX(point, theta);
	else if (axis == 1) return rotateY(point, theta);
	if (axis > 2) std::cout << "Invalid axis in rotate, calling rotateZ" << std::endl;
	return rotateZ(point, theta);
}

/*	Rotates the point counter-clockwise by first shifting to origin
*	and then rotates the new point by theta in axis and then shifts back
*	@point: point to rotate
*	@theta: angle (in deg)
*	@axis: axis to rotate (1=X, 2=Y, 3=Z)
*	returns the rotated point
*/
vec3 rotateAboutPoint(vec3 point, double theta, int axis) {
	double x = point[0];
	double y = point[1];
	double z = point[2];
	vec3 ret = translate(point, -x, -y, -z);
	if (axis == 0) ret = rotateX(ret, theta);
	else if (axis == 1) ret = rotateY(ret, theta);	
	else
		ret = rotateZ(point, theta);
	
	ret = translate(ret, x, y, z);
	return ret;
}

/* Returns the reflected vector of v about normal n
*	@v: vector to reflect
*	@n: the normal of the surface
*	returns the reflected vector of v
*/
vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n)*n;
}

/*	Returns the refracted vector of uc about normal n
*	@uv: vector to refract
*	@n: normal of the surface
*	@etai_over_etat: refractive index
*	returns the partially refracted-reflected vector
*/
vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

/*	Returns a vector with random x,y,z values between (0,1]
*	returns a vec3 with random x,y,z
*/
vec3 randomVec() {
        return vec3(random_double(), random_double(), random_double());
}

/*	Returns a vector with random x,y,z values between [min,max)
*	@min: The min value (inclusive)
*	@max: The max value (exclusive)
*	returns a vec3 with random x,y,z between [min,max)
*/
vec3 randomVec(double min, double max) {
        return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
}

/*	Gets a random vector inside a unit disk
*	returns a vec3
*/
inline vec3 random_in_unit_disk() {
    while (true) {
        auto p = vec3(random_double(-1,1), random_double(-1,1), 0);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

/*	Gets a random vector inside a unit sphere
*	returns a vec3
*/
vec3 random_in_unit_sphere() {
    while (true) {
        auto p = randomVec(-1,1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

/* Gets the normalized random vector inside a unit sphere
*	returns a vec3
*/
vec3 random_unit_vector() {
	return normalize(random_in_unit_sphere());
}

/* Gets the random vector that is in the same hemisphere as normal
*	@normal: normal to compare
*	returns a vec3
*/
vec3 random_in_hemisphere(const vec3& normal) {
    vec3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}
