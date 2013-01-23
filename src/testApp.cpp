#include "testApp.h"

#define CONSUMER_KEY "b7BrQXjIRKT8MhEkWnhQ"
#define CONSUMER_SECRET "bx9rw5bsAb4K5WPmiT2UIc2j8Kmo5JVrlLHw9qpToh4"

//--------------------------------------------------------------
testApp::testApp() : gui(0,0,ofGetWidth()/4,ofGetHeight()), ofBaseApp(){ }

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofBackgroundHex(0xffff00);
	//ofSetLogLevel(OF_LOG_VERBOSE);
	myFont.loadFont("verdana.ttf",12);
	myTwitter.setup(CONSUMER_KEY,CONSUMER_SECRET);

	vector<Tweet> theTweets = myTwitter.getTweets();

	gui.setFont("verdana.ttf");
    gui.setScrollableDirections(false, true);

	gui.addWidgetDown(new ofxUILabel("Live Tweets", OFX_UI_FONT_MEDIUM));
	gui.addLabelToggle("Enabled", false);
	gui.addSpacer(gui.getRect()->width,4);
	gui.addWidgetDown(new ofxUILabel("Tweets", OFX_UI_FONT_MEDIUM));

	
	for(int i=0;i<50;i++){
		gui.addLabelButton("LABEL BUTTON"+ofToString(i), false);
	}
	gui.autoSizeToFitWidgets();
}

//--------------------------------------------------------------
void testApp::update(){
	ofSetHexColor(0xff00ff);
}

//--------------------------------------------------------------
void testApp::draw(){

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
