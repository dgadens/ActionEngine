
#ifndef __GENERICTOOLS_H
#define __GENERICTOOLS_H

#include "ActionTypeDef.h"
#include <WTypes.h>
#include <string>
#include <sstream>
#include <math.h>

#define SAFE_DELETE(p)   { if(p) { delete (p);     (p)=nullptr; } }
#define SAFE_DELETE_A(p) { if(p) { delete [] (p);  (p)=nullptr; } }
#define SAFE_FREE(p)     { if(p) { free(p);        (p)=nullptr; } }
#define SAFE_RELEASE(p)  { if(p) { (p)->Release(); (p)=nullptr; } }

//Usado para limpar os container, limpa ja a memoria
template <typename T> void SAFE_MAP_DELETE_CLEAR( T & amap ) 
{
    for ( auto it = amap.begin(); it != amap.end(); ++it ) {
        SAFE_DELETE(it->second);
    }
    amap.clear();
}

template <typename T> void SAFE_MAP_RELEASE_CLEAR( T & amap ) 
{
    for ( auto it = amap.begin(); it != amap.end(); ++it ) {
        SAFE_RELEASE(it->second);
    }
    amap.clear();
}

class GenericTools
{
public:

	static FLOAT ConvertPixelsToClipSpace( const UINT pixelDimension, const UINT pixels );
	static FLOAT ConvertPixelsToClipSpaceDistance( const UINT pixelDimension, const UINT pixels );

	static BOOL WithinEpsilon(FLOAT a, FLOAT b);
	static FLOAT Sign(FLOAT value);
	static FLOAT Max(FLOAT val1, FLOAT val2);
	static FLOAT Min(FLOAT val1, FLOAT val2);
	static FLOAT IEEERemainder(FLOAT x, FLOAT y);

	static void ClearCharArray(char* value, int size);

	static LPWSTR StringToLPWSTR(std::string value);

	static int StringToInt(std::string &value);
	static unsigned int StringToUInt(std::string value);
	static long StringToLong(std::string value);
	static unsigned long StringToULong(std::string value);
	static float StringToFloat(std::string value);
	static double StringToDouble(std::string value);

	static std::string IntToString(int value);
	static std::string UIntToString(unsigned int value);
	static std::string LongToString(long value);
	static std::string ULongToString(unsigned long value);
	static std::string FloatToString(float value);
    static std::string DoubleToString(double value);
};

inline FLOAT GenericTools::ConvertPixelsToClipSpace( const UINT pixelDimension, const UINT pixels )
{
	return (FLOAT)pixels/pixelDimension*2 -1;
}

inline FLOAT GenericTools::ConvertPixelsToClipSpaceDistance( const UINT pixelDimension, const UINT pixels )
{
	return (FLOAT)pixels/pixelDimension*2;
}

inline BOOL GenericTools::WithinEpsilon(FLOAT a, FLOAT b)
{
	float num = a - b;
	return ((-1.401298E-45f <= num) && (num <= FLOAT_EPSILON));
};

inline FLOAT GenericTools::Sign(FLOAT value)
{
	if (value < 0.0)
	{
		return -1;
	}
	if (value > 0.0)
	{
		return 1;
	}
	return 0;
};

inline FLOAT GenericTools::Max(FLOAT val1, FLOAT val2)
{
	if (val1 > val2)
	{
		return val1;
	}
	return val2;
};

inline FLOAT GenericTools::Min(FLOAT val1, FLOAT val2)
{
	if (val1 < val2)
	{
		return val1;
	}
	return val2;
};

inline void GenericTools::ClearCharArray(char* value, int size)
{
	for (int i = 0; i < size; i++)
		value[i] = '\0';
};

inline int GenericTools::StringToInt(std::string &value)
{
	int ret;
	std::istringstream stream(value);
	stream >> std::dec >> ret;
	return ret;
};

inline unsigned int GenericTools::StringToUInt(std::string value)
{
	unsigned int ret;
	std::istringstream stream(value);
	stream >> std::dec >> ret;
	return ret;
};
	
inline long GenericTools::StringToLong(std::string value)
{
	long ret;
	std::istringstream stream(value);
	stream >> std::dec >> ret;
	return ret;
};

inline unsigned long GenericTools::StringToULong(std::string value)
{
	unsigned long ret;
	std::istringstream stream(value);
	stream >> std::dec >> ret;
	return ret;
};

inline float GenericTools::StringToFloat(std::string value)
{
	float ret;
	std::istringstream stream(value);
	stream >> std::dec >> ret;
	return ret;
};

inline double GenericTools::StringToDouble(std::string value)
{
	double ret;
	std::istringstream stream(value);
	stream >> std::dec >> ret;
	return ret;
};

inline std::string GenericTools::IntToString(int value)
{
	std::stringstream stream;
	std::string ret;
	stream << value;
	stream >> ret;

	return ret;
};

inline std::string GenericTools::UIntToString(unsigned int value)
{
	std::stringstream stream;
	std::string ret;
	stream << value;
	stream >> ret;

	return ret;
};

inline std::string GenericTools::LongToString(long value)
{
	std::stringstream stream;
	std::string ret;
	stream << value;
	stream >> ret;

	return ret;
};

inline std::string GenericTools::ULongToString(unsigned long value)
{
	std::stringstream stream;
	std::string ret;
	stream << value;
	stream >> ret;

	return ret;
};

inline std::string GenericTools::FloatToString(float value)
{
	std::stringstream stream;
	std::string ret;
	stream << value;
	stream >> ret;

	return ret;
};

inline std::string GenericTools::DoubleToString(double value)
{
	std::stringstream stream;
	std::string ret;
	stream << value;
	stream >> ret;

	return ret;
};

inline LPWSTR GenericTools::StringToLPWSTR(std::string value)
{
	int buffSize = value.size() + 1;
	LPWSTR str = new WCHAR[buffSize];
	MultiByteToWideChar(CP_ACP, 0, value.c_str(), buffSize, str, buffSize);

	return str;
};

#endif
