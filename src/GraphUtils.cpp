//
//  GraphUtils.cpp
//  AbismoTwitterOF
//
//  Created by thiago on 1/23/13.
//
//

#include "GraphUtils.h"
#include "ofLog.h"

ofEvent<Node> Node::addNodeToGraph = ofEvent<Node>();

Node::Node(const string name_){
	name = name_;
	distance = 1e9;
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
	if(theEdges.find(e->getName()) == theEdges.end()){
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
	minCost = 1e9;
	ofNotifyEvent(Edge::addEdgeToGraph, *this);
}
Edge::~Edge(){}

void Edge::setCost(const int td){
	// if there's a shorter way to get here, update minCost
	if(td+cost < minCost){
		minCost = td+cost;

		// send new cost to all nodes
		for (map<string,Node*>::const_iterator it=theNodes.begin(); it!=theNodes.end(); ++it){
			if(minCost+cost < (it->second)->getDistance()){
				(it->second)->setDistance(minCost+cost);
				// Add node to graph Q
				(it->second)->setInQ(true);
				ofNotifyEvent(Edge::addNodeToQ, *(it->second));
			}
		}
	}
}

void Edge::resetMinCost(){
	minCost = 1e9;
}

string Edge::getName() const{
	return name;
}
int Edge::getCost() const{
	return minCost;
}

void Edge::addNode(Node& n){
	addNode(&n);
}
void Edge::addNode(Node* n){
	if(theNodes.find(n->getName()) == theNodes.end()){
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

Graph::~Graph(){
	for (map<string,Node*>::const_iterator it=theNodes.begin(); it!=theNodes.end(); ++it){
		delete it->second;
	}
	for (map<string,Edge*>::const_iterator it=theEdges.begin(); it!=theEdges.end(); ++it){
		delete it->second;
	}
}

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

void Graph::calculateDists(Node& fromNode){
	// clear costs from nodes
	for (map<string,Node*>::const_iterator it=theNodes.begin(); it!=theNodes.end(); ++it){
		(it->second)->setDistance(1e9);
	}
	// clear minDist from edges
	for (map<string,Edge*>::const_iterator it=theEdges.begin(); it!=theEdges.end(); ++it){
		(it->second)->resetMinCost();
	}

	// push root calculate dists.
	fromNode.setDistance(0);
	theQ.push(&fromNode);

	while(!theQ.empty()){
		Node n = *(theQ.front());
		n.setInQ(false);
		theQ.pop();
		n.process();
	}
}

// for debug
void Graph::calculateDists(){
	int mi = (int)ofRandom(theNodes.size());
	int i = 0;
	Node *n0 = NULL;
	for (map<string,Node*>::const_iterator it=theNodes.begin(); it!=theNodes.end()&&i<mi; ++it,++i){
		n0 = it->second;
	}
	if(n0){
		calculateDists(*n0);
	}
}

// for debug
void Graph::printGraph() const{
	ofLogWarning("Nodes:")<< "";
	for (map<string,Node*>::const_iterator it=theNodes.begin(); it!=theNodes.end(); ++it){
		ofLogWarning() << it->first << ": " << (it->second)->getDistance();
	}
	ofLogWarning("Edges:") << "";
	for (map<string,Edge*>::const_iterator it=theEdges.begin(); it!=theEdges.end(); ++it){
		ofLogWarning() << it->first << ": " << (it->second)->getCost();
	}
}
