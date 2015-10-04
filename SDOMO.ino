#include <DynamixelSerial2.h>
int out1 = 3, out2 = 4, out3 = 5; 
int entrada=0;

void setup(){
  Serial.begin(9600);
  pinMode(out1,OUTPUT);
  pinMode(out2,OUTPUT);
  pinMode(out3,OUTPUT);
  Dynamixel.begin(1000000,2);  
  delay(1000);
}

void loop(){
  if(Serial.available()>0){
    entrada=Serial.read();
    if(entrada=='0'){
      digitalWrite(out1,HIGH);
      Serial.println("LED 1 ENCENDIDO");}//LED1 1
    if(entrada=='1'){
      digitalWrite(out1,LOW);
      Serial.println("LED 1 APAGADO");}//LED1 0
    if(entrada=='2'){
      digitalWrite(out2,HIGH);
      Serial.println("LED 2 ENCENDIDO");}//LED2 1
    if(entrada=='3'){
      digitalWrite(out2,LOW);
      Serial.println("LED 2 APAGADO");}//LED2 0
    if(entrada=='4'){
      digitalWrite(out3,HIGH);
      Serial.println("LED 3 ENCENDIDO");}//LED3 1
    if(entrada=='5'){
      digitalWrite(out3,LOW);
      Serial.println("LED 3 APAGADO");}//LED3 0
    if(entrada=='6'){abrir();}//MOTOR
    if(entrada=='7'){cerrar();}//MOTOR
    if(entrada=='8'){encender();}//PANTALLA
    if(entrada=='9'){apagar();}//PANTALLA
  } 
}
void abrir(){
  Dynamixel.setEndless(1,ON);
  Dynamixel.turn(1,RIGTH,600); //numero de servo, direccion, velocidad
  delay(1000);
  Dynamixel.turn(1,RIGTH,0);
  //Dynamixel.setEndless(1,OFF);
}
void cerrar(){
  Dynamixel.setEndless(1,ON);
  Dynamixel.turn(1,LEFT,600);
  delay(1000);
  Dynamixel.turn(1,LEFT,0);
  //Dynamixel.setEndless(1,OFF);
}
void encender(){
}
void apagar(){
}

