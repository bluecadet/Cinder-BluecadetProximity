# Cinder-BluecadetProximity

## Overview

Cinder block to provide proximity sensor data. Currently supports *MaxBotix HRUSB-MaxSonar-EZ1*.

The main [`Proximity`](src/Proximity.h) class is a singleton that provides access to supported devices. Each device has a `path` and a `distance` property. Each path is unique to its device and is used to identify individual devices. Distance is measured in *mm*.

## Usage

Use `Proximity::getInstance()->start()` to establish the serial connection, and `Proximity::getInstance()->stop()` to disconnect. When connected, the Cinder block will update the device list and read the sensor data before every update cycle of the Cinder app.

`Proximity::getInstance()->getAllDevices()` returns all the devices that have ever been connected at runtime, including those disconnected currently. `Proximity::getInstance()->getDevice(const std::string &path)` returns devices by path. It will throw exceptions on unmatched path. hasDevice(const std::string &path) checks whether a device exists.

Hot plugging of devices is supported. Tested scenrios include adding/removing devices, reconnect a pre-existed device to the same COM port or a new port. It is recommended that `Proximity::getInstance()->stop()` is called before the hot plugging operations (and `Proximity::getInstance()->start()` after the operations). For now, (re)connecting to a device causes a few seconds of penalty of the main thread.

A sample app has been implemented to test the basic functions of the API and hot plugging robustness.

## Next Steps

* Read device data from worker thread to prevent main thread blocking
* Add support to smooth output signal
