#pragma once
#include "Cube.h"

class CHand
{
public:
	CHand();
	~CHand();
	void Initialize(int _shaderID);
	void Update();
	void Render();
	void SetLClicked(bool val) { isLClicked = val; }
	void SetRClicked(bool val) { isRClicked = val; }

private:
	bool isLClicked = false;
	bool isRClicked = false;
	int iAnimFrame = 0;
	bool  isSwinging = false;  
	float currentRotX = 0.f;
	CCube* HandCube;
};