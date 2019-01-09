#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> /* offsetof */
#include "..\GL\glew.h"
#include "..\GL\glut.h"
#include <iostream>
#include <vector>
#include<atlimage.h >

#include "..\include\Eigen\Eigen\Eigen"

using namespace Eigen;
using namespace std;

#include "..\shader_lib\shader.h"

//#include "LeastSquaresSparseSolver.h"

#include "triangle.cpp"

//#pragma comment(lib, "legacy_stdio_definitions.lib")

#ifdef __cplusplus
extern "C"
#endif

FILE __iob_func[3] = { __acrt_iob_func(0),__acrt_iob_func(1),__acrt_iob_func(2) };

extern "C" { FILE _iob[3] = { __acrt_iob_func(0),__acrt_iob_func(1),__acrt_iob_func(2) }; }

void init(void);
void display(void);
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void idle(void);

GLuint program;
GLuint program1;

unsigned int  PG = 0;
//---------------------�������Լ�����Ķ���---------------------------------

void mouse(int button, int state, int x, int y);

int getnearmarkeri(float glx, float gly, float& dist);



//
int window_width;//��ǰ���ڿ�ȣ����أ�
int window_height;//��ǰ���ڸ߶ȣ����أ�





//
bool drawmode = true;
bool mouseleft = false;
bool meshok = false;
bool draged = false;
int lx, ly;
//


//
triangulateio obj;//�������ǻ������������

//
vector<int> markerpointlist;//���Ƶ��±�



//raw line (user input)
float rawline[20480];//�û����Ƶ�����
int numrawline = 0;//������

int selectindex = -1;//��ǰѡ��Ŀ��Ƶ��±�


struct Vertex
{
	float position[2];
	//float normal[3];
	//float texcord[2];
};
struct Vertex3
{
	float position[3];
	//float normal[3];
	//float texcord[2];
};
struct	Texc
{
	//float position[2];
	//float normal[3];
	float texcord[2];
};

float corner[] = { -0.99,0.99,0,  -0.99,-0.99,0,  0.99,0.99,0,  0.99,-0.99,0 };
float backtexc[] = { 0,1.0,  0,0,  1.0,1.0, 1.0,0 };
int ind[] = { 0,1,2,1,2,3 };



int main(int argc, char *argv[]) {




	std::cout << "hello" << std::endl;
	getchar();
}


int getnearmarkeri(float glx, float gly, float& dist)  // �����Ż���һ��������Ҿ������  // ��ǰ�Ǽ�������룬Ȼ�����
{
	float dis, mindis = 999.0;
	int index = -1;

	for (int i = 0; i < markerpointlist.size(); i++)
	{ //����pow�İ취
		dis = pow(obj.pointlist[2 * markerpointlist[i]] - glx, 2) + pow(obj.pointlist[2 * markerpointlist[i] + 1] - gly, 2);
		if (dis < mindis)
		{
			mindis = dis;
			index = i;
		}
	}

	dist = mindis;
	return index;
}


void mouse(int button, int state, int x, int y) {
	switch (state) {
	case GLUT_DOWN:
		switch (button)
		{
		case GLUT_LEFT_BUTTON:
		{
			mouseleft = true;
			lx = x;
			ly = y;

			if (!meshok)
			{
				meshok = false;//�����־
				markerpointlist.clear();
				numrawline = 0;
				drawmode = true;
			}
			else
			{
				drawmode = false;

				if (markerpointlist.size() > 0)
				{
					float dist;
					float glx = (x - 0.5*window_width) / (0.5*window_width);
					float gly = (y - 0.5*window_height) / (0.5*window_height);

					int pin = getnearmarkeri(glx, gly, dist);
					
					if (dist < 0.1)
					{
						selectindex = pin;
					}
				}

			}
			


		}
		case GLUT_RIGHT_BUTTON:
		{}
		case GLUT_MIDDLE_BUTTON:
		{}
		default:
			break;
		}
		break;

	case GLUT_UP:
		switch (button)
		{
		case GLUT_LEFT_BUTTON:
		{}
		case GLUT_RIGHT_BUTTON:
		{}
		case GLUT_MIDDLE_BUTTON:
		{}
		default:
			break;
		}
		break;

	default:
		break;
	 }
}


