#include "testApp.h"

#define CONSUMER_KEY "b7BrQXjIRKT8MhEkWnhQ"
#define CONSUMER_SECRET "bx9rw5bsAb4K5WPmiT2UIc2j8Kmo5JVrlLHw9qpToh4"

#define OSC_HOST "172.20.10.3"
#define OSC_PORT 12345

//--------------------------------------------------------------
testApp::testApp() : gui(0,0,ofGetWidth()/4,ofGetHeight()), ofBaseApp(){ }

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofBackgroundHex(0xffff00);
	myFont.loadFont("verdana.ttf",12);
	myTwitter.setup(CONSUMER_KEY,CONSUMER_SECRET);
	ofEventArgs voidEventArg;
	myTwitter.update(voidEventArg);

	vector<Tweet> theTweets = myTwitter.getTweets();

	gui.setFont("verdana.ttf");
    gui.setScrollableDirections(false, true);

	gui.addWidgetDown(new ofxUILabel("Live Tweets", OFX_UI_FONT_MEDIUM));
	gui.addLabelToggle("Enabled", false);
	gui.addSpacer(gui.getRect()->width,4);
	gui.addWidgetDown(new ofxUILabel("Tweets", OFX_UI_FONT_MEDIUM));
	
	for(int i=0;i<theTweets.size();i++){
		string tweetText = fitStringToWidth(theTweets.at(i).text, gui.getRect()->width, *gui.getFontMedium());
		gui.addLabelButton(tweetText, false, 0);
	}
	gui.autoSizeToFitWidgets();
	ofAddListener(gui.newGUIEvent,this,&testApp::guiEvent);
	
	////////// osc
	sender.setup(OSC_HOST,OSC_PORT);

	//////////// graph
	//Graph myGraph;
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
	//myGraph.printGraph();
	cout << "calculated from: " << n0->getName()+ " in: " << et << " millis"<<endl;
}

//--------------------------------------------------------------
void testApp::update(){
	ofSetHexColor(0xff00ff);
	//myGraph.calculateDists();
}

//--------------------------------------------------------------
void testApp::draw(){

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
void testApp::guiEvent(ofxUIEventArgs &e){
	string name = e.widget->getName();
	int kind = e.widget->getKind();

	if(kind == OFX_UI_WIDGET_LABELBUTTON) {
        ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
		if(button->getValue()){
			string tweetText = button->getLabel()->getLabel();
			cout << "sending to osc: " << tweetText << endl;
			ofxOscMessage m;
			m.setAddress("/kinho/tweet");
			m.addStringArg(tweetText);
			sender.sendMessage(m);
		}
    }
    else if(kind == OFX_UI_WIDGET_LABELTOGGLE) {
        ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
        cout << name << "\t value: " << toggle->getValue() << endl;
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
