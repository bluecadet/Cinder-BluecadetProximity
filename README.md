# Cinder-BluecadetProximity

Cinder block to provide proximity sensor data. Currently supports MaxBotix HRUSB-MaxSonar-EZ1.

Always use Proximity::getInstance() to get the singleton and call member functions of it. 

Use start() to establish the serial connection, and stop() to disconnect. When connected, the Cinder block will update the device list and read sensor data before every update cycle of the Cinder app.

getAllDevices() returns all the devices that has ever been connected at runtime, including those disconnected currently. getDevice(const std::string &path) returns device by path. It will throw exceptions on unmatched path. hasDevice(const std::string &path) checks whether a device exists.

Hot plug of the devices is supported. Tested scenrios include adding/removing devices, reconnect a pre-existed device to the same COM port or a new port. It is recommended that stop() is called before the hot plugging operations (and start() after the operations). For now, (re)connecting to a device causes a few seconds of penalty of the main thread.

A sample app has been implemented to test the basic functions of the API and hot plugging robustness.