using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ACFramework.FileStructs
{
    public struct MS3D_HEAD
    {
        public byte Version;                   
        public ulong NumVertices;
        public ulong NumIndices;
        public ulong NumFaces;
        public ulong NumMeshes;
        public uint  NumMaterials;
        public uint NumJoints;					
        public float AnimationFPS;             
        public float CurrentTime;              
        public uint NumFrames;                 
        public uint NumAnimations;             
    }

    public struct MS3D_VERTEX
    {
        public Vector3 fXYZ;                 // Coordinates
        public Vector2 fUV0;                 // UV 0 Texturescoor.
        public Vector2 fUV1;                 // UV 1 Texturescoor.
        public Vector3 fNormal;              // Normalvector
        public ushort usReferences;              // References
        public uint uiBoneID_A;                  // BoneID
        public float fWeight_A;                  // Weight
        public uint uiBoneID_B;                  // BoneID
        public float fWeight_B;                  // Weight
        public byte byFlags;                     // Flags

        public static bool operator ==(MS3D_VERTEX v1, MS3D_VERTEX v2)
        {
            if (v1.fXYZ != v2.fXYZ || v1.fUV0 != v2.fUV0 || v1.fUV1 != v2.fUV1 || v1.fNormal != v2.fNormal || v1.usReferences != v2.usReferences ||
                v1.uiBoneID_A != v2.uiBoneID_A || v1.fWeight_A != v2.fWeight_A || v1.uiBoneID_B != v2.uiBoneID_B || v1.fWeight_B != v2.fWeight_B)
                return false;

            return true; 
        }

        public static bool operator !=(MS3D_VERTEX v1, MS3D_VERTEX v2)
        {
            if (v1.fXYZ != v2.fXYZ || v1.fUV0 != v2.fUV0 || v1.fUV1 != v2.fUV1 || v1.fNormal != v2.fNormal || v1.usReferences != v2.usReferences ||
                v1.uiBoneID_A != v2.uiBoneID_A || v1.fWeight_A != v2.fWeight_A || v1.uiBoneID_B != v2.uiBoneID_B || v1.fWeight_B != v2.fWeight_B)
                return true;

            return false;
        }
    }

    public struct MS3D_FACE
    {
        public List<int> ulIndices;            // Indices
        public Vector3 fNormal;              // Normalvector
        public ulong ulMeshID;             // MeshID
        public uint uiMaterialID;         // MaterialID
        public ushort byFlags;              // Flags
    }

    public struct MS3D_MATERIAL
    {
        public string cName;                         // Name
        public Vector4 fAmbient;                       // Color Ambient
        public Vector4 fDiffuse;                       // Color Diffuse
        public Vector4 fSpecular;                      // Color Specular
        public Vector4 fEmissive;                      // Color Emissive
        public float fSpecularPower;                      // Specular Power
        public float fTransparency;                       // Transparency
        public string cTexture_1;                   // Texturename
        public string cTexture_2;                   // Texturename
        public byte byFlags;                             // Flags
    }

    public struct MS3D_MESH
    {
        public string cName;                         // Name
        public ushort wNumFaces;                         // Number of Faces
        public List<ushort> pIndices;                     // Faceindex
        public uint uiMaterialID;                        // MaterialID
        public byte byFlags;                             // Flags
    }

    public struct MS3D_KF_ROT
    {
        public float fTime;                               // Time
        public Vector4 vRotation;                           // Rotation
    } 

    public struct MS3D_KF_POS
    {
        public float fTime;                               // Time
        public Vector4 vPosition;                           // Position
    }

    public struct MS3D_JOINT
    {
        public string cName;                         // Descriptor
        public string cParentName;                   // Parentdescriptor
        public ushort wParentID;                           // ParentID
        public Vector4 vRotation;                           // Rotation
        public Vector4 vPosition;                           // Position
        public ushort wNumKF_Rotation;                     // Number of KF_Rots
        public ushort wNumKF_Position;                     // Number of KF_Pos
        public List<MS3D_KF_ROT> pKF_Rotation;                        // KF Rotations
        public List<MS3D_KF_POS> pKF_Position;                        // Position
        public bool bAnimated;                           // Animiert
        public byte byFlags;                             // Flags
        public Matrix sMatrix;                             // Matrix
        public Matrix sMatrix_absolute;                    // Matrix absolute
        public Matrix sMatrix_relative;                    // Matrix relativ
    }

    public struct MS3D_ANIMATION
    {
        public string cName;                         // Bezeichnung
        public float fStartFrame;                         // Startframe
        public float fEndFrame;                           // Endframe
        public bool bActive;                             // Aktiv
    }

    public struct MS3D_MODEL
    {
        public MS3D_HEAD sHead;                 // Header
        public List<MS3D_VERTEX> pVertices;             // Vertices
        public List<MS3D_FACE> pFaces;                // Faces
        public List<MS3D_MESH> pMeshes;                // Meshs
        public List<MS3D_MATERIAL> pMaterials;            // Materials
        public List<MS3D_JOINT> pJoints;               // Joints
        public List<MS3D_ANIMATION> pAnimation;            // Animations
    } 
}
