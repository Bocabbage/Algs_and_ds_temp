# -*- coding: utf-8 -*-
# Script: FFT.py
# Description: Achive the FFT algorithm
# Update date: 2019/09/20
# Author: Zhuofan Zhang
from math import pi
from cmath import exp

# Recursive Version
def Recursive_FFT(arr):
    '''
        input   : [list] of coefficients
        output  : [list] of fft results
        Note    : Only for arrays whose length are 2^n

    '''
    Len = len(arr)

    if(Len==1):
        return arr;
    a_0 = [ arr[idx] for idx in range(Len) if idx % 2 ==0 ]
    a_1 = [ arr[idx] for idx in range(Len) if idx % 2 ==1 ]

    y0 = Recursive_FFT(a_0)
    y1 = Recursive_FFT(a_1)

    Omega = 1
    Omega_n = exp(2*pi*1j/Len)

    res_y = [ 0 for x in range(Len) ]

    for k in range(Len//2):
        res_y[k] = y0[k] + Omega*y1[k]
        res_y[k+Len//2] = y0[k] - Omega*y1[k]
        Omega = Omega * Omega_n

    return res_y

if __name__ == '__main__':
    print(Recursive_FFT([x for x in range(8)]))
