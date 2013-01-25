//
//  TextSelectBar.h
//  AbismoTwitterOF
//
//  Created by thiago on 1/24/13.
//
//

#pragma once

#include "ofConstants.h"
#include "ofEvents.h"
#include "ofTrueTypeFont.h"
#include <map>

class TextSelectBar {
public:
	TextSelectBar();
	~TextSelectBar();
	void setup(int x_, int y_, int w_, int h_, string s_="");
	void setString(string s_);

	void mouseMoved(ofMouseEventArgs & args);
    void mouseDragged(ofMouseEventArgs & args);
    void mousePressed(ofMouseEventArgs & args);
    void mouseReleased(ofMouseEventArgs & args);
	void draw();
protected:
	int x,y,w,h;
	string s;
	ofTrueTypeFont mFont;
	ofVec2f mClick;
	vector<string> phrase;
	map<float,int> wordLocations;
	int colorUntil, colorFrom;
};

