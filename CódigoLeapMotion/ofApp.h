#pragma once

#include "ofMain.h"
#include "ofxLeapMotion2.h"
#include "ofxJSONElement.h"
#include "ofxHttpUtils.h"


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
	
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void exit();
    
    // MŽtodos propios
    int     detectaGesto();
    int     detectaGesto2();
    void    escribirJSON(int gesto);
    
    // Objetos
	ofxLeapMotion       leap;
    ofxJSONElement      data;
    ofEasyCam           cam;

    // Vectores
	vector <ofxLeapMotionSimpleHand> simpleHands;
	vector <int> fingersFound;
    
    // Variables globales
    bool        usuarioPresente;
    bool        gestoMapeable[22];
    int         numManos;
    int         gesto;
    int         contador;
    int         contadorJSON;
    int         numLampara;
    int         menu;
    int         presente, pasado;
    bool        lecturaSwitch;
    bool        lecturaGestos;
    bool        lampara[8];
    
    // Tipograf’a
    ofTrueTypeFont  letra;
    
    // Escribir JSON
    
    void newResponse(ofxHttpResponse & response);
    
    ofxHttpUtils httpUtils;
    int counter;
    string responseStr;
    string requestStr;
    string action_url;
    
};
