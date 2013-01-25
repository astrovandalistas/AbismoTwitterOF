#include "testApp.h"

#define CONSUMER_KEY "b7BrQXjIRKT8MhEkWnhQ"
#define CONSUMER_SECRET "bx9rw5bsAb4K5WPmiT2UIc2j8Kmo5JVrlLHw9qpToh4"

#define OSC_HOST "172.20.10.3"
#define OSC_PORT 12345

//--------------------------------------------------------------
testApp::testApp() :
tweetGui(0,0,ofGetWidth()/4,ofGetHeight()),
buttonGui(ofGetWidth()/4,4.0*ofGetHeight()/5,3.0*ofGetWidth()/4,ofGetHeight()/5),
ofBaseApp(){ }

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofBackgroundHex(0xffff00);
	myFont.loadFont("verdana.ttf",12);
	myTwitter.setup(CONSUMER_KEY,CONSUMER_SECRET);
	ofEventArgs voidEventArg;
	myTwitter.update(voidEventArg);

	bSendLiveTweets = false;
	ofAddListener(ofxBaseTwitterApi::liveTweetEvent, this, &testApp::sendLiveTweet);

	vector<Tweet> theTweets = myTwitter.getTweets();

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
	tweetGui.autoSizeToFitWidgets();
	ofAddListener(tweetGui.newGUIEvent,this,&testApp::tweetGuiEvent);

	////////// BUTTON GUI
	vector<string> fontItems;
	fontItems.push_back("FONT_0");
	fontItems.push_back("FONT_1");
	fontItems.push_back("FONT_2");

	buttonGui.setFont("verdana.ttf");
	buttonGui.addWidgetDown(new ofxUILabel("Control Panel", OFX_UI_FONT_MEDIUM));
	buttonGui.addSpacer(buttonGui.getRect()->width,4);
	buttonGui.addSpacer(buttonGui.getRect()->width*2.0/5.0,0, "SPACER0");
	float buttonHeight = buttonGui.getRect()->height/4;
	buttonGui.addWidgetDown(new ofxUIToggle("Increment Word Pos",false,buttonHeight,buttonHeight));
	buttonGui.addWidgetDown(new ofxUILabelButton("Send", false));
	//gui->addWidgetRight(new ofxUIButton("Fade In", false));
    buttonGui.addWidgetRight(new ofxUILabelButton("Clear",false));
	buttonGui.addWidgetRight(new ofxUILabelButton("Clear All",false));

    buttonGui.addWidgetEastOf(new ofxUIDropDownList("__FONT__", fontItems), "SPACER0");
	buttonGui.addWidgetEastOf(new ofxUISlider("SIZE",16,64,32,buttonGui.getRect()->width/3,buttonHeight),"__FONT__");

	////////// text bar
	mTSB.setup(ofGetWidth()/4, 0, 3.0*ofGetWidth()/4, 40,
			   "Calculate but display the number of characters within a text with this script. Useful, for example, in helping your visitors keep their hat.");

	////////// osc
	sender.setup(OSC_HOST,OSC_PORT);

	//////////// graph
	//testGraphSetup();
}

//--------------------------------------------------------------
void testApp::update(){
	ofSetHexColor(0xff00ff);
	//testGraphUpdate();
}

//--------------------------------------------------------------
void testApp::draw(){
	mTSB.draw();
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

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

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
	if(bSendLiveTweets){
		cout << "sending to osc: " << t.text << endl;
		ofxOscMessage m;
		m.setAddress("/kinho/live");
		m.addStringArg(t.text);
		sender.sendMessage(m);
	}
}

//--------------------------------------------------------------
void testApp::buttonGuiEvent(ofxUIEventArgs &e){
	// TODO : fill this out
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
	myGraph.calculateDists(*n0);
	long long unsigned int et = AbsoluteToDuration(UpTime())-t0;
	cout << "calculated from: " << n0->getName()+ " in: " << et << " millis"<<endl;
	myGraph.orderGraph();
	myGraph.printGraph();
}

void testApp::testGraphUpdate(){
	myGraph.calculateDists();
	long long unsigned int t0 = AbsoluteToDuration(UpTime());
	myGraph.orderGraph();
	long long unsigned int et = AbsoluteToDuration(UpTime())-t0;
	cout << "ordered graph in: " << et << " millis"<<endl;
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
			/*****
			 cout << "sending to osc: " << tweetText << endl;
			 ofxOscMessage m;
			 m.setAddress("/kinho/tweet");
			 m.addStringArg(tweetText);
			 sender.sendMessage(m);
			 ****/
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
