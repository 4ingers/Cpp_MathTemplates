#pragma once

#include <map>
#include <iterator>

template <typename T = int>
class Polynom {
public:
	// ===== Конструкторы =====
	Polynom() = default;
	Polynom(std::map<unsigned, T>&);
	Polynom(const Polynom&);

	// ===== Перегрузки =====
	Polynom operator +(const Polynom&) const;
	Polynom operator -(const Polynom&) const;
	Polynom operator *(const Polynom&) const;
	Polynom operator *(const int&) const;
	Polynom operator /(const Polynom&) const;
	Polynom operator %(const Polynom&) const;

	Polynom& operator =(const Polynom&);

	// ----- Добавить моном -----
	void addMonom(const T&, const unsigned&);
	// ----- Производная -----
	Polynom derivative() const;
	// ----- Значение функции в точке -----
	T atX(const T&) const;
	// ----- Суперпозиция -----
	Polynom superposition(const Polynom&) const;

  // ===== Перегрузка оператора вставки в поток =====
	template<typename U>
	friend std::ostream& operator<< (std::ostream &, const Polynom<U>&);

private:
	// ===== Хэш: ключ-степень, значение-коэффициент =====
	std::map<unsigned, T> polynom;
};



// ===== Конструкторы =====
template <typename T>
Polynom<T>::Polynom(std::map<unsigned, T>& initmap) {
	this->polynom = initmap;
}

template <typename T>
Polynom<T>::Polynom(const Polynom& other) {
	this->polynom = other.polynom;
}


// ===== Перегрузки операторов =====

// ----- + -----
template <typename T>
Polynom<T> Polynom<T>::operator +(const Polynom<T>& other) const {
	Polynom<T> result(*this);
	for (auto it : other.polynom)
		result.addMonom(it.second, it.first);
	return result;
}

// ----- - -----
template <typename T>
Polynom<T> Polynom<T>::operator -(const Polynom<T>& other) const {
	Polynom<T> result(other * -1);
	result = *this + result;
	return result;
}

// ----- * -----
template <typename T>
Polynom<T> Polynom<T>::operator *(const Polynom<T>& other) const {
	Polynom<T> result;
	for (auto it : this->polynom)
		for (auto jt : other.polynom)
			result.addMonom(it.second * jt.second, it.first + jt.first);
	return result;
}

template <typename T>
Polynom<T> Polynom<T>::operator *(const int& num) const {
	Polynom<T> result;
	for (auto it : this->polynom)
		result.addMonom(it.second * num, it.first);
	return result;
}

// ----- / -----
template<typename T>
Polynom<T> Polynom<T>::operator /(const Polynom<T>& other) const {
	Polynom<T> divident(*this), result;
	auto inp_it = other.polynom.end(); --inp_it;
	while (true) {
		auto div_it = divident.polynom.end(); --div_it;
		if (div_it->first < inp_it->first)
			return result;
		else {
			for (auto it = other.polynom.begin(); it != inp_it; ++it)
				divident.addMonom(T(-1) * it->second * div_it->second / inp_it->second,
													it->first +	div_it->first -	inp_it->first);
			result.addMonom(div_it->second / inp_it->second,
			div_it->first - inp_it->first);
			divident.polynom.erase(div_it);
		}
	}
}

// ----- % -----
template <typename T>
Polynom<T> Polynom<T>::operator %(const Polynom<T>& other) const {
	Polynom<T> result(*this);
	auto inp_it = other.polynom.end(); --inp_it;
	while (true) {
		auto res_it = result.polynom.end(); --res_it;
		if (res_it->first < inp_it->first)
			return result;
		else {
			for (auto it = other.polynom.begin(); it != inp_it; ++it)
				result.addMonom(T(-1) * it->second * res_it->second / inp_it->second,
												it->first + res_it->first - inp_it->first);
			result.polynom.erase(res_it);
		}
	}
}


// ----- = -----
template <typename T>
Polynom<T>& Polynom<T>::operator =(const Polynom<T>& other) {
	this->polynom = other.polynom;
	return *this;
}


// ===== Добавить моном =====
template <typename T>
void Polynom<T>::addMonom(const T& coef, const unsigned& degree) {
	if (this->polynom.count(degree))
		this->polynom[degree] = this->polynom[degree] + coef;
	else
		this->polynom[degree] = coef;

	if (this->polynom[degree] == T(0)) {
		auto it = this->polynom.find(degree);
		this->polynom.erase(it);
	}
}

// ===== Производная =====
template <typename T>
Polynom<T> Polynom<T>::derivative() const {
	Polynom<T> result;
	for (auto it : this->polynom)
		if (0 != it.first)
			result.addMonom(it.second * it.first, it.first - 1);
	return result;
}

// ===== Значение функции в точке =====
template <typename T>
T Polynom<T>::atX(const T& num) const {
	if (this->polynom.empty())
		return T(0);
	T result(0), pw_val;
	for (auto it : this->polynom) {
		if (0 != it.first)
			pw_val = num;
		else
			pw_val = 1;
		for (int i = it.first - 1; i > 0; --i)
			pw_val = pw_val * num;
		result = result + pw_val * it.second;
	}
	return result;
}

// ===== Суперпозиция =====
template <typename T>
Polynom<T> Polynom<T>::superposition(const Polynom<T>& inner) const {
	Polynom<T> result;
	for (auto it : this->polynom) {
		if (it.first > 0) {
			Polynom<T> grPol(inner), coefPol;
			for (int i = it.first - 1; i > 0; --i)
				grPol = grPol * inner;
			coefPol.addMonom(it.second, 0);
			result = result + grPol * coefPol;
		}
		else
			result.addMonom(it.second, 0);
	}
	return result;
}

// ===== Оператор вывода в поток =====
template <typename T>
std::ostream & operator<<(std::ostream& out, const Polynom<T>& pol) {
	if (pol.polynom.empty())
		out << 0;
	else {
		auto it = pol.polynom.end();
		--it;
		for (auto jt = pol.polynom.begin(); jt != it; ++jt)
			if (jt->first == 0)
				out << jt->second << " + ";
			else
				if (jt->first == 1)
					out << jt->second << "x" << " + ";
				else
					out << jt->second << "x^" << jt->first << " + ";

		if (0 == it->first)
			out << it->second;
		else
			if (it->first == 1)
				out << it->second << "x";
			else
				out << it->second << "x^" << it->first;
	}
	return out;
}
