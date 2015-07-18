// 4 digit display: segments diagram
//         A
//      ________
//     |        |
//   F |        |  B
//     |    G   |
//     |________|
//     |        |
//     |        |
//   E |        |   C
//     |________|
//                 o <-- the dot :D
//         D         H
//
#define DEBUG  0

//Declare what pins are connected to the digits
int digit1 = 13; //Pin 12 on my 4 digit display
int digit2 = 10; //Pin 9 on my 4 digit display
int digit3 = 9; //Pin 8 on my 4 digit display
int digit4 = 2; //Pin 6 on my 4 digit display
 
//Declare what pins are connected to the segments
int segA = 12; //Pin 11 on my 4 digit display
int segB = 8; //Pin 7 on my 4 digit display
int segC = 4; //Pin 4 on my 4 digit display
int segD = 6; //Pin 2 on my 4 digit display
int segE = 7; //Pin 1 on my 4 digit display
int segF = 11; //Pin 10 on my 4 digit display
int segG = 3; //Pin 5 on my 4 digit display
/*int segH = 13; //Pin 3 on my 4 digit display*/

//Other pins
int captador = 5;
int buzzer = A0;

/**
How to use the 4 digit display:
https://www.adafruit.com/forums/viewtopic.php?f=25&p=105954
Common Cathode version
How to:
In loop():
(1) All four cathodes high (pins 2,3,4,5 HIGH)
(2) Desired anodes for the digit high (write 0 or whatever with HIGHs)
(3) Cathode for first digit low (pin 2 LOW)
(4) Delay (sleep)

Repeat for digit 2

Repeat for digit 3

Repeat for digit 4

*/

/**
Esquema de la rueda con los 4 puntos de detección

        1  o 
     o        o 
    o          1 
    1          o 
     o        o 
        o  1 

maximo a medir: 3000rpm
eso son 50Hz. Pero en cada vuelta tenemos 4 puntos de detección, así que: 200Hz
al máximo de revoluciones se producirán señales con una frecuencia de 200Hz
para detectar 200Hz hay que muestrear a 400Hz
es decir, tomar una muestra cada 2.5ms (400 muestras por segundo)

por encima de 1599rpm hacer sonar zumbador
**/

int espera = 2500;
int state = 0;
int revoluciones = 0;
int medidas = 0;
int valores[402];
boolean sonando = false;

void setup() {
  if (DEBUG) Serial.begin(9600);
  pinMode(captador, INPUT);
  pinMode(buzzer, OUTPUT);
  for(int i=2; i<14; i++) {
    if(i!=5) {
      pinMode(i, OUTPUT);
    }
  }
}

void loop() {
  state = digitalRead(captador);
  valores[medidas] = state;
  
  if (medidas%6==0) { //cada 6 medidas (15ms) repintamos el display (si no deja de verse)
    printCounter(revoluciones); //esto supone 4*500ms de espera (2000ms)
    espera = 500;
  } else {
    espera = 2500;
  }
  //como 402 es multiplo de 6, y en un segundo caben exactamente 400 medidas, nos pilla bien 402
  //y asi el repaint de los digitos se hace siempre cada 6 medidas exactamente
  if (medidas==401) { //cada 402 medidas (0..401), calculamos revoluciones
    medidas = 0;
    //actualizar revoluciones
    revoluciones = calcularRevoluciones( valores );
  } else {
    medidas++;
  }
  
  if (revoluciones>1599) {
    if (!sonando) {
      analogWrite(buzzer, 128); //emite sonido
      sonando = true;
    } else {
      analogWrite(buzzer, LOW); //deja de emitir
      sonando = false;
    }
  }
  
  //completamos la espera que nos quede (depende de si hemos repintado o no)
  delayMicroseconds(espera);
  
  //printStop();
  //printAll(LOW);
  //delay(15);
}

