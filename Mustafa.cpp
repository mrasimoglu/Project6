#include "Mustafa.h"

#define PI 3.14159265359

Vector2 Collision::UnitVector(Vector2 normal)
{
	normal = normal / Vector2(sqrt((normal.x * normal.x) + (normal.y*normal.y)), sqrt((normal.x * normal.x) + (normal.y*normal.y)));
	return normal;
}

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

bool Collision::CheckCollision(void* obj, Vector2 vec, Camera* cam/*, PrimitiveBatch<VertexPositionColor>* primitiveBatch*/)
{
	Entity* ent = (Entity*)obj;

	Vertexs *ev1 = ent->GetVertexs();

	Vector2 normal;
	float min, max, a;

	for (int i = 0; i < ev1->numOfVertex; i++)
	{
		normal.x = (ev1->vertex[(i + 1) % ev1->numOfVertex].y - ev1->vertex[i].y);
		normal.y = -(ev1->vertex[(i + 1) % ev1->numOfVertex].x - ev1->vertex[i].x);

		min = Collision::ProjectVector(ev1->vertex[0], normal), max = Collision::ProjectVector(ev1->vertex[0], normal);
		a = Collision::ProjectVector(vec, normal);

		for (int j = 1; j < ev1->numOfVertex; j++)
		{
			double temp = Collision::ProjectVector(ev1->vertex[j], normal);
			if (temp > max)
				max = temp;
			if (temp < min)
				min = temp;
		}

		if (!(a > min && a < max))
			return false;
	}

	return true;
}

void Collision::DrawVertex(void* obj, Camera* camera)
{
	Entity* e1 = (Entity*)obj;

	Vertexs *ev1 = e1->GetVertexs();
	Vector2 middle;

	for (int j = 0; j < ev1->numOfVertex; j++)
	{

		StaticObject* test = new StaticObject(L"img/test.dds", Vector2(ev1->vertex[j].x, ev1->vertex[j].y), 0, rectangle, camera);
		test->GetSprite()->SetOrigin(Vector2(0, 128));
		test->Draw(DXApp::spriteBatch);
		free(test);

		middle.y = (ev1->vertex[(j + 1) % ev1->numOfVertex].y + ev1->vertex[j].y) / 2;
		middle.x = (ev1->vertex[(j + 1) % ev1->numOfVertex].x + ev1->vertex[j].x) / 2;
		Vector2 normal;

		normal.x = (ev1->vertex[(j + 1) % ev1->numOfVertex].y - ev1->vertex[j].y);
		normal.y = -(ev1->vertex[(j + 1) % ev1->numOfVertex].x - ev1->vertex[j].x);

		test = new StaticObject(L"img/test2.dds", middle, atan2(normal.y, normal.x), rectangle, camera);
		test->GetSprite()->SetScale(Vector2(0.1f, 0.1f));
		test->GetSprite()->SetOrigin(Vector2(0, 128));
		test->Draw(DXApp::spriteBatch);
	}

	
}

bool Collision::isOver(DirectX::Mouse::State state, Vector2 pos, Vector2 WidthHeight)
{
	if (state.x < pos.x + (WidthHeight.x / 2) && state.x > pos.x - (WidthHeight.x / 2))
		if (state.y < pos.y + (WidthHeight.y / 2) && state.y > pos.y - (WidthHeight.y / 2))
			return true;

	return false;
}

bool Collision::isOver(void* obj, Vector2 pos)
{
	Entity* ent = (Entity*)obj;

	Vector2 state = ent->GetPosition();

	Vector2 WidthHeight;
	WidthHeight.x = ent->GetWidth() * ent->GetSprite()->GetScale().x;
	WidthHeight.y = ent->GetHeight() * ent->GetSprite()->GetScale().y;


	if (state.x < pos.x + (WidthHeight.x / 2) && state.x > pos.x - (WidthHeight.x / 2))
		if (state.y < pos.y + (WidthHeight.y / 2) && state.y > pos.y - (WidthHeight.y / 2))
			return true;

	return false;
}

