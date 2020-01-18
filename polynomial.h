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
#include <range/v3/all.hpp>

template <class T>
class polynomial {
  using ratio = boost::multiprecision::cpp_rational;
  std::vector<T> coefficients;
  std::string var;
public:
  // ------------------------------------------------ CONSTRUCTORS ------------------------------------------------ //
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
        if(c.first.front() == 'x') {
          c.first = "1";
        }
      }
      if(std::stoi(c.second) > this->degree()) {
        this->coefficients.resize(std::stoi(c.second) + 1);
      }
      coefficients[std::stoi(c.second)] += std::stoi(c.first);
    }
    this->remove_trailing_zeroes();
  }
  // ------------------------------------------------- OPERATORS -------------------------------------------------- //
  polynomial operator*(polynomial p) const {
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
  polynomial operator+(polynomial p) const {
    if(this->degree() >= p.degree()) {
      auto cp = *this;
      for(auto [a,b] : ranges::v3::view::zip(cp.coefficients, p.coefficients)) {
        a += b;
      }
      cp.remove_trailing_zeroes();
      return cp;
    } else {
      return p + *this;
    }
  }
  polynomial operator-() const {
    auto cp = *this;
    for(auto &a : cp.coefficients) {
      a = -a;
    }
    cp.remove_trailing_zeroes();
    return cp;
  }
  polynomial operator-(polynomial p) const {
    auto cp = *this;
    cp = cp + (-p);
    cp.remove_trailing_zeroes();
    return cp;
  }
  bool operator==(polynomial p) const {
    if(this-> degree() != p.degree()) {
      return false;
    }
    for(auto [a,b] : ranges::v3::view::zip(this->coefficients, p.coefficients)) {
      if(a != b) {
        return false;
      }
    }
    return true;
  }
  bool operator!=(polynomial p) const {
    return !(*this == p);
  }

  polynomial<int> operator/(polynomial<int> p) const { // mathematically, p / q = s + r / q but we ignore the residue r and return s
    if(p.degree() == -1) {                             // just like dividing ints
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
      residue = residue - m * p1;
      residue.remove_trailing_zeroes();
      --n;
    }
    ratio r = out.coefficients.back();
    polynomial<int> o (out);
    return o;
  }
  polynomial<int> operator%(polynomial<int> p) const {
    auto q = *this / p;
    return *this - p * q;
  }
  template <class U>
  bool operator>=(polynomial<U> p) const {
    if(this->degree() > p.degree()) {
      return this->coefficients.back() > static_cast<U>(0);
    }
    if(this->degree() < p.degree()) {
      return p.coefficients.back() > static_cast<U>(0);
    }
    for(int i = this->degree(); i >= 0; --i) {
      if(this->coefficients[i] != p.coefficients[i]) {
        return this->coefficients[i] > p.coefficients[i];
      }
    }
    return true;
  }
  bool operator>(polynomial p) const {
    return (*this >= p) && (*this != p);
  }
  bool operator<=(polynomial p) const {
    return p >= *this;
  }
  bool operator<(polynomial p) const {
    return p > *this;
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
  void operator%=(polynomial p) {
    *this = *this % p;
  }

  template<class U>
  friend std::ostream& operator<<(std::ostream& out, const polynomial<U>& p);

  template <class U>
  polynomial operator*(U u) const { // multiply the polynomial by a scalar
    polynomial<T> out;
    int counter = 0;
    for(auto a : this->coefficients) {
      out.coefficients[counter] = u * a;
      ++counter;
    }
    out.remove_trailing_zeroes();
    return out;
  }

  // ------------------------------------------------ OTHER FUNCTIONS --------------------------------------------- //

  int degree() const;
  template <class U>
  polynomial<U> monomial(U a, int b) const;
  void set_var(std::string s);
private:
  void remove_trailing_zeroes();
};

template <class U, class T>
polynomial<T> operator*(U u, polynomial<T> p) {
  return p * u;
}

template <class T>
void polynomial<T>::remove_trailing_zeroes() {  // if the coeffient on the highest exponent is zero, shrink the coefficent vector to fit
  int i = this->degree();                       // it is important to do this after all operations to ensure the next operations will be correct
  while(this->coefficients[i] == T{0}) {
    this->coefficients.resize(i);
    --i;
    if(i == -1) break;
    if(i == 0) {
      if(this->coefficients[0] == T{0}) {
        this->coefficients.resize(0);
      }
      break;
    }
  }
  return;
}

template <class T>
int polynomial<T>::degree() const {      // the degree of p
  return this->coefficients.size() - 1;  // this is the highest exponent on x, except when p is identically 0 when it is -1
}                                        // in which case it should be interpreted as negative infinity

template <class T>
void polynomial<T>::set_var(std::string s) {
  this->var = s;
}

template <class T>
void append(std::ostringstream& stream, T a) {
  if(a > 0) {
    stream << "+" << a;
  } else if(a != 0){
    stream << a;
  }
}

template <class T>
void append(std::ostringstream& stream, polynomial<T> a) {
  if(a != polynomial<int>{0}) {
    stream << "+(" << a << ")";
  }
}

template <class T>
std::ostream& operator<<(std::ostream& out, const polynomial<T>& p) { // print in a nice human readable format
  auto var = p.var;
  if(var == "") {
    var = "x";
  }
  std::ostringstream stream;
  for(unsigned int i = 0; i != p.coefficients.size(); ++i) {
    auto a = p.coefficients[i];
    append(stream, a);
    stream << var << "^" << i;
  }
  if(p.coefficients.empty()) {
    out << "0";
  }
  std::string s = stream.str();
  s = std::regex_replace(s, std::regex("^\\+"), "");
  s = std::regex_replace(s, std::regex(var + "\\^0"), "");
  s = std::regex_replace(s, std::regex("\\^1"), "");
  s = std::regex_replace(s, std::regex("1" + var), var);
  out << s;
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
polynomial<U> polynomial<T>::monomial(U a, int b) const { // a * x ^ b
  polynomial<U> out;
  out.coefficients.resize(b + 1);
  out.coefficients[b] = a;
  return out;
}
template <typename T>
polynomial<T> monomial(T a, int b) {
  polynomial<T> out;
  out.coefficients.resize(b + 1);
  out.coefficients[b] = a;
  return out;
}

#endif // POLYNOMIAL_H_INCLUDED

