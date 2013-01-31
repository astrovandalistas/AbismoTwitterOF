//
//  WordCloud.h
//  AbismoTwitterOF
//
//  Created by thiago on 1/31/13.
//
//

#pragma once

#include "GraphUtils.h"
#include <set>

class WordCloud {
	public:
		WordCloud();
		~WordCloud();
	private:
		vector<set<Node*> > theGroups;
		int groupSize;
};
