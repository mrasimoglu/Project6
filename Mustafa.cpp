#include "Mustafa.h"

#define PI 3.14159265359


float Collision::Distance2D(Vector2 v1, Vector2 v2)
{	
	return Magnitude(v1 - v2);
}

float Collision::Magnitude(Vector2 v)
{
	return sqrt(v.x*v.x + v.y*v.y);
}

Vector2 Collision::CalculateRectangleCorner(void* objx, int i)
{
	Entity *obj = (Entity*) objx;

	double rot = obj->GetSprite()->GetRotation();
	if (i == 0)
		rot += atan((double)(obj->GetHeight() / 2) / (obj->GetWidth() / 2));
	else if (i == 1)
		rot += PI - atan((double)(obj->GetHeight() / 2) / (obj->GetWidth() / 2));
	else if (i == 2)
		rot += PI + atan((double)(obj->GetHeight() / 2) / (obj->GetWidth() / 2));
	else if (i == 3)
		rot -= atan((double)(obj->GetHeight() / 2) / (obj->GetWidth() / 2));
	else;

	int h = obj->GetHeight() / 2;
	h = h * obj->GetSprite()->GetScale().y;

	int w = obj->GetWidth() / 2;
	w = w * obj->GetSprite()->GetScale().x;

	double a = (h*h) + (w*w);
	a = sqrt(a);

	Vector2 pos;

	pos.x = obj->GetSprite()->GetPosition().x;
	pos.x += cos(rot) * a;

	pos.y = obj->GetSprite()->GetPosition().y;
	pos.y += sin(rot) * a;

	return pos;
}

void Collision::CalculateCircleVertex(void *obj, Vertexs *ver)
{
	Entity* e1 = (Entity*)obj;

	int h = (e1->GetHeight() / 2) * e1->GetSprite()->GetScale().y;
	int w = (e1->GetWidth() / 2) * e1->GetSprite()->GetScale().x;

	ver->vertex[7] = e1->GetSprite()->GetPosition() + Vector2(w / sqrt(2), -h / sqrt(2));
	ver->vertex[6] = e1->GetSprite()->GetPosition() + Vector2(0, -h);
	ver->vertex[5] = e1->GetSprite()->GetPosition() + Vector2(-w / sqrt(2), -h / sqrt(2));
	ver->vertex[4] = e1->GetSprite()->GetPosition() + Vector2(-w, 0);

	ver->vertex[3] = e1->GetSprite()->GetPosition() + Vector2(-w / sqrt(2), h / sqrt(2));
	ver->vertex[2] = e1->GetSprite()->GetPosition() + Vector2(0, h);
	ver->vertex[1] = e1->GetSprite()->GetPosition() + Vector2(w/sqrt(2), h/sqrt(2));
	ver->vertex[0] = e1->GetSprite()->GetPosition() + Vector2(h, 0);
}

double Collision::ProjectVector(Vector2 a, Vector2 n)
{
	double nlenght = (n.x * n.x) + (n.y * n.y);
	nlenght = sqrt(nlenght);

	n.x = n.x / nlenght;
	n.y = n.y / nlenght;

	return a.x * n.y + a.y * n.x;
}

bool Collision::isOverlap(double min1, double max1, double min2, double max2)
{
	double totalRange = max(max1, max2) - min(min1, min2);
	double sumOfRanges = (max1 - min1) + (max2 - min2);

	if (sumOfRanges > totalRange)
		return true;
	else
		return false;
}

Vector2 Collision::CheckCollisionT(void *obj1, void *obj2)
{
	Entity *e1 = static_cast<Entity*>(obj1);
	Entity *e2 = static_cast<Entity*>(obj2);

	/*if (Distance2D(e1->GetSprite()->GetPosition(), e2->GetSprite()->GetPosition()) > 200)
		return Vector2(0,0);*/

	Vertexs *ev1 = e1->GetVertexs(), *ev2 = e2->GetVertexs();

	Vector2 normal;
	float closest = INFINITE;
	Vector2 returnV;

	for (int i = 0; i < ev1->numOfVertex; i++)
	{
		normal.x = (ev1->vertex[(i + 1) % ev1->numOfVertex].y - ev1->vertex[i].y);
		normal.y = -(ev1->vertex[(i + 1) % ev1->numOfVertex].x - ev1->vertex[i].x);

		float min1 = Collision::ProjectVector(ev1->vertex[0], normal), max1 = Collision::ProjectVector(ev1->vertex[0], normal);
		float min2 = Collision::ProjectVector(ev2->vertex[0], normal), max2 = Collision::ProjectVector(ev2->vertex[0], normal);

		for (int j = 1; j < ev1->numOfVertex; j++)
		{
			double temp = Collision::ProjectVector(ev1->vertex[j], normal);
			if (temp > max1)
				max1 = temp;
			if (temp < min1)
				min1 = temp;
		}
		for (int j = 1; j < ev2->numOfVertex; j++)
		{
			double temp = Collision::ProjectVector(ev2->vertex[j], normal);
			if (temp > max2)
				max2 = temp;
			if (temp < min2)
				min2 = temp;
		}

		if (!(isOverlap(min1, max1, min2, max2))) {
			return Vector2(0, 0);
		}
	}

	for (int i = 0; i < ev2->numOfVertex; i++)
	{
		normal.x = (ev2->vertex[(i + 1) % ev2->numOfVertex].y - ev2->vertex[i].y);
		normal.y = -(ev2->vertex[(i + 1) % ev2->numOfVertex].x - ev2->vertex[i].x);

		Vector2 temp = (ev2->vertex[i] + ev2->vertex[(i + 1) % ev2->numOfVertex]) / Vector2(2.0f, 2.0f);

		float a, b;
		a = temp.x - e1->GetSprite()->GetPosition().x;
		b = temp.y - e1->GetSprite()->GetPosition().y;

		float temp2 = sqrt((a*a)+(b*b));

		if (temp2 < closest)
		{
			closest = temp2;
			returnV = normal;
		}

		float min1 = Collision::ProjectVector(ev1->vertex[0], normal), max1 = Collision::ProjectVector(ev1->vertex[0], normal);
		float min2 = Collision::ProjectVector(ev2->vertex[0], normal), max2 = Collision::ProjectVector(ev2->vertex[0], normal);

		for (int j = 1; j < ev1->numOfVertex; j++)
		{
			double temp = Collision::ProjectVector(ev1->vertex[j], normal);
			if (temp > max1)
				max1 = temp;
			if (temp < min1)
				min1 = temp;
		}
		for (int j = 1; j < ev2->numOfVertex; j++)
		{
			double temp = Collision::ProjectVector(ev2->vertex[j], normal);
			if (temp > max2)
				max2 = temp;
			if (temp < min2)
				min2 = temp;
		}

		if (!(isOverlap(min1, max1, min2, max2))) {
			return Vector2(0, 0);;
		}
	}

	returnV = returnV / Vector2(sqrt((returnV.x * returnV.x) + (returnV.y*returnV.y)), sqrt((returnV.x * returnV.x) + (returnV.y*returnV.y)));

	return returnV;
}

