#include "Default.h"
#include "Cube.h"
#include "Line.h"
#include "Camera.h"
#include "Pyramid.h"
#include "Obj.h"
#include "Circle.h"
#include "SkyBox.h"
#include "CRobot.h"
#include "KeyMgr.h"
#include "Camera1.h"
// 숙제

char* filetobuf(const char* file);
void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void KeyInput(unsigned char key, int x, int y);
void KeyUp(unsigned char key, int x, int y);
void MouseInput(int button, int state, int x, int y);
void DragInput(int x, int y);
void Animation(int value);
void Initial();
void DrawOrbit(glm::vec3 center, float radiusInner, float radiusOuter, int slices, float angleDeg);
void ResetBalls();
void InitMaze();
void MakeMaze(int x, int y);

float rangee = 15.f;
float minyy = 6.f;
float speedd = 0.1f;
int isV = 0;

GLint width, height;
GLuint shaderProgramID;
GLuint vertexShader;
GLuint fragmentShader;

int WinSizeX = 800;
int WinSizeY = 600;

int sizeX = 0;
int sizeZ = 0;
float scaleX = 0.f;
float scaleZ = 0.f;

bool isStop = false;
bool MazeEnd = false;

vector<CShape*> vecShape;
CRobot* robot = nullptr;

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WinSizeX, WinSizeY);
	glutCreateWindow("2019180037");
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		cout << "GLEW Initialized Succeced\n";

	// Shader
	make_vertexShaders();
	make_fragmentShaders();

	shaderProgramID = make_shaderProgram();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glClearColor(0.f, 0.f, 0.f, 0.f);

	glutKeyboardFunc(KeyInput);
	glutKeyboardUpFunc(KeyUp);
	glutMouseFunc(MouseInput);
	glutMotionFunc(DragInput);

	Initial();

	glutTimerFunc(10, Animation, 51);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}

char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb");
	if (!fptr)
		return NULL;
	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);
	buf = (char*)malloc(length + 1);
	fseek(fptr, 0, SEEK_SET);
	fread(buf, length, 1, fptr);
	fclose(fptr);
	buf[length] = 0;
	return buf;
}

