#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/params/Params.h"
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

	bool mConnected = false;
	void startConnection();
	void stopConnection();
	void toggleConnection();

	params::InterfaceGlRef mParams;
};

void ProximityVisualizerApp::setup() {

	mParams = params::InterfaceGl::create(getWindow(), "App Controls", toPixels(ivec2(200, 300)));
	mParams->addButton("Toggle Connection", bind(&ProximityVisualizerApp::toggleConnection, this));
	mParams->addText("connectionStatus", "label=`Serial connection: OFF`");

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
		if (mConnected) {
			stopConnection();
		} else {
			startConnection();
		}
	}
}

void ProximityVisualizerApp::update() {
	if (mConnected) {
		auto &devices = Proximity::getInstance()->getAllDevices();
		console() << "-------------------------------" << endl << endl;
		for (auto &device : devices) {
			console() << device.second.name << endl;
			console() << device.second.path << endl;
			console() << (device.second.status == Proximity::Device::Status::CONNECTED ? "CONNECTED" : "DISCONNECTED") << endl;
			console() << device.second.distance << endl;
			console() << endl;
		}

		mParams->setOptions("connectionStatus", "label=`Serial connection: ON`");
	} else {
		mParams->setOptions("connectionStatus", "label=`Serial connection: OFF`");
	}

	//console() << Proximity::getInstance()->hasDevice("\\\\?\\ftdibus#vid_0403+pid_6001+mb9xk7ya#0000#{4d36e978-e325-11ce-bfc1-08002be10318}") << endl; // Make sure to escape the \s!
}

void ProximityVisualizerApp::draw() {
	gl::clear(Color(0, 0, 0));
	mParams->draw();
}

void ProximityVisualizerApp::startConnection() {
	if (mConnected == false) {
		Proximity::getInstance()->start();
		console() << "Serial connection started." << endl << endl;
		mConnected = true;
	}
}

void ProximityVisualizerApp::stopConnection() {
	if (mConnected == true) {
		Proximity::getInstance()->stop();
		console() << "Serial connection stopped." << endl << endl;
		mConnected = false;
	}
}

void ProximityVisualizerApp::toggleConnection() {
	if (mConnected == true) {
		stopConnection();
	} else {
		startConnection();
	}
}

CINDER_APP(ProximityVisualizerApp, RendererGl)
