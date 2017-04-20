#ifndef _MATRIX_SIMPLE_INCLUDED_
#define _MATRIX_SIMPLE_INCLUDED_


template <int N, int M>
class Matrix{
public:
	double val[N][M];
	
	Matrix(){
		
	}
	Matrix(double **a){
		Matrix();
		int i,j;
		for(i=0;i<N;i++){
			for(j=0;j<M;j++){
				val[i][j] = a[i][j];
			}
		}
	}
	Matrix(double a[N][M]){
		Matrix();
		int i,j;
		for(i=0;i<N;i++){
			for(j=0;j<M;j++){
				val[i][j] = a[i][j];
			}
		}
	}
	Matrix(double **a, int ni, int nj){
		Matrix();
		int i,j;
		int li=0;
		int lj=0;
		for(i=0;i<=N;i++){
			if(i != ni){
				
				for(j=0;j<=M;j++){
					if(j != nj){
						val[li][lj] = a[i][j];
						lj++;
					}	
				}
				li++;
			}
		}
	}
	Matrix(double a[N+1][M+1], int ni, int nj){
		Matrix();
		int i,j;
		int li=0;
		int lj=0;
		for(i=0;i<=N;i++){
			if(i != ni){
				lj = 0;
				for(j=0;j<=M;j++){
					if(j != nj){
						val[li][lj] = a[i][j];
						//cout<<li<<" "<<lj<<" "<<a[i][j]<<"\n";
						lj++;
					}	
				}
				li++;
			}
		}
	}
	Matrix(double d){
		for(int i=0;i<N;i++){
			for(int j=0;j<M;j++){
				if(i == j) val[i][j] = d;
				else val[i][j] = 0;
			}
		}
	}
	
	Matrix (const Matrix<N,M> &obj) {
		for(int i=0;i<N;i++){
			for(int j=0;j<M;j++){
				val[i][j] = obj.val[i][j];
			}
		}
	}
	Matrix(Vector v);
	Matrix(Vector v, int i, int j);

	Matrix<N-1,M-1> slice(int i, int j){
		Matrix <N-1,M-1>r = Matrix<N-1,M-1>(val,i,j);
		return r;
	}
	int set(int i, int j, double v){
		if(i<N && j<M && i>0 && j>0){
			val[i][j] = v;
			return 1;
		}
		return 0;
	}
	Matrix<N,M> operator+(Matrix<N,M> b){
		Matrix<N,M> r;
		int i,j;
		for(i=0;i<N;i++){
			for(j=0;j<N;j++){
				r.val[i][j] = val[i][j]+b.val[i][j];
			}
		}
		return r;
	}
	Matrix<N,M> operator-(Matrix<N,M> b){
		Matrix<N,M> r;
		int i,j;
		for(i=0;i<N;i++){
			for(j=0;j<N;j++){
				r.val[i][j] = val[i][j]-b.val[i][j];
			}
		}
		return r;
	}
	Matrix<N,M> operator*(double b){
		Matrix<N,M> r;
		int i,j;
		for(i=0;i<N;i++){
			for(j=0;j<N;j++){
				r.val[i][j] = val[i][j]*b;
			}
		}
		return r;
	}
	Matrix<N,M> operator/(double b){
		Matrix<N,M> r;
		int i,j;
		for(i=0;i<N;i++){
			for(j=0;j<N;j++){
				r.val[i][j] = val[i][j]/b;
			}
		}
		return r;
	}
	Matrix<N,M> operator-(){
		Matrix<N,M> r;
		int i,j;
		for(i=0;i<N;i++){
			for(j=0;j<N;j++){
				r.val[i][j] = -val[i][j];
			}
		}
		return r;
	}
	Matrix<N,M> operator+(){
		return *this;
	}
	double mod();
	Matrix<M,N> transpose(){
		Matrix<M,N> r;
		int i,j;
		for(i=0;i<N;i++){
			for(j=0;j<N;j++){
				r.val[j][i] = val[i][j];
			}
		}
		return r;
	}
	Matrix<M,N> operator~(){
		return (*this).transpose();
	}
	void print(){
		int i,j;
		
		for(i=0;i<N;i++){
			cout<<endl;
			for(j=0;j<M;j++){
				cout<<val[i][j]<<" ";
			}
		}
		cout<<endl;
	}

