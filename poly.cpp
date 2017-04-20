#ifndef _POLYGON_SIMPLE_INCLUDED_
#define _POLYGON_SIMPLE_INCLUDED_

class Polygon;

typedef Color (*ColorFn)(Polygon *poly, Vector v, void *arg);

Color defaultColorFn(Polygon *poly, Vector v, void *arg);

class Polygon{
public:
	Point **p;
	int c;
	Vector n;
	Vector lb, ub;
	double rhs;
	ColorFn colorFn;
	void *colorArg;
	Vector ka, kd, ks;
	Polygon(){
		p = NULL;
		c = 0;
		ka = kd = ks = 1.0;
	}
	Polygon(Point **_p, int _c){
		p = _p;
		c = _c;
		n = ((p[1]->v - p[0]->v)*(p[2]->v - p[1]->v)).unit();
		rhs = (n^(p[0]->v));
		lb = ub = p[0]->v;
		for(int i=1;i<c;i++){
			if(p[i]->v.x > ub.x){
				ub.x = p[i]->v.x;
			}else if(p[i]->v.x < lb.x){
				lb.x = p[i]->v.x;
			}
			if(p[i]->v.y > ub.y){
				ub.y = p[i]->v.y;
			}else if(p[i]->v.y < lb.y){
				lb.y = p[i]->v.y;
			}
			if(p[i]->v.z > ub.z){
				ub.z = p[i]->v.z;
			}else if(p[i]->v.z < lb.z){
				lb.z = p[i]->v.z;
			}
		}
		colorFn = &defaultColorFn;
		colorArg = (void*)100;
		ka = kd = ks = 1.0;
	}
	Point intersect(Ray r){
		Point ret;
		ret.color.a = 100;
		double den = ((r.d)^(n));
		if(den == 0){
			ret.color.a = -1;
		}else{
			den = (rhs - ((r.v)^(n)))/den;
			if(compare(den, 0) == 1){
				ret.v = ((r.v) + ((r.d)*den));
				
				int on = 1;
				for(int i=0; i<c; i++){
					if( (((p[(i+1)%c]->v - p[i]->v)*(ret.v - p[i]->v))^n) < 0 )
						on = 0;
				}
				if(on) ret.color = (*colorFn)(this, ret.v, colorArg);
				else ret.color.a = -1;
				//ret.print();
				
			}else{
				ret.color.a = -1;
			}
		}
		return ret;
	}

	void print(){
		for(int i=0;i<c;i++){
			p[i]->print();
		}
	}


};

Color defaultColorFn(Polygon *poly, Vector v, void *arg){
	long int alp = (long int)arg;
	Color ret;
	ret = poly->p[0]->color;
	double dt = (poly->p[0]->v - v).mod2();
	if(dt == 0) return ret;
	dt = 1/dt;
	double dt2;
	double dt3;
	double tmp;

	int allAS = 1;
	for(int i=1; i<poly->c; i++){
		dt3 = (poly->p[i]->v - v).mod2();
		if(dt3 == 0) return poly->p[i]->color;
		dt3 = 1/dt3;
		dt2 = dt + dt3;

		tmp = ((double) ret.r)*dt;
		tmp += ((double) poly->p[i]->color.r)*dt3;
		tmp /= dt2;
		ret.r = tmp;

		tmp = ((double) ret.g)*dt;
		tmp += ((double) poly->p[i]->color.g)*dt3;
		tmp /= dt2;
		ret.g = tmp;

		tmp = ((double) ret.b)*dt;
		tmp += ((double) poly->p[i]->color.b)*dt3;
		tmp /= dt2;
		ret.b = tmp;

		tmp = ((double) ret.a)*dt;
		tmp += ((double) poly->p[i]->color.a)*dt3;
		tmp /= dt2;
		ret.a = tmp;

		if(poly->p[i]->color.a != poly->p[i-1]->color.a) allAS = 0;

		dt = dt2;
	}
	if(allAS) ret.a = poly->p[0]->color.a;
	int aa = ret.a;
	aa *= (int)alp;
	aa /= 100;
	ret.a = aa;
	//if(ret.a != 100) cout<<(int)ret.a<<" "<<aa<<" "<<alp<<"\n";
	return ret;
}

Color firstColorFn(Polygon *poly, Vector v, void *arg){
	return poly->p[0]->color;
}

Color solidColorFn(Polygon *poly, Vector v, void *arg){
	Color* ret = (Color*) arg;
	return *ret;
}

#endif