#ifndef _SMatrix_HPP_
#define _SMatrix_HPP_

#include<vector>
//#include<ostream>

template <typename T> class SMatrix
{
public:
	/// constructeurs/destructeur

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

	/// @brief accesseurs
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

	/// @brief dimensions
	inline size_t rows() const { return _iL; }
	inline size_t cols() const { return _iC; }

	/// opérations sur la classe
	size_t resize(const size_t rows, const size_t cols){
		this.rows = rows;
		this.cols = cols;
		_v.resize(rows, cols);
	}
	
	/// @brief operateur d'assignation
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
	
	/// @brief extrait la ligne l
	SMatrix row(const size_t l){
		return (n < _iC) ? submatrix(l, 0, l, _iC - 1) : NULL;
	}

	/// @brief extrait la colonne c
	SMatrix column(const size_t c){
		return (c < _iC) ? submatrix(0, c, _iL - 1, c) : NULL;
	}
	
	void swap_rows(const size_t l1, const size_t l2){
		// Todo
	}
	
	/// @brief remplit la table avec le scalaire s
	inline size_t fill(const T s) {
		size_t nbelem(_iL * _iC);
		_v.assign(nbelem, s);
		return nbelem;
	}
	
	// !! ATTENTION !!
	// Les operations suivantes modifient par défaut la matrice elle-même, ceci afin
	// d'éviter d'allouer de nombreuses matrices sur la pile
	// pour les calculs intermédiaires.
	// Par ex: 3+M*N alloue 2 matrices sur la pile
	// Si on ne veut pas modifier la matrice elle-même, il faut au préalable
	// appeler set_in_place(false). Les matrices générées pour les calculs intermédiaires 
	// seront désallouées à la sortie du scope.
	// Par ex, si set_in_place(false)
	// 3 + M.tr() alloue 2 matrices sur la pile, une pour le calcul M.tr(), et une pour 3 + (M.tr())
	// Si set_in_place(true)
	// 3 + M.tr() n'alloue aucune matrice sur la pile, mais M est modifiée deux fois
	
	SMatrix submatrix(SMatrix &sub, const i1, const size_t j1, const size_t i2, const size_t j2){
		if (in_place){
			in_place_submatrix(i1, j1, i2, j2);
		}
		else {
			sub.in_place_submatrix(i1, j1, i2, j2);
			return sub;		// take advantage of gcc return value optimization		
		}
	}

	/// @brief transposee
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

	
	/// opérations avec un scalaire
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


	/// @brief applique la fonction f(arg) aux éléments de la table     
	template<typename ret>
	void apply(ret (*f)(T)) {
		typename vector<T>::iterator it = _v.begin();
		while(it != _v.end()) {
			*it = (*f)(*it);
			++it;
		}
	}
	
	/// @brief applique la fonction f(arg1, arg2) aux éléments de la table
	/// arg2 doit être du type !!! exact !!! demandé par la fonction f, aucune conversion implicite n'étant possible
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
	size_t _iL, _iC;	    // lignes, colonnes
	std::vector<T> _v;      // notre structure d'accueil
	bool in_place = true;	// Si true, la matrice sur laquelle s'applique l'operation est modifiee.
	                        // Sinon, l'operation alloue une nouvelle matrice et c'est elle qui est retournee.
							
	
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