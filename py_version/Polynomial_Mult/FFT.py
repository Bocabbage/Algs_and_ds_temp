# -*- coding: utf-8 -*-
# Script: FFT.py
# Description: Achive the FFT algorithm
# Update date: 2019/09/28
# Author: Zhuofan Zhang
from math import pi,log2
from cmath import exp

#### Recursive Version ####
def _Recursive_FFT(arr):
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

    y0 = _Recursive_FFT(a_0)
    y1 = _Recursive_FFT(a_1)

    Omega = 1
    Omega_n = exp(2*pi*1j/Len)

    res_y = [ 0 for x in range(Len) ]

    for k in range(Len//2):
        res_y[k] = y0[k] + Omega*y1[k]
        res_y[k+Len//2] = y0[k] - Omega*y1[k]
        Omega *= Omega_n

    return res_y

def _Recursive_IFFT(y,cut):
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

    a0 = _Recursive_IFFT(y_0)
    a1 = _Recursive_IFFT(y_1)

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

#### Iterative Version ####
def _Bit_Reverse_Copy(arr):
    ''' Return leaf-rank arr A '''
    REV = lambda x,n:int('{:0{width}b}'.format(x,width=n)[::-1],2)
    n = len(arr)
    Arr = [0 for x in range(n)]
    for k in range(n):
        Arr[REV(k, int(log2(n)))] = arr[k]
    return Arr


def _Iterative_FFT(arr):
    Arr = _Bit_Reverse_Copy(arr)
    Len = len(arr)
    for s in range(int(log2(Len))):
        m = 2**(s+1)
        Omega_m = exp(2*pi*1j/m)
        for k in range(Len//m):
            Omega = 1
            for j in range(m//2):
                # Remaining problem: 
                # The pseudocode in CLRS doesn't have
                # the coef 'm' in the following expr
                t = Omega*Arr[k*m+j+m//2]
                u = Arr[k*m+j]
                Arr[k*m+j] = u + t
                Arr[k*m+j+m//2] = u - t
                Omega *= Omega_m
        # print("{}:{}".format(s,Arr))
    return Arr


def _Iterative_IFFT(y,cut):
    Y = _Bit_Reverse_Copy(y)
    Len = len(y)
    for s in range(int(log2(Len))):
        m = 2**(s+1)
        I_Omega_m = exp(-2*pi*1j/m)
        for k in range(Len//m):
            I_Omega = 1
            for j in range(m//2):
                t = I_Omega*Y[k*m+j+m//2]
                u = Y[k*m+j]
                Y[k*m+j] = 0.5 * (u + t)
                Y[k*m+j+m//2] = 0.5 * (u - t)
                I_Omega *= I_Omega_m
    CUT= lambda x: x.real if abs(x.imag) < cut else x
    return [CUT(yi) for yi in Y]

def FFT(arr,mode='iterative'):
    '''
        input: 
                [arr] list of coefs
                [mode] 'iterative' or 'recursive'

        output:
                [res] DFT result

    '''
    MODE = ['iterative','recursive']
    try:
        if mode not in MODE:
            raise ValueError
    except ValueError:
        print("WRONG mode parameter: Only 'iterative' or 'recursive'.")
        exit(-1)
    else:
        if(mode=='iterative'):
            return _Iterative_FFT(arr)
        else:
            return _Recursive_FFT(arr)

def IFFT(y,mode='iterative',cut=1e-10):
    '''
        input: 
                [arr] list of DFT-coef
                [mode] 'iterative' or 'recursive'

        output:
                [res] IDFT result

    '''
    MODE = ['iterative','recursive']
    try:
        if mode not in MODE:
            raise ValueError
    except ValueError:
        print("WRONG mode parameter: Only 'iterative' or 'recursive'.")
        exit(-1)
    else:
        if(mode=='iterative'):
            return _Iterative_IFFT(y,cut)
        else:
            return _Recursive_IFFT(y,cut)



if __name__ == '__main__':
    x = [x for x in range(8)]
    y = _Iterative_FFT(x)
    yr = _Recursive_FFT(x)
    rx = _Iterative_IFFT(y,1e-10)
    rxr = _Recursive_IFFT(yr,1e-10)
    print("x:{}".format(x))
    print("y:{}".format(y))
    print("yr:{}".format(yr))
    print("rx:{}".format(rx))
    print("rxr:{}".format(rxr))
