# Arduino_Nanoject
Using an Arduino Uno with motor and lcd shield to control a nanoject injector from Drummond.<br><br>
<b> WARNING: THIS PROJECT WORKED FOR US. I WAS CAREFUL AND CHECKED VOLTAGES ALL THE TIME TO NOT DAMAGE THE NANOJECT INJECTER. I ADVISE YOU TO DO THE SAME. I TAKE NO RESPONSIBILITY WHAT SO EVER FOR DAMAGE DONE TO YOUR EQUIPMENT.</B>

When the control box of our nanoject 2 died I decided to make an alternative controler from an arduino I had lying around. A motor and LCD shield were added a program was written over two weekends. If you have a nanoject lying around, this is a simple way to get it back to life.<br>
<br>
<b>You will need:</b><br>
* Nanoject injector (we used a nanoject 2, but it should work with other injectors as well, just check the wiring!)<br>
* Arduino Uno
* Velleman VMA03 Motor shield
* Velleman VMA203 LCD shield
* Small replacement capacitors for the motor shield, we used Cornell Dubilier HZC and HZF series. 100 and 22 uF.
* 9V adapter brick
* RJ22 plug to plug in your nanoject injector (4p4c, so four wires, narrow plug!)
<br>
* For the calibration you will need to put the nanoject under a microscope with some way of measuring the sizes of the droplets you are going to produce from the nanoject. We used a Narishige micropipette forge, which has a microscope micrometer in one of the eye pieces.
<br>
<b>Construction:</b><br>
Replace the 100uF and 22uF capacitor on the motor shield for lower ones. This ensures that you can properly fit the lcd shield on top.<br>
Set the jumpers on the motor shield as follows:<br>
* DIRB: 12 <br>
* PWMB: 11 <br>
* DIRA: 2 <br>
* PWMA: 3 <br>
(Selecting these pins ensures that your motor and shield do not use the same pins.)<br>
Set the jumer for the power of the motor to internal.<br>
Plug the motor shield onto the arduino.<br>
Plug the lcd shield onto the motor shield<br>
<br>
Solder the RJ22 plug in such an order that the following connections from the nanoject wires are made:<br>
**********<br>
Will be added soon when I have the pictures....<br>
**********<br>
Connect the arduino uno to your computer, download the four .ino files from this project, and upload them to your arduino. You should be able to test the menu functions on your arduino. The whole interface is straightforward:<br>
1. Set volume   - Volume to inject<br>
2. Set time     - Set the time over which to inject<br>
3. Inject!      - Starts the procedure to inject the volume over the given time.<br>
4. Manual move  - Enables you to manually move the plunger up or down.<br>
5. Calibrate    - Use this menu item to set up your nanoject and get the best precision possible.<br>
6. Position     - You can use this menu item to check what the current position of the plunger is. 0 is fully in.<br>
<br>
A casing can be downloaded from https://www.thingiverse.com/thing:1959406. You should adapt the height of the case to your exact needs. Print it with a suitable 3d printer.<br>
Connect the nanoject to the RJ22 plug. Check with the manual move whether the connections are correct. If the nanoject doesn't move, first try swicthing the polarity (switch A and B on the motor board). If that fails you must have some faulty wiring somewhere.<br>
<br>
Now it is time to calibrate your nanoject. Fill a glass needle (as you would normally pull it for your experiments) with saline or water. Put the tip of the needle under a microscope to observe the volume being extruded. The program will extrude several 'steps' of your stepper motor. Carefully measure the diameter and put that into the nanoject control box. When all volumes have been dispensed your nanoject arduino is calibrated.<br>
Check a few volumes under the microscope to make sure you calibrated it correctly and to ensure everything works.<br>
<br>
Done! Happy injecting!<br>
