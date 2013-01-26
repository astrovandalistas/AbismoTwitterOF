#include "testApp.h"

#define CONSUMER_KEY "aSP37b2OHYSjtHSuxG7g"
#define CONSUMER_SECRET "FYFxhSFNMqa4tpSG1mvL9EPWN7kW013YpHYsarwk"

#define OSC_HOST "192.168.1.201"
#define OSC_PORT 12345

#define MAX_FONT_SIZE 64
#define MIN_FONT_SIZE 16

#define READ_FROM_XML 1
#define XML_FILE_NAME "testTweetXml.xml"

//--------------------------------------------------------------
testApp::testApp() :
tweetGui(0,0,ofGetWidth()/4,ofGetHeight()),
buttonGui(ofGetWidth()/4+20,0.8*ofGetHeight(),0.75*ofGetWidth(),0.2*ofGetHeight()),
ofBaseApp(){ }

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofBackgroundHex(0x00);

	/////// TWITTER API
	vector<Tweet> theTweets;
	ofxXmlSettings xmlTweets;
	mTwitter.setup(CONSUMER_KEY,CONSUMER_SECRET);

	if(READ_FROM_XML){
		if(xmlTweets.loadFile(XML_FILE_NAME)){
			int numberOfSavedPoints = xmlTweets.getNumTags("tweet");

			// read from xml file
			for(int i = 0; i < numberOfSavedPoints; i++){
				xmlTweets.pushTag("tweet", i);
				string user, date, text, id_;
				long long int id;
				user = xmlTweets.getValue("user","");
				date = xmlTweets.getValue("date","");
				text = xmlTweets.getValue("text","");
				id_ = xmlTweets.getValue("id","");
				istringstream cur(id_);
				cur >> id;
				theTweets.push_back(Tweet(text, user, date, id));
				xmlTweets.popTag();
			}
		}
	}
	else{
		theTweets = mTwitter.getTweets();

		// write out to xml file
		for(int i=0; i<theTweets.size(); ++i){
			xmlTweets.addTag("tweet");
			xmlTweets.pushTag("tweet",i);
			xmlTweets.addValue("text", theTweets.at(i).text);
			xmlTweets.addValue("user", theTweets.at(i).user);
			xmlTweets.addValue("date", theTweets.at(i).date);
			xmlTweets.addValue("id", ofToString(theTweets.at(i).id));
			xmlTweets.popTag();
		}
		xmlTweets.saveFile(XML_FILE_NAME);
	}

	ofAddListener(ofxBaseTwitterApi::liveTweetEvent, this, &testApp::sendLiveTweet);

	////////// TWEET GUI
	tweetGui.setFont("verdana.ttf");
	tweetGui.setScrollableDirections(false, true);

	tweetGui.addWidgetDown(new ofxUILabel("Tweets", OFX_UI_FONT_MEDIUM));
	tweetGui.addSpacer(tweetGui.getRect()->width,4);

	for(int i=0;i<theTweets.size();i++){
		string tweetText = fitStringToWidth(theTweets.at(i).text, tweetGui.getRect()->width-10, *tweetGui.getFontMedium());
		tweetGui.addLabelButton(tweetText, false, 0);
	}

	tweetGui.setColorBack(ofColor(100,200));
	tweetGui.autoSizeToFitWidgets();
	ofAddListener(tweetGui.newGUIEvent,this,&testApp::tweetGuiEvent);

	////////// BUTTON GUI
	vector<string> fontItems;
	fontItems.push_back("verdana.ttf");
	fontItems.push_back("georgia.ttf");
	fontItems.push_back("arial.ttf");

	buttonGui.setFont("verdana.ttf");
	buttonGui.addWidgetDown(new ofxUILabel("Control Panel", OFX_UI_FONT_MEDIUM));
	buttonGui.addSpacer(buttonGui.getRect()->width,4);
	buttonGui.addSpacer(0.4*buttonGui.getRect()->width,0, "SPACER0");
	float buttonHeight = buttonGui.getRect()->height/4.0;
	buttonGui.addWidgetDown(new ofxUIToggle("SEND ONE WORD",false,buttonHeight,buttonHeight));
	buttonGui.addWidgetDown(new ofxUILabelButton("Send", false));
	buttonGui.addWidgetRight(new ofxUILabelButton("Clear",false));
	buttonGui.addWidgetRight(new ofxUILabelButton("Clear Live",false));
	buttonGui.addWidgetRight(new ofxUILabelButton("Clear All",false));

	ofxUIDropDownList *ddlist = (ofxUIDropDownList *) buttonGui.addWidgetEastOf(new ofxUIDropDownList("__FONT__", fontItems), "SPACER0");
	ofxUISlider *slide = (ofxUISlider *) buttonGui.addWidgetEastOf(new ofxUISlider("SIZE",MIN_FONT_SIZE,MAX_FONT_SIZE,MAX_FONT_SIZE/2,
											  buttonGui.getRect()->width/3,buttonHeight),"__FONT__");

	// update local variables with default values
	if(ddlist->getToggles().size()){
		oscFontName = ddlist->getToggles()[0]->getName();
	}
	oscFontSize = slide->getScaledValue();
	bWordByWord = false;

	buttonGui.setColorBack(ofColor(100,200));
	ofAddListener(buttonGui.newGUIEvent,this,&testApp::buttonGuiEvent);

	////////// text bar
	mTSB.setup(ofGetWidth()/4+20, 0, 0.75*ofGetWidth(), 40, "#elabismo no nos detiene");

	////////// draw area
	drawArea = ofRectangle(ofGetWidth()/4+20, 50, 0.75*ofGetWidth(), 0.8*ofGetHeight()-60);
	staticSendPosition = ofVec2f(drawArea.x,drawArea.y);
	lastStaticSendPosition = ofVec2f(drawArea.x,drawArea.y);

	////////// osc
	sender.setup(OSC_HOST,OSC_PORT);
	oscFont.loadFont(oscFontName, oscFontSize, true, true);

	//////////// graph
	//testGraphSetup();
}

