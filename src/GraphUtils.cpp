//
//  GraphUtils.cpp
//  AbismoTwitterOF
//
//  Created by thiago on 1/23/13.
//
//

#include "GraphUtils.h"

ofEvent<Node> Node::addNodeToGraph = ofEvent<Node>();

Node::Node(const string name_){
	name = name_;
	ofNotifyEvent(Node::addNodeToGraph, *this);
}

Node::~Node(){}

void Node::setDistance(int d){
	distance = d;
}
const int Node::getDistance() const{
	return distance;
}
const string Node::getName() const{
	return name;
}
const bool Node::isInQ() const{
	return bInQ;
}
void Node::setInQ(const bool q){
	bInQ = q;
}

void Node::process() const{
	// send my distance to all edges
	for (map<string,Edge*>::const_iterator it=theEdges.begin(); it!=theEdges.end(); ++it){
		(it->second)->setCost(distance);
	}
}

// add edge to map. use name as key.
void Node::addEdge(Edge* e){
	if(theEdges.find(e->getName()) != theEdges.end()){
		theEdges[e->getName()] = e;
		// link back to node...
		e->addNode(this);
	}
}
void Node::addEdge(Edge& e){
	addEdge(&e);
}

//////////////////////////////////
//////////////////////////////////
//////////////////////////////////
ofEvent<Node> Edge::addNodeToQ = ofEvent<Node>();
ofEvent<Edge> Edge::addEdgeToGraph = ofEvent<Edge>();

Edge::Edge(const string name_, const int cost_){
	name = name_;
	cost = cost_;
	ofNotifyEvent(Edge::addEdgeToGraph, *this);
}
Edge::~Edge(){}

void Edge::setCost(const int td){
	// if there's a shorter way to get here, update minCost
	if(td+cost < minCost){
		minCost = td+cost;
		
		// send new cost to all nodes
		for (map<string,Node*>::iterator it=theNodes.begin(); it!=theNodes.end(); ++it){
			if(minCost+cost < (it->second)->getDistance()){
				(it->second)->setDistance(minCost+cost);
				// Add node to graph Q
				(it->second)->setInQ(true);
				ofNotifyEvent(Edge::addNodeToQ, *(it->second));
			}
		}
	}
}

string Edge::getName() const{
	return name;
}

void Edge::addNode(Node& n){
	addNode(&n);
}
void Edge::addNode(Node* n){
	if(theNodes.find(n->getName()) != theNodes.end()){
		theNodes[n->getName()] = n;
	}
}

//////////////////////////////////////
//////////////////////////////////////
//////////////////////////////////////

// adds every created node and edge to graph using listeners. ????
Graph::Graph(){
	ofAddListener(Edge::addNodeToQ, this, &Graph::addNodeToQ);
	ofAddListener(Node::addNodeToGraph, this, &Graph::addNodeToGraph);
	ofAddListener(Edge::addEdgeToGraph, this, &Graph::addEdgeToGraph);
}
Graph::~Graph(){}

void Graph::addNodeToGraph(Node& n){
	theNodes[n.getName()] = &n;
}
void Graph::addEdgeToGraph(Edge& e){
	theEdges[e.getName()] = &e;
}

void Graph::addNodeToQ(Node& n){
	// add to Q
	theQ.push(&n);
}

void Graph::calculateDists(Node* fromNode){
	// push root.
	// calculate dists.
	theQ.push(fromNode);

	while(!theQ.empty()){
		Node n = *(theQ.front());
		n.setInQ(false);
		theQ.pop();
		n.process();
	}
}

