

char bt_index = 0;

uint8_t put_chr_buf(char c){
  // puts a character into the bt_buffer.
  // If the character is '\n' returns 1 (and doesn't put it into the buffer)
  // else returns 0
  if(c == '\n'){
      return 1;
  }
  if(bt_index >= BT_BUF_LEN - 2){
    return 0;
  }
  bt_buffer[bt_index] = c;
  bt_index++;
  bt_buffer[bt_index] = 0;
  return 0;
}

void analyze_buf(){
  // Determines if the buffer contains the SS notation
  if(bt_connected == -1){
    if(strncmp(bt_buffer, "SS=", 3) == 0){
      bt_connected = 0;
      Serial.println(F("1st Connect to BT"));
    }
  }
  else{
    if(strncmp(bt_buffer, "OK", 2) == 0){
    }
    else{
      Serial.println(F("ERROR: Unknown response:"));
      Serial.println(bt_buffer);
    }
    bt_connected = 0;
  }
  bt_buffer[0] = 0;
  bt_index = 0;
}

void update_bt_status(){
  // This should be called in the loop with at least a hundred milliescond delay before it is called again.
  // Determine if the BT module is connected or not.
  uint8_t temp = 0;
  if(mySerial.available()){
    while(mySerial.available()){
      temp = put_chr_buf(mySerial.read());
      if(temp){
        analyze_buf();
      }
    }
  }
  else{
    if(bt_connected >= 0){
      bt_connected = 1;
    }
  }
  
  // clear excess
  while(mySerial.available()){
    mySerial.read();
  }
  
  // Send out another signal for the next loop
  mySerial.println(F("BC:UI=01"));
  
  // Indicate connection status via the LED
  digitalWrite(LEDPIN, HIGH);
  if(bt_connected < 1){
    delay(100);
    digitalWrite(LEDPIN, LOW);
  }
}
