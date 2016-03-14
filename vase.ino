/* File:    vase.ino
 * By:      Jean René Clemenceau
 * Created: 03/06/2016
 * Controls a set of Adafruit Neopixels: One ring in the vase
 * and another single Neopixel to simulate a beating heart.
 * 
 * Some ring animations are incorporated from Adafruit's buttoncycler example.
 * 
 * Operation:
 * Short press the button to cycle though vase animations and colors.
 * Long press the button to cycle through heartbeat animations.
*/
#include <Adafruit_NeoPixel.h>

//Settings constants
const int ANALOG_MAX = 255;               //Highest possible PWM analog output value (max brightness)
const byte BPM_MAX = 61;                  //Heart's maxmimum beats per minute before strobing effect
const byte BPM = 21;                      //Heart's default beats per minute
const int BUTTON_PRESS_THRESHOLD = 500;  //Button press time that determines which lights settings to change

//Set pins
const byte BUTTON_PIN =  3;   //Digital IO pin connected to the button.
const byte HEART_PIN = 5;     //Digital IO pin number that represents heart
const byte RING_PIN =   6;    //Digital IO pin connected to the vase's NeoPixel ring.

//Initialize NeoPixels
const byte PIXEL_COUNT = 16;  //Number of NeoPixels in vase's ring
Adafruit_NeoPixel VASE_RING = Adafruit_NeoPixel(PIXEL_COUNT, RING_PIN, NEO_GRB + NEO_KHZ800 );  //Set vase
Adafruit_NeoPixel HEART = Adafruit_NeoPixel(1, HEART_PIN, NEO_GRB + NEO_KHZ800 );               //Set heart

//Mode tracker for heart
byte heart_rate;               //Determines variable heart rate
uint32_t heart_color;         //Determines color of heart NeoPixel

//Mode trackers for button input
bool old_state;               //Tracks the previous state of the input button
byte heart_mode;              //Marks the upcoming heartbeat mode for frequency and color
byte vase_mode;               //Marks the vase mode for color and animations


//Time trackers for non-blocking delays
unsigned long button_timer;    //Tracks time of previous button event

unsigned long beat_timer;     //Tracks time of previous beat for heartbeat delay
int beat_delay;               //Determines non-blocking wait time
byte heart_iteration;         //Tracks which stage the heartbeat is on
int dimmer;                   //Tracks the brightness value at dimming stage

unsigned long vase_timer;     //Tracks start time of previous vase light animation
int vase_delay;               //Determines non-blocking wait time
byte vase_anim_iteration;     //Tracks which stage the vase light animation is on

  
/***************************************************/

void setup() {
  //Initialize Pins
  pinMode(BUTTON_PIN, INPUT_PULLUP); //(HIGH = unpressed, LOW = pressed)
  pinMode(HEART_PIN, OUTPUT);
  pinMode(RING_PIN, OUTPUT);

  // Initialize all Neopixels to 'off'
  VASE_RING.begin();
  VASE_RING.show(); 
  HEART.begin();
  HEART.show();

  //Initialize heart light status
  heart_rate = BPM;
  heart_color = HEART.Color(255,0,0);

  //Initialize mode variables
  old_state = HIGH;
  heart_mode = 1;
  vase_mode = 0;

  //Initialize delay variables
  button_timer = 0;
  
  beat_timer = 0;
  beat_delay = 0;
  heart_iteration = 0;
  dimmer = ANALOG_MAX;
  
  vase_timer = 0;
  vase_delay = 0;
  vase_anim_iteration = 0;
}

void loop() {
  //check button state and time.
  bool new_state = digitalRead(BUTTON_PIN);
  unsigned long current_press_time = millis();
  bool buttonLifted = old_state == LOW && new_state == HIGH;
  bool shortPress = current_press_time - button_timer < BUTTON_PRESS_THRESHOLD;

  //If button lifted before threshold, change vase state (short press)
  if( shortPress && buttonLifted){
    vase_mode++;
    vase_anim_iteration = 0;
  }
  //If button lifted after more than threshold, change heart state (long press)
  else if( !shortPress && buttonLifted ){
    heart_mode = change_heart_state(heart_mode, heart_rate, heart_color, HEART);
  }
  //If button state changes, record change
  if( old_state != new_state ){
    button_timer = current_press_time;
    old_state = new_state;
  }

  //Execute heartbeat
  if(wait(beat_delay,beat_timer) ){
    beat_delay = heartbeat(heart_rate, HEART, heart_color, heart_iteration,dimmer);
  }

  //Animate Vase
  animate_vase(vase_mode, VASE_RING, vase_timer, vase_anim_iteration );
}


