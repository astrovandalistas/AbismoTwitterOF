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
		static ofEvent<Node> addNodeToGraph;
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
	int getCost() const;
	void addNode(Node& n);
	void addNode(Node* n);
	static ofEvent<Node> addNodeToQ;
	static ofEvent<Edge> addEdgeToGraph;
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
	void addNodeToGraph(Node& n);
	void addEdgeToGraph(Edge& e);
	void addNodeToQ(Node& n);
	void calculateDists(Node* fromNode);
	// debug
	void printGraph() const;
protected:
	map<string, Node*> theNodes;
	map<string, Edge*> theEdges;
	queue<Node*> theQ;
};


