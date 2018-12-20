#pragma once
#include <vector>
#include <type_traits>
#include "fraction.h"

// ===== По умолчанию 2-мерна и состоит из рациональных дробей =====
template <unsigned dim = 2, class T = int>
class Matrix {
public:
	// ===== Конструкторы =====
	Matrix();
	Matrix(const T& num);
	Matrix(const std::vector<std::vector<T> >&);
	Matrix(const Matrix&);

	// ===== Перегрузки =====
	Matrix operator +(const Matrix&) const;
	Matrix operator -(const Matrix&) const;
	Matrix operator *(const Matrix&) const;
	Matrix operator *(const T&) const;
	Matrix operator /(const Matrix&) const; /* Требует Matrix<Frac> */

	Matrix& operator =(const Matrix&);
	Matrix& operator =(const T&);

	bool operator ==(const Matrix&) const;

	// ----- Определитель (требует Matrix<Frac>) -----
	T determinant() const;
	// ----- Обратная матрица (требует Matrix<Frac>) -----
	Matrix inverse() const;

	// ===== Перегрузка операторов потокового ввода-вывода =====
	template <unsigned dm, class U>
	friend std::ostream& operator<< (std::ostream&, const Matrix<dm, U>&);
	template <unsigned dm, class U>
	friend std::istream& operator>> (std::istream&, Matrix<dm, U>&);

	// ===== Обработка исключений =====
	class inversion_error : public std::logic_error {
	public:
		explicit inversion_error() : std::logic_error("Uninvertable matrix") { }
	};

	class null_dim : public std::runtime_error {
	public:
		explicit null_dim() : std::runtime_error("Null dimensioned matrix") { }
	};

	class invalid_type : public std::runtime_error {
	public:
		explicit invalid_type() : std::runtime_error(
		"Using of Matrix<Fraction> is necessary to calculate determinant"
		) { }
	};

	// ----- Проверка размерности -----
	void isDimNull() const;
	// ----- Проверка типа : определитель и обратная матрица -----
	void isTypeSuite() const;

private:
	// ===== Ячейки: вектор векторов =====
	std::vector<std::vector<T> > cells;
};




// ===== Конструкторы =====
template <unsigned dim, class T>
Matrix<dim, T>::Matrix() {
	this->isDimNull();
	this->cells.resize(dim);
	for (int i = 0; i < dim; ++i) {
		this->cells[i].resize(dim);
		for (int j = 0; j < dim; ++j)
			this->cells[i][j] = 0;
	}
}

template <unsigned dim, class T>
Matrix<dim, T>::Matrix(const T& num) {
	this->isDimNull();
	this->cells.resize(dim);
	for (int i = 0; i < dim; ++i) {
		this->cells[i].resize(dim);
		for (int j = 0; j < dim; ++j)
			this->cells[i][j] = num;
	}
}

template <unsigned dim, class T>
Matrix<dim, T>::Matrix(const std::vector<std::vector<T> >& vec2d) {
	this->isDimNull();
	this->cells = vec2d;
}

template <unsigned dim, class T>
Matrix<dim, T>::Matrix(const Matrix<dim, T>& basic) {
	this->cells = basic.cells;
}


// ===== Перегрузки операторов =====

// ----- + -----
template <unsigned dim, class T>
Matrix<dim, T> Matrix<dim, T>::operator +(const Matrix<dim, T>& other) const {
	Matrix<dim, T> result;
	for (int i = 0; i < dim; ++i)
		for (int j = 0; j < dim; ++j)
			result.cells[i][j] = this->cells[i][j] + other.cells[i][j];
	return result;
}

// ----- - -----
template <unsigned dim, class T>
Matrix<dim, T> Matrix<dim, T>::operator -(const Matrix<dim, T>& other) const {
	Matrix<dim, T> result;
	for (int i = 0; i < dim; ++i)
		for (int j = 0; j < dim; ++j)
			result.cells[i][j] = this->cells[i][j] - other.cells[i][j];
	return result;
}

// ----- * -----
template <unsigned dim, class T>
Matrix<dim, T> Matrix<dim, T>::operator *(const Matrix<dim, T>& other) const {
	Matrix<dim, T> result;
	for (int i = 0; i < dim; ++i)
		for (int j = 0; j < dim; ++j)
			for (int k = 0; k < dim; ++k)
				result.cells[i][j] = result.cells[i][j] + this->cells[i][k] * other.cells[k][j];
	return result;
}

template <unsigned dim, class T>
Matrix<dim, T> Matrix<dim, T>::operator *(const T& num) const {
	Matrix<dim, T> result;
	for (int i = 0; i < dim; ++i)
		for (int j = 0; j < dim; ++j)
			result.cells[i][j] = this->cells[i][j] * num;
	return result;
}

// ----- / -----
template <unsigned dim, class T>
Matrix<dim, T> Matrix<dim, T>::operator /(const Matrix<dim, T>& other) const {
	return *this * other.inverse();
}


// ----- = -----
template <unsigned dim, class T>
Matrix<dim, T>& Matrix<dim, T>::operator =(const Matrix<dim, T>& other) {
	if (&other != this)
		this->cells = other.cells;
	return *this;
}

template <unsigned dim, class T>
Matrix<dim, T>& Matrix<dim, T>::operator =(const T& num) {
	for (int i = 0; i < dim; ++i)
		for (int j = 0; j < dim; ++j)
			this->cells[i][j] = num;
	return *this;
}


