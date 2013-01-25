#pragma once

#include "ofMain.h"
#include "ofxBaseTwitterApi.h"
#include "ofxUI.h"
#include "ofxOsc.h"
#include "GraphUtils.h"
#include "TextSelectBar.h"
//#include "TextStack.h"

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

		// font
		ofTrueTypeFont myFont;
		int mFontSize;

		// twitter
		ofxBaseTwitterApi myTwitter;
		void sendLiveTweet(Tweet& t);
		bool bSendLiveTweets;

		// gui
		ofxUIScrollableCanvas tweetGui;
		ofxUICanvas buttonGui;
		TextSelectBar mTSB;
		// this could've been in a class... could've been...
		ofRectangle drawArea, tweetArea, liveArea;
		void tweetGuiEvent(ofxUIEventArgs &e);
		void buttonGuiEvent(ofxUIEventArgs &e);

		// osc
		ofxOscSender sender;


		// helpers
		string fitStringToWidth(const string s, const int w, ofTrueTypeFont ttf);

		// TextStack (DEBUG)
		//TextStack mTextStack;

		// Graph (DEBUG)
		Graph myGraph;
		void testGraphSetup();
		void testGraphUpdate();
};
