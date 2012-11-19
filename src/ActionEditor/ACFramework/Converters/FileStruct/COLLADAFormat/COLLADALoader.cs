using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Xml.Linq;
using ACFramework.FileStructs;

namespace ACFramework.FileStruct.COLLADAFormat
{
    public class COLLADALoader
    {
        internal static string Namespace;

        private List<Effect> _effects;
        private Dictionary<string, Effect> _materials;
        private List<Node> _libraryNodes;

        public AMT_MODEL ConvertCollada(Stream stream)
        {
            var amtModel = new AMT_MODEL();

            var doc = XDocument.Load(stream);
            var colladaNode = doc.Root;

            Namespace = doc.Root.Name.Namespace.NamespaceName;
            //ReadGeometries(new XElement(XName.Get("library_geometries", Namespace)), ref amtModel);

            //_effects = ReadEffects(colladaNode.Element(XName.Get("library_effects", COLLADAConverter.Namespace)));
            //_materials = ReadMaterials(colladaNode.Element(XName.Get("library_materials", COLLADAConverter.Namespace)));

            ////Lê os layers
            //var geometryNodes = colladaNode.Element(XName.Get("library_geometries", COLLADAConverter.Namespace));
            //if (geometryNodes != null)
            //    pmobDoc.Layers = ReadLayers(geometryNodes.Elements());

            //ReadLibraryNodes(colladaNode.Element(XName.Get("library_nodes", COLLADAConverter.Namespace)));

            //var scenesElement = colladaNode.Element(XName.Get("library_visual_scenes", COLLADAConverter.Namespace));
            //if (scenesElement != null)
            //{
            //    var nodes = ReadNodes(scenesElement.Element(XName.Get("visual_scene", COLLADAConverter.Namespace)));

            //    var rootNode = new Node("root", "root", nodes);
            //    pmobDoc.Primitive = CreatePrimitive(rootNode);
            //}

            ////Gira o objeto dependendo do up
            //var assetNode = colladaNode.Element(XName.Get("asset", COLLADAConverter.Namespace));
            //if (assetNode != null)
            //{
            //    var upAxis = assetNode.Elements(XName.Get("up_axis", COLLADAConverter.Namespace));
            //}


            return amtModel;
        }

        //private void ReadLibraryNodes(XElement element)
        //{
        //    _libraryNodes = new List<Node>();

        //    if (element != null)
        //        foreach (var nodeElement in element.Elements())
        //            _libraryNodes.Add(new Node(nodeElement, _libraryNodes));
        //}

        //private List<Node> ReadNodes(XElement element)
        //{
        //    var nodes = new List<Node>();

        //    if (element != null)
        //        foreach (var nodeElement in element.Elements())
        //            nodes.Add(new Node(nodeElement, _libraryNodes));

        //    return nodes;
        //}

        //private Dictionary<string, Effect> ReadMaterials(XElement materialsElement)
        //{
        //    var materials = new Dictionary<string, Effect>();

        //    if (materialsElement == null)
        //        return materials;

        //    foreach (var materialElement in materialsElement.Elements())
        //    {
        //        string name = materialElement.Attribute("name").Value;
        //        var effectElement = materialElement.Element(XName.Get("instance_effect", COLLADAConverter.Namespace));
        //        string effectName = effectElement.Attribute("url").Value.Substring(1);
        //        materials.Add(name, _effects.Where(e => e.Name == effectName).FirstOrDefault());
        //    }

        //    return materials;
        //}

        //private List<AMT_VERTEX> ReadLayers(IEnumerable<XElement> elements)
        //{
        //    var list = new List<AMT_VERTEX>(elements.Count());

        //    foreach (var geometryNode in elements)
        //        list.Add(ReadLayer(geometryNode));

        //    return list;
        //}

        //private PMobLayer ReadLayer(XElement geometryNode)
        //{
        //    var layer = new PMobLayer();
        //    if (geometryNode.Attribute("id") != null)
        //        layer.Name = geometryNode.Attribute("id").Value;

        //    layer.Meshes = ReadMeshes(geometryNode.Elements(XName.Get("mesh", COLLADAConverter.Namespace)));

        //    return layer;
        //}

        //private List<PMobMesh> ReadMeshes(IEnumerable<XElement> meshNodes)
        //{
        //    var meshes = new List<PMobMesh>();

        //    if (meshNodes == null)
        //        return meshes;

        //    foreach (var meshNode in meshNodes)
        //    {
        //        if (meshNode != null)
        //        {
        //            var sourceElements = meshNode.Elements(XName.Get("source", COLLADAConverter.Namespace));

