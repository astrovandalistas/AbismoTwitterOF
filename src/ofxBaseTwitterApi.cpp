//
//  ofxBaseTwitterApi.cpp
//  AbismoTwitterOF
//
//  Created by thiago on 1/20/13.
//
//

#include "ofxBaseTwitterApi.h"

#define DEFAULT_USER "DEFAULT_USER"

void ofxBaseTwitterApi::setup(const string& _consumerKey, const string& _consumerSecret) {
	ofxOAuth::setup("https://api.twitter.com",_consumerKey,_consumerSecret);
	
	userName = "";
	lastTweetID = 75502655925002239;
	searchTerm = "#Abismo #abismo";
	
	ofThread::startThread();
}

string ofxBaseTwitterApi::getTweets() {
	string foo = "";
	if(userName.compare("") == 0){
		setUserName();
	}

	// first time this is called. Fill it with stuff that's already online.
	if(staticTweets.empty()){
		ofxBaseTwitterApi::parseTweets(getStaticTweetsJson(), staticTweets);
	}
	for(int i=0; i<staticTweets.size(); ++i){
		foo += staticTweets[i]+"\n";
	}

	return foo;
}

/*
 Returns most recent retweets posted by the authenticated user
 http://api.twitter.com/1/statuses/retweeted_by_me.json
 
 Returns most recent tweets of the authenticated user that have been retweeted by others
 http://api.twitter.com/1/statuses/retweets_of_me.json
 
 Returns most recent retweets posted by the authenticating users friends
 http://api.twitter.com/1/statuses/retweeted_to_me.json
 */

// set userName based on who's signed in
void ofxBaseTwitterApi::setUserName(){
	string userJson = get("/1.1/account/settings.json");
	json_error_t error;
	json_t* root = json_loads(userJson.c_str(), 0, &error);
	if(root) {
		json_t* node = json_object_get(root, "screen_name");
		if(json_is_string(node)) {
			userName = json_string_value(node);
			return;
		}
	}
	userName = DEFAULT_USER;
}

const string ofxBaseTwitterApi::getStaticTweetsJson() {
	if(isAuthorized()){
		stringstream ss;
		ss<<lastTweetID;
		return get("/1.1/statuses/user_timeline.json?include_entities=false&trim_user=true&exclude_replies=true&include_rts=true&since_id="+ss.str());
	}
	return "";
}

const string ofxBaseTwitterApi::getLiveTweetsJson() {
	if(isAuthorized()){
		stringstream ss;
		ss<<lastTweetID;
		return get("/1.1/search/tweets.json?q="+searchTerm+"&include_entities=false&count=5&result_type=recent&since_id="+ss.str());
	}
	return "";
}

void ofxBaseTwitterApi::threadedFunction(){
	while(isThreadRunning()){
		//cout << getLiveTweetsJson() << endl << endl;
		if(lock()){
			ofxBaseTwitterApi::parseTweets(getLiveTweetsJson(), liveTweets);
			unlock();
			ofThread::sleep(8000);
		}
	}
}

// return the biggest tweet id
void ofxBaseTwitterApi::parseTweets(const string& _json, vector<string>& theTweets){
	vector<string> jsonTweets;
	splitJson(_json, jsonTweets);

	json_error_t error;
	json_t* root;
	
	// create own struct/object here
	for(int i=jsonTweets.size()-1; i>=0; --i){
		// load json. get root.
		root = json_loads(jsonTweets[i].c_str(), 0, &error);
		if(root) {
			json_t* node;
			
			string text,date,user,id;

			////////// id
			long long int myID = 0;
			node =	json_object_get(root, "id_str");
			if(json_is_string(node)) {
				id = json_string_value(node);
				istringstream cur(id);
				cur >> myID;
				if(myID > lastTweetID){
					lastTweetID = myID;
				}
			}
			// couldn't parse id
			else{
				ofLogError("parseTweets") << "cannot get id from: " << jsonTweets[i] << endl;
			}
			// if this id is less than lastID, skip this tweet
			if(myID<lastTweetID){
				continue;
			}
			
			////////// date
			node =	json_object_get(root, "created_at");
			if(json_is_string(node)) {
				date = json_string_value(node);
			}
			// couldn't parse date
			else{
				ofLogError("parseTweets") << "cannot get date from: " << jsonTweets[i] << endl;
			}

			////////// text & user
			node =	json_object_get(root, "text");
			json_t* juser = json_object_get(root, "user");
			if(json_is_string(node)) {
				text = json_string_value(node);
				// if retweet, remove "RT @user:", and set user:
				if(text.compare(0,4,"RT @") == 0){
					user = text.substr(text.find("RT @")+4, text.find(":")-text.find("RT @")-4);
					text = text.substr(text.find(":")+2);
				}
				// else try to read the user object
				else if(json_is_object(juser) && json_is_string(json_object_get(juser, "screen_name"))){
					user = json_string_value(json_object_get(juser, "screen_name"));
				}
				else{
					user = userName;
				}
			}
			// couldn't parse text
			else{
				ofLogError("parseTweets") << "cannot get text from: " << jsonTweets[i] << endl;
			}

			////// results
			ofLogNotice("tweeeeet!") << id << "@" << user << "(" << date << "):" << text;
			theTweets.push_back("@"+user+"("+date+"):"+text);
		}
		// couldn't get root of json record
		else{
			ofLogError("parseTweets") << "can't get root from: " << jsonTweets[i] << endl;
		}
	}
}

void ofxBaseTwitterApi::splitJson(const string& _json, vector<string>& jsonTweets){
	// split jsons...
	int lastStart = -1;
	int openCnt = 0;
	bool seenBracket = false;
	
	for(int i=0; i<_json.size(); ++i){
		if(_json[i] == '['){
			seenBracket = true;
		}
		if((_json[i] == '{')&&seenBracket){
			// starting {
			if(openCnt == 0){
				lastStart = i;
			}
			openCnt++;
		}
		if((_json[i] == '}')&&seenBracket){
			// found last }
			if(openCnt == 1){
				jsonTweets.push_back(_json.substr(lastStart,i-lastStart+1));
				//cout << jsonTweets[jsonTweets.size()-1]<<endl<<endl;
			}
			openCnt--;
		}
	}
}