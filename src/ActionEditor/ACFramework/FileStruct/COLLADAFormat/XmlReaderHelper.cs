using System.Collections.Generic;
using System.Globalization;
using System.Xml.Linq;

namespace ACFramework.FileStruct
{
    internal static class XmlReaderHelper
    {
        public static List<float> ReadFloatList(string name, IEnumerable<XElement> sources)
        {
            foreach (var source in sources)
            {
                if ("#" + source.Attribute("id").Value == name)
                {
                    var floatList = new List<float>();
                    var floatArrayElement = source.Element(XName.Get("float_array", COLLADAConverter.Namespace));
                    if (floatArrayElement != null)
                        return ReadFloatList(floatArrayElement);
                }
            }

            return null;
        }

        public static List<float> ReadFloatList(XElement element)
        {
            var floatList = new List<float>();

            var strArray = element.Value.Trim().Replace("\n", "").Split(' ');
            for (int i = 0; i < strArray.Length; i++)
                if(strArray[i] != "")
                    floatList.Add(float.Parse(strArray[i], CultureInfo.InvariantCulture));

            return floatList;
        }

        public static List<Vector3> ReadVector3List(string name, IEnumerable<XElement> sources)
        {
            foreach (var source in sources)
            {
                if ("#" + source.Attribute("id").Value == name)
                {
                    var floatArrayElement = source.Element(XName.Get("float_array", COLLADAConverter.Namespace));
                    if (floatArrayElement != null)
                        return ReadVector3List(floatArrayElement);
                }
            }

            return null;
        }

        public static List<Vector3> ReadVector3List(XElement element)
        {
            var vector3List = new List<Vector3>();

            var strArray = element.Value.Trim().Split(' ');
            for (int i = 0; i < strArray.Length; i += 3)
                vector3List.Add(new Vector3(float.Parse(strArray[i], CultureInfo.InvariantCulture),
                    float.Parse(strArray[i + 1], CultureInfo.InvariantCulture),
                    float.Parse(strArray[i + 2], CultureInfo.InvariantCulture)));

            return vector3List;
        }

        public static List<Vector2> ReadVector2List(string name, IEnumerable<XElement> sources)
        {
            foreach (var source in sources)
            {
                if ("#" + source.Attribute("id").Value == name)
                {
                    var floatArrayElement = source.Element(XName.Get("float_array", COLLADAConverter.Namespace));
                    if (floatArrayElement != null)
                        return ReadVector2List(floatArrayElement);
                }
            }

            return null;
        }

        public static List<Vector2> ReadVector2List(XElement floatArrayElement)
        {
            var vector2List = new List<Vector2>();

            var strArray = floatArrayElement.Value.Trim().Split(' ');
            for (int i = 0; i < strArray.Length; i += 2)
                vector2List.Add(new Vector2(float.Parse(strArray[i], CultureInfo.InvariantCulture),
                    float.Parse(strArray[i + 1], CultureInfo.InvariantCulture)));

            return vector2List;
        }

        public static Vector4 ReadColor(XElement element)
        {
            var strColor = element.Value.Trim().Split(' ');
            return new Vector4(float.Parse(strColor[0]), float.Parse(strColor[1]), float.Parse(strColor[2]), float.Parse(strColor[3]));
        }

        public static float ReadFloat(XElement element)
        {
            var strFloat = element.Value.Trim();
            return float.Parse(strFloat, CultureInfo.InvariantCulture);
        }

        public static Matrix ReadMatrix(XElement element)
        {
            var floatList = ReadFloatList(element);
            var matrix = new Matrix(
                floatList[0], floatList[4], floatList[8], floatList[12],
                floatList[1], floatList[5], floatList[9], floatList[13],
                floatList[2], floatList[6], floatList[10], floatList[14],
                floatList[3], floatList[7], floatList[11], floatList[15]);

            return matrix;
        }
    }
}
