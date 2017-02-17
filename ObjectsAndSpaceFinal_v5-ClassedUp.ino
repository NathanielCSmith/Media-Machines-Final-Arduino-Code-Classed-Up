
/*************************
  Course | Objects and Space
  Verison | 0.0
  Date | 2016.12.12
  Project |
  Name | Nathaniel Smith, Weiwei Hsu
*************************/
// LIBRARIES //
#include <Adafruit_DotStar.h>
#include <SPI.h>
/*
  TEAM 01 = BLUE TEAM
  TEAM 02 = RED TEAM

  Button 01 LED pin 12 //player red (right)bottom button led
  Button 02 LED pin 11 //player red (left)top button led
  Button 03 LED Pin 10 //player blue (left)top button led
  Button 04 LED Pin 09 //player blue (right) bottom button led

  Switch attached to Button 04 pin 05 //player blue (left) bottom button
  Switch attached to Button 03 pin 06 //player blue (right) top button
  Switch attached to Button 02 pin 07 //player red (right) top button
  Switch attached to Button 01 pin 08 //player red (left)bottom button

  //player red dot star
  //Di pin 13
  //CI pin 4

  //player blue dot star
  //Di pin 3
  //CI pin 2

  Team 01 Strip
  - Clock Pin
  - Data Pin
  Team 02 Strip
  - Clock Pin
  - Data Pin
*/

// VARAIBLES //
#define DATA_REDTEAM 31
#define CLOCK_REDTEAM 29
#define DATA_BLUETEAM 3
#define CLOCK_BLUETEAM 2
Adafruit_DotStar team01LEDstrip = Adafruit_DotStar(144, DATA_BLUETEAM, CLOCK_BLUETEAM, DOTSTAR_BRG); //WS2801 LED Strip for Team 01 (side with power port) //assign var data pin & clock pin
Adafruit_DotStar team02LEDstrip = Adafruit_DotStar(144, DATA_REDTEAM, CLOCK_REDTEAM, DOTSTAR_BRG); //WS2801 LED Strip for Team 02 (side without power port) //assign var data pin & clock pin

// LEDS
const int team02LeftButtonLED = 11; //Left is top button
const int team02RightButtonLED = 12; //Right button is bottom button
const int team01LeftButtonLED = 9;
const int team01RightButtonLED = 10;

// Buttons
const int team02LeftButtonPin = 7;
const int team02RightButtonPin = 8;
const int team01LeftButtonPin = 5;
const int team01RightButtonPin = 6;

// States
int team01ButtonPushCount = 0; // Total Push count for team
int team02ButtonPushCount = 0; // Total Push count for team
int team01LeftButtonState = 0; // Tracks current button state
int team01RightButtonState = 0; // Tracks current button state
int team02LeftButtonState = 0; // Tracks current button state
int team02RightButtonState = 0; // Tracks current button state
int team01LeftButtonLastState = 0; // Tracks perivious buttons state
int team01RightButtonLastState = 0; // Tracks perivious buttons state
int team02LeftButtonLastState = 0; // Tracks perivious buttons state
int team02RightButtonLastState = 0; // Tracks perivious buttons state

int team01PushCount = 0;
int team02PushCount = 0;

int team01LeftPushCount = 0;
int team01RightPushCount = 0;
int team02LeftPushCount = 0;
int team02RightPushCount = 0;

//put outside of loop

// TEAM COLORS TO ANIMATE
int redcolor = 0xFF0000 ; //PURE RED
int redcolortail = 0xFF0033;
int bluecolor = 0x0000FF; //PURE BLUE
int bluecolortail = 0x9900FF;

int DELAY = 1000;
unsigned long previousMillis = 0;
long interval = 10000;

//put all LED FUNCTIONS ABOVE THIS LINE


class StripAnimation {
  private:
    Adafruit_DotStar *strip;
    int start;
    int end;

    int head, tail;

    int headColor, tailColor;
    bool isAnimating = false;

  public:
    StripAnimation(Adafruit_DotStar *strip_in, int start_in, int end_in, int headColor_in, int tailColor_in) {
      strip = strip_in;
      start = start_in;
      end = end_in;
      head = start_in;
      tail = start_in - 10;
      headColor = headColor_in;
      tailColor = tailColor_in;
    }

    void setAnimate(bool v) {
      isAnimating = v;
    }

