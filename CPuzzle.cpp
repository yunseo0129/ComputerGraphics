#include "CPuzzle.h"
#include "Camera.h"

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
				float r, g, b;
				while (true)
				{
					r = RandFloat0_1();
					g = RandFloat0_1();
					b = RandFloat0_1();
					if (std::fabs(r - 0.2f) > 0.05f ||
						std::fabs(g - 0.6f) > 0.05f ||
						std::fabs(b - 0.8f) > 0.05f)
					{
						break;
					}
				}
				Blocks[i][j][k].Cube->SetColor(r, g, b);
				Blocks[i][j][k].Cube->Update();
			}
		}
	}
}
void CPuzzle::Update()
{
	if (isLClicked)
	{
		isLClicked = false;
		Collision(true);
		
	}
	if (isRClicked)
	{
		isRClicked = false;
		Collision(false);
	}
}
void CPuzzle::Render()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			for (int k = 0; k < 4; ++k)
			{
				if (isNormal)
				{
					if (Blocks[i][j][k].Exist)
						Blocks[i][j][k].Cube->Draw();
				}
				else
				{
					if (Blocks[i][j][k].Quiz)
						Blocks[i][j][k].Cube->Draw();
				}
			}
		}
	}
}

bool CPuzzle::AnswerCheck() const
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 1; j < 5; ++j)
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
				if (i < level + 1 && j < level + 2 && k < level + 1)
				{
					Blocks[i][j][k].ban = false;
					if (j == 0)
						Blocks[i][j][k].Exist = true;
				}
				else
					Blocks[i][j][k].ban = true;
			}
		}
	}

	int ran = RandIntNum(3);
	if (level == 1)
	{
		if (ran == 0)
			level1_1();
		else if (ran == 1)
			level1_2();
		else if (ran == 2)
			level1_3();
	}
	else if (level == 2)
	{
		if (ran == 0)
			level2_1();
		else if (ran == 1)
			level2_2();
		else if (ran == 2)
			level2_3();
	}
	else if (level == 3)
	{
		if (ran == 0)
			level3_1();
		else if (ran == 1)
			level3_2();
		else if (ran == 2)
			level3_3();
	}
}

void CPuzzle::Collision(bool isDel)
{
	glm::vec3 rayorigin = CCamera::GetInstance()->GetPos();
	glm::vec3 raydir = glm::normalize(CCamera::GetInstance()->GetLook());

	float nearestDist = 100.f;
	int hitX = -1, hitY = -1, hitZ = -1;
	HitFace hitFaceFinal = HIT_NONE;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			for (int k = 0; k < 4; ++k)
			{
				if (Blocks[i][j][k].ban || !Blocks[i][j][k].Exist)
					continue;
				glm::vec3 Bmin = Blocks[i][j][k].Cube->GetPosition();
				glm::vec3 Bmax = Blocks[i][j][k].Cube->GetPosition() + glm::vec3(1.f);

				float tHit = 0.f;
				HitFace face;
				if (RayAABB(rayorigin, raydir, Bmin, Bmax, tHit, face))
				{
					if (tHit > 0 && tHit < nearestDist)
					{
						nearestDist = tHit;
						hitX = i;
						hitY = j;
						hitZ = k;
						hitFaceFinal = face;
					}
				}
			}
		}
	}
	if (nearestDist < 100.f)
	{
		if (isDel)
		{
			if (hitY == 0)
				return;
			Blocks[hitX][hitY][hitZ].Exist = false;
		}
		else
		{
			if (hitFaceFinal == HIT_Y_MAX && hitY + 1 < 5)
			{
				if (!Blocks[hitX][hitY + 1][hitZ].ban)
					Blocks[hitX][hitY + 1][hitZ].Exist = true;
			}

			if (hitFaceFinal == HIT_Y_MIN && hitY - 1 >= 0)
			{
				if (!Blocks[hitX][hitY - 1][hitZ].ban)
					Blocks[hitX][hitY - 1][hitZ].Exist = true;
			}
			if (hitFaceFinal == HIT_X_MAX && hitX + 1 < 4)
			{
				if (!Blocks[hitX + 1][hitY][hitZ].ban)
					Blocks[hitX + 1][hitY][hitZ].Exist = true;
			}
			if (hitFaceFinal == HIT_X_MIN && hitX - 1 >= 0)
			{
				if (!Blocks[hitX - 1][hitY][hitZ].ban)
					Blocks[hitX - 1][hitY][hitZ].Exist = true;
			}
			if (hitFaceFinal == HIT_Z_MAX && hitZ + 1 < 4)
			{
				if (!Blocks[hitX][hitY][hitZ + 1].ban)
					Blocks[hitX][hitY][hitZ + 1].Exist = true;
			}
			if (hitFaceFinal == HIT_Z_MIN && hitZ - 1 >= 0)
			{
				if (!Blocks[hitX][hitY][hitZ - 1].ban)
					Blocks[hitX][hitY][hitZ - 1].Exist = true;
			}
		}
	}
}

