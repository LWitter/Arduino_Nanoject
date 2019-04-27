// This function will generate the 2 menu items that can fit on the screen. They will change as you scroll through your menu. Up and down arrows will indicate your current menu position.
void mainMenuDraw() {
  Serial.print(menuPage);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(menuItems[menuPage]);
  lcd.setCursor(1, 1);
  lcd.print(menuItems[menuPage + 1]);
  if (menuPage == 0) {
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
  } else if (menuPage > 0 and menuPage < maxMenuPages) {
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
  } else if (menuPage == maxMenuPages) {
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
  }
}

// When called, this function will erase the current cursor and redraw it based on the cursorPosition and menuPage variables.
void drawCursor() {
  for (int x = 0; x < 2; x++) {     // Erases current cursor
    lcd.setCursor(0, x);
    lcd.print(" ");
  }

  // The menu is set up to be progressive (menuPage 0 = Item 1 & Item 2, menuPage 1 = Item 2 & Item 3, menuPage 2 = Item 3 & Item 4), so
  // in order to determine where the cursor should be you need to see if you are at an odd or even menu page and an odd or even cursor position.
  if (menuPage % 2 == 0) {
    if (cursorPosition % 2 == 0) {  // If the menu page is even and the cursor position is even that means the cursor should be on line 1
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
    }
    if (cursorPosition % 2 != 0) {  // If the menu page is even and the cursor position is odd that means the cursor should be on line 2
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
    }
  }
  if (menuPage % 2 != 0) {
    if (cursorPosition % 2 == 0) {  // If the menu page is odd and the cursor position is even that means the cursor should be on line 2
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
    }
    if (cursorPosition % 2 != 0) {  // If the menu page is odd and the cursor position is odd that means the cursor should be on line 1
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
    }
  }
}


void operateMainMenu() {
  int activeButton = 0;
  motorOff();
  while (activeButton == 0) {
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
      case 1:  // This case will execute if the "forward" button is pressed
        button = 0;
        Serial.println(cursorPosition);
        switch (cursorPosition) { // The case that is selected here is dependent on which menu page you are on and where the cursor is.
           case 0:
            menuItem1();
            break;
          case 1:
            menuItem2();
            break;
          case 2:
            menuItem3();
            break;
          case 3:
            menuItem4();
            break;
          case 4:
            menuItem5();
            break;
          case 5:
            menuItem6();
            break;
        }
        activeButton = 1;
        mainMenuDraw();
        drawCursor();
        break;
      case 2:
        button = 0;
        if (menuPage == 0) {
          cursorPosition = cursorPosition - 1;
          cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));
        }
        if (menuPage % 2 == 0 and cursorPosition % 2 == 0) {
          menuPage = menuPage - 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        if (menuPage % 2 != 0 and cursorPosition % 2 != 0) {
          menuPage = menuPage - 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        cursorPosition = cursorPosition - 1;
        cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));

        mainMenuDraw();
        drawCursor();
        activeButton = 1;
        break;
      case 3:
        button = 0;
        if (menuPage % 2 == 0 and cursorPosition % 2 != 0) {
          menuPage = menuPage + 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        if (menuPage % 2 != 0 and cursorPosition % 2 == 0) {
          menuPage = menuPage + 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        cursorPosition = cursorPosition + 1;
        cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));
        mainMenuDraw();
        drawCursor();
        activeButton = 1;
        break;
    }
  }
}

// This function is called whenever a button press is evaluated. The LCD shield works by observing a voltage drop across the buttons all hooked up to A0.
int evaluateButton(int x) {
  /* original mapping:
  1 right
  2 up
  3 down
  4 left
  5 select
  */
  int result = 0;
  if (x < 50) {
    result = 5; // right
  } else if (x < 195) {
    result = 2; // up
  } else if (x < 380) {
    result = 3; // down
  } else if (x < 555) {
    result = 1; // left
  } else if (x < 790) {
    result = 4; // Select
  }
  return result;
}

// If there are common usage instructions on more than 1 of your menu items you can call this function from the sub
// menus to make things a little more simplified. If you don't have common instructions or verbage on multiple menus
// I would just delete this void. You must also delete the drawInstructions()function calls from your sub menu functions.
void drawInstructions() {
  lcd.setCursor(0, 1); // Set cursor to the bottom line
  lcd.print("Use ");
  lcd.write(byte(1)); // Up arrow
  lcd.print("/");
  lcd.write(byte(2)); // Down arrow
  lcd.print(" buttons");
}

