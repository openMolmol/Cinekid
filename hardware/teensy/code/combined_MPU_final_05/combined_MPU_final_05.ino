

#define MASTER 0        // 1 = you are the BT MASTER; 0 = you are the SENDER / SLAVE
#define USE_LSM303 1
//---------------------------------------------------------- sensors

//#define DEBUG_SERIAL


#include <Adafruit_L3GD20_U.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_9DOF.h>

/* Assign a unique ID to the sensors */
Adafruit_9DOF                dof   = Adafruit_9DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);

/* Update this with the correct SLP for accurate altitude measurements */
float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;



/**************************************************************************/
/*!
    @brief  Initialises all the sensors used by this example
*/
/**************************************************************************/

#define N_SAMPLES_TO_RECORD (5*30)   // about 30 fps 
unsigned short samples[N_SAMPLES_TO_RECORD];
int sampleCounter = 0;
float valSmooth = 0;

void addSample(short val){
  samples[sampleCounter] = val;
  sampleCounter++;
  sampleCounter %= N_SAMPLES_TO_RECORD;
}

int findMin(){
  int minVal = 100000;
  for (int i = 0; i < N_SAMPLES_TO_RECORD; i++){
    if (samples[i] < minVal) minVal = samples[i];
  }
  return minVal;
}

int findMax(){
  int maxVal = -100000;
  for (int i = 0; i < N_SAMPLES_TO_RECORD; i++){
    if (samples[i] > maxVal) maxVal = samples[i];
  }
  return maxVal;
}


void initSensors()
{
  
  
  if(!accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    #ifdef DEBUG_SERIAL
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    #endif
    while(1);
  }
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    #ifdef DEBUG_SERIAL
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    #endif
    
    while(1);
  }
}

/**************************************************************************/



//MPU6050 imu;
int16_t mx, my, mz;
int16_t gx, gy, gz;
int16_t ax, ay, az;

float yaw, pitch, roll;
char 	sensorMessage[200]; //long message!

//---------------------------------------------------------- xbee
HardwareSerial2 xbeeSerial = HardwareSerial2();
int 	xbeeMessageCounter;
char 	xbeeMessage[200]; //long message!
char 	xbeeMessageLastOne[200]; //long message!
int     sendFrame = false; //sendFrame (could become a state for individual sensors)
boolean sentXbeeOverBluetooth;

//---------------------------------------------------------- bluetooth
HardwareSerial3 btSerial = HardwareSerial3();
int spkrPin = 2;
int ctr = 0;
boolean bConnected = false;
boolean bSpeaking = false;
boolean bGotHearbeat = false;
unsigned long lastConnectedMessageTime;

//---------------------------------------------------------- timers and states
unsigned long cTime = 0;
unsigned long btStatusCheckTime = 0; //time stamp for bt connection status check
unsigned long lastRecvdSlaveTime = 0; //time stamp for last received message from slave
unsigned long frameCount = 0;

//----------------------------------------------------------- mac address
uint8_t mac[6];
char mac_string[6];      // this is a string ...

byte led[] = {
  3, 4, 5, 6, 23, 22, 21, 20};
int numLeds;
float ledEnergy;






