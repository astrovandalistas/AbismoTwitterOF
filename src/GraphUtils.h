//
//  GraphUtils.h
//  AbismoTwitterOF
//
//  Created by thiago on 1/23/13.
//
//

#pragma once

#include "ofConstants.h"
#include "ofEvents.h"
#include<map>
#include<queue>

class Edge;

class Node {
	public:
		Node(const string name_);
		~Node();
		void setDistance(int d);
		const int getDistance() const;
		const string getName() const;
		const bool isInQ() const;
		void setInQ(const bool q);
		void process() const;
		void addEdge(Edge& e);
		void addEdge(Edge* e);
	protected:
		int distance;
		map<string, Edge*> theEdges;
		string name;
		bool bInQ;
};

class Edge {
public:
	Edge(const string name_, const int cost_);
	~Edge();
	void setCost(const int td);
	string getName() const;
	void addNode(Node& n);
	void addNode(Node* n);
	static ofEvent<Node> addNodeToQ;
protected:
	int minCost;
	int cost;
	map<string,Node*> theNodes;
	string name;
};

class Graph {
public:
	Graph();
	~Graph();
	void addNode(Node& n);
	void addNode(Node* n);
	void addNodeToQ(Node& n);
	void calculateDists(Node* fromNode) const;
protected:
	map<string, Node*> theNodes;
	queue<Node*> theQ;
	// listen for addNodeToQ
};