	static Matrix cross(double x, double y, double z){
		Matrix <3,3> m = 0.0;
		m.val[0][1] = z;
		m.val[0][2] = -y;
		m.val[1][0] = -z;
		m.val[1][2] = x;
		m.val[2][0] = y;
		m.val[2][1] = -x;
		return m;
	}

	static Matrix rotX(double a){
		a = M_PI*a;
		a /= 180;
		double ct = cos(a);
		double st = sqrt(1 - (ct*ct));
		Matrix <3,3> m = 1.0;
		m.val[1][1] = ct;
		m.val[1][2] = -st;
		m.val[2][1] = st;
		m.val[2][2] = ct;
		return m;
	}

	static Matrix rotXC(double a){
		double ct = a;
		double st = sqrt(1 - (ct*ct));
		Matrix <3,3> m = 1.0;
		m.val[1][1] = ct;
		m.val[1][2] = -st;
		m.val[2][1] = st;
		m.val[2][2] = ct;
		return m;
	}

	static Matrix rotY(double a){
		a = M_PI*a;
		a /= 180;
		double ct = cos(a);
		double st = sqrt(1 - (ct*ct));
		Matrix <3,3> m = 1.0;
		m.val[0][0] = ct;
		m.val[0][2] = st;
		m.val[2][0] = -st;
		m.val[2][2] = ct;
		return m;
	}
	static Matrix rotYC(double a){
		double ct = a;
		double st = sqrt(1 - (ct*ct));
		Matrix <3,3> m = 1.0;
		m.val[0][0] = ct;
		m.val[0][2] = st;
		m.val[2][0] = -st;
		m.val[2][2] = ct;
		return m;
	}

	static Matrix rotZ(double a){
		a = M_PI*a;
		a /= 180;
		double ct = cos(a);
		double st = sqrt(1 - (ct*ct));
		Matrix <3,3> m = 1.0;
		m.val[0][0] = ct;
		m.val[0][1] = -st;
		m.val[1][0] = st;
		m.val[1][1] = ct;
		return m;
	}
	static Matrix rotZC(double a){
		double ct = a;
		double st = sqrt(1 - (ct*ct));
		Matrix <3,3> m = 1.0;
		m.val[0][0] = ct;
		m.val[0][1] = -st;
		m.val[1][0] = st;
		m.val[1][1] = ct;
		return m;
	}

	static Matrix translate(double x, double y, double z){
		Matrix <4,4> m = 1.0;
		m.val[0][3] = x;
		m.val[1][3] = y;
		m.val[2][3] = z;
		return m;
	}

};

template <int N, int M, int L>
Matrix<N,L> operator*(Matrix<N,M> A, Matrix<M,L> B){
	Matrix <N,L> r;
	int n,m,l;
	for(n=0;n<N;n++){
		for(l=0;l<L;l++){
			r.val[n][l] = 0;
			for(m=0;m<M;m++){
				r.val[n][l] += A.val[n][m]*B.val[m][l];
			}
		}
	}
	return r;
}

template <int N, int M>
Matrix<N,M> operator*(double b, Matrix<N,M> a){
	Matrix<N,M> r;
	int i,j;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			r.val[i][j] = a.val[i][j]*b;
		}
	}
	return r;
}

template <int N, int M>
Matrix<N,M> operator/(double b, Matrix<N,M> a){
	Matrix<N,M> r;
	int i,j;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			r.val[i][j] = a.val[i][j]/b;
		}
	}
	return r;
}


template <int N, int M>
double Matrix<N,M>::mod(){
	double r = 0;
	for(int i=0;i<N;i++){
		Matrix<N-1,M-1> tmp = slice(0,i);

		
		if(i%2) r -= val[0][i]*tmp.mod();
		else r += val[0][i]*tmp.mod();
		
		
	}
	return r;
}

template <>
double Matrix<1,1>::mod(){

	return val[0][0];
}

#endif