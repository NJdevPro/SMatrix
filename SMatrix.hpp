#ifndef _SMatrix_HPP_
#define _SMatrix_HPP_

/**
 *
 * SMatrix : a simple matrix class for use in embedded environement
 *
 * Author: Nicolas Janin
 */
#include<vector>
//#include<ostream>

template <typename T> class SMatrix
{
public:
	SMatrix(const size_t l, const size_t c, const T val = 0): _iL(l), _iC(c) {
		_v.resize(l * c, val);
	}

	SMatrix(const size_t l, const size_t c, const T* tab): _iL(l), _iC(c) {
		_v.resize(l * c);
		for( size_t l = 0; l < _iL; l++ )
			for( size_t c = 0; c < _iC; c++ )
				_v[ l*_iC + c ] = tab[ l*_iC + c ];
	}

	SMatrix(const SMatrix &rhs): _iL(rhs.rows()), _iC(rhs.cols()) {
		_v.resize(_iL * _iC);
		for( size_t l = 0; l < _iL; l++ )
			for( size_t c = 0; c < _iC; c++ )
				_v[ l*_iC + c ] = rhs(l, c);
	}

	~SMatrix(){}

	void set_in_place(const bool b = true){
		in_place = b;
	}
	bool get_in_place(){
		return b;
	}
	inline T operator()(const size_t l, const size_t c) const {
		return _v[l * _iC + c];
	}
	inline T& operator()(const size_t l, const size_t c) {
		return _v[l * _iC + c];
	}
	inline T get(const size_t x, const size_t y) const {
		return _v[x * _iC + y];
	}
	inline void set(const size_t x, const size_t y, T val) {
		_v[xw * _iC + y] = val;
	}

	/// Matrix dimensions
	inline size_t rows() const { return _iL; }
	inline size_t cols() const { return _iC; }

	/// Class operations
	size_t resize(const size_t rows, const size_t cols){
		this.rows = rows;
		this.cols = cols;
		_v.resize(rows, cols);
	}
	
	T& operator=(const T& rhs) {
		if (&rhs != this)
		{
			if(rhs.rows() != _iL || rhs.cols() != _iC)
				resize(rhs.rows(), rhs.cols());				
			for(size_t l = 0; l < _iL; ++l)
				for(size_t c = 0; c < _iC; ++c)
					_v[l * _iC + c] = rhs(l, c);
		}
		return (*this);
	}
	
	/// Extract a row
	SMatrix row(const size_t l){
		return (n < _iC) ? submatrix(l, 0, l, _iC - 1) : NULL;
	}

	/// Extract a column
	SMatrix column(const size_t c){
		return (c < _iC) ? submatrix(0, c, _iL - 1, c) : NULL;
	}
	
	void swap_rows(const size_t l1, const size_t l2){
		// Todo
	}
	
	/// Fill with the scalar s
	inline size_t fill(const T s) {
		size_t nbelem(_iL * _iC);
		_v.assign(nbelem, s);
		return nbelem;
	}
	
	// !! IMPORTANT !!
	//
	// The following operations mutate the matrix itself, in order to avoid numerous
	// stack allocations for intermediate calculations.
	// For instance, 3 + M x N allocates 2 matrices on the stack
	// If one doesn't want to mutate the matrix itself, one must call set_in_place(false).
	// The matrices that have been created for intermediate computations will be destroyed
	// at the end of the scope.
	//
	// For instance, 
	// If set_in_place(false)
	// 3 + M.tr() allocates 2 matrices on the stack, one for the computation of M.tr() 
	// and one for 3 + M.tr()
	// But if set_in_place(true)
	// 3 + M.tr() does no stack allocation, but M is mutated twice.
	
	SMatrix submatrix(SMatrix &sub, const i1, const size_t j1, const size_t i2, const size_t j2){
		if (in_place){
			in_place_submatrix(i1, j1, i2, j2);
		}
		else {
			sub.in_place_submatrix(i1, j1, i2, j2);
			return sub;		// take advantage of gcc return value optimization		
		}
	}

	/// Transpose
	SMatrix& tr(bool in_place = true){
		if (in_place){
			size_t temp = _iC;
			_iC = _iL;
			_iL = temp;
			return *this;
		}
		else {
			SMatrix(_iC, _iL) tr;
			for (size_t l = 0; l < _iC; ++l)
				for (size_t c = 0; c < _iL; ++c)
					tr[c * _iL + l] = _v[l * _iC + c];
			return tr;
		}
	}

	
	/// Scalar operations
	SMatrix operator+(const T s){
		SMatrix(_iC, _iL) m;
		for (size_t ij = 0; ij < _iC * _iL; ++ij) m._v[ij] += s;
		return m;
	}
	
	SMatrix operator-(const T s){
		SMatrix(_iC, _iL) m;
		for (size_t ij = 0; ij < _iC * _iL; ++ij) m._v[ij] -= s;
		return m;
	}
	
	SMatrix operator*(const T s){
		SMatrix(_iC, _iL) m;
		for (size_t ij = 0; ij < _iC * _iL; ++ij) m._v[ij] *= s;
		return m;
	}

	SMatrix operator/(const T s){
		SMatrix(_iC, _iL) m;
		for (size_t ij = 0; ij < _iC * _iL; ++ij) m._v[ij] /= s;
		return m;
	}


	/// Apply the function f(arg) to all the matrix elements     
	template<typename ret>
	void apply(ret (*f)(T)) {
		typename vector<T>::iterator it = _v.begin();
		while(it != _v.end()) {
			*it = (*f)(*it);
			++it;
		}
	}
	
	/// Apply the function f(arg1, arg2) to all the matrix elements 
	/// arg2 must be of the *exact* type required by function f, no implicit conversion being possible
	template<typename ret, typename A>
	void apply(ret (*f)(T, A), A arg2) {
		typename vector<T>::iterator it = _v.begin();
		while(it != _v.end()) {
			*it = (*f)(*it, arg2);
			++it;
		}
	}
	
//friend ostream& operator<<(ostream& os, SMatrix<T>& mat) const {;}
private:
	size_t _iL, _iC;	    // rows, columns
	std::vector<T> _v;      // the matrix elements
	bool in_place = true;	// If true, some operations can mutate the original matrix
	                        // else, they will allocate a new matrix that will be returned.
							
	
	SMatrix(const size_t l, const size_t c): _iL(l), _iC(c) {
		_v.resize(l * c);
	}

	SMatrix in_place_submatrix(const i1, const size_t j1, const size_t i2, const size_t j2){
		if (i1 >= _iL || i2 >= _iL || j1 >= _iC || j2 >= _iC)
			return NULL;
		
		size_t cols = j2 - j1 + 1;		
		SMatrix sub(i2 - i1 + 1, cols);
		for(size_t l = i1, lsub = 0; l <= i2; ++l, ++lsub)
			for(size_t c = j1, csub = 0; c <= j2; ++c, ++csub)
				sub[lsub * cols + csub] = _v[l * cols + c];
		this = sub;
	}

};

#endif