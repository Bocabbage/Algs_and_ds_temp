# -*- coding: utf-8 -*-
# Script: FFT.py
# Description: Achive the FFT algorithm
# Update date: 2019/09/21
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
        Omega *= Omega_n

    return res_y

def Recursive_IFFT(y,cut=1e-10):
    '''
        input   : [list] of fft results
        output  : [list] of coefficients
        cut     : if imaginary part of the output coef is below this parameter,
                  only the keep the real part of the coef
        Note    : Do Inverse-FFT

    '''
    Len = len(y)
    if(Len==1):
        return y

    y_0 = [ y[idx] for idx in range(Len) if idx % 2 ==0 ]
    y_1 = [ y[idx] for idx in range(Len) if idx % 2 ==1 ]

    # print("{}:y0:{}".format(Len,y_0))
    # print("{}:y1:{}".format(Len,y_1))

    a0 = Recursive_IFFT(y_0)
    a1 = Recursive_IFFT(y_1)

    I_Omega = 1
    I_Omega_n = exp(-2*pi*1j/Len)

    res_a = [ 0 for x in range(Len) ]

    for j in range(Len//2):
        # Attention: the recursive-function format:
        # a[j] = (1/Len) *(A0[j]+I_Omega*A1[j]) 
        # while Ax[j] = 0.5*Len*ax[j]
        res_a[j] = 0.5 * (a0[j]+I_Omega*a1[j])
        res_a[j+Len//2] = 0.5 * (a0[j]-I_Omega*a1[j])
        I_Omega *= I_Omega_n

    CUT= lambda x: x.real if abs(x.imag) < cut else x
    # print("a_{}:{}".format(Len,res_a))
    # print("a_{}_cut:{}".format(Len,[CUT(x) for x in res_a]))
    return [CUT(x) for x in res_a]


if __name__ == '__main__':
    x = [x for x in range(8)]
    y = Recursive_FFT(x)
    rx = Recursive_IFFT(y)
    print("x:{}".format(x))
    print("y:{}".format(y))
    print("rx:{}".format(rx))
