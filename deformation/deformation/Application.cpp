#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> /* offsetof */
#include "C:\CXZ\deformation\deformation\GL\glew.h"
#include "C:\CXZ\deformation\deformation\GL\glut.h"
#include <iostream>
#include <vector>
#include<atlimage.h >

#include "C:\CXZ\deformation\deformation\include\Eigen\Eigen\Eigen"

using namespace Eigen;
using namespace std;

#include "C:\CXZ\deformation\deformation\shader_lib\shader.h"

//#include "LeastSquaresSparseSolver.h"

//#include "triangle.cpp"

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

//---------------------�������Լ�����Ķ���---------------------------------
void mouse(int button, int state, int x, int y);//��갴��������



void mousepos(int x, int y);//���λ�ô�����
//void addpoint(float x, float y);//���һ������
float* absline(float* in, unsigned int numin, unsigned int* numout, float value);//�����Ƶ��������в�������ɶ���Σ������ܼ��Ķ������Ϊϡ�趥�㣬in:���룬numin: ���붥��������numout���������������value���������ȣ�ֵԽС����Խ��
/*triangulateio triangulate(float* points, unsigned int numberofpoints);//������������������ǻ�
void updateBuffer(GLint vbo, void* pointlist, unsigned int size);//����VBO,IBO ������ͼ����
void updateMesh(triangulateio obj);
GLuint ATLLoadTexture(const char *fileName);
void initVAOLine();
void initVAOMesh();
void initVAOMarker();
void reset();

void getneighbor(triangulateio obj);
void findboundary(triangulateio obj);
int getnearmarkeri(float glx, float gly, float& dist);
void getlap();
void setb();
void setsolver(LeastSquaresSparseSolver &solver);
void solve();

int window_width;//��ǰ���ڿ�ȣ����أ�
int window_height;//��ǰ���ڸ߶ȣ����أ�

bool drawmode = true;//������ģʽ
bool mouseleft = false;//���������µı�־
bool meshok = false;//��������ǻ�����ı�־
bool draged = false;
int lx, ly;

triangulateio obj;//�������ǻ������������
float linelist[40960];//���ǻ���ɺ󴢴�������ʾ�����������б�,����ʱ�õģ��Ѿ�û����
float* poly;//������Ķ����������
unsigned int numpoly;//����������
float oldpointlist[40960];//����

//marker
float* markerdef;//���Ƶ���״�����
int markerlenth = 0;//���Ƶ���״����ε������
vector<int> markerpointlist;//���Ƶ��±�
float* marker;//���Ƶ���״��ǰʵ��
float markersize = 9;//���Ƶ��С(����)���Լ�����

//raw line (user input)
float rawline[20480];//�û����Ƶ�����
int numrawline = 0;//������

int selectindex = -1;//��ǰѡ��Ŀ��Ƶ��±�

float *lap;//���� laplace 
vector<int>* neighborlist;//neighborlist
float* boundarypoints;//������
int numboundarypoints;//����������
vector<int> boundarypointi, boundaryedges, internaledges, internalpointi;

vector<MatrixXf> RotList;
//��ģ��
GLuint vaoMesh;
GLuint vboMesh;
GLuint iboMesh;
GLuint vboTexc;

//����Ƶ�
GLuint vaoMarker;
GLuint vboMarker;
GLuint iboMarker;

//���������
GLuint vaoLine;
GLuint vboLine;
GLuint iboLine;

//����ͼ
GLuint text;

bool fix = true;

LeastSquaresSparseSolver solverx, solvery;
float **b = new float*[2];
*/
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