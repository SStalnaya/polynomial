#ifndef POLYNOMIAL_H_INCLUDED
#define POLYNOMIAL_H_INCLUDED

#include <vector>
#include <ostream>
#include <numeric>
#include <boost/multiprecision/cpp_int.hpp>
#include <type_traits>
#include <string>
#include <regex>
#include <algorithm>
#include <initializer_list>

template <class T>
class polynomial {
  using ratio = boost::multiprecision::cpp_rational;
public:
  std::vector<T> coefficients;
  polynomial(std::vector<T> in) : coefficients(in) {
    this->remove_trailing_zeroes();
  }
  polynomial(std::initializer_list<T> in) : coefficients(in) {
    this->remove_trailing_zeroes();
  }
  template <class U>
  polynomial(const polynomial<U>& in) {
    for(auto x : in.coefficients) {
      this->coefficients.push_back(static_cast<T>(x));
    }
    this->remove_trailing_zeroes();
  }
  polynomial() {}
  polynomial(std::string s) {           // initialize the polynomial with a string e.g. 3+x^4-2x^3+3x^3
    std::string term;
    s = std::regex_replace(s, std::regex("(.)-"), "$1+-");
    s = std::regex_replace(s, std::regex("\\+x"), "+1x");
    s = std::regex_replace(s, std::regex("-x"), "-1x");
    s.erase(std::remove_if(s.begin(), s.end(), [](unsigned char x){return isspace(x) || x == '{' || x == '}' || x == '*';}), s.end());
    std::stringstream t (s);
    while(std::getline(t, term, '+')) { // divide into terms on the above example they will be 3, +x^4, -2x^3, 3x^3
      std::stringstream tern (term);
      std::pair<std::string, std::string> c;         // pair (coefficient, exponent) of the term
      std::string d;
      bool is_first = true;
      while(std::getline(tern, d, 'x')) {
        if(is_first) {
          c.first = d;
        } else {
          c.second = d;
        }
        is_first = false;
      }
      if(c.first == "+") {
        c.first = "1";
      }
      if(c.first == "-") {
        c.first = "-1";
      }
      if(c.second.front() == '^') {
        c.second.erase(0, 1);
      }
      if(c.second.empty()) {
        if(term.back() == 'x') {
          //c.first = "1";
          c.second = "1";
        } else {
          c.second = "0";
        }
      }
      if(c.first.empty()) {
        c.first = term;
        if(c.first.back() == 'x') {
          c.first.erase(c.first.size() - 1, 1);
          if(c.first.empty()) {
            c.first = "1";
          }
        }
      }
      if(std::stoi(c.second) > this->degree()) {
        this->coefficients.resize(std::stoi(c.second) + 1);
      }
      coefficients[std::stoi(c.second)] += std::stoi(c.first);
    }
    this->remove_trailing_zeroes();
  }
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

  polynomial<int> operator/(polynomial<int> p) { // mathematically, p / q = s + r / q but we ignore the residue r and return s
    if(p.degree() == -1) {                       // just like dividing ints
      throw std::domain_error("Division by 0!");
    }
    polynomial<ratio> residue (*this);
    polynomial<ratio> out;
    polynomial<ratio> p1 (p);
    int n = residue.degree() - p.degree();
    if(n < 0) {
      polynomial<int> o;
      return o;
    }
    while(n >= 0) {
      ratio leading_coefficient_ratio = residue.coefficients.back() / p.coefficients.back();
      auto m = monomial<ratio>(leading_coefficient_ratio, n);
      out = out + m;
      residue = m * p1 - residue;
      residue.remove_trailing_zeroes();
      --n;
    }
    ratio r = out.coefficients.back();
    polynomial<int> o (out);
    return o;
  }
  void operator+=(polynomial p) {
    *this = *this + p;
  }
  void operator-=(polynomial p) {
    *this = *this - p;
  }
  void operator*=(polynomial p) {
    *this = *this * p;
  }
  void operator/=(polynomial p) {
    *this = *this / p;
  }
  template <class U>
  polynomial operator*(U u) { // multiply the polynomial by a scalar
    polynomial<T> out;
    int counter = 0;
    for(auto a : this->coefficients) {
      out.coefficients[counter] = u * a;
      ++counter;
    }
    out.remove_trailing_zeroes();
    return out;
  }

  int degree();
  void remove_trailing_zeroes();
  template <class U>
  polynomial<U> monomial(U a, int b);
  polynomial monomial(T a, int b);
};

template <class U, class T>
polynomial<T> operator*(U u, polynomial<T> p) {
  return p * u;
}

template <class T>
void polynomial<T>::remove_trailing_zeroes() {      // if the coeffient on the highest exponent is zero, shrink the coefficent vector to fit
  unsigned int i = this->degree();                  // it is important to do this after all operations to ensure the next operations will be correct
  while(this->coefficients[i] == 0) {
    this->coefficients.resize(i);
    --i;
    if(i == 0) {
      if(this->coefficients[0] == 0) {
        this->coefficients.resize(0);
      }
      break;
    }
  }
  return;
}

template <class T>
int polynomial<T>::degree() {            // the degree of p
  return this->coefficients.size() - 1;  // this is the highest exponent on x, except when p is identically 0 when it is -1
}                                        // in which case it should be interpreted as negative infinity

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

std::vector<int> factor(int n) { // the positive divisors of n
  if(n < 0) {                    // remember to write something more efficient later
    return factor(-n);
  }
  std::vector<int> out;
  for(int a = 1; a != n + 1; ++a) {
    if(n % a == 0) {
      out.push_back(a);
    }
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

template <class T>
void factor(polynomial<T> in) {
  // use the rational root test to eliminate linear factors
  auto v1 = factor(in.coefficients.front() / std::gcd(in.coefficients.front(), in.coefficients.back()));
  auto v2 = factor(in.coefficients.back() / std::gcd(in.coefficients.front(), in.coefficients.back()));
  for(auto p : v1) {
    for(auto q : v2) {
      if(evaluate(in, ratio(p, q)) == 0) {
        std::cout << p << "," << q << std::endl;
      }
      if(evaluate(in, ratio(-p, q)) == 0) {
        std::cout <<-p << "," << q << std::endl;
      }
    }
  }
  // find non-linear factors using kronecker's method
  if constexpr(std::is_same<T, int>::value) {
    std::cout << "is int";
  }
  return;
}

template <typename T> template <typename U>
polynomial<U> polynomial<T>::monomial(U a, int b) { // a * x ^ b
  polynomial<U> out;
  out.coefficients.resize(b + 1);
  out.coefficients[b] = a;
  return out;
}
template <typename T>
polynomial<T> monomial(T a, int b) { // a * x ^ b
  polynomial<T> out;
  out.coefficients.resize(b + 1);
  out.coefficients[b] = a;
  return out;
}

#endif // POLYNOMIAL_H_INCLUDED

