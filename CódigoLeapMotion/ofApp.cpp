#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // Funciones de inicializaci—n
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_VERBOSE);
    
    // Inicializar Leap Motion
	leap.open();
    leap.setupGestures();

    // C‡mara para puntos 3Ds
	cam.setOrientation(ofPoint(-20, 0, 0));
    
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    
    // Inicializar variables globales
    usuarioPresente     = FALSE;
    lecturaSwitch       = TRUE;
    lecturaGestos       = TRUE;
    numManos            = 0;
    gesto               = 0;
    contador            = 0;
    contadorJSON        = 0;
    numLampara          = 0;
    menu                = 0;
    presente            = 0;
    pasado              = 0;
    
    // Inicializar arreglos
    for (int i = 0; i < 8; i++) {
        lampara[i] = FALSE;
    }
    
    for (int i = 0; i < 22; i++) {
        gestoMapeable[i] = FALSE;
    }
    
    // Tipograf’a
    letra.loadFont("futura.ttc", 32);
    
    // Mandar JSON
    action_url = "https://vast-mesa-3135.herokuapp.com/domotica";
	ofAddListener(httpUtils.newResponseEvent,this,&ofApp::newResponse);
	httpUtils.start();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // Actualizaci—n de valores
	fingersFound.clear();
    leap.updateGestures();
    
    // Contadores de frames recorridos
    contador++;
    if(contador > 35){
        contador            = 0;
        lecturaSwitch       = TRUE;
    }
    
    contadorJSON++;
    if(contadorJSON > 45){
        contadorJSON        = 0;
        lecturaGestos       = TRUE;
    }
	
    
    // Obtener datos de Leap Motion
    simpleHands = leap.getSimpleHands();
    
    if( leap.isFrameNew() && simpleHands.size() ){
        
        leap.setMappingX(-230, 230, -ofGetWidth()/2, ofGetWidth()/2);
		leap.setMappingY(90, 490, -ofGetHeight()/2, ofGetHeight()/2);
        leap.setMappingZ(-150, 150, -200, 200);
        
        fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
        
        
        for(int i = 0; i < simpleHands.size(); i++){
            for (int f=0; f<5; f++) {
                int id = simpleHands[i].fingers[ fingerTypes[f] ].id;
                ofPoint mcp = simpleHands[i].fingers[ fingerTypes[f] ].mcp; // metacarpal
                ofPoint pip = simpleHands[i].fingers[ fingerTypes[f] ].pip; // proximal
                ofPoint dip = simpleHands[i].fingers[ fingerTypes[f] ].dip; // distal
                ofPoint tip = simpleHands[i].fingers[ fingerTypes[f] ].tip; // fingertip
                fingersFound.push_back(id);
            }
        }
        usuarioPresente     = TRUE;
    }
    
    // Presencia de usuario
    if( simpleHands.size() == 0){
        usuarioPresente     = FALSE;
        numManos            = 0;
    }
    
    // Escribir JSONs
    if (usuarioPresente && lecturaGestos) {
        presente = detectaGesto2();
        if(presente != pasado){
            escribirJSON(presente);
        }
    }
    
    pasado = presente;
    
	leap.markFrameAsOld();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // Inicializar par‡metros de fondo
    ofBackgroundGradient(ofColor(90, 90, 90), ofColor(30, 30, 30),  OF_GRADIENT_BAR);
	ofSetColor(200);
    
    // Inicializar entorno 3D
	cam.begin();
    
	ofPushMatrix();
        ofRotate(90, 0, 0, 1);
        ofSetColor(20);
        ofDrawGridPlane(800, 20, false);
	ofPopMatrix();
    
    
    fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
    
    for(int i = 0; i < simpleHands.size(); i++){
        bool isLeft        = simpleHands[i].isLeft;
        ofPoint handPos    = simpleHands[i].handPos;
        ofPoint handNormal = simpleHands[i].handNormal;
        
        ofSetColor(0, 0, 255);
        ofDrawSphere(handPos.x, handPos.y, handPos.z, 20);
        ofSetColor(255, 255, 0);
        ofDrawArrow(handPos, handPos + 100*handNormal);
        
        for (int f=0; f<5; f++) {
            ofPoint mcp = simpleHands[i].fingers[ fingerTypes[f] ].mcp;  // metacarpal
            ofPoint pip = simpleHands[i].fingers[ fingerTypes[f] ].pip;  // proximal
            ofPoint dip = simpleHands[i].fingers[ fingerTypes[f] ].dip;  // distal
            ofPoint tip = simpleHands[i].fingers[ fingerTypes[f] ].tip;  // fingertip
            
            ofSetColor(0, 255, 0);
            ofDrawSphere(mcp.x, mcp.y, mcp.z, 12);
            ofDrawSphere(pip.x, pip.y, pip.z, 12);
            ofDrawSphere(dip.x, dip.y, dip.z, 12);
            ofDrawSphere(tip.x, tip.y, tip.z, 12);
            
            ofSetColor(255, 0, 0);
            ofSetLineWidth(20);
            ofLine(mcp.x, mcp.y, mcp.z, pip.x, pip.y, pip.z);
            ofLine(pip.x, pip.y, pip.z, dip.x, dip.y, dip.z);
            ofLine(dip.x, dip.y, dip.z, tip.x, tip.y, tip.z);
        }
    }
	cam.end();
    
    // Texto para depurar datos
    
    if(usuarioPresente){
        letra.drawString("Usuario detectado", 20, 200);
        
        string s = "Num manos " + ofToString(numManos);
        letra.drawString(s, 20, 250);
        
        s = "Gesto " + ofToString(gesto);
        letra.drawString(s, 20, 300);
        
        s = "Lampara " + ofToString(numLampara);
        letra.drawString(s, 20, 350);
        
        s = "Menu " + ofToString(menu);
        letra.drawString(s, 20, 400);
    
    }

    for (int i = 0; i < 8; i++) {
        ofPushStyle();
            ofSetColor(255*lampara[i],255*lampara[i],255*lampara[i],128);
            ofRect(i*ofGetWidth()/8, 0, ofGetWidth()/8, ofGetHeight());
        ofPopStyle();
    }
    
}