bool Collision::isOver(Camera* cam, Vector2 pos)
{
	Vector2 state = cam->GetPosition();
	Vector2 WidthHeight = cam->GetView();

	if (state.x < pos.x + (WidthHeight.x / 2) && state.x > pos.x - (WidthHeight.x / 2))
		if (state.y < pos.y + (WidthHeight.y / 2) && state.y > pos.y - (WidthHeight.y / 2))
			return true;

	return false;
}

bool Collision::CheckDistance(void *obj1, void *obj2, float distance)
{
	Entity *e1 = static_cast<Entity*>(obj1);
	Entity *e2 = static_cast<Entity*>(obj2);

	return !(Collision::Distance2D(e1->GetPosition(), e2->GetPosition()) > distance);
}

bool Collision::Trace(void *obj, StaticObject** statics, Vector2 pos, Camera* cam, PrimitiveBatch<VertexPositionColor>* primitiveBatch)
{
	Entity* ent = (Entity*)obj;

	int sayac = 0;
	int i = 0;
	while (i == sayac && i < ent->GetVertexs()->numOfVertex)
	{
		for (int k = 0; k < 100; k++)
		{
			if (statics[k] != nullptr)
			{
				Vector2 A = pos;
				Vector2 B = ent->GetVertexs()->vertex[i];

				bool tf = false;

				int j = 1;
				while(!tf && j < statics[k]->GetVertexs()->numOfVertex + 1)
				{
					Vector2 C = statics[k]->GetVertexs()->vertex[j - 1];
					Vector2 D = statics[k]->GetVertexs()->vertex[j % (statics[k]->GetVertexs()->numOfVertex)];

					float T = (((D.y - C.y) * (C.x - A.x)) - ((C.y - A.y)*(D.x - C.x))) / ((D.y - C.y)*(B.x - A.x) - (B.y - A.y) * (D.x - C.x));

					Vector2 E;
					E.x = A.x + (B.x - A.x) * T;
					E.y = A.y + (B.y - A.y) * T;

					Vector2 normal;
					normal.x = (C.y - D.y);
					normal.y = -(C.x - D.x);

					normal = normal / Vector2(sqrt((normal.x * normal.x) + (normal.y*normal.y)), sqrt((normal.x * normal.x) + (normal.y*normal.y)));
					normal = normal * 10;
					E = E + normal;

					primitiveBatch->DrawLine(VertexPositionColor(A - cam->GetOffset(), Colors::Green), VertexPositionColor(B - cam->GetOffset(), Colors::Green));
					//primitiveBatch->DrawLine(VertexPositionColor(C, Colors::Red), VertexPositionColor(D, Colors::Red));

					if (Collision::CheckCollision(statics[k], E, cam))
						tf = true;
					else
						primitiveBatch->DrawQuad(VertexPositionColor(E - cam->GetOffset() + Vector2(5, 5), Colors::HotPink),
							VertexPositionColor(E - cam->GetOffset() + Vector2(-5, 5), Colors::HotPink),
							VertexPositionColor(E - cam->GetOffset() + Vector2(-5, -5), Colors::HotPink),
							VertexPositionColor(E - cam->GetOffset() + Vector2(5, -5), Colors::HotPink));

					j++;
				}

 				if (tf)
					sayac++;

				i++;
			}
		}

			int a = ent->GetVertexs()->numOfVertex;
			if (sayac == a)
				return false;
	}

	return true;
}

