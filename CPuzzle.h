#pragma once
#include "Default.h"
#include "Cube.h"

struct Block
{
	CCube* Cube = nullptr;
	bool Exist = false;
	bool Quiz = false;
	bool ban = true;
};

class CPuzzle
{
public:
	CPuzzle();
	~CPuzzle();
	void Initialize(int _shaderID);
	void Update();
	void Render();

public:
	bool AnswerCheck() const;
	void LevelSet(int level);

private:
	Block Blocks[4][5][4] = {};
};