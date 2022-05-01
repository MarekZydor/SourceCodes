#include <iostream>
#include <complex>
#include <cmath>

const std::complex<double> i(0,1);
std::complex<double> * dft(double * f, int N){
    std::complex<double> * ck = new std::complex<double>[N];
    for(int k = 0; k<N; k++)
    {
        ck[k] = 0;
        for(int n = 0; n<N; n++)
        {
            ck[k] += f[n]* std::exp((double)-2*k*n/N*M_PI*i);
        }
    }
    return ck;
}


std::complex<double> * fft(double * f, int N){
    auto * ck = new std::complex<double>[N];
    if(N==1)
    {
        ck[0] = f[0];
        return ck;
    }
    int Nprzez2 = N/2;
    double Fe[Nprzez2];
    double Fo[Nprzez2];
    for(int j = 0; j < Nprzez2; j++)
    {
        Fe[j] = f[j * 2];
        Fo[j] = f[j * 2 + 1];
    }
    std::complex<double> * E = fft(Fe, Nprzez2);
    std::complex<double> * O = fft(Fo, Nprzez2);
    for(int k = 0; k<(Nprzez2); k++)
    {
        ck[k] = E[k]+std::exp((double)-2*k/N*M_PI*i)*O[k];
        ck[k+(Nprzez2)] = E[k]-std::exp((double)-2*k/N*M_PI*i)*O[k];
    }
    delete [] E;
    delete [] O;
    return ck;
}

std::complex<double> err(std::complex<double> * ck1, std::complex<double> * ck2, int N)
{
    std::complex<double> result = 0;
    for(int j = 0; j < N; j++){
        result += std::abs(ck1[j] - ck2[j]);
    }
    result /= (double)N;
    return result;

}

int main ()
{
    const int MAX_ORDER = 13;

    const bool PRINT_COEFS = true;
    for ( int o = 1; o <= MAX_ORDER ; o ++)
    {
        const int N = 1 << o;
        std::cout<<"N: "<<N<<std::endl;
        double * f = new double [ N ] ;
        for ( int n = 0; n < N ; n ++)
            f [ n ] = n / ( double ) N;
        clock_t t1 = clock () ;
        std::complex < double >* cDFT = dft (f, N) ;
        clock_t t2 = clock () ;
        double dft_time = ( t2 - t1 ) / ( double ) CLOCKS_PER_SEC * 1000.0;
        std::cout<<"DFT time [ms]: "<<dft_time<<std::endl;
        t1 = clock () ;
        std::complex < double >* cFFT = fft (f, N) ;
        t2 = clock () ;
        double fft_time = ( t2 - t1 ) / ( double ) CLOCKS_PER_SEC * 1000.0;
        std::cout<<"FFT time [ms]: " << fft_time<<std::endl;
        std::cout<<"mean error: "<< err(cDFT,cFFT,N)<<std::endl<<std::endl;
        if ( PRINT_COEFS  && N <= 64) {
            printf("%s %20s\n", "DFT:", "FFT:");
            for (int k = 0; k < N; k++) {
                printf("(%f,%f) (%f,%f)\n", real(cDFT[k]), imag(cDFT[k]), real(cFFT[k]), imag(cFFT[k]));
            }
        }
        std::cout<<"-----\n";

        delete [] f ;
        delete [] cDFT ;
        delete [] cFFT ;
    }
    return 0;
}