#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 7
Adafruit_SSD1306 display(OLED_RESET);

//definicion de pins
const int motorPin1 = 8;    // 28BYJ48 In1
const int motorPin2 = 9;    // 28BYJ48 In2
const int motorPin3 = 10;   // 28BYJ48 In3
const int motorPin4 = 11;   // 28BYJ48 In4

const int botonDE = 4; //2; // boton Derecha
const int botonIZ = 3; // boton Izquierda
const int botonOK = 2; //4; // boton OK
//int estadoOK;
//int estadoDE;
//int estadoIZ;
  
// Disparador
const int Enfocar  = 5; // Focus
const int Disparar = 6; // Disparo

//definicion variables
int motorSpeed = 1200;   //variable para fijar la velocidad del motor
int stepCounter = 0;     // contador para los pasos de motor
int stepsPerRev = 4076;  // pasos para una vuelta completa
bool sentido=0; //0 derecha 1 izquierda
unsigned int intervaloMov = 1;
unsigned int velocidadMov = 1;
bool usarDisparador=0;
unsigned int offsetDis;
float duracionDis;

//tablas con la secuencia de encendido (descomentar la que necesiteis)
//secuencia 1-fase
const int numSteps = 4;
const int stepsLookup[4] = { B1000, B0100, B0010, B0001 };
 
//secuencia 2-fases
//const int numSteps = 4;
//const int stepsLookup[4] = { B1100, B0110, B0011, B1001 };
 
//secuencia media fase
//const int numSteps = 8;
//const int stepsLookup[8] = { B1000, B1100, B0100, B0110, B0010, B0011, B0001, B1001 };
 
 
void setup()
{
  Serial.begin(9600);
  while (!Serial);             // Leonardo: wait for serial monitor
  Serial.println("\nValores capturados");
  
  //declarar pines como salida de motor
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  
  //declarar pines como salida de disparador
  pinMode(Enfocar, OUTPUT);
  pinMode(Disparar, OUTPUT);

  //declarar pines como rntrada de pulsadores
  pinMode(botonOK, INPUT);
  pinMode(botonDE, INPUT);
  pinMode(botonIZ, INPUT);
  
  //Display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  //display.display();
  display.clearDisplay(); 

  // inicializo menu de seleccion
  //attachInterrupt(digitalPinToInterrupt(botonOK), pausa, CHANGE);
  menu();
  while (digitalRead(botonOK)) delay(1);
  // obtengo:
  // sentido => 0=der 1=izq
  // intervaloMov == tiempo entre cada movimiento
  // velocidadMov == numero de semirotaciones por cada movimiento
  // usarDiaparador => 0=no 1=si
  // offsetDis == tiempo desde que se mueve hasta que dispara
  // duracionDis == duracion del disparo
}
 
