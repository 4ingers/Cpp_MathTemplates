#include <iostream>
#include "fraction.h"
#include "matrix.h"
#include "polynom.h"

typedef Fraction<int> frac;
typedef Fraction<unsigned char> shortfrac;
using namespace std;

void testFraction();
void testMatrixInt();
void testMatrixFracInt();
void testMatrixFrac();
void testPolynomialFrac();
void testPolynomialMatrixFrac();

int main() {
	try {
		testFraction();
	}
	catch (exception& e) {
		std::cerr << "FRACTION ERROR" << endl << e.what() << std::endl;
		return -1;
	}
	try {
		testMatrixInt();
	}
	catch (exception& e) {
		std::cerr << "MATRIX ERROR" << endl << e.what() << std::endl;
		return -1;
	}
	try {
		testMatrixFracInt();
	}
	catch (exception& e) {
		std::cerr << "MATRIX-FRACTION ERROR" << endl << e.what() << std::endl;
		return -1;
	}
	try {
		testMatrixFrac();
	}
	catch (exception& e) {
		std::cerr << "MATRIX-FRACTION ERROR" << endl << e.what() << std::endl;
		return -1;
	}
	try {
		testPolynomialFrac();
	}
	catch (exception& e) {
		std::cerr << "POLYNOMIAL-FRACTION ERROR" << endl << e.what() << std::endl;
		return -1;
	}
	try {
		testPolynomialMatrixFrac();
	}
	catch (exception& e) {
		std::cerr << "POLYNOMIAL-MATRIX-FRACTION ERROR" << endl << e.what() << std::endl;
		return -1;
	}
  return 0;
}


void testFraction() {
	// ===== Дробь =====
	frac f(0);
	cout << "FRACTION TEST" << endl;
	cout << "Fraction = " << f << endl << endl;
}

void testMatrixInt() {
	// ===== Матрица 4х4: целые числа =====
	vector<vector<int> > vi_4 = {{1, 2, 3, 4},
															 {5, 6, 7, 8},
															 {9, 10, 11, 12},
															 {13, 14, 15, 16}};
	Matrix<4, int> mi_4(vi_4);

	cout << "MATRIX OF INTEGERS TEST";
	cout << mi_4 << endl << endl;
}

void testMatrixFracInt() {
	// ===== Матрица 3x3: frac(целые) =====
	vector<vector<frac> > vf_3 = {{frac(2), frac(5), frac(7)},
																{frac(6), frac(3), frac(4)},
																{frac(5), frac(-2), frac(-3)}};
	Matrix<3, frac> mf_3(vf_3);

	cout << "MATRIX OF FRAC(INTEGERS) TEST" << endl;
	cout << "Determinant of" << mf_3;
	cout << "is " << mf_3.determinant() << endl << endl;
	cout << "Inversed is" << mf_3.inverse() << endl;
}

void testMatrixFrac() {
	// ===== Матрица 2х2: дроби =====
	vector<vector<frac> > vf_2 = {{frac(4, 2), frac(4, -6)},
																{frac(2, -5), frac(5, 2)}};
	Matrix<2, frac> mf_2(vf_2);

	cout << endl << "MATRIX OF FRACTIONS TEST" << endl;
	cout << "Determinant of" << mf_2;
	cout << "is " << mf_2.determinant() << endl << endl;
	cout << "A * A^1 == E:" << mf_2 * mf_2.inverse() << endl;
}

void testPolynomialFrac() {
	// Полином дробных коэффициентов
	map<unsigned, frac> mf1 = {{0, frac(1, 2)},
														 {2, frac(1)}},
	mf2 = {{0, frac(1)},
	{1, frac(1)}};
	Polynom<frac> pf1(mf1), pf2(mf2);

	cout << endl << "POLYNOMIAL OF FRACTIONS TEST" << endl;
	cout << "Polynom 1 :      " << pf1 << endl;
	cout << "Polynom 2 :      " << pf2 << endl;
	cout << "Summary :        " << pf1 + pf2 << endl;
	cout << "Substraction :   " << pf1 - pf2 << endl;
	cout << "Multiplication : " << pf1 * pf2 << endl;
	cout << "Division :       " << pf1 / pf2 << endl;
	cout << "Modulo :         " << pf1 % pf2 << endl << endl;
}

void testPolynomialMatrixFrac() {
	// ===== Полином с матричными коэффициентами =====
	vector<vector<frac> > vf_3_p = {{frac(1, 2), frac(2), frac(3)},
																	{frac(4), frac(0), frac(0)},
																	{frac(7), frac(8), frac(9)}};
	Matrix<3, frac> m_3_p(vf_3_p);
	map<unsigned, Matrix<3, frac> > mmf = {{1, m_3_p}, {5, m_3_p}};
	Polynom<Matrix<3, frac> > pm_3f(mmf);

	cout << endl << "POLYNOMIAL OF MATRIX OF FRACTIONS TEST" << endl;
	cout << "Polynom A:" << pm_3f << endl << endl;
	cout << "Calculate (A^2 - 10A + A):";
	cout << (pm_3f * pm_3f) - (pm_3f * 10) + pm_3f << endl << endl;
	cout << "Derivative:" << pm_3f.derivative() << endl << endl;
	cout << "Divided by itself:" << pm_3f / pm_3f << endl << endl;


	// ===== Проверка суперпозиции подстановкой обратного коэффициента =====
	map<unsigned, Matrix<3, frac> > simple_mmf = {{1, m_3_p}};
	Polynom<Matrix<3, frac> > simple_pm_3f(simple_mmf);
	cout << "SUPERPOSITION TEST" << endl;
	cout << "Monom A(x):" << simple_pm_3f << endl << endl;
	cout << "A^(-1): " << m_3_p.inverse() << endl;
	cout << "A(A^(-1)):" << simple_pm_3f.atX(m_3_p.inverse()) << endl;
}