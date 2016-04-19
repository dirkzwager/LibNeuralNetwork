#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "Vector.h"

#include <iostream>
#include <fstream>
#include <stdarg.h>
#include <cassert>
#include <cstdlib>

using namespace std;

typedef unsigned int uint;

template <class T> class Matrix;
template <class T> class Vector;

template <class T> std::ostream& operator<<(std::ostream&, const Matrix<T>&);

typedef Matrix<Vector<double> > VectorMatrix;

template <class T>
class Matrix {
 protected:
  uint dimx, dimy;
  T* values;
  
  bool cstruct(T*& arr, uint N);
  bool dstruct(T*& arr);
public:
  Matrix();
  Matrix(const uint _dimx, const uint _dimy);
  Matrix(const uint _dimx, const uint _dimy, const T init);
  Matrix(const Matrix& v);
  ~Matrix();

  static Matrix define(uint _dimx, uint _dimy, ...);
  void randomnize(const int max);
  void normalize(const T& min=0, const T& max=1);
  int Dimx() const;
  int Dimy() const;
  T Max() const;
  T Min() const;
  void Col(Vector<T>& col, uint idx) const;
  void Row(Vector<T>& row, uint idy) const;  

  Matrix  operator + (const Matrix&);
  Matrix  operator - (const Matrix&);
  Matrix  operator * (const Matrix&);
  Matrix  operator / (const Matrix&);
  Matrix& operator += (const Matrix&);
  Matrix& operator -= (const Matrix&);
  Matrix& operator *= (const Matrix&);
  Matrix& operator /= (const Matrix&);
  Matrix  operator +  (const T&);
  Matrix  operator -  (const T&);
  Matrix  operator *  (const T&);
  Matrix  operator /  (const T&);
  Matrix& operator += (const T&);
  Matrix& operator -= (const T&);
  Matrix& operator *= (const T&);
  Matrix& operator /= (const T&);
  Matrix& operator ++ ();//prefix
  Matrix& operator -- ();//prefix
  T& operator() (const uint idx, const uint idy);
  const T& operator() (const uint idx, const uint idy) const;
  
  Matrix& operator = (const Matrix& v);
  Matrix& operator = (const T val);
  bool operator == (const Matrix& v);
  bool operator != (const Matrix& v);
  friend ostream& operator << <T>(std::ostream&, const Matrix<T>&);

  static VectorMatrix readPixmap(const string& fname);
  Vector<T> flatten() const;
};

//null constructor
template <class T>
Matrix<T>::Matrix() : dimx(0), dimy(0), values(NULL) {}

//alloc constructor
template <class T>
Matrix<T>::Matrix(const uint _dimx, const uint _dimy) 
: dimx(_dimx), dimy(_dimy), values(NULL) {
  const int N = dimx*dimy;
  cstruct(values,N);
}

//init constructor
template <class T>
Matrix<T>::Matrix(const uint _dimx, const uint _dimy, const T init) 
: dimx(_dimx), dimy(_dimy), values(new T[_dimx*_dimy]) 
{
  const int N = dimx*dimy;
  for (int i=0; i<N; i++) 
    values[i]=init;
}

//copy constructor
template <class T>
Matrix<T>::Matrix(const Matrix<T>& m) 
: dimx(m.dimx), dimy(m.dimy), values(NULL) {
  const int N = m.dimx*m.dimy;
  cstruct(values,N);
  for (int i=0; i<N; i++)
    values[i] = m.values[i];
}

//destructor
template <class T>
Matrix<T>::~Matrix() {
  dstruct(values);
  dimx = dimy = 0;
}

//memory allocator
template <class T>
bool Matrix<T>::cstruct(T*& arr, uint N) {
  dstruct(arr);
  arr = new T[N];
  for (int i=0; i<N; i++) 
    arr[i]=T();
  return true;
}

