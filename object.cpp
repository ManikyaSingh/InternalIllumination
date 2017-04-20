#ifndef _OBJECT_SIMPLE_INCLUDED_
#define _OBJECT_SIMPLE_INCLUDED_

int max_ray_trace_count = 1;


int _OBJ_LIGHTS_OFF = 0;

double _OBJ_CMAX = 255.0;
double _OBJ_DMAX = 3500.0;
double _OBJ_LRADIUS = 50.0;

Color red = Color(200,50,90,100);
Color green = Color(150,210,57,100);
Color blue = Color(132,193,224,100);
Color black = Color(0,0,0,100);
Color white = Color(194,163,112,100);
Color transparent = Color(255,255,255,20);

class Light{
public:
	Point source;
	Vector dir;
	double radius;
	double area;
	double power;
	Vector coeff;
	int n;
	Light(){
		source = Point(Vector(0,0,0), Color(255,255,255,100));
		dir = 0.0;
		radius = area = 0.0;
		power = 1.0;
		n = 16;
		// double c = (((_OBJ_CMAX+1)*2*_OBJ_LRADIUS*_OBJ_LRADIUS) - (_OBJ_DMAX*_OBJ_DMAX) - (_OBJ_LRADIUS*_OBJ_DMAX));
		// c /= (2*_OBJ_CMAX+_OBJ_LRADIUS-_OBJ_DMAX);
		// double k = (2*_OBJ_LRADIUS*(_OBJ_LRADIUS - c));
		// coeff = Vector(1.0, _OBJ_LRADIUS - c, -_OBJ_LRADIUS*_OBJ_CMAX*_OBJ_CMAX);
		//power *= k;
		coeff = Vector(1.0/2500000.0,0.0,1.0);
	}
	//for now, only point light source of a particular color is applicable
	Color shade(Point p, Vector nrm, Vector vp, Vector kd, Vector ks, double mu){
		Color ret = Color(0,0,0,p.color.a);
		if(p.color.a <= 0) return ret;
		nrm = nrm.unit();
		Vector l = (source.v - p.v).unit();
		double dot = (nrm^l);

		Vector ref = (-l).reflect(nrm);
		ref = ref.unit();
		vp = vp.unit();
		double dot2 = (ref^vp);

		if(dot2 < 0) dot2 = 0;
		
		dot2 = pow(dot2, n);
		

		double dist2 = (source.v - p.v).mod();
		double dist = sqrt(dist2);

		double deno = (coeff.x*dist2) + (coeff.y*dist) + coeff.z;
		




		if(ret.a >= 100){
			if(dot < 0) dot = 0;
		}else{
			int tir = 0;
			ref = ((-l).refract(nrm, mu, &tir));
			if(tir == 1){
				ret.a = 0;
				//cout<<"tir-ed\n";
				return ret;
			}
			double tdot = (nrm^ref);
			if(tdot < 0) tdot = dot;
			//double dotdir = (nrm^vp);
			double tmpdot2 = (ref^vp);
			if(tmpdot2 < 0){
				tmpdot2 = 0;
				// if(dotdir < 0){
				// 	dot *= (ret.a);
				// }else{
				// 	dot *= (100-ret.a);
				// }
			}	//else{
			// 	if(dotdir < 0){
			// 		dot *= (100-ret.a);
			// 	}else{
			// 		dot *= (ret.a);
			// 	}
			// }

			tmpdot2 = pow(dot2, n);

			double tmpal = ret.a;
			tmpal /= 100;

			tmpdot2*=(1-tmpal);

			dot2 *= tmpal;

			dot2 += tmpdot2;
			
			//dot /= 100;
		}

		

		double tmp, tmp2;

		tmp2 = ( ((kd.x)*dot) + ((ks.x)*dot2) );
		tmp = p.color.r;
		tmp *= source.color.r;
		tmp *= tmp2;
		tmp /= 255;
		tmp *= power;
		tmp /= deno;

		if(tmp < 255) ret.r = tmp;
		else ret.r = 255;

		tmp2 = ( ((kd.y)*dot) + ((ks.y)*dot2) );
		tmp = p.color.g;
		tmp *= source.color.g;
		tmp *= tmp2;
		tmp /= 255;
		tmp *= power;
		tmp /= deno;

		if(tmp < 255) ret.g = tmp;
		else ret.g = 255;


		tmp2 = ( ((kd.z)*dot) + ((ks.z)*dot2) );
		tmp = p.color.b;
		tmp *= source.color.b;
		tmp *= tmp2;
		tmp /= 255;
		tmp *= power;
		tmp /= deno;

		if(tmp < 255) ret.b = tmp;
		else ret.b = 255;



		return ret;
	}

};