/*void mouse(int button, int state, int x, int y)
{
	switch (state)
	{
	case GLUT_DOWN:
		switch (button)
		{
		case GLUT_LEFT_BUTTON://����������
		{
			mouseleft = true;//����������������
			lx = x;
			ly = y;
			if (!meshok)
			{
				meshok = false;//�����־
				markerpointlist.clear();
				numrawline = 0;
				drawmode = true;
			}
			else
			{
				drawmode = false;

				if (markerpointlist.size() > 0)
				{
					float dist;
					float glx = (x - 0.5*window_width) / (0.5*window_width);
					float gly = -(y - 0.5*window_height) / (0.5*window_height);

					int pin = getnearmarkeri(glx, gly, dist);

					if (dist < 0.1)
					{
						selectindex = pin;
					}
				}
			}
			break;
		}
		case GLUT_RIGHT_BUTTON:
		{
			reset();

			break;
		}
		case GLUT_MIDDLE_BUTTON:
		{
			float dist, mindist = 99;
			int index;
			int find = -1;
			for (int i = 0; i < obj.numberofpoints; i++)
			{
				dist = pow(obj.pointlist[2 * i] - (x - 0.5*window_width) / (0.5*window_width), 2) + pow(obj.pointlist[2 * i + 1] + (y - 0.5*window_height) / (0.5*window_height), 2);
				if (dist < mindist)
				{
					mindist = dist;
					index = i;
				}
			}
			for (int i = 0; i < markerpointlist.size(); i++)
			{
				if (markerpointlist[i] == index)
				{
					find = i;
					break;
				}
			}
			if (find == -1)
			{
				markerpointlist.push_back(index);
			}
			else
			{
				markerpointlist.erase(markerpointlist.begin() + find);
			}

			break;
		}

		default:
			break;
		}
		break;

	case GLUT_UP:
		switch (button)
		{
		case GLUT_LEFT_BUTTON:
		{
			selectindex = -1;
			mouseleft = false;
			if (!meshok)
			{
				if (numrawline < 10)//�������ͼ����10����Ͳ�����
				{
					break;
				}


				poly = absline(rawline, numrawline, &numpoly, 0.06);//����

				cout << "Sample point: " << numpoly << endl;

				//setlinepoint(poly, numpoly);//ʹ�ò�����Ķ��������ʾ

				delete (obj.pointlist);
				delete (obj.pointattributelist);
				delete (obj.pointmarkerlist);
				delete (obj.trianglelist);
				delete (obj.triangleattributelist);
				delete (obj.trianglearealist);
				delete (obj.neighborlist);
				delete (obj.segmentlist);
				delete (obj.segmentmarkerlist);
				delete (obj.edgelist);
				delete (obj.edgemarkerlist);

				obj = triangulate(poly, numpoly);//���ǻ�
				for (int i = 0; i < obj.numberofpoints; i++)
				{
					oldpointlist[2 * i] = obj.pointlist[2 * i];
					oldpointlist[2 * i + 1] = obj.pointlist[2 * i + 1];
				}



				int* indexbuffer = (int*)calloc(obj.numberoftriangles * 3, sizeof(int));


				getneighbor(obj);
				findboundary(obj);
				getlap();

				updateMesh(obj);
				cout << "Boundary Points Number: " << boundarypointi.size() << endl << "Boundary Edges Number: " << boundaryedges.size() / 2 << endl << "Internal Edges Number: " << internaledges.size() / 2 << endl;
				//��ȡ���ǻ����ÿһ���ߣ��Ա���ʾ
				for (unsigned int i = 0; i < obj.numberofedges; i++)
				{
					linelist[4 * i] = obj.pointlist[2 * obj.edgelist[2 * i]];//�߶���ʼ��x����
					linelist[4 * i + 1] = obj.pointlist[2 * obj.edgelist[2 * i] + 1];//�߶���ʼ��y����
					linelist[4 * i + 2] = obj.pointlist[2 * obj.edgelist[2 * i + 1]];//�߶��յ�x����
					linelist[4 * i + 3] = obj.pointlist[2 * obj.edgelist[2 * i + 1] + 1];//�߶��յ�y����
					//cout << linelist[2 * i] << "��" << linelist[2 * i+1] << "��" << linelist[2 * i+2] << "��" << linelist[2 * i+3] << endl;
				}

				//setlinepoint(linelist, obj.numberofedges * 2);//ʹ�����ǻ�����߶θ�����ʾ
				meshok = true;//���ǻ�������ϱ�־λ����
			}
			else
			{
				if (!draged)
				{
					float dist, mindist = 99;
					int index;
					int find = -1;
					for (int i = 0; i < obj.numberofpoints; i++)
					{
						dist = pow(obj.pointlist[2 * i] - (x - 0.5*window_width) / (0.5*window_width), 2) + pow(obj.pointlist[2 * i + 1] + (y - 0.5*window_height) / (0.5*window_height), 2);
						if (dist < mindist)
						{
							mindist = dist;
							index = i;
						}
					}
					for (int i = 0; i < markerpointlist.size(); i++)
					{
						if (markerpointlist[i] == index)
						{
							find = i;
							break;
						}
					}
					if (find == -1)
					{
						markerpointlist.push_back(index);
					}
					else
					{
						markerpointlist.erase(markerpointlist.begin() + find);
					}
				}
			}
			draged = false;

			break;
		}
		case GLUT_RIGHT_BUTTON:
		{

			break;
		}
		case GLUT_MIDDLE_BUTTON:
		{

			break;
		}

		default:
			break;
		}
		break;

	default:
		break;
	}

}


*/