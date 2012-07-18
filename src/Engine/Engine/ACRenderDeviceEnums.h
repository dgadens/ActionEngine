//************************************************
// Mantem os enums para os states do dispositivo
// creator: Daniel Gadens
// date: 01/08/2011
//************************************************

#ifndef __ACRENDERDEVICEENUMS_H
#define __ACRENDERDEVICEENUMS_H

enum ACBLENDSTATE
{
	ACBS_Default,
	ACBS_Additive,
	ACBS_NonPremultiplied,
	ACBS_AlphaBlend,
	ACBS_Opaque
};

enum ACDEPTHBUFFERSTATE
{
	ACDBS_WriteEnable,
	ACDBS_WriteDisable
};

enum ACRASTERIZESTATE
{
	ACRS_SolidCullCCW,
	ACRS_WireframeCullCCW,
	ACRS_SolidCullCW,
	ACRS_WireframeCullCW,
	ACRS_SolidCullNone,
	ACRS_WireframeCullNone
};

enum ACSAMPLERSTATE
{
	ACSS_Point_Wrap,
	ACSS_Bilinear_Wrap,
	ACSS_Trilinear_Wrap,
	ACSS_Anisotropic_Wrap,
	ACSS_Point_Clamp,
	ACSS_Bilinear_Clamp,
	ACSS_Trilinear_Clamp,
	ACSS_Anisotropic_Clamp,
	ACSS_Point_Border,
	ACSS_Bilinear_Border,
	ACSS_Trilinear_Border,
	ACSS_Anisotropic_Border,
};

enum ACTEXTURETYPE
{
	ACTT_Texture1D,
	ACTT_Texture2D,
	ACTT_Texture3D,
	ACTT_TextureCube
};

enum ACSHADEMODE
{
	ACSM_TriangleList,
	ACSM_TriangleStrip,
	ACSM_LineList,
	ACSM_LineStrip,
	ACSM_Point,
	ACSM_PointSprite,
};



#endif