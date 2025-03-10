#include <ADF4156_250.h>

// Pin dei PLL
#define LE    49   // Va' al punto del connettore sulla scheda ADF4156
#define DATA  53
#define CLK   51
//#define CE    A3 settato a 1 via hardware


#define   Fref 250000
#define   Step 500

/*********************** Scrittura registri AD 9951 **************************/
 
void ADF4156_::begin(void)
{ 
//pinMode(CE,OUTPUT);       //  CE Alto (Abilita il chip)
//digitalWrite(CE, HIGH); 
pinMode(LE,OUTPUT);       //  LE basso
digitalWrite(LE,HIGH);
pinMode(DATA,OUTPUT);     //  Data basso
digitalWrite(DATA, LOW); 
pinMode(CLK,OUTPUT);      //  Clk basso 
digitalWrite(CLK, LOW);

//------------------------- Setta i registri fissi -----------------------------

// Registro 4 - Ind. 100
reg.reg_byte[0]=B00000100;    // B0 0000 100
reg.reg_byte[1]=B00000000;    // B  00000000
reg.reg_byte[2]=B00001000;    // B000 00 000 B00001000;  (con lo 00 non ci sono i click nel cambio freq.)
reg.reg_byte[3]=0;
invia(4);

// Registro 3 - Ind. 011
reg.reg_byte[0]=B01000011;    // B1 1 0 0 0 011  Polarita' positiva + precisione in LD
reg.reg_byte[1]=B00000000;    // B0 1 000000     (il Bit 6=1 ha eliminato i click cambiando canale)
reg.reg_byte[2]=0;
reg.reg_byte[3]=0;
invia(4);
                              // la Fref= 250KHz / Mod=500 / 500 -> passo minimo
// Registro 2 - ind.010
reg.reg_byte[0]=B10100010;    // B10100 010   MOD = 500 -> Step da 500Hz
reg.reg_byte[1]=B00001111;    // B0 0001111   Ref=250KhZ
reg.reg_byte[2]=B00101001;    // B0 11 0 1001  Rcount=18 /  Div x2 abilitato si arriva a 36 / 9MHz/36=250KHz
reg.reg_byte[3]=B00010000;    // B0 00 1 0000???   Ottimizzato per il noise
invia(4);

// Registro 1 - Ind. 001
reg.reg_byte[0]=B00001001;    // B00001 001
reg.reg_byte[1]=B00000000;    // B0 0000000
reg.reg_byte[2]=0;
reg.reg_byte[3]=0;
invia(4);
}

// Invia n_byte
void ADF4156_::invia(int n_byte)
{
char bout; int n;
digitalWrite(LE,LOW);
for (n=n_byte-1;n>=0;n--) // Invia n_byte
   {
   bout=reg.reg_byte[n]; 
   shiftOut(DATA,CLK,MSBFIRST,bout);
   } 
digitalWrite(LE,HIGH);   
digitalWrite(DATA, LOW); 
digitalWrite(CLK, LOW); 
}

//************************ Registro 0 - Ind. 000 *******************************
 
long unsigned int ADF4156_::imposta_f(long unsigned int f)
{ 
// Calcolo N e Frac 
unsigned long int m,q,q1; char m1,m2;
//---------------- Imposta il Frac -----------------
m=f%Fref;
m=m/Step;
m1=(char)(m<<3);
reg.reg_byte[0]=m1;           // B XXXXX 000 
m2=(char)(m>>5);
//---------------- Parte Intera --------------------
q=f/Fref; 
// BX 0000000
m1=B00000000;
if((q & 1)==1)
   m1=B10000000;              // 0 0101 001 11110100 00000000 00000000
// Imposta INT   
reg.reg_byte[1]=m2 | m1;      // B Y XXXXXXX
q1=q>>1;
reg.reg_byte[2]=(char)q1;     // B YYYYYYYY
q1=q>>9;
reg.reg_byte[3]=(char)q1;     // B 0 ZZZZ yyy
reg.reg_byte[3]=reg.reg_byte[3] | B00101000; // B0 0101  Lock analog detect in MUXOUT
// Invio i 4 byte
invia(4); 
}

//------------------------------------------------------------------------------

void ADF4156_::scansione(unsigned long f1, unsigned long f2, unsigned long passo)
{ 
// Calcolo N e Frac 
unsigned int n,p; char m1,m2,q1;
//digitalWrite(CE, HIGH); 
f1=f1/Fref;
f2=f2/Fref;
for(n=f1;n<=f2;n++)
   for(p=0;p<Step;p=p+passo)
      {
      //delay(1);
      reg.reg_byte[0]=(char)(p<<3);
      m2=(char)(p>>5);
      m1=B00000000;
      if ((n & 1)==1) 
         m1=B10000000;
      reg.reg_byte[1]=m1|m2;
      q1=(char)(n>>1);
      reg.reg_byte[2]=q1;
      reg.reg_byte[3]=0; 
      invia(4);
      } 
}

ADF4156_ ADF4156;