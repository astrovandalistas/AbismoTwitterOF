//
//  TextStack.cpp
//  AbismoTwitterOF
//
//  Created by thiago on 1/25/13.
//
//

#include "TextStack.h"
#define DEFAULT_FONT "verdana.ttf"

TextObject::TextObject(ofVec2f pos_, int size_, string font_, string text_){
	pos = pos_;
	size = size_;
	font = font_;
	text = text_;
}

TextObject::~TextObject(){}

TextStack::TextStack(){
	// the live tweet
	TextObject tto = TextObject(ofVec2f(0,0), 18, DEFAULT_FONT, "");
	theStack.push_back(tto);
}
TextStack::~TextStack(){}

void TextStack::pushObject(TextObject obj_){
	theStack.push_back(obj_);
}
void TextStack::popObject(){
	if(theStack.size() > 1){
		theStack.pop_back();
	}
}
void TextStack::clearObjects(){
	theStack.clear();
	// the live tweet
	TextObject tto = TextObject(ofVec2f(0,0), 18, DEFAULT_FONT, "");
	theStack.push_back(tto);
}

const int TextStack::size() const{
	return (theStack.size() - 1);
}

const TextObject& TextStack::operator[](const unsigned int i){
	if(i < theStack.size()-1){
		return theStack.at(i+1);
	}
}

const TextObject& TextStack::peek(){
	if(theStack.size() > 1){
		return theStack.back();
	}
	return theStack.at(0);
}

void TextStack::setLive(TextObject obj_){
	theStack.at(0) = obj_;
}
const TextObject& TextStack::getLive() const{
	return theStack.at(0);
}
void TextStack::clearLive(){
	// the live tweet
	TextObject tto = TextObject(ofVec2f(0,0), 18, DEFAULT_FONT, "");
	theStack.at(0) = tto;
}
