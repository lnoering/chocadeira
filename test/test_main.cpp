#include <Arduino.h>
#include <AUnit.h>

test(exampleTest) {
  assertEqual(3, 3);
}

void setup() {
#if ARDUINO
  delay(1000); // wait for stability on some boards to prevent garbage Serial
#endif
  Serial.begin(115200); // ESP8266 default of 74880 not supported on Linux
  while(!Serial); // for the Arduino Leonardo/Micro only
}

void loop() {
  aunit::TestRunner::run();
}