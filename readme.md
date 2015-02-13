#SensorShieldLib  
A simple library for your custom sensorShields
___

![SensorShield](SensorShield.jpg)

This lib tends to communicate sensors values from your board in JSON format.  
___
###Features
- a SensorShield class: `SensorShield board;`
- simple initialisation: `board.init();`
- support initialisation with different Stream: `Serial.begin( 115200 ); board.init( Serial );`
- set your digital/analog pins ranges ( lib is configured by default for UNO ): `board.setDigitalPinsRange( 5, 10 );`
- easy sensor attach with auto recognition for analog/digital read: `board.addSensor( "btn1", 2 ); board.addSensor( "pot1", A0 );`
- a SensorShield class: `SensorShield board;`
- simple initialisation: `board.init();`
- support initialisation with different Stream: `Serial.begin( 115200 ); board.init( Serial );`
- set your digital/analog pins ranges ( lib is configured by default for UNO ): `board.setDigitalPinsRange( 5, 10 );`
- easy sensor attach with auto recognition for analog/digital read: `board.addSensor( "btn1", 2 ); board.addSensor( "pot1", A0 );`
- support INPUT\_PULLUP pinMode for digital sensors: `board.addSensor( "btn2", 8, INPUT\_PULLUP );`
- set analog sensitivity (ie minimun change on analog captor for sending JSON) for all analog sensors: `board.setAnalogSensititvity(5);`
- or set analog sensitivity for a particular analog sensor: `board.setAnalogSensitivity( "pot1", 10 );`
- set limit values of interest for all analog sensors: `board.setAnalogLimits( 150, 850 );`
- or for a particular one: `board.setAnalogLimits( "pot1", 150, 850 );`
- reads all digital/analog sensors on update and automatically send JSON on changes: `board.update();`
- possibility to add a visual signal when sending JSON with a led: `board.emitlightOnChange( 13 );`
- public access to boolean hasNewValue: `if( board.hasNewValue == true ) { ...... }`
- public access to String JSONMessage: `String s = board.JSONMessage;`

___
###Example:

![Example](examples/SensorShield101/SensorShieldLib.png)

```
#include <sensorShieldLib.h>

SensorShield board;

void setup()
{
	board.init(); // always needed first, initialises some values and start Serial with 9600 baudrate
	
	// you can set digital/analog pins ranges if working with a board different from Arduino UNO configuration
	// example with arduino MEGA:
	// board.setDigitalPinsRange( 2, 53 ); //digital pins 0 & 1 are required for Serial Communication
	// board.setAnalogPinsRange( A0, A15 );
 	
 	// add analog/digital sensors, 
	board.addSensor( "btn1", 2 );
	board.addSensor( "btn2", 8, INPUT_PULLUP );
	board.addSensor( "pot1", A0 );
	
	// set minimal change on analog sensors 
	board.setAnalogSensitivity( 10 );
	
	// connect a led that will lightup when sending JSON
	board.emitLightOnChange( 13 );
}

void loop()
{
	board.update(); 
	// checks if any sensor changed 
	// if so, sends a JSON with all sensors and their value:
	// {"btn1":1,"btn2":0,"pot1":768}
}
```

___
###Coming soon
- example with ethernetShield