bool Collision::Trace3(void *obj, StaticObject** statics, Vector2 pos, Camera* cam, PrimitiveBatch<VertexPositionColor>* primitiveBatch)
{
	//Bir kenarýnýn görünmesi yeterli

	Entity* ent = (Entity*)obj;
	
	int i = 0;
	while (i < ent->GetVertexs()->numOfVertex)
	{
		bool tf = false;

		Vector2 temp;
		Vector2 A = pos;
		Vector2 B = ent->GetVertexs()->vertex[i];

		primitiveBatch->DrawLine(VertexPositionColor(A - cam->GetOffset(), Colors::Green), VertexPositionColor(B - cam->GetOffset(), Colors::Green));

		int j = 0;
		while (!tf && j < 99)
		{
			if (statics[j] != nullptr)
			{
				int k = 1;
				while (!tf && k < statics[j]->GetVertexs()->numOfVertex)
				{
					Vector2 C = statics[j]->GetVertexs()->vertex[k - 1];
					Vector2 D = statics[j]->GetVertexs()->vertex[k % (statics[j]->GetVertexs()->numOfVertex)];

					float T = (((D.y - C.y) * (C.x - A.x)) - ((C.y - A.y)*(D.x - C.x))) / ((D.y - C.y)*(B.x - A.x) - (B.y - A.y) * (D.x - C.x));

					Vector2 normal;
					normal.x = (C.y - D.y);
					normal.y = -(C.x - D.x);

					normal = normal / Vector2(sqrt((normal.x * normal.x) + (normal.y*normal.y)), sqrt((normal.x * normal.x) + (normal.y*normal.y)));
					normal = normal * 10;

					Vector2 E;
					E.x = A.x + (B.x - A.x) * T;
					E.y = A.y + (B.y - A.y) * T;

					E = E + normal;

					primitiveBatch->DrawQuad(VertexPositionColor(E + Vector2(5, 5) - cam->GetOffset(), Colors::HotPink),
						VertexPositionColor(E + Vector2(-5, 5) - cam->GetOffset(), Colors::HotPink),
						VertexPositionColor(E + Vector2(-5, -5) - cam->GetOffset(), Colors::HotPink),
						VertexPositionColor(E + Vector2(5, -5) - cam->GetOffset(), Colors::HotPink));

					if ((E.x < A.x && E.x > ent->GetPosition().x) || (E.x > A.x && E.x < ent->GetPosition().x))
						if ((E.y < A.y && E.y > ent->GetPosition().y) || (E.y > A.y && E.y < ent->GetPosition().y))
							if (Collision::CheckCollision(statics[j], E, cam))
							{
								tf = true;
							}
							else;
						else;
					else;

					k++;
				}
			}
			j++;

			if (!tf)
				return true;
		}
		i++;
	}

	return false;
}

bool Collision::Traceee(void * obj, StaticObject ** statics, Vector2 pos, Camera * cam, PrimitiveBatch<VertexPositionColor>* primitiveBatch)
{
	Entity* ent = (Entity*)obj;

	bool tf2 = false;

	int i = 0;
	while (i < ent->GetVertexs()->numOfVertex)
	{
		bool tf = false;

		Vector2 temp;
		Vector2 A = pos;
		Vector2 B = ent->GetVertexs()->vertex[i];

		primitiveBatch->DrawLine(VertexPositionColor(A - cam->GetOffset(), Colors::Green), VertexPositionColor(B - cam->GetOffset(), Colors::Green));

		int j = 0;
		while (j < 99)
		{
			if (statics[j] != nullptr)
			{
				if (j == 8);
				//printf("a");
				int k = 1;
				while (k < statics[j]->GetVertexs()->numOfVertex)
				{
					Vector2 C = statics[j]->GetVertexs()->vertex[k - 1];
					Vector2 D = statics[j]->GetVertexs()->vertex[k % (statics[j]->GetVertexs()->numOfVertex)];

					float T = (((D.y - C.y) * (C.x - A.x)) - ((C.y - A.y)*(D.x - C.x))) / ((D.y - C.y)*(B.x - A.x) - (B.y - A.y) * (D.x - C.x));

					Vector2 normal;
					normal.x = (C.y - D.y);
					normal.y = -(C.x - D.x);

					normal = normal / Vector2(sqrt((normal.x * normal.x) + (normal.y*normal.y)), sqrt((normal.x * normal.x) + (normal.y*normal.y)));
					normal = normal * 10;

					Vector2 E;
					E.x = A.x + (B.x - A.x) * T;
					E.y = A.y + (B.y - A.y) * T;

					E = E + normal;

					if ((E.x < A.x && E.x > ent->GetPosition().x) || (E.x > A.x && E.x < ent->GetPosition().x))
						if ((E.y < A.y && E.y > ent->GetPosition().y) || (E.y > A.y && E.y < ent->GetPosition().y))
							if (Collision::CheckCollision(statics[j], E, cam))
							{
								temp = E;
								primitiveBatch->DrawQuad(VertexPositionColor(temp + Vector2(5, 5) - cam->GetOffset(), Colors::HotPink),
									VertexPositionColor(temp + Vector2(-5, 5) - cam->GetOffset(), Colors::HotPink),
									VertexPositionColor(temp + Vector2(-5, -5) - cam->GetOffset(), Colors::HotPink),
									VertexPositionColor(temp + Vector2(5, -5) - cam->GetOffset(), Colors::HotPink));

								return false;
							}
							else;
						else;
					else;

					k++;
				}
			}
			j++;
		}
		i++;
	}

	return true;
}

