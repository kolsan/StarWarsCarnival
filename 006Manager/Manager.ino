//Comunicacion i2c
//1-manager
//2 diana inferior izqda 3 diana inferior central 4 diana inferior dcha
//5 diana superior medio 6 diana superior alta

#include <Wire.h>

String inSerial;
const int buttonPinStop = 2;
const int buttonPinStart = 3;
const int ledPin =  13;      // the number of the LED pin
int buttonState = 0;
int inByte;
char MessageReceived;
int Shoots = 0;
String texto;
String SerialString;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPinStop, INPUT);
  pinMode(buttonPinStart, INPUT);
  Wire.begin(1); // join i2c bus (address optional for master)
  Wire.onReceive(receiveEvent); // register event
  
}





void loop() {





  buttonState = digitalRead(buttonPinStart);

  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);

    // Now listen on the second port

    Serial.println("Comienza Partida");
    Wire.beginTransmission(2); // transmit to device #0 means broadcast
    Wire.write("go");
    Wire.endTransmission();    // stop transmitting

    Wire.beginTransmission(3);
    Wire.write("go");
    Wire.endTransmission();

       Wire.beginTransmission(4);
    Wire.write("go");
    Wire.endTransmission();

    Wire.beginTransmission(5);
    Wire.write("go");
    Wire.endTransmission();
    
    Wire.beginTransmission(6);
    Wire.write("go");
    Wire.endTransmission();
    delay (500);

  }
  else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }

  buttonState = digitalRead(buttonPinStop);

  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);

    Wire.beginTransmission(2);
    Wire.write("end");
    Wire.endTransmission();

    Wire.beginTransmission(3);
    Wire.write("end");
    Wire.endTransmission();

        Wire.beginTransmission(4);
    Wire.write("end");
    Wire.endTransmission();

    Wire.beginTransmission(5);
    Wire.write("end");
    Wire.endTransmission();

        Wire.beginTransmission(6);
    Wire.write("end");
    Wire.endTransmission();
    delay (500);
    Shoots = 0;

  

  }
  else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }


  if (texto != "")
  {
    Serial.println (texto);
    texto = "";


  }

  getSerialData ();
    WorkWithReceivedData ();

    SerialString = "";

  // int x = Wire.read();    // receive byte as an integer
  //  Serial.println(x);         // print the integer

}

void receiveEvent(int howMany) {
  while ( Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    texto = texto + c;
    //Serial.print(c);         // print the character

  }

}

void getSerialData ()
{


  while (Serial.available())
  {
    delay(3);  //delay to allow buffer to fill

    if (Serial.available() > 0) {
      char c = Serial.read();  //gets one byte from serial buffer
      //Serial.print (c); //makes the string readString
      SerialString += c;

    }

  }
  
}
void WorkWithReceivedData ()
{

  if ((SerialString[0] == '1'))
  {
    Wire.beginTransmission(2); // transmit to device #0 means broadcast
    Wire.write("go");
    Wire.endTransmission();    // stop transmitting

    Wire.beginTransmission(3);
    Wire.write("go");
    Wire.endTransmission();

        Wire.beginTransmission(4);
    Wire.write("go");
    Wire.endTransmission();

    Wire.beginTransmission(5);
    Wire.write("go");
    Wire.endTransmission();

        Wire.beginTransmission(6);
    Wire.write("go");
    Wire.endTransmission();
    delay (500);
  }
  if ((SerialString[0] == '2'))
  {
    Wire.beginTransmission(2); // transmit to device #0 means broadcast
    Wire.write("end");
    Wire.endTransmission();    // stop transmitting

    Wire.beginTransmission(3);
    Wire.write("end");
    Wire.endTransmission();

        Wire.beginTransmission(4);
    Wire.write("end");
    Wire.endTransmission();

    Wire.beginTransmission(5);
    Wire.write("end");
    Wire.endTransmission();

        Wire.beginTransmission(6);
    Wire.write("end");
    Wire.endTransmission();
    delay (500);
  }
  if ((SerialString[0] == '3'))
  {
    Wire.beginTransmission(2); // transmit to device #0 means broadcast
    Wire.write("gup");
    Wire.endTransmission();    // stop transmitting

    Wire.beginTransmission(3);
    Wire.write("gup");
    Wire.endTransmission();

        Wire.beginTransmission(4);
    Wire.write("gup");
    Wire.endTransmission();

    Wire.beginTransmission(5);
    Wire.write("gup");
    Wire.endTransmission();

        Wire.beginTransmission(6);
    Wire.write("gup");
    Wire.endTransmission();
    delay (500);
  }
  if ((SerialString[0] == '4'))
  {
    Wire.beginTransmission(2); // transmit to device #0 means broadcast
    Wire.write("gdo");
    Wire.endTransmission();    // stop transmitting

    Wire.beginTransmission(3);
    Wire.write("gdo");
    Wire.endTransmission();

        Wire.beginTransmission(4);
    Wire.write("gdo");
    Wire.endTransmission();

    Wire.beginTransmission(5);
    Wire.write("gdo");
    Wire.endTransmission();

       Wire.beginTransmission(6);
    Wire.write("gdo");
    Wire.endTransmission();
    delay (500);
  }
  if ((SerialString[0] == '5'))
  {
    Wire.beginTransmission(2); // transmit to device #0 means broadcast
    Wire.write("gme");
    Wire.endTransmission();    // stop transmitting

    Wire.beginTransmission(3);
    Wire.write("gme");
    Wire.endTransmission();

        Wire.beginTransmission(4);
    Wire.write("gme");
    Wire.endTransmission();

    Wire.beginTransmission(5);
    Wire.write("gme");
    Wire.endTransmission();

       Wire.beginTransmission(6);
    Wire.write("gme");
    Wire.endTransmission();
    delay (500);
  }
  if ((SerialString[0] == '6'))
  {
    Wire.beginTransmission(2); // transmit to device #0 means broadcast
    Wire.write("rst");
    Wire.endTransmission();    // stop transmitting

    Wire.beginTransmission(3);
    Wire.write("rst");
    Wire.endTransmission();

        Wire.beginTransmission(4);
    Wire.write("rst");
    Wire.endTransmission();

    Wire.beginTransmission(5);
    Wire.write("rst");
    Wire.endTransmission();

        Wire.beginTransmission(6);
    Wire.write("rst");
    Wire.endTransmission();
    delay (500);
  }
}



