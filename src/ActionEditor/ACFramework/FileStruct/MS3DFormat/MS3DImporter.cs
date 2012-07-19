using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using ACFramework.FileStruct;

namespace ACFramework.FileStructs
{
    public class MS3DImporter
    {
        Import _import;

        public MS3D_MODEL? Import(string path, Import import)
        {
            _import = import;

            MS3D_MODEL model = new MS3D_MODEL();
            using (BinaryReader rdr = new BinaryReader(File.OpenRead(path)))
            {
                while (rdr.BaseStream.Position < rdr.BaseStream.Length)
                {
                    if (!ReadType(rdr))
                    {
                        rdr.Close();
                        throw new Exception("Wrong type of file.");
                    }

                    if (!ReadVersion(rdr))
                    {
                        rdr.Close();
                        throw new Exception("Wrong Version of file.");
                    }

                    if (!ReadVertices(rdr, ref model))
                    {
                        rdr.Close();
                        throw new Exception("Error on vertices import.");
                    }

                    if (!ReadFaces(rdr, ref model))
                    {
                        rdr.Close();
                        throw new Exception("Error on faces import.");
                    }

                    if (!ReadGroups(rdr, ref model))
                    {
                        rdr.Close();
                        throw new Exception("Error on group import.");
                    }

                    if (!ReadMaterials(rdr, ref model))
                    {
                        rdr.Close();
                        throw new Exception("Error on material import.");
                    }

                    //esse break é pq estou carregando parte do arquivo so
                    break;
                };

                rdr.Close();
            };

            return model;
        }

        private bool ReadType(BinaryReader br)
        {
            char[] value = br.ReadChars(10);
            string str = new string(value);
            return str == "MS3D000000";
        }

        private bool ReadVersion(BinaryReader br)
        {
            int value = br.ReadInt32();

            if ((value < 3) || (value > 4))
                return false;

            return true;
        }

        private bool ReadVertices(BinaryReader br, ref MS3D_MODEL model)
        {
            model.sHead.NumVertices = (ulong)br.ReadInt16();

            if (model.sHead.NumVertices <= 0)
            {
                throw new Exception("No vertices found");
            }

            model.pVertices = new List<MS3D_VERTEX>();

            for (ulong i = 0; i < model.sHead.NumVertices; i++)
            {
                _import.ProgressText = "Importing Vertices " + i.ToString() + "/" + model.sHead.NumVertices.ToString();

                MS3D_VERTEX v = new MS3D_VERTEX();

                //flag
                byte flag = br.ReadByte();
                v.byFlags = flag;

                //posicao
                v.fXYZ = new Vector3();
                v.fXYZ.X = br.ReadSingle();
                v.fXYZ.Y = br.ReadSingle();
                v.fXYZ.Z = br.ReadSingle();

                //boneid
                char boneidA = br.ReadChar();
                v.uiBoneID_A = boneidA;
                v.fWeight_A = 1.0f;

                //reference
                char references = br.ReadChar();
                v.usReferences = references;

                model.pVertices.Add(v);
            }

            return true;
        }

