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

                bw.Write(vertices[i].Position.X);
                bw.Write(vertices[i].Position.Y);
                bw.Write(vertices[i].Position.Z);

                bw.Write(vertices[i].TexCoord1.X);
                bw.Write(vertices[i].TexCoord1.Y);

                bw.Write(vertices[i].TexCoord2.X);
                bw.Write(vertices[i].TexCoord2.Y);

                bw.Write(vertices[i].Normal.X);
                bw.Write(vertices[i].Normal.Y);
                bw.Write(vertices[i].Normal.Z);

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

                bw.Write(faces[i].Normal.X);
                bw.Write(faces[i].Normal.Y);
                bw.Write(faces[i].Normal.Z);

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

                bw.Write(materials[i].Ambient.X);        
                bw.Write(materials[i].Ambient.Y);
                bw.Write(materials[i].Ambient.Z);

                bw.Write(materials[i].Diffuse.X);        
                bw.Write(materials[i].Diffuse.Y);
                bw.Write(materials[i].Diffuse.Z);

                bw.Write(materials[i].Specular.X);        
                bw.Write(materials[i].Specular.Y);
                bw.Write(materials[i].Specular.Z);

                bw.Write(materials[i].Emissive.X);        
                bw.Write(materials[i].Emissive.Y);
                bw.Write(materials[i].Emissive.Z);

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
    }
}
