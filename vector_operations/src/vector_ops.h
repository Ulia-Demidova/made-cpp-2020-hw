#pragma once
#include <vector>
#include <iostream>
using std::vector;


namespace task {

vector<double> operator+ (const vector<double>& a, const vector<double>& b){
    vector<double> c;
    for (size_t i = 0; i < a.size(); ++i){
        c.push_back(a[i] + b[i]);
    }
    return c;
}

vector<double> operator+ (const vector<double>& a){
    return a;
}

vector<double> operator- (const vector<double>& a, const vector<double>& b){
    vector<double> c;
    for (size_t i = 0; i < a.size(); ++i){
        c.push_back(a[i] - b[i]);
    }
    return c;
}

vector<double> operator- (const vector<double>& a){
    vector<double> c;
    for (size_t i = 0; i < a.size(); ++i){
        c.push_back(-a[i]);
    }
    return c;
}

double operator* (const vector<double>& a, const vector<double>& b){
    double c = 0;
    for (size_t i = 0; i < a.size(); ++i){
        c += a[i] * b[i];
    }
    return c;
}

vector<double> operator% (const vector<double>& a, const vector<double>& b){
    vector<double> c;
    c.push_back(a[1] * b[2] - a[2] * b[1]);
    c.push_back(a[2] * b[0] - a[0] * b[2]);
    c.push_back(a[0] * b[1] - a[1] * b[0]);
    return c;
}

bool operator|| (const vector<double>& a, const vector<double>& b){
    const double eps = 1e-7;
    
    for (size_t i = 1; i < a.size(); ++i){
        if (a[i] * b[0] - a[0] * b[i] > eps)
            return false;
    }
    return true;
}

bool operator&& (const vector<double>& a, const vector<double>& b){
    if ((a || b) && (a[0] * b[0]) >= 0)
        return true;
    return false;
}

std::istream& operator>> (std::istream &is, vector<double>& a){
    int n;
    is >> n;
    if (a.size() > 0)
        a.clear();
    for (size_t i = 0; i < n; ++ i) {
        double val;
        is >> val;
        a.push_back(val);
    }
    return is;
}

std::ostream& operator<< (std::ostream &os, vector<double>& a){
    for (size_t i = 0; i < a.size(); ++i){
        os << a[i] << " ";
    }
    os << '\n';
    return os;
}

void reverse (vector<double>& a){
    int n = a.size();
    double tmp;
    for (size_t i = 0; i < n / 2; ++i){
        tmp = a[i];
        a[i] = a[n - i - 1];
        a[n - i - 1] = tmp;
    }
}

vector<int> operator| (const vector<int>& a, const vector<int>& b){
    vector<int> c;
    for (size_t i = 0; i < a.size(); ++i){
        c.push_back(a[i] | b[i]);
    }
    return c;
}

vector<int> operator& (const vector<int>& a, const vector<int>& b){
    vector<int> c;
    for (size_t i = 0; i < a.size(); ++i){
        c.push_back(a[i] & b[i]);
    }
    return c;
}
}  // namespace task