    void animate() {
      if (isAnimating) {
        //  This is the animation part and it has a reference in //variables start for the color animations
        strip->setPixelColor(head, headColor);
        if (tail >= start) {
          strip->setPixelColor(tail, tailColor);
        }
        strip->show();
        //  delay(35);                        // Pause 20 milliseconds (~50 FPS)

        if (++head >= end) {        // Increment head index.  Off end of strip?
          head = start;                       //  Yes, reset head index to start
        }
        if (++tail >= end) {
          tail = start;
        }
      } else { //dont do the animation and reset if not time
        for (int i = start; i <= end; ++i) {
          strip->setPixelColor(i, 0);
        }
        strip->show();
        head = start;
        tail = end - 1;
      }
    }
};

int redHead = 0xFF0000;
int redTail = 0xFF0099;
int blueHead = 0x0000FF;
int blueTail = 0x9900FF;

StripAnimation red1(&team01LEDstrip, 0, 35, redHead, redTail);
StripAnimation red2(&team01LEDstrip, 36, 36 + 35, redHead, redTail);
StripAnimation red3(&team01LEDstrip, 72, 72 + 35, redHead, redTail);
StripAnimation red4(&team01LEDstrip, 98, 98 + 35, redHead, redTail);

StripAnimation blue1(&team02LEDstrip, 0, 35, blueHead, blueTail);
StripAnimation blue2(&team02LEDstrip, 36, 36 + 35, blueHead, blueTail);
StripAnimation blue3(&team02LEDstrip, 72, 72 + 35, blueHead, blueTail);
StripAnimation blue4(&team02LEDstrip, 98, 98 + 35, blueHead, blueTail);


void setup() {

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  // Define Pins
  pinMode(team01LeftButtonPin, INPUT);
  pinMode(team01RightButtonPin, INPUT);
  pinMode(team02LeftButtonPin, INPUT);
  pinMode(team02RightButtonPin, INPUT);

  pinMode(team01LeftButtonLED, OUTPUT);
  pinMode(team01RightButtonLED, OUTPUT);
  pinMode(team01LeftButtonLED, OUTPUT);
  pinMode(team02RightButtonLED, OUTPUT);

  // Start DotStar
  team01LEDstrip.begin();
  team01LEDstrip.show();
  team02LEDstrip.begin();
  team02LEDstrip.show();

  Serial.begin(230400); // USE THIS SAME NUMBER IN PROCESSING
  Serial.println("Ready Player One");
}

void loop() {
  //  WTF();
  theGame();
  Serial.println("Hello");
  //  delay(DELAY);

}

void stopAnimating() {
  red1.setAnimate(false);
  red2.setAnimate(false);
  red3.setAnimate(false);
  red4.setAnimate(false);

  blue1.setAnimate(false);
  blue2.setAnimate(false);
  blue3.setAnimate(false);
  blue4.setAnimate(false);

}

