/*
 * simondice.c
 *
 * Created: 02/08/2021 08:37:33 p. m.
 * Author: Irving Aguilar
 */

#include <mega328p.h>
#include <delay.h>
#include <stdlib.h>
/*
[START] CONSTANTS SECTION
*/
#define   BUZZER   7
#define   ENTRADA_A  A0
#define   ENTRADA_B  A1
#define   ENTRADA_C  A2
#define   ENTRADA_D  A3
#define   SALIDA_A   2
#define   SALIDA_B   3
#define   SALIDA_C   4
#define   SALIDA_D   5
/*
[END] CONSTANTS SECTION
*/
/*
[START] VARIABLES SECTION
*/
int melodia[ ] = {262, 196, 196, 220, 196, 0, 247, 262};
int duracionNotas[] = {4, 8, 8, 4, 4, 4, 4, 4};
int nivelActual = 1;
int velocidad = 500;
const int NIVEL_MAX = 100;
int secuencia[NIVEL_MAX];
int secuenciaUsuario[NIVEL_MAX];
int pausaEntreNotas;
int i=0;
/*
[END] VARIABLES SECTION
*/
/*
[START] FUNCTIONS SECTION
*/
// NOTA: Cambiar funcion
void tono (float frec)
{
    float Cuentas;
    unsigned int CuentasEnt;
    DDRB.1=1;
    Cuentas=500000.0/frec;
    CuentasEnt=Cuentas;
    if ((Cuentas-CuentasEnt)>=0.5)
        CuentasEnt++;
        
    TCCR1A=0x40;
    TCCR1B=0x09;
    OCR1AH=(CuentasEnt-1)/256;
    OCR1AL=(CuentasEnt-1)%256;
}

void noTono()
{
   TCCR1A=0;
   TCCR1B=0; // stop
}

void secuenciaCorrecta(){
   if(nivelActual < NIVEL_MAX);
      nivelActual++;
   velocidad -= 50;
   delay_ms(200);
}

void melodiaError(){
   for(i = 0; i < 8; i++){
      int duracionNota = 1000/duracionNotas[i];
      tono(melodia[i]);
      pausaEntreNotas = duracionNota * 1.30;
      delay_ms(pausaEntreNotas);
      noTono();
   }
}

void secuenciaError(){
//   digitalWrite(SALIDA_A, HIGH);
//   digitalWrite(SALIDA_B, HIGH);
//   digitalWrite(SALIDA_C, HIGH);
//   digitalWrite(SALIDA_D, HIGH);
    PORTD.2=1;
    PORTD.3=1;
    PORTD.4=1;
    PORTD.5=1;
   delay_ms(250);
//   digitalWrite(SALIDA_A, LOW);
//   digitalWrite(SALIDA_B, LOW);
//   digitalWrite(SALIDA_C, LOW);
//   digitalWrite(SALIDA_D, LOW);
    PORTD.2=0;
    PORTD.3=0;
    PORTD.4=0;
    PORTD.5=0;
   delay_ms(250);
   melodiaError();
   nivelActual = 1;
   velocidad = 500;
}