void loop()
{
  /*
  sentido        = 1;  //=> 0=der 1=izq
  intervaloMov   = 15;  //== tiempo entre cada movimiento
  velocidadMov   = 10; //== numero de semirotaciones por cada movimiento
  usarDisparador = 1;  //=> 0=no 1=si
  offsetDis      = 2;  //== tiempo desde que se mueve hasta que dispara
  duracionDis    = 3.1;  //== duracion del disparosentido => 0=der 1=izq
  */
  while (!digitalRead(botonOK)) {
  int i;
  //mover motor ///////////////////////
    for (i = 0; i < velocidadMov; i++)
    {
      if (sentido) anticlockwise(); //derecha
      else clockwise();             //izquierda
      display.ssd1306_command(SSD1306_SETCONTRAST);
      display.ssd1306_command(125);
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.println("Moviendo:");
      //display.setCursor(32,32);
      //display.setTextSize(2);
      //display.setTextColor(WHITE);
      display.print(i);
      display.print(" de ");
      display.println(velocidadMov);
      display.display();
      if (pausa()) break;
    }
    if (usarDisparador) camara();
    //esperar siguiente mov ///////////////////////
    else { 
      //delay(intervaloMov*1000)
      for (i = 0; i < intervaloMov; i++)
      {
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.println("Esperando");
        display.print(i);
        display.print(" de ");
        display.print(intervaloMov);
        display.println(" seg.");
        display.display();
        if (pausa()) break;
        delay(1000);
      }
    }
  }
  menu();
}
void camara(){
  //offsetDis      = 2;  //== tiempo desde que se mueve hasta que dispara
  //duracionDis    = 1;
  bool disparado=0;
  for (unsigned int i = 0; i < intervaloMov; i++)
    {
      if (i <= offsetDis || i > offsetDis+ int(duracionDis)){
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.println("Esperando");
        display.print(i);
        display.print(" de ");
        if (i <= offsetDis) display.print(offsetDis);
        else display.print(intervaloMov);
        display.println(" seg.");
        display.display();
        if (pausa()) break;
        delay(1000);
      }
      else if (!disparado){
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.println("Disparando");
        display.print(duracionDis);
        display.println(" seg.");
        display.display();
        digitalWrite(Enfocar,  HIGH);  //activo enfoque 
        digitalWrite(Disparar, HIGH); //Disparo
        delay(duracionDis * 1000);
        digitalWrite(Disparar, LOW); //Fin Disparo
        digitalWrite(Enfocar,  LOW);  //fin enfoque 
        disparado=1;
        i = i + int(duracionDis + 0.9);
        if (pausa()) break;
      }
    }
}

bool pausa(){
  bool sino = 0;
  if (digitalRead(botonOK)){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("PAUSA ir a menu?");
    display.display();
    display.setTextSize(2);
    pintaSino(0);
    while (digitalRead(botonOK)) delay(1);
    while (!digitalRead(botonOK)) {
      if (digitalRead(botonIZ)) {
        pintaSino(1);
        sino=1; //1 = si
      }
      if (digitalRead(botonDE)) {
        pintaSino(0);
        sino=0; //0 = no
      }
    }
  }
  return sino;
}