void menuItem1() { // Function executes when you select the 2nd item from main menu
  int activeButton = 0;

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Set Volume");
  lcd.setCursor(3,1);
  lcd.print(Volume);
  lcd.print(" nl");

  while (activeButton == 0) {
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // This case will execute if the "back" button is pressed
        // This is a good moment to write the EEPROM:
        if (Volume<=50) EEPROM.write(0,Volume/5);
        else if (Volume<=250) EEPROM.write(0,Volume/10+5);
        else EEPROM.write(0,Volume/50+25);
        button = 0;
        activeButton = 1;
        break;
      case 2:  // UP
        if(Volume>=0){
          VolumeIncr = 5;
          if(Volume>=50){
            VolumeIncr = 10;
            if(Volume>=250){
              VolumeIncr = 50;
              if(Volume>=1000){
              VolumeIncr = 0;
              }
            }
          }  
        }
        Volume+=VolumeIncr;
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("Set Volume");
        lcd.setCursor(3,1);
        lcd.print(Volume);
        lcd.print(" nl");
        break;
      case 3: // DOWN
        if(Volume<=1000){
          VolumeIncr=50;
          if(Volume<=250){
            VolumeIncr=10;
            if(Volume<=50){
              VolumeIncr=5;
              if(Volume<=0){
                VolumeIncr=0;
              }
            }
          }
        }
        Volume-=VolumeIncr;      
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("Set Volume");
        lcd.setCursor(3,1);
        lcd.print(Volume);
        lcd.print(" nl");
        break;
    }
  }
}

void menuItem2() { // Function executes when you select the 2nd item from main menu
  int activeButton = 0;

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Set Time");
  lcd.setCursor(3,1);
  lcd.print(Time);
  lcd.print(" sec");

  while (activeButton == 0) {
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // This case will execute if the "back" button is pressed
        // This is a good moment to write the EEPROM:
        if (Time<=50) EEPROM.write(1,Time/5);
        else if (Time<=250) EEPROM.write(1,Time/10+5);
        else EEPROM.write(1,Time/50+25);
        button = 0;
        activeButton = 1;
        break;
      case 2:  // UP
        if(Time>=0){
          TimeIncr = 5;
          if(Time>=50){
            TimeIncr = 10;
            if(Time>=250){
              TimeIncr = 50;
              if(Time>=1200){
              TimeIncr = 0;
              }
            }
          }  
        }
        Time+=TimeIncr;
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("Set Time");
        lcd.setCursor(3,1);
        lcd.print(Time);
        lcd.print(" sec");
        break;
      case 3: // DOWN
        if(Time<=1200){
          TimeIncr=50;
          if(Time<=250){
            TimeIncr=10;
            if(Time<=50){
              TimeIncr=5;
              if(Time<=0){
                TimeIncr=0;
              }
            }
          }
        }
        Time-=TimeIncr;      
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("Set Time");
        lcd.setCursor(3,1);
        lcd.print(Time);
        lcd.print(" sec");
        break;
    }
  }
}

void menuItem3() { // Function executes when you select the 3rd item from main menu
  int activeButton = 0;

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(Volume);
  lcd.print("nl");
  lcd.setCursor(1,1);
  lcd.print(Time);
  lcd.print("s");
  lcd.setCursor(12,0);
  lcd.print(" yes ");
  lcd.setCursor(12,1);
  lcd.print(">NO");
  choice = 0;

  while (activeButton == 0) {
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // This case will execute if the "back" button is pressed
        button = 0;
        activeButton = 1;
        break;
      case 2: // UP
        lcd.setCursor(12,0);
        lcd.print(">YES");
        lcd.setCursor(12,1);
        lcd.print(" no");
        choice = 1;
      break;
      case 3: // DOWN
        lcd.setCursor(12,0);
        lcd.print(" yes");
        lcd.setCursor(12,1);
        lcd.print(">NO");
        choice = 0;
      break;
      case 5:  // Select
        if (choice==1){
          // DO INJECTION
          lcd.clear();
          lcd.setCursor(0,1);
          lcd.print("INJECTING");
          // Now we need to run the injection with the parameters provided in the settings:
          DoInjection(Time, Volume);
          return;
          break;
        }
        else {
          lcd.clear();
          button = 0;
          activeButton = 1;
          break;
        }
      break;
    }
  }
}

