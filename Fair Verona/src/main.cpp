//where we lay our scene

// #include "romiFight.h"
#include "romiBalcony.h"
// #include "romiFinal.h"
// #include "tybaltFight.h"
// #include "mercutioFight.h"
#include "julietBalcony.h"
// #include "julietFinal.h"

// romiFight romifight;
romiBalcony romibalcony;
// romiFinal romifinal;
// tybaltFight tybaltfight;
// mercutioFight mercutiofight;
julietBalcony julietbalcony;
// julietFinal julietfinal;

typedef enum {
  ROMI,
  MERCUTIO,
  TYBALT,
  JULIET
} Robot;
Robot robot = ROMI; //set which robot you want it to be

typedef enum {
  FIGHT,
  BALCONY,
  FINALSCENE
}  Scene;
Scene scene = BALCONY; //set which scene it is

void setup(){
  Serial.begin(115200);
  switch(robot){
    case JULIET:
      julietbalcony.setup();
    break;
    case ROMI:
      romibalcony.setup();
    break;
 }
}

void loop(){
  switch(robot){
  case JULIET:
    julietbalcony.loop();
  break;
  case ROMI:
    romibalcony.loop();
    break;
  }
}

//based on selected robot and scene the correct setup and loop will be run
/*
void setup() {
  //prints out which scene is running at start
  Serial.begin(115200);

  switch(robot){
    case ROMI:
      Serial.print(F("ROMI"));
    break;
    case TYBALT:
      Serial.print(F("TYBALT"));
    break;
    case MERCUTIO:
      Serial.print(F("MERCOOLIO"));
    break;
    case JULIET:
      Serial.print(F("JULIET"));
    break;
  }
  Serial.print(" ");
  switch(scene){
    case FIGHT:
      Serial.println(F("FIGHT"));
    break;
    case BALCONY:
      Serial.println(F("BALCONY"));
    break;
    case FINALSCENE:
      Serial.println(F("FINAL"));
    break;
  }


  switch(robot){
    case ROMI:
      switch(scene){
        case FIGHT:
          romifight.setup();
        break;
        case BALCONY:
          romibalcony.setup();
        break;
        case FINALSCENE:
          romifinal.setup();
        break;
      }
      break;
    case TYBALT:
      tybaltfight.setup();
    break;
    case MERCUTIO:
      mercutiofight.setup();
    break;
    case JULIET:
      switch(scene){
        case BALCONY:
          julietbalcony.setup();
        break;
        case FINALSCENE:
          julietfinal.setup();
        break;
        default:
          Serial.println(F("incorrect selection"));
        break;
      }
    break;
  }
}

void loop() {
  switch(robot){
    case ROMI:
      switch(scene){
        case FIGHT:
          romifight.loop();
        break;
        case BALCONY:
          romibalcony.loop();
        break;
        case FINALSCENE:
          romifinal.loop();
        break;
      }
      break;
    case TYBALT:
      tybaltfight.loop();
    break;
    case MERCUTIO:
      mercutiofight.loop();
    break;
    case JULIET:
      switch(scene){
        case BALCONY:
          julietbalcony.loop();
        break;
        case FINALSCENE:
          julietfinal.loop();
        break;
      }
    break;
  }
}*/

// Two households, both alike in dignity,
// In fair Verona, where we lay our scene,
// From ancient grudge break to new mutiny,
// Where civil blood makes civil hands unclean.
// From forth the fatal loins of these two foes
// A pair of star-cross'd lovers take their life;
// Whose misadventured piteous overthrows
// Do with their death bury their parents' strife.
// The fearful passage of their death-mark'd love,
// And the continuance of their parents' rage,
// Which, but their children's end, nought could remove,
// Is now the two hours' traffic of our stage;
// The which if you with patient ears attend,
// What here shall miss, our toil shall strive to mend.