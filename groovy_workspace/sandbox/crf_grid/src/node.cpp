#include "node.h"

node::node () {
	nodeValue = 0.5;
	neighbours = new std::vector<node *>();
	fixed = false;
};

node::node(std::vector<int> coordinates) {
	nodeValue = 0.5;
	neighbours = new std::vector<node*>();
	this->coordinates = coordinates;
	fixed = false;
};

std::vector<node *> * node::getNeighbours() {
	return this->neighbours;
};
void node::addNeighbour(node * neighbour) {
	this->neighbours->push_back(neighbour);
};

float node::getValue() {
	return this->nodeValue;
};

std::vector<int> node::getCoords() {
	return this->coordinates;
};

void node::setTempValue(float tempVal) {
	this->__LBPTEMPVALUE = tempVal;
};
bool node::pushTempValue() {
	if (fixed) {
		return false;
	}
	else {
		//if (__LBPTEMPVALUE > 2) {
		//	this->nodeValue = 2;
		//}
		//else {
		//	this->nodeValue = this->__LBPTEMPVALUE;
		//}		
		this->nodeValue = this->__LBPTEMPVALUE; //for debugging
		return true;
	}
}; 

void node::setFixedValue(float value) {
	this->fixed = true;
	this->nodeValue = value;
};

bool node::setValue(float value) {
	if (fixed) {
		return false;
	}
	else {
		//if (value > 2) {
		//	this->nodeValue = 2;
		//}
		//else {
			this->nodeValue = value;
		//}
		return true;
	}
};

std::vector<int>	node::getCoordinates() {
	return this->coordinates;
}