//--------------------------------------------------------------
void ofApp::newResponse(ofxHttpResponse & response){
	responseStr = ofToString(response.status) + ": " + (string)response.responseBody;
    cout << responseStr << " ";
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

//--------------------------------------------------------------
void ofApp::exit(){
    // Cerrar conexi—n con Leap
    leap.close();
}

//--------------------------------------------------------------
int ofApp::detectaGesto(){

    // Detectar gestos
    int gestoInterno        = 0;
    int altura              = 120;
    
    if(!simpleHands[0].isLeft){
        numLampara              = ofMap(simpleHands[0].fingers[INDEX].tip.x,-300, 300, 0, 8);
        if (numLampara > 7) {
            numLampara = 7;
        }
        if (numLampara < 0) {
            numLampara = 0;
        }
    }

    if (!lecturaSwitch) {
        numLampara = 99;
    }
    
    switch (numLampara) {
        case 0:
            if (simpleHands[0].fingers[INDEX].tip.y > altura) {
                lampara[0]          = !lampara[0];
                lecturaSwitch       = FALSE;
                contador            = 0;

            }
            break;
        case 1:
            if (simpleHands[0].fingers[INDEX].tip.y > altura) {
                lampara[1]          = !lampara[1];
                lecturaSwitch       = FALSE;
                contador            = 0;

            }
            break;
        case 2:
            if (simpleHands[0].fingers[INDEX].tip.y > altura) {
                lampara[2] = !lampara[2];
                lecturaSwitch       = FALSE;
                contador            = 0;


            }
            break;
        case 3:
            if (simpleHands[0].fingers[INDEX].tip.y > altura) {
                lampara[3]          = !lampara[3];
                lecturaSwitch       = FALSE;
                contador            = 0;


            }
            break;
        case 4:
            if (simpleHands[0].fingers[INDEX].tip.y > altura) {
                lampara[4]          = !lampara[4];
                lecturaSwitch       = FALSE;
                contador            = 0;

            }
            break;
        case 5:
            if (simpleHands[0].fingers[INDEX].tip.y > altura) {
                lampara[5]          = !lampara[5];
                lecturaSwitch       = FALSE;
                contador            = 0;
            }
            break;
        case 6:
            if (simpleHands[0].fingers[INDEX].tip.y > altura) {
                lampara[6]          = !lampara[6];
                lecturaSwitch       = FALSE;
                contador            = 0;

            }
            break;
        case 7:
            if (simpleHands[0].fingers[INDEX].tip.y > altura) {
                lampara[7]          = !lampara[7];
                lecturaSwitch       = FALSE;
                contador            = 0;

            }
            break;
        default:
            break;
    }
    
    return gestoInterno;
}



//--------------------------------------------------------------
int ofApp::detectaGesto2(){
    
    // Detecci—n de gestos
    int gestoInterno        = 0;

    
    /* Leap iGesture Key
     --------------------------------
     1 = Screen Tap
     2 = Key Tap
     3 = Swipe Right
     4 = Swipe Left
     5 = Swipe Down
     6 = Swipe Up
     7 = Swipe Forward
     8 = Swipe Backward (towards yourself)
     9 = Circle Left (counter-clockwise)
     10 = Circle Right (clockwise)
     --------------------------------
     */
    
    if( leap.isFrameNew() && simpleHands.size() ){
        for(int i = 0; i < simpleHands.size(); i++){
            // Variables de estado del usuario
            usuarioPresente     = TRUE;
            numManos            = simpleHands.size();
            //gesto               = detectaGesto();
            
            // Gestos de mano izquierda
            if(simpleHands[i].isLeft){
                
                ofVec3f orientacion = simpleHands[i].handNormal;
                ofVec3f origen;
                origen.x        = 0;
                origen.y        = 1;
                origen.z        = 1;
                int angulo = orientacion.angle(origen);
                //cout << angulo << "\n";
                
                int delta       = 22;
                
                /* Elige menu
                 
                 0 - Luces
                 1 - Cortinas
                 2 - TV
                 3 - Spotify
                 
                 */
                if (abs(angulo - 45) < delta) {
                    gestoInterno = 10;
                } else  if (abs(angulo - 90) < delta){
                    if(simpleHands[i].fingers[THUMB].tip.y < simpleHands[i].handPos.y){
                        gestoInterno = 13;
                    } else {
                        gestoInterno = 11;
                    }
                } else  if (abs(angulo - 135) < delta){
                    gestoInterno = 12;
                } else {
                    gestoInterno = 99;
                }
            } else {
                gestoInterno     = leap.iGestures;
            }
        }
    }
    
    return gestoInterno;
}


//--------------------------------------------------------------
void ofApp::escribirJSON(int gesto){
    
    gestoMapeable[gesto] = !gestoMapeable[gesto];
    string s = ofToString(gesto);
    s = s + "," + ofToString(gestoMapeable[gesto]);
    ofxHttpForm form;
        form.clearFormFields();
        form.action = action_url;
        form.method = OFX_HTTP_POST;
    form.addFormField("gesto", s);
    
}
