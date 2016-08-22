#include "Proximity.h"
#include "cinder/Log.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace bluecadet {
namespace proximity {

const double Proximity::UPDATE_INTERVAL = 0;

ProximityRef Proximity::getInstance() {
	static ProximityRef instance = nullptr;
	if (!instance) instance = ProximityRef(new Proximity());
	return instance;
}

Proximity::Proximity() {
}

Proximity::~Proximity() {
	if (mSignalsConnection.isConnected()) {
		mSignalsConnection.disconnect();
	}
}

void Proximity::start() {
	if (!mSignalsConnection.isConnected()) {
		mSignalsConnection = AppBase::get()->getSignalUpdate().connect(std::bind(&Proximity::updateData, this));
	} else if (!mSignalsConnection.isEnabled()) {
		mSignalsConnection.enable();
	}
}

void Proximity::stop() {
	if (mSignalsConnection.isConnected() && mSignalsConnection.isEnabled()) {
		mSignalsConnection.disable();
	}
}

const Proximity::Device& Proximity::getDevice(const string & sn) const {
	return mDevices.at(sn);
}

const map<string, Proximity::Device>& Proximity::getAllDevices() const {
	return mDevices;
}

void Proximity::updateDeviceList() {
	// Mark all previous existing devices as DISCONNECTED.
	for (auto &storedDevice : mDevices) {
		storedDevice.second.status = Device::Status::DISCONNECTED;
	}
	
	// Check all the currently connected devices:
	for (const auto &connectedDevice : Serial::getDevices()) {
		auto storedDeviceMatch = mDevices.find(connectedDevice.getPath());
		if (storedDeviceMatch != mDevices.end()) { // It's a previous existing device. Update it's stuatus.
			storedDeviceMatch->second.status = Device::Status::CONNECTED;
			if (storedDeviceMatch->second.name != connectedDevice.getName()) { // The serial port (represented by device name) has changed. Rebuild serial connection.
				try {
					storedDeviceMatch->second.serial = Serial::create(connectedDevice, 57600);
				} catch (SerialExc &exception) {
					CI_LOG_EXCEPTION("could not initialize the serial device", exception);
					exit(-1);
				}
			}
		} else { // It's a new device. Add it to mDevices.
			Device newDevice;
			try {
				newDevice.serial = Serial::create(connectedDevice, 57600);
			} catch (SerialExc &exception) {
				CI_LOG_EXCEPTION("could not initialize the serial device", exception);
				exit(-1);
			}
			newDevice.name = connectedDevice.getName();
			newDevice.path = connectedDevice.getPath();
			newDevice.status = Device::Status::CONNECTED;
			mDevices.emplace(newDevice.path, newDevice);
		}
	}

	// Clean up the serial connection of disconnected devices:
	for (auto &storedDevice : mDevices) {
		if (storedDevice.second.status == Device::Status::DISCONNECTED) {
			storedDevice.second.serial = nullptr;
		}
	}
}

void Proximity::updateData() {
	// Only update once per UPDATE_INTERVAL:
	double currentTime = getElapsedSeconds();
	if (currentTime - lastUpdateTime < UPDATE_INTERVAL) {
		return;
	}
	lastUpdateTime = currentTime; // If going to update the data, update the time stamp first.

	// For now we update device list every time when updating data:
	updateDeviceList();

	// For all connected devices, read data from serial connection:
	for (auto &storedDevice : mDevices) {
		try {
			string reading = storedDevice.second.serial->readStringUntil('\n');
			storedDevice.second.serial->flush();

			if (reading.length() == 5) {
				storedDevice.second.distance = atoi(reading.substr(1).c_str());
			}
		} catch (SerialTimeoutExc &exception) {
			CI_LOG_EXCEPTION("timeout when reading from serial", exception);
		}
	}
}

} // namespace proximity
} // namespace bluecadet