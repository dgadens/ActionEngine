using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using ACFramework.FileStruct;

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

                bw.Write(vertices[i].BoneID_A);
                bw.Write(vertices[i].BoneWeight_A);

                bw.Write(vertices[i].BoneID_B);
                bw.Write(vertices[i].BoneWeight_B);

                bw.Write(vertices[i].BoneID_C);
                bw.Write(vertices[i].BoneWeight_C);

                bw.Write(vertices[i].BoneID_D);
                bw.Write(vertices[i].BoneWeight_D);

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

                char[] parentName = Tools.GetCharArray(joints[i].ParentName, 64);
                bw.Write(parentName);

                _import.ProgressText = "Exporting Bones: Parent" + parentName + " name: " + name;

                bw.Write(joints[i].ID);
                bw.Write(joints[i].ParentID);

                WriteVector(bw, joints[i].Rotation);
                WriteVector(bw, joints[i].Position);

                bw.Write(joints[i].NumKFRotation);
                bw.Write(joints[i].NumKFPosition);

                WriteKFRotations(bw, joints[i].KFRotation);
                WriteKFRotations(bw, joints[i].KFPosition);               

                bw.Write(joints[i].IsAnimated);

                bw.Write(joints[i].Flag);

                WriteMatrix(bw, joints[i].Matrix);
                WriteMatrix(bw, joints[i].MatrixAbsolute);
                WriteMatrix(bw, joints[i].MatrixRelative);
            }
        }

        private void WriteKFRotations(BinaryWriter bw, List<AMT_KF_ROT> kfRots)
        {
            foreach (var item in kfRots)
            {
                bw.Write(item.Time);
                bw.Write(item.Rotation.X);
                bw.Write(item.Rotation.Y);
                bw.Write(item.Rotation.Z);
            }
        }

        private void WriteKFRotations(BinaryWriter bw, List<AMT_KF_POS> kfPos)
        {
            foreach (var item in kfPos)
            {
                bw.Write(item.Time);
                bw.Write(item.Position.X);
                bw.Write(item.Position.Y);
                bw.Write(item.Position.Z);
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
