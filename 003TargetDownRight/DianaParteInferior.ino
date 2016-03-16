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
int UpDegrees = 70;  //Grados a los que la diana esta vertical
int DownDegrees = 10;  //Grados a los que la diana esta abajo
int Shoots = 0; //Guardaremos el numero de diana y en función de ella modificaremos la dificultad
boolean TargetUp;
const int delayForSound = 100;

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
  Wire.begin(4);                // join i2c bus with address #2
  Wire.onReceive(receiveEvent); // register event
  //Serial.println (millis());ç
  //Manager.begin(9600);

}

void (* resetFunc) (void) = 0;


void loop() {
  //Esperamos la señal de que comience la partida -1(49) - Acabar la partida 0 (48) - 2 modo demo (50) - 3 tiro acertado (51)


  if (Playing)
  {



    if ((timer < millis()) && (!TargetUp))
    { //levantamos la diana transcurrido un tiempo aleatorio desde el ultimo blanco
      Wire.beginTransmission(1); // transmit to device #1
      Wire.write("go001");              // sends one byte
      Wire.endTransmission();    // stop transmitting
      delay (delayForSound);
      for (pos = DownDegrees; pos <= UpDegrees; pos += 1) // goes from 0 degrees to 180 degrees
      { // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(5);                       // waits 5ms for the servo to reach the position
      }
      TargetUp = true;

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

    if (timer + (time_between_target * 1000) < millis()) //Si ha pasado el tiempo marcado desde el ultimo tiro, bajamos la diana
    {
      //      Serial.println (timer);
      //      Serial.println (time_between_target * 1000);
      //      Serial.println (timer + (time_between_target * 1000) );
      //      Serial.println (millis());
      //      Serial.println ("Aqui2");
      Wire.beginTransmission(1); // transmit to device #1
      Wire.write("nok001");              // sends one byte
      Wire.endTransmission();    // stop transmitting
      for (pos = UpDegrees; pos >= DownDegrees; pos -= 1)
      {
        myservo.write(pos);
        delay(5);
      }
      TargetUp = false;
      randNumber = random(2, time_between_target);
      //Serial.println (randNumber);
      timer = randNumber * 1000 + millis();
      randomSeed(analogRead(0));
      irrecv.resume(); // Receive the next value
      //      Serial.println (timer);
      //      Serial.println (millis());
      //      Serial.println ("Aqui3");

    }



    if (irrecv.decode(&results) && TargetUp) {
      //Serial.println(results.value, DEC);
      irrecv.resume(); // Receive the next value
      if (results.value == 2704)  //valor que dispara el emisor de infrarrojos (apagado de tele sony)
      {
        //Serial.print ("Blanco: ");
        //Manager.listen();
        Wire.beginTransmission(1); // transmit to device #1
        Wire.write("ok001");              // sends one byte
        Wire.endTransmission();    // stop transmitting
        myservo.write(UpDegrees - 30);
        delay (1500);
        Shoots = Shoots + 1;
        randNumber = random(2, time_between_target);
        //Serial.println (randNumber);
        timer = randNumber * 1000 + millis();
        randomSeed(analogRead(0));
        //Serial.println (timer);
        //Serial.println (millis());
        for (pos = UpDegrees - 30; pos >= DownDegrees; pos -= 1) // goes from 180 degrees to 0 degrees
        {
          myservo.write(pos);              // tell servo to go to position in variable 'pos'
          delay(5);                       // waits 15ms for the servo to reach the position
        }
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
    randomSeed(analogRead(0));
    for (pos = DownDegrees; pos <= UpDegrees; pos += 1) // goes from 0 degrees to 180 degrees
    { // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(5);                       // waits 15ms for the servo to reach the position
      time_between_target = 7;
      Shoots = 0;
      digitalWrite(ledPin, HIGH);

    }
    TargetUp = true;
    timer = millis();
  }
  if (texto == "end")
  {
    Playing = false;
    Serial.println ("Fin de la partida!!");
    for (pos = UpDegrees; pos >= DownDegrees; pos -= 1) // goes from 180 degrees to 0 degrees
    {
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(5);                       // waits 5ms for the servo to reach the position
    }
    TargetUp = false;
    Shoots = 0;
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

    myservo.write(UpDegrees / 2);
  }



  texto = "";


}
