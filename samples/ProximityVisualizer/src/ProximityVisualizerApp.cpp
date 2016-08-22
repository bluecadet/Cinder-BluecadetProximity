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
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void ProximityVisualizerApp::setup()
{
}

void ProximityVisualizerApp::mouseDown( MouseEvent event )
{
}

void ProximityVisualizerApp::update()
{
}

void ProximityVisualizerApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( ProximityVisualizerApp, RendererGl )
