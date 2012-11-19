using System.Collections.Generic;
using System.Xml.Linq;

namespace ACFramework.FileStruct
{
    internal class Node
    {
        public string Id;
        public string Name;
        public string GeometryName;
        public Matrix Matrix;
        public List<Node> Nodes;

        public Node(XElement element, List<Node> libraryNodes)
        {
            if (element.Attribute("id") != null)
                Id = element.Attribute("id").Value;
            if (element.Attribute("name") != null)
                Name = element.Attribute("name").Value;

            var matrixElement = element.Element(XName.Get("matrix", COLLADAConverter.Namespace));
            if (matrixElement != null)
                Matrix = XmlReaderHelper.ReadMatrix(matrixElement);
            else
                Matrix = Matrix.Identity;

            var geometryElement = element.Element(XName.Get("instance_geometry", COLLADAConverter.Namespace));
            if (geometryElement != null)
                GeometryName = geometryElement.Attribute("url").Value.Substring(1);

            var nodesElement = element.Elements(XName.Get("node", COLLADAConverter.Namespace));
            Nodes = new List<Node>();
            foreach (var nodeElement in nodesElement)
                Nodes.Add(new Node(nodeElement, libraryNodes));

            var instanceElement = element.Element(XName.Get("instance_node", COLLADAConverter.Namespace));
            if (instanceElement != null)
            {
                var childName = instanceElement.Attribute("url").Value.Substring(1);
                foreach (var node in libraryNodes)
                    if (node.Name == childName)
                        Nodes.Add(node);
            }
        }

        public Node(string id, string name, List<Node> nodes)
        {
            this.Id = id;
            this.Name = name;
            this.Nodes = nodes;
            this.Matrix = Matrix.Identity;
        }
    }
}
