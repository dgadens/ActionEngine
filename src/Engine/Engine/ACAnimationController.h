//****************************
// Classe que controla as animacoes e calcula as matrizes
// Creator: Daniel Gadens
// Date: 21/10/2012
//****************************


#ifndef __ACANIMATIONCONTROLLER_H
#define __ACANIMATIONCONTROLLER_H

#include <string>

#include "EngineMath.h"
#include "ACAMTStructs.h"

class ACAnimationController
{
private:
	AMT_MODEL* mpModel;

	//armazena as matrizes finais 
	Matrix* mpSkinMatrizes;

	//armazena o primeiro bone com animacao (é necessario pq pode haver bones sem animacao, mas depois todos sao animados)
	AMT_JOINT* mpRootAnimatedBone;
	//armazena a animacao corrente
	AMT_ANIMATION* mpCurrentAnimation;
	//KF corrent (pego o indice do kf inicial e o final pq ele pode pular um kf dependendo do elapsedtime)
	UINT mCurrentInitKFIndex;
	UINT mCurrentEndKFIndex;
	//tempo corrente dentro da animacao
	float mCurrentTime;
	//intervalor de tempo entre o frame final e inicial
	float mAnimationClipTotalTime;
	//tempo do frame inicial (para descontar do tempo total)
	float mStartTime;
	//fator de interpolacao da animacao entre as matrizes
	float mLerpAmount;

	BOOL mIsRunning;

	void UpdateBones(const Matrix& world);
	void UpdateBones(AMT_JOINT* joint);
	//update na posicao final de cada matriz (inversa * abs)
	void UpdateSkinMatrizes();

public:
	ACAnimationController();
	ACAnimationController(AMT_MODEL* amtModel);
	~ACAnimationController();

	void SetAnimation(const std::string& name);
	void Run();
	void Pause();
	void Update(float elapsedTime, const Matrix& world);

	Matrix* GetSkinMatrizes();

};

#endif