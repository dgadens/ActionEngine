#include "ACAnimationController.h"

ACAnimationController::ACAnimationController()
{
	mpModel = nullptr;
	mCurrentInitKFIndex = 0;
	mCurrentEndKFIndex = 0;
	mCurrentTime = 0;
	mAnimationClipTotalTime = 0;
	mIsRunning = FALSE;
	mpSkinMatrizes = nullptr;
};

ACAnimationController::ACAnimationController(AMT_MODEL* amtModel)
{
	mIsRunning = TRUE;
	mpModel = amtModel;
	mCurrentInitKFIndex = 0;
	mCurrentEndKFIndex = 0;
	mAnimationClipTotalTime = 0;
	mCurrentTime = 0;
	mpRootAnimatedBone = nullptr;
	mpCurrentAnimation = nullptr;

	mpSkinMatrizes = new Matrix[amtModel->Head.NumJoints];

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
	SAFE_DELETE_A(mpSkinMatrizes);
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
				mAnimationClipTotalTime = mpRootAnimatedBone->KFData[mpCurrentAnimation->EndFrame].Time -
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
		//update na animacao
		if (mpRootAnimatedBone != nullptr)
		{
			mCurrentTime += elapsedTime;
			//se for maior entao ele pega o ultimo frame e interpola com o primeiro
			if (mCurrentTime > mAnimationClipTotalTime)
			{
				mCurrentTime = mCurrentTime - mAnimationClipTotalTime;
				mLerpAmount = 0.5f;
				mCurrentInitKFIndex = mpCurrentAnimation->StartFrame;
				mCurrentEndKFIndex = mpCurrentAnimation->EndFrame;
			}
			else
			{
				//iniciando do kfcorrente percorro atras de um kf q seja menor q to currenttime
				for (int i = mCurrentInitKFIndex; i < mpCurrentAnimation->EndFrame - 1; i++)
				{
					if (mpRootAnimatedBone->NumKF > i)
					{
						float ctime = mpRootAnimatedBone->KFData[i].Time - mStartTime;
						if (ctime <= mCurrentTime)
						{
							//ja q aqui eu sei q o current time é maior q o ctime entao vo atras do um kf q seja maior q o current time
							for (int j = i + 1; j < mpCurrentAnimation->EndFrame; j++)
							{
								float ntime = mpRootAnimatedBone->KFData[j].Time - mStartTime;
								if (ntime >= mCurrentTime)
								{
									//calculo o fator de interpolacao entre os tempos do frame atual e o proximo
									float frameTotalTime = ntime - ctime;
									mLerpAmount = (mCurrentTime - ctime) / frameTotalTime;
									mCurrentInitKFIndex = i;
									mCurrentEndKFIndex = j;
									break;
								}
							}
						}
					}
				}
			}

			UpdateBones(world);
			UpdateSkinMatrizes();
		}
	}
};

void ACAnimationController::UpdateBones(const Matrix& world)
{
	//pega o joint root e passa por todos os  joints
	AMT_JOINT* rootJoint = mpModel->pJoints[0];
	if (rootJoint->IsAnimated)
	{
		Matrix::Lerp(&rootJoint->KFData[mCurrentInitKFIndex].BindMatrix, 
					 &rootJoint->KFData[mCurrentEndKFIndex].BindMatrix,
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
			Matrix::Lerp(&joint->KFData[mCurrentInitKFIndex].BindMatrix, 
					     &joint->KFData[mCurrentEndKFIndex].BindMatrix,
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

void ACAnimationController::UpdateSkinMatrizes()
{
	for (int i = 0; i < mpModel->Head.NumJoints; i++)
		mpSkinMatrizes[i] = mpModel->pOriginalJoints[i]->InverseBindMatrix * mpModel->pJoints[i]->MatrixAbsolute;
};

Matrix* ACAnimationController::GetSkinMatrizes()
{
	return mpSkinMatrizes;
};
