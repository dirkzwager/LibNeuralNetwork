#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <iostream>
#include <fstream>
#include <stdarg.h>
#include <cassert>
#include <cstdlib>
#include <math.h>

using namespace std;

typedef unsigned int uint;

template <class T> 
class Vector;

template <class T> std::ostream& operator<<(std::ostream&, const Vector<T>&);
template <class T> std::istream& operator>>(std::istream&, const Vector<T>&);

template <class T>
class Vector {
private:
  int dim;
  T* values;
  
  bool cstruct(T*& arr, uint N);
  bool dstruct(T*& arr);
public:
  Vector();
  Vector(const uint _dim);
  Vector(const uint _dim, const T init);
  Vector(const Vector& v);
  ~Vector();

  static Vector define(uint _dim, ...);
  void randomnize(const int max);
  void randomnize(const int& min, const int& max);
  const int& Dim() const;
  T Max() const;
  T Min() const;
  T Max(T bound) const;
  T Min(T bound) const;
  T Max(int& idx, T bound) const;
  T Min(int& idx, T bound) const;
  T Sum() const;
  void normalize();
  void softMax();

  T dotProduct(const Vector& v) const;
  T vectorLength() const;
  T vectorAngle(const Vector& v) const;
  Vector& unitVector();
  static T dotProduct(const Vector& v1, const Vector& v2);
  static T vectorLength(const Vector& v);
  static T vectorAngle(const Vector& a, const Vector& b);
  static Vector unitVector(const Vector& v);
  static Vector concatVector(const int n, Vector<T>* v);

  Vector  operator +  (const Vector&) const;
  Vector  operator -  (const Vector&) const;
  Vector  operator *  (const Vector&) const;
  Vector  operator /  (const Vector&) const;
  Vector& operator += (const Vector&);
  Vector& operator -= (const Vector&);
  Vector& operator *= (const Vector&);
  Vector& operator /= (const Vector&);
  Vector  operator +  (const T&) const;
  Vector  operator -  (const T&) const;
  Vector  operator *  (const T&) const;
  Vector  operator /  (const T&) const;
  Vector& operator += (const T&);
  Vector& operator -= (const T&);
  Vector& operator *= (const T&);
  Vector& operator /= (const T&);
  Vector& operator ++ ();//prefix
  Vector& operator -- ();//prefix
  T& operator() (const uint idx);
  const T& operator() (const uint idx) const;
  
  Vector& operator = (const Vector& v);
  Vector& operator = (const T val);
  bool operator == (const Vector& v) const;
  bool operator != (const Vector& v) const;
  bool operator <  (const Vector& v) const;
  bool operator >  (const Vector& v) const;
  bool operator <= (const Vector& v) const;
  bool operator >= (const Vector& v) const;
  friend ostream& operator << <T>(std::ostream&, const Vector<T>&);
  friend istream& operator >> <T>(std::istream&, const Vector<T>&);
};

//null constructor
template <class T>
Vector<T>::Vector() : dim(0), values(NULL) {  }

//alloc constructor
template <class T>
Vector<T>::Vector(const uint _dim) : dim((int)_dim), values(NULL) {
  cstruct(values,dim);
}

//init constructor
template <class T>
Vector<T>::Vector(const uint _dim, const T init) : dim((int)_dim), 
  values(NULL) {
  cstruct(values,dim);
  for (int i=0; i<dim; i++)
    values[i]=init;
}

//copy constructor
template <class T>
Vector<T>::Vector(const Vector<T>& v) : dim(v.dim), values(NULL) {
  cstruct(values,dim);
  for (int i=0; i<dim; i++)
    values[i] = v.values[i];
}

//destructor
template <class T>
Vector<T>::~Vector() {
  dstruct(values);
  dim = 0;
}

//memory allocator
template <class T>
bool Vector<T>::cstruct(T*& arr, uint N) {
  dstruct(arr);
  arr = new T[N];
  for (int i=0; i<dim; i++) 
    arr[i]=T();
  return true;
}

//memory deallocator
template <class T>
bool Vector<T>::dstruct(T*& arr) {
  if (arr) {
    delete[] arr;
    arr = NULL;
    return true;
  }
  return false;
}

