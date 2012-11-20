using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using ACFramework.FileStruct;
using EngineMathCLI;

namespace ACFramework.FileStructs
{
    public class AMTExport
    {
        Import _import;

        public bool Export(AMT_MODEL model, string filePath, Import import)
        {
            _import = import;

            using (BinaryWriter bw = new BinaryWriter(File.Open(filePath, FileMode.Create)))
            {
                WriteHead(bw, model);
                WriteVertices(bw, model);
                WriteFaces(bw, model);
                WriteMeshes(bw, model);
                WriteMaterials(bw, model);
                WriteJoints(bw, model);
                WriteAnimations(bw, model);

                bw.Close();
            }

            return true;
        }

        private void WriteHead(BinaryWriter bw, AMT_MODEL model)
        {
            _import.ProgressText = "Exporting Head";

            AMT_HEAD head = model.Head;

            bw.Write(head.Version);
            bw.Write(head.NumVertices);
            bw.Write(head.NumIndices);
            bw.Write(head.NumFaces);
            bw.Write(head.NumMeshes);
            bw.Write(head.NumMaterials);
            bw.Write(head.NumJoints);
            bw.Write(head.AnimationFPS);
            bw.Write(head.CurrentTime);
            bw.Write(head.NumFrames);
            bw.Write(head.NumAnimations);
            bw.Write(head.HasSkeleton);
        }

        private void WriteVertices(BinaryWriter bw, AMT_MODEL model)
        {
            List<AMT_VERTEX> vertices = model.Vertices;

            for (int i = 0; i < vertices.Count; i++)
            {
                _import.ProgressText = "Exporting Vertices " + i.ToString() + "/" + vertices.Count.ToString();

                WriteVector(bw, vertices[i].Position);
                WriteVector(bw, vertices[i].TexCoord1);
                WriteVector(bw, vertices[i].TexCoord2);
                WriteVector(bw, vertices[i].Normal);

                for (int j = 0; j < 4; j++)
                {
                    bw.Write(vertices[i].BoneIndices[j]);
                    bw.Write(vertices[i].BoneWeights[j]);                   
                }

                bw.Write(vertices[i].Flag);
            }
        }

        private void WriteFaces(BinaryWriter bw, AMT_MODEL model)
        {
            List<AMT_FACE> faces = model.Faces;

            for (int i = 0; i < faces.Count; i++)
            {
                _import.ProgressText = "Exporting Faces " + i.ToString() + "/" + faces.Count.ToString();

                for (int j = 0; j < faces[i].Indices.Count; j++)
                    bw.Write(faces[i].Indices[j]);

                WriteVector(bw, faces[i].Normal);

                bw.Write(faces[i].MeshID);

                bw.Write(faces[i].Flag);   
            }
        }


        private void WriteMeshes(BinaryWriter bw, AMT_MODEL model)
        {
            List<AMT_MESH> meshes = model.Meshes;

            for (int i = 0; i < meshes.Count; i++)
            {
                char[] name = Tools.GetCharArray(meshes[i].Name, 64);
                bw.Write(name);

                _import.ProgressText = "Exporting Mesh " + meshes[i].Name;

                bw.Write(meshes[i].NumFaces);
                for (int j = 0; j < meshes[i].FaceIndices.Count; j++)
                    bw.Write(meshes[i].FaceIndices[j]);

                bw.Write(meshes[i].MaterialID);
                bw.Write(meshes[i].Flag);
            }
        }