void muestraSecuencia(){
//   digitalWrite(SALIDA_A, LOW);
//   digitalWrite(SALIDA_B, LOW);
//   digitalWrite(SALIDA_C, LOW);
//   digitalWrite(SALIDA_D, LOW);
    PORTD.2=0;
    PORTD.3=0;
    PORTD.4=0;
    PORTD.5=0;
   for(i = 0; i < nivelActual; i++){
      if( secuencia[i] == SALIDA_A ){
         tono(200);
         delay_ms(200);
         noTono();
      }
      if( secuencia[i] == SALIDA_B ){
         tono(300);
         delay_ms(200);
         noTono();
      }
      if( secuencia[i] == SALIDA_C ){
         tono(400);
         delay_ms(200);
         noTono();
      }
      if( secuencia[i] == SALIDA_D ){
         tono(500);
         delay_ms(200);
         noTono();
      }
      //digitalWrite(secuencia[i], HIGH); 
      if(secuencia[i] == SALIDA_A)
      {
        PORTD.2=1;
      }
      if(secuencia[i] == SALIDA_B)
      {
        PORTD.3=1;
      }
      if(secuencia[i] == SALIDA_C)
      {
        PORTD.4=1;
      }
      if(secuencia[i] == SALIDA_D)
      {
        PORTD.5=1;
      }
      delay_ms(velocidad);
      //digitalWrite(secuencia[i], LOW);
      if(secuencia[i] == SALIDA_A)
      {
        PORTD.2=0;
      }
      if(secuencia[i] == SALIDA_B)
      {
        PORTD.3=0;
      }
      if(secuencia[i] == SALIDA_C)
      {
        PORTD.4=0;
      }
      if(secuencia[i] == SALIDA_D)
      {
        PORTD.5=0;
      }
      delay_ms(200);
   }
}



void leeSecuencia(){
   int flag = 0;
   for(i = 0; i < nivelActual; i++){
      flag = 0;
      while(flag == 0){
         if(PINC.3 == 0){
            //digitalWrite(SALIDA_D, HIGH);
            PORTD.5=1;
            tono(500);
            delay_ms(300);
            noTono();
            secuenciaUsuario[i] = SALIDA_D;
            flag = 1;
            delay_ms(200);
            if(secuenciaUsuario[i] != secuencia[i]){
               secuenciaError();
               return;
            }
            //digitalWrite(SALIDA_D, LOW);
            PORTD.5=0;
         }
         if(PINC.2 == 0){
            //digitalWrite(SALIDA_C, HIGH);
            PORTD.4=1;
            tono(400);
            delay_ms(300);
            noTono();
            secuenciaUsuario[i] = SALIDA_C;
            flag = 1;
            delay_ms(200);
            if(secuenciaUsuario[i] != secuencia[i]){
               secuenciaError();
               return;
            }
            //digitalWrite(SALIDA_C, LOW); 
            PORTD.4=0;
         }
         if(PINC.1 == 0){
            //digitalWrite(SALIDA_B, HIGH); 
            PORTD.3=1;
            tono(300);
            delay_ms(300);
            noTono();
            secuenciaUsuario[i] = SALIDA_B;
            flag = 1;
            delay_ms(200);
            if(secuenciaUsuario[i] != secuencia[i]){
               secuenciaError();
               return;
            }
            //digitalWrite(SALIDA_B, LOW);
            PORTD.3=0;
         }
         if(PINC.0 == 0){
            //digitalWrite(SALIDA_A, HIGH); 
            PORTD.2=1;
            tono(200);
            delay_ms(300);
            noTono();
            secuenciaUsuario[i] = SALIDA_A;
            flag = 1;
            delay_ms(200);
            if(secuenciaUsuario[i] != secuencia[i]){
               secuenciaError();
               return;
            }
            //digitalWrite(SALIDA_A, LOW);
            PORTD.2=0;
         }
      }
   }
   secuenciaCorrecta();
}

void generaSecuencia(){
   srand(TCNT0);
   for(i = 0; i < NIVEL_MAX; i++){
      secuencia[i] = (rand()%(6-2+1))+2;
   }
}
/*
[END] FUNCTIONS SECTION
*/
void main(void)
{
PORTC = 0x0FF; // PC0 a PC7 con pull-up
DDRB.1 = 1; // PB1 de salida para el buzzer
DDRD=0x0FF;  // PD0 a PD7 de salida para LEDs
TCCR0B=0x01; //Timer 0 a tiempo con CK
while (1)
    {
    // Please write your application code here
       if(nivelActual == 1){
          generaSecuencia();
          muestraSecuencia();
          leeSecuencia();
       }
       if(nivelActual != 1){
          muestraSecuencia();
          leeSecuencia();
       }
    }
}
