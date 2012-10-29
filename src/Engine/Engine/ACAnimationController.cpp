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
	mIsRunning = TRUE;
	mpModel = amtModel;
	mCurrentKFIndex = 0;
	mCurrentTime = 0;
	mpRootAnimatedBone = nullptr;
	mpCurrentAnimation = nullptr;

	//primeiro pego o primeiro joint animado
	for (int b = 0; b < mpModel->Head.NumJoints; b++)
	{
		if (mpModel->pJoints[b]->IsAnimated)
		{
			mpRootAnimatedBone = mpModel->pJoints[b];
			break;
		}
	}

	//seta a primeira animacao como padrao
	SetAnimation(mpModel->pAnimations[0]->Name);
};

ACAnimationController::~ACAnimationController()
{
};

void ACAnimationController::SetAnimation(const std::string& name)
{
	//procura a animacao
	if (mpCurrentAnimation == nullptr || mpCurrentAnimation->Name != name)
	{
		if (mpCurrentAnimation != nullptr)
			mpCurrentAnimation->Active = FALSE;

		for (int i = 0; i < mpModel->Head.NumAnimations; i++)
		{
			if (mpModel->pAnimations[i]->Name == name)
			{
				mpCurrentAnimation = mpModel->pAnimations[i];
				mpCurrentAnimation->Active = TRUE;
				mCurrentTime = 0;

				mStartTime = mpRootAnimatedBone->KFData[mpCurrentAnimation->StartFrame].Time;
				//intervalo entre os frames
				mKFTotalTime = mpRootAnimatedBone->KFData[mpCurrentAnimation->EndFrame].Time -
						       mStartTime;

				break;
			}
		}
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
		if (mpRootAnimatedBone != nullptr)
		{
			for (int i = mCurrentKFIndex; i < mpModel->Head.NumFrames - 1; i++)
			{
				if (mpRootAnimatedBone->NumKF > i)
				{
					float ctime = mpRootAnimatedBone->KFData[i].Time - mStartTime;
					float ntime = mpRootAnimatedBone->KFData[i+1].Time - mStartTime;
					if (ctime <= mCurrentTime &&
						ntime >= mCurrentTime)
					{
						//calculo o fator de interpolacao entre os tempos do frame atual e o proximo
						float frameTotalTime = ntime - ctime;
						mLerpAmount = mCurrentTime / frameTotalTime;
						mCurrentKFIndex = i;
						break;
					}
				}
			}

			UpdateBones(world);
			mCurrentTime += elapsedTime;
			if (mCurrentTime > mKFTotalTime)
			{
				mCurrentTime = mCurrentTime - mKFTotalTime;
				mCurrentKFIndex = mpCurrentAnimation->StartFrame;
			}
		}
	}
};

void ACAnimationController::UpdateBones(const Matrix& world)
{
	//pega o joint root e passa por todos os  joints
	AMT_JOINT* rootJoint = mpModel->pJoints[0];
	if (rootJoint->IsAnimated)
	{
		Matrix::Lerp(&rootJoint->KFData[mCurrentKFIndex].BindMatrix, 
					 &rootJoint->KFData[mCurrentKFIndex + 1].BindMatrix,
					 mLerpAmount,
					 &rootJoint->BindMatrix);

		rootJoint->BindMatrix = rootJoint->BindMatrix * world;
	}
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
		{
			Matrix::Lerp(&joint->KFData[mCurrentKFIndex].BindMatrix, 
					     &joint->KFData[mCurrentKFIndex + 1].BindMatrix,
					     mLerpAmount,
					     &joint->BindMatrix);

			joint->MatrixAbsolute = joint->BindMatrix * mpModel->pJoints[joint->ParentID]->MatrixAbsolute;
		}
		else
			joint->MatrixAbsolute = joint->BindMatrix * mpModel->pJoints[joint->ParentID]->MatrixAbsolute;
	}

	if (joint->NumChildren <= 0)
		return;

	for (int i = 0; i < joint->NumChildren; i++)
		UpdateBones(mpModel->pJoints[joint->JointChildren[i]]);
};

void ACAnimationController::ComputeKFLerp(int currentKF, Matrix& outMatrix)
{

};
