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

//---------------------以下是自己定义的东东---------------------------------
void mouse(int button, int state, int x, int y);//鼠标按键处理函数



void mousepos(int x, int y);//鼠标位置处理函数
//void addpoint(float x, float y);//添加一个顶点
float* absline(float* in, unsigned int numin, unsigned int* numout, float value);//将绘制的线条进行采样，变成多边形，即将密集的顶点采样为稀疏顶点，in:输入，numin: 输入顶点数量，numout：输出顶点数量，value：采样长度，值越小精度越高
/*triangulateio triangulate(float* points, unsigned int numberofpoints);//将多边形轮廓顶点三角化
void updateBuffer(GLint vbo, void* pointlist, unsigned int size);//更新VBO,IBO 或者贴图坐标
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

int window_width;//当前窗口宽度（像素）
int window_height;//当前窗口高度（像素）

bool drawmode = true;//画轮廓模式
bool mouseleft = false;//鼠标左键按下的标志
bool meshok = false;//已完成三角化网格的标志
bool draged = false;
int lx, ly;

triangulateio obj;//储存三角化后的网格数据
float linelist[40960];//三角化完成后储存用于显示的网格线条列表,测试时用的，已经没用了
float* poly;//采样后的多边形轮廓点
unsigned int numpoly;//轮廓点数量
float oldpointlist[40960];//如名

//marker
float* markerdef;//控制点形状多边形
int markerlenth = 0;//控制点形状多边形点的数量
vector<int> markerpointlist;//控制点下标
float* marker;//控制点形状当前实例
float markersize = 9;//控制点大小(像素)，自己控制

//raw line (user input)
float rawline[20480];//用户绘制的线条
int numrawline = 0;//点数量

int selectindex = -1;//当前选择的控制点下标

float *lap;//轮廓 laplace 
vector<int>* neighborlist;//neighborlist
float* boundarypoints;//轮廓点
int numboundarypoints;//轮廓点数量
vector<int> boundarypointi, boundaryedges, internaledges, internalpointi;

vector<MatrixXf> RotList;
//存模型
GLuint vaoMesh;
GLuint vboMesh;
GLuint iboMesh;
GLuint vboTexc;

//存控制点
GLuint vaoMarker;
GLuint vboMarker;
GLuint iboMarker;

//存绘制线条
GLuint vaoLine;
GLuint vboLine;
GLuint iboLine;

//存贴图
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
		case GLUT_LEFT_BUTTON://鼠标左键按下
		{
			mouseleft = true;//设置鼠标左键被按下
			lx = x;
			ly = y;
			if (!meshok)
			{
				meshok = false;//清除标志
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
				if (numrawline < 10)//如果鼠标绘图少于10个点就不处理
				{
					break;
				}


				poly = absline(rawline, numrawline, &numpoly, 0.06);//采样

				cout << "Sample point: " << numpoly << endl;

				//setlinepoint(poly, numpoly);//使用采样后的顶点更新显示

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

				obj = triangulate(poly, numpoly);//三角化
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
				//读取三角化后的每一条边，以便显示
				for (unsigned int i = 0; i < obj.numberofedges; i++)
				{
					linelist[4 * i] = obj.pointlist[2 * obj.edgelist[2 * i]];//线段起始点x坐标
					linelist[4 * i + 1] = obj.pointlist[2 * obj.edgelist[2 * i] + 1];//线段起始点y坐标
					linelist[4 * i + 2] = obj.pointlist[2 * obj.edgelist[2 * i + 1]];//线段终点x坐标
					linelist[4 * i + 3] = obj.pointlist[2 * obj.edgelist[2 * i + 1] + 1];//线段终点y坐标
					//cout << linelist[2 * i] << "，" << linelist[2 * i+1] << "，" << linelist[2 * i+2] << "，" << linelist[2 * i+3] << endl;
				}

				//setlinepoint(linelist, obj.numberofedges * 2);//使用三角化后的线段更新显示
				meshok = true;//三角化网格完毕标志位设置
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