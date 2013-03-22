#include "node.h"

node::node () {
	nodeValue = 0.5;
//	neighbours = new std::vector<node *>();
	fixed = false;
	this->variablesTracked = 2;
};

node::node(std::vector<int> coordinates) {
	nodeValue = 0.5;
//	neighbours = new std::vector<node*>();
	this->coordinates = coordinates;
	fixed = false;
	this->variablesTracked = 2;
};

std::map<node *, vector<float>> node::getNeighbours() {
	return this->neighbours;
};
void node::addNeighbour(node * neighbour) {
//	this->neighbours->push_back(neighbour);
//	this->neighbours[neighbour] = 0.5;
	this->neighbours[neighbour] = vector<float>(this->variablesTracked, 1);
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

int node::getVariablesTracked() {
	return this->variablesTracked;
};
