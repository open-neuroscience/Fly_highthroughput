
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SerialCommand.h>
#define arduinoLED 13   // Arduino LED on board

//int channel = 0;
//int intensity = 0;


Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);




SerialCommand sCmd;     // The demo SerialCommand object





void setup() {
  Serial.begin(9600);
  //Serial.println("16 channel PWM test!");

  pwm1.begin();
  pwm2.begin();
  /*
  
   * In theory the internal oscillator (clock) is 25MHz but it really isn't
   * that precise. You can 'calibrate' this by tweaking this number until
   * you get the PWM update frequency you're expecting!
   * The int.osc. for the PCA9685 chip is a range between about 23-27MHz and
   * is used for calculating things like writeMicroseconds()
   * Analog servos run at ~50 Hz updates, It is importaint to use an
   * oscilloscope in setting the int.osc frequency for the I2C PCA9685 chip.
   * 1) Attach the oscilloscope to one of the PWM signal pins and ground on
   *    the I2C PCA9685 chip you are setting the value for.
   * 2) Adjust setOscillatorFrequency() until the PWM update frequency is the
   *    expected value (50Hz for most ESCs)
   * Setting the value here is specific to each individual I2C PCA9685 chip and
   * affects the calculations for the PWM update frequency. 
   * Failure to correctly set the int.osc value will cause unexpected PWM results
   */
  //pwm1.setOscillatorFrequency(27000000);
  pwm1.setPWMFreq(1000);  // This is the maximum PWM frequency
  //pwm2.setOscillatorFrequency(27000000);
  pwm2.setPWMFreq(1000);  // This is the maximum PWM frequency

  // if you want to really speed stuff up, you can go into 'fast 400khz I2C' mode
  // some i2c devices dont like this so much so if you're sharing the bus, watch
  // out for this!
  Wire.setClock(200000);


  // Setup callbacks for SerialCommand commands

    /*
  sCmd.addCommand("motors",motors); //sets channel and intensity of motors
  
  sCmd.addCommand("houselight",houselight); //sets channel and intensity of houselight

  sCmd.addCommand("optoRED",optoRED); //sets channel and intensity of RED opto LEDs
  
  sCmd.addCommand("optoGREEN",optoGREEN); //sets channel and intensity of GREEN opto LEDs
  
  sCmd.addCommand("optoBLUE",optoBLUE); //sets channel and intensity of Blue opto LEDs
  */
  
  // one function will control all systems:
  sCmd.addCommand("set_system",set_system);
  // the function call follows this logic:
  //   channel, intensity and subsystem
  //   subsystem: ir: control infrared leds
  //              motor: control motors
  //              hl: control house lights
  //              opto: control optogenetics lights
  //   channel: IR: any integer from 1 to 12
  //            motor: any integer from 1 to 12
  //            house light: any integer from 1 to 2
  //            opto: integers 1 and 2 set the RED channel
  //                  integers 3 and 4 set the GREEN channel
  //                  integers 5 and 6 set the BLUE channel
  //   intensity: any integer value from 0 to 100


  
  //sCmd.addCommand("irleds",irleds); //sets channel and intensity of IR LEDs
  

  sCmd.addCommand("ON",    LED_on);          // Turns LED on to test communication, turns the onboard led on.
  sCmd.addCommand("OFF",   LED_off);         // Turns LED off
  sCmd.addCommand("HELLO", sayHello);        // Echos the string argument back
  sCmd.addCommand("P",     processCommand);  // Converts two arguments to integers and echos them back
  sCmd.setDefaultHandler(unrecognized);      // Handler for command that isn't matched  (says "What?")

  //write a bit of info so that the DART knows which hardware system we have
  Serial.println("HT2 modular v0.1");
  int waitFlag=0;

  while(waitFlag != '1'){
    //read forever until the computer sends 1 

    waitFlag = Serial.read();


  }//end while

  Serial.println("Ready");

}