/**
Tenemos 402 medidas (0s y 1s) correspondientes a un segundo de mediciones.
Sabiendo cuantas crestas y valles hay, y que cada vuelta entera supone 4 crestas,
podemos saber cuantas vueltas ha dado en este ultimo segundo de mediciones.
*/
int calcularRevoluciones( int vals[] ) {
  float rpm = 0;
  int crestas = 0;
  int valles = 0;
  int actual;
  int anterior = vals[0];
  
  for (int i=1; i<402; i++) {
    actual = valores[i];
    if (DEBUG) Serial.print(actual);
    if (anterior==0 && actual==1) {
      // 01 cresta
      crestas++;
    } else if (anterior==1 && actual==0) {
      // 10 valle
      valles++;
    }
    anterior = valores[i];
  }
  if (DEBUG) Serial.println("");
  
  //una cresta y un valle son un ciclo: ciclos = (crestas+valles/2)
  //como hay 4 sondas en la rueda, 4 ciclos son 1 revolución por segundo. 
  //así que: rpm = (ciclos/4)*60s o lo que es lo mismo, rpm = ciclos*15.
  rpm = ((crestas+valles)/2)*15;
  return (int) rpm;
}

// clear selection
void digitsHigh() {
  digitalWrite(digit1, HIGH);
  digitalWrite(digit2, HIGH);
  digitalWrite(digit3, HIGH);
  digitalWrite(digit4, HIGH);
}

//clear segments
void clearDigit() {
  digitalWrite(segA, LOW);
  digitalWrite(segB, LOW);
  digitalWrite(segC, LOW);
  digitalWrite(segD, LOW);
  digitalWrite(segE, LOW);
  digitalWrite(segF, LOW);
  digitalWrite(segG, LOW);
  //digitalWrite(segH, LOW);
}

void printStop() {
  //S
  digitsHigh();
  digitalWrite(segA, HIGH);
  digitalWrite(segF, HIGH);
  digitalWrite(segG, HIGH);
  digitalWrite(segC, HIGH);
  digitalWrite(segD, HIGH);
  digitalWrite(digit1, LOW);
  delayMicroseconds(500);
  clearDigit();
  
  //t
  digitsHigh();
  digitalWrite(segF, HIGH);
  digitalWrite(segG, HIGH);
  digitalWrite(segE, HIGH);
  digitalWrite(segD, HIGH);
  digitalWrite(digit2, LOW);
  delayMicroseconds(500);
  clearDigit();
  
  //O
  digitsHigh();
  digitalWrite(segA, HIGH);
  digitalWrite(segB, HIGH);
  digitalWrite(segC, HIGH);
  digitalWrite(segD, HIGH);
  digitalWrite(segE, HIGH);
  digitalWrite(segF, HIGH);
  digitalWrite(digit3, LOW);
  delayMicroseconds(500);
  clearDigit();
  
  //P
  digitsHigh();
  digitalWrite(segA, HIGH);
  digitalWrite(segB, HIGH);
  digitalWrite(segG, HIGH);
  digitalWrite(segF, HIGH);
  digitalWrite(segE, HIGH);
  digitalWrite(digit4, LOW);
  delayMicroseconds(500);
  clearDigit();
}

void printAll(int val) {
  //S
  digitsHigh();
  digitalWrite(segA, val);
  digitalWrite(segB, val);
  digitalWrite(segC, val);
  digitalWrite(segD, val);
  digitalWrite(segE, val);
  digitalWrite(segF, val);
  digitalWrite(segG, val);
  digitalWrite(digit1, LOW);
  delayMicroseconds(500);
  clearDigit();
  //t
  digitsHigh();
  digitalWrite(segA, val);
  digitalWrite(segB, val);
  digitalWrite(segC, val);
  digitalWrite(segD, val);
  digitalWrite(segE, val);
  digitalWrite(segF, val);
  digitalWrite(segG, val);
  digitalWrite(digit2, LOW);
  delayMicroseconds(500);
  clearDigit();
  //O
  digitsHigh();
  digitalWrite(segA, val);
  digitalWrite(segB, val);
  digitalWrite(segC, val);
  digitalWrite(segD, val);
  digitalWrite(segE, val);
  digitalWrite(segF, val);
  digitalWrite(segG, val);
  digitalWrite(digit3, LOW);
  delayMicroseconds(500);
  clearDigit();
  //P
  digitsHigh();
  digitalWrite(segA, val);
  digitalWrite(segB, val);
  digitalWrite(segC, val);
  digitalWrite(segD, val);
  digitalWrite(segE, val);
  digitalWrite(segF, val);
  digitalWrite(segG, val);
  digitalWrite(digit4, LOW);
  delayMicroseconds(500);
  clearDigit();
}

