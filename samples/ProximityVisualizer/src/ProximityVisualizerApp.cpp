#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "Proximity.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace bluecadet::proximity;

class ProximityVisualizerApp : public App {
public:
	void setup() override;
	void cleanup() override;
	void mouseDown(MouseEvent event) override;
	void keyDown(KeyEvent event) override;
	void update() override;
	void draw() override;

	bool connected = false;
	void startConnection();
	void stopConnection();
};

void ProximityVisualizerApp::setup() {
	startConnection();
}

void ProximityVisualizerApp::cleanup() {
	stopConnection();
}

void ProximityVisualizerApp::mouseDown(MouseEvent event) {
}

void ProximityVisualizerApp::keyDown(KeyEvent event) {
	switch (event.getCode()) {
	case KeyEvent::KEY_SPACE:
		if (connected) {
			stopConnection();
		} else {
			startConnection();
		}
	}
}

void ProximityVisualizerApp::update() {
	if (connected) {
		auto &devices = Proximity::getInstance()->getAllDevices();
		console() << "-------------------------------" << endl << endl;
		for (auto &device : devices) {
			console() << device.second.name << endl;
			console() << device.second.path << endl;
			console() << (device.second.status == Proximity::Device::Status::CONNECTED ? "CONNECTED" : "DISCONNECTED") << endl;
			console() << device.second.distance << endl;
			console() << endl;
		}
	}
}

void ProximityVisualizerApp::draw() {
	gl::clear(Color(0, 0, 0));
}

void ProximityVisualizerApp::startConnection() {
	if (connected == false) {
		Proximity::getInstance()->start();
		console() << "Serial connection started." << endl << endl;
		connected = true;
	}
}

void ProximityVisualizerApp::stopConnection() {
	if (connected == true) {
		Proximity::getInstance()->stop();
		console() << "Serial connection stopped." << endl << endl;
		connected = false;
	}
}

CINDER_APP(ProximityVisualizerApp, RendererGl)