void make_vertexShaders()
{
	GLchar* vertexSource;
	vertexSource = filetobuf("vertex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "“ERROR: vertex shader 컴파일 실패\n”" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders()
{
	GLchar* fragmentSource;
	fragmentSource = filetobuf("fragment.glsl");
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: frag_shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

GLuint make_shaderProgram()
{
	GLuint shaderID;
	GLint result;
	GLchar errorLog[512];
	shaderID = glCreateProgram();
	glAttachShader(shaderID, vertexShader);
	glAttachShader(shaderID, fragmentShader);
	glLinkProgram(shaderID);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		return false;
	}
	glUseProgram(shaderID);
	return shaderID;
}

GLvoid drawScene(GLvoid)
{
	glViewport(0, 0, WinSizeX, WinSizeY);
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CCamera::GetInstance()->SetMini(false);
	for (CShape* s : vecShape)
	{
		s->Draw();
	}
	if (robot != nullptr)
		robot->Draw();

	int mini = WinSizeX / 4;
	glViewport(WinSizeX - mini, 2 * mini, mini, mini);
	glClear(GL_DEPTH_BUFFER_BIT);
	CCamera::GetInstance()->SetMini(true);
	for (CShape* s : vecShape)
	{
		s->Draw();
	}
	if (robot != nullptr)
		robot->Draw();
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	WinSizeX = w;
	WinSizeY = h;
	glutPostRedisplay();
}

void KeyInput(unsigned char key, int x, int y)
{
	glm::mat4 Rot;

	switch (key)
	{
	case '7':
		speedd += 0.01f;
		if (speedd > 0.2f)
			speedd = 0.2f;
		break;
	case '8':
		speedd -= 0.01f;
		if (speedd < 0.f)
			speedd = 0.f;
		break;
	case '9':
		rangee += 1.f;
		if (rangee > 20.f)
			rangee = 20.f;
		break;
	case '0':
		rangee -= 1.f;
		if (rangee < 6.f)
			rangee = 6.f;
		break;
	case 'o':
		CCamera::GetInstance()->ChangePro(true);
		break;
	case 'p':
		CCamera::GetInstance()->ChangePro(false);
		break;
	case 'z':
		CCamera::GetInstance()->OnlyMoveZ(1.f);
		break;
	case 'Z':
		CCamera::GetInstance()->OnlyMoveZ(-1.f);
		break;
	case 'm':
		isStop = true;
		break;
	case 'M':
		isStop = false;
		break;
	case 'y':
		CCamera::GetInstance()->RotateY(1.f);
		break;
	case 'Y':
		CCamera::GetInstance()->RotateY(-1.f);
		break;
	case 'v':
		if (isV == 0)
			isV = 1;
		else if (isV == 3)
			isV = 2;
		break;
	case 'r':
		InitMaze();
		break;
	case 's':
		robot = new CRobot;
		robot->Initialize(glm::vec3((sizeX - 2) * scaleX + (scaleX / 2), 0.2f, 25.f - (scaleZ / 2)), shaderProgramID);
		robot->RotateY(180.f);
		robot->SetMirror(vecShape);
		robot->SetMirrorScale(scaleX / 2.f, scaleZ / 2.f);
		if (scaleX < scaleZ)
		{
			robot->SetScale(scaleX / 4.5f, scaleX / 4.5f, scaleX / 4.5f);
			CCamera1::GetInstance()->SetYpos(scaleX / 4.5f * 11.5f);
		}
		else
		{
			robot->SetScale(scaleZ / 4.5f, scaleZ / 4.5f, scaleZ / 4.5f);
			CCamera1::GetInstance()->SetYpos(scaleZ / 4.5f * 11.5f);
		}
		CCamera1::GetInstance()->SetRobot(robot);
		break;
	case '+':
		robot->SetSpeed(true);
		break;
	case '-':
		robot->SetSpeed(false);
		break;
	case '1':
		CCamera1::GetInstance()->Setview(true);
		CCamera1::GetInstance()->SetEnable(true);
		break;
	case '2':
		CCamera1::GetInstance()->SetEnable(false);
		break;
	case '3':
		CCamera1::GetInstance()->Setview(false);
		CCamera1::GetInstance()->SetEnable(true);
		break;
	case 'c':
		Initial();
		break;
	case 'q':
		glutLeaveMainLoop();
		return;
	default:
		break;
	}

	
	glutPostRedisplay();
}

void KeyUp(unsigned char key, int x, int y)
{

}

void MouseInput(int button, int state, int x, int y)
{
	GLclampf MouseX = (x / GLfloat(WinSizeX / 2)) - 1.f;
	GLclampf MouseY = -((y / GLfloat(WinSizeY / 2)) - 1.f);
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		
	}
	glutPostRedisplay();
}

void DragInput(int x, int y)
{
	
}

void Animation(int value)
{
	CCamera::GetInstance()->UpdateView();
	CKeyMgr::Get_Instance()->Key_Update();

	for (CShape* s : vecShape)
	{
		if (!s->IsDel())
			s->Update();
	}

	if (robot != nullptr)
		robot->Update();

	if (!isStop)
	{
		if (value > 1)
		{
			for (int i = 1; i < sizeX * sizeZ + 1; ++i)
			{
				if (!vecShape[i]->IsDel())
					vecShape[i]->GrowY(speedd);
			}
			value -= 1;
		}
		else
		{
			switch (isV)
			{
			case 1:
				if (speedd > 0.f)
					speedd -= 0.0005f;
				if (minyy > 3.f)
					minyy -= 0.1f;
				if (speedd <= 0.f)
				{
					speedd = 0.f;
					isV = 3;
				}
				break;
			case 2:
				if (speedd < 0.1f)
					speedd += 0.0005f;
				if (minyy < 6.f)
					minyy += 0.1f;
				if (speedd >= 0.1f)
				{
					isV = 0;
				}
				break;
			default:
				break;
			}
			for (int i = 1; i < sizeX * sizeZ + 1; ++i)
			{
				if (!vecShape[i]->IsDel())
					vecShape[i]->YChange();
			}
		}
	}
	glutPostRedisplay();
	glutTimerFunc(10, Animation, value);
}

void Initial()
{
	CCamera::GetInstance()->Initial();
	CCamera1::GetInstance()->SetEnable(false);
	CKeyMgr::Get_Instance();

	vecShape.clear();
	delete robot;
	robot = nullptr;
	MazeEnd = false;

	cout << "가로길이와 세로길이를 입력해주세요." << endl;
	cout << "* 길이 제한 : 5 ~ 25 *" << endl;
	cout << "입력 : ";
	cin >> sizeX >> sizeZ;
	scaleX = 25.f / (float)sizeX;
	scaleZ = 25.f / (float)sizeZ;

	CShape* map = new CCube;
	map->Initialize(glm::vec3(0.f, 0.f, 0.f), shaderProgramID);
	map->SetScale(25, 0.1, 25);
	static_cast<CCube*>(map)->SetColor(0.9f, 0.9f, 0.9f);
	vecShape.push_back(map);

	for (int x = 0; x < sizeX; ++x)
	{
		for (int z = 0; z < sizeZ; ++z)
		{
			CShape* block = new CCube;
			block->Initialize(glm::vec3(x * scaleX, 0.2f, z * scaleZ), shaderProgramID);
			block->SetScale(scaleX, -((x * scaleX) + (z * scaleZ) / 2.f), scaleZ);
			static_cast<CCube*>(block)->SetColor(RandFloat0_1(), RandFloat0_1(), RandFloat0_1());
			static_cast<CCube*>(block)->SetBlock();
			vecShape.push_back(block);
		}
	}

	cout << endl;
	cout << "==========================================================" << endl;
	cout << "                조작 설명서" << endl << endl;
	cout << "  o/p  : 투영을 선택한다 (직각 투영 / 원근 투영)" << endl;
	cout << "  7/8  : 육면체들의 움직이는 속도 증가 / 감소" << endl;
	cout << "  9/0  : 육면체들의 최대 높이값 증가 / 감소" << endl;
	cout << "  z/Z  : 원근 투영 시 z축으로 이동할 수 있게한다." << endl;
	cout << "  m/M  : 육면체들이 위 아래로 움직인다 / 멈춘다." << endl;
	cout << "  y/Y  : 카메라가 바닥의 y축을 기준으로 양 / 음 방향으로 회전한다." << endl;
	cout << "   v   : 육면체들 움직임이 멈추고 낮은 높이로 변한다. / 원상복귀" << endl;
	cout << "   r   : 미로 생성" << endl;
	cout << "   s   : 로봇 생성" << endl;
	cout << " 1/2/3 : 카메라 시점 1, 3인칭 변환 / 2: 맵뷰(기본카메라)" << endl;
	cout << " 방향키: 로봇 조작" << endl;
	cout << "  +/-  : 로봇의 속도 증가 / 감소" << endl;
	cout << "   c   : 초기화 후 재입력" << endl;
	cout << "   q   : 프로그램 종료" << endl;
}

void DrawOrbit(glm::vec3 center, float radiusInner, float radiusOuter, int slices, float angleDeg)
{
	glm::mat4 rot = glm::rotate(glm::mat4(1.f), glm::radians(angleDeg), glm::vec3(0.f, 0.f, 1.f));
	glm::mat4 trans = glm::translate(glm::mat4(1.f), center);

	glm::mat4 world = trans * rot;

	glm::mat4 MVP = CCamera::GetInstance()->GetMatProjView() * world;

	GLuint locMVP = glGetUniformLocation(shaderProgramID, "MVP");
	glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(MVP));

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < slices; ++i)
	{
		float theta = 2.0f * 3.1415926f * float(i) / float(slices);
		glm::vec4 p(radiusInner * cos(theta), 0.f, radiusInner * sin(theta), 1.f);
		p = world * p;
		glVertex3f(p.x, p.y, p.z);
	}
	glEnd();
}

