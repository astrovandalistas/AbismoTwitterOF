#pragma once

#include "ofMain.h"
#include "ofxBaseTwitterApi.h"
#include "ofxUI.h"
#include "ofxOsc.h"
#include "GraphUtils.h"
#include "TextSelectBar.h"

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
	
		ofxUIScrollableCanvas tweetGui;
		ofxUICanvas buttonGui;
		void tweetGuiEvent(ofxUIEventArgs &e);
		void buttonGuiEvent(ofxUIEventArgs &e);
		void sendLiveTweet(Tweet& t);
		bool bSendLiveTweets;

		string fitStringToWidth(const string s, const int w, ofTrueTypeFont ttf);

		Graph myGraph;
		ofxOscSender sender;
		TextSelectBar mTSB;
		// this could've been in a class... could've been...
		ofRectangle drawArea, tweetArea, liveArea;
		int mFontSize;

		// debug
		void testGraphSetup();
		void testGraphUpdate();
};
