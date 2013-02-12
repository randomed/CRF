#include "node.h"
using namespace std;
void node::setTempValue(float tempVal) {
	this->__LBPTEMPVALUE = tempVal;
};
bool node::pushTempValue() {
	if (fixed) {
		return false;
	}
	else {
		if (__LBPTEMPVALUE > 2) {
			this->nodeValue = 2;
		}
		else {
			this->nodeValue = this->__LBPTEMPVALUE;
		}		
		//this->nodeValue = this->__LBPTEMPVALUE; //for debugging
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
		if (value > 2) {
			this->nodeValue = 2;
		}
		else {
			this->nodeValue = value;
		}
		return true;
	}
};