/*
 * Ping-Pong
 * Por: Christian Elías Cruz González <http://itsoeh.edu.mx>
 * Este código contiene una licencia de uso.
 * 
 * Modificado 12 de marzo de 2020
 * Por Christian Elías Cruz González
 */

// Includes Wire, SPI y Adafruit
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Declaración del display utilizado.
Adafruit_SSD1306 display(128, 64, &Wire, -1);

// Almacenan posiciones de los jugadores.
int a0, a1;

// Almacenan las posiciones (en X y en Y) de la pelota en el momento inicial.
int posBolIniX = 63, posBolIniY = 31;

// Almacenan las posiciones actuales (en X y en Y) de la pelota.
int posBolX, posBolY;

// Almacenan la dirección que debe tomar la pelota:
// dirX ->  1 --- Hacia la izquierda
// dirX -> -1 --- Hacia la derecha
// dirY ->  1 --- Hacia la abajo
// dirY -> -1 --- Hacia la arriba
int dirX = 1, dirY = 1;

// Almacenan el puntaje actual de cada jugador (Izquierda y derecha).
int markIzq = 0, markDer = 0;

void setup(){
  // Inicialización del Serial
  Serial.begin(9600);

  // Inicialización del Display, conectado al 0x3C
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Limpieza del display
  display.clearDisplay();

  // Se muestra display limpio
  display.display();

  // Se coloca la pelota en la posición de inicio
  reinicio();
}

// Método que se ejecuta al inicio del juego, y una vez más, cada que incrementa
// el puntaje de algún jugador (al traspasar los límites izquierda o derecha).
void reinicio(){
  posBolX = posBolIniX;
  posBolY = posBolIniY;
  dirX = - dirX;
  delay(1000);
}

void loop(){
  // Limpieza del display
  display.clearDisplay();

  // Entradas de los potenciómetros
  a0 = map(analogRead(A0),0,1010,0,56);
  a1 = map(analogRead(A1),0,1010,0,56);

  // Gráfica del juego
  display.drawLine(0, a0+1, 0, a0+7, SSD1306_WHITE);
  display.drawLine(127, a1+1, 127, a1+7, SSD1306_WHITE);
  display.drawPixel(posBolX,posBolY, SSD1306_WHITE);
  display.setTextSize(1);

  // Gráfica del marcador
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(51, 0);
  display.cp437(true);
  display.print(markIzq);
  display.print(" - ");
  display.print(markDer);

  display.setCursor(55, 56);
  display.print("CECG");
  
  // Pintamos el display
  display.display();

  //Movimiento ball
  posBolX += dirX;
  posBolY += dirY;

  //Movimiento arriba y abajo
  if(posBolY <= 0 || posBolY >=63)
    dirY = -dirY;

  //Movimiento izquierda y derecha (Marcador izquierdo)
  if(posBolX <= 1 && posBolY > a0 && posBolY < a0+8)
    dirX = -dirX;
  else if (posBolX <= 1){
    reinicio();
    if(markIzq<9)
      markIzq+=1;
  }

  //Movimiento izquierda y derecha (Marcador derecho)
  if(posBolX >= 126 && posBolY > a1 && posBolY < a1+8)
    dirX = -dirX;
  else if (posBolX >= 126){
    reinicio();
    if(markDer<9)
      markDer+=1;
  }
}