void ResetBalls()
{
	CShape* ball = new CObj();
	static_cast<CObj*>(ball)->SetObj("Sphere.obj");
	ball->Initialize(glm::vec3(0.f, 0.f, 0.f), shaderProgramID);
	vecShape.push_back(ball);

}

void InitMaze()
{
	for (int i = 0; i < sizeX; ++i)
	{
		for (int j = 0; j < sizeZ; ++j)
		{
			if (i == 0)
			{
				vecShape[(i * sizeZ) + j + 1]->SetDontBreak();
			}
			else if (i == sizeX - 1)
			{
				vecShape[(i * sizeZ) + j + 1]->SetDontBreak();
			}
			else if (j == 0)
			{
				vecShape[(i * sizeZ) + j + 1]->SetDontBreak();
			}
			else if (j == sizeZ - 1)
			{
				vecShape[(i * sizeZ) + j + 1]->SetDontBreak();
			}
		}
	}
	MakeMaze(sizeX - 2, sizeZ - 1);
}

void MakeMaze(int x, int z)
{
	vecShape[(x * sizeZ) + z + 1]->SetDel();
	if (z == 1 && !MazeEnd)
	{
		vecShape[(x * sizeZ) + z]->SetDel();
		MazeEnd = true;
		return;
	}
	bool go[4] = { false, false, false, false };
	//			 { right, left , down , up    };

	while(true)
	{
		bool allno = true;

		if (x + 1 < sizeX && !vecShape[((x + 1) * sizeZ) + z + 1]->isWall())
		{
			go[0] = true;
			allno = false;
		}
		if (0 < x - 1 && !vecShape[((x - 1) * sizeZ) + z + 1]->isWall())
		{
			go[1] = true;
			allno = false;
		}
		if (z + 1 < sizeZ && !vecShape[(x * sizeZ) + z + 2]->isWall())
		{
			go[2] = true;
			allno = false;
		}
		if (0 < z - 1 && !vecShape[(x * sizeZ) + z]->isWall())
		{
			go[3] = true;
			allno = false;
		}

		if (allno)
			break;
		else
		{
			int cnt = 0;
			for (int i = 0; i < 4; ++i)
			{
				if (go[i])
					++cnt;
			}
			int ran = RandIntNum(cnt);
			cnt = 0;
			for (int i = 0; i < 4; ++i)
			{
				if (go[i])
				{
					if (cnt == ran)
					{
						switch (i)
						{
						case 0:
							if (z + 1 < sizeZ)
								vecShape[(x * sizeZ) + z + 2]->SetDontBreak();
							if (z - 1 > 0)
								vecShape[(x * sizeZ) + z]->SetDontBreak();
							MakeMaze(x + 1, z);
							break;
						case 1:
							if (z + 1 < sizeZ)
								vecShape[(x * sizeZ) + z + 2]->SetDontBreak();
							if (z - 1 > 0)
								vecShape[(x * sizeZ) + z]->SetDontBreak();
							MakeMaze(x - 1, z);
							break;
						case 2:
							if (x + 1 < sizeX)
								vecShape[((x + 1)*sizeZ) + z + 1]->SetDontBreak();
							if (x - 1 > 0)
								vecShape[((x - 1)*sizeZ) + z + 1]->SetDontBreak();
							MakeMaze(x, z + 1);
							break;
						case 3:
							if (x + 1 < sizeX)
								vecShape[((x + 1) * sizeZ) + z + 1]->SetDontBreak();
							if (x - 1 > 0)
								vecShape[((x - 1) * sizeZ) + z + 1]->SetDontBreak();
							MakeMaze(x, z - 1);
							break;
						default:
							break;
						}
						break;
					}
					++cnt;
				}
			}
		}
	}
}
