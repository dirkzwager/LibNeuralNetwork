#ifndef __GNUPLOT_H__
#define __GNUPLOT_H__

#include <iostream>

using namespace std;

class GnuPlot {
  FILE* pipe;
 public:
  GnuPlot();
  ~GnuPlot();
  void writeExpr(const string& expr);
};

#endif 