        private void WriteMaterials(BinaryWriter bw, AMT_MODEL model)
        {
            List<AMT_MATERIAL> materials = model.Materials;

            for (int i = 0; i < materials.Count; i++)
            {
                char[] name = Tools.GetCharArray(materials[i].Name, 64);
                bw.Write(name);

                _import.ProgressText = "Exporting Materials " + materials[i].Name;

                WriteVector(bw, materials[i].Ambient);
                WriteVector(bw, materials[i].Diffuse);
                WriteVector(bw, materials[i].Specular);
                WriteVector(bw, materials[i].Emissive);
                bw.Write(materials[i].SpecularPower);
                bw.Write(materials[i].Transparency);

                char[] diffuseTexture = Tools.GetCharArray(materials[i].DiffuseTexture, 64);
                bw.Write(diffuseTexture);

                char[] specularTexture = Tools.GetCharArray(materials[i].SpecularTexture, 64);
                bw.Write(specularTexture);

                char[] normalTexture = Tools.GetCharArray(materials[i].NormalTexture, 64);
                bw.Write(normalTexture);

                char[] animatedTexture = Tools.GetCharArray(materials[i].AnimatedTexture, 64);
                bw.Write(animatedTexture);

                bw.Write(materials[i].Flag);
            }
        }

        private void WriteJoints(BinaryWriter bw, AMT_MODEL model)
        {
            List<AMT_JOINT> joints = model.Joints;

            for (int i = 0; i < joints.Count; i++)
            {
                char[] name = Tools.GetCharArray(joints[i].Name, 64);
                bw.Write(name);

                _import.ProgressText = "Exporting Bones: name: " + joints[i].Name;

                bw.Write(joints[i].ID);
                bw.Write(joints[i].ParentID);

                //grava os ids dos filhos
                bw.Write(joints[i].NumChildren);
                if (joints[i].NumChildren > 0)
                    foreach (var item in joints[i].JointChildren)
                        bw.Write(item);

                //grava todos os keyframes desse joint
                bw.Write(joints[i].NumKF);
                if (joints[i].NumKF > 0)
                    WriteKFData(bw, joints[i].KFData);

                bw.Write(joints[i].IsAnimated);

                bw.Write(joints[i].Flag);

                WriteMatrix(bw, joints[i].BindMatrix);
                WriteMatrix(bw, joints[i].MatrixAbsolute);
                WriteMatrix(bw, joints[i].InverseBindMatrix);
            }
        }

        private void WriteAnimations(BinaryWriter bw, AMT_MODEL model)
        {
            List<AMT_ANIMATION> animations = model.Animations;

            for (int i = 0; i < animations.Count; i++)
            {
                char[] name = Tools.GetCharArray(animations[i].Name, 64);
                bw.Write(name);

                _import.ProgressText = "Exporting Bones: animation: " + animations[i].Name;

                bw.Write(animations[i].StartFrame);
                bw.Write(animations[i].EndFrame);
                bw.Write(animations[i].Active);
            }
        }

        private void WriteKFData(BinaryWriter bw, List<AMT_KF> kfData)
        {
            foreach (var item in kfData)
            {
                bw.Write(item.Time);
                WriteMatrix(bw, item.BindMatrix);
            }
        }

        private void WriteVector(BinaryWriter bw, Vector2 v)
        {
            bw.Write(v.X);
            bw.Write(v.Y);
        }

        private void WriteVector(BinaryWriter bw, Vector3 v)
        {
            bw.Write(v.X);
            bw.Write(v.Y);
            bw.Write(v.Z);
        }

        private void WriteVector(BinaryWriter bw, Vector4 v)
        {
            bw.Write(v.X);
            bw.Write(v.Y);
            bw.Write(v.Z);
            bw.Write(v.W);
        }

        private void WriteMatrix(BinaryWriter bw, Matrix mat)
        {
            bw.Write(mat.M11);
            bw.Write(mat.M12);
            bw.Write(mat.M13);
            bw.Write(mat.M14);

            bw.Write(mat.M21);
            bw.Write(mat.M22);
            bw.Write(mat.M23);
            bw.Write(mat.M24);

            bw.Write(mat.M31);
            bw.Write(mat.M32);
            bw.Write(mat.M33);
            bw.Write(mat.M34);

            bw.Write(mat.M41);
            bw.Write(mat.M42);
            bw.Write(mat.M43);
            bw.Write(mat.M44);
        }
    }
}
