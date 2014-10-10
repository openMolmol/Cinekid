

//----------------------------------------------------------
//------------------- bluetooth ----------------------------
//----------------------------------------------------------


void sendToBluetooth( char * data){
  //Serial.println("sendToBluetooth");
  if (bConnected == false) return;

  ///Serial.print("string length (data): ");
  //Serial.println(strlen(data));

  if (strlen(data) > 40){  //needed to break it up even more for accel+gyro+mag reporting
    //btSerial.print("aty\"");
    for (int i = 0; i < 20; i++){
      btSerial.print(data[i]);
    }
    //btSerial.println("\"");
    //btSerial.print("aty\"");
    for (int i = 20; i < 40; i++){
      btSerial.print(data[i]);
    }
    //btSerial.println("\"");
    //btSerial.print("aty\"");
    for (int i = 40; i < strlen(data); i++){
      btSerial.print(data[i]);
    }
    //btSerial.println("\"");
    //btSerial.println("aty\"\"");
  }

  else if (strlen(data) > 20){ 

    //btSerial.print("aty\"");
    for (int i = 0; i < 20; i++){
      btSerial.print(data[i]);
    }
    //btSerial.println("\"");
    //btSerial.print("aty\"");
    for (int i = 20; i < strlen(data); i++){
      btSerial.print(data[i]);
    }

    //btSerial.println("\"");
    //btSerial.println("aty\"\"");
  } 

  else {
    //btSerial.print("atx\"");
    for (int i = 0; i < strlen(data); i++){
      btSerial.print(data[i]);
    }
    //btSerial.println("\"");
  }
}

//----------------------------------------------------------- setup
void setupBluetooth(char * nameStr){
  //btSerial.begin(9600);
  char name[100];
  sprintf(name, "Talking Shoe %s", nameStr);
  
  btSerial.begin(57600);
  pinMode(spkrPin, OUTPUT);  // turn on speaker pin
  digitalWrite(spkrPin, HIGH);  // (simple PNP switch)
  delay(500); //delay to open the serial monitor in time
  bluetoothConfig(name);
  lastConnectedMessageTime = millis();
}

void bluetoothDropOutofCommandMode(){
  //  Serial.println("------------------------------------");
  //  btSerial.println("ats531=0");  
  //  delay(100);
  //  bluetoothFlushOut();
  //  Serial.println("------------------------------------");
}

//---------------------------------------------------------- maintenance
void bluetoothFlushOut() {
  byte incomingByte;
  bGotHearbeat = false;
  
  if ( (millis() - lastConnectedMessageTime) > 3000){
     bConnected = false; 
  }
  //Serial.println(btSerial.available());
  while (btSerial.available()) {
    incomingByte = btSerial.read();
    if (incomingByte == '>' || incomingByte == '}'){
      bConnected = true;
      bGotHearbeat = true;
      if ( incomingByte == '}' ) bSpeaking = true;
      else bSpeaking = false;
      lastConnectedMessageTime = millis();
    }
    Serial.write(incomingByte);
  }
}

//void clearOutbluetoothBuffer(){
//  byte incomingByte;
//  while (btSerial.available()){
//    incomingByte = btSerial.read();
//    Serial.write(incomingByte); //print to serial monitor
//  }
//}

void checkConnectionBluetooth(){
  Serial.println("checkConnectionBluetooth()");
  btSerial.println("ati9");

  delay(50);

  byte incomingByte;
  while (btSerial.available()) {
    incomingByte = btSerial.read();
    if (incomingByte == '0') bConnected = false;
    if (incomingByte == '1'){
      bConnected = true;
      //shoeToggle = true; // make sure we send local data first (which also sends data request to slave)
    }
  }
}



//------------------- old, working as SPP
void bluetoothConfig(String permanentName) {  

  
  
  Serial.println("in bluetooth config");
  // perform factory reset:  
  btSerial.println("at&f*");
  delay(2000);
  bluetoothFlushOut();

  // configure the module to startup (and remain in) a  connectable state:
  btSerial.println("ats512=4");  // default to connectable (but not discoverable)
  delay(100);
  bluetoothFlushOut();

  btSerial.println("ats554=0");  // never time out of this state
  delay(100);
  bluetoothFlushOut();

  btSerial.println("ate1");  // enable echo
  delay(100);
  bluetoothFlushOut();

  // specify that the device has no input/output capabilities 
  // (no button, shoe should accept
  btSerial.println("ats321=3");
  delay(100);
  bluetoothFlushOut();  

  // when the host attempts to open the serial port, auto-answer after one ring
  btSerial.println("ats0=1");
  delay(100);  
  bluetoothFlushOut();

  //enable local command mode
  btSerial.println("ats531=0");
  delay(100);
  bluetoothFlushOut();

  // set the device's permanent friendly name
  btSerial.print("at+btn=\"");
  btSerial.print(permanentName);
  btSerial.println("\"");
  delay(100);
  bluetoothFlushOut();

  btSerial.println("at+btc001414");//device class: LOUDSPEAKER w/SERIAL ENABLED
  delay(100);
  bluetoothFlushOut();
//  
//  btSerial.println("ats520=57600");
//  btSerial.end();
//  delay(100);
//  btSerial.begin(57600);
//  delay(100);
  // write the new settings to non-volatile memory:
  btSerial.println("at&w");
  delay(2000);
  bluetoothFlushOut();

  // restart the module to have new settings take effect:
  btSerial.println("atz");
  delay(2000);
  bluetoothFlushOut();
}

void bluetoothIncreaseVol(){

  for(int i = 0; i< 3; i++){      // 3 step
    btSerial.println("AT+GOU");  // increases by 1 step
    delay(200);
    bluetoothFlushOut();
  }
}