void menu()
{
  //noInterrupts();
  //detachInterrupt(botonOK);
  bool test = 0;
  while (digitalRead(botonOK)) delay(1);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("CONFIGURAC");
  display.println("----------");
  display.display();
  delay(3000);
  /////////////////////////////////////////////
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(3.141592);
  float duracionDis=0.3;
  display.println(duracionDis);
  display.display();
  //delay(10000);
  //////////////////////////////////////////////
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.println(" Selecciona Sentido:");
  display.display();  
  display.setTextSize(3);
  
  //seleciona izq der ---------------------------------------------
  pintaFlecha(0);
  //estadoOK = digitalRead(botonOK);
  while (!digitalRead(botonOK) && test==0) {
    //estadoDE = digitalRead(botonDE);
    //estadoIZ = digitalRead(botonIZ);
    if  (digitalRead(botonIZ)) {
      pintaFlecha(1);
      sentido=1; //1 = izquierda
    }
    if  (digitalRead(botonDE)) {
      pintaFlecha(0);
      sentido=0; //0=derecha
    }
    //estadoOK = digitalRead(botonOK);
  }
  while (digitalRead(botonOK)) delay(1);
  Serial.print("sentido=");
  Serial.println(sentido);
  
  //intervalo entre movimiento ----------------------------------------------
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Intervalo movimiento:");
  intervaloMov=5;
  display.setTextSize(1);
  display.setCursor(74, 14);
  display.println("Segundos");
  display.setCursor(74, 24);
  display.println(" 1-999");
  display.setTextSize(3);
  pintaValor(15, 12, intervaloMov, 0);
  //estadoOK = digitalRead(botonOK);
  int cont;
  while (!digitalRead(botonOK) && test==0) {
    //estadoIZ = digitalRead(botonIZ);
    cont=0;
    while (digitalRead(botonIZ) && intervaloMov > 1) {
      intervaloMov--;
      pintaValor(15, 12, intervaloMov, cont);
      cont++;
      //estadoIZ = digitalRead(botonIZ);
    }
    //estadoDE = digitalRead(botonDE);
    cont=0;
    while  (digitalRead(botonDE) && intervaloMov < 999) {
      intervaloMov++;
      pintaValor(15, 12, intervaloMov, cont);
      cont++;
      //estadoDE = digitalRead(botonDE);
    }
    //estadoOK = digitalRead(botonOK);
  }
  while (digitalRead(botonOK)) delay(1);
  Serial.print("intervaloMov=");
  Serial.println(intervaloMov);
  
  //velocidad Movimiento --------------------------------------------
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Velocidad movimiento:");
  velocidadMov=50;
  display.setTextSize(1);
  display.setCursor(65, 12);
  display.println("vueltas/");
  display.setCursor(65, 24);
  display.println("intevalo");
  display.setTextSize(3);
  pintaValor(5, 12, velocidadMov, 1);
  //estadoOK = digitalRead(botonOK);
  while (!digitalRead(botonOK) && test==0) {
    //estadoIZ = digitalRead(botonIZ);
    cont=0;
    while (digitalRead(botonIZ) && velocidadMov > 1) {
      velocidadMov--;
      pintaValor(5, 12, velocidadMov, cont);
      cont++;
      //estadoIZ = digitalRead(botonIZ);
    }
    //estadoDE = digitalRead(botonDE);
    cont=0;
    while  (digitalRead(botonDE) && velocidadMov < 999) {
      velocidadMov++;
      pintaValor(5, 12, velocidadMov, cont);
      cont++;
      //estadoDE = digitalRead(botonDE);
    }
    //estadoOK = digitalRead(botonOK);
  }
  while (digitalRead(botonOK)) delay(1);
  Serial.print("velocidadMov=");
  Serial.println(velocidadMov);

  //seleciona disparador o no ---------------------------------------------
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.println("Usar Disparador?");
  display.display();  
  display.setTextSize(3);
  pintaSino(1);
  usarDisparador=1;
  //estadoOK = digitalRead(botonOK);
  while (digitalRead(botonOK) && test==0) {
    //estadoDE = digitalRead(botonDE);
    //estadoIZ = digitalRead(botonIZ);
    if  (digitalRead(botonIZ)) {
      pintaSino(1);
      usarDisparador=1; //1 = si
    }
    if  (digitalRead(botonDE)) {
      pintaSino(0);
      usarDisparador=0; //0 = no
    }
    //estadoOK = digitalRead(botonOK);
  }
  while (digitalRead(botonOK)) delay(1);
  Serial.print("usarDisparador=");
  Serial.println(usarDisparador);
  
  //offset Disparo --------------------------------------------
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Offset disparo:");
  offsetDis=0;
  display.setTextSize(1);
  display.setCursor(74, 14);
  display.println("Segundos");
  display.setCursor(74, 24);
  display.print(" 0 - ");
  display.println(intervaloMov-2);
  display.setTextSize(3);
  pintaValor(15, 12, offsetDis, 1);
  //estadoOK = digitalRead(botonOK);
  while (!digitalRead(botonOK) && test==0) {
    //estadoIZ = digitalRead(botonIZ);
    cont=0;
    while  (digitalRead(botonIZ) && offsetDis > 0) {
      offsetDis--;
      pintaValor(15, 12, offsetDis, cont);
      cont++;
      //estadoIZ = digitalRead(botonIZ);
    }
    
    //estadoDE = digitalRead(botonDE);
    cont=0;
    while  (digitalRead(botonOK) && offsetDis < (intervaloMov-2)) {
      offsetDis++;
      pintaValor(15, 12, offsetDis, cont);
      cont++;
      //estadoDE = digitalRead(botonDE);
    }
    //estadoOK = digitalRead(botonOK);
  }
  while (digitalRead(botonOK)) delay(1);
  Serial.print("offsetDis=");
  Serial.println(offsetDis);
  
  //Duracion del Disparo --------------------------------------------
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Duracion del disparo:");
  duracionDis=0.1;
  display.setTextSize(1);
  display.setCursor(74, 14);
  display.println("Segundos");
  display.setCursor(74, 24);
  display.print(" 0 - ");
  display.println(intervaloMov-offsetDis-1);
  display.setTextSize(3);
  pintaValor(15, 12,  duracionDis, 1);
  //estadoOK = digitalRead(botonOK);
  while (!digitalRead(botonOK) && test==0) {
    //estadoIZ = digitalRead(botonIZ);
    cont=0;
    while  (digitalRead(botonIZ) && duracionDis > 0.1) {
      if (duracionDis < 0.9){
        duracionDis -= 0.1;
        pintaValorFloat(15, 12, duracionDis, cont);
      }
      else {
        duracionDis--;
        pintaValor(15, 12, duracionDis, cont);
      }
      cont++;
      //estadoIZ = digitalRead(botonIZ);
    }
    //estadoDE = digitalRead(botonDE);
    cont=0;
    while  (digitalRead(botonDE) && duracionDis < (intervaloMov-offsetDis-1)) {
      if (duracionDis < 0.9) {
        duracionDis += 0.1;
        pintaValorFloat(15, 12, duracionDis, cont);
      }
      else {
        duracionDis++;
        pintaValor(15, 12, duracionDis, cont);
      }
      cont++;
      //estadoDE = digitalRead(botonDE);
    }
    //estadoOK = digitalRead(botonOK);
  }
  while (digitalRead(botonOK)) delay(1);
  Serial.print("duracionDis=");
  Serial.println(duracionDis);
  
  ///////devuelvo a loop////////////////////////
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("OK para");
  display.println("comenzar");
  display.display();
  while (!digitalRead(botonOK)) delay(1);
  delay(1000);
  //attachInterrupt(digitalPinToInterrupt(botonOK), pausa, CHANGE);
}

