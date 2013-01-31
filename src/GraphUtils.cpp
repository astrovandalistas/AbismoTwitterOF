//
//  GraphUtils.cpp
//  AbismoTwitterOF
//
//  Created by thiago on 1/23/13.
//
//

#include "GraphUtils.h"
#include "ofAppRunner.h"
#include "ofLog.h"

#define GROUP_SIZE 100

PhysNode::PhysNode(){
	size = 0;
	name = "";
	pos = ofVec2f(0,0);
	vel = ofVec2f(0,0);
	acc = ofVec2f(0,0);
}
PhysNode::~PhysNode(){}

void PhysNode::setAcceleration(const ofVec2f acc_){
	acc = acc_;
}
void PhysNode::setSize(const float size_){
	size = size_;
}
const string PhysNode::getName() const{
	return name;
}

const ofRectangle PhysNode::getBoundingBox() const{
	ofRectangle r = ofRectangle(pos.x-size/2, pos.y-size/2, size, size);
}

void PhysNode::update(){
	vel += acc;
	pos += vel;
}

//////////////////////////////////
//////////////////////////////////
//////////////////////////////////
ofEvent<Node> Node::addNodeToGraph = ofEvent<Node>();

Node::Node(const string name_){
	// TODO: super(); ???
	name = name_;
	distance = 1e9;
	ofNotifyEvent(Node::addNodeToGraph, *this);
}

Node::~Node(){}

bool Node::operator < (const Node &on) const{
	return distance < on.distance;
}

bool Node::sortComp(Node *n0, Node *n1){
	return *n0 < *n1;
}

void Node::setDistance(float f){
	distance = f;
}
const float Node::getDistance() const{
	return distance;
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

//////////////////////////////////
//////////////////////////////////
//////////////////////////////////
ofEvent<Node> Edge::addNodeToQ = ofEvent<Node>();
ofEvent<Edge> Edge::addEdgeToGraph = ofEvent<Edge>();

Edge::Edge(const string name_, const int cost_){
	// TODO: super(); ???
	name = name_;
	cost = cost_;
	minCost = 1e9;
	ofNotifyEvent(Edge::addEdgeToGraph, *this);
}
Edge::~Edge(){}

bool Edge::operator < (const Edge &oe) const{
	return minCost < oe.minCost;
}

bool Edge::sortComp(Edge *e0, Edge *e1){
	return *e0 < *e1;
}

void Edge::setCost(const float td){
	// if there's a shorter way to get here, update minCost
	if(td+avgCost < minCost){
		minCost = td+avgCost;
		
		// send new cost to all nodes
		for (map<string,Node*>::const_iterator it=theNodes.begin(); it!=theNodes.end(); ++it){
			if(minCost+avgCost < (it->second)->getDistance()){
				(it->second)->setDistance(minCost+avgCost);
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

float Edge::getCost() const{
	return minCost;
}

void Edge::addNode(Node* n){
	if(theNodes.find(n->getName()) == theNodes.end()){
		theNodes[n->getName()] = n;
		avgCost = cost/theNodes.size();
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
	//
	collisionGroupSize = GROUP_SIZE;
	// init sets
	for(int i=0; i<(ofGetHeight()*ofGetWidth())/(collisionGroupSize*collisionGroupSize); ++i){
		collisionGroups.push_back(set<Node*>());
	}
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
	orderedNodes.push_back(&n);
}
void Graph::addEdgeToGraph(Edge& e){
	theEdges[e.getName()] = &e;
	orderedEdges.push_back(&e);
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

void Graph::orderGraph(){
	// check sizes
	if(orderedNodes.size() != theNodes.size()){
		ofLogWarning("orderedNodes and theNodes are inconsistent");
		orderedNodes.clear();
		for (map<string,Node*>::const_iterator it=theNodes.begin(); it!=theNodes.end(); ++it){
			orderedNodes.push_back(it->second);
		}
	}
	if(orderedEdges.size() != theEdges.size()){
		ofLogWarning("orderedEdges and theEdges are inconsistent");
		orderedEdges.clear();
		for (map<string,Edge*>::const_iterator it=theEdges.begin(); it!=theEdges.end(); ++it){
			orderedEdges.push_back(it->second);
		}
	}
	// sort
	sort(orderedNodes.begin(), orderedNodes.end(), Node::sortComp);
	sort(orderedEdges.begin(), orderedEdges.end(), Edge::sortComp);
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
		long long unsigned int t0 = AbsoluteToDuration(UpTime());
		calculateDists(*n0);
		long long unsigned int et = AbsoluteToDuration(UpTime())-t0;
		cout << "calculated from: " << n0->getName()+ " in: " << et << " millis"<<endl;
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
