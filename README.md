# arduino-revcounter
> a revcounter sketch for arduino  
> http://salvatorelab.es

# Español
Hice este proyecto hace unos meses con los siguientes materiales:
 - Arduino nano
 - display de 4 dígitos - 7 segmentos (aquí unos ejemplos: http://salvatorelab.es/2013/11/arduino-4-digit-7-segments-display-ejemplos/)
 - sensor inductivo

El objetivo era añadir un contador de revoluciones a un antiguo torno que no tenía.

Funciona hasta 3000rpm. La precisión depende de cómo coloques las placas de detección del sensor inductivo (espaciadas regularmente y con el mismo tamaño) aunque se puede calibrar después con un medidor externo y retocarlo un poco en código.

# English
This is a small project I made a few months ago with:
 - an Arduino nano
 - a 4 digit - 7 segment display
 - an inductive sensor

With all that stuff I added a rev counter to a lathe that didn't have one :D

It detects up to 3000rpm, accuracy depends on the inductive sensor and plates positioning.
