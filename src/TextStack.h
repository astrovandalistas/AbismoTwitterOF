//
//  TextStack.h
//  AbismoTwitterOF
//
//  Created by thiago on 1/25/13.
//
//

#pragma once

#include "ofConstants.h"
#include "ofTypes.h"

class TextObject {
	public:
		TextObject(ofVec2f pos_, int size_, string font_, string text_);
		~TextObject();
		ofVec2f pos;
		int size;
		string font, text;
};

class TextStack {
	public:
		TextStack();
		~TextStack();
		void pushObject(TextObject obj_);
		void popObject();
		void clearObjects();
		const int size() const;
		const TextObject& operator[](const unsigned int i);
		const TextObject& peek();
		// for live tweets
		void setLive(TextObject obj_);
		const TextObject& getLive() const;
		void clearLive();
	protected:
		vector<TextObject> theStack;
};
