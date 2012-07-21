﻿//****************************
// Tipo da action para armazenar os modelos 3d
// creator: Daniel Gadens
// date: 10/06/2011
//*****************************
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ACFramework.FileStructs
{
    public struct AMT_HEAD
    {
        public uint Version;
        public uint NumVertices;
        public uint NumIndices;
        public uint NumFaces;
        public uint NumMeshes;
        public uint NumMaterials;
        public uint NumJoints;
        public float AnimationFPS;
        public float CurrentTime;
        public uint NumFrames;
        public uint NumAnimations;
        public uint HasSkeleton;
    }

    public struct AMT_VERTEX
    {
        public Vector3 Position;
        public Vector2 TexCoord1;                 // UV 0 Texturescoor.
        public Vector2 TexCoord2;                 // UV 1 Texturescoor.
        public Vector3 Normal;                    // Normalvector
        public uint BoneID_A;                  // Id dos bones q influenciam o vertice
        public float BoneWeight_A;              // peso do bone sobre o vertices
        public uint BoneID_B;                  // Id dos bones q influenciam o vertice
        public float BoneWeight_B;              // peso do bone sobre o vertices
        public uint BoneID_C;                  // Id dos bones q influenciam o vertice
        public float BoneWeight_C;              // peso do bone sobre o vertices
        public uint BoneID_D;                  // Id dos bones q influenciam o vertice
        public float BoneWeight_D;              // peso do bone sobre o vertices
        public uint Flag;                          // flag para objetivos diversos

        public static bool operator ==(AMT_VERTEX v1, AMT_VERTEX v2)
        {
            if (v1.Position != v2.Position || v1.TexCoord1 != v2.TexCoord1 || v1.TexCoord2 != v2.TexCoord2 ||
                v1.Normal != v2.Normal || 
                v1.BoneID_A != v2.BoneID_A ||
                v1.BoneID_B != v2.BoneID_B ||
                v1.BoneID_C != v2.BoneID_C ||
                v1.BoneID_D != v2.BoneID_D || 
                v1.BoneWeight_A != v2.BoneWeight_A ||
                v1.BoneWeight_B != v2.BoneWeight_B ||
                v1.BoneWeight_C != v2.BoneWeight_C ||
                v1.BoneWeight_D != v2.BoneWeight_D)
                return false;

            return (v1.Flag == v2.Flag);
        }

        public static bool operator !=(AMT_VERTEX v1, AMT_VERTEX v2)
        {
            if (v1.Position != v2.Position || v1.TexCoord1 != v2.TexCoord1 || v1.TexCoord2 != v2.TexCoord2 ||
                v1.Normal != v2.Normal ||          
                v1.BoneID_A != v2.BoneID_A ||
                v1.BoneID_B != v2.BoneID_B ||
                v1.BoneID_C != v2.BoneID_C ||
                v1.BoneID_D != v2.BoneID_D || 
                v1.BoneWeight_A != v2.BoneWeight_A ||
                v1.BoneWeight_B != v2.BoneWeight_B ||
                v1.BoneWeight_C != v2.BoneWeight_C ||
                v1.BoneWeight_D != v2.BoneWeight_D)
                return true;

            return (v1.Flag != v2.Flag);
        }

        public static bool EqualsWithoutFlagTest(AMT_VERTEX v1, AMT_VERTEX v2)
        {
            if (v1.Position != v2.Position || v1.TexCoord1 != v2.TexCoord1 || v1.TexCoord2 != v2.TexCoord2 ||
                v1.Normal != v2.Normal ||
                v1.BoneID_A != v2.BoneID_A ||
                v1.BoneID_B != v2.BoneID_B ||
                v1.BoneID_C != v2.BoneID_C ||
                v1.BoneID_D != v2.BoneID_D ||
                v1.BoneWeight_A != v2.BoneWeight_A ||
                v1.BoneWeight_B != v2.BoneWeight_B ||
                v1.BoneWeight_C != v2.BoneWeight_C ||
                v1.BoneWeight_D != v2.BoneWeight_D)
                return false;

            return true;
        }
    }


    public struct AMT_FACE
    {
        public List<int> Indices;            // Indices
        public Vector3 Normal;              // Normalvector
        public uint MeshID;             // MeshID
        public uint Flag;              // Flags
    }

    public struct AMT_MATERIAL
    {
        public string Name;                           // Name
        public Vector3 Ambient;                       // Color Ambient
        public Vector3 Diffuse;                       // Color Diffuse
        public Vector3 Specular;                      // Color Specular
        public Vector3 Emissive;                      // Color Emissive
        public float SpecularPower;                   // Specular Power
        public float Transparency;                    // Transparency
        public string DiffuseTexture;
        public string SpecularTexture;
        public string NormalTexture;
        public string AnimatedTexture;
        public uint Flag;                             // Flags
    }

    public struct AMT_MESH
    {
        public string Name;                         // Name
        public int NumFaces;                         // Number of Faces
        public List<int> FaceIndices;                     // Indices das faces
        public uint MaterialID;                        // MaterialID
        public uint Flag;                             // Flags
    }

    public struct AMT_KF_ROT
    {
        public float Time;                               // Time
        public Vector3 Rotation;                           // Rotation
    }

    public struct AMT_KF_POS
    {
        public float Time;                               // Time
        public Vector3 Position;                           // Position
    }

    public struct AMT_JOINT
    {
        public string Name;                         // Descriptor
        public string ParentName;                   // Parentdescriptor
        public uint ParentID;                           // ParentID
        public Vector4 Rotation;                           // Rotation
        public Vector4 Position;                           // Position
        public uint NumKFRotation;                     // Number of KF_Rots
        public uint NumKFPosition;                     // Number of KF_Pos
        public List<AMT_KF_ROT> KFRotation;                        // KF Rotations
        public List<AMT_KF_POS> KFPosition;                        // Position
        public uint IsAnimated;                           // Animiert
        public uint Flag;                             // Flags
        public Matrix Matrix;                             // Matrix
        public Matrix MatrixAbsolute;                    // Matrix absolute
        public Matrix MatrixRelative;                    // Matrix relativ
    }

    public struct AMT_ANIMATION
    {
        public string Name;                         
        public float StartFrame;                    
        public float EndFrame;
        public uint Active;                         
    }

    public struct AMT_MODEL
    {
        public AMT_HEAD Head;                 // Header
        public List<AMT_VERTEX> Vertices;             // Vertices
        public List<AMT_FACE> Faces;                // Faces
        public List<AMT_MESH> Meshes;                // Meshs
        public List<AMT_MATERIAL> Materials;            // Materials
        public List<AMT_JOINT> Joints;               // Joints
        public List<AMT_ANIMATION> Animation;            // Animations
    } 
}