bool Collision::Trace2(void *obj, StaticObject** statics, Vector2 pos, Camera* cam)
{
	//Her kenarý görünür olmalý

	Entity* ent = (Entity*)obj;
	
	bool tf = false;

	int i = 0;
	while (!tf && i < ent->GetVertexs()->numOfVertex)
	{

		Vector2 A = pos;
		Vector2 B = ent->GetVertexs()->vertex[i];

		//primitiveBatch->DrawLine(VertexPositionColor(A - cam->GetOffset(), Colors::Green), VertexPositionColor(B - cam->GetOffset(), Colors::Green));

		int j = 0;
		while (!tf && j < 100)
		{

			if (statics[j] != nullptr)
			{
				int k = 0;
				while (!tf && k < statics[j]->GetVertexs()->numOfVertex)
				{
					Vector2 C = statics[j]->GetVertexs()->vertex[k - 1];
					Vector2 D = statics[j]->GetVertexs()->vertex[k % (statics[j]->GetVertexs()->numOfVertex)];

					float T = (((D.y - C.y) * (C.x - A.x)) - ((C.y - A.y)*(D.x - C.x))) / ((D.y - C.y)*(B.x - A.x) - (B.y - A.y) * (D.x - C.x));

					Vector2 normal;
					normal.x = (C.y - D.y);
					normal.y = -(C.x - D.x);

					normal = normal / Vector2(sqrt((normal.x * normal.x) + (normal.y*normal.y)), sqrt((normal.x * normal.x) + (normal.y*normal.y)));
					normal = normal * 10;

					Vector2 E;
					E.x = A.x + (B.x - A.x) * T;
					E.y = A.y + (B.y - A.y) * T;

					E = E + normal;

					if (Collision::CheckCollision(statics[j], E, cam))
						tf = true;

					k++;
				}
			}
			j++;
		}
		i++;
	}

	if (tf)
		return false;
	else
		return true;
}

bool Collision::TraceEfka(void *obj, StaticObject** statics, Vector2 pos, Camera* cam)
{
	//Bir kenarýnýn görünmesi yeterli

	Entity* ent = (Entity*)obj;

	bool rv = false;

	int i = 0;
	while (!rv && i < ent->GetVertexs()->numOfVertex)
	{
		bool tf = false;

		Vector2 A = pos;
		Vector2 B = ent->GetVertexs()->vertex[i];

		//primitiveBatch->DrawLine(VertexPositionColor(A - cam->GetOffset(), Colors::Green), VertexPositionColor(B - cam->GetOffset(), Colors::Green));

		int j = 0;
		while (!tf && j < 100)
		{
			if (statics[j] != nullptr)
			{
				int k = 0;
				while (!tf && k < statics[j]->GetVertexs()->numOfVertex)
				{
					Vector2 C = statics[j]->GetVertexs()->vertex[k - 1];
					Vector2 D = statics[j]->GetVertexs()->vertex[k % (statics[j]->GetVertexs()->numOfVertex)];

					float T = (((D.y - C.y) * (C.x - A.x)) - ((C.y - A.y)*(D.x - C.x))) / ((D.y - C.y)*(B.x - A.x) - (B.y - A.y) * (D.x - C.x));

					Vector2 normal;
					normal.x = (C.y - D.y);
					normal.y = -(C.x - D.x);

					normal = normal / Vector2(sqrt((normal.x * normal.x) + (normal.y*normal.y)), sqrt((normal.x * normal.x) + (normal.y*normal.y)));
					normal = normal * 10;

					Vector2 E;
					E.x = A.x + (B.x - A.x) * T;
					E.y = A.y + (B.y - A.y) * T;

					E = E + normal;

					Vector2 temp = E;

					if ((E.x < A.x && E.x > ent->GetPosition().x) || (E.x > A.x && E.x < ent->GetPosition().x))
						if (Collision::CheckCollision(statics[j], E, cam))
							tf = true;

					k++;
				}
			}
			j++;
		}

		if (tf == false)
			return rv = true;

		i++;
	}

	return false;
}