//regular procedures

//vararg constructor
template <class T>
inline Vector<T> Vector<T>::define(const uint _dim, ...) {
  Vector<T> v(_dim);
  va_list ap;
  va_start(ap,_dim);
  for (int i=0; i<_dim; i++)
    v.values[i] = T(va_arg(ap,T));
  va_end(ap);
  return v;
}

template <class T>
void Vector<T>::randomnize(const int max) {
  for (int i=0; i<dim; i++)
    values[i] = T(rand()%max);
}

template <class T>
void Vector<T>::randomnize(const int& min, const int& max) {
  for (int i=0; i<dim; i++)
    values[i] = (T)(min+(rand()%(max-min+1)));
    //values[i] = min+(rand()%(max-min+1));
}

template <class T>
const int& Vector<T>::Dim() const {
  return dim;
}

template <class T>
T Vector<T>::Max() const {
  assert(dim > 0);
  T max = values[0];
  if (dim == 1) return max;

  for (int n=1; n<dim; n++)
    max = values[n] > max ? values[n] : max;
  return max;
}

template <class T>
T Vector<T>::Min() const {
  assert(dim > 0);
  T min = values[0];
  if (dim == 1) return min;

  for (int n=1; n<dim; n++)
    min = values[n] < min ? values[n] : min;
  return min;
}

//returns max(values) && < bound. if no vals < bound returns -1;
template <class T>
T Vector<T>::Max(T bound) const {
  if (bound == 0) return -1;
  int i=0;
  while (values[i] >= bound) i++;

  if (i == dim-1) return values[dim-1];
  if (i == dim)   return T(-1);
  T max = values[i];
  for (i+1; i<dim; i++) 
    max = values[i] > max && values[i] < bound ? values[i] : max;
  return max;
}

template <class T>
T Vector<T>::Min(T bound) const {
  int i=0;
  while (values[i] <= bound) i++;

  if (i == dim-1) return values[dim-1];
  if (i == dim)   return T(-1);
  T min = values[i];
  for (i+1; i<dim; i++) 
    min = values[i] < min && values[i] > bound ? values[i] : min;
  return min;
}

template <class T>
T Vector<T>::Max(int& idx, T bound) const {
  int i=0;
  
  if (bound != 0) {
    while (values[i] >= bound && i < dim) i++;
    
    if (i == dim-1) {
      idx = dim-1;
      return values[dim-1];
    }
    if (i == dim) {
      idx = -1;
      return T(-1);
    }
  }
  T max = values[i];
  idx = i;
  if (bound != 0) {
    for (i+1; i<dim; i++) 
      if (values[i] > max && values[i] < bound) {
	max = values[i];
	idx = i;
      }
  }
  else { 
     for (i+1; i<dim; i++) 
       if (values[i] > max) {
	 max = values[i];
	 idx = i;
       }
  }
  return max;   
}

template <class T>
T Vector<T>::Min(int& idx, T bound) const {
  int i=0;
  
  if (bound != 0) {
    while (values[i] <= bound && i < dim) i++;
    
    if (i == dim-1) {
      idx = dim-1;
      return values[dim-1];
    }
    if (i == dim) {
      idx = -1;
      return T(-1);
    }
  }
  T min = values[i];
  idx = i;
  if (bound != 0) {
    for (i+1; i<dim; i++) 
      if (values[i] < min && values[i] > bound) {
	min = values[i];
	idx = i;
      }
  }
  else { 
     for (i+1; i<dim; i++) 
       if (values[i] < min) {
	 min = values[i];
	 idx = i;
       }
  }
  return min;   
}

template <class T>
T Vector<T>::Sum() const {
  T sum = 0;
  
  for (int i=0; i<dim; i++)
    sum += values[i];
  return sum;
}

template <class T>
void Vector<T>::normalize() {
  const T min = Min();
  const T max = Max();
  const T dff = max-min;
  
  if (dff == 0) return;
  (*this) -= min;
  (*this) /= dff;
}

