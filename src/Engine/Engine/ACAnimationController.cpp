#include "ACAnimationController.h"

ACAnimationController::ACAnimationController()
{
	mpModel = nullptr;
	mCurrentKFIndex = 0;
	mCurrentTime = 0;
	mIsRunning = FALSE;
};

ACAnimationController::ACAnimationController(AMT_MODEL* amtModel)
{
	mpModel = amtModel;
	mCurrentKFIndex = 0;
	mCurrentTime = 0;
	mIsRunning = FALSE;
};

ACAnimationController::~ACAnimationController()
{
};

void ACAnimationController::SetAnimation(const std::string& name)
{
	if (mCurrentAnimation != name)
	{
		mCurrentAnimation = name;
		mCurrentTime = 0;
	}
};

void ACAnimationController::Run()
{
	mIsRunning = TRUE;
};

void ACAnimationController::Pause()
{
	mIsRunning = FALSE;
};

void ACAnimationController::Update(float elapsedTime, const Matrix& world)
{
	if (mIsRunning && mpModel != nullptr)
	{
		mCurrentTime += elapsedTime;

		//procuro o kf
		mCurrentKFIndex = 999999;
		//primeiro pego o primeiro joint animado
		AMT_JOINT* animatedJoint = nullptr;
		for (int b = 0; b < mpModel->Head.NumJoints; b++)
		{
			if (mpModel->pJoints[b]->IsAnimated)
			{
				animatedJoint = mpModel->pJoints[b];
				break;
			}
		}

		if (animatedJoint != nullptr)
		{
			for (int i = 0; i < mpModel->Head.NumFrames-1; i++)
			{
				if (animatedJoint->NumKF > i)
				{
					if (animatedJoint->KFData[i].Time <= mCurrentTime &&
						animatedJoint->KFData[i+1].Time >= mCurrentTime)
					{
						mCurrentKFIndex = i;
						break;
					}
				}
			}
			if (mCurrentKFIndex == 999999)
			{
				mCurrentKFIndex = 0;
				mCurrentTime = 0;
			}

			UpdateBones(world);
		}
	}
};

void ACAnimationController::UpdateBones(const Matrix& world)
{
	//pega o joint root e passa por todos os  joints
	AMT_JOINT* rootJoint = mpModel->pJoints[0];
	if (rootJoint->IsAnimated)
		rootJoint->BindMatrix = rootJoint->KFData[mCurrentKFIndex].BindMatrix * world;
	else
		rootJoint->BindMatrix = mpModel->pOriginalJoints[0]->BindMatrix * world;

	rootJoint->MatrixAbsolute = rootJoint->BindMatrix;

	for (int i = 0; i < rootJoint->NumChildren; i++)
		UpdateBones(mpModel->pJoints[rootJoint->JointChildren[i]]);
};

void ACAnimationController::UpdateBones(AMT_JOINT* joint)
{
	if (joint->ParentID != -1)
	{
		if (joint->IsAnimated)
			joint->MatrixAbsolute = joint->KFData[mCurrentKFIndex].BindMatrix * mpModel->pJoints[joint->ParentID]->MatrixAbsolute;
		else
			joint->MatrixAbsolute = joint->BindMatrix * mpModel->pJoints[joint->ParentID]->MatrixAbsolute;
	}

	if (joint->NumChildren <= 0)
		return;

	for (int i = 0; i < joint->NumChildren; i++)
		UpdateBones(mpModel->pJoints[joint->JointChildren[i]]);
};