void theGame() {
  Serial.println("Im in the game statement");

  // Trigger Statements for LEDS and Serial
  if (team01PushCount >= 10) {
    blue1.setAnimate(true);
    //    bluestrip1animating = true; //change this back when led strips work!!!!!
    Serial.println ("team01\t10"); //this is how we use serial instead of firmatat F firmata!
  }
  if (team01PushCount >= 20) {
    blue2.setAnimate(true);
    //    bluestrip1animating = true;
    Serial.println ("team01\t20"); //this is how we use serial instead of firmatat F firmata!
  }
  if (team01PushCount >= 30) {
    blue3.setAnimate(true);
    //    bluestrip3animating = true;
    Serial.println ("team01\t30"); //this is how we use serial instead of firmatat F firmata!
  }
  if (team01PushCount >= 40) {
    blue4.setAnimate(true);
    //    bluestrip4animating = true;
    Serial.println ("team01\t40"); //this is how we use serial instead of firmatat F firmata!
  }

  if (team02PushCount >= 10) {
    red1.setAnimate(true);
    //    redstrip1animating = true;
    Serial.println ("team02\t10"); //this is how we use serial instead of firmata F firmata!
  }
  if (team02PushCount >= 20) {
    red2.setAnimate(true);
    //    redstrip2animating = true;
    Serial.println ("team02\t20"); //this is how we use serial instead of firmatat F firmata!
  }
  if (team02PushCount >= 30) {
    red3.setAnimate(true);
    //    redstrip3animating = true;
    Serial.println ("team02\t30"); //this is how we use serial instead of firmatat F firmata!
  }
  if (team02PushCount >= 40) {
    red4.setAnimate(true);
    //    redstrip4animating = true;
    Serial.println ("team02\t40"); //this is how we use serial instead of firmatat F firmata!
  }

  team01LeftButtonState = digitalRead(team01LeftButtonPin);
  if (team01LeftButtonState != team01LeftButtonLastState) {
    if (team01LeftButtonState == HIGH) {
      team01LeftPushCount++;
      Serial.print("Team 1 Left = ");
      Serial.println(team01LeftPushCount);
      digitalWrite(team01LeftButtonLED, HIGH);

    } else {
      Serial.println("Team 01 Button Left | Low");
      digitalWrite(team01LeftButtonLED, LOW);
    }
    team01LeftButtonLastState = team01LeftButtonState;
    delay(10);
  }
  team01RightButtonState = digitalRead(team01RightButtonPin);
  if (team01RightButtonState != team01RightButtonLastState) {
    if (team01RightButtonState == HIGH) {
      team01RightPushCount++;
      Serial.print("Team 1 Right =");
      Serial.println(team01RightPushCount);
      digitalWrite(team01RightButtonLED, HIGH);

    } else {
      Serial.println("Team 01 Button Right | Low");
      digitalWrite(team01RightButtonLED, LOW);
    }
    team01RightButtonLastState = team01RightButtonState;
    delay(10);
  }

  team02LeftButtonState = digitalRead(team02LeftButtonPin);
  if (team02LeftButtonState != team02LeftButtonLastState) {
    if (team02LeftButtonState == HIGH) {
      team02LeftPushCount++;
      Serial.print("Team 2 Left =");
      Serial.println(team02LeftPushCount);
      digitalWrite(team02LeftButtonLED, HIGH);

    } else {
      Serial.println("Team 02 Button Left | Low");
      digitalWrite(team02LeftButtonLED, LOW);
    }
    team02LeftButtonLastState = team02LeftButtonState;
    delay(10);
  }

  team02RightButtonState = digitalRead(team02RightButtonPin);
  if (team02RightButtonState != team02RightButtonLastState) {
    if (team02RightButtonState == HIGH) {
      team02RightPushCount++;
      Serial.print("Team 2 Right = ");
      Serial.println(team02RightPushCount);
      digitalWrite(team02RightButtonLED, HIGH);

    } else {
      Serial.println("Team 02 Button Left | Low");
      digitalWrite(team02RightButtonLED, LOW);
    }
    team02RightButtonLastState = team02RightButtonState;
    delay(10);
  }

  team01PushCount = (team01LeftPushCount + team01RightPushCount);
  Serial.print("Team 1 Total = ");
  Serial.println(team01PushCount);
  team02PushCount = (team02LeftPushCount + team02RightPushCount);
  Serial.print("Team 2 Total = ");
  Serial.println(team02PushCount);


  //  What happen is Someone Wins?
  if (team01PushCount >= 60 || team01PushCount >= 60) {
    for (int i = 0; i <= 144; ++i) {
      team01LEDstrip.setPixelColor(i, 0);
      team02LEDstrip.setPixelColor(i, 0);
    }
    team01LEDstrip.show();
    team02LEDstrip.show();

    team01LeftPushCount = 0;
    team01RightPushCount = 0;
    team02LeftPushCount = 0;
    team02RightPushCount = 0;
    team01PushCount = 0;
    team02PushCount = 0;

    stopAnimating();

    delay(3000);
  }
  else {
  }

  // run all the LED functions here.
  red1.animate();
  red2.animate();
  red3.animate();
  red4.animate();
  blue1.animate();
  blue2.animate();
  blue3.animate();
  blue4.animate();

  // blue also
}

/*
  STRIP FUNCTIONS ARE BELOW HERE BOTH RED(PLAYER 1) BLUE(PLAYER 2):
  These are all the strip Functions they correspond to each led set of pixels
  on the game board. If you end up having the strip only show strip 3 when the count get there
  Then you need to go back and change the beginning of the strip value to o to start
  at the head of the whole entire Led Strip. I suspect that may happen, but build
  it this way to start.
*/