Color shade(Light *l, int lc, Point p, Vector nrm, Vector vp, Vector kd, Vector ks, Color ambientColor, Vector ka, double mu){
	Color ret = Color(0,0,0,p.color.a);
	if(p.color.a <= 0) return ret;
	Color tmp;
	double t;
	for(int i=0;i<lc;i++){
		tmp = l[i].shade(p, nrm, vp, kd, ks, mu);

		t = tmp.r;
		t += ret.r;
		ret.r = t;

		t = tmp.g;
		t += ret.g;
		ret.g = t;

		t = tmp.b;
		t += ret.b;
		ret.b = t;
	}

	tmp = ambientColor;

	t = tmp.r;
	t *= ka.x;
	tmp.r = t;

	t = tmp.g;
	t *= ka.y;
	tmp.g = t;

	t = tmp.b;
	t *= ka.z;
	tmp.b = t;


	t = tmp.r;
	t += ret.r;
	if(t <= 255 && t >= 0) ret.r = t;else ret.r = 255;

	t = tmp.g;
	t += ret.g;
	if(t <= 255 && t >= 0) ret.g = t;else ret.g = 255;

	t = tmp.b;
	t += ret.b;
	if(t <= 255 && t >= 0) ret.b = t;else ret.b = 255;

	//cout<<(int)ret.r<<" "<<(int)ret.g<<" "<<(int)ret.b<<endl;

	return ret;
}

class Object{
public:
	Object *obj;
	Polygon *poly;
	int oc, pc;
	double mu;
	Color ambientColor;
	Object(){
		obj = NULL;
		oc = 0;
		poly = NULL;
		pc = 0;
		mu = 1;
		ambientColor = Color(20,20,20,100);
	}

	Point intersect(Ray r, Polygon *normal, void *param){
		//long int c = (long int)param;
		int i;
		Point p, min;
		Ray tmp;
		min.color.a = -1;
		int frm = 0;
		int ind = 0;

		for(i=0;i<pc;i++){
			p = poly[i].intersect(r);
			//p.print();
			if(p.color.a == -1){

			}else{
				if(min.color.a == -1){
					if(compare(((p.v - r.v)^(r.d)),0) == 1){
						min = p;
						frm = 0;
						ind = i;
						if(normal != NULL) *normal = poly[i];
					}
				}else{
					if(compare(((min.v - p.v)^(r.d)),0) == 1){
						min = p;
						frm = 0;
						ind = i;
						if(normal != NULL) *normal = poly[i];
					}
				}
			}
		}
		Polygon tmpNormal;
		for(i=0;i<oc;i++){
			p = obj[i].intersect(r, &tmpNormal, param);
			if(p.color.a == -1){

			}else{
				if(min.color.a == -1){
					if(compare(((p.v - r.v)^(r.d)),0) == 1){
						min = p;
						frm = 2;
						ind = i;
						if(normal != NULL) *normal = tmpNormal;
					}
				}else{
					if(compare(((min.v - p.v)^(r.d)),0) == 1){
						min = p;
						frm = 2;
						ind = i;
						if(normal != NULL) *normal = tmpNormal;
					}
				}
			}
		}

		return min;

	}

