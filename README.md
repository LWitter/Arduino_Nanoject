# Arduino_Nanoject
Using an Arduino Uno with motor and lcd shield to control a nanoject injector from Drummond.

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

