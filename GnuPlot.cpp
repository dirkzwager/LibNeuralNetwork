#include "GnuPlot.h"

GnuPlot::GnuPlot() : pipe(NULL) {
  pipe = popen("gnuplot -persist","w");
}

GnuPlot::~GnuPlot() {
  pclose(pipe);
}

void GnuPlot::writeExpr(const string& expr) {
  fprintf(pipe,(expr + "\n").c_str());
}