	Color** project(Light *l, int lc, int w, int h, Vector vp){
		Color **ret = new Color*[2*h+1];
		int i,j;
		Ray r;
		Point tmp, tmp2;
		Polygon nrm;
		int ct, ct2, ct3;
		double prog = 0;
		double tot = (2*h+1)*(2*w+1);
		double x;
		int f = 0;
		int p = 0;
		Color sc;
		int tir;
		for(j=-h;j<=h;j++){
			ret[j+h] = new Color[2*w+1];
			for(i=-w;i<=w;i++){
				prog++;
				x = prog/tot;
				x *= 100;
				f = x;
				if(f != p){
					cout<<f<<"%\n";
					p = f;
				}
				int cc = 0;

				r = Ray(Vector(vp.x + i, vp.y + j, 0), Vector(i , j, -vp.z));

				tmp = intersect(r, &nrm, NULL);

				// specular color is independent of alpha
				// sc = tmp.color;
				// sc.a = 99;
				// sc = shade(l, lc, Point(tmp.v, sc), nrm.n, -(r.d) , Vector(0.0), nrm.ks, ambientColor, Vector(0.0), mu);
				// if(sc.a == 99 && (sc.r != 0 || sc.g != 0 || sc.b != 0)){
				// 	cout<<(int)sc.r<<" "<<(int)sc.g<<" "<<(int)sc.b<<" "<<(int)sc.a<<endl;
				// }

				sc = tmp.color;
				sc.a = 99;
				sc = shade(l, lc, Point(tmp.v, sc), nrm.n, -(r.d) , Vector(0.0), nrm.ks, ambientColor, Vector(0.0), mu);

				//compute diffused and ambient color based on lights
				if(!_OBJ_LIGHTS_OFF) tmp.color = shade(l, lc, tmp, nrm.n, -(r.d) , nrm.kd, 0.0 , ambientColor, nrm.ka, mu);

				

				while(tmp.color.a < 100 && tmp.color.a >= 0 && cc < max_ray_trace_count){
					r = Ray(tmp.v, r.d.refract(nrm.n, mu, &tir) );

					r.v = tmp.v;
					tmp2 = intersect(r, &nrm, NULL);

					//compute tmp2.color based on lights
					if(!_OBJ_LIGHTS_OFF) tmp2.color = shade(l, lc, tmp2, nrm.n, -(r.d) , nrm.kd, nrm.ks, ambientColor, nrm.ka, mu);

					tmp.v = tmp2.v;




					cc++;

					//tmp2.print();

					ct = tmp.color.a;
					ct2 = tmp2.color.a;
					ct2 *= (100 - ct);
					ct2 /= 100;
					tmp2.color.a = ct2;



					ct3 = tmp.color.a + tmp2.color.a;

					if(ct3 != 0){

						ct = tmp.color.r;
						ct *= tmp.color.a;
						ct2 = tmp2.color.r;
						ct2 *= tmp2.color.a;
						tmp.color.r = (ct+ct2)/(ct3);


						ct = tmp.color.g;
						ct *= tmp.color.a;
						ct2 = tmp2.color.g;
						ct2 *= tmp2.color.a;
						tmp.color.g = (ct+ct2)/(ct3);


						ct = tmp.color.b;
						ct *= tmp.color.a;
						ct2 = tmp2.color.b;
						ct2 *= tmp2.color.a;
						tmp.color.b = (ct+ct2)/(ct3);

					}

					


					tmp.color.a += tmp2.color.a;

					
				}

				if(cc == max_ray_trace_count){
					tmp.color.a = 100;
				}

				if(sc.a > 0 && !_OBJ_LIGHTS_OFF){
					ct = tmp.color.r;
					ct += sc.r;
					if(ct <= 255) tmp.color.r = ct;
					else tmp.color.r = 255;


					ct = tmp.color.g;
					ct += sc.g;
					if(ct <= 255) tmp.color.g = ct;
					else tmp.color.g = 255;

					ct = tmp.color.b;
					ct += sc.b;
					if(ct <= 255) tmp.color.b = ct;
					else tmp.color.b = 255;
				}
				

				//if(tmp.color.a != -1) tmp.print();
				
				ret[j+h][i+w] = tmp.color;
			}
			
			
		}

		return ret;
	}

};



