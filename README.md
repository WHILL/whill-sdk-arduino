# whill-sdk-arduino

## About

The "whill-sdk-arduino" is a simple Arduino library for Model CR series. <br>

<img width=22% title="WHILL Model CR2" src="https://github.com/WHILL/Model_CR_Technical_Support/assets/129816934/387a8aac-3808-4727-895d-9857059ee342">
<img width=24% title="Wheeled Robot Base" src="https://github.com/WHILL/Model_CR_Technical_Support/assets/129816934/6a63ed45-9e0c-40ca-b71c-8dff614af141">

We also have [Model CR Series Technical Support](https://github.com/WHILL/Model_CR_Technical_Support) for current and potential Model CR series users. <br>


## Requirements

- Host device
  - An Arduino device suports SoftwareSerial. e.g. Arduino Uno.
- Target device
  - Model CR series (Normal **Model C** or **Model C2** does not support serial communication.)
- Connector
  - RS232C Shield  ([example](https://www.seeedstudio.com/RS232-Shield-p-1910.html))


## Getting Started

Download this repository and place on your Arduino library folder.


## APIs

### Initialize

```cpp
WHILL(SoftwareSerial*)
```
Initialize WHILL instance with SoftwareSerial.


### Power Control

```cpp
WHILL::setPower(bool)
```
Turn on/off a WHILL. `True` to power WHILL on. 

```cpp
WHILL::setBatteryVoltaegeOut(bool)
```
Enable/Disable power supply to the interface connector. `True` to enable power supply. <br>
**Note:** This command is for **Model CR** only.


### Motor Control

```cpp
WHILL::setJoystick(int x, int y)
```
Manipulate a WHILL via this command. Both *x* and *y* are integer values with range -100 ~ 100.

```cpp
WHILL::setVelocity(int y, int x)
```
Control the speed of a WHILL directly via this command. *y* is integer values with range -500 ~ 1500 [0.004km/h]. *x* is integer values with range -750 ~ 750 [0.004km/h]. <br>
**Attention:** WHILL moves so quickly using SetVelocity command and so pay enough attention to use SetVelocity command. Basically, send this command to increase speed gradually.


### Data Fetching

```cpp
WHILL::begin(unsigned int interval_ms)
```
Command WHILL to start reporting WHILL status (Dataset1).

```cpp
WHILL::refresh()
```
Fetch serial interface and do internal process.
Call this function, `WHILL::delay()` and/or `WHILL::keep_joy_delay()` once at least in `loop()`.
Callbacks are fired when these functions are called.

```cpp
WHILL::stopSendingData()
```
Command WHILL to stop report WHILL status.


### Data Reference

#### Battery

```cpp
unsigned char  WHILL::battery.level
signed long    WHILL::battery.current
```
Remaining battery level and consumpting current[mA].


#### Motor State

```cpp
float   WHILL::left_motor.angle
int     WHILL::left_motor.speed
float   WHILL::right_motor.angle
int     WHILL::right_motor.speed
```
Angle and speed of motors. The angle range is -*PI* to +*PI*, the speed unit is [km/h]. <br> 
**Note:** The speed value is low-pass filtered.


#### Speed Mode

```cpp
bool   WHILL::speed_mode_indicator
```
Current selected speed mode.


### Callback

By registering callback functions, you can hook at status is updated. <br>
*See Example:* [callback.ino](https://github.com/WHILL/whill-sdk-arduino/blob/master/examples/callback/callback.ino)


## License

MIT License
