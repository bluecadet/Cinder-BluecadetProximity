#pragma once
#include "cinder/app/App.h"

namespace bluecadet {
namespace proximity {

typedef std::shared_ptr<class Proximity> ProximityRef;

class Proximity {
public:
	struct Device {
		std::string vid;
		std::string pid;
		std::string serial;
		enum class Status {
			CONNECTED,
			DISCONNECTED
		} status;
		int distance;
		//const int minDistance = 300;
		//const int maxDistance = 5000;
		//bool hasDetectedObject();
	};

	static ProximityRef getInstance() {
		static ProximityRef instance = nullptr;
		if (!instance) instance = ProximityRef(new Proximity());
		return instance;
	}

	Proximity();
	~Proximity();

	void start();
	void stop();

	size_t getNumDevices() const { return mDevices.size(); };
	const Device& getDevice(const std::string &serial) const;
	const vector<Device>& getAllDevices() const;

private:
	cinder::signals::Signal<void(const Device&)> mDeviceConncted;
	vector<Device> mDevices;
	void updateDeviceList();
	void updateData();
}; // class Proximity

} // namespace proximity
} // namespace bluecadet