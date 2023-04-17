#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif


U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

int esq_frente = 11;
int esq_re = 10;
int dir_frente = 9;
int dir_re = 6;


float vel = 1;
float max_vel = 10;
int vel_atual = (vel/max_vel)*255;

#include "SoftwareSerial.h"

//Create a new software  serial
SoftwareSerial bluetooth(2, 3); //TX, RX (Bluetooth)


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(esq_frente, OUTPUT);
  pinMode(esq_re, OUTPUT);
  pinMode(dir_frente, OUTPUT);
  pinMode(dir_re, OUTPUT);


  bluetooth.begin(9600);

  u8g2.begin();

  u8g2.clearBuffer();

  rosto_sorrindo();

  u8g2.sendBuffer();

  
}

int x = u8g2.getDisplayWidth()/2;
int y = u8g2.getDisplayHeight()/2;
int raio = y-1;

void loop() {
  // put your main code here, to run repeatedly:
  
  if (bluetooth.available()) {
    char r = bluetooth.read(); //Read and save the byte
    
    switch (r) {

      case 'F':
        frente();
        break;

      case 'B':
        re();
        break;

      case 'L':
        esquerda();
        break;

      case 'R':
        direita();
        break;

      case 'S':
        parar();
        break;

      case '0':
        vel = 1;
        break;
      
      case '1':
        vel = 2;
        break;

      case '2':
        vel = 3;
        break;

      case '3':
        vel = 4;
        break;

      case '4':
        vel = 5;
        break;

      case '5':
        vel = 6;
        break;

      case '6':
        vel = 7;
        break;

      case '7':
        vel = 8;
        break;

      case '8':
        vel = 9;
        break;

      case '9':
        vel = 10;
        break;

      case 'q':
        vel = 10;
        break;

      
      default:
        parar();
      
    }
    
    if (r != 'S') {
      
      Serial.print("Comando: ");
      Serial.print(r);
      Serial.print(" - Velocidade atual: ");
      Serial.print(vel_atual);
      Serial.print(" - vel: ");
      Serial.println(vel);
      
    }
    
    
  }
    

  vel_atual = (vel/max_vel)*255;
  //Serial.println(vel_atual);
  
}

void frente() {
  
  analogWrite(esq_frente, vel_atual);
  analogWrite(esq_re, 0);

  analogWrite(dir_frente, vel_atual);
  analogWrite(dir_re, 0);
  
}

void re() {

  analogWrite(esq_frente, 0);
  analogWrite(esq_re, vel_atual);

  analogWrite(dir_frente, 0);
  analogWrite(dir_re, vel_atual);

  
}

void parar() {

  analogWrite(esq_frente, 0);
  analogWrite(esq_re, 0);

  analogWrite(dir_frente, 0);
  analogWrite(dir_re, 0);
  
}

void direita() {

  analogWrite(esq_frente, vel_atual);
  analogWrite(esq_re, 0);

  analogWrite(dir_frente, 0);
  analogWrite(dir_re, 0);
  
}

void esquerda() {

  analogWrite(esq_frente, 0);
  analogWrite(esq_re, 0);

  analogWrite(dir_frente, vel_atual);
  analogWrite(dir_re, 0);
  
}


void rosto_irritado() {
  
  u8g2.drawCircle(x, y, raio);
  
  //olho esquerdo
  //u8g2.drawCircle(x - raio/2.5, y/1.5, 5);

  int altura = 13;
  int largura = 13;
  
  //coordenada do ponto de cima do triângulo
  int x1 = x - raio/2;
  int y1 = y - raio/1.8;

  int x3 = x1;
  int y3 = y1+altura;

  int x2 = x1 + largura;
  int y2 = y3;
  
  u8g2.drawTriangle(x1-2, y1, x2, y2-2, x3+1, y3);
  
  //olho direito
  x1 = x + raio/2;

   x3 = x1;
   y3 = y1+altura;
  
   x2 = x1 - largura;
   y2 = y3;
  
  u8g2.drawTriangle(x1+2, y1, x2, y2-2, x3-1, y3);

  //boca
  u8g2.drawFilledEllipse(x, y + raio/2, 11, 8, U8G2_DRAW_UPPER_RIGHT);
  u8g2.drawFilledEllipse(x, y + raio/2, 11, 8, U8G2_DRAW_UPPER_LEFT);

}

void rosto_sorrindo() { 
  u8g2.drawCircle(x, y, raio);
  //olho esquerdo
  u8g2.drawCircle(x - raio/2.5, y/1.5, 5);

  //olho direito
  u8g2.drawCircle(x + raio/2.5, y/1.5, 5);

  //boca
  u8g2.drawFilledEllipse(x, y + raio/3, 11, 8, U8G2_DRAW_LOWER_RIGHT);
  u8g2.drawFilledEllipse(x, y + raio/3, 11, 8, U8G2_DRAW_LOWER_LEFT);

}

void rosto_piscando() {
  u8g2.clearBuffer(); 
  u8g2.drawCircle(x, y, raio);
  //olho esquerdo
  u8g2.drawCircle(x - raio/2.5, y/1.5, 5);

  //olho direito
  u8g2.drawCircle(x + raio/2.5, y/1.5, 5,U8G2_DRAW_LOWER_RIGHT);
  u8g2.drawCircle(x + raio/2.5, y/1.5, 5,U8G2_DRAW_LOWER_LEFT);
  //boca
  u8g2.drawFilledEllipse(x, y + raio/3, 11, 8, U8G2_DRAW_LOWER_RIGHT);
  u8g2.drawFilledEllipse(x, y + raio/3, 11, 8, U8G2_DRAW_LOWER_LEFT);

  u8g2.sendBuffer();
}