bool Collision::Trace66(void *obj, Vector2 pos, StaticObject** statics, Camera* cam)
{
	Entity* ent = (Entity*)obj;

	Vector2 A, B, C, D, E;
	float T;

	A = pos;

	//Objenin tim noktalarýna bakýcaz
	bool tf = false;
	int ent_numofvertex = ent->GetVertexs()->numOfVertex;
	int i = 0;
	while (!tf && i < ent_numofvertex)
	{
		B = ent->GetVertexs()->vertex[i];

		//Tüm static objecelere bakýcaz
		int counter = 0;
		int j = 0;
		while (statics[j] != nullptr)
		{
			//Static objenin tüm vertexlerine bakýcaz
			bool tf3 = false;
			int statics_numofvvertex = statics[j]->GetVertexs()->numOfVertex; 
			int k = 0;
			while (!tf3 && k < statics_numofvvertex)
			{
				C = statics[j]->GetVertexs()->vertex[k];
				D = statics[j]->GetVertexs()->vertex[(k + 1) % statics_numofvvertex];

				T = ((D.y - C.y)*(C.x - A.x) - (C.y - A.y)*(D.x - C.x)) / ((D.y - C.y)*(B.x - A.x) - (B.y - A.y)*(D.x - C.x));

				E.x = A.x + (B.x - A.x)*T;
				E.y = A.y + (B.y - A.y)*T;

				if((E.x < A.x && E.x > B.x) || (E.x > A.x && E.x < B.x))
					if ((E.y < A.y && E.y > B.y) || (E.y > A.y && E.y < B.y))
					{
						Vector2 normal;
						normal.x = (D.y - C.y);
						normal.y = -(D.x - C.x);

						normal = normal / Vector2(sqrt((normal.x * normal.x) + (normal.y*normal.y)), sqrt((normal.x * normal.x) + (normal.y*normal.y)));
						normal = normal * 10;

						E = E - normal;

						if (CheckCollision(statics[j], E, cam))
						{
							tf3 = true;
						}
					}
					else;
				else;

				k++;
			}

			if (tf3 == false)
				counter++;

			j++;
		}

		if (j == counter)
			tf = true;

		i++;
	}

	return tf;
}

void Collision::Shadow(StaticObject **statics, Vector2 pos, Camera* cam, Survivor* sur)
{
	SpriteSheet* sprite;
	sprite = new SpriteSheet();
	sprite->Load(DXApp::m_pDevice, L"img/map/shadow.dds");
	sprite->SetScale(Vector2(0.32f, 0.32f));

	Vector2 cPos = Vector2(5, 5);
	
	while (cPos.x < cam->GetView().x)
	{
		sprite->SetPosition(cPos);
		
		if (!Trace2(sur, statics, cPos, cam))
			sprite->Draw(DXApp::spriteBatch, cam);	

		if (cPos.y + 5 < cam->GetView().y)
			cPos.y += 5;
		else
			cPos = Vector2(cPos.x + 5, 5);
	}
}

