#include "VerifySelection.h"

void VerifySelection::Process(ACModel* model, Ray* ray)
{
	Ray transformedRay;
	Matrix inverseWorld;
	float distance;

	/*model->SetRenderBB(false);
	if (Ray::Intersects(ray, &model->BB, distance))
		model->SetRenderBB(true);*/
};