        private bool ReadFaces(BinaryReader br, ref MS3D_MODEL model)
        {
            int ulIndex = 0;

            model.sHead.NumFaces = (ulong)br.ReadInt16();
            model.pFaces = new List<MS3D_FACE>();

            if (model.sHead.NumFaces <= 0)
            {
                throw new Exception("No faces found");
            }

            MS3D_VERTEX[] indexVertices = new MS3D_VERTEX[model.sHead.NumFaces * 3];
            MS3D_FACE[] faces = new MS3D_FACE[model.sHead.NumFaces];

            for (ulong i = 0; i < model.sHead.NumFaces; i++)
            {
                _import.ProgressText = "Importing Faces " + i.ToString() + "/" + model.sHead.NumFaces.ToString();

                MS3D_FACE f = new MS3D_FACE();

                //flag
                ushort flag = br.ReadUInt16();
                f.byFlags = flag;

                //indices
                f.ulIndices = new List<int>();
                ushort index1 = br.ReadUInt16();
                ushort index2 = br.ReadUInt16();
                ushort index3 = br.ReadUInt16();
                f.ulIndices.Add(index1);
                f.ulIndices.Add(index2);
                f.ulIndices.Add(index3);

                //copia os vertices na ordem dos indices
                for (int n = 0; n < 3; n++)
                {
                    MS3D_VERTEX pVertex = model.pVertices[(int)f.ulIndices[n]];
                    indexVertices[ulIndex++] = pVertex;
                }

                //carrega as normais
                for (int n = 0; n < 3; n++)
                {
                    MS3D_VERTEX pVertex = indexVertices[ulIndex - 3 + n];

                    pVertex.fNormal = new Vector3();
                    pVertex.fNormal.X = br.ReadSingle();
                    pVertex.fNormal.Y = br.ReadSingle();
                    pVertex.fNormal.Z = br.ReadSingle();

                    indexVertices[ulIndex - 3 + n] = pVertex;
                }

                //float   s[3];
                //float   t[3];

                //carrega as coordenadas de textura
                for (int n = 0; n < 3; n++)
                {
                    MS3D_VERTEX pVertex = indexVertices[ulIndex - 3 + n];
                    pVertex.fUV0.X = br.ReadSingle();
                    indexVertices[ulIndex - 3 + n] = pVertex;
                }

                for (int n = 0; n < 3; n++)
                {
                    MS3D_VERTEX pVertex = indexVertices[ulIndex - 3 + n];
                    pVertex.fUV0.Y = br.ReadSingle() - 1.0f;
                    indexVertices[ulIndex - 3 + n] = pVertex;
                }

                //pula um char
                br.ReadChar();

                //pega o id do mesh
                f.ulMeshID = br.ReadChar();

                f.ulIndices[0] = ulIndex - 3;
                f.ulIndices[1] = ulIndex - 2;
                f.ulIndices[2] = ulIndex - 1;

                model.pFaces.Add(f);
            }

            //grava os vertices agora na ordem certa triangulados
            model.pVertices.Clear();
            model.sHead.NumVertices = model.sHead.NumFaces * 3;
            ulong currentVertices = model.sHead.NumVertices;

            // optimazacao dor vertices
            for (ulong i = 0; i < model.sHead.NumVertices; i++)
            {
                _import.ProgressText = "Importing Faces, recompute vertices " + i.ToString() + "/" + model.sHead.NumVertices.ToString();

                // se o flag for 42 ele vai para o proximo
                if (indexVertices[i].byFlags == 42) continue;

                for (ulong j = 0; j < model.sHead.NumVertices; j++)
                    if ((i != j) && indexVertices[i] == indexVertices[j])
                    {
                        //desliga o vertice pq esta duplicado
                        indexVertices[j].byFlags = 42;

                        currentVertices--;
                    }
            }

            // Copia os vertices para o array da estrutura
            for (ulong i = 0; i < model.sHead.NumVertices; i++)
            {
                // todos q nao sao 42
                if (indexVertices[i].byFlags != 42)
                    model.pVertices.Add(indexVertices[i]);
            }

            //percorre o array de indices para procurar os vertices q estao na mesma posicao, se estiverem entao ele deixa os indices iguais
            for (ulong i = 0; i < model.sHead.NumFaces; i++)
            {
                _import.ProgressText = "Importing Faces, recompute indices " + i.ToString() + "/" + model.sHead.NumFaces.ToString();

                MS3D_FACE f = model.pFaces[(int)i];

                for (int n = 0; n < 3; n++)
                {
                    for (int j = 0; j < (int)currentVertices; j++)
                    {
                        if (indexVertices[f.ulIndices[n]] == model.pVertices[j])
                        {
                            f.ulIndices[n] = j;
                        }
                    }
                }

                model.pFaces[(int)i] = f;
            }

            //calcula as normais das faces
            for (ulong i = 0; i < model.sHead.NumFaces; i++)
            {
                _import.ProgressText = "Importing Faces, recompute normals " + i.ToString() + "/" + model.sHead.NumFaces.ToString();

                Vector3 v1 = model.pVertices[model.pFaces[(int)i].ulIndices[0]].fXYZ;
                Vector3 v2 = model.pVertices[model.pFaces[(int)i].ulIndices[1]].fXYZ;
                Vector3 v3 = model.pVertices[model.pFaces[(int)i].ulIndices[2]].fXYZ;

                MS3D_FACE face = model.pFaces[(int)i];
                face.fNormal = Tools.GetNormal(v1, v2, v3);
                model.pFaces[(int)i] = face;
            }

            model.sHead.NumVertices = currentVertices;

            return true;
        }

