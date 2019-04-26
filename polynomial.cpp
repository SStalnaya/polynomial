#include "polynomial.h"

template<>
polynomial<int>::polynomial(std::string s) {           // initialize the polynomial with a string e.g. 3+x^4-2x^3+3x^3
  std::string term;
  s = std::regex_replace(s, std::regex("(.)-"), "$1+-");
  s = std::regex_replace(s, std::regex("\\+x"), "+1x");
  s = std::regex_replace(s, std::regex("-x"), "-1x");
  s.erase(std::remove_if(s.begin(), s.end(), [](unsigned char x) {
    return isspace(x) || x == '{' || x == '}' || x == '*';
  }), s.end());
  std::stringstream t(s);
  while(std::getline(t, term, '+')) { // divide into terms on the above example they will be 3, +x^4, -2x^3, 3x^3
    std::stringstream tern(term);
    std::pair<std::string, std::string> c;         // pair (coeffient, exponent) of the term
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


/*template <class T>
void polynomial<T>::polynomial operator-(polynomial p) {
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
}*/

template <class T>
void polynomial<T>::remove_trailing_zeroes() {      // if the coeffient on the highest exponent is zero, shrink the coefficent vector to fit
  unsigned int i = this->degree();                  // it is important to do this after all operations to ensure the next operations will be correct
  while(this->coefficients[i] == 0) {
    this->coefficients.resize(i + 1);
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

std::vector<int> factor(int n) { // the positive divisors of n
  if(n < 0) {                    // remember to write something more efficient later
    return factor(-n);
  }
  std::vector<int> out;
  for(unsigned int a = 1; a != n + 1; ++a) {
    if(n % a == 0) {
      out.push_back(a);
    }
  }
  return out;
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
