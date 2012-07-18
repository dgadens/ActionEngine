
#ifndef __ENUMS_H
#define __ENUMS_H

enum PlaneIntersectionType
{ 
	PIT_Front, 
	PIT_Back, 
	PIT_Intersecting
};

enum ContainmentType
{
	CT_Disjoint,
	CT_Contains,
	CT_Intersects
};

enum EffectType
{
	ET_Sprite,
	ET_NormalTextured,
	ET_Textured,
	ET_Colored
};

//operacao nos buffers
enum BufferOperation
{
	BO_Insert,  //insere normal, se ja existir ele nao insere so incrementa o instance
	BO_ForceInsert, //forca a insercao, se existe ele concatena o globalsid
	BO_UpdateData //atualiza os dados no caso usa do VB dinamico
};

enum DDSType 
{ 
	DDS_DXT1,
	DDS_DXT3,
	DDS_DXT5
};

enum EndianType 
{ 
	ENDIAN_Unknown = 0, 
	ENDIAN_Little, 
	ENDIAN_Big 
};

enum InputDevType 
{
   IDV_Keyboard,
   IDV_Joystick,
   IDV_Mouse
};

enum NetModeType 
{
   NMT_Server=0,
   NMT_Client=1,
};

enum VertexFormat
{
	VF_VertexPosition,
	VF_VertexPositionColored,
	VF_VertexPositionTextured,
	VF_VertexPositionNormalTextured,
	VF_VertexPositionTexturedExtraInfo,
	VF_VertexSkinnedMesh,
	VF_VertexSprite
};

enum FlipType
{
	FT_None = 0,
	FT_HorizontalFlip = 1,
	FT_VerticalFlip = 2
};

#endif