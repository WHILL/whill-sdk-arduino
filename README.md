# whill-sdk-arduino
A simple WHILL Model CR Library for Arduino.

<img src="https://user-images.githubusercontent.com/2618822/45492944-89421c00-b7a8-11e8-9c92-22aa3f28f6e4.png" width=30%>


## Requirements
- WHILL **Model CR**  (Normal **Model C** does not support serial communication.)
- An Arduino device suports SoftwareSerial. e.g. Arduino Uno.
- RS232C Shield  (Recommended:https://www.seeedstudio.com/RS232-Shield-p-1910.html)



## Getting Started
Download this repository and place on your Arduino library folder.

## APIs

### Communication

```cpp
WHILL(SoftwareSerial*)
```
Initialize WHILL instance with SoftwareSerial.

```cpp
WHILL::begin(unsigned int interval_ms)
```
Command WHILL to start reporting WHILL status.

```cpp
WHILL::refresh()
```
Fetch serial interface and do internal process.
Call this function , `WHILL::delay()` and/or `WHILL::keep_joy_delay()` once at least in `loop()`. Callbacks are fired when these functions are called.

```cpp
stopSendingData()
```
Command WHILL to stop report WHILL status.


### Manipulation

```cpp
WHILL::setJoystick(int x,int y)
```
Manipulate a WHILL via this command.


```cpp
WHILL::setPower(bool)
```
Turn on/off a WHILL. `True` to power WHILL on. 

```cpp
WHILL::setBatteryVoltaegeOut(bool)
```
Enable/Disable power supply to the interface connector. `True` to enable power supply.


### Sensors and Status

### Accelerometer
```cpp
int WHILL::accelerometer.x
int WHILL::accelerometer.y
int WHILL::accelerometer.z
```
Accelerometer mounted on body.

#### Gyro
```cpp
int WHILL::gyro.x
int WHILL::gyro.y
int WHILL::gyro.z
```
Gyro sensor mounted on body.


#### Battery
```cpp
unsigned char  WHILL::battery.level
signed   long  WHILL::battery.current
```
Remaining battery level and consumpting current[mA].


#### Motor State
```cpp
float   WHILL::left_motor.angle
int     WHILL::left_motor.speed

float   WHILL::right_motor.angle
int     WHILL::right_motor.speed
```
Motors angle and speed. The angle range is -PI to +PI, Speed unit is km/h.  
**Note that the speed value is low-pass filterd.**

#### Speed Mode
```cpp
bool   WHILL::speed_mode_indicator
```
Current selected speed mode.

### Callback

By registering callback functions, You can hook at status is updated.  
See Example: [callback.ino](https://github.com/WHILL/whill-sdk-arduino/blob/master/examples/callback/callback.ino)
