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
enum HitFace
{
	HIT_NONE,
	HIT_X_MIN, HIT_X_MAX,
	HIT_Y_MIN, HIT_Y_MAX,
	HIT_Z_MIN, HIT_Z_MAX
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
	void Click(bool isR) { isR ? isRClicked = true : isLClicked = true; }

	bool CheckCollision(const glm::vec3& camNextPos) const;

	void SetView(bool is) {
		isNormal = is;
	}

private:
	void Collision(bool isDel);
	bool RayAABB(const glm::vec3& O, const glm::vec3& D,
		const glm::vec3& Bmin, const glm::vec3& Bmax,
		float& tHit, HitFace& faceHit);
	void level1_1();
	void level1_2();
	void level1_3();
	void level2_1();
	void level2_2();
	void level2_3();
	void level3_1();
	void level3_2();
	void level3_3();

private:
	static const bool QuizData[3][3][4][5][4];
	Block Blocks[4][5][4] = {};
	bool isNormal = true;
	bool isLClicked = false;
	bool isRClicked = false;
};