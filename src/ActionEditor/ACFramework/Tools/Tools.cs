using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ACFramework.FileStructs;
using System.Globalization;

namespace ACFramework
{
    public static class Tools
    {
        public static Vector3 GetNormal(Vector3 p1, Vector3 p2, Vector3 p3)
        {
            Vector3 v1 = p2 - p1;
            Vector3 v2 = p1 - p3;

            Vector3 norm = Vector3.Cross(v2, v1);
            norm = Vector3.Normalize(norm);

            return norm;
        }

        public static char[] GetCharArray(string str, int size)
        {
            string abc;
            if (str == null) str = "";
            str = Tools.CleanString(str);
            if (str.Length > size)
                abc = str.Substring(0, size);
            else
                abc = str.PadRight(size,'\0');

            return abc.ToCharArray();
        }

        public static Vector2 ConvertStringToVector2(string value)
        {
            Vector2 ret = new Vector2();
            string[] splitValue = value.Split(' ');

            if (splitValue.Length >= 2)
            {
                ret.X = Convert.ToSingle(splitValue[0], CultureInfo.InvariantCulture);
                ret.Y = Convert.ToSingle(splitValue[1], CultureInfo.InvariantCulture);
            }

            return ret;
        }

        public static Vector3 ConvertStringToVector3(string value)
        {
            Vector3 ret = new Vector3();
            string[] splitValue = value.Split(' ');

            if (splitValue.Length >= 3)
            {
                ret.X = Convert.ToSingle(splitValue[0], CultureInfo.InvariantCulture);
                ret.Y = Convert.ToSingle(splitValue[1], CultureInfo.InvariantCulture);
                ret.Z = Convert.ToSingle(splitValue[2], CultureInfo.InvariantCulture);                       
            }

            return ret;
        }

        public static Vector4 ConvertStringToVector4(string value)
        {
            Vector4 ret = new Vector4();
            string[] splitValue = value.Split(' ');

            if (splitValue.Length == 4)
            {
                ret.X = Convert.ToSingle(splitValue[0], CultureInfo.InvariantCulture);
                ret.Y = Convert.ToSingle(splitValue[1], CultureInfo.InvariantCulture);
                ret.Z = Convert.ToSingle(splitValue[2], CultureInfo.InvariantCulture);
                ret.W = Convert.ToSingle(splitValue[3], CultureInfo.InvariantCulture);
            }

            return ret;
        }

        public static Matrix ConvertStringToMatrix(string value, int offset)
        {
            Matrix ret = new Matrix();
            string[] splitValue = value.Split(' ');

            //crio um array de floats
            float[] mat = new float[splitValue.Length];
            for (int i = 0; i < mat.Length; i++)
                mat[i] = Convert.ToSingle(splitValue[i], CultureInfo.InvariantCulture);

            //armazendo como column major o collada armazena como rowmajor
            ret = new Matrix(mat[offset + 0], mat[offset + 4], mat[offset + 8], mat[offset + 12],
                                mat[offset + 1], mat[offset + 5], mat[offset + 9], mat[offset + 13],
                                mat[offset + 2], mat[offset + 6], mat[offset + 10], mat[offset + 14],
                                mat[offset + 3], mat[offset + 7], mat[offset + 11], mat[offset + 15]);

            return ret;
        }

        public static string CleanString(string s)
        {
            int index = 0;
            if (s != null && s.Length > 0)
            {
                foreach (char c in s)
                {
                    if(Char.IsSymbol(c) || char.IsControl(c))
                        break;
                    index++;
                }
            }

            char[] ca = s.ToArray();
            for (int i = index+1; i < ca.Length; i++)
            {
                ca[i] = '\0';
            }

            string ret = new string(ca);
            return ret;
        }