//RED TEAM 01 FUNCTIONS (START HERE)---------------
//
//void redteamstrip1 () { //pixel 0 to 35
//  if (redstrip1animating) {
//    //  This is the animation part and it has a reference in //variables start for the color animations
//    team02LEDstrip.setPixelColor(redhead1, redcolor);
//    if (redtail1 >= 0) {
//      team02LEDstrip.setPixelColor(redtail1, redcolortail);
//    }
//    team02LEDstrip.show();
//    //  delay(35);                        // Pause 20 milliseconds (~50 FPS)
//
//    if (++redhead1 >= 35) {        // Increment head index.  Off end of strip?
//      redhead1 = 0;                       //  Yes, reset head index to start
//    }
//    if (++redtail1 >= 35) {
//      redtail1 = 0;
//    }
//  } else { //dont do the animation and reset if not time
//    for (int i = 0; i <= 35; ++i) {
//      team02LEDstrip.setPixelColor(i, 0);
//    }
//    team02LEDstrip.show();
//    redhead1 = 0;
//    redtail1 = -10;
//  }
//}
//
//void redteamstrip2 () { //pixel 36 to 71
//  if (redstrip2animating) {
//    //  This is the animation part and it has a reference in //variables start for the color animations
//    team02LEDstrip.setPixelColor(redhead2, redcolor);
//    if (redtail2 >= 36) {
//      team02LEDstrip.setPixelColor(redtail2, redcolortail);
//    }
//    team02LEDstrip.show();
//    //  delay(35);                        // Pause 20 milliseconds (~50 FPS)
//
//    if (++redhead2 >= 71) {        // Increment head index.  Off end of strip?
//      redhead2 = 36;                       //  Yes, reset head index to start
//    }
//    if (++redtail2 >= 71) {
//      redtail2 = 36;
//    }
//  } else { //dont do the animation and reset if not time
//    for (int i = 36; i <= 71; ++i) {
//      team02LEDstrip.setPixelColor(i, 0);
//    }
//    team02LEDstrip.show();
//    redhead2 = 36;
//    redtail2 = 36 - 10;
//  }
//}
//
//void redteamstrip3 () { //pixel 72 to 106
//  if (redstrip3animating) {
//    //  This is the animation part and it has a reference in //variables start for the color animations
//    team02LEDstrip.setPixelColor(redhead3, redcolor);
//    if (redtail3 >= 72) {
//      team02LEDstrip.setPixelColor(redtail3, redcolortail);
//    }
//    team02LEDstrip.show();
//    //  delay(35);                        // Pause 20 milliseconds (~50 FPS)
//
//    if (++redhead3 >= 106) {        // Increment head index.  Off end of strip?
//      redhead3 = 72;                       //  Yes, reset head index to start
//    }
//    if (++redtail3 >= 106) {
//      redtail3 = 72;
//    }
//  } else { //dont do the animation and reset if not time
//    for (int i = 72; i <= 106; ++i) {
//      team02LEDstrip.setPixelColor(i, 0);
//    }
//    team02LEDstrip.show();
//    redhead3 = 72;
//    redtail3 = 72 - 10;
//  }
//}
//
//void redteamstrip4 () { //pixel 107 to 143
//  if (redstrip4animating) {
//    //  This is the animation part and it has a reference in //variables start for the color animations
//    team02LEDstrip.setPixelColor(redhead4, redcolor);
//    if (redtail4 >= 107) {
//      team02LEDstrip.setPixelColor(redtail4, redcolortail);
//    }
//    team02LEDstrip.show();
//    //  delay(35);                        // Pause 20 milliseconds (~50 FPS)
//
//    if (++redhead4 >= 143) {        // Increment head index.  Off end of strip?
//      redhead4 = 107;                       //  Yes, reset head index to start
//    }
//    if (++redtail4 >= 143) {
//      redtail4 = 107;
//    }
//  } else { //dont do the animation and reset if not time
//    for (int i = 107; i <= 143; ++i) {
//      team02LEDstrip.setPixelColor(i, 0);
//    }
//    team02LEDstrip.show();
//    redhead4 = 107;
//    redtail4 = 107 - 10;
//  }
//}

//RED TEAM 01 FUNCTIONS (END HERE)---------------

