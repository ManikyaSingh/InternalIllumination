#ifndef _POINT_SIMPLE_INCLUDED_
#define _POINT_SIMPLE_INCLUDED_

class Point{
public:
	Vector v;
	Color color;
	Point(){

	}
	Point(Vector a, Color b){
		v = a;
		color = b;
	}
	Point(const Point& p){
		v = p.v;
		color = p.color;
	}
	void print(){
		printf("\n%f %f %f - %d %d %d %d\n", v.x, v.y, v.z, color.r, color.g, color.b, color.a);
	}
};

#endif