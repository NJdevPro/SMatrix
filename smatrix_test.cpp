#include <cmath>
#include <iostream>
#include <iomanip>
#include "SMatrix.h"

using namespace std;

typedef SMatrix<double> dmat;

ostream& operator<<(ostream& os, dmat& mat) {
	int L = mat.rows(), C = mat.cols();
	for(int l = 0; l < L; ++l) {
		for(int c = 0; c < C; ++c)
			os  << mat(l, c) << setw(9);
        os << '\n';
	}
    os << endl;
    return os;
}

double f1(double x){ return sqrt(fabs(x)); }
double f2(double x, int i){ return (x * i); }

int main(int argc, char* argv[])
{
    cout << fixed << setprecision(5);
    cout << "Original SMatrix\n" << endl;
	dmat M(7, 7);
	dmat N(7, 7);
	cout << "fill M" << endl;//M;
	M.fill(-1.);
	cout << "Random read/write accesses" << endl;
	for(short i = 1; i < M.rows(); i++) M(i, 0) = i;
	for(short j = 1; j < M.cols(); j++) M(0, j) = j;
	for(short j = 0; j < M.cols(); j++) M(j, j) = 1.;
	cout << "Transpose " << endl;
	N = M.tr();
	cout << "M =\n" << M;
	cout << "N =\n" << N;
	M.apply(&f1);
	cout << "sqrt(fabs(elem)) = \n" << M;
	M.apply(&f2, 2);
	cout << "M x 2 = \n" << M;
	return 0;
}