void Collision::Shadow2(StaticObject** statics, Vector2 pos, float rot, Camera* cam, Survivor* sur)
{
	SpriteSheet* sprite;
	sprite = new SpriteSheet();
	sprite->Load(DXApp::m_pDevice, L"img/map/shadow.dds");
	sprite->SetScale(Vector2(0.32f, 0.32f));

	Vector2 fPos = pos;

	int i = 0;
	while (i < 1)
	{
		pos = fPos;
		bool tf = true;
		while (isOver(cam, pos))
		{
			sprite->SetPosition(pos);

			if (tf && !Trace2(sur, statics, pos, cam) && !isOver(statics[0], pos))
			{
				sprite->Draw(DXApp::spriteBatch, cam);
				tf = false;
			}
			if(!tf)
				sprite->Draw(DXApp::spriteBatch, cam);
			pos += Vector2(cos(rot), sin(rot)) * 5;
		}

		rot += 0.005;
		i++;
	}
}

void Collision::DrawRay(PrimitiveBatch<VertexPositionColor>* primitiveBatch, StaticObject ** statics, Vector2 pos, Camera * cam)
{
	for (int i = 0; i < 100; i++)
	{
		if (statics[i] != nullptr)
		{
			float* slopes;
			Vector2* vectors;

			slopes = (float*)malloc(statics[i]->GetVertexs()->numOfVertex * sizeof(float));
			vectors = (Vector2*)malloc(statics[i]->GetVertexs()->numOfVertex * sizeof(Vector2));

			for (int j = 0; j < statics[i]->GetVertexs()->numOfVertex; j++)
			{
				Vector2 pos2 = statics[i]->GetVertexs()->vertex[j];

				Vector2 v;
				float slope;

				if (pos.x == pos2.x)
				{
					if (pos.y < pos2.y)
					{
						v = Vector2(pos.x, cam->GetView().y);
						slope = (3 * PI) / 2;
					}
					else
					{
						v = Vector2(pos.x, 0);
						slope = PI / 2;
					}
				}
				else if (pos.y == pos2.y)
				{
					if (pos.x < pos2.x)
					{
						v = Vector2(cam->GetView().x, pos.y);
						slope = 0.0f;
					}
					else
					{
						v = Vector2(0, pos.y);
						slope = PI;
					}
				}
				else
				{
					slope = (pos2.y - pos.y) / (pos2.x - pos.x);

					if (pos.y < pos2.y)
					{
						v.y = cam->GetView().y + 150000;
					}
					else
					{
						v.y = -150000;
					}

					v.x = ((v.y - pos2.y) / slope) + pos2.x;
				}
				//primitiveBatch->DrawLine(VertexPositionColor(pos, Colors::Red), VertexPositionColor(v, Colors::Red));

				slope = atan2(-(pos2.y - pos.y), (pos2.x - pos.x));
				if (slope < 0)
					slope += 2 * PI;

				slope *= (180.0 / 3.141592653589793238463);

				slopes[j] = slope;
				vectors[j] = v;
			}
			int* a = (int*)malloc(sizeof(int) * 2);
			a = FindMostDifference(slopes, statics[i]->GetVertexs()->numOfVertex);

			//primitiveBatch->DrawLine(VertexPositionColor(statics[i]->GetVertexs()->vertex[a[0]], Colors::YellowGreen), VertexPositionColor(statics[i]->GetVertexs()->vertex[a[1]], Colors::YellowGreen));
			primitiveBatch->DrawQuad(VertexPositionColor(statics[i]->GetVertexs()->vertex[a[0]] - cam->GetOffset(), Colors::Black),
				VertexPositionColor(statics[i]->GetVertexs()->vertex[a[1]] - cam->GetOffset(), Colors::Black),
				VertexPositionColor(vectors[a[1]] - cam->GetOffset(), Colors::Black),
				VertexPositionColor(vectors[a[0]] - cam->GetOffset(), Colors::Black));
			free(a);
			free(slopes);
			free(vectors);
		}
	}	
}

