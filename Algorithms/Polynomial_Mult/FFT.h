#ifndef FFT
#define FFT
#include<iostream>
#include<complex>
#include<vector>

using std::complex;
using std::vector;

vector<complex<double>> Recursive_FFT(vector<double> &);
vector<double> Recursive_IFFT(vector<complex<double>> &);

#endif // !FFT