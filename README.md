# whill-sdk-arduino for ESP32

## About

The "whill-sdk-arduino for ESP32" is a simple Arduino library for Model CR series. <br>

<img width=22% title="WHILL Model CR2" src="https://github.com/WHILL/Model_CR_Technical_Support/assets/129816934/387a8aac-3808-4727-895d-9857059ee342">
<img width=24% title="Wheeled Robot Base" src="https://github.com/WHILL/Model_CR_Technical_Support/assets/129816934/6a63ed45-9e0c-40ca-b71c-8dff614af141">

We also have [Model CR Series Technical Support](https://github.com/WHILL/Model_CR_Technical_Support) for current and potential Model CR series users. <br>


## Requirements

- Host device
  - An Arduino device suports HardwareSerial. e.g. ESP32.
- Target device
  - Model CR series (Normal **Model C** or **Model C2** does not support serial communication.)
- Connector
  - RS232C Shield  ([example](https://www.seeedstudio.com/RS232-Shield-p-1910.html))


## Getting Started

Download this repository and place on your Arduino library folder.


## APIs

### Initialize

```cpp
WHILL(HardwareSerial*)
```
Initialize WHILL instance with HardwareSerial.


### Power Control

```cpp
WHILL::powerOn()
```
Turn on a WHILL.

```cpp
WHILL::powerOff()
```
Turn off a WHILL.

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


### Setting Change

```cpp
WHILL::setSpeedProfile(SpeedProfile* profile, unsigned char speed_mode)
```
Change the speed profile of WHILL.

```cpp
WHILL::setBatterySaving(int low_battery_level, bool sounds_buzzer)
```
Configure battery protection settings. low_battery_level is battery charge level to engage the standby mode with range 1 ~ 90. sounds_buzzer is Enable/Disable a buzzing sound at the battery charge level of low_battery_level + 10 percentage points. True to enable a buzzing sound when battery level low. As default, low_battery_level is 19 and sounds_buzzer is True. <br>
**Note:** This command is for **Model CR2** only.


### Data Fetching

```cpp
WHILL::begin(unsigned int interval_ms)
```
Command WHILL to start reporting WHILL status.

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

```cpp
WHILL::updateSpeedProfile()
```
Command WHILL to update speed profile cache. If you want the latest speed profile, send this command.


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


#### Speed Profile

```cpp
unsigned char WHILL::speed_profile[mode].forward_speed
unsigned char WHILL::speed_profile[mode].forward_acceleration
unsigned char WHILL::speed_profile[mode].forward_deceleration
unsigned char WHILL::speed_profile[mode].reverse_speed
unsigned char WHILL::speed_profile[mode].reverse_acceleration
unsigned char WHILL::speed_profile[mode].reverse_deceleration
unsigned char WHILL::speed_profile[mode].turn_speed
unsigned char WHILL::speed_profile[mode].turn_acceleration
unsigned char WHILL::speed_profile[mode].turn_deceleration
```
Current speed profile. `mode` is 0 ~ 5.


### Callback

By registering callback functions, you can hook at status is updated. <br>
*See Example:* [callback.ino](https://github.com/WHILL/whill-sdk-arduino/blob/esp32/examples/callback/callback.ino)


## License

MIT License
