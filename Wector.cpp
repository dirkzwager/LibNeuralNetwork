#include "Wector.h"

Wector::Wector() : Vector< Vector<double> >()  {}

Wector::Wector(const int& n, const int& w) : Vector< Vector<double> >(n) {
  (*((Vector< Vector<double> >*)this)) = Vector<double>(w);
}

Wector::Wector(const Wector& wv) : Vector< Vector<double> >(wv) {}

Wector::~Wector() {}

void Wector::randomnize(const int min, const int max) {
  for (int i=0; i<numWeights(); i++) {
    const int k=rand()%10;
    for (int m=0; m<k; m++)
      rand()%2;
    (*this)(i).randomnize(min,max);
    (*this)(i).unitVector();
  }
  /*
  const int range = max-min;

  for (int i=0; i<numWeights(); i++) {
    for (int j=0; j<weightDim(i); j++) {
      int rnd = min+rand()%(range+1);
      (*this)(i)(j) = double(rnd);
    }
    //(*this)(i).unitVector();
  }
  */
}

Wector& Wector::operator = (const Wector& wv) {
  (*((Vector< Vector<double> >*)this)) = ((Vector< Vector<double> >&)wv);
  return *this;
}

Vector<double>& Wector::operator () (const int n) {
  return (*(Vector<Vector <double> >*)this)(n);
}

const Vector<double>& Wector::operator () (const int n) const {
  return (*(Vector<Vector <double> >*)this)(n);
}

const int& Wector::numWeights() const {
  return Dim();
}

const int& Wector::weightDim(const int& i) const {
  return (*this)(i).Dim();
}

ostream& operator << (ostream& os, const Wector& wv) {
  for (int j=0; j<wv.weightDim(0); j++) {
    for (int i=0; i<wv.numWeights(); i++) 
      os << wv(i)(j) << " ";
    os << endl;
  }
  return os;
}