/**********Handling functions************/

/*heartbeat: animates an Adafruit NeoPixel to look like a heartbeat without blocking delays
 * Input
 *  -bpm:         (byte) Desired beats per minute.
 *  -pixel:       (Adafruit_NeoPixel) NeoPixel object containing the LED representing a heart.
 *  -neopix_color:(uint32_t) NeoPixel Color for the heartbeat.
 *  -iteration:   (byte) iterative stage of heartbeat.
 *  -dimmer:      (int) Brightness of NeoPixel during relaxation stage.
 * Output
 * The delay needed until the next heartbeat stage. (int)
*/
int heartbeat(byte bpm, Adafruit_NeoPixel &pixel, uint32_t neopix_color, byte &iteration, int &dimmer){
  int returnDelay = 0;                                        //Wait time until next stage

  //Turn off if BPM is less than 1
  if(bpm < 1){
    pixel.setPixelColor(0, 0);
    pixel.show();
    returnDelay = 0;
    iteration = 0;
  }
  //Turn on solid if BPM is greater than Max
  else if(bpm >= BPM_MAX){
    pixel.setPixelColor(0, neopix_color);
    pixel.show();
    returnDelay = 0;
    iteration = 0;
  }
  //Proceed to heartbeat stages if BPM is within range
  else{
    int interval = (60/bpm)*1000;                             //Total time of beat period
    int beat = interval/15;                                   //length of contraction
    int lag = interval/20;                                    //Lenth of time between beats
    int hb_attenuation = (interval/3) / ANALOG_MAX;           //Fade after ventricular delay offset
    int leftover_time = interval - (beat+lag+hb_attenuation); //Remainder of beat duration
    
    switch(iteration){
      case 0:
        //Start Atrial contraction
        pixel.setPixelColor(0, neopix_color);
        pixel.show();
        returnDelay = beat;
        iteration++;
        break;
      case 1:
        //Stop Atrial contraction
        pixel.setPixelColor(0,0);
        pixel.show();
        returnDelay = lag;
        iteration++;
        break;
      case 2:
        //Start Ventricular contraction
        pixel.setPixelColor(0, neopix_color);
        pixel.show();
        returnDelay = beat;
        iteration++;
        break;
      case 3:
        //Relax contraction gradually until finished
        if(dimmer > 0){
          pixel.setBrightness(dimmer);
          pixel.show();
          dimmer--;
        }else{
          //Reset dimmer and set next stage
          iteration++;
          dimmer = ANALOG_MAX;
        }
        returnDelay = hb_attenuation;
        break;
      case 4:
        //Wait for next beat
        pixel.setBrightness(ANALOG_MAX);
        pixel.setPixelColor(0,0);
        pixel.show();
        returnDelay = leftover_time;
        iteration=0;
        break;
      default:
        //If no recognized stage, reset.
        iteration=0;
        returnDelay = 0;
        break;
    }
  }
  return returnDelay;
}

/*change_heart_state: applies settings for different heartbeat animations
 * Input
 *  -i:           (byte) Heartbeat mode to be applied.
 *  -bpm:         (byte) Current beats per minute.
 *  -neopix_color:(uint32_t) Current NeoPixel Color for the heartbeat.
 *  -pixel:       (Adafruit_NeoPixel) NeoPixel object containing the LED representing a heart.
 * Output
 * The next heartbeat mode. (byte)
*/
byte change_heart_state(byte i, byte &bpm, uint32_t &neopix_color, Adafruit_NeoPixel &pixel) {
  byte new_state = i;
  switch(i){
    case 0: 
      //Default heart rate, Red color
      bpm = BPM;
      neopix_color = pixel.Color(255,0,0);
      new_state++;
      break;
    case 1:
      //Default heart rate, Blue color
      bpm = BPM;
      neopix_color = pixel.Color(0,0,255);
      new_state++;
      break;
    case 2:
      //Solid, Red color 
      bpm = BPM_MAX;
      neopix_color = pixel.Color(255,0,0);
      new_state++;
      break; 
    case 3:
      //Solid, Blue color
      bpm = BPM_MAX;
      neopix_color = pixel.Color(0,0,255);
      new_state++;
      break;
    case 4:
      //Solid, White color
      bpm = BPM_MAX;
      neopix_color = pixel.Color(255,255,255);
      new_state++;
      break;
    case 5:
      //off
      bpm = 0;
      new_state = 0;
      break;
     default:
      //Default heart rate, Red color
      bpm = BPM;
      neopix_color = pixel.Color(255,0,0);
      new_state = 0;
      break;
  }
  return new_state;
}

