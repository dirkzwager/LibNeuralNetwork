/*

  main.cpp sets up 2 training schemas for the static and temporal xor problem. 
  File is self contained in the sense that it takes no arguments, patterns 
  are generated automatically for this purpose.
  
  training consists of 600 epochs on sequences of 2000 patterns (3000 bits xorbits).
  classification is based on sequences of 20 patterns (30 xorbits).

  To compile under linux type: make
  to run type: ./xor

  *Only dependency is gnuplot which is used to plot the results.
  
  Execution produces 6 files:
  trn.dat      - 3000 xorbits.
  log.dat      - training error.
  cls.dat      - classification performance.
  tst.dat      - 30 xorbits.
  trn_bitf.dat - 2000 random bits.
  tst_bitf.dat - 20 random bits.
  srn.w        - weights of the srn.
  bpn.w        - weights of the bpn.

*/

#include <iostream>
#include "RecurrentNetwork.h"
#include "BackpropNetwork.h"
#include "GnuPlot.h"
#include <sstream>

using namespace std;

//forward declaration of some auxilary functions.
string double2str(double d);
int XOR(int a, int b); 
int OR(int a, int b);
void resetStream(ifstream& s); 
int cntPatterns(ifstream& s);
void mkXORfile(const int n, const string& fname);
void mkORfile(const int n, const string& fname);
void xorBitFile(const string& iname, const string& oname);
void orBitFile(const string& iname, const string& oname);
void randomBitFile(const string& fname, const int& nbits);

//######################################################################################
//temporal XOR problem
//######################################################################################

//generate a sequence of patterns with a simple recurrent network by feeding
//the output it generates back to itself.
double genSrn(RecurrentNetwork& srn, const double& start) {
  Vector<double> pat(1);
  Vector<double> tar(1);

  double cnt = 0;
  pat(0) = start;
  while (cnt++ < 100) {      
    srn.setPattern(pat,tar);
    srn.propForward();
    pat = srn.output();
    cout << "i: " << pat << endl;
  }
  return 0;
}

//Average the results of feeding a srn a large number of temporal patterns. 
//writes the averaged sum square error to a file.
void avgError(RecurrentNetwork& srn, int epochs, int patterns) {
  Vector<double> pat(1);
  Vector<double> tar(1);
  Vector<double> avg(patterns+patterns/2-1);//accumulator bin.

  //temporary files.
  string bitstr = "/tmp/avgtmp.bit";
  string xorstr = "/tmp/avgtmp.pat";

  double error = 0;
  for (int e=0; e<epochs; e++) {
    //new random pattern file.
    randomBitFile(bitstr,patterns);
    xorBitFile(bitstr,xorstr);

    ifstream is(xorstr.c_str());
    int cnt = 0;
    is >> pat >> tar;
    //feed file.
    while (!is.eof()) {      
      srn.setPattern(pat,tar);
      //prop pattern forward through the network.
      srn.propForward();
      //prop the error gradient back through the network.
      srn.propBackward(error);
      avg(cnt) += error;
      pat = tar;
      is >> tar;
      cnt++;
    }
    is.close();
  }
  //average vector.
  avg/=double(epochs);

  //write results to avgoff.dat
  ofstream avgoff("avgoff.dat");
  for (int i=0; i<avg.Dim()-1; i++) 
    avgoff << i << " " << avg(i) << endl;
  avgoff.close();
  cout << "avgmin: " << avg.Min() << " avgmax: " << avg.Max() << endl;
}

//test a srn on a pattern sequence and write the results to a file.
double tstSrn(RecurrentNetwork& srn, const string& tstfile, const string& logfile) {
  ifstream is(tstfile.c_str());
  
  if (!is.good()) {
    cout << "Unable to locate tstfile" << endl;
    return -1;
  }
  
  ofstream os(logfile.c_str()); 
  
  Vector<double> pat(1);
  Vector<double> tar(1);

  double error = 0, tot_error = 0;
  double cnt = 0;

  is >> pat >> tar;
  while (!is.eof()) {      
    srn.setPattern(pat,tar);
    //prop pattern forward through the network.
    srn.propForward();
    //prop the error gradient back through the network.
    srn.propBackward(error);
    tot_error += error;
    //write to file
    os << cnt++ << " " 
       << pat << " " 
       << tar << " " 
       << error << " " 
       << srn.output() << endl;
    pat = tar;
    is >> tar;
  }
  is.close();
  os.close();
  return tot_error/cnt;
}