        private bool ReadGroups(BinaryReader br, ref MS3D_MODEL model)
        {
            model.sHead.NumMeshes = (ulong)br.ReadInt16();

            if (model.sHead.NumMeshes <= 0)
            {
                throw new Exception("No meshes found");
            }

            model.pMeshes = new List<MS3D_MESH>();

            for (ulong i = 0; i < model.sHead.NumMeshes; i++)
            {
                _import.ProgressText = "Importing Mesh " + i.ToString() + "/" + model.sHead.NumMeshes.ToString();

                MS3D_MESH m = new MS3D_MESH();

                //flag
                byte flag = br.ReadByte();
                m.byFlags = flag;

                //carrega o nome
                m.cName = new string(br.ReadChars(32)); //32 o tamanho da descricao do arquivo

                //numero de faces no grupo
                m.wNumFaces = br.ReadUInt16();

                m.pIndices = new List<ushort>(m.wNumFaces);
                for (int j = 0; j < m.wNumFaces; j++)
                    m.pIndices.Add(br.ReadUInt16());

                //id do material
                m.uiMaterialID = br.ReadChar();

                //passa para as faces do grupo o id do material
                for (int j = 0; j < m.wNumFaces; j++)
                {
                    MS3D_FACE f = model.pFaces[m.pIndices[j]];
                    f.uiMaterialID = m.uiMaterialID;
                }

                model.pMeshes.Add(m);
            }

            return true;
        }

        private bool ReadMaterials(BinaryReader br, ref MS3D_MODEL model)
        {
            model.sHead.NumMaterials = (uint)br.ReadInt16();

            if (model.sHead.NumMaterials <= 0)
            {
                throw new Exception("No materials found");
            }

            model.pMaterials = new List<MS3D_MATERIAL>();

            for (int i = 0; i < model.sHead.NumMaterials; i++)
            {
                _import.ProgressText = "Importing Materials " + i.ToString() + "/" + model.sHead.NumMaterials.ToString();

                MS3D_MATERIAL m = new MS3D_MATERIAL();

                m.cName = new string(br.ReadChars(32));

                //ambient
                m.fAmbient = new Vector4();
                m.fAmbient.X = br.ReadSingle();
                m.fAmbient.Y = br.ReadSingle();
                m.fAmbient.Z = br.ReadSingle();
                m.fAmbient.W = br.ReadSingle();

                //diffuse
                m.fDiffuse = new Vector4();
                m.fDiffuse.X = br.ReadSingle();
                m.fDiffuse.Y = br.ReadSingle();
                m.fDiffuse.Z = br.ReadSingle();
                m.fDiffuse.W = br.ReadSingle();

                //specular
                m.fSpecular = new Vector4();
                m.fSpecular.X = br.ReadSingle();
                m.fSpecular.Y = br.ReadSingle();
                m.fSpecular.Z = br.ReadSingle();
                m.fSpecular.W = br.ReadSingle();

                //emissive
                m.fEmissive = new Vector4();
                m.fEmissive.X = br.ReadSingle();
                m.fEmissive.Y = br.ReadSingle();
                m.fEmissive.Z = br.ReadSingle();
                m.fEmissive.W = br.ReadSingle();

                //specular power
                m.fSpecularPower = br.ReadSingle();

                //transparencia
                m.fTransparency = br.ReadSingle();

                //pula um byte
                br.ReadByte();

                m.cTexture_1 = new string(br.ReadChars(128));
                int pos = m.cTexture_1.LastIndexOf('\\');
                if (pos > 0)
                {
                    string t = m.cTexture_1;
                    m.cTexture_1 = t.Substring(pos + 1, t.Length - pos - 1);
                }

                m.cTexture_2 = new string(br.ReadChars(128));
                pos = m.cTexture_2.LastIndexOf('\\');
                if (pos > 0)
                {
                    string t = m.cTexture_2;
                    m.cTexture_2 = m.cTexture_2.Substring(pos + 1, t.Length - pos - 1);
                }

                model.pMaterials.Add(m);
            }

            return true;
        }
    }
}
