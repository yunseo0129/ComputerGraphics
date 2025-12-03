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
				Blocks[i][j][k].Cube->SetColor(RandFloat0_1(), RandFloat0_1(),RandFloat0_1());
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

const bool CPuzzle::QuizData[3][3][4][5][4] = {

};