//test a srn on a pattern sequence without writing the results to a file.
double tstSrn(RecurrentNetwork& srn, const string& tstfile) {
  ifstream is(tstfile.c_str());
  
  if (!is.good()) {
    cout << "Unable to locate tstfile" << endl;
    return -1;
  }
  
  Vector<double> pat(1);
  Vector<double> tar(1);

  double error = 0, tot_error = 0;
  double cnt = 0;

  is >> pat >> tar;
  while (!is.eof()) {      
    srn.setPattern(pat,tar);
    srn.setPattern(pat,tar);
    //prop pattern forward through the network.
    srn.propForward();
    //prop the error gradient back through the network.
    srn.propBackward(error);
    tot_error += error;
    pat = tar;
    is >> tar;
    cnt++;
  }
  is.close();
  return tot_error/cnt;
}

//train a srn on the temporal xor-problem for x epochs.
void trainSrn(RecurrentNetwork& srn, const int& epochs, 
	      const string& trnfile, const string& logfile, double& avg_error) {
  const double a0 = .9;//begin value of the learning rate.
  const double a1 = .0;//end value of the learning rate.

  ifstream is(trnfile.c_str());
  
  if (!is.good()) {
    cout << "Unable to locate trnfile" << endl;
    return;
  }
  
  ofstream os(logfile.c_str()); 
  
  Vector<double> pat(1);
  Vector<double> tar(1);

  int n_patterns = -1;

  while (is >> pat) n_patterns++;
  resetStream(is);

  const int tot_n_patterns = n_patterns*epochs;
  const double da = (a0-a1)/(tot_n_patterns-1);
  
  double a = a0;
  double error = 0;
  double cnt = 0;

  os << 0 << " " << tstSrn(srn,trnfile)  << endl;

  for (int e=1; e<=epochs; e++) {
    is >> pat >> tar;
    while (!is.eof()) {   
      srn.setPattern(pat,tar);
      //prop pattern forward through the network.
      srn.propForward();
      //prop the error gradient back through the network.
      srn.propBackward(error);
      //update weights with lrate = a.
      srn.updateWeights(a);
      avg_error += error;
      //update lrate.
      if ((a-da) > a1) 
	a = a-da;
      else a = a1;
      cnt++;
      pat = tar;
      is >> tar;
    }
    avg_error /= double(n_patterns);
    os << e << " " << avg_error << endl;
    resetStream(is);
  }
  is.close();
  os.close();
}

void initSrnTraining(const int& nepochs, const int& ppepoch,
		     const string& trn_file, 
		     const string& cls_file, 
		     const string& tst_file,
		     const string& log_file) {

  
  double avg_error = 0;
  RecurrentNetwork srn;
  //setup a network with 1 input unit, 2 hidden units and 1 output unit
  srn.setupNet(1,2,1);
  srn.randomnizeWeights();

  double init_error = tstSrn(srn,tst_file);//random performance

  //train network.
  trainSrn(srn,nepochs,trn_file,log_file,avg_error);
  srn.saveWeights("srn.w");

  //compute classification performance.
  double cls_error = tstSrn(srn,tst_file,cls_file);

  //present 20 patterns for 1000 times and average result. 
  avgError(srn,1000,20);
  
  //plot training error with gnuplot.
  GnuPlot logplot;
  logplot.writeExpr("set xlabel 'Epoch'");
  logplot.writeExpr("set ylabel 'Sum Square Error'");
  logplot.writeExpr("plot '" + log_file + "' using 1:2 title 'Average Epoch SSE' with lines");
  //plot classification error with gnuplot.
  GnuPlot clsplot;  
  clsplot.writeExpr("set border 2");
  clsplot.writeExpr("set pointsize 2");
  clsplot.writeExpr("set xtic 3");
  clsplot.writeExpr("set grid");
  clsplot.writeExpr("set xlabel 'Pattern'");
  clsplot.writeExpr("plot '" + cls_file + "' using 1:2 title 't', '" + cls_file + "' using 1:4 title 'SSE' with lines, '" + cls_file + "' using 1:5 title 'f(t)' with lines");

  cout << "init_error: " << init_error << endl << " cls_error: " << cls_error << endl;
}

