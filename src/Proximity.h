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

	const Device& getDevice(const std::string &sn) const;
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