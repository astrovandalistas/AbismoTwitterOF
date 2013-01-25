#include "testApp.h"

#define CONSUMER_KEY "b7BrQXjIRKT8MhEkWnhQ"
#define CONSUMER_SECRET "bx9rw5bsAb4K5WPmiT2UIc2j8Kmo5JVrlLHw9qpToh4"

#define OSC_HOST "192.168.1.201"
#define OSC_PORT 12345

#define MAX_FONT_SIZE 64
#define MIN_FONT_SIZE 16

//--------------------------------------------------------------
testApp::testApp() :
tweetGui(0,0,ofGetWidth()/4,ofGetHeight()),
buttonGui(ofGetWidth()/4,0.8*ofGetHeight(),0.75*ofGetWidth(),0.2*ofGetHeight()),
ofBaseApp(){ }

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofBackgroundHex(0x00);
	//mTwitter.setup(CONSUMER_KEY,CONSUMER_SECRET);
	ofEventArgs voidEventArg;
	//mTwitter.update(voidEventArg);

	bSendLiveTweets = false;
	ofAddListener(ofxBaseTwitterApi::liveTweetEvent, this, &testApp::sendLiveTweet);

	vector<Tweet> theTweets;
	//theTweets = mTwitter.getTweets();

	////////// TWEET GUI
	tweetGui.setFont("verdana.ttf");
	tweetGui.setScrollableDirections(false, true);
	
	tweetGui.addWidgetDown(new ofxUILabel("Live Tweets", OFX_UI_FONT_MEDIUM));
	tweetGui.addLabelToggle("Enabled", false);
	tweetGui.addSpacer(tweetGui.getRect()->width,4);
	tweetGui.addWidgetDown(new ofxUILabel("Tweets", OFX_UI_FONT_MEDIUM));

	for(int i=0;i<theTweets.size();i++){
		string tweetText = fitStringToWidth(theTweets.at(i).text, tweetGui.getRect()->width, *tweetGui.getFontMedium());
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
	//gui->addWidgetRight(new ofxUIButton("Fade In", false));
    buttonGui.addWidgetRight(new ofxUILabelButton("Clear",false));
	buttonGui.addWidgetRight(new ofxUILabelButton("Clear All",false));

	ofxUIDropDownList *ddlist = (ofxUIDropDownList *) buttonGui.addWidgetEastOf(new ofxUIDropDownList("__FONT__", fontItems), "SPACER0");
	ofxUISlider *slide = (ofxUISlider *) buttonGui.addWidgetEastOf(new ofxUISlider("SIZE",MIN_FONT_SIZE,MAX_FONT_SIZE,MAX_FONT_SIZE/2,
											  buttonGui.getRect()->width/3,buttonHeight),"__FONT__");

	// update local variables with default values
	if(ddlist->getToggles().size()){
		oscFontName = ddlist->getToggles()[0]->getName();
	}
	oscFontSize = slide->getScaledValue();

	buttonGui.setColorBack(ofColor(100,200));
	ofAddListener(buttonGui.newGUIEvent,this,&testApp::buttonGuiEvent);

	////////// text bar
	mTSB.setup(ofGetWidth()/4, 0, 0.75*ofGetWidth(), 40,
			   "Calculate and display the number of characters within a text with this script. Useful, for example, in helping your visitors keep their hat.");

	////////// draw area
	drawArea = ofRectangle(ofGetWidth()/4+20, 50, 0.75*ofGetWidth(), 0.8*ofGetHeight()-60);

	////////// osc
	sender.setup(OSC_HOST,OSC_PORT);
	oscFont.loadFont(oscFontName, oscFontSize);

	//////////// graph
	//testGraphSetup();
}

//--------------------------------------------------------------
void testApp::update(){
	//testGraphUpdate();
}

//--------------------------------------------------------------
void testApp::draw(){
	mTSB.draw();
	ofSetColor(100,100);
	ofRect(drawArea);
	ofSetColor(255,20,20,100);
	ofRect(tweetArea);
	ofSetColor(20,20,255,100);
	ofRect(liveArea);
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
			tweetArea.width = x-tweetArea.x;
			tweetArea.height = y-tweetArea.y;
		}
		else {
			liveArea.width = x-liveArea.x;
			liveArea.height = y-liveArea.y;
		}
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	if(drawArea.inside(x, y)){
		if(button == 0){
			tweetArea.x = x;
			tweetArea.y = y;
			tweetArea.width = 0;
			tweetArea.height = 0;
		}
		else {
			liveArea.x = x;
			liveArea.y = y;
			liveArea.width = 0;
			liveArea.height = 0;
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
	if(bSendLiveTweets && liveArea.width>0){
		cout << "sending to osc: " << t.text << endl;
		ofxOscMessage m;
		m.setAddress("/kinho/push");
		// TODO : add % float 
		m.addIntArg(liveArea.x);
		m.addIntArg(liveArea.y);
		m.addStringArg(oscFontName);
		m.addIntArg(oscFontSize);
		string sizedText = fitStringToWidth(t.text, liveArea.width, oscFont);
		m.addStringArg(sizedText);
		sender.sendMessage(m);
	}
}

//--------------------------------------------------------------
void testApp::buttonGuiEvent(ofxUIEventArgs &e){
	string name = e.widget->getName();
	int kind = e.widget->getKind();

    if(e.widget->getName().compare("SIZE") == 0){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		oscFontSize = (int)slider->getScaledValue();
		cout << "sisiisisis\n";
		oscFont.loadFont(oscFontName, oscFontSize);
	}
	else if(e.widget->getName().compare("__FONT__") == 0){
		ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
		if(ddlist->getSelected().size()){
			oscFontName = ddlist->getSelected()[0]->getName();
			cout << "fofofofofo\n";
			oscFont.loadFont(oscFontName, oscFontSize);
		}
	}
	else if((e.widget->getName().compare("Clear") == 0) && ((ofxUIButton*)e.widget)->getValue()){
		ofxOscMessage m;
		m.setAddress("/kinho/pop");
		m.addIntArg(1);
		sender.sendMessage(m);
	}
	else if((e.widget->getName().compare("Clear All") == 0) && ((ofxUIButton*)e.widget)->getValue()){
		ofxOscMessage m;
		m.setAddress("/kinho/clear");
		m.addIntArg(1);
		sender.sendMessage(m);
	}
	else if((e.widget->getName().compare("Send") == 0) && ((ofxUIButton*)e.widget)->getValue()){
		ofxOscMessage m;
		m.setAddress("/kinho/push");
		// TODO : add % float 
		m.addIntArg(tweetArea.x);
		m.addIntArg(tweetArea.y);
		m.addStringArg(oscFontName);
		m.addIntArg(oscFontSize);
		// TODO : add WORDxWORD conditional + logic
		string sizedText = fitStringToWidth("TEXT HOLDER", tweetArea.width, oscFont);
		m.addStringArg(sizedText);
		sender.sendMessage(m);
	}
}

//--------------------------------------------------------------
void testApp::tweetGuiEvent(ofxUIEventArgs &e){
	string name = e.widget->getName();
	int kind = e.widget->getKind();

	if(kind == OFX_UI_WIDGET_LABELBUTTON) {
        ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
		if(button->getValue()){
			string tweetText = button->getLabel()->getLabel();
			mTSB.setString(tweetText);
		}
    }
    else if(kind == OFX_UI_WIDGET_LABELTOGGLE) {
        ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
		if(toggle->getLabel()->getLabel().compare("Enabled") == 0){
			bSendLiveTweets = toggle->getValue();
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