void pintaValor(int posx, int posy, int valor, unsigned int cont ) //0=der, 1=izq
{
  display.setTextColor(WHITE, BLACK);
  display.setCursor(posx, posy);
  display.print("   ");
  display.setCursor(posx, posy);
  display.print(valor);
  display.display(); 
  if (cont<5) delay(200);
}

void pintaValorFloat(int posx, int posy, float valor, unsigned int cont ) //0=der, 1=izq
{
  display.setTextColor(WHITE, BLACK);
  display.setCursor(posx, posy);
  display.print("   ");
  display.setCursor(posx, posy);
  display.print(valor, 1);
  display.display(); 
  if (cont<5) delay(200);
}
void pintaFlecha(int dir) //0=der, 1=izq
{
  if (dir==0) display.setTextColor(WHITE, BLACK);
  else display.setTextColor(BLACK, WHITE);
  display.setCursor(40,12);
  //display.write(17);
  display.print("<");
  
  if (dir==0) display.setTextColor(BLACK, WHITE);
  else display.setTextColor(WHITE, BLACK);
  display.setCursor(74,12);
  //display.write(16);
  display.print(">");
  display.display(); 
}

void pintaSino(int dir) //0=der, 1=izq
{
  if (dir==0) display.setTextColor(WHITE, BLACK);
  else display.setTextColor(BLACK, WHITE);
  display.setCursor(20,12);
  display.print("SI");
  
  if (dir==0) display.setTextColor(BLACK, WHITE);
  else display.setTextColor(WHITE, BLACK);
  display.setCursor(85,12);
  display.print("NO");
  display.display(); 
}

void clockwise()
{
  stepCounter++;
  if (stepCounter >= numSteps) stepCounter = 0;
  setOutput(stepCounter);
}
 
void anticlockwise()
{
  stepCounter--;
  if (stepCounter < 0) stepCounter = numSteps - 1;
  setOutput(stepCounter);
}
 
void setOutput(int step)
{
  digitalWrite(motorPin1, bitRead(stepsLookup[step], 0));
  digitalWrite(motorPin2, bitRead(stepsLookup[step], 1));
  digitalWrite(motorPin3, bitRead(stepsLookup[step], 2));
  digitalWrite(motorPin4, bitRead(stepsLookup[step], 3));
}
