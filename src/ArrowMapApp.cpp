#include <vector>
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
    vector<Agent*> agents;
    vector<Color*> agentColours;
    int currentAgent;
    unsigned int mouseOn;
    Vec2i mousePos;
    bool spaceDown, dDown, aDown;
    Timer timer;
    float t;
    
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
    aDown = false;
    mousePos = Vec2i(-1, 0);
    mouseOn = 0;
    
    //http://kuler.adobe.com/#themeID/2063161
    agents.push_back(new Agent(am, "localhost", "localhost", 1234, 1234));
    agentColours.push_back(new Color(72 / 255.0f, 82 / 255.0f, 255 / 255.0f));
    agents.push_back(new Agent(am, "localhost", "localhost", 1234, 1234));
    agentColours.push_back(new Color(255 / 255.0f, 63 / 255.0f, 179 / 255.0f));
    agents.push_back(new Agent(am, "localhost", "localhost", 1234, 1234));
    agentColours.push_back(new Color(31 / 255.0f, 209 / 255.0f, 232 / 255.0f));
    agents.push_back(new Agent(am, "localhost", "localhost", 1234, 1234));
    agentColours.push_back(new Color(50 / 255.0f, 255 / 255.0f, 53 / 255.0f));
    currentAgent = 0;
    
    timer.start();
    t = 0.0f;
    
    gl::enableAlphaBlending();
}

void ArrowMapApp::mouseDown(MouseEvent event)
{
    if (aDown)
    {
        float shortestDist, currentDist;
        unsigned int closest;
        for (int i = 0; i < am->mapSize(); i++)
        {
            currentDist = (event.getPos()-particles[i]->getx()).lengthSquared();
            if (i == 0 || currentDist < shortestDist)
            {
                shortestDist = currentDist;
                closest = i;
            }
        }
        agents[currentAgent]->drop(closest, 0.4f, 70, 90);
    }
    else
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
        case 'a':
            aDown = true;
            break;
        case '1':
            currentAgent = 0;
            break;
        case '2':
            currentAgent = 1;
            break;
        case '3':
            currentAgent = 2;
            break;
        case '4':
            currentAgent = 3;
            break;
    }
}

void ArrowMapApp::keyUp(KeyEvent event)
{
    spaceDown = false;
    dDown = false;
    aDown = false;
}

void ArrowMapApp::update()
{
    if (timer.getSeconds() - t >= 0.07)
    {
        for (vector<Agent*>::iterator it = agents.begin(); it < agents.end(); it++)
            (*it)->step();
        t = timer.getSeconds();
    }
    
    for (int i = 0; i < am->mapSize(); i++)
        particles[i]->update(1.0f / 60.0f);

}

void ArrowMapApp::draw()
{
    //Light purple: 0.878f, 0.69f, 0.965f
    
	gl::clear(Color(0, 0, 0));
    
    for (int i = 0; i < am->mapSize(); i++)
    {
        if (am->isAttractor(i))
            gl::color(0.765f, 0.384f, 0.941f);
        else
            gl::color(0.9f, 0.9f, 0.9f);
        gl::drawLine(particles[i]->getx(), particles[am->getNextState(i)]->getx());
        
        if (particles[i]->isMouseOn())
            gl::color(1.0f, 0.0f, 0.0f);
        else if (particles[i]->isAsleep())
            gl::color(0.6f, 0.6f, 0.6f);
        else if (am->isAttractor(i))
            gl::color(0.765f, 0.384f, 0.941f);
        else
            gl::color(0.9f, 0.9f, 0.9f);
        gl::drawSolidCircle(particles[i]->getx(), 2.0f);
    }
    
    for (int i = 0; i < agents.size(); i++)
        if (agents[i]->isDropped())
        {
            gl::color(agentColours[i]->r, agentColours[i]->g, agentColours[i]->b, agents[i]->getStrength());
            gl::drawSolidCircle(particles[agents[i]->getState()]->getx(), 4.0f);
        }
    
    if (mousePos.x >= 0)
    {
        gl::color(1.0f, 0.0f, 0.0f, 0.5f);
        gl::drawSolidCircle(mousePos, 3.0f);
    }
}

void ArrowMapApp::shutdown()
{
    for (vector<Agent*>::iterator it = agents.begin(); it < agents.end(); it++)
        delete *it;
    for (vector<Color*>::iterator it = agentColours.begin(); it < agentColours.end(); it++)
        delete *it;
    for (int i = 0; i < am->mapSize(); i++)
        delete particles[i];
    delete [] particles;
    delete am;
}

CINDER_APP_BASIC(ArrowMapApp, RendererGl)
