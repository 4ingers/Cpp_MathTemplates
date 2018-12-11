#include <iostream>
#include "fraction.h"
#include "matrix.h"
#include "polynom.h"

typedef Fraction<int> frac;
typedef Fraction<unsigned char> shortfrac;
using namespace std;

int main() try {

  // ===== Дробь =====

  frac f;//(4, 0);

  cout << endl;
  cout << "-------------" << endl;
  cout << "FRACTION TEST" << endl;
  cout << "-------------" << endl;
  cout << "Fraction = "   << f << endl << endl;


  // ===== Матрица 4х4: целые числа =====
  vector<vector<int> > vi_4 = { { 1, 2, 3, 4 },
                                { 5, 6, 7, 8 },
                                { 9, 10, 11, 12 },
                                { 13, 14, 15, 16 } };
  Matrix<4,int> mi_4(vi_4);

  cout << "-----------------------" << endl;
  cout << "MATRIX OF INTEGERS TEST" << endl;
  cout << "-----------------------";
  cout << mi_4 << endl << endl;


  // ===== Матрица 3x3: frac(целые) =====
  vector<vector<frac> > vf_3 = { {frac(2), frac(5), frac(7) },
                                 {frac(6), frac(3), frac(4) },
                                 {frac(5), frac(-2), frac(-3) } };
  Matrix<3, frac> mf_3(vf_3);

  cout << "-----------------------------" << endl;
  cout << "MATRIX OF FRAC(INTEGERS) TEST" << endl;
  cout << "-----------------------------" << endl;
  cout << "Determinant of" << mf_3;
  cout << "is " << mf_3.determinant() << endl << endl;
  cout << "Inversed is" << mf_3.inverse() << endl;


  // ===== Матрица 2х2: дроби =====
  vector<vector<frac> > vf_2 = { { frac(4, 2), frac(4, -6) },
                                 { frac(2, -5), frac(5, 2) } };
  Matrix<2, frac> mf_2(vf_2);

  cout << "------------------------" << endl;
  cout << "MATRIX OF FRACTIONS TEST" << endl;
  cout << "------------------------" << endl;
  cout << "Determinant of" << mf_2;
  cout << "is " << mf_2.determinant() << endl << endl;
  cout << "A * A^1 == E:" << mf_2 * mf_2.inverse() << endl;


  // ===== Полином целых коэффициентов =====
  map<unsigned, int> m1 = { { 0,  1},
                            { 2, 1 } },
                     m2 = { { 0, 1 },
                            { 1, 1 } };
	Polynom<int> pi1(m1), pi2(m2);

   cout << "------------------------" << endl;
   cout << "POLYNOM OF INTEGERS TEST" << endl;
   cout << "------------------------" << endl;
	 cout << "Polynom 1 :      " << pi1 << endl;
   cout << "Polynom 2 :      " << pi2 << endl;
   cout << "Summary :        " << pi1 + pi2 << endl;
   cout << "Substraction :   " << pi1 - pi2 << endl;
   cout << "Multiplication : " << pi1 * pi2 << endl;
   cout << "Division :       " << pi1 / pi2 << endl;
   cout << "Modulo :         " << pi1 % pi2 << endl << endl;


   // Полином дробных коэффициентов
   map<unsigned, frac> mf1 = { { 0,  frac(1, 2)},
                                { 2, frac(1) } },
                       mf2 = { { 0, frac(1) },
                               { 1, frac(1) } };
 	 Polynom<frac> pf1(mf1), pf2(mf2);

   cout << "-------------------------" << endl;
   cout << "POLYNOM OF FRACTIONS TEST" << endl;
   cout << "-------------------------" << endl;
   cout << "Polynom 1 :      " << pf1 << endl;
   cout << "Polynom 2 :      " << pf2 << endl;
   cout << "Summary :        " << pf1 + pf2 << endl;
   cout << "Substraction :   " << pf1 - pf2 << endl;
   cout << "Multiplication : " << pf1 * pf2 << endl;
   cout << "Division :       " << pf1 / pf2 << endl;
   cout << "Modulo :         " << pf1 % pf2 << endl << endl;


   // ===== Полином с матричными коэффициентами =====
   vector<vector<frac> > vf_3_p = { { frac(1), frac(2), frac(3) },
                                  { frac(4), frac(0), frac(0) },
                                  { frac(7), frac(8), frac(9) } };
   Matrix<3, frac> m_3_p(vf_3_p);
   map<unsigned, Matrix<3, frac> > mmf = { { 1, m_3_p }, {5, m_3_p} };
   Polynom<Matrix<3, frac> > pm_3f(mmf);

   cout << "-----------------------------------" << endl;
   cout << "POLYNOM OF MATRIX OF FRACTIONS TEST" << endl;
   cout << "-----------------------------------" << endl;
   cout << "Polynom A:" << endl << pm_3f << endl << endl << endl;
   cout << "Calculate (A^2 - 10A + A):" << endl;
   cout << (pm_3f*pm_3f)-(pm_3f*10)+pm_3f << endl << endl << endl;
   cout << "Derivative:" << endl << pm_3f.derivative() << endl << endl << endl;
   cout << "Divided by itself:" << pm_3f / pm_3f << endl << endl;


   // ===== Проверка суперпозиции подстановкой обратного коэффициента =====
   map<unsigned, Matrix<3, frac> > simple_mmf = { { 1, m_3_p } };
   Polynom<Matrix<3, frac> > simple_pm_3f(simple_mmf);
   cout << "------------------" << endl;
   cout << "SUPERPOSITION TEST" << endl;
   cout << "------------------" << endl;
   cout << "Monom A(x):" << simple_pm_3f << endl << endl;
   cout << "A^(-1): "    << m_3_p.inverse() << endl;
   cout << "A(A^(-1)):"  << simple_pm_3f.atX(m_3_p.inverse()) << endl;

  return 0;
}
catch(exception& e) {
  std::cerr << "> " << e.what() << std::endl;
  return -1;
}

// 2 5 7
// 6 3 4
// 5 -2 -3