//memory deallocator
template <class T>
bool Matrix<T>::dstruct(T*& arr) {
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
inline Matrix<T> Matrix<T>::define(const uint _dimx, uint _dimy, ...) {
  const int N = _dimx*_dimy;
  Matrix<T> v(_dimx,_dimy);
  va_list ap;
  va_start(ap,_dimy);
  for (int i=0; i<N; i++)
    v.values[i] = va_arg(ap,T);
  va_end(ap);
  return v;
}

template <class T>
void Matrix<T>::randomnize(const int max) {
  const int N = dimx*dimy;
  for (int i=0; i<N; i++)
    values[i] = rand()%max;
}

template <class T>
void Matrix<T>::normalize(const T& nmin, const T& nmax) {
  const int N = dimx*dimy;
  const T nrange = nmax-nmin;

  if (nrange == 0.0) {
    cout << "error, invalid nrange" << endl;
    return;
  }

  const T omin = Min();
  const T omax = Max();
  const T orange = omax-omin;
  const T drange = nrange/orange;

  for (int i=0; i<N; i++)
    values[i] = (values[i]-omin)*drange + nmin;
}

template <class T>
int Matrix<T>::Dimx() const {
  return dimx;
}

template <class T>
int Matrix<T>::Dimy() const {
  return dimy;
}

template <class T>
T Matrix<T>::Max() const {
  const int N = dimx*dimy;
  assert(N > 0);
  T max = values[0];
  if (N == 1) return max;

  for (int n=1; n<N; n++)
    max = values[n] > max ? values[n] : max;
  return max;
}

template <class T>
T Matrix<T>::Min() const {
  const int N = dimx*dimy;
  assert(N > 0);
  T min = values[0];
  if (N == 1) return min;

  for (int n=1; n<N; n++)
    min = values[n] < min ? values[n] : min;
  return min;
}

template <class T>
void Matrix<T>::Col(Vector<T>& col, uint idx) const {
  if (col.Dim() != dimy)
    col = Vector<T>(dimy);
  for (int j=0; j<dimy; j++)
    col(j) = values[j*dimx+idx];
}

template <class T>
void Matrix<T>::Row(Vector<T>& row, uint idy) const {
  if (row.Dim() != dimx)
    row = Vector<T>(dimx);
  for (int i=0; i<dimx; i++)
    row(i) = values[idy*dimx+i];
}

//arithmic operators

template <class T>
Matrix<T>  Matrix<T>::operator + (const Matrix<T>& v) {
  assert(dimx == v.dimx && dimy == v.dimy);
  const int N = dimx*dimy;
  Matrix<T> ans(dimx,dimy);
  for (int i=0; i<N; i++)
    ans.values[i] = values[i] + v.values[i];
  return ans;
}

template <class T>
Matrix<T>  Matrix<T>::operator - (const Matrix<T>& v) {
  assert(dimx == v.dimx && dimy == v.dimy);
  const int N = dimx*dimy;
  Matrix<T> ans(dimx,dimy);
  for (int i=0; i<N; i++)
    ans.values[i] = values[i] - v.values[i];
  return ans;
}

template <class T>
Matrix<T>  Matrix<T>::operator * (const Matrix<T>& v) {
  assert(dimx == v.dimx && dimy == v.dimy);
  const int N = dimx*dimy;
  Matrix<T> ans(dimx,dimy);
  for (int i=0; i<N; i++)
    ans.values[i] = values[i] * v.values[i];
  return ans;
}

template <class T>
Matrix<T>  Matrix<T>::operator / (const Matrix<T>& v) {
  assert(dimx == v.dimx && dimy == v.dimy);
  const int N = dimx*dimy;
  Matrix<T> ans(dimx,dimy);
  for (int i=0; i<N; i++)
    ans.values[i] = values[i] / v.values[i];
  return ans;
}

template <class T>
Matrix<T>& Matrix<T>::operator += (const Matrix<T>& v) {
  assert(dimx == v.dimx && dimy == v.dimy);
  const int N = dimx*dimy;
  for (int i=0; i<N; i++)
    values[i] += v.values[i];
  return *this;
}

template <class T>
Matrix<T>& Matrix<T>::operator -= (const Matrix<T>& v) {
  assert(dimx == v.dimx && dimy == v.dimy);
  const int N = dimx*dimy;
  for (int i=0; i<N; i++)
    values[i] -= v.values[i];
  return *this;
}

template <class T>
Matrix<T>& Matrix<T>::operator *= (const Matrix<T>& v) {
  assert(dimx == v.dimx && dimy == v.dimy);
  const int N = dimx*dimy;
  for (int i=0; i<N; i++)
    values[i] *= v.values[i];
  return *this;
}

template <class T>
Matrix<T>& Matrix<T>::operator /= (const Matrix<T>& v) {
  assert(dimx == v.dimx && dimy == v.dimy);
  const int N = dimx*dimy;
  for (int i=0; i<N; i++)
    values[i] /= v.values[i];
  return *this;
}

template <class T>
Matrix<T> Matrix<T>::operator + (const T& x)  {
  const int N = dimx*dimy;
  Matrix<T> ans(N);
  for (int i=0; i<N; i++)
    ans(i) = (values[i] + x);
  return ans;
}

template <class T>
Matrix<T>  Matrix<T>::operator -  (const T& x) {
  const int N = dimx*dimy;
  Matrix<T> ans(N);
  for (int i=0; i<N; i++)
    ans(i) = values[i] - x;
  return ans;
}

template <class T>
Matrix<T>  Matrix<T>::operator *  (const T& x) {
  const int N = dimx*dimy;
  Matrix<T> ans(N);
  for (int i=0; i<N; i++)
    ans(i) = values[i] * x;
  return ans;
}

template <class T>
Matrix<T> Matrix<T>::operator /  (const T& x) {
  const int N = dimx*dimy;
  assert(x!=0);
  Matrix<T> ans(N);
  for (int i=0; i<N; i++)
    ans(i) = values[i] / x;
  return ans;
}

template <class T>
Matrix<T>& Matrix<T>::operator += (const T& x) {
  const int N = dimx*dimy;
  for (int i=0; i<N; i++)
    values[i]+=x;
  return *this;
}

template <class T>
Matrix<T>& Matrix<T>::operator -= (const T& x) {
  const int N = dimx*dimy;
  for (int i=0; i<N; i++)
    values[i]-=x;
  return *this;
}

template <class T>
Matrix<T>& Matrix<T>::operator *= (const T& x) {
  const int N = dimx*dimy;
  for (int i=0; i<N; i++)
    values[i]*=x;
  return *this;
}

template <class T>
Matrix<T>& Matrix<T>::operator /= (const T& x) {
  const int N = dimx*dimy;
  assert(x!=T(0));
  for (int i=0; i<N; i++)
    values[i]/=x;
  return *this;
}

template <class T>
Matrix<T>& Matrix<T>::operator ++ () {
  const int N = dimx*dimy;
  for (int i=0; i<N; i++)
    values[i]++;
  return *this;
}

template <class T>
Matrix<T>& Matrix<T>::operator -- () {
  const int N = dimx*dimy;
  for (int i=0; i<N; i++)
    values[i]--;
  return *this;
}

//assignment operator
template <class T>
Matrix<T>& Matrix<T>::operator = (const Matrix<T>& m) {
  if (dimx != m.dimx || dimy != m.dimy) {
    dimx = m.dimx; dimy = m.dimy;
    cstruct(values,dimx*dimy);
  }
  const int N = m.dimx*m.dimy;
  for (int i=0; i<N; i++)
    values[i] = m.values[i];  
  return *this;
}

//init assignment operator
template <class T>
Matrix<T>& Matrix<T>::operator = (const T val) {
  const int N = dimx*dimy;
  for (int i=0; i<N; i++)
    values[i] = val;
  return *this;
}

//selector
template <class T>
T& Matrix<T>::operator() (const uint idx, const uint idy) {
  assert(idx < dimx && idy < dimy);
  return values[idy*dimx+idx];
}

template <class T>
const T& Matrix<T>::operator() (const uint idx, const uint idy) const {
  assert(idx < dimx && idy < dimy);
  return values[idy*dimx+idx];
}

//predicates 

template <class T>
bool Matrix<T>::operator == (const Matrix<T>& v) {
  const int N = dimx*dimy;
  if (dimx != v.dimx || dimy != v.dimy) return false;
  for (int i=0; i<N; i++) 
    if (values[i] != v.values[i]) return false;
  return true;
}

template <class T>
bool Matrix<T>::operator != (const Matrix<T>& v) {
  return !((*this) == v);
}

template <class T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& v) {
  for (int j=0; j<v.dimy; j++) {
    for (int i=0; i<v.dimx; i++) {
      os.width(3);
      os << v.values[j*v.dimx+i] << " ";
    }
    os << endl;
  }
  return os;
}

template <class T>
Vector<T> Matrix<T>::flatten() const {
  const int N = dimx*dimy;
  Vector<T> fv(N);

  int tN = N;
  for (int i=0; i<N; i++)
    fv(i) = values[i];
  return fv;
}

template <class T>
VectorMatrix Matrix<T>::readPixmap(const string& fname) {
  int dmx, dmy, dmz, max;
  VectorMatrix pixmap;

  string filecode, s;
  ifstream is(fname.c_str());

  is >> filecode >> s;

  if (filecode == "P2")
    dmz = 1;
  else if (filecode == "P3")
    dmz = 3;
  else {
    cout << "Unknown file identifier: " << filecode << endl;
    return pixmap;
  }

  if (s=="#") 
    is.ignore(256,'\n');
  
  is >> dmx >> dmy >> max;

  cout << "Found " << dmx 
       << "x" << dmy 
       << "x" << dmz 
       << " image" << endl;

  pixmap = VectorMatrix(dmx,dmy);
  pixmap = Vector<double>(dmz);

  for (int j=0; j<dmy; j++)
    for (int i=0; i<dmx; i++) 
      for (int k=0; k<dmz; k++)
	is >> pixmap(i,j)(k);

  is.close();
  return pixmap;
}


#endif 
