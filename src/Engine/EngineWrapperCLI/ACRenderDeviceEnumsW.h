#pragma once

using namespace System;

namespace EngineWrapper
{
	public enum class ACBLENDSTATEW
	{
		ACBS_Default,
		ACBS_Additive,
		ACBS_NonPremultiplied,
		ACBS_AlphaBlend,
		ACBS_Opaque
	};

	public enum class ACDEPTHBUFFERSTATEW
	{
		ACDBS_WriteEnable,
		ACDBS_WriteDisable
	};

	public enum class ACRASTERIZESTATEW
	{
		ACRS_SolidCullCCW,
		ACRS_WireframeCullCCW,
		ACRS_SolidCullCW,
		ACRS_WireframeCullCW,
		ACRS_SolidCullNone,
		ACRS_WireframeCullNone
	};

	public enum class ACSAMPLERSTATEW
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

	public enum class ACTEXTURETYPEW
	{
		ACTT_Texture1D,
		ACTT_Texture2D,
		ACTT_Texture3D,
		ACTT_TextureCube
	};

	public enum class ACSHADEMODEW
	{
		ACSM_TriangleList,
		ACSM_TriangleStrip,
		ACSM_LineList,
		ACSM_LineStrip,
		ACSM_Point,
		ACSM_PointSprite,
	};
}