#ifndef EDGE_H
#define EDGE_H

#include "common.h"

class Edge
{
private:
	int srcVId;
	int destVId;
	int weight;
public:
	Edge();
	bool isNull() const;
	Edge(const int srcVId, const int destVId, const int weight);
	int getSrcVId() const; 
	int getDestVId() const;
	int getWeight() const;
	void addWeight();
};

#endif