template <class T>
void Vector<T>::softMax() {
  T sum = 0;
  for (int i=0; i<dim; i++) {
    (*this)(i) = exp((*this)(i));
    sum += (*this)(i);
  }
  if (sum == (T)0) {
    (*this) = (T)0;
    return;
  }
  (*this) /= sum;
}

//algebraic operators

template <class T>
T Vector<T>::dotProduct(const Vector<T>& v) const {
  assert(dim==v.dim);
  T dot=T(0);
  for (int i=0; i<dim; i++)
    dot += (values[i]*v(i));
  return dot;
  
}

template <class T>
T Vector<T>::vectorLength() const {
  return sqrt(dotProduct(*this));
}

template <class T>
T Vector<T>::vectorAngle(const Vector<T>& v) const {
  const double dotab = (double)dotProduct(v);
  const double magab = (double)vectorLength() * v.vectorLength();
  if (magab == 0.0) return (T)(M_PI/2.0);
  return (T)acos(dotab/magab);  
}

template <class T>
Vector<T>& Vector<T>::unitVector() {
  T len = vectorLength();
  if (len > T(0))
    for (int i=0; i<dim; i++)
      values[i]/=len;
  return *this;
}

template <class T>
T Vector<T>::dotProduct(const Vector<T>& v1, const Vector<T>& v2) {
  return v1.dotProduct(v2);
}

template <class T>
T Vector<T>::vectorLength(const Vector<T>& v) {
  return v.vectorLength();
}

template <class T>
T Vector<T>::vectorAngle(const Vector<T>& a, const Vector<T>& b) {
  const double dotab = (double)a.dotProduct(b);
  const double magab = (double)a.vectorLength() * b.vectorLength();
  if (magab == 0.0) return (T)(M_PI/2.0);
  return (T)acos(dotab/magab);  
}

template <class T>
Vector<T> Vector<T>::unitVector(const Vector<T>& v) {
  Vector<T> cpy = v;
  return cpy.unitVector();
}

template <class T>
Vector<T> Vector<T>::concatVector(const int n, Vector<T>* v) {
  int ndim = v[0].Dim();
  for (int i=0; i<n; i++)
    ndim += v[i].Dim();
  
  Vector<T> cat(ndim);
  
  int k=0;
  for (int i=0; i<n; i++)
    for (int j=0; j<v[i].Dim(); j++)
      cat(k++) = v[i](j);
  return cat;
}

//arithmic operators

template <class T>
Vector<T>  Vector<T>::operator + (const Vector<T>& v) const {
  assert(dim == v.dim);
  Vector<T> ans(dim);
  for (int i=0; i<dim; i++)
    ans.values[i] = values[i] + v.values[i];
  return ans;
}

template <class T>
Vector<T>  Vector<T>::operator - (const Vector<T>& v) const {
  assert(dim == v.dim);
  Vector<T> ans(dim);
  for (int i=0; i<dim; i++)
    ans.values[i] = values[i] - v.values[i];
  return ans;
}

template <class T>
Vector<T>  Vector<T>::operator * (const Vector<T>& v) const {
  assert(dim == v.dim);
  Vector<T> ans(dim);
  for (int i=0; i<dim; i++)
    ans.values[i] = values[i] * v.values[i];
  return ans;
}

template <class T>
Vector<T>  Vector<T>::operator / (const Vector<T>& v) const {
  assert(dim == v.dim);
  Vector<T> ans(dim);
  for (int i=0; i<dim; i++)
    ans.values[i] = values[i] / v.values[i];
  return ans;
}

template <class T>
Vector<T>& Vector<T>::operator += (const Vector<T>& v) {
  assert(dim == v.dim);
  for (int i=0; i<dim; i++)
    values[i] += v.values[i];
  return *this;
}

template <class T>
Vector<T>& Vector<T>::operator -= (const Vector<T>& v) {
  assert(dim == v.dim);
  for (int i=0; i<dim; i++)
    values[i] -= v.values[i];
  return *this;
}

template <class T>
Vector<T>& Vector<T>::operator *= (const Vector<T>& v) {
  assert(dim == v.dim);
  for (int i=0; i<dim; i++)
    values[i] *= v.values[i];
  return *this;
}