void loop() {

  sCmd.readSerial();     // We don't do much, just process serial commands
  /*
  // Drive each PWM in a 'wave'
  for (uint16_t i=0; i<4096; i += 8) {
    for (uint8_t pwmnum=0; pwmnum < 16; pwmnum++) {
      pwm.setPWM(pwmnum, 0, (i + (4096/16)*pwmnum) % 4096 );
    }
#ifdef ESP8266
    yield();  // take a breather, required for ESP8266
#endif
  }
  */
}// end void loop


void set_system() {
  int subsystem;
  int channel;
  int intensity;

  char *arg;

  Serial.println("Processing command");
  
  arg = sCmd.next();

  if (arg != NULL) {
    ////////////////////////////////////////////////////
    if (strcmp(arg,"ir")==0) {
        subsystem=0;
    }
    if (strcmp(arg,"motor")==0) {
        subsystem=1;
    }
    if (strcmp(arg,"hl")==0) {
        subsystem=2;
    }
    if (strcmp(arg,"opto")==0) {
        subsystem=3;
    }

  }
    arg = sCmd.next();
       if (arg != NULL) {
    /////////////////////////////////////////////////////////
    channel = atoi(arg);    // Converts a char string to an integer
       }//end if
    if (channel<1){
      Serial.print("no channel should be below 1... ");
      Serial.println("defaulting channel to 1");
      channel = 1;
      }//end if channel

    if (arg != NULL) {
    intensity = atol(arg);
    }//end if
  else {
    Serial.println("No arguments");
    channel = 1;   
    intensity = 0;
  
  }
    //Serial.print("channel was: ");
    //Serial.println(channel);

    
    

      //Serial.print("intensity argument was: ");
    //Serial.println(intensity);
    if (intensity>100){
      Serial.print("intensity is above 100%... ");
      Serial.println("defaulting intensity to 100%");
      intensity = 100;
    }//end if intensity
    if (intensity<0){
      Serial.print("intensity can't be below 0... ");
      Serial.println("defaulting intensity to 0");
      intensity = 0;

    }//end if intensity
    
    // now map intensity to the range o to 4096 (12 bit resolution of the PCA9685 chips)
    intensity = map(intensity, 0,100,0,4095);
  


  if (subsystem==0){    
    if (channel>12){
      Serial.print("no IR channel should be above 12 ");
      Serial.println("defaulting channel to 0");
      channel = 0;
    }//end if channel

  
    pwm1.setPWM(channel, 0, intensity);
    Serial.println("infrared leds set");
  }//end subsystem 0
  
  if (subsystem==1){
  
    pwm2.setPWM(channel, 0, intensity);
        if (channel>12){
      Serial.print("no motor channel should be above 12 ");
      Serial.println("defaulting channel to 0");
      channel = 0;
    }//end if channel

    Serial.println("motors set");
  }//end subsystem 1
  
  
  if (subsystem==2){
      if (channel>2){
      Serial.print("no houselight channel should be above 2 ");
      Serial.println("defaulting channel to 0");
      channel = 0;
    }//end if channel

    if (channel == 1){
  
    /* since we are dividing the 16 channels on the PCA chips for houselight, opto, IR and motors, 
       in this bit of code we adjust it so that it goes to the correct channel and chip */
    channel = 15;
    pwm1.setPWM(channel, 0, intensity);
  }// end if channel==0
  
  if (channel == 2){
    /* since we are dividing the 16 channels on the PCA chips for houselight, opto, IR and motors, 
       in this bit of code we adjust it so that it goes to the correct channel and chip*/
    channel = 15;
    pwm2.setPWM(channel, 0, intensity);
  }// end if channel==0
  Serial.println("houselight set");



  }//end houselight configuration

if (subsystem==3){
  if (channel == 1){
    /* since we are dividing the 16 channels on the PCA chips for houselight, opto, IR and motors, 
       in this bit of code we adjust it so that it goes to the correct channel and chip*/
    channel = 13;
    pwm1.setPWM(channel, 0, intensity);
    Serial.println("optoRED set");
  }// end if channel==0
  if (channel == 2){
    /* since we are dividing the 16 channels on the PCA chips for houselight, opto, IR and motors, 
       in this bit of code we adjust it so that it goes to the correct channel and chip*/
    channel = 13;
    pwm2.setPWM(channel, 0, intensity);
    Serial.println("optoRED set");
  }// end if channel==0
  

//end optoRED configuration

  if (channel == 3){
    /* since we are dividing the 16 channels on the PCA chips for houselight, opto, IR and motors, 
       in this bit of code we adjust it so that it goes to the correct channel and chip*/
    channel = 12;
    pwm1.setPWM(channel, 0, intensity);
    Serial.println("optoGREEN set");
  }// end if channel==0
  if (channel == 4){
    /* since we are dividing the 16 channels on the PCA chips for houselight, opto, IR and motors, 
       in this bit of code we adjust it so that it goes to the correct channel and chip*/
    channel = 12;
    pwm2.setPWM(channel, 0, intensity);
    Serial.println("optoGREEN set");
  }// end if channel==0
  

//end optoGREEN configuration


  if (channel == 5){
    /* since we are dividing the 16 channels on the PCA chips for houselight, opto, IR and motors, 
       in this bit of code we adjust it so that it goes to the correct channel and chip*/
    channel = 14;
    pwm1.setPWM(channel, 0, intensity);
    Serial.println("optoBLUE set");
  }// end if channel==0
  if (channel == 6){
    /* since we are dividing the 16 channels on the PCA chips for houselight, opto, IR and motors, 
       in this bit of code we adjust it so that it goes to the correct channel and chip*/
    channel = 14;
    pwm2.setPWM(channel, 0, intensity);
    Serial.println("optoBLUE set");
  }// end if channel==0


}//end opto configuration

}//end set_system