bool CPuzzle::RayAABB(const glm::vec3& O, const glm::vec3& D, const glm::vec3& Bmin, const glm::vec3& Bmax, float& tHit, HitFace& faceHit)
{
	faceHit = HIT_NONE;

	float tmin = -FLT_MAX;
	float tmax = FLT_MAX;

	int hitAxis = -1;

	for (int axis = 0; axis < 3; axis++)
	{
		float invD = 1.0f / D[axis];
		float t0 = (Bmin[axis] - O[axis]) * invD;
		float t1 = (Bmax[axis] - O[axis]) * invD;

		float tmin_axis = std::min(t0, t1);
		float tmax_axis = std::max(t0, t1);

		if (tmin_axis > tmin)
		{
			tmin = tmin_axis;
			hitAxis = axis;
		}

		tmax = std::min(tmax, tmax_axis);

		if (tmax < tmin) return false;
	}

	tHit = tmin;
	if (hitAxis == 0)
		faceHit = (D.x > 0 ? HIT_X_MIN : HIT_X_MAX);
	else if (hitAxis == 1)
		faceHit = (D.y > 0 ? HIT_Y_MIN : HIT_Y_MAX);
	else if (hitAxis == 2)
		faceHit = (D.z > 0 ? HIT_Z_MIN : HIT_Z_MAX);

	return true;
}

void CPuzzle::level1_1()
{
	Blocks[0][1][0].Quiz = false;
	Blocks[1][1][0].Quiz = true;
	Blocks[0][1][1].Quiz = true;
	Blocks[1][1][1].Quiz = true;
	Blocks[0][2][0].Quiz = false;
	Blocks[1][2][0].Quiz = false;
	Blocks[0][2][1].Quiz = false;
	Blocks[1][2][1].Quiz = true;
}

void CPuzzle::level1_2()
{
	Blocks[0][1][0].Quiz = true;
	Blocks[1][1][0].Quiz = true;
	Blocks[0][1][1].Quiz = true;
	Blocks[1][1][1].Quiz = true;
	Blocks[1][2][1].Quiz = true;
}

void CPuzzle::level1_3()
{
	Blocks[0][1][0].Quiz = true;
	Blocks[0][1][1].Quiz = true;
	Blocks[1][1][0].Quiz = true;
	Blocks[0][2][1].Quiz = true;
}

void CPuzzle::level2_1()
{
	Blocks[0][1][1].Quiz = true;

	Blocks[1][1][0].Quiz = true;
	Blocks[1][2][0].Quiz = true;
	Blocks[1][1][1].Quiz = true;

	Blocks[2][1][0].Quiz = true;
	Blocks[2][2][0].Quiz = true;
	Blocks[2][1][1].Quiz = true;
	Blocks[2][1][2].Quiz = true;
}

void CPuzzle::level2_2()
{
	Blocks[0][1][2].Quiz = true;
	Blocks[0][2][2].Quiz = true;

	Blocks[1][1][0].Quiz = true;
	Blocks[1][1][2].Quiz = true;
	Blocks[1][2][2].Quiz = true;
	Blocks[1][3][2].Quiz = true;

	Blocks[2][1][1].Quiz = true;
	Blocks[2][1][2].Quiz = true;
}

void CPuzzle::level2_3()
{
	Blocks[0][1][2].Quiz = true;

	Blocks[1][1][0].Quiz = true;
	Blocks[1][2][0].Quiz = true;
	Blocks[1][3][0].Quiz = true;
	Blocks[1][1][1].Quiz = true;
	Blocks[1][2][1].Quiz = true;

	Blocks[2][1][0].Quiz = true;
}

void CPuzzle::level3_1()
{
	Blocks[0][1][0].Quiz = true;

	Blocks[1][1][1].Quiz = true;
	Blocks[1][2][1].Quiz = true;

	Blocks[2][1][2].Quiz = true;
	Blocks[2][2][2].Quiz = true;
	Blocks[2][3][2].Quiz = true;

	Blocks[3][1][3].Quiz = true;
	Blocks[3][2][3].Quiz = true;
	Blocks[3][3][3].Quiz = true;
	Blocks[3][4][3].Quiz = true;
}

void CPuzzle::level3_2()
{
	Blocks[0][1][3].Quiz = true;
	Blocks[0][2][3].Quiz = true;
	Blocks[0][3][3].Quiz = true;
	Blocks[0][4][3].Quiz = true;

	Blocks[1][1][2].Quiz = true;
	Blocks[1][2][2].Quiz = true;
	Blocks[1][3][2].Quiz = true;

	Blocks[2][1][1].Quiz = true;
	Blocks[2][2][1].Quiz = true;

	Blocks[3][1][0].Quiz = true;
}

void CPuzzle::level3_3()
{
	Blocks[0][1][2].Quiz = true;
	Blocks[0][2][2].Quiz = true;

	Blocks[1][1][0].Quiz = true;
	Blocks[1][2][0].Quiz = true;
	Blocks[1][3][0].Quiz = true;
	Blocks[1][4][0].Quiz = true;

	Blocks[2][1][3].Quiz = true;
	Blocks[2][2][3].Quiz = true;
	Blocks[2][3][3].Quiz = true;

	Blocks[3][1][1].Quiz = true;
}



bool CPuzzle::CheckCollision(const glm::vec3& camNextPos) const
{
	glm::vec3 camMin = camNextPos + glm::vec3(-0.4f, -1.7f, -0.4f);
	glm::vec3 camMax = camNextPos + glm::vec3(0.4f, 0.2f, 0.4f);

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			for (int k = 0; k < 4; ++k)
			{
				if (!Blocks[i][j][k].Exist)
					continue;

				glm::vec3 blkMin(i, j - 1, k);
				glm::vec3 blkMax = blkMin + glm::vec3(1.0f);

				if (camMax.x > blkMin.x && camMin.x < blkMax.x &&
					camMax.y > blkMin.y && camMin.y < blkMax.y &&
					camMax.z > blkMin.z && camMin.z < blkMax.z)
				{
					return true; 
				}
			}
		}
	}

	return false; 
}