void transform(Point *p, int c, Matrix<3,3> M){
	for(int i=0;i<c;i++){
		p[i].v = p[i].v.transform(M);
	}
}

void transform(Point *p, int c, Matrix<4,4> M){
	for(int i=0;i<c;i++){
		p[i].v = p[i].v.transform(M);
	}
}

Object readFile(){
	char kkk[100];
	cout<<"\nEnter filename of .off file : ";
	cin>>kkk;
	FILE* file = fopen (kkk, "r");
	char com[20];
	int v, f, tmp, tmp2;
	cout<<"\nIs the file in 'Color OFF' format? (0 = No , 1 = Yes) ";
	int kkkk;
	cin>>kkkk;

	fscanf(file,"%s", com);

	fscanf(file, "%d", &v);
	fscanf(file, "%d", &f);
	fscanf(file, "%d", &tmp);

	Point *points = new Point[v];

	float x, y, z;
	float minx, miny, minz; 
	float maxx, maxy, maxz;
	int r=255,g=255,b=255,a=100;

	fscanf(file, "%f", &x);
	minx = maxx = x;
	fscanf(file, "%f", &y);
	miny = maxy = y;
	fscanf(file, "%f", &z);
	minz = maxz = z;

	
	if(kkkk){
		fscanf(file, "%d", &r);
		fscanf(file, "%d", &g);
		fscanf(file, "%d", &b);
		fscanf(file, "%d", &a);
		a *= 100;
		a /= 255;
	}
	points[0] = Point(Vector(x, y, z), Color(r,g,b,a));
	for(int i=1;i<v;i++){
		fscanf(file, "%f", &x);
		fscanf(file, "%f", &y);
		fscanf(file, "%f", &z);
		if(x > maxx) maxx = x;
		if(x < minx) minx = x;
	
		if(y > maxy) maxy = y;
		if(y < miny) miny = y;

		if(z > maxz) maxz = z;
		if(z < minz) minz = z;
		if(kkkk){
			fscanf(file, "%d", &r);
			fscanf(file, "%d", &g);
			fscanf(file, "%d", &b);
			fscanf(file, "%d", &a);
			a *= 100;
			a /= 255;
		}
		points[i] = Point(Vector(x, y, z), Color(r,g,b,a));

	}
	maxx -= minx;
	maxy -= miny;
	maxz -= minz;
	if(maxx < 0) maxx = -maxx;
	if(maxy < 0) maxy = -maxy;
	if(maxz < 0) maxz = -maxz;

	if(maxy > maxx) maxx = maxy;
	if(maxz > maxx) maxx = maxz;

	cout<<"\nInput Scale factor: ";
	float sf;
	cin>>sf;

	float xrot, yrot, zrot;

	cout<<"\nX rotation in degree: ";
	cin>>xrot;

	cout<<"\nY rotation in degree: ";
	cin>>yrot;

	cout<<"\nZ rotation in degree: ";
	cin>>zrot;

	transform(points, v, Matrix<3,3>((400.0*sf)/maxx));
	transform(points, v, Matrix<3,3>::rotX(xrot));
	transform(points, v, Matrix<3,3>::rotY(yrot));
	transform(points, v, Matrix<3,3>::rotZ(zrot));
	transform(points, v, Matrix<4,4>::translate(0,0,-800));


	Point ***p = new Point**[f];
	Polygon *po = new Polygon[f];

	for(int i=0; i<f; i++){

		fscanf(file, "%d", &tmp);
		p[i] = new Point*[tmp];
		for(int j=0;j<tmp;j++){
			fscanf(file, "%d", &tmp2);
			p[i][j] = &points[tmp2];
		}

		po[i] = Polygon(p[i], tmp);
		po[i].colorFn = solidColorFn;
		po[i].colorArg = (void*) &transparent;
	}

	Object o = Object();
	o.poly = &po[0];
	o.pc = f;

	return o;
	
}

#endif