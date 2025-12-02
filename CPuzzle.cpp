#include "CPuzzle.h"

CPuzzle::CPuzzle()
{
}
CPuzzle::~CPuzzle()
{
	for (auto& cube : Blocks) {
		delete cube;
	}
}
void CPuzzle::Initialize(int _shaderID)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			for (int k = 0; k < 4; ++k)
			{
				Blocks[i][j][k].Cube = new CCube();
				Blocks[i][j][k].Cube->Initialize(glm::vec3(i, j - 1, k), _shaderID);
				Blocks[i][j][k].Cube->SetColor(RandFloat0_1(), RandFloat0_1(),RandFloat0_1());
				Blocks[i][j][k].Cube->Update();
			}
		}
	}
}
void CPuzzle::Update()
{

}
void CPuzzle::Render()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			for (int k = 0; k < 4; ++k)
			{
				if (Blocks[i][j][k].Exist)
					Blocks[i][j][k].Cube->Draw();
			}
		}
	}
}

bool CPuzzle::AnswerCheck() const
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			for (int k = 0; k < 4; ++k)
			{
				if (Blocks[i][j][k].Exist != Blocks[i][j][k].Quiz)
					return false;
			}
		}
	}
	return true;
}

void CPuzzle::LevelSet(int level)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			for (int k = 0; k < 4; ++k)
			{
				Blocks[i][j][k].Quiz = false;
				Blocks[i][j][k].Exist = false;
				if (i < level && j < level + 1 && k < level)
					Blocks[i][j][k].ban = false;
				else
					Blocks[i][j][k].ban = true;
			}
		}
	}
}