// ===== Сравнительные перегрузки =====
template <unsigned dim, class T>
bool Matrix<dim, T>::operator ==(const Matrix<dim, T>& other) const {
	for (int i = 0; i < dim; ++i)
		for (int j = 0; j < dim; ++j)
			if ( !(other.cells[i][j] == this->cells[i][j]) )
				return false;
	return true;
}


// ----- Перегрузка вывода в поток -----
template <unsigned dim, class T>
std::ostream& operator <<(std::ostream& out, const Matrix<dim, T>& matr) {
	for (int i = 0; i < dim; ++i) {
		out << "\n( ";
		for (int j = 0; j < dim; ++j)
			out << matr.cells[i][j] << " ";
		out << ')';
	}
	out << '\n';
	return out;
}

// ----- Перегрузка вставки в поток -----
template <unsigned dim, class T>
std::istream& operator >>(std::istream& in, Matrix<dim, T>& matr) {
	for (int i = 0; i < dim; ++i)
		for (int j = 0; j < dim; ++j) {
			in >> matr.cells[i][j];
		}
	return in;
}


// ===== Определитель =====
template <unsigned dim, class T>
T Matrix<dim, T>::determinant() const {
	this->isTypeSuite();

	std::vector<std::vector<T> > matrix(dim);
	Matrix<dim, int> matr;
	for (int i = 0; i < dim; ++i) {
		matrix[i].resize(dim);
		for (int j = 0; j < dim; ++j)
			matrix[i][j] = this->cells[i][j];
	}

	T result = 1;
	for (int i = 0; i < dim - 1; ++i) {
		if (matrix[i][i] == 0)
			result = result * -1;
		int tmp = i;
		while (matrix[tmp][i] == 0 && tmp < dim)
			++tmp;
		if (tmp == dim) {
			result = 0;
			return result;
		}
		for (int j = 0; j < dim; ++j) {
			T swapper = matrix[i][j];
			matrix[i][j] = matrix[tmp][j];
			matrix[tmp][j] = swapper;
		}
		for (int k = i + 1; k < dim; ++k) {
			T factor = matrix[k][i] / matrix[i][i];
			for (int j = i; j < dim; ++j)
				matrix[k][j] = matrix[k][j] - matrix[i][j] * factor;
		}
		result = result * matrix[i][i];
	}
	result = result * matrix[dim - 1][dim - 1];
	return result;
}


// ===== Обращение матрицы =====
template <unsigned dim, typename T>
Matrix<dim, T> Matrix<dim, T>::inverse() const {
	this->isTypeSuite();

	// Нулевой определитель == необратимость матрицы
	if (this->determinant() == 0)
		throw inversion_error();

	std::vector<std::vector<T> > transit(dim);
	for (int i = 0; i < dim; ++i) {
		transit[i].resize(dim);
		for (int j = 0; j < dim; ++j)
			transit[i][j] = this->cells[i][j];
	}

	std::vector<std::vector<T> > res(dim);
	for (int i = 0; i < dim; ++i)	{
		res[i].resize(dim);
		for (int j = 0; j < dim; ++j)	{
			if (i == j)
				res[i][j] = 1;
			else
				res[i][j] = 0;
		}
	}

	for (int i = 0; i < dim - 1; ++i) {
		int tmp = i;
		while (this->cells[tmp][i] == 0 && tmp < dim)
			++tmp;
		for (int j = 0; j < dim; ++j) {
			T tmpMean = transit[i][j];
			transit[i][j] = transit[tmp][j];
			transit[tmp][j] = tmpMean;
			tmpMean = res[i][j];
			res[i][j] = res[tmp][j];
			res[tmp][j] = tmpMean;
		}
		T tmpKoef = transit[i][i];
		for (int j = 0; j < dim; ++j)	{
			transit[i][j] = transit[i][j] / tmpKoef;
			res[i][j] = res[i][j] / tmpKoef;
		}
		for (int k = i + 1; k < dim; ++k)	{
			T koef = transit[k][i];
			for (int j = 0; j < dim; ++j) {
				transit[k][j] = transit[k][j] - transit[i][j] * koef;
				res[k][j] = res[k][j] - res[i][j] * koef;
			}
		}
	}
	T tmpKoef = transit[dim - 1][dim - 1];
	for (int j = 0; j < dim; ++j)	{
		transit[dim - 1][j] = transit[dim - 1][j] / tmpKoef;
		res[dim - 1][j] = res[dim - 1][j] / tmpKoef;
	}
	for (int i = dim - 1; i > 0; --i) {
		for (int k = i - 1; k >= 0; --k) {
			T koef = transit[k][i];
			for (int j = 0; j < dim; ++j)
				res[k][j] = res[k][j] - res[i][j] * koef;
		}
	}
	Matrix<dim, T> inverted;
	for (int i = 0; i < dim; ++i)
		for (int j = 0; j < dim; ++j)
			inverted.cells[i][j] = res[i][j];
	return res;
}

// ===== Проверка размерности =====
template <unsigned dim, class T>
void Matrix<dim, T>::isDimNull() const {
	if (0 == dim)
		throw null_dim();
}

// ===== Проверка типа : определитель и обратная матрица =====
template <unsigned dim, class T>
void Matrix<dim, T>::isTypeSuite() const {
	if ( !(std::is_same<T, Fraction<int>>::value	||
				 std::is_same<T, Fraction<unsigned char>>::value) )
		throw invalid_type();
}