void menuItem4() { // Function executes when you select the 4th item from main menu
  int activeButton = 0;
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Manual Move");
  lcd.setCursor(0,1);
  lcd.print("Use +/- to move");

  while (activeButton == 0) {
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // This case will execute if the "back" button is pressed
        button = 0;
        activeButton = 1;
        break;
      case 2:  // UP
        while (button==2){
          CW(wait);
          readKey = analogRead(0);
          button = evaluateButton(readKey);
          revs++;
          if(revs>20){
            wait = 30;
            if(revs>100){
              wait = 15;
            }
          }
        }
        revs = 0;
        wait = 50;
        break;
      case 3:  // DOWN
        while(button==3){
          CCW(wait);
          readKey = analogRead(0);
          button = evaluateButton(readKey);
          revs++;
          if(revs>20){
            wait = 30;
            if(revs>100){
              wait = 15;
            }
          }
        }
        revs = 0;
        wait = 50;
        break;  
    }
  }
}

void menuItem5() { // Function executes when you select the 5th item from main menu
  int activeButton = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Redo Calibr?");
  lcd.setCursor(0,1);
  lcd.print("slope:  ");
  lcd.print(CalibrationSlope);
  lcd.setCursor(12,0);
  lcd.print(" yes ");
  lcd.setCursor(12,1);
  lcd.print(">NO");
  choice = 0;
  while (activeButton == 0) {
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // This case will execute if the "back" button is pressed
        button = 0;
        activeButton = 1;
        break;
      case 2: // UP
        lcd.setCursor(12,0);
        lcd.print(">YES");
        lcd.setCursor(12,1);
        lcd.print(" no ");
        choice = 1;
      break;
      case 3: // DOWN
        lcd.setCursor(12,0);
        lcd.print(" yes ");
        lcd.setCursor(12,1);
        lcd.print(">NO");
        choice = 0;
      break;
      case 5:  // Select
        if (choice==1){
          // DO INJECTION
          lcd.clear();
          lcd.setCursor(0,1);
          lcd.print("Calibrating");
          delay(1000);
          // We need to make sure some things are reset:
          sum_xy = 0;
          sum_xx = 0;
          avgVolumes = 0;
          Rres = 0;
          Rtot = 0;
          DoCalibration();
          Serial.println(CalibrationSlope);
          Save(CalibrationSlope);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Slope: ");
          lcd.setCursor(0,1);
          lcd.print("Rsq: ");
          lcd.setCursor(8,0);
          lcd.print(CalibrationSlope);
          lcd.setCursor(8,1);
          lcd.print(Rsquare);
          delay(5000);

          // We now want to go back to the main menu
          return;
        }
        else {
          lcd.clear();
          button = 0;
          activeButton = 1;
          lcd.clear();
          lcd.setCursor(2,0);
          lcd.print("Calibration");
          lcd.setCursor(2,1);
          lcd.print("not changed");
          delay(2000);
          break;
        }
    }
  }
}

void menuItem6() { // Function executes when you select the 5th item from main menu
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Plunger position");
  lcd.setCursor(3,1);
  lcd.print(MotorPosition);
  delay(2000);
}

void CalibrationSub(int stepnr){    // This menu will take care of the different calibration steps.
// We provide several dispenses and ask for the diameter of the liquid under a microscope.
  int activeButton = 0;
  int done = 0;
  
  lcd.clear();
  lcd.print("Fill in diameter");
  lcd.setCursor(6,1);
  lcd.print("um");
  lcd.setCursor(0,1);
  lcd.print(Diameter);
  while (activeButton == 0 && done == 0) {
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // This case will execute if the "back" button is pressed
      break;
      case 2:  // UP
        
        Diameter+=1;
        lcd.clear();
        lcd.print("Fill in diameter");
        lcd.setCursor(6,1);
        lcd.print("um");
        lcd.setCursor(0,1);
        lcd.print(Diameter);
        break;
      case 3: // DOWN
        Diameter-=1;  
        lcd.clear();
        lcd.print("Fill in diameter");
        lcd.setCursor(6,1);
        lcd.print("um");
        lcd.setCursor(0,1);
        lcd.print(Diameter);
        break;
      case 5: //  Select
        // Save and move on to the next calibration level.
        // Calculate the volume based on the diameter:
        // 4/3*pi()*r^3
        CalibrationVolumes[stepnr] = 4.188790*pow((Diameter/2),3);
        // This is now defined in cubic micrometers! This is not a nl!
        // We need to divide by 1e6!
        CalibrationVolumes[stepnr] /=1000000;
        // Display that the value is saved and move on:
        done = 1;
        lcd.clear();  
        lcd.setCursor(5,0);
        lcd.print("Volume is:");
        lcd.setCursor(5,1);
        lcd.print(CalibrationVolumes[stepnr]);
        lcd.print(" nl");
        delay(1000);
        break;
        return;
    }

  }
  
  
}