        //deixa todos os modelos com tamanho 1 para serem redimensionados no editor
        public static void UniformScale(ref AMT_MODEL model)
        {
            List<Vector3> vertices = new List<Vector3>();
            for (int i = 0; i < model.Vertices.Count; i++)
                vertices.Add(model.Vertices[i].Position);

            BoundingSphere sphere = BoundingSphere.CreateFromPoints(vertices);

            //redimensiona os vertices
            float scalefactor = 1.0f / sphere.Radius;

            //redimensiona tb todos os bones e os keyframes
            Matrix scale = Matrix.CreateScale(scalefactor);

            for (int i = 0; i < model.Vertices.Count; i++)
            {
                model.Vertices[i].Position = Vector3.Transform(model.Vertices[i].Position, scale);
                model.Vertices[i].Normal = Vector3.TransformNormal(model.Vertices[i].Normal, scale);
            }

            if (model.Joints != null)
            {
                for (int i = 0; i < model.Joints.Count; i++)
                {
                    AMT_JOINT v = model.Joints[i];

                    //como ele mexeu na posicao entao mexe so no raiz
                    if (v.Name == "Root")
                        v.BindMatrix *= scale;

                    if (model.Joints[i].ParentID != -1)
                        v.MatrixAbsolute = model.Joints[i].BindMatrix *
                                           model.Joints[model.Joints[i].ParentID].MatrixAbsolute;
                    else
                        v.MatrixAbsolute = model.Joints[i].BindMatrix;

                    v.InverseBindMatrix = Matrix.Invert(scale) * v.InverseBindMatrix;

                    for (int j = 0; j < v.KFData.Count; j++)
                    {
                        AMT_KF p = v.KFData[j];
                        p.bindMatrix *= scale;
                        v.KFData[j] = p;
                    }

                    model.Joints[i] = v;
                }
            }
        }

        public static void CenterPivot(ref AMT_MODEL model)
        {
            List<Vector3> vertices = new List<Vector3>();
            for (int i = 0; i < model.Vertices.Count; i++)
                vertices.Add(model.Vertices[i].Position);

            BoundingBox bb = BoundingBox.CreateFromPoints(vertices);
            Vector3 positionAjust = -(bb.Max + bb.Min) / 2;

            Matrix translation = Matrix.CreateTranslation(positionAjust);

            //redimensiona os vertices
            for (int i = 0; i < model.Vertices.Count; i++)
                model.Vertices[i].Position = Vector3.Transform(model.Vertices[i].Position, translation);

            //reposiciona tb todos os bones e os keyframes
            if (model.Joints != null)
            {
                for (int i = 0; i < model.Joints.Count; i++)
                {
                    AMT_JOINT v = model.Joints[i];

                    //como ele mexeu na posicao entao mexe so no raiz
                    if (v.Name == "Root")
                        v.BindMatrix *= translation;

                    if (model.Joints[i].ParentID != -1)
                        v.MatrixAbsolute = model.Joints[i].BindMatrix *
                                           model.Joints[model.Joints[i].ParentID].MatrixAbsolute;
                    else
                        v.MatrixAbsolute = model.Joints[i].BindMatrix;

                    v.InverseBindMatrix = Matrix.Invert(translation) * v.InverseBindMatrix;

                    //mexe so no raiz
                    if (v.Name == "Root")
                    {
                        for (int j = 0; j < v.KFData.Count; j++)
                        {
                            AMT_KF p = v.KFData[j];
                            p.bindMatrix *= translation;
                            v.KFData[j] = p;
                        }
                    }

                    model.Joints[i] = v;
                }
            }
        }

        public static void SetYUp(ref AMT_MODEL model, Vector3 upVector)
        {
            if (upVector == Vector3.UnitY) return;

            //aplica a rotacao
            Matrix rotation;
            if (upVector == Vector3.UnitX)
                rotation = Matrix.CreateRotationZ(-MathHelper.PiOver2);
            else
                rotation = Matrix.CreateRotationX(-MathHelper.PiOver2);

            List<Vector3> vertices = new List<Vector3>();
            for (int i = 0; i < model.Vertices.Count; i++)
            {
                AMT_VERTEX v = model.Vertices[i];
                Vector3.Transform(ref v.Position, ref rotation, out v.Position);
                Vector3.TransformNormal(ref v.Normal, ref rotation, out v.Normal);
            }

            if (model.Joints != null)
            {
                for (int i = 0; i < model.Joints.Count; i++)
                {
                    AMT_JOINT v = model.Joints[i];

                    //como ele mexeu na posicao entao mexe so no raiz
                    if (v.Name == "Root")
                        v.BindMatrix *= rotation;

                    if (model.Joints[i].ParentID != -1)
                        v.MatrixAbsolute = model.Joints[i].BindMatrix *
                                           model.Joints[model.Joints[i].ParentID].MatrixAbsolute;
                    else
                        v.MatrixAbsolute = model.Joints[i].BindMatrix;

                    v.InverseBindMatrix = Matrix.Invert(rotation) * v.InverseBindMatrix;
                      
                    //mexe so no raiz
                    if (v.Name == "Root")
                    {
                        for (int j = 0; j < v.KFData.Count; j++)
                        {
                            AMT_KF p = v.KFData[j];
                            p.bindMatrix *= rotation;
                            v.KFData[j] = p;
                        }
                    }

                    model.Joints[i] = v;
                }
            }
        }

    }
}