        //            //Lê os vértices antes, pois são comuns a triângulos e linhas
        //            List<Vector3> vertices = null;
        //            var verticesNode = meshNode.Element(XName.Get("vertices", COLLADAConverter.Namespace));
        //            if (verticesNode != null)
        //            {
        //                var inputElementVert = verticesNode.Element(XName.Get("input", COLLADAConverter.Namespace));
        //                vertices = XmlReaderHelper.ReadVector3List(inputElementVert.Attribute("source").Value, sourceElements);
        //            }

        //            //Leitura dos triângulos (índices e coordenadas de textura)
        //            //Cada conjunto de triângulos é considerado um pmobmesh pois pode ter material diferente
        //            //As normais não são lidas, pois o collada usa normais por face e nós usamos por vértice
        //            //O collada também tem índices separados pra vértices e coordenadas de textura e pode ter mais de uma 
        //            //coordenada por vértice. Nesse caso replicamos o vértice para manter o modelo fiel ao original
        //            //A lista de índices segue o seguinte padrão:
        //            // v1 n1 c1 v2 n2 c2 v3 n3 c3...
        //            //Sendo que v = vertice, n = normal e c = coordenada de textura. Nesse caso o offset dos vértices é 0
        //            //e o offset das coordenadas é 2.
        //            var trianglesNodes = meshNode.Elements(XName.Get("triangles", COLLADAConverter.Namespace));
        //            foreach (var trianglesNode in trianglesNodes)
        //            {
        //                var pmobMesh = ReadMesh(sourceElements, vertices, trianglesNode);

        //                meshes.Add(pmobMesh);
        //            }
        //        }
        //    }

        //    return meshes;
        //}

        //private PMobMesh ReadMesh(IEnumerable<XElement> sourceElements, List<Vector3> vertices, XElement trianglesNode)
        //{
        //    var pmobMesh = new PMobMesh();

        //    if (trianglesNode.Attribute("material") != null)
        //    {
        //        var materialName = trianglesNode.Attribute("material").Value;
        //        if (_materials.ContainsKey(materialName))
        //        {
        //            var effect = _materials[materialName];
        //            pmobMesh.Color = effect.Diffuse;
        //        }
        //    }

        //    var inputElements = trianglesNode.Elements(XName.Get("input", COLLADAConverter.Namespace));
        //    int count = inputElements.Count();
        //    int[] offsets = new int[2];
        //    List<Vector2> texCoords = null;

        //    //Lê a lista de coordenadas e os offsets dos índices dos vértices e coordenadas
        //    //offset[0] = vértices
        //    //offset[1] = coordenadas de textura
        //    foreach (var inputElement in inputElements)
        //    {
        //        if (inputElement.Attribute("semantic") != null)
        //        {
        //            if (inputElement.Attribute("semantic").Value == "VERTEX")
        //                offsets[0] = int.Parse(inputElement.Attribute("offset").Value);
        //            else if (inputElement.Attribute("semantic").Value == "TEXCOORD")
        //            {
        //                offsets[1] = int.Parse(inputElement.Attribute("offset").Value);
        //                texCoords = XmlReaderHelper.ReadVector2List(inputElement.Attribute("source").Value, sourceElements);
        //            }
        //        }
        //    }

        //    Vector3[] newVertices = texCoords == null ? vertices.ToArray() : new Vector3[texCoords.Count];

        //    var indicesNode = trianglesNode.Element(XName.Get("p", COLLADAConverter.Namespace));
        //    if (indicesNode != null)
        //    {
        //        var strIndices = indicesNode.Value.Trim().Split(' ');
        //        var indices = strIndices.Select(index => int.Parse(index)).ToList();

        //        int i = 0;
        //        while (i < indices.Count)
        //        {
        //            //Se tiver coordenadas de textura cria um vértice pra cada uma, senão usa os vértices que já existem
        //            if (texCoords == null)
        //                pmobMesh.Indices.Add(indices[i + offsets[0]]);
        //            else
        //            {
        //                newVertices[indices[i + offsets[1]]] = vertices[indices[i + offsets[0]]];
        //                pmobMesh.Indices.Add(indices[i + offsets[1]]);
        //            }

        //            i += count;
        //        }

        //        pmobMesh.TextCoords = texCoords;
        //        pmobMesh.Vertices = newVertices.ToList();

        //        //inverte os triangulos
        //        for (int j = 0; j < pmobMesh.Indices.Count; j += 3)
        //        {
        //            var aux = pmobMesh.Indices[j];
        //            pmobMesh.Indices[j] = pmobMesh.Indices[j + 1];
        //            pmobMesh.Indices[j + 1] = aux;
        //        }
        //    }

        //    pmobMesh.GenerateNormals(true);

        //    return pmobMesh;
        //}

        //private List<Effect> ReadEffects(XElement effectsElement)
        //{
        //    var effects = new List<Effect>();

        //    if (effectsElement == null)
        //        return effects;

        //    foreach (var effectElement in effectsElement.Elements())
        //        effects.Add(new Effect(effectElement));

        //    return effects;
        //}

    }
}
