/*==============================================================================
 
 Copyright (c) 2010-2013 Christopher Baker <http://christopherbaker.net>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 ==============================================================================*/

#pragma once

#include "ofxOAuth.h"
#include "jansson.h"

struct ofxTweet {
	string text, user, date;
	long long int id;
	ofxTweet(string t, string u, string d, long long int i){
		text = t;
		user = u;
		date = d;
		id = i;
	}
	operator string(){
		return "@"+user+"("+date+"):"+text;
	}
};

class ofxBaseTwitterApi : public ofxOAuth, public ofThread {
public:
    ofxBaseTwitterApi() {}
    virtual ~ofxBaseTwitterApi() {
		ofThread::stopThread();
		ofThread::waitForThread();
	}
    void setup(const string& _consumerKey, const string& _consumerSecret);
	string getTweets();
	string getLiveTweets();
protected:
	string userName, searchTerm;
	long long int lastTweetID;
	vector<ofxTweet> staticTweets, liveTweets;
	void threadedFunction();
private:
	const string getStaticTweetsJson();
	const string getLiveTweetsJson();
	void setUserName();
	void parseTweets(const string& _json, vector<ofxTweet>& theTweets);
	void splitJson(const string& _json, vector<string>& jsonTweets);
};

