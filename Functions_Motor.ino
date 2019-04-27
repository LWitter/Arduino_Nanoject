// Functions to move the stepper
void motorOff(){
  digitalWrite(dir_a,LOW);
  digitalWrite(dir_b,LOW);
  digitalWrite(pwm_a,LOW);
  digitalWrite(pwm_b,LOW);
}
void CW(int w){
  if(MotorPosition<=MaxMotorPosition){
    digitalWrite(pwm_a,HIGH);
    digitalWrite(pwm_b,HIGH);
    if(dir_a_val==dir_b_val){
      dir_a_val=!dir_a_val;
      digitalWrite(dir_a,dir_a_val);  
    }
    else {
      dir_b_val=!dir_b_val;
      digitalWrite(dir_b,dir_b_val);
    }
    MotorPosition++;
  }
  else {
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Plunger fully");
    lcd.setCursor(4,1);
    lcd.print("extended!");
  }
  delay(w);
}

void CCW(int w){
  if(MotorPosition>=0){
    digitalWrite(pwm_a,HIGH);
    digitalWrite(pwm_b,HIGH);
    if(digitalRead(dir_a)==digitalRead(dir_b)){
      digitalWrite(dir_b,!digitalRead(dir_b));
    }
    else {
      digitalWrite(dir_a,!digitalRead(dir_a));
    }
    MotorPosition--;
  }
  else{
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Plunger fully");
    lcd.setCursor(3,1);
    lcd.print("retracted!");
  }
  delay(w);
}

void DoInjection(int time, int vol){
  // First we need to convert the volume to the number of steps, so we need to pull the calibration from EEPROM:
  float nrsteps = float(Volume)/float(CalibrationSlope);
  // We make it a floating point so we can estimate the error we are going to make:
  Serial.println(int(nrsteps));
  int OneStepUp = int(nrsteps+1);
  int OneStepDown = int(nrsteps);
  float OneUpVol = float(OneStepUp)*CalibrationSlope;
  float OneDownVol = float(OneStepDown)*CalibrationSlope;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Upper: ");
  lcd.print(OneUpVol);
  lcd.setCursor(0,1);
  lcd.print("Lower: ");
  lcd.print(OneDownVol);

  lcd.setCursor(15,0);
  lcd.print("+");
  lcd.setCursor(15,1);
  lcd.print("-");
  // Now we want to get a choice of either + or - and then do that number of (integer) steps over time.
  int activeButton = 0, done = 0;
  while (activeButton == 0 && done==0) {
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 0: // When button returns as 0 there is no action taken
        break;
      case 2: // UP
        // Choice was made for the large injection, so nrsteps++
        nrsteps++;
        done = 1;
        break;
      case 3: // DOWN
        // Choice was made for the small injection, so nrsteps
        done = 1;
        break;
      case 4: // BACK/ CANCEL
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("Cancel, return");
        lcd.setCursor(1,1);
        lcd.print("to main menu.");
        delay(500);
        lcd.clear();
        return;
      }
  }
  // Then we need to space the steps over the amount of time and loop through the steps.
  // That means constructing a for loop with the number of needed injections, and a delay statement to space it all out.
  lcd.clear();
  // We do a number of steps, so we can make a progressbar:
  lcd.setCursor(3,0);
  lcd.print("Progress:");
  for(int i=1;i<=nrsteps;i++){
    CW(20);      // Do the injection!
    // Draw the progress bar:
    // Progress is 0-100% over 10 characters.
    // Start writing the percentage on character 11:
    int percentage = i/nrsteps*100;
    Serial.println(percentage);
    lcd.setCursor(11,1);
    lcd.print(percentage);
    lcd.print("%");
    // Figure out how many bars to fill:
    lcd.setCursor(0,1);
    while (percentage>=10){
      if(percentage>=10){
        lcd.write(byte(7));  // Full block;
        percentage -=10;
      }
    }
    // We are now left with a percentage in single digits:
    Serial.println(percentage);
    if(percentage>=2 && percentage<4){
      lcd.write(byte(3));
    }
    else if(percentage>=4 && percentage<6){
        lcd.write(byte(4));
    }
    else if(percentage>=6 && percentage<=8){
          lcd.write(byte(6));
    }  
    
    // Wait time per cycle is: time/(nrsteps-1)
    delay((float(time)/(nrsteps-1))*1000);
  }
  // Injection done!
  lcd.setCursor(0,1);
  for (int i=1;i<=10;i++){
    lcd.write(byte(7));
  }
  lcd.setCursor(11,1);
    lcd.print("100%");
delay(5000);
    return;
  // Get some sort of progress bar over the bottom row, countdown to zero and slowly filling bar:
  // We use ten characters for the bar, a indication of 0% on the left, and 100% on the right, that's 16 chars.
  // Then we have the countdown on the top row, starting at char 10
}

void DoCalibration(){
  // We arrive here from the calibration menu directly. We dispense several different amounts and ask for user
  // input to tell the diameter of the droplet. We then calculate the dispensed volume based on the given
  // diameter to create a calibration line. We save the slope of this line.
  // We dispense, 1, 5, 10, 25, 50, 100, 250, 500 steps.
  
  // Dispense once:
 for (int i=0;i<NrCalibrationSteps;i++){
   int nrsteps = CalibrationSteps[i];
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Running ");
   lcd.print(nrsteps);
   lcd.print(" steps");
   delay(500);
   for (int j=0;j<nrsteps;j++){
     CW(20);
   }
   CalibrationSub(i);
   Serial.print(nrsteps);
   Serial.print("   ");
   Serial.println(CalibrationVolumes[i]);
   Serial.println();
 }
  // Do the fit to the collected datapoints to get our slope:
  // We have 8 points:
  for(int i=0; i<NrCalibrationSteps;i++){
    sum_xy +=(CalibrationSteps[i]*CalibrationVolumes[i]);
    sum_xx +=pow(CalibrationSteps[i],2);
    avgVolumes += CalibrationVolumes[i];
  }
  Serial.println("SumXY");
  Serial.println(sum_xy);
  Serial.println("SumXX");
  Serial.println(sum_xx);
  Serial.println("AvgVol");
  Serial.println(avgVolumes);
  // Now we have the sums and we can calculate the slope:
  CalibrationSlope = (sum_xy/sum_xx);
  // We now calculate how good the fit really is
  for(int i=0;i<NrCalibrationSteps;i++){
    Rtot += pow(CalibrationVolumes[i]-avgVolumes,2);
    Rres += pow(CalibrationVolumes[i]-(CalibrationSteps[i]*CalibrationSlope),2); 
  }
  Rsquare = (1-(Rres/Rtot));
  return;
}
