#pragma once

#include "ofMain.h"
#include "ofxBaseTwitterApi.h"
#include "ofxUI.h"
#include "ofxOsc.h"
#include "GraphUtils.h"
#include "TextSelectBar.h"
#include "TextStack.h"

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

		// twitter
		ofxBaseTwitterApi mTwitter;
		void sendLiveTweet(Tweet& t);

		// gui
		ofxUIScrollableCanvas tweetGui;
		ofxUICanvas buttonGui;
		TextSelectBar mTSB;
		bool bWordByWord;
		float wordByWordOffset;
		// this could've been in a class... could've been...
		ofRectangle drawArea, staticTweetArea, liveTweetArea;
		void tweetGuiEvent(ofxUIEventArgs &e);
		void buttonGuiEvent(ofxUIEventArgs &e);

		// osc
		ofxOscSender sender;
		ofTrueTypeFont oscFont;
		string oscFontName;
		int oscFontSize;

		// helpers
		string fitStringToWidth(const string s, const int w, ofTrueTypeFont ttf);

		// TextStack (DEBUG)
		TextStack mTextStack;

		// Graph (DEBUG)
		Graph mGraph;
		void testGraphSetup();
		void testGraphUpdate();
};
