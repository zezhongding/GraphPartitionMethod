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

bool Edge::operator==(const Edge& other) const
{
	return (this->srcVId == other.getSrcVId() && this->destVId == other.getDestVId());// && this->weight == other.getWeight());
}

/* 为 Edge 设计一个 hash 函数，供 unordered_set 使用 */


size_t EdgeHash::operator()(const Edge& edge) const {
	// 采用组合散列的方法
	size_t h1 = std::hash<unsigned int>()(edge.getSrcVId());
	size_t h2 = std::hash<unsigned int>()(edge.getDestVId());
	// 合并两个哈希值，这里使用了位运算来组合它们
	return h1 ^ (h2 << 1);
}