//######################################################################################
//static XOR problem
//######################################################################################

//test a bpn on the static XOR problem and write result to file.
double tstBpn(BackpropNetwork& bpn, const string& tstfile, const string& logfile) {
  ifstream is(tstfile.c_str());

  if (!is.good()) {
    cout << "Unable to locate tstfile" << endl;
    return -1;
  }

  ofstream os(logfile.c_str()); 

  Vector<double> pat(2);
  Vector<double> tar(1);

  double error = 0, tot_error = 0;

  int i=0;
  double cnt = 0;
  while (is >> pat >> tar) {
      bpn.setPattern(pat,tar);
      //prop pattern forward through the network.
      bpn.propForward();
      //prop the error gradient back through the network.
      bpn.propBackward(error);
      tot_error += error;
      //write to file.
      os << cnt++ << " " 
	 << pat << " " 
	 << tar << " " 
	 << error << " " 
	 << bpn.readOut() << endl;
  }

  is.close();
  os.close();
  return tot_error/cnt;
}

//test a bpn on the static XOR problem and do not write result to file.
double tstBpn(BackpropNetwork& bpn, const string& tstfile) {
  ifstream is(tstfile.c_str());

  if (!is.good()) {
    cout << "Unable to locate tstfile" << endl;
    return -1;
  }

  Vector<double> pat(2);
  Vector<double> tar(1);

  double error = 0, tot_error = 0;

  int i=0;
  double cnt = 0;
  while (is >> pat >> tar) {
      bpn.setPattern(pat,tar);
      //prop pattern forward through the network.
      bpn.propForward();
      //prop the error gradient back through the network.
      bpn.propBackward(error);
      tot_error += error;
      cnt++;
  }
  is.close();
  return tot_error/cnt;
}

//train a bpn on the static XOR problem for x epochs.
void trainBpn(BackpropNetwork& bpn, const int& epochs, 
	      const string& trnfile, const string& logfile, double& avg_error) {
  ifstream is(trnfile.c_str());

  if (!is.good()) {
    cout << "Unable to locate trnfile" << endl;
    return;
  }

  ofstream os(logfile.c_str()); 

  Vector<double> pat(2);
  Vector<double> tar(1);

  int n_patterns = 0;

  while (is >> pat >> tar) n_patterns++;

  resetStream(is);

  const int tot_n_patterns = n_patterns*epochs;

  //just lrate from 1 to 0.
  const double da = 1.0/(tot_n_patterns-1);
  double a = 1.0;

  double error = 0;
  double cnt = 0;

  os << 0 << " " << tstBpn(bpn,trnfile) << endl;

  for (int e=1; e<epochs; e++) { 
    while (is >> pat >> tar) {
      bpn.setPattern(pat,tar);
      //prop pattern forward through the network.
      bpn.propForward();
      //prop the error gradient back through the network.
      bpn.propBackward(error);
      //update weights according to lrate = a.
      bpn.updateWeights(a);
      avg_error += error;
      //update lrate.
      if ((a-da) > 0) 
	a = a-da;
      else a = 0;
      cnt++;
    }
    avg_error /= double(n_patterns);
    os << e << " " << avg_error << endl;
    resetStream(is);
  }
  is.close();
  os.close();
}

//setup bpn network for the static XOR problem.
void initBpnTraining(const int& nepochs, const int& ppepoch,
		     const string& trn_file, 
		     const string& cls_file, 
		     const string& tst_file,
		     const string& log_file) {
  double avg_error = 0;
  BackpropNetwork  bpn;
  //setup a network with 2 input units 2 hidden units and 1 output unit.
  bpn.setupNet(2,2,1);
  bpn.randomnizeWeights();

  //train the network.
  trainBpn(bpn,nepochs,trn_file,log_file,avg_error);
  bpn.saveWeights("bpn.w");

  //test performance.
  double cls_error = tstBpn(bpn,tst_file,cls_file);
  string cerr = double2str(cls_error);
  string serr = double2str(avg_error);

  //plot training error to gnuplot.
  GnuPlot logplot;
  logplot.writeExpr("set title 'Figure 3 - mlp training performance (" + serr + ")'");
  logplot.writeExpr("set xlabel 'Epoch (" + double2str((double)ppepoch) + ")'");
  logplot.writeExpr("set ylabel 'Mean Square Error'");
  logplot.writeExpr("plot '" + log_file + "' using 1:2 title 'Average Epoch MSE' with lines"); 

  //plot classification performance to gnuplot.
  GnuPlot clsplot;
  clsplot.writeExpr("set title 'Figure 5 - mlp classification performance (" + cerr + ")'");
  clsplot.writeExpr("set border 2");
  clsplot.writeExpr("set pointsize 2");
  clsplot.writeExpr("set xlabel 'Pattern'");
  clsplot.writeExpr("plot '" + cls_file + "' using 1:2 title 't1', '" + cls_file + "' using 1:3 title 't2', '" + cls_file + "' using 1:6 title 'f(t)' with lines ");
}