void LED_on() {
  Serial.println("LED on");
  digitalWrite(arduinoLED, HIGH);
}

void LED_off() {
  Serial.println("LED off");
  digitalWrite(arduinoLED, LOW);
}

void sayHello() {
  char *arg;
  arg = sCmd.next();    // Get the next argument from the SerialCommand object buffer
  if (arg != NULL) {    // As long as it existed, take it
    Serial.print("Hello ");
    Serial.println(arg);
  }
  else {
    Serial.println("Hello, whoever you are");
  }
}


void processCommand() {
  int aNumber;
  char *arg;

  Serial.println("We're in processCommand");
  arg = sCmd.next();
  if (arg != NULL) {
    aNumber = atoi(arg);    // Converts a char string to an integer
    Serial.print("First argument was: ");
    Serial.println(aNumber);
  }
  else {
    Serial.println("No arguments");
  }

  arg = sCmd.next();
  if (arg != NULL) {
    aNumber = atol(arg);
    Serial.print("Second argument was: ");
    Serial.println(aNumber);
  }
  else {
    Serial.println("No arguments");
  }
}

// This gets set as the default handler, and gets called when no other command matches.
void unrecognized(const char *command) {
  Serial.println("What?");
}



/*
void irleds() {
  //int channel;
  ////int intensity;
  char *arg;

  Serial.println("Processing IR LED command");
  Serial.println("getting channel");
  arg = sCmd.next();
  if (arg != NULL) {

    channel = atoi(arg);    // Converts a char string to an integer
    //Serial.print("channel was: ");
    //Serial.println(channel);
    if (channel>12){
      Serial.print("channel is not IRLED... ");
      Serial.println("defaulting to IR channel 0");
      channel = 0;
    }//end if channel
    if (channel<0){
      Serial.print("channel is invalid... ");
      Serial.println("defaulting to IR channel 0");
      channel = 0;
    }//end if channel
    
    

  }
  else {
        Serial.println("No arguments");
    channel = 0;
    intensity = 0;


    //
  }


  arg = sCmd.next();

  if (arg != NULL) {
    intensity = atol(arg);
    //Serial.print("intensity argument was: ");
    //Serial.println(intensity);
    if (intensity>100){
      Serial.print("intensity is above 100%... ");
      Serial.println("defaulting IR intensity to 100%");
      intensity = 100;
    }//end if intensity
    if (intensity<0){
      Serial.print("intensity is invalid... ");
      Serial.println("defaulting IR intensity to 0");
      intensity = 0;

    }//end if intensity
    
    // now map intensity to the range o to 4096 (12 bit resolution of the PCA9685 chips)
    intensity = map(intensity, 0,100,0,4095);
  }
  else {
    Serial.println("No arguments");
    channel = 0;
    intensity = 0;
  
  }


  
  pwm1.setPWM(channel, 0, intensity);
  Serial.println("infrared leds set");
}//end IR LED
*/



