#pragma once

#include "ofMain.h"
#include "ofxBaseTwitterApi.h"
#include "ofxUI.h"
#include "ofxOsc.h"
#include "GraphUtils.h"

class testApp : public ofBaseApp{

	public:
		testApp();
	
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
		
		ofTrueTypeFont myFont;
		ofxBaseTwitterApi myTwitter;
	
		ofxUIScrollableCanvas gui;
		void guiEvent(ofxUIEventArgs &e);

		string fitStringToWidth(const string s, const int w, ofTrueTypeFont ttf);

		Graph myGraph;
		ofxOscSender sender;
};
