#include "Edge.h"

Edge::Edge() {
	this->srcVId = -1;
	this->destVId = -1;
	this->weight = -1;
}

bool Edge::isNull() const {
	return ((this->srcVId < 0) && (this->destVId < 0) && (this->weight < 0));
}

Edge::Edge(const int srcVId, const int destVId, const int weight) {
	this->srcVId = srcVId;
	this->destVId = destVId;
	this->weight = weight;
}

int Edge::getSrcVId() const {
	return srcVId;
}

int Edge::getDestVId() const {
	return destVId;
}

int Edge::getWeight() const {
	return weight;
}

void Edge::addWeight() {
	weight++;
}