int main() {
  cout << endl << endl;
  srand(time(NULL));
  rand()%2;
  const int n_trn = 2000;                 //sequence length of a training pattern.
  const int n_tst = 20;                   //sequence length of a classification pattern.
  const int nepochs = 600;                //number of training epochs.

  const string trn_file = "trn.dat";      //file containing the training sequence
  const string log_file = "log.dat";      //file containing the training error.
  const string cls_file = "cls.dat";      //file containing the classification error.
  const string tst_file = "tst.dat";      //file containing the classification pattern.

  const string trn_bitf = "trn_bitf.dat"; //random bitfile.
  const string tst_bitf = "tst_bitf.dat"; //random classification file.

  //generate random bit pattern.
  randomBitFile(trn_bitf,n_trn);
  randomBitFile(tst_bitf,n_tst);

  //construct xor sequences from the random bit patterns.
  xorBitFile(trn_bitf,trn_file);
  xorBitFile(tst_bitf,tst_file);
  
  //setup training
  initSrnTraining(nepochs,n_trn,trn_file,cls_file,tst_file,log_file);
  initBpnTraining(nepochs,n_trn,trn_file,cls_file,tst_file,log_file);

  cout << endl << endl;
  return 0;
}

//######################################################################################
//auxilary function implementations
//######################################################################################

string double2str(double d) {
  stringstream ss;
  string s;
  ss << d;
  ss >> s;
  return s;
}

inline int XOR(int a, int b) {
  return ((a || b) && !(a && b));
}

inline int OR(int a, int b) {
  return (a || b);
}

void resetStream(ifstream& s) {
  s.clear();
  s.seekg(0);
}

int cntPatterns(ifstream& s) {
  resetStream(s);
  int cnt = 0;
  int tmp;

  s >> tmp;
  while (!s.eof()) {
    s >> tmp;
    cnt++;
  }
  resetStream(s);
  return cnt-1;
}

void mkXORfile(const int n, const string& fname) {
  ofstream ofs(fname.c_str());

  for (int i=0; i<n; i++) {
    bool a = rand()%2 ? true : false;
    bool b = rand()%2 ? true : false;
    bool c = XOR(a,b);
    ofs << a << " " << b << " " << c << " ";
  }
  ofs << endl;
  ofs.close();
}

void mkORfile(const int n, const string& fname) {
  ofstream ofs(fname.c_str());

  for (int i=0; i<n; i++) {
    int a = rand()%2 ? 1 : 0;
    int b = rand()%2 ? 1 : 0;
    int c = a || b;
    ofs << a << " " << b << " " << c << " ";
  }
  ofs << endl;
  ofs.close();
}

void xorBitFile(const string& iname, const string& oname) {
  int bit1,bit2;

  ifstream is(iname.c_str());
  ofstream os(oname.c_str());

  if (!is.good()) {
    cout << "Error, invalid bitfile-name" << endl;
    return;
  }

  while (is >> bit1 >> bit2) 
    os << bit1 << " " << bit2 << " " << XOR(bit1,bit2) << " ";

  is.close();
  os.close();  
}

void orBitFile(const string& iname, const string& oname) {
  int bit1,bit2;

  ifstream is(iname.c_str());
  ofstream os(oname.c_str());

  if (!is.good()) {
    cout << "Error, invalid bitfile-name" << endl;
    return;
  }

  while (is >> bit1 >> bit2) 
    os << bit1 << " " << bit2 << " " << (bit1 || bit2) << " ";

  is.close();
  os.close();  
}

void randomBitFile(const string& fname, const int& nbits) {
  ofstream os(fname.c_str());

  for (int i=0; i<nbits; i++)
    os << rand()%2 << " ";
  os.close();
}


