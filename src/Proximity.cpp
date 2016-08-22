#include "cinder/Serial.h"
#include "Proximity.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace bluecadet {
namespace proximity {

Proximity::Proximity()
{
}

Proximity::~Proximity()
{
}

void Proximity::setup()
{
	AppBase::get()->getSignalUpdate().connect(std::bind(&Proximity::update, this));
}

void Proximity::update()
{
	// device connected
	const Device newDevice;
	mDeviceConncted.emit(newDevice);
}

} // namespace proximity
} // namespace bluecadet