int* Collision::FindMostDifference(float* arr, int n)
{
	float difference = 0;
	int* a = (int*)malloc(sizeof(int) * 2);
	a[0] = 0; a[1] = 0;

	for (int i = 0; i < n - 1; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			if ((arr[i] > 270 && arr[j] < 90) || (arr[j] > 270 && arr[i] < 90))
			{
				if (arr[i] < 90)
				{
					arr[i] += 360;
					if (difference < fabs(arr[i] - arr[j]))
					{

						difference = fabs(arr[i] - arr[j]);
						a[0] = i;
						a[1] = j;
					}
				}
				else
				{
					arr[j] += 360;
					if (difference < fabs(arr[i] - arr[j]))
					{

						difference = fabs(arr[i] - arr[j]);
						a[0] = i;
						a[1] = j;
					}
				}
			}
			else if (difference < fabs(arr[i] - arr[j]))
			{
				
					difference = fabs(arr[i] - arr[j]);
					a[0] = i;
					a[1] = j;
			}
		}
	}

	return a;

	free(a);
}

int Collision::CalculateMinDistance(void* obj1, void* obj2)
{
	Entity* e1 = (Entity*)obj1;
	Entity* e2 = (Entity*)obj2;

	Vertexs* ev1 = e1->GetVertexs();
	Vertexs* ev2 = e2->GetVertexs();

	int min = 1000000;

	for (int i = 0; i < ev1->numOfVertex; i++)
	{
		for (int j = 0; j < ev2->numOfVertex; j++)
		{
			int temp = Distance2D(ev1->vertex[i], ev2->vertex[j]);
			if (temp < min)
				min = temp;
		}
	}

	return min;
}

float Collision::DamageDistance(void* obj1, void* obj2, Camera* camera, PrimitiveBatch<VertexPositionColor>* primitiveBatch)
{
	Entity* ent1 = (Entity*)obj1;
	Entity* ent2 = (Entity*)obj2;

	Vector2 A = ent1->GetPosition() - camera->GetOffset();
	Vector2 C = ent2->GetPosition() - camera->GetOffset();

	float cosx = cos(ent1->GetRotation()), sinx = sin(ent1->GetRotation());
	Vector2 B = A + Vector2(-sinx * 105, cosx * 105);

	cosx = cos(ent2->GetRotation()), sinx = sin(ent2->GetRotation());
	Vector2 D = C + Vector2(cosx * 105, sinx * 105);

	float T = (((D.y - C.y) * (C.x - A.x)) - ((C.y - A.y)*(D.x - C.x))) / ((D.y - C.y)*(B.x - A.x) - (B.y - A.y) * (D.x - C.x));

	Vector2 E;
	E.x = A.x + (B.x - A.x) * T;
	E.y = A.y + (B.y - A.y) * T;

	primitiveBatch->DrawLine(VertexPositionColor(A, Colors::Red), VertexPositionColor(B, Colors::Red));
	primitiveBatch->DrawLine(VertexPositionColor(C, Colors::Yellow), VertexPositionColor(D, Colors::Yellow));

	primitiveBatch->DrawQuad(VertexPositionColor(E + Vector2(1, 1), Colors::HotPink),
		VertexPositionColor(E + Vector2(-1, 1), Colors::HotPink),
		VertexPositionColor(E + Vector2(-1, -1), Colors::HotPink),
		VertexPositionColor(E + Vector2(1, -1), Colors::HotPink));

	return Distance2D(A, E);
}

float Collision::DamageDistance2(void* obj1, void* obj2, Camera* camera)
{
	Entity* ent1 = (Entity*)obj1;
	Entity* ent2 = (Entity*)obj2;

	Vector2 A = ent1->GetPosition() - camera->GetOffset();
	Vector2 C = ent2->GetPosition() - camera->GetOffset();

	float cosx = cos(ent1->GetRotation()), sinx = sin(ent1->GetRotation());
	Vector2 B = A + Vector2(-sinx * 105, cosx * 105);

	cosx = cos(ent2->GetRotation()), sinx = sin(ent2->GetRotation());
	Vector2 D = C + Vector2(cosx * 105, sinx * 105);

	float T = (((D.y - C.y) * (C.x - A.x)) - ((C.y - A.y)*(D.x - C.x))) / ((D.y - C.y)*(B.x - A.x) - (B.y - A.y) * (D.x - C.x));

	Vector2 E;
	E.x = A.x + (B.x - A.x) * T;
	E.y = A.y + (B.y - A.y) * T;

	float dist = Distance2D(A, E);
	dist = dist / 6;

	return dist;
}