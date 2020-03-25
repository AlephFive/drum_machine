#include <ILI9341_t3.h>
#include <font_Arial.h>
#include <XPT2046_Touchscreen.h>
#include <SPI.h>
#define CS_PIN 8
#define TFT_DC 9
#define TFT_CS 10
XPT2046_Touchscreen ts(CS_PIN);
#define TIRQ_PIN 2
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);

#define CLOCK_A_TICK 0
#define CLOCK_B_TICK 1
#define DATA 3
#define RW 4
#define E_write 5
#define CLOCK_A_RESET 6
#define TOGGL 7
#define CLOCK_B_0 19
#define CLOCK_B_1 20
#define CLOCK_B_2 21
#define CLOCK_B_3 22
#define BLUE 16
#define YELLOW 15
#define GREEN 14

int BPM = 400;
int counter;

bool BLUE_val = 0;
bool YELLOW_val = 0;
bool GREEN_val = 0;

bool BLUE_val_temp = 0;
bool YELLOW_val_temp = 0;
bool GREEN_val_temp = 0;

int clockA_DEC = 0;
int clockA_DEC_temp = 0;

int clockB_0 = 0;
int clockB_1 = 0;
int clockB_2 = 0;
int clockB_3 = 0;

int clockB_DEC = 0;
int clockB_DEC_temp = 0;

int tick_cycle = 0;

bool SHOULD_WRITE_TO_DISP = true;
bool prevTOGGL = 1;
bool TOG_val = 1;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(1);
  
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(3);
  tft.setCursor(0,43);
  tft.printf("BPM: %d", BPM);

  pinMode(CLOCK_A_TICK, OUTPUT);
  digitalWrite(CLOCK_A_TICK, LOW);
  pinMode(CLOCK_B_TICK, OUTPUT);
  digitalWrite(CLOCK_B_TICK, LOW);  
  pinMode(RW, OUTPUT);
  digitalWrite(RW, LOW);
  pinMode(E_write, OUTPUT);
  digitalWrite(E_write, LOW);
  pinMode(CLOCK_A_RESET, OUTPUT);
  digitalWrite(CLOCK_A_RESET, HIGH);

  pinMode(BLUE, OUTPUT);
  digitalWrite(BLUE, LOW);
  pinMode(YELLOW, OUTPUT);
  digitalWrite(YELLOW, LOW);
  pinMode(GREEN, OUTPUT);
  digitalWrite(GREEN, LOW);
  
  pinMode(CLOCK_B_0, INPUT);
  pinMode(CLOCK_B_1, INPUT);
  pinMode(CLOCK_B_2, INPUT);
  pinMode(CLOCK_B_3, INPUT);
  pinMode(TOGGL, INPUT);
  pinMode(DATA, INPUT);
 
  resetClock_A();
  resetClock_B_to_VAL(0);
  
  Serial.println("SUCCESS");

  

  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(10, 162);
  tft.printf("YELLOW %d", YELLOW_val);

  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(2);
  tft.setCursor(10, 182);
  tft.printf("GREEN %d", GREEN_val);

  tft.setTextColor(ILI9341_CYAN);
  tft.setTextSize(2);
  tft.setCursor(10, 142);
  tft.printf("BLUE %d", BLUE_val);
}

void loop() {
  //TOGGL goes from HIGH to LOW
  prevTOGGL = TOG_val;
  TOG_val = digitalRead(TOGGL);
  SHOULD_WRITE_TO_DISP = ((TOG_val != prevTOGGL) && (prevTOGGL == 1))? true:false;
  
  
  if(!SHOULD_WRITE_TO_DISP){
    if(counter >= BPM*100){
      tick(CLOCK_A_TICK);
      MODE_ReadData();
      PlaySounds(); 
      MODE_WriteDispToMem();
      counter = 0;
    }
  }
  else{
    Serial.println("WriteToMEM");
    MODE_WriteMemToDisp();
  }

  /*
  for(int i = 0; i < 40; i++){
      MODE_WriteDispToMem();

      if(i == 20){
        MODE_WriteMemToDisp(); 
      }
  }
*/
  updateDebug();
  counter++;
}

