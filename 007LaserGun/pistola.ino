//Para el mega se usa el pin 9
//para el nano se usa el pin 3

#include <IRremote.h>
#include <Wire.h>

IRsend irsend;

int inPin = 2;   // choose the input pin (for a pushbutton)
int val = 0;     // variable for reading the pin status
int ledPin = 7; // choose the pin for the LED
int GoodStatusLed = 5;
int BadStatusLed = 10;
int Num_Max_Tiros = 10;
int Num_Tiros = 1;
const int Vibrador = 4;
unsigned long StartShootSerieTime = millis () -10000;
int TimeBetweenShoots = 10;

char MessageReceived;
String texto;

void setup()
{
  Serial.begin(9600);
  Serial.println ("Comenzamos...");
  pinMode(inPin, INPUT);    // declare pushbutton as input
  pinMode(ledPin, OUTPUT);  // declare LED as output
  pinMode(GoodStatusLed, OUTPUT);  // declare LED as output
  pinMode(BadStatusLed, OUTPUT);  // declare LED as output
  pinMode(Vibrador, OUTPUT);
  Wire.begin(6);                // join i2c bus with address #6
  Wire.onReceive(receiveEvent); // register event
  digitalWrite(Vibrador, HIGH);
  digitalWrite(GoodStatusLed, HIGH);
  digitalWrite(BadStatusLed, HIGH);
  digitalWrite(ledPin, HIGH);
  delay (3000);
  digitalWrite(Vibrador, LOW);
  digitalWrite(GoodStatusLed, HIGH);
  digitalWrite(BadStatusLed, LOW);
  digitalWrite(ledPin, LOW);
  
}


void (* resetFunc) (void) = 0;

void loop() {

  val = digitalRead(inPin);  // read input value

//
//  Serial.println (millis() - StartShootSerieTime); 
//  Serial.println (TimeBetweenShoots * 1000); 

  if (val == LOW) {         // disparo? 
    
    if (Num_Tiros == 10 && (millis() - StartShootSerieTime <= TimeBetweenShoots * 1000))
    {
      Serial.println ("Caliente..");
      digitalWrite(GoodStatusLed, LOW);
      digitalWrite(Vibrador, HIGH);
      for (int i = 0; i < 5; i += 1) // Parpadeamos para que se vea que esta caliente
      {
        digitalWrite(BadStatusLed, HIGH);
        delay (500);
        digitalWrite(BadStatusLed, LOW);
        delay (500);
      }

      digitalWrite(Vibrador, LOW);
      digitalWrite(GoodStatusLed, HIGH);
      digitalWrite(BadStatusLed, LOW);
    }
    else
    {
      digitalWrite(ledPin, HIGH);  // turn LED ON
      irsend.sendSony(0xa90, 12);
      delay(300);
      Num_Tiros = Num_Tiros + 1;
      Serial.print ("Shoot..");
      Serial.println (Num_Tiros);

    }
  }
  else {
    digitalWrite(ledPin, LOW);  // turn LED OFF
  }

  if (Num_Tiros > 10)
  {
      Serial.println (millis() );
      Serial.println ( StartShootSerieTime); 
      Serial.println (TimeBetweenShoots * 1000); 
    Num_Tiros = 0;
    StartShootSerieTime = millis ();
    Serial.println ( StartShootSerieTime); 
  }

}

void receiveEvent(int howMany) {
  while ( Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    texto = texto + c;
    Serial.print(c);         // print the character
  }

  if (texto == "rst")
  {
    resetFunc();
  }


  texto = "";


}

