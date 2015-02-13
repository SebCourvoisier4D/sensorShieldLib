#include "Arduino.h"
#include "HardwareSerial.h"
#include "sensorShieldLib.h"

SensorShield::SensorShield(){}

/////////////////////////////////////////////////////////////////////
void SensorShield::init()
{
	// default SensorShieldStream to Serial, 9600 baudrate
	Serial.begin( 9600 );
	init( Serial );
}

void SensorShield::init( Stream &stream )
{
	SensorShieldStream = &stream;
	initValues();
}

void SensorShield::initValues()
{
	// default values for Arduino UNO
	digitalPinMin = 2;
	digitalPinMax = 13;
	analogPinMin = A0;
	analogPinMax = A5;
	nbSensors = 0;
	analogSensitivity = 1;
	analogMin = 0;
	analogMax = 1023;
	indicatorLedPin = NULL;
	turnLightOn = false;
}

/////////////////////////////////////////////////////////////////////
void SensorShield::setDigitalPinsRange( int pinMin, int pinMax )
{
	digitalPinMin = pinMin;
	digitalPinMax = pinMax;
}

void SensorShield::setAnalogPinsRange( int pinMin, int pinMax )
{
	analogPinMin = pinMin;
	analogPinMax = pinMax;
}

/////////////////////////////////////////////////////////////////////
void SensorShield::addSensor( String sensorID, int pin )
{
	addSensor( sensorID, pin, INPUT );
}

void SensorShield::addSensor( String sensorID, int pin, int pinmode )
{
	if( ( pin >= digitalPinMin && pin <= digitalPinMax ) 
		|| ( pin >= analogPinMin && pin <= analogPinMax ) ) {

		sensors[ nbSensors ].sensorID = sensorID;
		sensors[ nbSensors ].pin = pin;
		sensors[ nbSensors ].value = 0;

		if( pin >= digitalPinMin && pin <= digitalPinMax ) {
			addDigitalSensor( pin, pinmode );
		}
		else {
			addAnalogSensor();
		}

		nbSensors++;
	} 
}

void SensorShield::addDigitalSensor( int pin, int pinmode )
{
	sensors[ nbSensors ].isDigital = true;

	if( pinmode == INPUT_PULLUP) {
		sensors[ nbSensors ].isInputPullUp = true;
	}
	else {	
		sensors[ nbSensors ].isInputPullUp = false;
	}

	pinMode( pin, pinmode );
}

void SensorShield::addAnalogSensor()
{
	sensors[ nbSensors ].isDigital = false;
	sensors[ nbSensors ].isInputPullUp = false;
	sensors[ nbSensors ].analogSensitivity = analogSensitivity;
	sensors[ nbSensors ].min = analogMin;
	sensors[ nbSensors ].max = analogMax;
}

/////////////////////////////////////////////////////////////////////
void SensorShield::setAnalogSensitivity( int sensitivity )
{
	analogSensitivity = sensitivity;
	for (int i = 0; i < nbSensors; ++i) {
		if( sensors[ i ].isDigital == false ) {
			sensors[ i ].analogSensitivity = sensitivity;
		}
	}
}

void SensorShield::setAnalogSensitivity( String sensorID, int sensitivity )
{
	for (int i = 0; i < nbSensors; ++i) {
		if( sensors[ i ].sensorID == sensorID ) {
			sensors[ i ].analogSensitivity = sensitivity;
			return;
		}
	}
}

/////////////////////////////////////////////////////////////////////
void SensorShield::setAnalogLimits( int min, int max )
{
	analogMin = min;
	analogMax = max;
	for (int i = 0; i < nbSensors; ++i) {
		if( sensors[ i ].isDigital == false ) {
			sensors[ i ].min = min;
			sensors[ i ].max = max;
		}
	}
}

void SensorShield::setAnalogLimits( String sensorID, int min, int max )
{
	for (int i = 0; i < nbSensors; ++i) {
		if( sensors[ i ].sensorID == sensorID ) {
			sensors[ i ].min = min;
			sensors[ i ].max = max;
			return;
		}
	}
}

/////////////////////////////////////////////////////////////////////
void SensorShield::emitLightOnChange( int ledPin )
{
	indicatorLedPin = ledPin;
	turnLightOn = true;
	lightup();
}

void SensorShield::emitLightOnChange( bool _turnLightOn )
{
	turnLightOn = _turnLightOn;	
}

void SensorShield::lightup()
{
	digitalWrite( indicatorLedPin, HIGH );
	delay( 50 );
	digitalWrite( indicatorLedPin, LOW );
}

/////////////////////////////////////////////////////////////////////
void SensorShield::update()
{
	hasNewValue = false;
	int tmpValue;

	for (int i = 0; i < nbSensors; ++i) {
		if( sensors[ i ].isDigital == true ) {
			if( sensors[ i ].isInputPullUp == true ) {
				tmpValue = 1 - digitalRead( sensors[ i ].pin );
			}
			else {	
				tmpValue = digitalRead( sensors[ i ].pin );
			}

			if( sensors[ i ].value != tmpValue ) {
				sensors[ i ].value = tmpValue;
				hasNewValue = true;
			}
		}
		else {
			tmpValue = analogRead( sensors[ i ].pin );
			if( abs( sensors[ i ].value - tmpValue ) > sensors[ i ].analogSensitivity ) {
				sensors[ i ].value = tmpValue;
				
				if( sensors[ i ].value >= sensors[ i ].min && sensors[ i ].value <= sensors[ i ].max ) {
					hasNewValue = true;
				}
			}
		}
	}
	
	if( hasNewValue ) {
		sendJSON();
	}
}

void SensorShield::createJSON()
{
	JSONMessage = "{";
	for ( int i = 0; i < nbSensors; ++i ) {
		if( i != 0) JSONMessage += ",";
		JSONMessage += "\"" + sensors[ i ].sensorID + "\":" + sensors[ i ].value;
	}
	JSONMessage += "}";
}

void SensorShield::sendJSON()
{
	createJSON();

	SensorShieldStream->println( JSONMessage );

	if( turnLightOn && indicatorLedPin != NULL) lightup();
}