#include <iostream>
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

// function input to the list sort routine, sorting by the first element
bool comp(const double *first, const double *second) 
{ if (first[0] > second[0]) return true; else return false; }
 
int main(int argv, char **argc) {
  list<double*>::iterator pit;

  cout << "Executing roc with command line arguments: ";
  for (int i=0; i<argv; i++)
	cout << argc[i] << " ";
  cout << endl;

  list<double*> probs;
  list<double*> roc;
  if (argv == 2) {
	if (read_input(argc, probs)) {
	  construct_roc(probs,roc);
	  /*
	  pit = probs.begin();
	  int i=0;
	  while (pit != probs.end()) { 
		cout << i << ": " << (*pit)[0] << " " << (*pit)[1] << endl; 
		pit++; i++; 
	  }
	  */
	}
  }
  else {
	cout << "Usage:  ./roc [Input Vector]" << endl;
  }

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
  double *d;
  int    nTP, nFP, nTN, nFN;

  // sorting the data in decreasing order by probability
  input.sort(comp);
  // finding the number false and true negatives for each unique probability
  it1 = input.begin();
  while (it1 != input.end()) {
	cout << (*it1)[0] << " " << (*it1)[1] << endl;
	it1++;
  }

}

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
	}

	infile.close();
  }
  else { cerr << "Bad input file name." << endl; return false; }
  
  return true;
}

/*
** The deallocate() function deallocates any memory allocated for the input list
*/
void deallocate(list<double*> &input) {
  list<double*>::iterator it;

  it = input.begin();
  while (it != input.end()) {
	delete *it;
	it = input.erase(it);
  }

}
