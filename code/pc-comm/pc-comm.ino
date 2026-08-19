#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SerialCommand.h>
#define arduinoLED 13   // Arduino LED on board

//#include <stdexcept>

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

// variables
int intensity;
bool verbose = true;
SerialCommand sCmd;     // The demo SerialCommand object

// parameters
int max_freq = 2000;    // maximum frequency
int pwm_count = 16;     // pwm count
int max_scale = 3000;   // max intensity scale factor
int baud_rate = 9600;   // serial baud rate

// set all stimuli channel enumeration flags
int all_motor_ch = 100; // use all motor channels flag
int all_red_ch = 200;      // use all red channels flag
int all_green_ch = 300;    // use all green channels flag
int all_blue_ch = 400;  // use all blue channels flag
int all_hl_ch = 500;    // use all house light channels flag
int all_ir_ch = 600;    // use all ir light channels flag

// fixed quantities 
int n_ir = 12;           // max ir channel count
int n_motor = 8;        // max motor channel count
int n_hl = 2;           // max house light channel count
int n_opto = 6;         // max opto channel count

// channel indices
int hl_ch1 = 0;
int hl_ch2 = 1;
int red_ch1 = 0;
int red_ch2 = 1;
int green_ch1 = 2;
int green_ch2 = 3;
int blue_ch1 = 4;
int blue_ch2 = 5;
int channel_red = 13;
int channel_green = 12;
int channel_blue = 14;
int channel_hl = 15;

// ------------------------------------------------------------------------