/*
void motors() {
  //int channel;
  //int intensity;
  char *arg;

  Serial.println("Processing Motor command");
  Serial.println("getting motor channel");
  arg = sCmd.next();
  if (arg != NULL) {

    channel = atoi(arg);    // Converts a char string to an integer
    //Serial.print("channel was: ");
    //Serial.println(channel);
    if (channel>12){
      Serial.print("channel is not Motor... ");
      Serial.println("defaulting to motor channel 0");
      channel = 0;
    }//end if channel
    if (channel<0){
      Serial.print("channel is invalid... ");
      Serial.println("defaulting to motor channel 0");
      channel = 0;
    }//end if channel
    
    

  }
  else {
    Serial.println("No arguments");
    channel = 0;
    intensity = 0;
    
  }

  Serial.println("getting motor intensity");

  arg = sCmd.next();

  if (arg != NULL) {
    intensity = atol(arg);
    //Serial.print("intensity argument was: ");
    //Serial.println(intensity);
    if (intensity>100){
      Serial.print("intensity is above 100%... ");
      Serial.println("defaulting Motor intensity to 100%");
      intensity = 100;
    }//end if intensity
    if (intensity<0){
      Serial.print("intensity is invalid... ");
      Serial.println("defaulting Motor intensity to 0");
      intensity = 0;

    }//end if intensity
    
    // now map intensity to the range o to 4096 (12 bit resolution of the PCA9685 chips)
    intensity = map(intensity, 0,100,0,4095);
  }
  else {
    Serial.println("No arguments");
    channel = 0;
    intensity = 0;
    
  }


  
  pwm2.setPWM(channel, 0, intensity);
  Serial.println("infrared leds set");

}//end motor LED


void houselight() {
  //int channel;
  //int intensity;
  char *arg;

  Serial.println("Processing houselight command");
  Serial.println("getting houselight channel");
  arg = sCmd.next();
  if (arg != NULL) {

    channel = atoi(arg);    // Converts a char string to an integer
    //Serial.print("channel was: ");
    //Serial.println(channel);
    if (channel>1){
      Serial.print("channel is not houselight... ");
      Serial.println("defaulting to houselight channel 0");
      channel = 0;
    }//end if channel
    if (channel<0){
      Serial.print("channel is invalid... ");
      Serial.println("defaulting to houselight channel 0");
      channel = 0;
    }//end if channel
    
    

  }
  else {
      Serial.println("No arguments");
    channel = 0;
    intensity = 0;
    
  }


  arg = sCmd.next();

  if (arg != NULL) {
    intensity = atol(arg);
    //Serial.print("intensity argument was: ");
    //Serial.println(intensity);
    if (intensity>100){
      Serial.print("intensity is above 100%... ");
      Serial.println("defaulting houselight intensity to 100%");
      intensity = 100;
    }//end if intensity
    if (intensity<0){
      Serial.print("intensity is invalid... ");
      Serial.println("defaulting houselight to 0");
      intensity = 0;

    }//end if intensity
    
    // now map intensity to the range o to 4096 (12 bit resolution of the PCA9685 chips)
    intensity = map(intensity, 0,100,0,4095);
  }
  else {
    Serial.println("No arguments");
    channel = 0;
    intensity = 0;
    
  }

  
  
  if (channel == 0){
  
    /* since we are dividing the 16 channels on the PCA chips for houselight, opto, IR and motors, 
       in this bit of code we adjust it so that it goes to the correct channel and chip 
    channel = 15;
    pwm1.setPWM(channel, 0, intensity);
  }// end if channel==0
  
  if (channel == 1){
    /* since we are dividing the 16 channels on the PCA chips for houselight, opto, IR and motors, 
       in this bit of code we adjust it so that it goes to the correct channel and chip
    channel = 15;
    pwm2.setPWM(channel, 0, intensity);
  }// end if channel==0
  Serial.println("houselight set");

}//end houselight

void optoRED() {
  //int channel;
  //int intensity;
  char *arg;

  Serial.println("Processing optoRED command");
  Serial.println("getting optoRED channel");
  arg = sCmd.next();
  if (arg != NULL) {

    channel = atoi(arg);    // Converts a char string to an integer
    //Serial.print("channel was: ");
    //Serial.println(channel);
    if (channel>1){
      Serial.print("channel is not optoRED... ");
      Serial.println("defaulting to optoRED channel 0");
      channel = 0;
    }//end if channel
    if (channel<0){
      Serial.print("channel is invalid... ");
      Serial.println("defaulting to optoRED channel 0");
      channel = 0;
    }//end if channel
    
    

  }
  else {
    Serial.println("No arguments");
    channel = 0;
    intensity = 0;
  }


  arg = sCmd.next();

  if (arg != NULL) {
    intensity = atol(arg);
    //Serial.print("intensity argument was: ");
    //Serial.println(intensity);
    if (intensity>100){
      Serial.print("intensity is above 100%... ");
      Serial.println("defaulting optoRED intensity to 100%");
      intensity = 100;
    }//end if intensity
    if (intensity<0){
      Serial.print("intensity is invalid... ");
      Serial.println("defaulting optoRED to 0");
      intensity = 0;

    }//end if intensity
    
    // now map intensity to the range o to 4096 (12 bit resolution of the PCA9685 chips)
    intensity = map(intensity, 0,100,0,4095);
  }
  else {
    Serial.println("No arguments");
    channel = 0;
    intensity = 0;
    
  }


  if (channel == 0){
    /* since we are dividing the 16 channels on the PCA chips for houselight, opto, IR and motors, 
       in this bit of code we adjust it so that it goes to the correct channel and chip
    channel = 13;
    pwm1.setPWM(channel, 0, intensity);
  }// end if channel==0
  if (channel == 1){
    /* since we are dividing the 16 channels on the PCA chips for houselight, opto, IR and motors, 
       in this bit of code we adjust it so that it goes to the correct channel and chip
    channel = 13;
    pwm2.setPWM(channel, 0, intensity);
  }// end if channel==0
  Serial.println("optoRED set");

}//end optoRED

void optoGREEN() {
  //int channel;
  //int intensity;
  char *arg;

  Serial.println("Processing optoGREEN command");
  Serial.println("getting optoGREEN channel");
  arg = sCmd.next();
  if (arg != NULL) {

    channel = atoi(arg);    // Converts a char string to an integer
    //Serial.print("channel was: ");
    //Serial.println(channel);
    if (channel>1){
      Serial.print("channel is not optoGREEN... ");
      Serial.println("defaulting to optoGREEN channel 0");
      channel = 0;
    }//end if channel
    if (channel<0){
      Serial.print("channel is invalid... ");
      Serial.println("defaulting to optoGREEN channel 0");
      channel = 0;
    }//end if channel
    
    

  }
  else {
    Serial.println("No arguments");
    channel = 0;
    intensity = 0;
  }


  arg = sCmd.next();

  if (arg != NULL) {
    intensity = atol(arg);
    //Serial.print("intensity argument was: ");
    //Serial.println(intensity);
    if (intensity>100){
      Serial.print("intensity is above 100%... ");
      Serial.println("defaulting optoGREEN intensity to 100%");
      intensity = 100;
    }//end if intensity
    if (intensity<0){
      Serial.print("intensity is invalid... ");
      Serial.println("defaulting optoGREEN to 0");
      intensity = 0;

    }//end if intensity
    
    // now map intensity to the range o to 4096 (12 bit resolution of the PCA9685 chips)
    intensity = map(intensity, 0,100,0,4095);
  }
  else {
    Serial.println("No arguments");
    channel = 0;
    intensity = 0;
    
  }


  if (channel == 0){
    /* since we are dividing the 16 channels on the PCA chips for houselight, opto, IR and motors, 
       in this bit of code we adjust it so that it goes to the correct channel and chip
    channel = 12;
    pwm1.setPWM(channel, 0, intensity);
  }// end if channel==0
  if (channel == 1){
    /* since we are dividing the 16 channels on the PCA chips for houselight, opto, IR and motors, 
       in this bit of code we adjust it so that it goes to the correct channel and chip
    channel = 12;
    pwm2.setPWM(channel, 0, intensity);
  }// end if channel==0
  Serial.println("optoGREEN set");

}//end optoGREEN

void optoBLUE() {
  //int channel;
  //int intensity;
  char *arg;

  Serial.println("Processing optoBLUE command");
  Serial.println("getting optoBLUE channel");
  arg = sCmd.next();
  if (arg != NULL) {

    channel = atoi(arg);    // Converts a char string to an integer
    //Serial.print("channel was: ");
    //Serial.println(channel);
    if (channel>1){
      Serial.print("channel is not optoBLUE... ");
      Serial.println("defaulting to optoBLUE channel 0");
      channel = 0;
    }//end if channel
    if (channel<0){
      Serial.print("channel is invalid... ");
      Serial.println("defaulting to optoBLUE channel 0");
      channel = 0;
    }//end if channel
    
    

  }
  else {
    Serial.println("No arguments");
    channel = 0;
    intensity = 0;
  }


  arg = sCmd.next();

  if (arg != NULL) {
    intensity = atol(arg);
    //Serial.print("intensity argument was: ");
    //Serial.println(intensity);
    if (intensity>100){
      Serial.print("intensity is above 100%... ");
      Serial.println("defaulting optoBLUE intensity to 100%");
      intensity = 100;
    }//end if intensity
    if (intensity<0){
      Serial.print("intensity is invalid... ");
      Serial.println("defaulting optoBLUE to 0");
      intensity = 0;

    }//end if intensity
    
    // now map intensity to the range o to 4096 (12 bit resolution of the PCA9685 chips)
    intensity = map(intensity, 0,100,0,4095);
  }
  else {
    Serial.println("No arguments");
    channel = 0;
    intensity = 0;
  }
  

  if (channel == 0){
    /* since we are dividing the 16 channels on the PCA chips for houselight, opto, IR and motors, 
       in this bit of code we adjust it so that it goes to the correct channel and chip
    channel = 14;
    pwm1.setPWM(channel, 0, intensity);
  }// end if channel==0
  if (channel == 1){
    /* since we are dividing the 16 channels on the PCA chips for houselight, opto, IR and motors, 
       in this bit of code we adjust it so that it goes to the correct channel and chip
    channel = 14;
    pwm2.setPWM(channel, 0, intensity);
  }// end if channel==0
  Serial.println("optoBLUE set");

}//end optoBLUE
*/
