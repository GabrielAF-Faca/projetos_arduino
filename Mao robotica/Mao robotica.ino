// MPU6050 & Servo

#include <SPI.h>
#include <Wire.h>
#include <Servo.h>
#define MPU 0x68 // endereço I2C do MPU-6050

Servo ServoPulso, ServoDedoIndicador, ServoDedoMedio;

double AcX,AcY,AcZ;
int Pitch, Roll;

void setup(){
  pinMode(A7, INPUT);
  Serial.begin(9600);
  ServoPulso.attach(6);
  ServoDedoIndicador.attach(9);
  ServoDedoMedio.attach(10);
  ServoPulso.write(-15);
  init_MPU(); // inicialização do MPU6050
}
 
void loop()
{

  
  if (analogRead(A7) < 1000){

    ServoPulso.write(0);
    ServoDedoIndicador.write(0);
    
  } else {

    FunctionsMPU(); // Adquira eixos AcX, AcY, AcZ.
    
    Roll = FunctionsPitchRoll(AcX, AcY, AcZ);   //Cálculo do ângulo de Roll
    Pitch = FunctionsPitchRoll(AcY, AcX, AcZ);  //cálculo do ângulo de inclinação
  
    int ServoPitch = map(Pitch, -90, 90, 100, 0);
    int ServoRoll = map(Roll, -90, 90, -100, 100);

    ServoPulso.write(ServoRoll);
    ServoDedoIndicador.write(ServoPitch);


    Serial.print("Pitch: "); Serial.print(Pitch);
    Serial.print("\t");
    Serial.print("Roll: "); Serial.print(Roll);
    Serial.print("\t");
    Serial.print("Pulso: "); Serial.print(ServoRoll);
    Serial.print("\n");

//    for (int i = 0; i < 100; i++) {
//
//      Serial.println(i);
//
//      ServoPulso.write(i);
//      ServoDedoMedio.write(i);
//      ServoDedoIndicador.write(i);
//
//      delay(50);
//      
//    }

    
  }
  

  

}

void init_MPU(){
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  // registro PWR_MGMT_1
  Wire.write(0);     // definido como zero (ativa o MPU-6050)
  Wire.endTransmission(true);
  delay(1000);
}

// Função para calcular os ângulos de inclinação e rotação
double FunctionsPitchRoll(double A, double B, double C){
  double DatoA, DatoB, Value;
  DatoA = A;
  DatoB = (B*B) + (C*C);
  DatoB = sqrt(DatoB);
  
  Value = atan2(DatoA, DatoB);
  Value = Value * 180/3.14;
  
  return (int)Value;
}

// Função para aquisição dos eixos X, Y, Z do MPU6050
void FunctionsMPU(){
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // começando com o registro 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,6,true);  // solicitar um total de 14 registros
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
}
