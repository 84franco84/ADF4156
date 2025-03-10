#ifndef ADF4156_250_h
#define ADF4156_250_h 
// library interface description
#include <Arduino.h>

/*********************** Scrittura registri **********************/

typedef union reg_ {char reg_byte[4]; long unsigned int reg_long;}; 

class ADF4156_
{
public: 
     reg_ reg;
     void begin(void); 
     long unsigned int imposta_f(unsigned long int f);
     void scansione(unsigned long f1, unsigned long f2, unsigned long passo); 
  private: 
     void invia(int n_byte);
};

extern ADF4156_ ADF4156;

#endif