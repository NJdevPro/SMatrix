#include <cmath>
#include <iostream>
#include <ostream>
#include "SMatrix.h"

using namespace std;

typedef SMatrix<double> dmat;

ostream& operator<<(ostream& os, dmat & mat) {
	int L = mat.rows(), C = mat.cols();
	for(int l = 0; l < L; ++l) {
		os << "[ ";
		for(int c = 0; c < C; ++c)
			os << mat(l, c) << '\t';
		os << "]" << endl;
	}
       return os;
}

double f1(double x){ return sqrt(fabs(x)); }
double f2(double x, int i){ return (x * i); }

int main(int argc, char* argv[])
{
	cout << "SMatrix d'origine\n" << endl;
	dmat M(7, 7);
	dmat N(7, 7);
	cout << "fill" << endl;//M;
	M.fill(-1.);
	cout << "Divers acces en lecture/ecriture" << endl;
	for(short i = 1; i < M.rows(); i++) M(i, 0) = i;
	for(short j = 1; j < M.cols(); j++) M(0, j) = j;
	for(short j = 0; j < M.cols(); j++) M(j, j) = 1.;
	cout << "Une transposee " << endl;
	N = M.tr();
	cout << "M =\n" << M;
	cout << "N =\n" << N;
	M.apply(&f1);
	cout << "sqrt(fabs(elem)) = \n" << M;
	M.apply(&f2, 2);
	cout << "M x 2 = \n" << M;
	return 0;
}