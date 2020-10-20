#pragma once
#include <iostream>
#include <vector>
using std::vector;

namespace task {

vector<double> operator+(const vector<double> &a, const vector<double> &b) {
  vector<double> c(a.size());
  for (size_t i = 0; i < a.size(); ++i) {
    c[i] = a[i] + b[i];
  }
  return c;
}

vector<double> operator+(const vector<double> &a) { return a; }

vector<double> operator-(const vector<double> &a, const vector<double> &b) {
  vector<double> c(a.size());
  for (size_t i = 0; i < a.size(); ++i) {
    c[i] = a[i] - b[i];
  }
  return c;
}

vector<double> operator-(const vector<double> &a) {
  vector<double> c(a.size());
  for (size_t i = 0; i < a.size(); ++i) {
    c[i] = -a[i];
  }
  return c;
}

double operator*(const vector<double> &a, const vector<double> &b) {
  double c = 0;
  for (size_t i = 0; i < a.size(); ++i) {
    c += a[i] * b[i];
  }
  return c;
}

vector<double> operator%(const vector<double> &a, const vector<double> &b) {
  vector<double> c(3);
  c[0] = a[1] * b[2] - a[2] * b[1];
  c[1] = a[2] * b[0] - a[0] * b[2];
  c[2] = a[0] * b[1] - a[1] * b[0];
  return c;
}

bool operator||(const vector<double> &a, const vector<double> &b) {
  const double eps = 1e-7;

  for (size_t i = 1; i < a.size(); ++i) {
    if (a[i] * b[0] - a[0] * b[i] > eps)
      return false;
  }
  return true;
}

bool operator&&(const vector<double> &a, const vector<double> &b) {
  return ((a || b) && (a[0] * b[0]) >= 0);
}

std::istream &operator>>(std::istream &is, vector<double> &a) {
  size_t n;
  is >> n;
  if (a.size() > 0)
    a.resize(n);
  for (size_t i = 0; i < n; ++i) {
    double val;
    is >> val;
    a[i] = val;
  }
  return is;
}

std::ostream &operator<<(std::ostream &os, vector<double> &a) {
  for (size_t i = 0; i < a.size(); ++i) {
    os << a[i] << " ";
  }
  os << '\n';
  return os;
}

void reverse(vector<double> &a) {
  size_t n = a.size();
  for (size_t i = 0; i < n / 2; ++i) {
    std::swap(a[i], a[n - i - 1]);
  }
}

vector<int> operator|(const vector<int> &a, const vector<int> &b) {
  vector<int> c(a.size());
  for (size_t i = 0; i < a.size(); ++i) {
    c[i] = a[i] | b[i];
  }
  return c;
}

vector<int> operator&(const vector<int> &a, const vector<int> &b) {
  vector<int> c(a.size());
  for (size_t i = 0; i < a.size(); ++i) {
    c[i] = a[i] & b[i];
  }
  return c;
}
} // namespace task
