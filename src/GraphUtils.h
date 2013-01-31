//
//  GraphUtils.h
//  AbismoTwitterOF
//
//  Created by thiago on 1/23/13.
//
//

#pragma once

#include "ofConstants.h"
#include "ofRectangle.h"
#include "ofEvents.h"
#include<map>
#include<queue>

class Edge;

class PhysNode {
public:
	PhysNode();
	~PhysNode();
	void setAcceleration(const ofVec2f acc_);
	void setSize(const float size_);
	const string getName() const;
	const ofRectangle getBoundingBox() const;
	void update();
	ofEvent<PhysNode> NodeClickEvent;
protected:
	ofVec2f pos, vel, acc;
	float size;
	string name;
};

class Node: public PhysNode {
	public:
		Node(const string name_);
		~Node();
		void setDistance(float f);
		const float getDistance() const;
		const bool isInQ() const;
		void setInQ(const bool q);
		void process() const;
		void addEdge(Edge* e);
		static ofEvent<Node> addNodeToGraph;
		bool operator < (const Node &on) const;
		static bool sortComp(Node *n0, Node *n1);
	protected:
		float distance;
		map<string, Edge*> theEdges;
		bool bInQ;
};

class Edge: public PhysNode {
public:
	Edge(const string name_, const int cost_);
	~Edge();
	void setCost(const float td);
	void resetMinCost();
	float getCost() const;
	void addNode(Node* n);
	static ofEvent<Node> addNodeToQ;
	static ofEvent<Edge> addEdgeToGraph;
	bool operator < (const Edge &oe) const;
	static bool sortComp(Edge *e0, Edge *e1);
protected:
	float minCost, cost, avgCost;
	map<string,Node*> theNodes;
};

class Graph {
public:
	Graph();
	~Graph();
	void addNodeToGraph(Node& n);
	void addEdgeToGraph(Edge& e);
	void addNodeToQ(Node& n);
	void calculateDists(Node& fromNode);
	void orderGraph();
	// debug
	void printGraph() const;
	void calculateDists();
protected:
	map<string, Node*> theNodes;
	map<string, Edge*> theEdges;
	vector<Node*> orderedNodes;
	vector<Edge*> orderedEdges;
	queue<Node*> theQ;
};