void setup() {
  Serial.begin(baud_rate);
  pwm1.begin();
  pwm2.begin();

  /*
  
   * In theory the internal oscillator (clock) is 25MHz but it really isn't
   * that precise. You can 'calibrate' this by tweaking this number until
   * you get the PWM update frequency you're expecting!
   * The int.osc. for the PCA9685 chip is a range between about 23-27MHz and
   * is used for calculating things like writeMicroseconds()
   *     servos run at ~50 Hz updates, It is importaint to use an
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
  //pwm2.setOscillatorFrequency(27000000);
  pwm1.setPWMFreq(max_freq);  // This is the maximum PWM frequency
  pwm2.setPWMFreq(2000);  // This is the maximum PWM frequency

  // turn everything off
  for (uint8_t pwmnum=0; pwmnum < pwm_count; pwmnum++) {
      pwm1.setPWM(pwmnum, 0, 4096 );
      pwm2.setPWM(pwmnum, 0, 4096 );
  }

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
  sCmd.addCommand("set",set_system);
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

  // debugging callback functions
  sCmd.addCommand("ON",    LED_on);          // Turns LED on to test communication, turns the onboard led on.
  sCmd.addCommand("OFF",   LED_off);         // Turns LED off
  sCmd.addCommand("HELLO", sayHello);        // Echos the string argument back
  sCmd.addCommand("P",     processCommand);  // Converts two arguments to integers and echos them back
  sCmd.setDefaultHandler(unrecognized);      // Handler for command that isn't matched  (says "What?") 

  //write a bit of info so that the DART knows which hardware system we have
  Serial.println("HT2 modular v0.1");
  int waitFlag=0;

  // wait until the start flag has been set
  while(waitFlag != '1') { 
    waitFlag = Serial.read();
  }

  if (verbose) {
    Serial.println("Ready");
  }

}

void loop() {

  // serial controller device inner loop function
  sCmd.readSerial();     // We don't do much, just process serial commands

}

void set_system() {

  // ----------------------------------------------------------------------
  // SERIAL COMMAND ARGUMENT RETRIEVAL
  // ----------------------------------------------------------------------
  

    // variables
    char *arg;
    int channel;
    int subsystem;

    if (verbose) {
      Serial.println("Processing command");
    }

    // stimuli argument type
    arg = sCmd.next();
    if (arg != NULL) {
      if (strcmp(arg,"ir")==0) {
          // ir lights
          subsystem=0;
      } else if (strcmp(arg,"motor")==0) {
          // motors
          subsystem=1;
      } else if (strcmp(arg,"hl")==0) {
          // house (white) lights
          subsystem=2;
      } else if (strcmp(arg,"opto")==0) {
          // optogenetic lights
          subsystem=3;
      }
    }

    // channel information
    arg = sCmd.next();
    if (arg != NULL) {
      channel = atoi(arg);
      if (channel<0) {
        // invalid/missing channel index
        channel = 0;
        if (verbose) {
          Serial.print("no channel should be below 0... ");
          Serial.println("defaulting channel to 0");
        }     
      }
    }
    
    // stimuli intensity
    arg = sCmd.next();
    if (arg != NULL) {    
      // valid stimuli intensity
      intensity = atoi(arg);
      if (verbose) {
        Serial.print("raw intensity: ");
        Serial.println(intensity);
      }

      if (intensity>100) {
        // intensity exceeds upper limit
        intensity = 100;
        if (verbose) {
          Serial.print("intensity is above 100%... ");
          Serial.println("defaulting intensity to 100%");
        }
      } else if (intensity<0) {
        // intensity below lower limit
        intensity = 0;
        if (verbose) {
          Serial.print("intensity can't be below 0... ");
          Serial.println("defaulting intensity to 0");
        }
      }

    } else {
      // invalid/missing stimuli intensity
      channel = 0;   
      intensity = 0;
      if (verbose) {
        Serial.println("No arguments");
      }
    }
    
    // now map intensity to the range o to 4096 (12 bit resolution of the 
    // PCA9685 chips) -  right now capped at 3000 `cause LED's were burning 
    // at 4096 - need to check electronics  
    intensity = map(intensity,0,100,0,max_scale);    
    if (intensity == 0) {
      // this is done to fully turn off the system (for zero intensity)
      intensity=4096;
    }
    
    // writes final intensity (verbose only) 
    if (verbose) {
      Serial.println(intensity);
    }

    // ----------------------------------------------------------------------
    // STIMULI INTENSITY UPDATES 
    // ----------------------------------------------------------------------

    /* since we are dividing the 16 channels on the PCA chips for 
       houselight, opto, IR and motors, in this bit of code we adjust it 
       so that it goes to the correct channel and chip */

    // IR-LIGHT CONFIGURATION UPDATE CHECK  
    if (subsystem==0) {    
      if (channel==all_ir_ch) {
        // all ir channel update (NB: channel count = 2 for HTController)
        for (channel=0; channel<8; channel++) {
          pwm2.setPWM(channel, 0, intensity);      
        }

        // SPECIAL NOTE - controls the side IR panels
        pwm2.setPWM(11, 0, intensity);
      } else {
        // individual channel update
        if (channel>n_ir) {
          // invalid channel index
          channel = 0;
          if (verbose) {
            Serial.print("no IR channel should be above 7 ");
            Serial.println("defaulting channel to 0");
          }
        }

        // updates the channel intensity
        pwm2.setPWM(channel, 0, intensity);
      }  

      if (verbose) {
        Serial.println("infrared leds set");
      }
    }
  
    // MOTOR CONFIGURATION UPDATE CHECK  
    if (subsystem==1) {
      if (channel==all_motor_ch) {
        // all motor channel update
        for (channel=0; channel<n_motor; channel++) {
          pwm1.setPWM(channel, 0, intensity);
        }

      } else {
        // individual channel update
          if (channel>n_motor) {
          // invalid channel index
          channel = 0;
          if (verbose) {
              Serial.print("no motor channel should be above 7 ");
              Serial.println("defaulting channel to 0");
          }
        }

        // updates the channel intensity
        pwm1.setPWM(channel, 0, intensity);
      }

      if (verbose) {
        Serial.println("motors set");
      }
    }

    // HOUSE-LIGHT CONFIGURATION UPDATE CHECK  
    if (subsystem==2) {
      if (channel==all_hl_ch) {
        // all house-light channel update
        channel = channel_hl;
        pwm1.setPWM(channel, 0, intensity);   
        pwm2.setPWM(channel, 0, intensity);

      } else {
        // individual channel update
        if (channel>1) {
          channel = 0;
          if (verbose) {
            Serial.print("no houselight channel should be above 1 ");
            Serial.println("defaulting channel to 0");
          }
        }

        // updates the house-light channel intensity
        if (channel == hl_ch1) {
          channel = channel_hl;
          pwm1.setPWM(channel, 0, intensity);    
        } else if (channel == hl_ch2) {
          channel = channel_hl;
          pwm2.setPWM(channel, 0, intensity);
        }
      }

      if (verbose) {
        Serial.println("houselight set");
      }
    }

    // OPTO CONFIGURATION UPDATE CHECK
    if (subsystem==3) {
      // set pwm flag
      bool set_pwm1 = false;
      bool set_pwm2 = false;

      // red opto channel check
      set_pwm1 = (channel==all_red_ch) || (channel==red_ch1);
      set_pwm2 = (channel==all_red_ch) || (channel==red_ch2);
      if (set_pwm1 || set_pwm2) {
        // if there is a match, reset channel index to red
        channel = channel_red;
        if (verbose) {
          Serial.println("optoRED set");
        }
      } else {
        // otherwise, check the green chanels
        set_pwm1 = (channel==all_green_ch) || (channel==green_ch1);
        set_pwm2 = (channel==all_green_ch) || (channel==green_ch2);
        if (set_pwm1 || set_pwm2) {
          // if there is a match, reset channel index to green
          channel = channel_green;
          if (verbose) {
            Serial.println("optoGreen set");
          }
        } else {
          // otherwise, check the blue channels
          set_pwm1 = (channel==all_blue_ch) || (channel==blue_ch1);
          set_pwm2 = (channel==all_blue_ch) || (channel==blue_ch2);
          if (set_pwm1 || set_pwm2) {
            channel = channel_blue;
            if (verbose) {
              Serial.println("optoBLUE set");
            }
          }
        }
      }

      // pwm1 update (if required)
      if (set_pwm1) {
        pwm1.setPWM(channel, 0, intensity);
      }  

      // pwm2 update (if required)
      if (set_pwm2) {
        pwm2.setPWM(channel, 0, intensity);
      }  
    }
}

// ------------------------------------------------------------------------

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

// ------------------------------------------------------------------------

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