/*animate_vase: executes light animation of an Adafruit NeoPixel ring according to an indexed list.
 * Input
 *  -vase_animation:  (byte) Number that maps to animation type.
 *  -pixel:           (Adafruit_NeoPixel) NeoPixel object containing the LED ring.
 *  -vase_timer:      (unsigned long) Start time of previous animation step.
 *  -iteration:       (byte) counter for animation steps.
 * Output
 * None
*/
void animate_vase(byte vase_animation, Adafruit_NeoPixel &pixel, unsigned long &vase_timer, byte &iteration ) {
  unsigned long curr_time = millis();
  switch(vase_animation % 7){
    case 0:                                                 //Rainbow
      if(wait(80,vase_timer) ){
          vase_timer = curr_time;
          iteration = rainbow( VASE_RING, iteration);
      }
      break;
    case 1:                                                 //Cycle Rainbow
      if(wait(20,vase_timer) ){
            vase_timer = curr_time;
            iteration = rainbowCycle(VASE_RING, iteration);
      }
      break; 
    case 2:                                                 // Red
      if(wait(50,vase_timer) ){
        vase_timer = curr_time;
        iteration = floodRing(pixel.Color(255, 0, 0), pixel, iteration);
      }
      break;
    case 3:                                                 // Green
      if(wait(50,vase_timer) ){
        vase_timer = curr_time;
        iteration = floodRing(pixel.Color(0, 255, 0), pixel, iteration);
      }
      break;
    case 4:                                                // Blue
      if(wait(50,vase_timer) ){
        vase_timer = curr_time;
        iteration = floodRing(pixel.Color(0, 0, 255), pixel, iteration);
      }
      break;
    case 5:                                                 // White
      if(wait(50,vase_timer) ){
        vase_timer = curr_time;
        iteration = floodRing(pixel.Color(255, 255, 255), pixel, iteration);
      }
      break;
    case 6:                                               // Black/off
      if(wait(20,vase_timer) ){
        vase_timer = curr_time;
        iteration = floodRing(pixel.Color(0, 0, 0), pixel, iteration);
      }
      break;
    default:
      iteration = floodRing(pixel.Color(0, 0, 0), pixel, iteration);
      break;
  }
}

/*floodRing: floods a NeoPixel ring with chosen color of light one by one.
 * Input
 *  -color: (uint32_t) Color in NeoPixel 32bit format.
 *  -pixel: (Adafruit_NeoPixel) NeoPixel object containing the LED ring.
 *  -i:     (byte) current iteration of the loop.
 * Output
 * Next iteration of the loop. (bool)
*/
byte floodRing(uint32_t color, Adafruit_NeoPixel &pixel, byte i) {
  if(i < pixel.numPixels() ){
    pixel.setPixelColor(i, color);
    pixel.show();
    i++;
  }
  return i;
}


/*wait: determines if enough time has passed since last indicated. Used for non-blocking delays.
 * Input
 *  -wait_time:      (unsigned long) amount of miliseconds delayed.
 *  -last_performed: (unsigned long) time at which the event was last executed (miliseconds relative to start of script).
 * Output
 * Wether the specified miliseconds have passed since the last execution or not. (bool)
*/
bool wait(unsigned long wait_time, unsigned long &last_performed){
  unsigned long current = millis();
  bool wait_done = false;
  if( (current - last_performed) >= wait_time ){
    last_performed = current;
    wait_done = true;
  }
  return wait_done;
}


/*******ADAPTED ADAFRUIT FUNCTIONS*********/

//Cycles through all colors available with RGB using all pixels in the ring
byte rainbow(Adafruit_NeoPixel &pixel, byte j) {
  //Cycle through colors
  if(j<256) {
    //Set all pixels
    for(uint16_t i=0; i<pixel.numPixels(); i++) {
      pixel.setPixelColor(i, Wheel((i+j) & 255, pixel));
    }
    pixel.show();
    j++;
  }
  return j;
}

// Slightly different, this makes the rainbow equally distributed throughout
byte rainbowCycle(Adafruit_NeoPixel &pixel, byte j) {
  if(j<256) {
    for(uint16_t i=0; i< pixel.numPixels(); i++) {
      byte k = 256 - j;
      pixel.setPixelColor(i, Wheel(((i * 256 / pixel.numPixels()) + k) & 255, pixel));
    }
    pixel.show();
    j++;
  }
  return j;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos, Adafruit_NeoPixel &pixel ) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixel.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixel.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixel.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

