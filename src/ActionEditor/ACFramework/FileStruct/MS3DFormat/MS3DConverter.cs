using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using ACFramework.FileStruct;

namespace ACFramework.FileStructs
{
    public class MS3DConverter : IConverter
    {
        Import _import;

        public Nullable<AMT_MODEL> Convert(string path, Import import)
        {
            _import = import;

            MS3DImporter importer = new MS3DImporter();
            MS3D_MODEL? model = importer.Import(path, _import);

            if (model != null)
            {
                AMT_MODEL amtModel = ConvertASM3DtoAMT(model.Value);
                Tools.UniformScale(ref amtModel);

                return amtModel;
            }

            return null;
        }

        private AMT_MODEL ConvertASM3DtoAMT(MS3D_MODEL ms3dModel)
        {
            AMT_MODEL amtModel = new AMT_MODEL();

            amtModel.Head = ConvertHead(ms3dModel);
            amtModel.Vertices = ConvertVertices(ms3dModel);
            amtModel.Faces = ConvertFaces(ms3dModel);
            amtModel.Meshes = ConvertMeshes(ms3dModel);
            amtModel.Materials = ConvertMaterials(ms3dModel);

            return amtModel;
        }

        private AMT_HEAD ConvertHead(MS3D_MODEL ms3dModel)
        {
            _import.ProgressText = "Converting Head";

            AMT_HEAD head = new AMT_HEAD();
            head.Version = 1;
            head.NumVertices     = (uint)ms3dModel.sHead.NumVertices;
            head.NumIndices      = (uint)ms3dModel.sHead.NumFaces * 3;
            head.NumFaces        = (uint)ms3dModel.sHead.NumFaces;
            head.NumMeshes       = (uint)ms3dModel.sHead.NumMeshes;
            head.NumMaterials    = ms3dModel.sHead.NumMaterials;
            head.NumJoints       = ms3dModel.sHead.NumJoints;
            head.AnimationFPS    = ms3dModel.sHead.AnimationFPS;
            head.CurrentTime     = ms3dModel.sHead.CurrentTime;
            head.NumFrames       = ms3dModel.sHead.NumFrames;
            head.NumAnimations   = ms3dModel.sHead.NumAnimations;

            return head;
        }

        private List<AMT_VERTEX> ConvertVertices(MS3D_MODEL ms3dModel)
        {
            List<AMT_VERTEX> amtVertexList = new List<AMT_VERTEX>();
            int index = 0;
            foreach (var item in ms3dModel.pVertices)
            {
                _import.ProgressText = "Converting Vertices " + index++.ToString();

                AMT_VERTEX vertice = new AMT_VERTEX();

                vertice.Position = item.fXYZ;
                vertice.TexCoord1 = item.fUV0;
                vertice.TexCoord2 = item.fUV1;
                vertice.Normal = Vector3.Normalize(item.fNormal);
                vertice.BoneID_A = item.uiBoneID_A;
                vertice.BoneWeight_A = item.fWeight_A;
                vertice.BoneID_B = item.uiBoneID_B;
                vertice.BoneWeight_B = item.fWeight_B;
                vertice.BoneID_C = 0;
                vertice.BoneWeight_C = 0;
                vertice.BoneID_D = 0;
                vertice.BoneWeight_D = 0;
                vertice.Flag = 0;

                amtVertexList.Add(vertice);
            }

            return amtVertexList;
        }

        private List<AMT_FACE> ConvertFaces(MS3D_MODEL ms3dModel)
        {
            List<AMT_FACE> amtFaceList = new List<AMT_FACE>();
            int index = 0;
            foreach (var item in ms3dModel.pFaces)
            {
                _import.ProgressText = "Converting Faces " + index++.ToString();

                AMT_FACE face = new AMT_FACE();

                face.Indices = item.ulIndices;           
                face.Normal = Vector3.Normalize(item.fNormal);             
                face.MeshID = (uint)item.ulMeshID;
                face.Flag = 0;

                amtFaceList.Add(face);
            }

            return amtFaceList;
        }

        private List<AMT_MESH> ConvertMeshes(MS3D_MODEL ms3dModel)
        {
            List<AMT_MESH> amtMeshList = new List<AMT_MESH>();
            foreach (var item in ms3dModel.pMeshes)
            {
                AMT_MESH mesh = new AMT_MESH();

                mesh.Name = item.cName;

                _import.ProgressText = "Converting Mesh " + mesh.Name;

                mesh.NumFaces = item.wNumFaces;
                mesh.FaceIndices = new List<int>();
                for (int i = 0; i < item.pIndices.Count; i++)
                    mesh.FaceIndices.Add((int)item.pIndices[i]);
                mesh.MaterialID = item.uiMaterialID;                        
                mesh.Flag = 0;

                amtMeshList.Add(mesh);
            }

            return amtMeshList;
        }

        private List<AMT_MATERIAL> ConvertMaterials(MS3D_MODEL ms3dModel)
        {
            List<AMT_MATERIAL> amtMaterialList = new List<AMT_MATERIAL>();
            foreach (var item in ms3dModel.pMaterials)
            {
                AMT_MATERIAL material = new AMT_MATERIAL();

                _import.ProgressText = "Converting Mesh " + material.Name;

                material.Name = item.cName;                        
                material.Ambient = new Vector3(item.fAmbient.X, item.fAmbient.Y, item.fAmbient.Z);                     
                material.Diffuse =  new Vector3(item.fDiffuse.X, item.fDiffuse.Y, item.fDiffuse.Z);                    
                material.Specular = new Vector3(item.fSpecular.X, item.fSpecular.Y, item.fSpecular.Z);
                material.Emissive = new Vector3(item.fEmissive.X, item.fEmissive.Y, item.fEmissive.Z);                  
                material.SpecularPower = item.fSpecularPower;               
                material.Transparency = item.fTransparency;                
                material.DiffuseTexture = item.cTexture_1;
                material.SpecularTexture = item.cTexture_2;
                material.Flag = 0; ;

                amtMaterialList.Add(material);
            }

            return amtMaterialList;
        }
    }
}
