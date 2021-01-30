#include <Servo.h>
#include <SoftwareSerial.h>
#include "Adafruit_Soundboard.h"


int servoPin = 3;

Servo myservo;

char mystr[5]; //Initialized variable to store recieved data


// Choose any two pins that can be used with SoftwareSerial to RX & TX
#define SFX_TX 5
#define SFX_RX 6

// Connect to the RST pin on the Sound Board
#define SFX_RST 4



// we'll be using software serial
SoftwareSerial ss = SoftwareSerial(SFX_TX, SFX_RX);

// pass the software serial to Adafruit_soundboard, the second
// argument is the debug port (not used really) and the third 
// arg is the reset pin
Adafruit_Soundboard sfx = Adafruit_Soundboard(&ss, NULL, SFX_RST);





void setup() {
  // Begin the Serial at 115200 Baud
  Serial.begin(115200);
  myservo.attach(servoPin);

  // softwareserial at 115200 baud
  ss.begin(9600);
  // can also do Serial1.begin(9600)

  //reset the board each time we start the board, this way we can check if the board is actually powered up when we start the system.
  if (!sfx.reset()) {
    Serial.println("Not found");
    while (1);
  }
  Serial.println("SFX board found");

  
}

void loop() {
  //reset the array every time we reach here, so we dont have any previous data. 
    for(int x=0;x<5;x++)
      mystr[x] = NULL;
      
    if(Serial.available()) //check the seial port for any data
    {
      Serial.println("MainSerial --> ");
      Serial.readBytes(mystr,1); //if vailable read it to the above array
      Serial.println(mystr[0]); //print that data on serial monitor for debugging
      controller_logic(mystr[0]); //calling the following function such that we can perform tasks according to the information received
    }
}

void controller_logic(char UART_input)
{
  switch(UART_input)
  {
    case '0': myservo.write(0);//move to initial position ---> Unlocks the door
            //delay(100); //give sometime for servo to do its job
            break;
    case '1': myservo.write(180);//move 180 degrees ------> locks the door
            //delay(100); //give sometime for servo to do its job
            break;
    case 'a': if (! sfx.playTrack((uint8_t)0)) //Command Adafruit board to play track #0 and check the return value to make sure it actually played the track
                  Serial.println("Failed to play track 0"); //if not played log the error to serial monitor
              break;
    case 'b': if (! sfx.playTrack((uint8_t)1)) //Command Adafruit board to play track #1 and check the return value to make sure it actually played the track
                  Serial.println("Failed to play track 1"); //if not played log the error to serial monitor
              break;            
    case 'c': if (! sfx.playTrack((uint8_t)2)) //Command Adafruit board to play track #2 and check the return value to make sure it actually played the track
                  Serial.println("Failed to play track 2");//if not played log the error to serial monitor
              break;
    case 'd': if (! sfx.playTrack((uint8_t)3)) //Command Adafruit board to play track #3 and check the return value to make sure it actually played the track
                  Serial.println("Failed to play track 3");//if not played log the error to serial monitor
              break;           
    case 'e': if (! sfx.playTrack((uint8_t)4)) //Command Adafruit board to play track #4 and check the return value to make sure it actually played the track
                  Serial.println("Failed to play track 4");//if not played log the error to serial monitor
              break; 
    case 'f': if (! sfx.playTrack((uint8_t)5)) //Command Adafruit board to play track #5 and check the return value to make sure it actually played the track
                  Serial.println("Failed to play track 5");//if not played log the error to serial monitor
              break; 

    case 'g': if (! sfx.playTrack((uint8_t)6)) //Command Adafruit board to play track #6 and check the return value to make sure it actually played the track
                  Serial.println("Failed to play track 6");//if not played log the error to serial monitor
              break; 

    case 'h': if (! sfx.playTrack((uint8_t)7)) //Command Adafruit board to play track #7 and check the return value to make sure it actually played the track
                  Serial.println("Failed to play track 7");//if not played log the error to serial monitor
              break;  

    case 'i': if (! sfx.playTrack((uint8_t)8)) //Command Adafruit board to play track #8 and check the return value to make sure it actually played the track
                  Serial.println("Failed to play track 8");//if not played log the error to serial monitor
              break;
              
    case 'j': if (! sfx.playTrack((uint8_t)9)) //Command Adafruit board to play track #9 and check the return value to make sure it actually played the track
                  Serial.println("Failed to play track 9");//if not played log the error to serial monitor
              break;

    case 'k': if (! sfx.playTrack((uint8_t)10)) //Command Adafruit board to play track #10 and check the return value to make sure it actually played the track
                  Serial.println("Failed to play track 10");//if not played log the error to serial monitor
              break; 

    case '+': uint16_t v;
              if (! (v = sfx.volUp()) ) //command Adafruit board to increase the volume and check the return value to make sure it actaully did it 
                Serial.println("Failed to adjust");
              else
                Serial.print("Volume: "); Serial.println(v);
              break; 

    case '-': uint16_t x;
              if (! (x=sfx.volDown()) ) //command Adafruit board to increase the volume and check the return value to make sure it actaully did it 
                Serial.println("Failed to adjust");
              else {
                Serial.print("Volume: "); 
                Serial.println(v);
              }
              break;                   
                
                       
  }
  
}