bool Collision::CheckCollision(void *obj1, void *obj2)
{
	Entity *e1 = static_cast<Entity*>(obj1);
	Entity *e2 = static_cast<Entity*>(obj2);

	/*if (Distance2D(e1->GetSprite()->GetPosition(), e2->GetSprite()->GetPosition()) > 20)
		return false;*/

	Vertexs *ev1 = e1->GetVertexs(), *ev2 = e2->GetVertexs();

	Vector2 normal;

	for (int i = 0; i < ev1->numOfVertex; i++)
	{
		normal.x = (ev1->vertex[(i + 1) % ev1->numOfVertex].y - ev1->vertex[i].y);
		normal.y = -(ev1->vertex[(i + 1) % ev1->numOfVertex].x - ev1->vertex[i].x);

		float min1 = Collision::ProjectVector(ev1->vertex[0], normal), max1 = Collision::ProjectVector(ev1->vertex[0], normal);
		float min2 = Collision::ProjectVector(ev2->vertex[0], normal), max2 = Collision::ProjectVector(ev2->vertex[0], normal);

		for (int j = 1; j < ev1->numOfVertex ; j++)
		{
			double temp = Collision::ProjectVector(ev1->vertex[j],normal);
			if (temp > max1)
				max1 = temp;
			if (temp < min1)
				min1 = temp;
		}
		for (int j = 1; j < ev2->numOfVertex; j++)
		{
			double temp = Collision::ProjectVector(ev2->vertex[j], normal);
			if (temp > max2)
				max2 = temp;
			if (temp < min2)
				min2 = temp;
		}

		if (!(isOverlap(min1,max1,min2,max2))) {
			return false;
		}
	}

	for (int i = 0; i < ev2->numOfVertex; i++)
	{
		Vector2 normal;

		normal.x = (ev2->vertex[(i + 1) % ev2->numOfVertex].y - ev2->vertex[i].y);
		normal.y = -(ev2->vertex[(i + 1) % ev2->numOfVertex].x - ev2->vertex[i].x);
		 
		float min1 = Collision::ProjectVector(ev1->vertex[0], normal), max1 = Collision::ProjectVector(ev1->vertex[0], normal);
		float min2 = Collision::ProjectVector(ev2->vertex[0], normal), max2 = Collision::ProjectVector(ev2->vertex[0], normal);

		for (int j = 1; j < ev1->numOfVertex; j++)
		{
			double temp = Collision::ProjectVector(ev1->vertex[j], normal);
			if (temp > max1)
				max1 = temp;
			if (temp < min1)
				min1 = temp;
		}
		for (int j = 1; j < ev2->numOfVertex; j++)
		{
			double temp = Collision::ProjectVector(ev2->vertex[j], normal);
			if (temp > max2)
				max2 = temp;
			if (temp < min2)
				min2 = temp;
		}

		if (!(isOverlap(min1, max1, min2, max2))) {
			return false;
		}
	}

	return true;
}

void DrawVertex()
{
	/*Vertexs *ev1 = statics[i]->GetVertexs();
	Vector2 middle;
	middle.y = (ev1->vertex[(j + 1) % ev1->numOfVertex].y + ev1->vertex[j].y) / 2;
	middle.x = (ev1->vertex[(j + 1) % ev1->numOfVertex].x + ev1->vertex[j].x) / 2;
	Vector2 normal;

	normal.x = (ev1->vertex[(j + 1) % ev1->numOfVertex].y - ev1->vertex[j].y);
	normal.y = -(ev1->vertex[(j + 1) % ev1->numOfVertex].x - ev1->vertex[j].x);

	StaticObject* test = new StaticObject(device, L"img/test2.dds", middle, atan2(normal.y, normal.x), rectangle);
	test->GetSprite()->SetOrigin(Vector2(0, 128));
	test->Draw(spriteBatch);*/
}

bool Collision::CheckDistance(void *obj1, void *obj2, float distance)
{
	Entity *e1 = static_cast<Entity*>(obj1);
	Entity *e2 = static_cast<Entity*>(obj2);

	return !(Collision::Distance2D(e1->GetPosition(), e2->GetPosition()) > distance);
}