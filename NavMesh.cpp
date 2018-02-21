#include "NavMesh.h"



NavMesh::NavMesh(StaticObject **statics)
{
	numOfNodes = 0;

	int i = 0;
	while (statics[i] != nullptr)
	{
		numOfNodes += statics[i]->GetVertexs()->numOfVertex;
		i++;
	}

	nodes = (GraphNode*)malloc(numOfNodes * sizeof(GraphNode));

	i = 0;
	int counter = 0;
	while (statics[i] != nullptr)
	{
		int temp;
		Vertexs *ver = statics[i]->GetVertexs();
		for (int j = 0; j < ver->numOfVertex; j++)
		{
			int offset = ver->numOfVertex;

			nodes[counter + j].pos = (ver->vertex[j] + 10 * Collision::UnitVector(ver->vertex[j] - statics[i]->GetPosition()));
			
			nodes[counter + j].numOfLinkedNodes = 2;
			nodes[counter + j].linkedNodes = (GraphNode**)malloc(nodes[counter].numOfLinkedNodes * sizeof(GraphNode*));
			
			nodes[counter + j].linkedNodes[0] = &nodes[counter + ((j + 1) % ver->numOfVertex)];
			nodes[counter + j].linkedNodes[1] = &nodes[counter + ((((j - 1) % ver->numOfVertex) + ver->numOfVertex) % ver->numOfVertex)];
			
			temp = j;
		}
		i++;
		counter += temp + 1;
	}
}

void NavMesh::DrawNodes()
{
	SpriteSheet* sprite = new SpriteSheet();
	sprite->Load(DXApp::m_pDevice, L"img/test4.dds");

	for (int i = 0; i < numOfNodes; i++)
	{
		sprite->SetPosition(nodes[i].pos);
		sprite->Draw();
	}
}

void NavMesh::DrawLinks(PrimitiveBatch<VertexPositionColor>* primitiveBatch)
{
	for (int i = 0; i < numOfNodes; i++)
	{
		for (int j = 0; j < nodes[i].numOfLinkedNodes; j++)
		{
			primitiveBatch->DrawLine(VertexPositionColor(nodes[i].pos, Colors::Blue), VertexPositionColor(nodes[i].linkedNodes[j]->pos, Colors::Blue));
		}
	}
}

NavMesh::~NavMesh()
{
}
