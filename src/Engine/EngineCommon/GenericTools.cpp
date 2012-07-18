#include "GenericTools.h"

FLOAT GenericTools::IEEERemainder(FLOAT x, FLOAT y)
{
	if (y==0.0f)
		return 0;
	
	FLOAT d = (FLOAT)(int(x) % int(y));
	if ((d == 0.0) && x < 0.0f)
		return 0.0f;

	FLOAT num2 = d - (abs(y) * Sign(x));
	if (abs(num2) == abs(d))
	{
		FLOAT a = x / y;
		if (abs(a) > abs(a))
		{
			return num2;
		}
		return d;
	}
	if (abs(num2) < abs(d))
	{
		return num2;
	}
	return d;
};
