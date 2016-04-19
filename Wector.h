#ifndef __WECTOR_H__
#define __WECTOR_H__

#include "Vector.h"

class Wector;

ostream& operator << (ostream&, const Wector&);

class Wector : public Vector< Vector<double> > {
 private:
 public:
  Wector();
  Wector(const int& n, const int& w);
  Wector(const Wector& wv);
  ~Wector();

  const int& numWeights() const;
  const int& weightDim(const int& i) const;
  void randomnize(const int min=0, const int max=100000);
  Wector& operator = (const Wector& wv);
  Vector<double>& operator () (const int n);
  const Vector<double>& operator () (const int n) const;
  friend ostream& operator << (ostream&, const Wector&);
};

#endif
