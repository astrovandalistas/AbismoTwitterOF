//
//  GraphUtils.cpp
//  AbismoTwitterOF
//
//  Created by thiago on 1/23/13.
//
//

#include "GraphUtils.h"

Node::Node(const string name_){
	name = name_;
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

Edge::Edge(const string name_, const int cost_){
	name = name_;
	cost = cost_;
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

Graph::Graph(){
	ofAddListener(Edge::addNodeToQ, this, &Graph::addNodeToQ);
}
Graph::~Graph(){}

void Graph::addNode(Node& n){
	addNode(&n);
}
void Graph::addNode(Node* n){
	// add to node lists...
}

void Graph::addNodeToQ(Node& n){
	// add to Q
	theQ.push(&n);
}

void Graph::calculateDists(Node* fromNode) const{
	// push root.
	// calculate dists.

}


