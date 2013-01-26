//
//  TextStack.cpp
//  AbismoTwitterOF
//
//  Created by thiago on 1/25/13.
//
//

#include "TextStack.h"

TextObject::TextObject(ofVec2f pos_, int size_, string font_, string text_){
	pos = pos_;
	size = size_;
	font = font_;
	text = text_;
}

TextObject::~TextObject(){}

TextStack::TextStack(){}
TextStack::~TextStack(){}

void TextStack::pushObject(TextObject obj_){
	theStack.push_back(obj_);
}
void TextStack::popObject(){
	if(!theStack.empty()){
		theStack.pop_back();
	}
}
void TextStack::clearObjects(){
	theStack.clear();
}

const TextObject& TextStack::operator[](const unsigned int i){
	if(i<theStack.size()){
		return theStack.at(i);
	}
}

const TextObject& TextStack::peek(){
	if(!theStack.empty()){
		return theStack.back();
	}
}
