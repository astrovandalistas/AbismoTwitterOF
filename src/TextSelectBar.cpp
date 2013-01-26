//
//  TextSelectBar.cpp
//  AbismoTwitterOF
//
//  Created by thiago on 1/24/13.
//
//

#include "TextSelectBar.h"
#include "ofGraphics.h"

#define TEXTBAR_FONTSIZE 28

TextSelectBar::TextSelectBar(){
	ofRegisterMouseEvents(this);
	mFont.loadFont("verdana.ttf", TEXTBAR_FONTSIZE);
	selectedText = "";
	colorUntil = -1;
	colorFrom = -1;
}
TextSelectBar::~TextSelectBar(){}

void TextSelectBar::setup(int x_, int y_, int w_, int h_, string s_){
	x = x_;
	y = y_;
	w = w_;
	h = h_;
	setText(s_);
}

void TextSelectBar::setText(string s_){
	fullText = s_;

	// clean up string
	for(int i=0;i<fullText.size();i++){
		if(fullText[i] == '\n'){
			fullText.replace(i, 1, " ");
		}
	}

	// find a font size
	mFont.loadFont("verdana.ttf",TEXTBAR_FONTSIZE);
	while(mFont.stringWidth(fullText) > w && mFont.getSize()>8){
		mFont.loadFont("verdana.ttf",mFont.getSize()-1);
	}
	
	// split into words and calculate word start position
	istringstream ss(fullText+" ");
	float xloc = x;
	int i = 0;
	phrase.clear();
	wordLocations.clear();
	
	while (!ss.eof()){
		string word;
		getline(ss,word,' ');
		word += " ";
		phrase.push_back(word);
		wordLocations[xloc] = i;
		xloc += mFont.stringWidth(word);
		xloc += mFont.stringWidth("p");
		i++;
	}
	colorUntil = -1;
	colorFrom = -1;
}

const string TextSelectBar::getSelectedText() const{
	return selectedText;
}

void TextSelectBar::consumeOneWord(){
	colorFrom++;
	selectedText = "";
	for(int i=colorFrom; i<phrase.size()&&i<=colorUntil; ++i){
		selectedText += phrase[i];
	}
}

void TextSelectBar::draw(){
	ofSetColor(100, 100);
	ofRect(x,y,w,h);
	
	float xloc = x;
	for(int i=0; i<phrase.size(); ++i){
		if(i<colorFrom || i>colorUntil){
			ofSetHexColor(0xffffff);
		}
		else{
			ofSetHexColor(0xe800fe);
		}
		mFont.drawString(phrase[i], xloc,y+h-5);
		xloc += mFont.stringWidth(phrase[i]);
		xloc += mFont.stringWidth("p");
	}
}

void TextSelectBar::mousePressed(ofMouseEventArgs & args){
	if(args.y > y && args.y < (y+h)){
		std::map<float,int>::iterator it = wordLocations.lower_bound(args.x);
		colorUntil = -1;
		colorFrom = (it->second)-1;
	}
}
void TextSelectBar::mouseDragged(ofMouseEventArgs & args){
	if(args.y > y && args.y < (y+h)){
		std::map<float,int>::iterator it = wordLocations.lower_bound(args.x);
		colorUntil = (it->second)-1;
	}
}
void TextSelectBar::mouseReleased(ofMouseEventArgs & args){
	if(args.y > y && args.y < (y+h) && colorUntil > -1){
		selectedText = "";
		for(int i=colorFrom; i<phrase.size()&&i<=colorUntil; ++i){
			selectedText += phrase[i];
		}
	}
}
void TextSelectBar::mouseMoved(ofMouseEventArgs & args){}