//BLUE TEAM 02 FUNCTIONS (START HERE)---------------
//
//void blueteamstrip1 () { //pixel 0 to 35
//  if (bluestrip1animating) {
//    //  This is the animation part and it has a reference in //variables start for the color animations
//    team01LEDstrip.setPixelColor(bluehead1, bluecolor);
//    if (redtail1 >= 0) {
//      team01LEDstrip.setPixelColor(bluetail1, bluecolortail);
//    }
//    team01LEDstrip.show();
//    //  delay(35);                        // Pause 20 milliseconds (~50 FPS)
//
//    if (++redhead1 >= 35) {        // Increment head index.  Off end of strip?
//      bluehead1 = 0;                       //  Yes, reset head index to start
//    }
//    if (++redtail1 >= 35) {
//      bluetail1 = 0;
//    }
//  } else { //dont do the animation and reset if not time
//    for (int i = 0; i <= 35; ++i) {
//      team01LEDstrip.setPixelColor(i, 0);
//    }
//    team01LEDstrip.show();
//    bluehead1 = 0;
//    bluetail1 = -10;
//  }
//}
//
//void blueteamstrip2 () { //pixel 36 to 71
//  if (bluestrip2animating) {
//    //  This is the animation part and it has a reference in //variables start for the color animations
//    team01LEDstrip.setPixelColor(bluehead2, bluecolor);
//    if (bluetail2 >= 36) {
//      team01LEDstrip.setPixelColor(bluetail2, bluecolortail);
//    }
//    team01LEDstrip.show();
//    //  delay(35);                        // Pause 20 milliseconds (~50 FPS)
//
//    if (++redhead2 >= 71) {        // Increment head index.  Off end of strip?
//      bluehead2 = 36;                       //  Yes, reset head index to start
//    }
//    if (++bluetail2 >= 71) {
//      bluetail2 = 36;
//    }
//  } else { //dont do the animation and reset if not time
//    for (int i = 36; i <= 71; ++i) {
//      team01LEDstrip.setPixelColor(i, 0);
//    }
//    team01LEDstrip.show();
//    bluehead2 = 36;
//    bluetail2 = 36 - 10;
//  }
//}
//
//void blueteamstrip3 () { //pixel 72 to 106
//  if (bluestrip3animating) {
//    //  This is the animation part and it has a reference in //variables start for the color animations
//    team01LEDstrip.setPixelColor(bluehead3, bluecolor);
//    if (redtail3 >= 72) {
//      team01LEDstrip.setPixelColor(bluetail3, bluecolortail);
//    }
//    team01LEDstrip.show();
//    //  delay(35);                        // Pause 20 milliseconds (~50 FPS)
//
//    if (++redhead3 >= 106) {        // Increment head index.  Off end of strip?
//      bluehead3 = 72;                       //  Yes, reset head index to start
//    }
//    if (++bluetail3 >= 106) {
//      bluetail3 = 72;
//    }
//  } else { //dont do the animation and reset if not time
//    for (int i = 72; i <= 106; ++i) {
//      team01LEDstrip.setPixelColor(i, 0);
//    }
//    team01LEDstrip.show();
//    bluehead3 = 72;
//    bluetail3 = 72 - 10;
//  }
//}
//
//void blueteamstrip4 () { //pixel 107 to 143
//  if (bluestrip4animating) {
//    //  This is the animation part and it has a reference in //variables start for the color animations
//    team01LEDstrip.setPixelColor(bluehead4, bluecolor);
//    if (bluetail4 >= 107) {
//      team01LEDstrip.setPixelColor(bluetail4, bluecolortail);
//    }
//    team01LEDstrip.show();
//    //  delay(35);                        // Pause 20 milliseconds (~50 FPS)
//
//    if (++bluehead4 >= 143) {        // Increment head index.  Off end of strip?
//      bluehead4 = 107;                       //  Yes, reset head index to start
//    }
//    if (++redtail4 >= 143) {
//      bluetail4 = 107;
//    }
//  } else { //dont do the animation and reset if not time
//    for (int i = 107; i <= 143; ++i) {
//      team01LEDstrip.setPixelColor(i, 0);
//    }
//    team01LEDstrip.show();
//    bluehead4 = 107;
//    bluetail4 = 107 - 10;
//  }
//}

//BLUE TEAM 02 FUNCTIONS (END HERE)---------------


/*
  STRIP FUNCTIONS ARE BELOW HERE BOTH RED(PLAYER 1) BLUE(PLAYER 2):
  These are all the strip Functions they correspond to each led set of pixels
  on the game board. If you end up having the strip only show strip 3 when the count get there
  Then you need to go back and change the beginning of the strip value to o to start
  at the head of the whole entire Led Strip. I suspect that may happen, but build
  it this way to start.
*/




//PUT ALL OF THIS ABOVE SET UP (END HERE)---------------------------


void WTF() {
  Serial.println("WTF");
}

/*DONT COPY THIS
  DEBUG:
  1) Error ('pinmode' was not declared in this scope):

*/
