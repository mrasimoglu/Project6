#pragma once

#include "Mustafa.h"

#include <d3d11.h>
#include "SimpleMath.h"

#include "DXApp.h"
#include "StaticObject.h"

using namespace DirectX::SimpleMath;

typedef struct GraphNode
{
	Vector2 pos;
	int numOfLinkedNodes;
	GraphNode *(*linkedNodes);
};

class NavMesh
{
public:
	NavMesh(StaticObject **);
	~NavMesh();

	void DrawNodes();
	void DrawLinks(PrimitiveBatch<VertexPositionColor>* primitiveBatch);

private:
	int numOfNodes;
	GraphNode *nodes;
};

