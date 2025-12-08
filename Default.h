#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define VK_MAX 0xff
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <random>
#include <vector>
#include <string>

using namespace std;

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

enum eVIEW { VIEW_NORMAL, VIEW_UI, VIEW_FRONT, VIEW_SIDE, VIEW_UP };

inline GLfloat RandFloat0_1()
{
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<float> dist(0.0f, 1.0f);
	return dist(gen);
}
inline GLfloat RandFloat(float min, float max)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<float> dist(min, max);
	return dist(gen);
}

inline int RandIntNum(int cnt)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dist(0, cnt - 1);
	return dist(gen);
}

struct MYPOINT
{
	GLfloat x = 0.f;
	GLfloat y = 0.f;
	GLfloat z = 0.f;
};

struct COLOR
{
	GLfloat r = RandFloat0_1();
	GLfloat g = RandFloat0_1();
	GLfloat b = RandFloat0_1();
};

struct VerTex
{
	glm::vec3 pos;
	glm::vec3 color = glm::vec3(RandFloat0_1(), RandFloat0_1(), RandFloat0_1());;
};

struct Face
{
	GLuint v1;
	GLuint v2;
	GLuint v3;
};

enum eDIRECTION { DIR_NONE, DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN, DIR_LU, DIR_RU, DIR_LD, DIR_RD };
enum eORDER { ORDER_NONE, ORDER_POINT, ORDER_LINE, ORDER_TRIANGLE, ORDER_RECTANGLE };
enum eCUBEPOS { PLUS_X, PLUS_Y, PLUS_Z, MINUS_X, MINUS_Y, MINUS_Z };

extern float rangee;
extern float minyy;
extern float speedd;
