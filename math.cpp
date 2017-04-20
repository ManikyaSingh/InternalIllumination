#ifndef _MATH_SIMPLE_INCLUDED_
#define _MATH_SIMPLE_INCLUDED_

#include <iostream>
using namespace std;
#include <math.h>


double EPS = ldexp (0.5 , -38);

int compare(double a, double b) {
	double c = a-b;
	double d = c;
	if(c<0) c *= -1;
	
	double param, result;
	int n;

	if(c < EPS && c > -EPS){
		return 0;
	}else if(d<0){
		return -1;
	}else if(d>0){
		return 1;
	}

	return 0;
}

int roots(double a, double b, double c, double *r1, double *r2){
	if(compare(a,0) == 0){
		if(compare(b,0) == 0) return 0;
		//printf("\neqn %fXX + %fX + %f = 0", a, b, c);
		*r1 = -c/b;
		return 1;
	}
	double d = b*b - 4*a*c;
	int x = compare(d,0);
	if(x<0) return 0;
	if(x == 0){
		a *= 2;
		b /= a;
		*r1 = *r2 = -b;
		return 1;
	}
	d = sqrt(d);
	a *= 2;
	*r1 = -(b + d)/a;
	*r2 = -(b - d)/a;
	return 2;
}

class Vector;

template <int N, int M>
class Matrix;

#include "matrix.cpp"
#include "vector.cpp"
#include "ray.cpp"


template <int N, int M>
Matrix<N,M>::Matrix(Vector v, int i, int j){
	int n = N;
	int m = M;
	if(n >= 3+i){
		if(m >= 3+j){
			val[i+0][j+0] = v.x;
			val[i+1][j+1] = v.y;
			val[i+2][j+2] = v.z;
		}else{
			val[i+0][0] = v.x;
			val[i+1][0] = v.y;
			val[i+2][0] = v.z;
			if(n >= 4+i){
				val[i+3][0] = 1;
			}
		}
	}else{
		if(m >= 3+j){
			val[0][j+0] = v.x;
			val[1][j+1] = v.y;
			val[2][j+2] = v.z;
			if(m >= 4+j) val[0][j+3] = 1;
		}
	}
}

template <int N, int M>
Matrix<N,M>::Matrix(Vector v){
	val[0][0] = v.x;
	val[1][0] = v.y;
	val[2][0] = v.z;
	if(N == 4) val[3][0] = 1;
}


Vector Vector::transform(Matrix<3,3> M){
	Matrix <3,1> r = (*this);
	
	r = M*r;

	return r;
}

Vector Vector::operator*(Matrix<3,3> M){
	return transform(M);
}

Vector Vector::transform(Matrix<4,4> M){
	Matrix <4,1> r = (*this);
	//r.print();
	r = M*r;
	return r;
}

Vector Vector::operator*(Matrix<4,4> M){
	return transform(M);
}

Vector Vector::cross(Vector v){
	Matrix <3,3> m = Matrix<3,3>::cross(v.x,v.y,v.z);

	Matrix <3,1> n = (*this);
	
	return (*this)*m;
}

Vector Vector::operator*(Vector v){
	return cross(v);
}


#endif