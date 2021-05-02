// #include <Arduino.h>
// #include <Romi32U4.h>

// #include <apriltags.h>

// //Function prototypes
// float getDistanceCam(uint16_t tagW);

// void setup()
// {
//   Serial.begin(115200);
//   delay(1000);

//   Wire.begin();
//   Wire.setClock(100000ul); // Wire should default to 100kHz, but let's be explicit

//   Serial.println(F("Welcome."));
// }

// uint8_t FindAprilTags()
// {
//     uint8_t tagCount = getTagCount();
//     if(tagCount) 
//     {
//       Serial.println(tagCount);
//       AprilTagDatum tag;
//       if(readTag(&tag))
//       {
//         Serial.print(F("Tag [cx="));
//         Serial.print(tag.cx);
//         Serial.print(F(", cy="));
//         Serial.print(tag.cy);
//         Serial.print(F(", w="));
//         Serial.print(tag.w);
//         Serial.print(F(", h="));
//         Serial.print(tag.h);
//         Serial.print(F(", id="));
//         Serial.print(tag.id);
//         Serial.print(F(", rot="));
//         Serial.print(tag.rot);
//         Serial.println(F(", Dist="));
//         Serial.print(getDistanceCam(tag.w));
//         Serial.println(F("]"));
//       }
//     }

//     return tagCount;
// }

// void loop() 
// { 
//   delay(1); //calm things down for a millisec...not needed in a program that does other things
//   FindAprilTags();
// }

// float getDistanceCam(uint16_t tagW) {
//   // float distance = tagW * (calDistance / calWidth);

// // float angle = 1.25 * tagW/45

//   // float distance = tagW * atan2(angle);

//   float distance = (actualWidth / 2) / (tan(tagW * (atan2((actualWidth / 2), calDistance) / calWidth)));

//   return distance;
// }