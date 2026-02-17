#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#define arduinoLED 13   // Arduino LED on board

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);

int duty = 0;
uint16_t pwm_value = 0;

void setup() {
  Serial.begin(9600);
  pwm1.begin();
  pwm2.begin();
  pwm1.setPWMFreq(1000);
  pwm2.setPWMFreq(1000);
  Wire.setClock(400000);
}

void loop() {
  // Drive each PWM in a 'wave'
  for (uint16_t i=0; i<4096; i += 8) {
    for (uint8_t pwmnum=0; pwmnum < 16; pwmnum++) {
      if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        duty = input.toInt();
        Serial.print("Duty set to: ");
        Serial.println(duty);
        pwm_value = (duty*4096)/100;
        Serial.println(pwm_value);
        
      }
      /*Serial.print("channel: ");
      Serial.println(pwmnum);
      
      Serial.print("duty cycle: ");
      Serial.println(duty);*/

      //pwm1.setPWM(pwmnum, 0, (i + (4096/16)*pwmnum) % 4096 );
      pwm1.setPWM(pwmnum, 0, (pwm_value));
      pwm2.setPWM(pwmnum, 0, (pwm_value));
      
      }
      delay(50);
#ifdef ESP8266
    yield();  // take a breather, required for ESP8266
#endif
  }
}//end loop