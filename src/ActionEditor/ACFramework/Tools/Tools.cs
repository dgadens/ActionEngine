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
                ret.X = float.Parse(splitValue[0], CultureInfo.InvariantCulture);
                ret.Y = float.Parse(splitValue[1], CultureInfo.InvariantCulture);
            }

            return ret;
        }

        public static Vector3 ConvertStringToVector3(string value)
        {
            Vector3 ret = new Vector3();
            string[] splitValue = value.Split(' ');

            if (splitValue.Length >= 3)
            {
                ret.X = float.Parse(splitValue[0], CultureInfo.InvariantCulture);
                ret.Y = float.Parse(splitValue[1], CultureInfo.InvariantCulture);
                ret.Z = float.Parse(splitValue[2], CultureInfo.InvariantCulture);
            }

            return ret;
        }

        public static Vector4 ConvertStringToVector4(string value)
        {
            Vector4 ret = new Vector4();
            string[] splitValue = value.Split(' ');

            if (splitValue.Length == 4)
            {
                ret.X = float.Parse(splitValue[0], CultureInfo.InvariantCulture);
                ret.Y = float.Parse(splitValue[1], CultureInfo.InvariantCulture);
                ret.Z = float.Parse(splitValue[2], CultureInfo.InvariantCulture);
                ret.W = float.Parse(splitValue[3], CultureInfo.InvariantCulture);
            }

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
            {
                vertices.Add(model.Vertices[i].Position);
            }

            BoundingSphere sphere = BoundingSphere.CreateFromPoints(vertices);

            //redimensiona os vertices
            float scalefactor = 1.0f / sphere.Radius;
            for (int i = 0; i < model.Vertices.Count; i++)
            {
                AMT_VERTEX v = model.Vertices[i];
                v.Position = v.Position * scalefactor;
                model.Vertices[i] = v;
            }

            //redimensiona tb todos os bones e os keyframes
            if (model.Joints != null)
            {
                for (int i = 0; i < model.Joints.Count; i++)
                {
                    AMT_JOINT v = model.Joints[i];
                    v.Position = v.Position * scalefactor;

                    for (int j = 0; j < v.KFPosition.Count; j++)
                    {
                        AMT_KF_POS p = v.KFPosition[j];
                        p.Position = p.Position * scalefactor;
                        v.KFPosition[j] = p;
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
            Vector3 positionAjust = (bb.Max + bb.Min) / 2;

            //redimensiona os vertices
            for (int i = 0; i < model.Vertices.Count; i++)
            {
                AMT_VERTEX v = model.Vertices[i];
                v.Position = v.Position - positionAjust;
                model.Vertices[i] = v;
            }

            //reposiciona tb todos os bones e os keyframes
            if (model.Joints != null)
            {
                for (int i = 0; i < model.Joints.Count; i++)
                {
                    AMT_JOINT v = model.Joints[i];
                    v.Position = v.Position - new Vector4(positionAjust,0);

                    for (int j = 0; j < v.KFPosition.Count; j++)
                    {
                        AMT_KF_POS p = v.KFPosition[j];
                        p.Position = p.Position - positionAjust;
                        v.KFPosition[j] = p;
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
                model.Vertices[i] = v;
            }

            //TODO: resposicionar as rotacoes dos bones, nao fiz pq nao estou usando os bones nessa primeira versao da engine

            //reposiciona tb todos os bones e os keyframes
            //if (model.Joints != null)
            //{
            //    for (int i = 0; i < model.Joints.Count; i++)
            //    {
            //        AMT_JOINT v = model.Joints[i];
            //        v.Position = v.Position - new Vector4(positionAjust, 0);

            //        for (int j = 0; j < v.KFPosition.Count; j++)
            //        {
            //            AMT_KF_POS p = v.KFPosition[j];
            //            p.Position = p.Position - positionAjust;
            //            v.KFPosition[j] = p;
            //        }

            //        model.Joints[i] = v;
            //    }
            //}
        }

    }
}
