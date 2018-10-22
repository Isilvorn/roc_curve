#include <iostream>
#include <iomanip>
#include <fstream>
#include <list>
#include <math.h>

using namespace std;

// reads the data file in
bool read_input(char**, list<double*>&, bool=true);
// constructs the roc_curve
void construct_roc(list<double*>&, list<double*>&);
// deallocates memory
void deallocate(list<double*>&);

// definitions to make access to the roc array components more plain
#define TPR    0
#define FPR    1
#define THRESH 2
#define NTP    3
#define NFP    4
#define NTN    5
#define NFN    6

// function input to the list sort routine, sorting by the first element
bool comp(const double *first, const double *second) 
{ if (first[0] > second[0]) return true; else return false; }

// the main program takes the input file name from the command line 
int main(int argv, char **argc) {
  list<double*>::iterator it;

  cout << "Executing roc with command line arguments: ";
  for (int i=0; i<argv; i++)
	cout << argc[i] << " ";
  cout << endl << endl;

  list<double*> probs;
  list<double*> roc;
  if (argv == 2) {
	if (read_input(argc, probs, false)) { // reading the input
	  construct_roc(probs,roc);    // constructing the roc
	  
	  // displaying the results
	  it = roc.begin();
	  int i=0;
	  cout << "     Threshold  TP    FP    TN    FN    TPR   FPR" << endl;
	  cout << "     =========  ====  ====  ====  ====  ====  ====" << endl;
	  while (it != roc.end()) { 
		cout << setw(3) << i << ": " 
			 << fixed << setprecision(2) 
			 << setw(9) << (*it)[THRESH] << "  "
			 << setprecision(0) 
			 << setw(4) << (*it)[NTP] << "  " 
			 << setw(4) << (*it)[NFP] << "  " 
			 << setw(4) << (*it)[NTN] << "  " 
			 << setw(4) << (*it)[NFN] << "  "
			 << setprecision(2) 
			 << setw(4) << (*it)[TPR] << "  " 
			 << setw(4) << (*it)[FPR] << endl; 
		it++; i++; 
	  } // end while (it)
	} // end if (read_input)
  } // end if (argv)
  else {
	cout << "Usage:  ./roc [Input Vector]" << endl;
  } // end else (argv)

  cout << endl;

  // deallocating dynamic memory
  deallocate(probs);
  deallocate(roc);
  return 0;
}

/*
** The construct_roc() function takes the input read in from the file
** and creates a table of values that define the ROC curve.
*/
void construct_roc(list<double*> &input, list<double*> &output) {
  list<double*>::iterator it1, it2;
  double *d, threshold;
  int    nTP, nFP, nTN, nFN, nElem, i;

  // sorting the data in decreasing order by probability
  input.sort(comp);
  // finding the number false and true negatives for each unique 
  // probability in the vector
  it1 = input.begin();
  threshold = -1.0;
  nElem     = 0;
  while (it1 != input.end()) {
	threshold = (*it1)[0];
	// counting the number of true/false positves/negatives at
	// each unique threshold
	nTP = nFP = nTN = nFN = 0.0;
	it2 = input.begin();
	i   = 0;
	while (it2 != input.end()) {
	  if (i <= nElem) {
		if ((*it2)[1] == 1) nTP++;
		else                nFP++;
	  } // end if (*it2)[0]
	  else {
		if ((*it2)[1] == 1) nFN++;
		else                nTN++;
	  } // end else (*it2)[0]
	  it2++; i++; // incrementing inner iterator
	} // end while (it2)
	// recording the TPR/FPR pair results
	d = new double[7];
	d[TPR]    = ((double)nTP)/(nTP+nFN);
	d[FPR]    = ((double)nFP)/(nFP+nTN);
	d[THRESH] = threshold;
	d[NTP]    = nTP;
	d[NFP]    = nFP;
	d[NTN]    = nTN;
	d[NFN]    = nFN;
	output.push_front(d);
	it1++; nElem++; // incrementing outer iterator
  } // end while (it1)

  // the last entry is always with TPR=0.0 and FPR=0.0
  d = new double[7];
  d[TPR]    = 0.0;
  d[FPR]    = 0.0;
  d[THRESH] = 1.0;
  d[NTP]    = 0;
  d[NFP]    = 0;
  d[NTN]    = nTP+nTN;
  d[NFN]    = nFP+nFN;
  output.push_back(d);

} // end construct_roc()

/*
** The read_input() function reads the input file and stores the data in
** the data arrays.
*/
bool read_input(char **argc, list<double*> &p, bool verbose) {
  ifstream infile;
  int      n,a;
  double   *d;

  // reading in initial weights file
  infile.open(argc[1]);
  if (infile.is_open()) {
	infile >> n;
	if (verbose) cout << "Reading in " << n << " elements..." << endl;

	for (int i=0; i<n; i++) { 
	  d = new double[2];
	  infile >> d[0] >> d[1]; 
	  if (verbose) cout << i << ": " << d[0] << " " << d[1] << endl;
	  p.push_front(d); 
	} // end for (i)

	infile.close();
  } // end if (infile.is_open())
  else { cerr << "Bad input file name." << endl; return false; }
  
  return true;
} // end read_input()

/*
** The deallocate() function deallocates any memory allocated for the 
** input list
*/
void deallocate(list<double*> &input) {
  list<double*>::iterator it;
  it = input.begin();
  while (it != input.end()) { delete *it; it = input.erase(it); } 
} // end deallocate()
