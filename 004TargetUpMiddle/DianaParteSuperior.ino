
//Comunicacion i2c
//1-manager
//2 diana inferior izqda 3 diana inferior central 4 diana inferior dcha
//5 diana superior medio 6 diana superior alta


// Receptor - Seria la diana de la feria
#include <IRremote.h>
#include <Servo.h>
#include <Wire.h>

const int ledPin =  13;
int RECV_PIN = 11;
Servo myservo;
int pos = 15;
boolean Playing;
IRrecv irrecv(RECV_PIN);
char MessageReceived;
unsigned long timer = millis();
unsigned long time_between_target = 7;
int randNumber = 0;
int UpDegrees = 160;  //Grados a los que la diana esta vertical
int Shoots = 0; //Guardaremos el numero de diana y en función de ella modificaremos la dificultad
boolean TargetUp;

int DownDegrees = 15;  //Grados a los que la diana esta vertical
int ShootAvailableDegrees = 82; //Grados a los que la diana esperará el tiro.
boolean DirectionRightToLeft = true;
String texto;
decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  myservo.attach(9);
  //Serial.println ("Comenzamos...");
  myservo.write(DownDegrees);
  delay(15);
  Playing = false;
  pinMode(ledPin, OUTPUT);
  Wire.begin(5);                // join i2c bus with address #2
  Wire.onReceive(receiveEvent); // register event
  //Serial.println (millis());ç
  //Manager.begin(9600);
  Playing = false;
  //TargetUp = true;

}

void (* resetFunc) (void) = 0;


void loop() {



  if (Playing)
  {

    if ((timer < millis()) && (!TargetUp))
    { //levantamos la diana transcurrido un tiempo aleatorio desde el ultimo blanco
      Wire.beginTransmission(1); // transmit to device #1
      Wire.write("go004");              // sends one byte
      Wire.endTransmission();    // stop transmitting
      if (DirectionRightToLeft)
      { //Serial.println ("pos1");
        for (pos = DownDegrees; pos <= ShootAvailableDegrees; pos += 1) // goes from 0 degrees to 180 degrees
        { // in steps of 1 degree
          myservo.write(pos);              // tell servo to go to position in variable 'pos'
          delay(5);                       // waits 5ms for the servo to reach the position
        }
      }
      else
      {
        for (pos = UpDegrees; pos >= ShootAvailableDegrees; pos -= 1)
        {
          myservo.write(pos);
          delay(5);
        }
      }
      TargetUp = true;
      //Serial.println (pos);
      //bajaremos la diana en 1-5 segundos depende de la dificultad

      switch (Shoots) {
        case  2:
          time_between_target = 5;
          //Serial.println (time_between_target);
          break;
        case  3:
          time_between_target = 4;
          //Serial.println (time_between_target);
          break;

        case  5:
          time_between_target = 3;
          //Serial.println (time_between_target);
          break;

        case  7:
          time_between_target = 1;
          //Serial.println (time_between_target);
          break;
      }
    }

    if (timer + (time_between_target * 1000) < millis() && TargetUp) //Si ha pasado el tiempo marcado desde el ultimo tiro, la nave continua
    {
      //      Serial.println (timer);
      //      Serial.println (time_between_target * 1000);
      //      Serial.println (timer + (time_between_target * 1000) );
      //      Serial.println (millis());
      //      Serial.println ("Aqui2");
      Wire.beginTransmission(1); // transmit to device #1
      Wire.write("nok004");              // sends one byte
      Wire.endTransmission();    // stop transmitting
      if (DirectionRightToLeft)
      { //Serial.println ("pos2");
        for (pos = ShootAvailableDegrees; pos <= UpDegrees; pos += 1)
        {
          myservo.write(pos);
          delay(5);
        }
      }
      else
      {
        for (pos = ShootAvailableDegrees ; pos >= DownDegrees; pos -= 1)
        {
          myservo.write(pos);
          delay(5);
        }
      }
      DirectionRightToLeft = !DirectionRightToLeft;
      TargetUp = false;
      randNumber = random(2, time_between_target);
      //Serial.println (randNumber);
      timer = randNumber * 1000 + millis();
      randomSeed(analogRead(0));

      //      Serial.println (timer);
      //      Serial.println (millis());
      //      Serial.println ("Aqui3");
      irrecv.resume(); // Receive the next value
    }


    //delay (200);
    if (irrecv.decode(&results) && TargetUp) {
      //Serial.println(results.value, DEC);
      irrecv.resume(); // Receive the next value
      //Serial.print (millis());
      if (results.value == 2704)  //valor que dispara el emisor de infrarrojos (apagado de tele sony)
      {
        Serial.print ("Blanco: ");
        //Serial.print (millis());
        //Manager.listen();
        Wire.beginTransmission(1); // transmit to device #1
        Wire.write("ok004");              // sends one byte
        Wire.endTransmission();    // stop transmitting

        Shoots = Shoots + 1;
        randNumber = random(2, time_between_target);
        //Serial.println (randNumber);
        timer = randNumber * 1000 + millis();
        randomSeed(analogRead(0));
        //Serial.println (timer);
        //Serial.println (millis());
        if (DirectionRightToLeft)
        {
          //Serial.println ("Aqui 1");
          myservo.write(ShootAvailableDegrees - 20);
          delay(1500);
          for (pos = ShootAvailableDegrees - 10 ; pos >= DownDegrees; pos -= 1)
          {
            myservo.write(pos);
            delay(2);
            //Serial.println ("Aqui 2");
          }
        }
        else
        { //Serial.println ("Aqui 3");
          myservo.write(ShootAvailableDegrees + 20);
          delay(1500);
          for (pos = ShootAvailableDegrees + 10; pos <= UpDegrees; pos += 1)
          {
            myservo.write(pos);
            delay(2);
            //Serial.println ("Aqui 4");
          }
        }
        delay (500);
        TargetUp = false;
        irrecv.resume();
        //Serial.println (pos);
        //delay(300);
      }
    }

  }


}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
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
  if (texto == "go")
  {
    Playing = true;
    Serial.println ("Comienza la partida!!");
    //    randomSeed(analogRead(0));
    //    for (pos = 0; pos <= UpDegrees; pos += 1) // goes from 0 degrees to 180 degrees
    //    { // in steps of 1 degree
    //      myservo.write(pos);              // tell servo to go to position in variable 'pos'
    //      delay(5);                       // waits 15ms for the servo to reach the position
    //      time_between_target = 7;
    //      Shoots = 0;
    //
    //
    //    }
    digitalWrite(ledPin, HIGH);
    TargetUp = false;
    timer = millis();
    DirectionRightToLeft = true;
    myservo.write(DownDegrees);              // tell servo to go to position in variable 'pos'
    delay(300);                       // waits 5ms for the servo to reach the position
  }
  if (texto == "end")
  {
    Playing = false;
    Serial.println ("Fin de la partida!!");
    //    for (pos = UpDegrees; pos >= 0; pos -= 1) // goes from 180 degrees to 0 degrees
    //    {
    myservo.write(DownDegrees);              // tell servo to go to position in variable 'pos'
    delay(300);                       // waits 5ms for the servo to reach the position
    //    }
    TargetUp = false;
    Shoots = 0;
    time_between_target = 7;
    digitalWrite(ledPin, LOW);

  }
  if (texto == "gup")
  {

    myservo.write(UpDegrees);
  }
  if (texto == "gdo")
  {

    myservo.write(DownDegrees);
  }
  if (texto == "gme")
  {

    myservo.write(ShootAvailableDegrees);
  }
  texto = "";


}
