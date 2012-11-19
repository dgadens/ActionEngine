using System.Xml.Linq;

namespace ACFramework.FileStruct
{
    internal class Effect
    {
        public string Id;
        public string Name;
        public Vector4 Emission;
        public Vector4 Ambient;
        public Vector4 Diffuse;
        public Vector4 Specular;
        public float Shininess;
        public float Reflectivity;
        public Vector4 Transparent;
        public float Transparency;
        public string DiffuseTex;

        public Effect(XElement element)
        {
            if (element.Attribute("id") != null)
                Id = element.Attribute("id").Value;
            if (element.Attribute("name") != null)
                Name = element.Attribute("name").Value;

            var commonElement = element.Element(XName.Get("profile_COMMON", COLLADAConverter.Namespace));
            if (commonElement != null)
            {
                var techniqueElement = commonElement.Element(XName.Get("technique", COLLADAConverter.Namespace));
                if (techniqueElement != null)
                {
                    var phongElement = techniqueElement.Element(XName.Get("phong", COLLADAConverter.Namespace));
                    if (phongElement != null)
                    {
                        Emission = XmlReaderHelper.ReadColor(phongElement.Element(XName.Get("emission", COLLADAConverter.Namespace)).Element(XName.Get("color", COLLADAConverter.Namespace)));
                        Ambient = XmlReaderHelper.ReadColor(phongElement.Element(XName.Get("ambient", COLLADAConverter.Namespace)).Element(XName.Get("color", COLLADAConverter.Namespace)));
                        Specular = XmlReaderHelper.ReadColor(phongElement.Element(XName.Get("specular", COLLADAConverter.Namespace)).Element(XName.Get("color", COLLADAConverter.Namespace)));
                        Shininess = XmlReaderHelper.ReadFloat(phongElement.Element(XName.Get("shininess", COLLADAConverter.Namespace)).Element(XName.Get("float", COLLADAConverter.Namespace)));
                        Reflectivity = XmlReaderHelper.ReadFloat(phongElement.Element(XName.Get("reflectivity", COLLADAConverter.Namespace)).Element(XName.Get("float", COLLADAConverter.Namespace)));
                        Transparent = XmlReaderHelper.ReadColor(phongElement.Element(XName.Get("transparent", COLLADAConverter.Namespace)).Element(XName.Get("color", COLLADAConverter.Namespace)));
                        Transparency = XmlReaderHelper.ReadFloat(phongElement.Element(XName.Get("transparency", COLLADAConverter.Namespace)).Element(XName.Get("float", COLLADAConverter.Namespace)));

                        var diffuseElement = phongElement.Element(XName.Get("diffuse", COLLADAConverter.Namespace));
                        var diffuseColorElement = diffuseElement.Element(XName.Get("color", COLLADAConverter.Namespace));
                        if (diffuseColorElement != null)
                            Diffuse = XmlReaderHelper.ReadColor(diffuseColorElement);
                    }
                }
            }
        }
    }
}
