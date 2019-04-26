#ifndef POLYNOMIAL_H_INCLUDED
#define POLYNOMIAL_H_INCLUDED

#include <vector>
#include <ostream>
#include <numeric>
#include <boost/multiprecision/cpp_int.hpp>
typedef boost::multiprecision::cpp_rational ratio;
#include <type_traits>
#include <string>
#include <regex>
#include <algorithm>

template <class T>
class polynomial {
  std::vector<T> coefficients;
 public:
  polynomial(std::vector<T> in) : coefficients(in) {
    this->remove_trailing_zeroes();
  }
  polynomial() {}
  polynomial(std::string);
  polynomial operator*(polynomial p) {
    polynomial<T> out;
    unsigned int counter1 = this->degree();
    unsigned int counter2 = p.degree();
    const unsigned int deg = counter1 + counter2 + 1;
    out.coefficients.resize(deg);
    for(auto a : this->coefficients) {
      counter2 = p.degree();
      for(auto b : p.coefficients) {
        out.coefficients.at(deg - counter1 - counter2 - 1) += a * b;
        --counter2;
      }
      --counter1;
    }
    out.remove_trailing_zeroes();
    return out;
  }
  polynomial operator+(polynomial p) {
    polynomial<T> out;
    unsigned int counter = 0;
    if(this->degree() > p.degree()) {
      out.coefficients = this->coefficients;
      for(auto a : p.coefficients) {
        out.coefficients[counter] += a;
        ++counter;
      }
    } else {
      out = p;
      for(auto a : this->coefficients) {
        out.coefficients[counter] += a;
        ++counter;
      }
    }
    out.remove_trailing_zeroes();
    return out;
  }
  polynomial operator-() {
    polynomial<T> out;
    out.coefficients.resize(this->degree() + 1);
    unsigned int counter = 0;
    for(auto a : this->coefficients) {
      out.coefficients[counter] = -a;
      ++counter;
    }
    out.remove_trailing_zeroes();
    return out;
  }
  polynomial operator-(polynomial p) {
    polynomial<T> out;
    unsigned int counter = 0;
    if(this->degree() > p.degree()) {
      out.coefficients = this->coefficients;
      for(auto a : p.coefficients) {
        out.coefficients[counter] -= a;
        ++counter;
      }
    } else {
      out = p;
      for(auto a : this->coefficients) {
        out.coefficients[counter] -= a;
        ++counter;
      }
    }
    out.remove_trailing_zeroes();
    return out;
  }
  bool operator==(polynomial p) {
    if(this-> degree() != p.degree()) {
      return false;
    }
    unsigned int counter = 0;
    for(auto a : this->coefficients) {
      if(a != p.coefficients[counter]) {
        return false;
      }
      ++counter;
    }
    return true;
  }
  bool operator!=(polynomial p) {
    return !(*this == p);
  }

  /*polynomial<int> operator/(polynomial<int> p) { // mathematically, p / q = s + r / d but we ignore the residue r and return s
    if(p == polynomial({0})) {                   // just like dividing ints
      throw std::domain_error("Division by 0!");
    }
    polynomial<ratio> residue = this;
    polynomial<ratio> out;
    while(degree(reside) > degree(p)) {
      ratio leading_coefficient_ratio = residue.coefficients.front() / p.coefficients.front();
      out += leading_coefficient_ratio * polynomial<int>("")
    }
  }*/
  template <class U>
  polynomial operator*(U u) { // multiply the polynomial by a scalar
    polynomial<T> out;
    out.coefficients.resize(this->degree() + 1);
    unsigned int counter = 0;
    for(auto a : this->coefficients) {
      out.coefficients[counter] = u * a;
      ++counter;
    }
    out.remove_trailing_zeroes();
    return out;
  }
  template <class U>
  friend std::ostream& operator<<(std::ostream& out, const polynomial<U>& p);

  int degree();
  void remove_trailing_zeroes();
};

template <class U, class T>
polynomial<T> operator*(U u, polynomial<T> p) {
  return p * u;
}

template <class T>
std::ostream& operator<<(std::ostream& out, const polynomial<T>& p) { // print in a nice human readable format
  unsigned int counter = 0;
  bool all_first_were_zero = true;
  for(auto a : p.coefficients) {
    //std::cout << a << std::endl;
    if(!all_first_were_zero && counter != 0 && counter != 1) {
      if(a < -1) {
        out << a << "x^" << counter;
      }
      if(a == -1) {
        out << "-x^" << counter;
      }
      if(a == 1) {
        out << "+x^" << counter;
      }
      if(a > 1) {
        out << "+" << a << "x^" << counter;
      }
    }
    if(!all_first_were_zero && counter == 1) {
      if(a < -1) {
        out << a << "x";
      }
      if(a == -1) {
        out << "-x";
      }
      if(a == 1) {
        out << "+x";
      }
      if(a > 1) {
        out << "+" << a << "x";
      }
    }
    if(counter != 0 && all_first_were_zero && a != 0) {
      if(counter != 1) {
        if(a < -1 || a > 1) {
          out << a << "x^" << counter;
        }
        if(a == -1) {
          out << "x^" << counter;
        }
        if(a == 1) {
          out << "x^" << counter;
        }
      } else {
        if(a < -1 || a > 1) {
          out << a << "x";
        }
        if(a == -1) {
          out << "x";
        }
        if(a == 1) {
          out << "x";
        }
      }
      all_first_were_zero = false;
    }
    if(counter == 0 && a != 0) {
      out << a;
      all_first_were_zero = false;
    }
    ++counter;
  }
  if(p.coefficients.empty()) {
    out << "0";
  }
  return out;
}

template <class T, class U>
U evaluate(polynomial<T> p, U x) { // evaluate P at x
  U result = 0;
  U x_to_the_n = 1;
  for(U i : p.coefficients) {
    result += i * x_to_the_n;
    x_to_the_n *= x;
  }
  return result;
}

#endif // POLYNOMIAL_H_INCLUDED
