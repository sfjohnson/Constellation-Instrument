#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "ArrowMap.h"
#include "MapParticle.h"
#include "Agent.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ArrowMapApp : public AppBasic
{
    ArrowMap *am;
    MapParticle **particles;
    unsigned int mouseOn;
    Vec2i mousePos;
    bool spaceDown, dDown;
    //osc::Sender oscSender;
    //Timer timer;
    //float t;
    
  public:
    void prepareSettings(Settings *settings);
	void setup();
	void mouseDown(MouseEvent event);
    void mouseDrag(MouseEvent event);
    void mouseUp(MouseEvent event);
    void keyDown(KeyEvent event);
    void keyUp(KeyEvent event);
	void update();
	void draw();
    void shutdown();
};

void ArrowMapApp::prepareSettings(Settings *settings)
{
    settings->setWindowSize(1440, 900);
    settings->setFrameRate(60.0f);
}

void ArrowMapApp::setup()
{
    am = new ArrowMap(getAssetPath("network512.rbn").string());
    particles = new MapParticle*[am->mapSize()];
    
    Rand::randomize();
    for (int i = 0; i < am->mapSize(); i++)
        particles[i] = new MapParticle(Vec2f(Rand::randFloat(getWindowWidth()), Rand::randFloat(getWindowHeight())), 50.0f, 10.0f, 0.001f);
    for (int i = 0; i < am->mapSize(); i++)
        particles[i]->connect(particles[am->getNextState(i)]);
    
    spaceDown = false;
    dDown = false;
    //oscSender.setup("localhost", 1234);
    mousePos = Vec2i(-1, 0);
    mouseOn = 0;
    //timer.start();
    //t = 0.0f;
    
    gl::enableAlphaBlending();
}

void ArrowMapApp::mouseDown(MouseEvent event)
{
    for (int i = 0; i < am->mapSize(); i++)
        if ((event.getPos()-particles[i]->getx()).lengthSquared() <= 9.0f)
        {
            if (dDown)
                particles[i]->disturb();
            else
            {
                particles[i]->mouseGrab();
                particles[i]->mouseDrag(event.getPos());
                mouseOn = i;
                if (!spaceDown)
                    break;
            }
        }
}

void ArrowMapApp::mouseDrag(MouseEvent event)
{
    if (spaceDown)
        for (int i = 0; i < am->mapSize(); i++)
            particles[i]->mouseDrag(event.getPos());
    else if (mouseOn < am->mapSize()) //DEBUG: Cheap hack, will usually work
        particles[mouseOn]->mouseDrag(event.getPos());
    
    mousePos = event.getPos();
}

void ArrowMapApp::mouseUp(MouseEvent event)
{
    if (spaceDown)
        for (int i = 0; i < am->mapSize(); i++)
            particles[i]->mouseRelease();
    else
        particles[mouseOn]->mouseRelease();
    
    mousePos = Vec2i(-1, 0);
}

void ArrowMapApp::keyDown(KeyEvent event)
{
    switch (event.getChar())
    {
        case ' ':
            spaceDown = true;
            break;
        case 'd':
            dDown = true;
            break;
    }
}

void ArrowMapApp::keyUp(KeyEvent event)
{
    spaceDown = false;
    dDown = false;
}

void ArrowMapApp::update()
{
    //float frameTime = timer.getSeconds() - t;
    for (int i = 0; i < am->mapSize(); i++)
        particles[i]->update(1.0f / 60.0f);
    //t = timer.getSeconds();
}

void ArrowMapApp::draw()
{
	gl::clear(Color(0, 0, 0));
    
    for (int i = 0; i < am->mapSize(); i++)
    {
        gl::color(1.0f, 1.0f, 1.0f);
        gl::drawLine(particles[i]->getx(), particles[am->getNextState(i)]->getx());
        
        if (particles[i]->isMouseOn())
            gl::color(1.0f, 0.0f, 0.0f);
        else if (particles[i]->isAsleep())
            gl::color(0.6f, 0.6f, 0.6f);
        else
            gl::color(1.0f, 1.0f, 1.0f);
        gl::drawSolidCircle(particles[i]->getx(), 2.0f);
    }
    
    if (mousePos.x >= 0)
    {
        gl::color(1.0f, 0.0f, 0.0f, 0.5f);
        gl::drawSolidCircle(mousePos, 3.0f);
    }
}

void ArrowMapApp::shutdown()
{
    for (int i = 0; i < am->mapSize(); i++)
        delete particles[i];
    delete [] particles;
    delete am;
}

CINDER_APP_BASIC(ArrowMapApp, RendererGl)
