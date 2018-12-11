#pragma once

template <typename T = int>
class Fraction {
public:
  // ===== Конструкторы =====
  Fraction() = default;
  Fraction(const T);
  Fraction(const T numerator, const T denominator);
  Fraction(const Fraction&);

  // ===== Перегрузки =====
  Fraction operator +(const Fraction&) const;
  Fraction operator -(const Fraction&) const;
  Fraction operator *(const Fraction&) const;
  Fraction operator /(const Fraction&) const;

  Fraction& operator =(const Fraction&);
  Fraction& operator =(const T&);

  bool operator ==(const Fraction&) const;
  bool operator ==(const T&) const;

  // ----- Сокращение -----
  void reduce();
  // ----- Корректировка знака -----
  void negatify();
  // ----- Обобщённая корректировка -----
  void simplify();

  // ===== Перегрузка операторов потокового ввода-вывода =====
  template <typename U>
  friend std::ostream& operator<< (std::ostream&, const Fraction<U>&);
  template <typename U>
  friend std::istream& operator>> (std::istream&, Fraction<U>&);

  // ===== Обработка исключений =====
  class null_dnm : public std::logic_error {
  public:
    explicit null_dnm() : std::logic_error("Denominator is null") {}
  };

  // ----- Проверка знаменателя -----
  void dnmCheck() const;

private:
  // ===== Числитель и знаменатель ======
  T nmr = 0, dnm = 1;
};



// ===== Конструкторы =====
template <typename T>
Fraction<T>::Fraction(const T num) {
  this->nmr = num;
  this->dnm = 1;
}

template <typename T>
Fraction<T>::Fraction(const T numerator, const T denominator) {
  this->nmr = numerator;
  this->dnm = denominator;
  this->dnmCheck();
  this->simplify();
}

template <typename T>
Fraction<T>::Fraction(const Fraction<T>& other) {
  other.dnmCheck();
  this->nmr = other.nmr;
  this->dnm = other.dnm;
  this->simplify();
}


// ===== Перегрузки операторов =====

// ----- + -----
template <typename T>
Fraction<T> Fraction<T>::operator +(const Fraction<T>& other) const {
  Fraction result((this->nmr * other.dnm) + (this->dnm * other.nmr),
                  (this->dnm * other.dnm));
  result.simplify();
  return result;
}

// ----- - -----
template <typename T>
Fraction<T> Fraction<T>::operator -(const Fraction<T>& other) const {
  Fraction result((this->nmr * other.dnm) - (this->dnm * other.nmr),
                  (this->dnm * other.dnm));
  result.simplify();
  return result;
}

// ----- * -----
template <typename T>
Fraction<T> Fraction<T>::operator *(const Fraction<T>& other) const {
  Fraction result(this->nmr * other.nmr, this->dnm * other.dnm);
  result.simplify();
  return result;
}

// ----- / (дробь/дробь)-----
template <typename T>
Fraction<T> Fraction<T>::operator /(const Fraction<T>& other) const {
  Fraction result(this->nmr * other.dnm, this->dnm * other.nmr);
  result.simplify();
  return result;
}


// ----- = -----
template <typename T>
Fraction<T>& Fraction<T>::operator =(const Fraction<T>& other) {
  other.dnmCheck();
	this->nmr = other.nmr;
	this->dnm = other.dnm;
	return *this;
}

template <typename T>
Fraction<T>& Fraction<T>::operator =(const T& num) {
	this->nmr = num;
	this->dnm = 1;
	return *this;
}


// ===== Сравнительные перегрузки =====
template<typename T>
bool Fraction<T>::operator ==(const Fraction<T>& other) const {
  return (this->nmr == other.nmr) && (this->dnm == other.dnm);
}

template<typename T>
bool Fraction<T>::operator ==(const T& num) const {
  return (1 == this->dnm) && (this->nmr == num);
}


// ----- Сокращение -----
template <typename T>
void Fraction<T>::reduce() {
  this->dnmCheck();

  T a = this->nmr,
    b = this->dnm,
    c = 0;

  while (b) {
    c = a % b;
    a = b;
    b = c;
  }

  T GCD = abs(a);
  this->nmr /= GCD;
  this->dnm /= GCD;
}

// ----- Корректировка знака -----
template <typename T>
void Fraction<T>::negatify() {
  if (this->dnm < 0) {
    this->nmr *= -1;
    this->dnm *= -1;
  }
}

// ----- Обобщённая корректировка -----
template <typename T>
void Fraction<T>::simplify() {
  this->reduce();
  this->negatify();
}

// ----- Проверка знаменателя -----
template <typename T>
void Fraction<T>::dnmCheck() const {
  if (0 == this->dnm)
    throw null_dnm();
}


// ----- Перегрузка вывода в поток -----
template<typename T>
std::ostream& operator <<(std::ostream& stream, const Fraction<T>& frac) {
  if (0 == frac.nmr)
    stream << "0";
  else {
    if (1 == frac.dnm)
      stream << frac.nmr;
    else
      stream << "[" << frac.nmr << "/" << frac.dnm << "]";
  }
  return stream;
}

// ----- Перегрузка вставки в поток -----
template <typename T>
std::istream& operator >>(std::istream& stream, Fraction<T>& frac) {
  std::string input;

  stream >> input;
  try {
    frac.nmr = std::stoi(input);
  }
  catch(std::invalid_argument) {
    std::cerr << "> Invalid input for numerator : " << '\"' << input << '\"';
    exit(EXIT_FAILURE);
  }

  stream >> input;
  try {
    frac.dnm = std::stoi(input);
  }
  catch(std::invalid_argument) {
    std::cerr << "> Invalid input for denominator : " << '\"' << input << '\"';
    exit(EXIT_FAILURE);
  }

  frac.simplify();
  return stream;
}
