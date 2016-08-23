#pragma once
#include "cinder/app/App.h"
#include "cinder/Serial.h"

namespace bluecadet {
namespace proximity {

typedef std::shared_ptr<class Proximity> ProximityRef;

class Proximity {
public:
	struct Device {
		cinder::SerialRef serial;
		std::string name;
		std::string path;
		enum class Status {
			CONNECTED,
			DISCONNECTED
		} status;
		int distance;
	};

	static ProximityRef getInstance();

	Proximity();
	~Proximity();

	void start();
	void stop();

	bool hasDevice(const std::string &path) const;
	//! Call hasDevice() first! Unmatched path in getDevice() results in exceptions.
	const Device& getDevice(const std::string &path) const;
	const std::map<std::string, Device>& getAllDevices() const;

private:
	std::map<std::string, Device> mDevices;
	cinder::signals::Connection mSignalsConnection;

	static const double UPDATE_INTERVAL;
	double lastUpdateTime = -UPDATE_INTERVAL;

	void updateDeviceList();
	void updateData();
}; // class Proximity

} // namespace proximity
} // namespace bluecadet