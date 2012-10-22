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

	float mCurrentTime;

	std::string mCurrentAnimation;

	UINT mCurrentKFIndex;

	BOOL mIsRunning;

	void UpdateBones(const Matrix& world);
	void UpdateBones(AMT_JOINT* joint);

public:
	ACAnimationController();
	ACAnimationController(AMT_MODEL* amtModel);
	~ACAnimationController();

	void SetAnimation(const std::string& name);
	void Run();
	void Pause();
	void Update(float elapsedTime, const Matrix& world);


};

#endif