//---------------------------------------------------------- setup
void setup() { 

  numLeds = sizeof(led);
  
  sentXbeeOverBluetooth = false;

#ifdef DEBUG_SERIAL
  Serial.begin(57600); // begin USB serial
#endif
  
  delay(500);
  xbeeSerial.begin(57600); //will try increasing this baud rate !

  delay(330);
  #ifdef DEBUG_SERIAL
  Serial.println("hello world");
  #endif
  
  pinMode(13, OUTPUT); //turn on teensy's orange LED
  digitalWrite(13, HIGH);  //so that we know it's on
  
  // get a unique string for this teensy....
  if (MASTER){
    read_mac();
  } else {
    sprintf(mac_string, "0000");
  }
  
  ledEnergy = 0;

  delay(330);

  if (MASTER){
    #ifdef DEBUG_SERIAL
    Serial.println("hello bluetooth");
    #endif
    setupBluetooth(mac_string);
  }
  pinMode(spkrPin, OUTPUT);  // turn on speaker pin
  digitalWrite(spkrPin, HIGH);  // (simple PNP switch)

#ifdef DEBUG_SERIAL
  Serial.println("battery status: ");
  Serial.println(analogRead(11));
  Serial.println("hello xbee");
#endif
  
  // start hardware serial 3 (connected to XBee)
  delay(330); //delay to open the serial monitor in time
  xbeeMessageCounter = 0;
  //xbeeFrameCounter = 0;

  
  
  bluetoothIncreaseVol();
  
  #ifdef DEBUG_SERIAL
  Serial.println("done with setup");
  #endif
  
  for (int i=0; i<numLeds; i++){
      pinMode(led[i], OUTPUT);
  }
  
  #ifdef DEBUG_SERIAL
  Serial.println("hello sensors");
  #endif
  
  setupSensors();
  //testMe();
  
 if (USE_LSM303){
  initSensors();
 }
} 

//---------------------------------------------------------- go
void loop() { 
//testMe();
//Serial.println("sasdfasdf");
   //void loopDMP();
 //  Serial.println("hello");
  if (bSpeaking){
    
    ledEnergy = ledEnergy * 0.97 + 0.03 * 1.0;
    
    
     
  } else {
    
    ledEnergy = ledEnergy * 0.9 + (1.0 - 0.9) * 0.0;
    
    
  }
  
  if (ledEnergy > 0.03){
    for (int i = 0; i < numLeds; i++){
        int ledPin = led[i];
        if (random(0,100) > 50 * ledEnergy){
          digitalWrite(led[i], LOW);
        } else {
          digitalWrite(led[i], HIGH);
        }
     } 
  } else {
    for (int i = 0; i < numLeds; i++){
        int ledPin = led[i];
        digitalWrite(led[i], LOW);
     } 
  }
    
  
  //Serial.print("bConnected ");
  //Serial.println(bConnected);
  //  cTime = millis();                                        // currentTime

  //frameCount++;

  if(MASTER){                                              //--- start MASTER loop

    //Serial.println("master loop");
    //Serial.println(bConnected);
    
    updateSensors();
    readFromXbee();  
    writeToXbee(); 
    // read from Xbee every frame
    if(bConnected){                                        // bluetooth is connected
      sendToBluetooth(sensorMessage);  
      if (sentXbeeOverBluetooth == false){
        //sendToBluetooth(xbeeMessageLastOne); 
        sentXbeeOverBluetooth = true; 
      } 
      bluetoothFlushOut();
      delay(20);
    } 
    else {
      bluetoothFlushOut();
      btStatusCheckTime = millis(); 
    }
  }

  if(!MASTER){ //---- slave loop
    updateSensors();
    //readFromXbee();
    writeToXbee();
    delay(27);
  } //--- end slave
}


//----------------------------------------------------------
//------------------- xbee ---------------------------------
//----------------------------------------------------------

void writeToXbee(){ 

  if(MASTER) { // sending commands/requests to slave
    //nothing
    if (bGotHearbeat){
      if (bConnected && !bSpeaking) xbeeSerial.print(">");
      else if (bConnected && bSpeaking) xbeeSerial.print("}");
    }
    
  }

  if(!MASTER){  // get your data together
    
    for (int i = 0; i < (int)strlen(sensorMessage); i++){
      xbeeSerial.print(sensorMessage[i]); 
    }
    xbeeSerial.println("--");
    #ifdef DEBUG_SERIAL
    Serial.print("wrote to xbee at ");
    Serial.println(millis());
    Serial.println("");
    #endif
    
  }
}

