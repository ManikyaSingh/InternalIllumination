#ifndef _COLOR_SIMPLE_INCLUDED_
#define _COLOR_SIMPLE_INCLUDED_



class Color{
public:
	unsigned char r;
	unsigned char g;
	unsigned char b;
	char a = -1;
	Color(){

	}
	Color(unsigned char R, unsigned char G, unsigned char B, char A){
		r = R;
		g = G;
		b = B;
		a = A;
	}
	Color(const Color& cc){
		r = cc.r;
		g = cc.g;
		b = cc.b;
		a = cc.a;
	}

	Color operator*(double d){
		double tr, tg, tb;
		tr = d*r;
		tg = d*g;
		tb = d*b;
		unsigned char R,G,B;

		if(tr > 255) R = 255;
		else R = tr;

		if(tg > 255) G = 255;
		else G = tg;

		if(tb > 255) B = 255;
		else B = tb;

		return Color(R, G, B, a);
	}
};

#endif