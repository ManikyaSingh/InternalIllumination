#ifndef _RAY_SIMPLE_INCLUDED_
#define _RAY_SIMPLE_INCLUDED_

class Ray{
public:
	Vector v;
	Vector d;
	Ray(){

	}
	Ray(Vector a, Vector b){
		v = a;
		d = b;
	}
	Ray(const Ray& rr){
		v = rr.v;
		d = rr.d;
	}
};

#endif