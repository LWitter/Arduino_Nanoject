float Load(){
  // This function loads the calibration from EEPROM (2-5) and converts it to the floating point variable we need
  int CharacterOne[8];
  int CharacterTwo[8];
  int DecimalOne[8];
  int DecimalTwo[8];
  int saved, i;
  
  // Now we extract the calibration from the EEPROM
  saved = EEPROM.read(2);
  Serial.println("EEPROM 2:");
  Serial.println(saved);
  IntegerToBinaryArray(saved);
  // And fill the arrays:
  for(i=0;i<8;i++){
   BinaryCharacterArray[i] = BinaryArray[i]; 
  }
  saved = EEPROM.read(3);
  Serial.println("EEPROM 3:");
  Serial.println(saved);
  IntegerToBinaryArray(saved);
  for(i=0;i<8;i++){
   BinaryCharacterArray[i+8] = BinaryArray[i]; 
  }
  saved = EEPROM.read(4);
  Serial.println("EEPROM 4:");
  Serial.println(saved);
  IntegerToBinaryArray(saved);
  for(i=0;i<8;i++){
   BinaryDecimalArray[i] = BinaryArray[i]; 
  }
  saved = EEPROM.read(5); 
  Serial.println("EEPROM 5:");
  Serial.println(saved);
  IntegerToBinaryArray(saved);
  for(i=0;i<8;i++){
   BinaryDecimalArray[i+8] = BinaryArray[i]; 
  }
  // Get the Floatingpoint:
  Serial.println();
  for(i=0;i<16;i++){
    Serial.print(BinaryCharacterArray[i]);
    Serial.print("     ");
    Serial.print(BinaryDecimalArray[i]);
    Serial.println();
  }
  Serial.println();
  CalibrationSlope = BinaryArraysToFloat();
  Serial.println("Getting the calibrationslope:");
  Serial.println(CalibrationSlope);
  return CalibrationSlope;
}

void Save(float Calib){
  int tosave;
  // We get the floating point handed from the calibration and we need to save it to EEPROM
  // First we convert the value to Binary arrays:
  float remainder = FloatToBinaryArray(Calib);
  // Now we have the two arrays with the character and decimal.
  // Split the two arrays, convert to an integer and save to EEPROM:
  for (int i=0;i<8;i++){
    BinaryArray[i] = BinaryCharacterArray[i];
  } 
  // Convert the array to an integer:
  tosave = BinaryArrayToInteger(BinaryArray);
  // Write to EEPROM:
  EEPROM.write(2,tosave);
  Serial.println("Saving 2:");
  Serial.println(tosave);
  
  for (int i=0;i<8;i++){
    BinaryArray[i] = BinaryCharacterArray[i+8];
  }
  tosave = BinaryArrayToInteger(BinaryArray);
  EEPROM.write(3,tosave);
  Serial.println("Saving 3:");
  Serial.println(tosave);
  
  for (int i=0;i<8;i++){
    BinaryArray[i] = BinaryDecimalArray[i];
  }
  tosave = BinaryArrayToInteger(BinaryArray);
  EEPROM.write(4,tosave);
  Serial.println("Saving 4:");
  Serial.println(tosave);
  
  for (int i=0;i<8;i++){
    BinaryArray[i] = BinaryDecimalArray[i+8];
  }
  tosave = BinaryArrayToInteger(BinaryArray);
  EEPROM.write(5,tosave);
  Serial.println("Saving 5:");
  Serial.println(tosave);
  
  
  
  CalibrationSlope = BinaryArraysToFloat();
  Serial.println(CalibrationSlope);
}

float FloatToBinaryArray(float floatvalue){
  for(int bitpos=15;bitpos>=0;bitpos--){
    if(floatvalue/pow(2,(bitpos))>1){
      floatvalue -=pow(2,(bitpos));;
      BinaryCharacterArray[bitpos] = 1;
    }
    else{
      BinaryCharacterArray[bitpos] = 0;
    }
  }
  BinaryDecimalArray[0] = 0;
  for(int bitpos=1;bitpos<=15;bitpos++){
    if(floatvalue-(1/pow(2,bitpos))>0){
      floatvalue -= (1/pow(2,bitpos));
      BinaryDecimalArray[bitpos] = 1;
    }
    else{
      BinaryDecimalArray[bitpos] = 0;
    }
  }
 return(floatvalue);  // This is to check the remainder!
}

float BinaryArraysToFloat(){
  float floatvalue=0;
  Serial.println("Checking the power rules for Binary==>Float");
  int power;
  for(int bitpos=0;bitpos<15;bitpos++){
    power = 1;
    for(int j=1;j<=bitpos;j++){
      power *=2;
    }
    Serial.print(bitpos);
    Serial.print("   ");
    Serial.println(power);
    floatvalue+=power*BinaryCharacterArray[bitpos];
    if(bitpos!=0){
      Serial.println("    Inside the decimal loop");
      Serial.print("    ");
      floatvalue+=(1/float(power))*BinaryDecimalArray[bitpos];
      Serial.println((1/float(power))*BinaryDecimalArray[bitpos]);
    }
  }
  Serial.println("FloatValue:");
  Serial.println(floatvalue);
  return floatvalue;
}

int BinaryArrayToInteger(int Array[8]){
  int IntegerValue, power = 1;
  for (int i=0;i<8;i++){
    if(i!=0){
      power*=2;
    }
    IntegerValue += power*Array[i];
  }
  return IntegerValue;
}

int IntegerToBinaryArray(int IntegerValue){
  int power = 1;
  for(int i=7;i>=0;i--){
    for(int j=1;j<=i;j++){
      // We can't realibly do pow() on integers on arduino. So, we make our own power function:
      power *=2;
    }
    if(IntegerValue>=power){
      IntegerValue -= power;
      BinaryArray[i] = 1;
    }
    else {
      BinaryArray[i] = 0;
    }
    power = 1;
  }
}