template <class T>
Vector<T>& Vector<T>::operator /= (const Vector<T>& v) {
  assert(dim == v.dim);
  for (int i=0; i<dim; i++)
    values[i] /= v.values[i];
  return *this;
}

template <class T>
Vector<T> Vector<T>::operator + (const T& x)  const {
  Vector<T> ans(dim);
  for (int i=0; i<dim; i++)
    ans(i) = (values[i] + x);
  return ans;
}

template <class T>
Vector<T>  Vector<T>::operator -  (const T& x) const {
  Vector<T> ans(dim);
  for (int i=0; i<dim; i++)
    ans(i) = values[i] - x;
  return ans;
}

template <class T>
Vector<T>  Vector<T>::operator *  (const T& x) const {
  Vector<T> ans(dim);
  for (int i=0; i<dim; i++)
    ans(i) = values[i] * x;
  return ans;
}

template <class T>
Vector<T> Vector<T>::operator /  (const T& x) const {
  assert(x!=0);
  Vector<T> ans(dim);
  for (int i=0; i<dim; i++)
    ans(i) = values[i] / x;
  return ans;
}

template <class T>
Vector<T>& Vector<T>::operator += (const T& x) {
  for (int i=0; i<dim; i++)
    values[i]+=x;
  return *this;
}

template <class T>
Vector<T>& Vector<T>::operator -= (const T& x) {
  for (int i=0; i<dim; i++)
    values[i]-=x;
  return *this;
}

template <class T>
Vector<T>& Vector<T>::operator *= (const T& x) {
  for (int i=0; i<dim; i++)
    values[i]*=x;
  return *this;
}

template <class T>
Vector<T>& Vector<T>::operator /= (const T& x) {
  assert(x!=T(0));
  for (int i=0; i<dim; i++)
    values[i]/=x;
  return *this;
}

template <class T>
Vector<T>& Vector<T>::operator ++ () {
  for (int i=0; i<dim; i++)
    values[i]++;
  return *this;
}

template <class T>
Vector<T>& Vector<T>::operator -- () {
  for (int i=0; i<dim; i++)
    values[i]--;
  return *this;
}

//assignment operator
template <class T>
Vector<T>& Vector<T>::operator = (const Vector<T>& v) {
  if (dim != v.dim) {
    dim = v.dim;
    cstruct(values,v.dim);
  }
  for (int i=0; i<dim; i++)
    values[i] = v.values[i]; 
  return *this;
}

//init assignment operator
template <class T>
Vector<T>& Vector<T>::operator = (const T val) {
  //cout << " = val " << endl;
  for (int i=0; i<dim; i++)
    values[i] = val;
  return *this;
}

//selector
template <class T>
T& Vector<T>::operator() (const uint idx) {
  assert(idx < dim);
  return values[idx];
}

template <class T>
const T& Vector<T>::operator() (const uint idx) const {
  assert(idx < dim);
  return values[idx];
}

//predicates 

template <class T>
bool Vector<T>::operator == (const Vector<T>& v) const {
  if (dim != v.dim) return false;
  for (int i=0; i<dim; i++) 
    if (values[i] != v.values[i]) return false;
  return true;
}

template <class T>
bool Vector<T>::operator != (const Vector<T>& v) const {
  return !((*this) == v);
}

template <class T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& v) {
  for (int i=0; i<v.dim; i++) {
    os.width(1);
    os << v.values[i] << " ";
  }
  return os;
}

template <class T>
std::istream& operator>>(std::istream& is, const Vector<T>& v) {
  for (int i=0; i<v.dim; i++) {
    is >> v.values[i];
  }
  return is;
}

template <class T>
bool Vector<T>::operator <  (const Vector& v) const {
  return vectorLength() < v.vectorLength();
}

template <class T>
bool Vector<T>::operator >  (const Vector& v) const { 
  return vectorLength() > v.vectorLength();
}

template <class T>
bool Vector<T>::operator <= (const Vector& v) const {
  return vectorLength() <= v.vectorLength();
}

template <class T>
bool Vector<T>::operator >= (const Vector& v) const {
  return vectorLength() >= v.vectorLength();
}



#endif