void printCounter( int digits ) {
  int unidades, decenas, centenas, millares;
  unidades = digits % 10;
  digits = digits-unidades;
  decenas = (digits % 100) / 10;
  digits = digits-(10*decenas);
  centenas = (digits % 1000) / 100;
  digits = digits-(100*centenas);
  millares = digits / 1000;
  
  printDigit(millares, digit1);
  delayMicroseconds(500);
  clearDigit();
  printDigit(centenas, digit2);
  delayMicroseconds(500);
  clearDigit();
  printDigit(decenas, digit3);
  delayMicroseconds(500);
  clearDigit();
  printDigit(unidades, digit4);
  delayMicroseconds(500);
  clearDigit();
}

void printDigit(int number, int pin) {
#define SEGMENT_ON  HIGH
#define SEGMENT_OFF LOW

  digitsHigh();
  switch (number){
    case 0:
      digitalWrite(segA, SEGMENT_ON);
      digitalWrite(segB, SEGMENT_ON);
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segD, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_ON);
      digitalWrite(segF, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_OFF);
      break;
  
    case 1:
      digitalWrite(segA, SEGMENT_OFF);
      digitalWrite(segB, SEGMENT_ON);
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segD, SEGMENT_OFF);
      digitalWrite(segE, SEGMENT_OFF);
      digitalWrite(segF, SEGMENT_OFF);
      digitalWrite(segG, SEGMENT_OFF);
      break;
  
    case 2:
      digitalWrite(segA, SEGMENT_ON);
      digitalWrite(segB, SEGMENT_ON);
      digitalWrite(segC, SEGMENT_OFF);
      digitalWrite(segD, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_ON);
      digitalWrite(segF, SEGMENT_OFF);
      digitalWrite(segG, SEGMENT_ON);
      break;
  
    case 3:
      digitalWrite(segA, SEGMENT_ON);
      digitalWrite(segB, SEGMENT_ON);
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segD, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_OFF);
      digitalWrite(segF, SEGMENT_OFF);
      digitalWrite(segG, SEGMENT_ON);
      break;
  
    case 4:
      digitalWrite(segA, SEGMENT_OFF);
      digitalWrite(segB, SEGMENT_ON);
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segD, SEGMENT_OFF);
      digitalWrite(segE, SEGMENT_OFF);
      digitalWrite(segF, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      break;
  
    case 5:
      digitalWrite(segA, SEGMENT_ON);
      digitalWrite(segB, SEGMENT_OFF);
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segD, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_OFF);
      digitalWrite(segF, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      break;
  
    case 6:
      digitalWrite(segA, SEGMENT_ON);
      digitalWrite(segB, SEGMENT_OFF);
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segD, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_ON);
      digitalWrite(segF, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      break;
  
    case 7:
      digitalWrite(segA, SEGMENT_ON);
      digitalWrite(segB, SEGMENT_ON);
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segD, SEGMENT_OFF);
      digitalWrite(segE, SEGMENT_OFF);
      digitalWrite(segF, SEGMENT_OFF);
      digitalWrite(segG, SEGMENT_OFF);
      break;
  
    case 8:
      digitalWrite(segA, SEGMENT_ON);
      digitalWrite(segB, SEGMENT_ON);
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segD, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_ON);
      digitalWrite(segF, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      break;
  
    case 9:
      digitalWrite(segA, SEGMENT_ON);
      digitalWrite(segB, SEGMENT_ON);
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segD, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_OFF);
      digitalWrite(segF, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      break;
      
    default:
      digitalWrite(segA, SEGMENT_ON);
      digitalWrite(segB, SEGMENT_ON);
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segD, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_ON);
      digitalWrite(segF, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_OFF);
      break;
  }
  digitalWrite(pin, LOW);
}
