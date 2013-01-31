//
//  WordCloud.cpp
//  AbismoTwitterOF
//
//  Created by thiago on 1/31/13.
//
//

#include "WordCloud.h"
#include "ofAppRunner.h"


#define GROUP_SIZE 100

WordCloud::WordCloud(){
	groupSize = GROUP_SIZE;
	// init sets
	for(int i=0; i<(ofGetHeight()*ofGetWidth())/(groupSize*groupSize); ++i){
		theGroups.push_back(set<Node*>());
	}
}
WordCloud::~WordCloud(){}