void updateDebug(){
  if(clockB_DEC_temp != clockB_DEC || clockA_DEC_temp != clockA_DEC){
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 102);
    tft.printf("Clock A: %d", clockA_DEC_temp);
    tft.setCursor(10, 122);
    tft.printf("Clock B: %d", clockB_DEC_temp);

    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.setCursor(10, 102);
    tft.printf("Clock A: %d", clockA_DEC);
    tft.setCursor(10, 122);
    tft.printf("Clock B: %d", clockB_DEC);

    clockA_DEC_temp = clockA_DEC;
    clockB_DEC_temp = clockB_DEC;
  }

  if(BLUE_val_temp != BLUE_val){
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 142);
    tft.printf("BLUE %d", BLUE_val_temp);

    tft.setTextColor(ILI9341_CYAN);
    tft.setTextSize(2);
    tft.setCursor(10, 142);
    tft.printf("BLUE %d", BLUE_val);

    BLUE_val_temp = BLUE_val;
  }

  if(YELLOW_val_temp != YELLOW_val){
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 162);
    tft.printf("YELLOW %d", YELLOW_val_temp);

    tft.setTextColor(ILI9341_YELLOW);
    tft.setTextSize(2);
    tft.setCursor(10, 162);
    tft.printf("YELLOW %d", YELLOW_val);

    YELLOW_val_temp = YELLOW_val;
  }
  
  if(GREEN_val_temp != GREEN_val){
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 182);
    tft.printf("GREEN %d", GREEN_val_temp);

    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.setCursor(10, 182);
    tft.printf("GREEN %d", GREEN_val);

    GREEN_val_temp = GREEN_val;
  }
  
}

void PlaySounds(){
  //play back sounds
  //read vertically ie blue yellow green
  String command = String(BLUE_val)+String(YELLOW_val)+String(GREEN_val);
  Serial.print(command);
}

void MODE_WriteMemToDisp(){
  digitalWrite(E_write, HIGH);
  digitalWrite(RW, HIGH);
  delayMicroseconds(200);
  for(int i = 0; i < 16; i++){
    delayMicroseconds(200);
    tick(CLOCK_B_TICK);
    Serial.println(readClock_B());
  }
  delayMicroseconds(200);
  digitalWrite(E_write, LOW);
  digitalWrite(RW, LOW);
}

void MODE_WriteDispToMem(){
  digitalWrite(E_write, LOW);
  digitalWrite(RW, LOW);
  
  for(int i = 0; i < 16; i++){
    delayMicroseconds(1);
    tick(CLOCK_B_TICK);
  }
  digitalWrite(E_write, LOW);
  digitalWrite(RW, LOW);
}

void MODE_ReadData(){
  resetClock_B_to_VAL(clockA_DEC);
  
  digitalWrite(E_write, LOW);
  digitalWrite(RW, HIGH);

  digitalWrite(BLUE, HIGH);
  delayMicroseconds(1);
  BLUE_val = digitalRead(DATA);
  digitalWrite(BLUE, LOW);

  digitalWrite(YELLOW, HIGH);
  delayMicroseconds(1);
  YELLOW_val = digitalRead(DATA);
  digitalWrite(YELLOW, LOW);

  digitalWrite(GREEN, HIGH);
  delayMicroseconds(1);
  GREEN_val = digitalRead(DATA);
  digitalWrite(GREEN, LOW);

  digitalWrite(E_write, LOW);
  digitalWrite(RW, LOW);
}

void tick(int PIN){
  digitalWrite(PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(PIN, LOW);

  if(PIN == CLOCK_A_TICK){
    if(clockA_DEC == 15){
      clockA_DEC = 0;
    }
    else{
      clockA_DEC++;
    }
  }
  
}

void resetClock_B_to_VAL(int val){
  for(int i = 0; i < 16; i++){
    delayMicroseconds(1);
    readClock_B();
    if(clockB_DEC == val){
      break;
    }
    else{
      tick(CLOCK_B_TICK);
    }
  }
}

void resetClock_A(){
  clockA_DEC = 0;
  digitalWrite(CLOCK_A_RESET, LOW);
  delayMicroseconds(1);
  tick(CLOCK_A_TICK);
  delayMicroseconds(1);
  digitalWrite(CLOCK_A_RESET, HIGH);
}

int readClock_B(){
  clockB_0 = digitalRead(CLOCK_B_0);
  clockB_1 = digitalRead(CLOCK_B_1);
  clockB_2 = digitalRead(CLOCK_B_2);
  clockB_3 = digitalRead(CLOCK_B_3);
  clockB_DEC = clockB_0 + clockB_1*2 + clockB_2*4 + clockB_3*8;
  return clockB_DEC;
}