//--------------------------------------------------------------
void testApp::update(){
	//testGraphUpdate();
	if(ofGetFrameNum()%100 == 0){
		cout << ofGetFrameRate() << endl;
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	mTSB.draw();
	ofSetColor(100,100);
	ofRect(drawArea);
	
	// DEBUG / TEST
	ofSetColor(255,200);
	for(unsigned int i=0; i<mTextStack.size(); i++){
		TextObject mto = mTextStack[i];
		oscFont.loadFont(mto.font, mto.size, true, true);
		oscFont.drawString(mto.text, mto.pos.x*drawArea.width+drawArea.x, mto.pos.y*drawArea.height+drawArea.y);
	}
	// live tweet
	TextObject mto = mTextStack.getLive();
	oscFont.drawString(mto.text, mto.pos.x*drawArea.width+drawArea.x, mto.pos.y*drawArea.height+drawArea.y);

	// draw text area rectangles
	ofSetColor(200,32,50,128);
	ofRect(staticTweetArea);
	ofSetColor(130,164,140,128);
	ofRect(liveTweetArea);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	if(drawArea.inside(x, y)){
		if(button == 0){
			staticTweetArea.width = x-staticTweetArea.x;
			staticTweetArea.height = y-staticTweetArea.y;
		}
		else {
			liveTweetArea.width = x-liveTweetArea.x;
			liveTweetArea.height = y-liveTweetArea.y;
		}
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	if(drawArea.inside(x, y)){
		if(button == 0){
			staticTweetArea.x = x;
			staticTweetArea.y = y;
			staticTweetArea.width = 0;
			staticTweetArea.height = 0;
			staticSendPosition = ofVec2f(staticTweetArea.x,staticTweetArea.y);
		}
		else {
			liveTweetArea.x = x;
			liveTweetArea.y = y;
			liveTweetArea.width = 0;
			liveTweetArea.height = 0;
		}
	}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}

//--------------------------------------------------------------
void testApp::sendLiveTweet(Tweet& t){
	// add to gui
	string tweetText = fitStringToWidth(t.text, tweetGui.getRect()->width-10, *tweetGui.getFontMedium());
	tweetGui.addLabelButton(tweetText, false, 0);
	tweetGui.autoSizeToFitWidgets();

	// add to osc/stack
	if(liveTweetArea.width>0){
		cout << "sending to osc: " << t.text << endl;
		ofxOscMessage m;
		m.setAddress("/kinho/setLive");
		m.addFloatArg((liveTweetArea.x-drawArea.x)/drawArea.width);
		m.addFloatArg((liveTweetArea.y-drawArea.y)/drawArea.height);
		m.addStringArg(oscFontName);
		m.addIntArg(oscFontSize);
		string sizedText = fitStringToWidth(t.text, liveTweetArea.width, oscFont);
		m.addStringArg(sizedText);
		sender.sendMessage(m);
		// DEBUG
		ofVec2f tempPos = ofVec2f((liveTweetArea.x-drawArea.x)/drawArea.width, (liveTweetArea.y-drawArea.y)/drawArea.height);
		//mTextStack.pushObject(TextObject(tempPos, oscFontSize, oscFontName,sizedText));
		mTextStack.setLive(TextObject(tempPos, oscFontSize, oscFontName,sizedText));
	}
}

//--------------------------------------------------------------
void testApp::buttonGuiEvent(ofxUIEventArgs &e){
	string name = e.widget->getName();

	if((name.compare("SIZE") == 0) && ofGetMousePressed()){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		oscFontSize = (int)slider->getScaledValue();
		oscFont.loadFont(oscFontName, oscFontSize, true, true);
	}
	else if(name.compare("__FONT__") == 0){
		ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
		if(ddlist->getSelected().size()){
			oscFontName = ddlist->getSelected()[0]->getName();
			oscFont.loadFont(oscFontName, oscFontSize, true, true);
		}
	}
	else if((name.compare("Clear") == 0) && ((ofxUIButton*)e.widget)->getValue()){
		ofxOscMessage m;
		m.setAddress("/kinho/pop");
		m.addIntArg(1);
		sender.sendMessage(m);
		staticSendPosition = ofVec2f(lastStaticSendPosition);
		// DEBUG
		mTextStack.popObject();
	}
	else if((name.compare("Clear Live") == 0) && ((ofxUIButton*)e.widget)->getValue()){
		ofxOscMessage m;
		m.setAddress("/kinho/clearLive");
		m.addIntArg(1);
		sender.sendMessage(m);
		// DEBUG
		mTextStack.clearLive();
	}
	else if((name.compare("Clear All") == 0) && ((ofxUIButton*)e.widget)->getValue()){
		ofxOscMessage m;
		m.setAddress("/kinho/clear");
		m.addIntArg(1);
		sender.sendMessage(m);
		staticSendPosition = ofVec2f(staticTweetArea.x,staticTweetArea.y);
		lastStaticSendPosition = ofVec2f(staticTweetArea.x,staticTweetArea.y);
		// DEBUG
		mTextStack.clearObjects();
	}
	else if((name.compare("Send") == 0) && (staticTweetArea.width>0) && (((ofxUIButton*)e.widget)->getValue())){
		string sendText = "";
		ofVec2f scaledPos( ((staticSendPosition.x-drawArea.x)/drawArea.width), ((staticSendPosition.y-drawArea.y)/drawArea.height) );
		lastStaticSendPosition = ofVec2f(staticSendPosition);

		if(bWordByWord){
			sendText = mTSB.consumeOneWord();
			// calculate offset
			if((staticSendPosition.x+oscFont.stringWidth(sendText)) > (staticTweetArea.x+staticTweetArea.width)){
				staticSendPosition.x = staticTweetArea.x;
				staticSendPosition.y += oscFont.stringHeight(sendText);
			}
			scaledPos.x = (staticSendPosition.x-drawArea.x)/drawArea.width;
			scaledPos.y = (staticSendPosition.y-drawArea.y)/drawArea.height;
			staticSendPosition.x += oscFont.stringWidth(sendText) + oscFont.stringWidth("p");
		}
		// send whole text at once
		else{
			sendText = fitStringToWidth(mTSB.getSelectedText(), staticTweetArea.width, oscFont);
			staticSendPosition.y += oscFont.stringHeight(sendText);
		}

		// don't send empty messages
		if(sendText.compare("") != 0){
			ofxOscMessage m;
			m.setAddress("/kinho/push");
			m.addFloatArg(scaledPos.x);
			m.addFloatArg(scaledPos.y);
			m.addStringArg(oscFontName);
			m.addIntArg(oscFontSize);
			m.addStringArg(sendText);
			sender.sendMessage(m);
			// DEBUG
			mTextStack.pushObject(TextObject(scaledPos, oscFontSize, oscFontName,sendText));
		}
	}
	else if(name.compare("SEND ONE WORD") == 0) {
		bWordByWord = ((ofxUIToggle*)e.widget)->getValue();
	}
}

//--------------------------------------------------------------
void testApp::tweetGuiEvent(ofxUIEventArgs &e){
	int kind = e.widget->getKind();

	if(kind == OFX_UI_WIDGET_LABELBUTTON) {
        ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
		if(button->getValue()){
			string tweetText = button->getLabel()->getLabel();
			mTSB.setText(tweetText);
		}
    }
}

// doesn't exclude empty words
string testApp::fitStringToWidth(const string s, const int w, ofTrueTypeFont ttf){
	string retStr="";
	istringstream ss( s );
	while (!ss.eof()){
		string word;
		getline(ss,word,' ');
		if(ttf.stringWidth(retStr+" "+word) > w){
			retStr += "\n";
		}
		else if(retStr.size() != 0){
			retStr += " ";
		}
		retStr += word;
	}
	return retStr;
}

//--------------------------------------------------------------
void testApp::testGraphSetup(){
	vector<Node*> someNodes;
	int numNodes = 100;
	int numEdges = 800;
	for(int i=0; i<numNodes; ++i){
		Node *n = new Node("v"+ofToString(i));
		someNodes.push_back(n);
	}
	
	for(int i=0; i<numEdges; ++i){
		int edgeCost = (int)(ofRandom(2)+1);
		string edgeType = (edgeCost<2)?"cat":"tag";
		Edge *e = new Edge(edgeType+ofToString(i), edgeCost);
		int npe = (int)ofRandom(numEdges/numNodes);
		for(int j=0; j<npe; ++j){
			// pick random node
			Node *n = someNodes.at((int)ofRandom(someNodes.size()));
			n->addEdge(e);
		}
	}
	
	Node *n0 = someNodes.at((int)ofRandom(someNodes.size()));
	long long unsigned int t0 = AbsoluteToDuration(UpTime());
	mGraph.calculateDists(*n0);
	long long unsigned int et = AbsoluteToDuration(UpTime())-t0;
	cout << "calculated from: " << n0->getName()+ " in: " << et << " millis"<<endl;
	mGraph.orderGraph();
	mGraph.printGraph();
}

void testApp::testGraphUpdate(){
	mGraph.calculateDists();
	long long unsigned int t0 = AbsoluteToDuration(UpTime());
	mGraph.orderGraph();
	long long unsigned int et = AbsoluteToDuration(UpTime())-t0;
	cout << "ordered graph in: " << et << " millis"<<endl;
}