//----------------------------------------------------------
void readFromXbee(){
  
  if(MASTER) {    //looking for data from slave

    while (xbeeSerial.available()){

      char val = xbeeSerial.read();
      if (val == '!'){
        xbeeMessageCounter = 0;
        memset(xbeeMessage, 0, 200);    // is this slow?
      }
      if (xbeeMessageCounter < 200){
        xbeeMessage[xbeeMessageCounter] = val;
      }
      xbeeMessageCounter++;

      if (val == '#'){         // last char!  let's rock and roll....	
        //sprintf(xbeeMessageLastOne, "%s", xbeeMessage);
        //sentXbeeOverBluetooth = false;
        
        Serial.println("got xbee message");
        if(bConnected) sendToBluetooth(xbeeMessage);                    // send slave data to phone
      }
    } //--- end while
  } //--- end MASTER


  if(!MASTER){   // look for commands
    //do nothing
     while (xbeeSerial.available()){
       
     char val = xbeeSerial.read();
     
     Serial.println(val);
      if (val == '>'){
        bSpeaking = false;
      } else if (val == '}'){
        bSpeaking = true;
      }
        
   }
    
    
  } //--- end slave
}






//----------------------------------------------------------
//------------------- sensors ------------------------------
//----------------------------------------------------------

void setupSensors(){
  //Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_INT, I2C_RATE_400);
  delay(100);
  //setupDMP();
  //Serial.println("Initializing I2C devices...");
  //imu.initialize();
  // verify connection
  //Serial.println("Testing device connections...");
  //Serial.println(imu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

//----------------------------------------------------------
void updateSensors(){
  
  int fsr3;
  fsr3 = analogRead(A10);
  int fsr1;
  fsr1 = analogRead(A0);
  int fsr2;
  fsr2 = analogRead(A1);  
  
  
   sensors_event_t accel_event;
  sensors_event_t mag_event;
  sensors_vec_t   orientation;
  
  if (USE_LSM303){


  /* Calculate pitch and roll from the raw accelerometer data */
  accel.getEvent(&accel_event);
 if (dof.accelGetOrientation(&accel_event, &orientation))
  {
//    /* 'orientation' should have valid .roll and .pitch fields */
    #ifdef DEBUG_SERIAL
    Serial.print(F("Roll: "));
    Serial.print(orientation.roll);
    Serial.print(F("; "));
    Serial.print(F("Pitch: "));
    Serial.print(orientation.pitch);
    Serial.print(F("; "));
    #endif
  }
  
  /* Calculate the heading using the magnetometer */
  mag.getEvent(&mag_event);
  if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation))
  {
//    /* 'orientation' should have valid .heading data now */
#ifdef DEBUG_SERIAL
    Serial.print(F("Heading: "));
    Serial.print(orientation.heading);
    Serial.print(F("; "));
    #endif
    
  }
#ifdef DEBUG_SERIAL
  Serial.println(F(""));
  #endif
  
  }
 
 
 
  gx = 0;
  gy = 0;
  gz = 0;
  mx = 0;
  my = 0;
  mz = 0;
  int mHeading = 0;
  //imu.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
  //int mHeading = 0;
  sprintf( sensorMessage, "!,%i,%i,%i,%i#",  fsr3, (int)(orientation.roll * 1000), (int)(orientation.pitch*1000), (int)(orientation.heading*1000)) ;
  //Serial.println(strlen(sensorMessage));
  
  if (false){
        addSample(fsr3);
        int minVal = findMin();
        int maxVal = findMax();
        if (maxVal == minVal) maxVal += 1;
        int val = map(fsr3, minVal, maxVal, 0, 8);
        valSmooth = 0.7f * valSmooth + 0.3 * val;
        
        for (int i = 0; i < 8; i++){
          int ledPin = led[i];
          if (valSmooth < i){
             digitalWrite(led[i], LOW);
           }  else {
              digitalWrite(led[i], HIGH);
         }
        }
  } else {
    
    int val = map(fsr3, 200, 500, 0, 8);
    
    for (int i = 0; i < 8; i++){
          int ledPin = led[i];
          if (val < i){
             digitalWrite(led[i], LOW);
           }  else {
              digitalWrite(led[i], HIGH);
         }
        }
  }
  
}




  









