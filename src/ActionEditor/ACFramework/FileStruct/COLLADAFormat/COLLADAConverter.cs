using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ACFramework.FileStructs;
using System.Xml.Linq;
using System.Globalization;

namespace ACFramework.FileStruct
{
    public class COLLADAConverter : IConverter
    {
        static string Namespace;

        Import _import;

        public Nullable<AMT_MODEL> Convert(string path, Import import)
        {
            _import = import;

            //carrega o arquivo
            var doc = XDocument.Load(path);
            Namespace = doc.Root.Name.Namespace.NamespaceName;

            Vector3 upVector = Vector3.UnitY;
            var colladaNode = doc.Root;

            #region Pega o vetor UP
            XElement asset = colladaNode.Element(XName.Get("asset", Namespace));
            if (asset != null)
            {
                string up = asset.Element(XName.Get("up_axis", Namespace)).Value;

                if (string.Compare(up, "z_up", true) == 0)
                    upVector = Vector3.UnitZ;
                else if (string.Compare(up, "x_up", true) == 0)
                    upVector = Vector3.UnitX;
            }
            #endregion

            AMT_MODEL amtModel = ConvertDAEtoAMT(colladaNode);
            Tools.CenterPivot(ref amtModel);
            Tools.UniformScale(ref amtModel);
            Tools.SetYUp(ref amtModel, upVector);

            return amtModel;
        }

        private AMT_MODEL ConvertDAEtoAMT(XElement rootElement)
        {
            AMT_MODEL amtModel = new AMT_MODEL();

            amtModel.Vertices = new List<AMT_VERTEX>();

            ConvertGeometries(rootElement, ref amtModel);

            ConvertBones(rootElement, ref amtModel);

            ConvertController(rootElement, ref amtModel);

            ConvertHead(ref amtModel);

            Optimize(ref amtModel);

            return amtModel;
        }

        private void ConvertBones(XElement rootElement, ref AMT_MODEL amtModel)
        {
            //primeiro eu encontro o library_visual_scenes
            XElement libraryVisualScenes = rootElement.Element(XName.Get("library_visual_scenes", Namespace));
            XElement visualScene = libraryVisualScenes.Element(XName.Get("visual_scene", Namespace));

            //se tem o nodo skeleton
            XElement rootNode = FindRootSkeletonNodeId(visualScene);

            //Começo a percorrer os nodes procurando pela hierarquia de bones
            if (rootNode != null)
            {
                //adiciona o bone raiz
                amtModel.Joints = new List<AMT_JOINT>();
                Matrix matrix = Matrix.Identity;

                string sid = rootNode.Attribute("sid").Value;

                XElement matrixElement = rootNode.Element(XName.Get("matrix", Namespace));
                matrix = Tools.ConvertStringToMatrix(matrixElement.Value, 0);
                
                AMT_JOINT newBone = new AMT_JOINT();
                newBone.SID = sid;
                newBone.ID = 0;
                newBone.ParentID = -1; //nao tem pai
                newBone.Name = "Root";
                
                newBone.NumChildren = 0;
                newBone.JointChildren = new List<uint>();

                newBone.NumKF = 0;
                newBone.KFData = new List<AMT_KF>();

                newBone.IsAnimated = 0;
                newBone.Flag = 0;

                newBone.BindMatrix = matrix;
                newBone.MatrixAbsolute = matrix;
                newBone.InverseBindMatrix = Matrix.Invert(matrix);

                amtModel.Joints.Add(newBone);

                FillBoneNodes(ref newBone, rootNode, ref amtModel);
            }
        }

        private XElement FindRootSkeletonNodeId(XElement visualNode)
        {
            string rootNodeId = null;
            List<XElement> nodes = visualNode.Elements(XName.Get("node", Namespace)).ToList();
            //encontro algum nodo q tem o instance_controller
            XElement controller = nodes.Find(
                item => { return item.Element(XName.Get("instance_controller", Namespace)) != null; }
                );
            controller = controller.Element(XName.Get("instance_controller", Namespace));


            //acho o skeleton pra procurar o id do nodo raiz
            if (controller != null)
                rootNodeId = controller.Element(XName.Get("skeleton", Namespace)).Value.Substring(1);
            else
                return null;

            //procuro o nodo raiz
            XElement rootNode = nodes.Find(
                                item => { return item.Attribute("type") != null && 
                                                 item.Attribute("type").Value.Contains("JOINT") &&
                                                 item.Attribute("id").Value == rootNodeId; }
                                );

            return rootNode;
        }

        private void FillBoneNodes(ref AMT_JOINT parentBone, XElement boneNode, ref AMT_MODEL amtModel)
        {
            //pego todos os nodes
            List<XElement> nodes = boneNode.Elements(XName.Get("node", Namespace)).ToList();
            foreach (var node in nodes)
            {
                //se é um bone
                if (node.Attribute("type") != null &&
                    node.Attribute("type").Value.Contains("JOINT"))
                {
                    Matrix matrix = Matrix.Identity;

                    //pega o nome e a matrix do bone
                    string boneName = node.Attribute("name").Value;
                    string sid = node.Attribute("sid").Value;
                    XElement matrixElement = node.Element(XName.Get("matrix", Namespace));
                    matrix = Tools.ConvertStringToMatrix(matrixElement.Value, 0);

                    // Create this node, use the current number of bones as number.
                    AMT_JOINT newBone = new AMT_JOINT();
                    newBone.SID = sid;
                    newBone.ID = (uint)amtModel.Joints.Count();
                    newBone.ParentID = (int)parentBone.ID; 
                    newBone.Name = boneName;

                    newBone.NumChildren = 0;
                    newBone.JointChildren = new List<uint>();

                    newBone.NumKF = 0;
                    newBone.KFData = new List<AMT_KF>();

                    newBone.IsAnimated = 0;
                    newBone.Flag = 0;

                    //atualiza o bone pai
                    parentBone.NumChildren++;
                    parentBone.JointChildren.Add(newBone.ID);

                    //matrix relativa
                    newBone.BindMatrix = matrix;
                    //absoluta = relativa do atual * absoluta do pai
                    newBone.MatrixAbsolute = matrix * parentBone.MatrixAbsolute;
                    //inversa da absoluta atual usado no skinning
                    newBone.InverseBindMatrix = Matrix.Invert(newBone.MatrixAbsolute);

                    amtModel.Joints.Add(newBone);
                    
                    // vai para os filhos
                    FillBoneNodes(ref newBone, node, ref amtModel);
                }
            }
        }

        private void ConvertController(XElement rootElement, ref AMT_MODEL amtModel)
        {
            XElement libraryController = rootElement.Element(XName.Get("library_controllers", Namespace));
            List<XElement> controllers = libraryController.Elements(XName.Get("controller", Namespace)).ToList();
            
            foreach (XElement controller in controllers)
	        {
                XElement skin = controller.Element(XName.Get("skin", Namespace));

                //pega o nome do source
                XElement vWeightsElement = skin.Element(XName.Get("vertex_weights", Namespace));
                int numberOfWeights = int.Parse(vWeightsElement.Attribute("count").Value);

                //pego o numero de bones q influenciam cada vertice
                string vCount = vWeightsElement.Element(XName.Get("vcount", Namespace)).Value;
                //par (indice do bone seguido do indice do peso)
                string v = vWeightsElement.Element(XName.Get("v", Namespace)).Value;

                List<XElement> inputElements = vWeightsElement.Elements(XName.Get("input", Namespace)).ToList();

                //procura o source joint
                XElement jointInputElement = inputElements.Find(item => { return item.Attribute("semantic").Value == "JOINT"; });
                string jointSourceName = jointInputElement.Attribute("source").Value.Substring(1);
                //procura o source weight
                XElement weightInputElement = inputElements.Find(item => { return item.Attribute("semantic").Value == "WEIGHT"; });
                string weightSourceName = weightInputElement.Attribute("source").Value.Substring(1);

                //pega o bindmatrix
                string bindShapeMatrix = skin.Element(XName.Get("bind_shape_matrix", Namespace)).Value;
                Matrix bindMatrix = Tools.ConvertStringToMatrix(bindShapeMatrix, 0);
                
                //vai no source Joint descoberto e pega a lista de joints e weights
                List<XElement> sourceElements = skin.Elements(XName.Get("source", Namespace)).ToList();

                //pega a lista de nomes de bones na ordem
                XElement jointSourceElement = sourceElements.Find(item => { return item.Attribute("id").Value == jointSourceName; });
                string jointNames = jointSourceElement.Element(XName.Get("Name_array", Namespace)).Value;
                //pega a lista de weights
                XElement weightSourceElement = sourceElements.Find(item => { return item.Attribute("id").Value == weightSourceName; });
                string weightValues = weightSourceElement.Element(XName.Get("float_array", Namespace)).Value;

                //joga pra o array as influencias
                string[] jointsOnSkin = jointNames.Trim().Split(' ');
                string[] weights = weightValues.Replace('\n', ' ').Trim().Split(' ');
                string[] influenceNumber = vCount.Trim().Split(' ');
                string[] boneIndexWeight = v.Trim().Split(' ');

                //construo um de para, dos indices que estao na lista de joints do amtmodel
                List<uint> _from = new List<uint>();
                List<uint> _to = new List<uint>();

                //converto os indices dos bones q estao no <v> para os indices da lista que ja esta na lista
                for (uint i = 0; i < amtModel.Joints.Count; i++)
                {
                    for (uint j = 0; j < jointsOnSkin.Length; j++)
                    {
                        //procuro o joint na lista ja salva
                        if (amtModel.Joints[(int)i].SID == jointsOnSkin[j])
                        {
                            _from.Add(j);
                            _to.Add(i);
                        }
                    }
                }

                //atribui para os vertices o id dos joints e tb os pesos
                int maxBonesPerVertex;
                int vIndex = 0;
                for (int i = 0; i < influenceNumber.Length; i++)
                {
                    //pra cada vertice ele procura no maximo 4 bones q o influenciam
                    maxBonesPerVertex = 4;
                    AMT_VERTEX vertex = amtModel.Vertices[i];

                    int numInfluence = int.Parse(influenceNumber[i]);

                    while (numInfluence > 0)
                    {
                        numInfluence--;

                        //seleciona o indice do bone na lista <v> e procura do de-para para achar o indice no amtmodel
                        uint boneIndex = uint.Parse(boneIndexWeight[vIndex++]);
                        for (int j = 0; j < _from.Count; j++)
                        {
                            if (_from[j] == boneIndex)
                                boneIndex = _to[j];
                        }


                        uint weightIndex = uint.Parse(boneIndexWeight[vIndex++]);

                        if (maxBonesPerVertex == 4)
                        {
                            vertex.BoneID_A = boneIndex;
                            vertex.BoneWeight_A = float.Parse(weights[weightIndex]);
                            maxBonesPerVertex--;
                            continue;
                        }
                        if (maxBonesPerVertex == 3)
                        {
                            vertex.BoneID_B = boneIndex;
                            vertex.BoneWeight_B = float.Parse(weights[weightIndex]);
                            maxBonesPerVertex--;
                            continue;
                        }
                        if (maxBonesPerVertex == 2)
                        {
                            vertex.BoneID_C = boneIndex;
                            vertex.BoneWeight_C = float.Parse(weights[weightIndex]);
                            maxBonesPerVertex--;
                            continue;
                        }
                        if (maxBonesPerVertex == 1)
                        {
                            vertex.BoneID_D = boneIndex;
                            vertex.BoneWeight_D = float.Parse(weights[weightIndex]);
                            maxBonesPerVertex--;
                            continue;
                        }

                    }
                }
	        }
            
        }

        private void ConvertGeometries(XElement rootElement, ref AMT_MODEL amtModel)
        {
            //pega a tag principal das geometrias
            XElement libraryGeometries = rootElement.Element(XName.Get("library_geometries", Namespace));
            List<XElement> geometries = libraryGeometries.Elements(XName.Get("geometry", Namespace)).ToList();
            if (geometries.Count > 0)
            {
                amtModel.Meshes = new List<AMT_MESH>();
                amtModel.Faces = new List<AMT_FACE>();
                amtModel.Materials = new List<AMT_MATERIAL>();
                amtModel.Head = new AMT_HEAD();

                foreach (XElement geometry in geometries)
                {
                    XElement meshElement = geometry.Element(XName.Get("mesh", Namespace));

                    //resgata os triangulos, se for uma versao anterior a 1.4.1 o nome é polygons
                    XElement triangles = meshElement.Element(XName.Get("triangles", Namespace));
                    if (triangles == null)
                        triangles = meshElement.Element(XName.Get("polygons", Namespace));

                    //se existe uma tag triangles entao ele vai pra frente
                    if (triangles != null)
                    {
                        AMT_MESH mesh = new AMT_MESH();
                        mesh.Name = geometry.Attribute("name").Value;
                        mesh.FaceIndices = new List<int>();
                        amtModel.Meshes.Add(mesh);

                        //converte o mesh, pega todos os vertices, indices e faces e ja armazena no model amt
                        ConvertMesh(meshElement, triangles, ref amtModel);

                        //carrega o material
                        //pega no visual scene o nome do material
                        XElement libraryVisualScene = rootElement.Element(XName.Get("library_visual_scenes", Namespace));
                        XElement visualSceneElement = libraryVisualScene.Element(XName.Get("visual_scene", Namespace));
                        List<XElement> visualSceneNodes = visualSceneElement.Elements(XName.Get("node", Namespace)).ToList();
                        XElement geometryScene = visualSceneNodes.Find(item => { return item.Attribute("name").Value == mesh.Name; });
                        XElement instanceGeometry = geometryScene.Element(XName.Get("instance_geometry", Namespace));

                        //se existe a instancia geometry entao ele pega senao ele tem skin ai vai pelo controller
                        XElement materialScene;
                        if (instanceGeometry != null)
                        {
                            XElement bindMaterial = instanceGeometry.Element(XName.Get("bind_material", Namespace));
                            XElement techniqueCommon = bindMaterial.Element(XName.Get("technique_common", Namespace));
                            List<XElement> materialsScene = techniqueCommon.Elements(XName.Get("instance_material", Namespace)).ToList();
                            materialScene = materialsScene.Find(item => { return item.Attribute("symbol").Value == triangles.Attribute("material").Value; });
                        }
                        else
                        {
                            XElement controller = geometryScene.Element(XName.Get("instance_controller", Namespace));
                            string skeletonName = controller.Element(XName.Get("skeleton", Namespace)).Value.Substring(1);
                            if (skeletonName != null)
                                amtModel.Head.HasSkeleton = 1;
                            XElement bindMaterial = controller.Element(XName.Get("bind_material", Namespace));
                            XElement techniqueCommon = bindMaterial.Element(XName.Get("technique_common", Namespace));
                            List<XElement> materialsScene = techniqueCommon.Elements(XName.Get("instance_material", Namespace)).ToList();
                            materialScene = materialsScene.Find(item => { return item.Attribute("symbol").Value == triangles.Attribute("material").Value; });
                        }

                        string materialID = materialScene.Attribute("target").Value.Substring(1);
                        if (materialID != null)
                        {
                            //carrega a tag de materiais
                            XElement libraryMaterials = rootElement.Element(XName.Get("library_materials", Namespace));
                            List<XElement> materials = libraryMaterials.Elements(XName.Get("material", Namespace)).ToList();

                            //carrega a tag de imagens
                            XElement libraryImages = rootElement.Element(XName.Get("library_images", Namespace));
                            List<XElement> images = null;
                            if (libraryImages!=null)
                                images = libraryImages.Elements(XName.Get("image", Namespace)).ToList();

                            //carrega os atributos do primeiro effect
                            XElement libraryEffect = rootElement.Element(XName.Get("library_effects", Namespace));
                            List<XElement> effectElements = libraryEffect.Elements(XName.Get("effect", Namespace)).ToList();

                            XElement materialElement = materials.Find(item => { return item.Attribute("id").Value == materialID; });

                            if (materialElement != null)
                            {
                                AMT_MATERIAL material = ConvertMaterial(materialElement, effectElements, images);
                                amtModel.Materials.Add(material);

                                //seta o id do material q ta na estrutura principal
                                mesh.MaterialID = (uint)amtModel.Materials.Count() - 1;
                            }
                        }
                    }
                }
            }
        }

        private void ConvertHead(ref AMT_MODEL amtModel)
        {
            amtModel.Head.Version = 1;
            amtModel.Head.NumFaces = (uint)amtModel.Faces.Count;
            amtModel.Head.NumIndices = (uint)amtModel.Faces.Count * 3;
            amtModel.Head.NumMeshes = (uint)amtModel.Meshes.Count;
            amtModel.Head.NumVertices = (uint)amtModel.Vertices.Count;
            amtModel.Head.NumMaterials = (uint)amtModel.Materials.Count;
            amtModel.Head.NumJoints = (uint)amtModel.Joints.Count;
        }

        private void ConvertMesh(XElement meshElement, XElement triangles, ref AMT_MODEL amtModel)
        {
            //pega o mesh atual
            uint meshID = (uint)amtModel.Meshes.Count - 1;
            AMT_MESH mesh = amtModel.Meshes[amtModel.Meshes.Count - 1];

            //ja armazena o numero de faces do mesh
            mesh.NumFaces = int.Parse(triangles.Attribute("count").Value);

            //pega os inputs vertex, normal, texcoord etc
            List<XElement> inputs = triangles.Elements(XName.Get("input", Namespace)).ToList();
            XElement vertex = inputs.Find(item => { return item.Attribute("semantic").Value == "VERTEX"; });
            XElement normal = inputs.Find(item => { return item.Attribute("semantic").Value == "NORMAL"; });
            XElement texcoord = inputs.Find(item => { return item.Attribute("semantic").Value == "TEXCOORD"; });

            //elementos que tem na tag vertices, procurar o id = vertex.source
            List<XElement> verticesElements = meshElement.Elements(XName.Get("vertices", Namespace)).ToList();
            XElement verticeElement = verticesElements.Find(item=> { return item.Attribute("id").Value == vertex.Attribute("source").Value.Substring(1); });
            //agora com o vertice certo pega o position
            List<XElement> inputsVerticesElement = verticeElement.Elements(XName.Get("input", Namespace)).ToList();
            XElement position = inputsVerticesElement.Find(item => { return item.Attribute("semantic").Value == "POSITION"; });

            //agora é so carregar os dados dos sources conforme os id dos inputs position, normal, texcoord
            List<XElement> sources = meshElement.Elements(XName.Get("source", Namespace)).ToList();
            XElement sourcePosition = sources.Find(item => { return item.Attribute("id").Value == position.Attribute("source").Value.Substring(1); });
            XElement sourceNormal = sources.Find(item => { return item.Attribute("id").Value == normal.Attribute("source").Value.Substring(1); });
            XElement sourceTexcoord = sources.Find(item => { return item.Attribute("id").Value == texcoord.Attribute("source").Value.Substring(1); });

            //pega os indices e junta tudo em um unico string
            List<XElement> ps = triangles.Elements(XName.Get("p", Namespace)).ToList();
            string indices = null;
            foreach (var item in ps)
                indices += ' ' + item.Value;
            //separa todos os indices usando os offsets 
            string[] strIndices = indices.Replace('\n',' ').Trim().Split(' ');
            List<int> positionIndices = new List<int>(); 
            int positionOffset = int.Parse(vertex.Attribute("offset").Value);
            List<int> normalIndices = new List<int>();
            int normalOffset = int.Parse(normal.Attribute("offset").Value);
            List<int> texcoordIndices = new List<int>();
            int texcoordOffset = int.Parse(texcoord.Attribute("offset").Value);

            for (int i = 0; i < strIndices.Length; i += sources.Count)
            {
                positionIndices.Add(int.Parse(strIndices[i + positionOffset]));
                normalIndices.Add(int.Parse(strIndices[i + normalOffset]));
                texcoordIndices.Add(int.Parse(strIndices[i + texcoordOffset]));
            }
            //vertices reais
            List<Vector3> positions = GetPositions(sourcePosition);
            List<Vector3> normals = GetNormals(sourceNormal);
            List<Vector2> texcoords = GetTexCoords(sourceTexcoord);

            //agora é so pegar os indices e vincular com os arrays pra criar os vertices q vao para o amt
            for (int i = 0; i < positionIndices.Count; i++)
            {
                AMT_VERTEX v = new AMT_VERTEX();
                v.Position = positions[positionIndices[i]];
                v.Normal = normals[normalIndices[i]];
                v.TexCoord1 = texcoords[texcoordIndices[i]];
                v.TexCoord1.Y = -v.TexCoord1.Y; //Inverto o Y pq o exportador do max usa as coordenadas do opengl

                amtModel.Vertices.Add(v);
            }

            //carrega as faces
            for (int i = 0; i < amtModel.Vertices.Count; i+=3)
            {
                AMT_FACE f = new AMT_FACE();
                f.MeshID = meshID;
                f.Indices = new List<int>();
                f.Indices.Add(i);
                f.Indices.Add(i+1);
                f.Indices.Add(i+2);
                f.Normal = Tools.GetNormal(amtModel.Vertices[i].Position, amtModel.Vertices[i + 1].Position, amtModel.Vertices[i + 2].Position);

                amtModel.Faces.Add(f);

                mesh.FaceIndices.Add(amtModel.Faces.Count - 1);
            }

            //finalmente seta o mesh atual
            amtModel.Meshes[amtModel.Meshes.Count - 1] = mesh;
        }

        private AMT_MATERIAL ConvertMaterial(XElement materialElement, List<XElement> effectElements, List<XElement> images)
        {
            AMT_MATERIAL material = new AMT_MATERIAL();

            material.Name = materialElement.Attribute("name").Value;

            XElement effect = materialElement.Element(XName.Get("instance_effect", Namespace));

            //pega o elemento effect
            string urlEffect = effect.Attribute("url").Value.Substring(1);
            XElement effectElement = effectElements.Find(item => { return item.Attribute("id").Value == urlEffect; });

            XElement profile_COMMON = effectElement.Element(XName.Get("profile_COMMON", Namespace));
            XElement technique = profile_COMMON.Element(XName.Get("technique", Namespace));

            #region Blinn
            //nao sei pq mas tem 2 materiais com nomes diferentes q tem os mesmo dados
            XElement blinn = technique.Element(XName.Get("blinn", Namespace));
            if (blinn == null)
                blinn = technique.Element(XName.Get("phong", Namespace));

            if (blinn != null)
            {
                //emissive color
                XElement emissive = blinn.Element(XName.Get("emission", Namespace));
                if (emissive != null)
                {
                    string emissiveColor = emissive.Element(XName.Get("color", Namespace)).Value;
                    if (!string.IsNullOrEmpty(emissiveColor))
                        material.Emissive = Tools.ConvertStringToVector3(emissiveColor);
                }


                //ambient color
                XElement ambient = blinn.Element(XName.Get("ambient", Namespace));
                if (ambient != null)
                {
                    string ambientColor = ambient.Element(XName.Get("color", Namespace)).Value;
                    if (!string.IsNullOrEmpty(ambientColor))
                        material.Ambient = Tools.ConvertStringToVector3(ambientColor);
                }


                //diffuse texture/color (se existir textura ele usa a textura senao a cor)
                XElement diffuse = blinn.Element(XName.Get("diffuse", Namespace));
                if (diffuse != null)
                {
                    XElement diffuseTextureElement = diffuse.Element(XName.Get("texture", Namespace));
                    if (diffuseTextureElement != null)
                    {
                        string textureName = diffuseTextureElement.Attribute("texture").Value.Replace("-sampler", "");

                        //vai para a lista de imagens para pegar o nome correto da imagem
                        XElement imageElement = images.Find(item => { return item.Attribute("id").Value == textureName; });
                        XElement imageInitForm = imageElement.Element(XName.Get("init_from", Namespace));

                        material.DiffuseTexture = System.IO.Path.GetFileName(imageInitForm.Value);
                    }
                    else
                    {
                        XElement diffuseColorElement = diffuse.Element(XName.Get("color", Namespace));
                        if (diffuseColorElement != null)
                        {
                            string diffuseColor = diffuseColorElement.Value;
                            if (!string.IsNullOrEmpty(diffuseColor))
                                material.Diffuse = Tools.ConvertStringToVector3(diffuseColor);
                        }
                    }
                }


                //specular texture/color (se existir textura ele usa a textura senao a cor)
                XElement specular = blinn.Element(XName.Get("specular", Namespace));
                if (specular != null)
                {
                    XElement specularTextureElement = specular.Element(XName.Get("texture", Namespace));
                    if (specularTextureElement != null)
                    {
                        string textureName = specularTextureElement.Attribute("texture").Value.Replace("-sampler", "");

                        //vai para a lista de imagens para pegar o nome correto da imagem
                        XElement imageElement = images.Find(item => { return item.Attribute("id").Value == textureName; });
                        XElement imageInitForm = imageElement.Element(XName.Get("init_from", Namespace));

                        material.SpecularTexture = System.IO.Path.GetFileName(imageInitForm.Value);
                    }
                    else
                    {
                        XElement specularColorElement = specular.Element(XName.Get("color", Namespace));
                        if (specularColorElement != null)
                        {
                            string specularColor = specularColorElement.Value;
                            if (!string.IsNullOrEmpty(specularColor))
                                material.Specular = Tools.ConvertStringToVector3(specularColor);
                        }
                    }
                }


                //transparency factor
                XElement transparency = blinn.Element(XName.Get("transparency", Namespace));
                if (transparency != null)
                {
                    string transparencyFactor = transparency.Element(XName.Get("float", Namespace)).Value;
                    if (!string.IsNullOrEmpty(transparencyFactor))
                        material.Transparency = float.Parse(transparencyFactor, CultureInfo.InvariantCulture);
                    else
                        material.Transparency = 0.0f; //opaco
                }
            }

            #endregion

            #region Extra/ NormalMap
            XElement extra = technique.Element(XName.Get("extra", Namespace));
            if (extra != null)
            {
                XElement techniqueExtra = extra.Element(XName.Get("technique", Namespace));
                XElement bump = techniqueExtra.Element(XName.Get("bump", Namespace));
                if (bump != null)
                {
                    XElement bumpTextureElement = bump.Element(XName.Get("texture", Namespace));
                    if (bumpTextureElement != null)
                    {
                        string textureName = bumpTextureElement.Attribute("texture").Value.Replace("-sampler", "");

                        //vai para a lista de imagens para pegar o nome correto da imagem
                        XElement imageElement = images.Find(item => { return item.Attribute("id").Value == textureName; });
                        XElement imageInitForm = imageElement.Element(XName.Get("init_from", Namespace));

                        material.NormalTexture = System.IO.Path.GetFileName(imageInitForm.Value);
                    }
                }
            }

            #endregion

            return material;
        }

        private void Optimize(ref AMT_MODEL amtModel)
        {
            List<AMT_VERTEX> vertices = new List<AMT_VERTEX>();
            vertices.AddRange(amtModel.Vertices);

            amtModel.Vertices.Clear();

            //grava os vertices agora na ordem certa triangulados
            int currentVertices = (int)amtModel.Head.NumVertices;

            // optimazacao dor vertices
            for (int i = 0; i < amtModel.Head.NumVertices; i++)
            {
                _import.ProgressText = "Importing Faces, recompute vertices " + i.ToString() + "/" + amtModel.Head.NumVertices.ToString();

                // se o flag for 42 ele vai para o proximo
                if (vertices[i].Flag == 42) continue;

                for (int j = 0; j < amtModel.Head.NumVertices; j++)
                    if ((i != j) && vertices[i] == vertices[j])
                    {
                        //desliga o vertice pq esta duplicado
                        AMT_VERTEX v = vertices[j];
                        v.Flag = 42;
                        vertices[j] = v;

                        currentVertices--;
                    }
            }

            // Copia os vertices para o array da estrutura
            for (int i = 0; i < amtModel.Head.NumVertices; i++)
            {
                // todos q nao sao 42
                if (vertices[i].Flag != 42)
                    amtModel.Vertices.Add(vertices[i]);
            }

            //percorre o array de indices para procurar os vertices q estao na mesma posicao, se estiverem entao ele deixa os indices iguais
            for (ulong i = 0; i < amtModel.Head.NumFaces; i++)
            {
                _import.ProgressText = "Importing Faces, recompute indices " + i.ToString() + "/" + amtModel.Head.NumFaces.ToString();

                AMT_FACE f = amtModel.Faces[(int)i];

                for (int n = 0; n < 3; n++)
                {
                    for (int j = 0; j < (int)currentVertices; j++)
                    {
                        if (AMT_VERTEX.EqualsWithoutFlagTest(vertices[f.Indices[n]], amtModel.Vertices[j]))
                        {
                            f.Indices[n] = j;
                        }
                    }
                }

                amtModel.Faces[(int)i] = f;
            }

            //calcula as normais das faces
            for (ulong i = 0; i < amtModel.Head.NumFaces; i++)
            {
                _import.ProgressText = "Importing Faces, recompute normals " + i.ToString() + "/" + amtModel.Head.NumFaces.ToString();

                Vector3 v1 = amtModel.Vertices[amtModel.Faces[(int)i].Indices[0]].Position;
                Vector3 v2 = amtModel.Vertices[amtModel.Faces[(int)i].Indices[1]].Position;
                Vector3 v3 = amtModel.Vertices[amtModel.Faces[(int)i].Indices[2]].Position;

                AMT_FACE face = amtModel.Faces[(int)i];
                face.Normal = Tools.GetNormal(v1, v2, v3);
                amtModel.Faces[(int)i] = face;
            }

            amtModel.Head.NumVertices = (uint)currentVertices;
        }

        #region Get Geometry Data
        private List<Vector3> GetPositions(XElement positionElement)
        {
            List<Vector3> positions = new List<Vector3>();

            XElement technique_common = positionElement.Element(XName.Get("technique_common", Namespace));
            XElement accessor = technique_common.Element(XName.Get("accessor", Namespace));
            int vector3Count = int.Parse(accessor.Attribute("count").Value);
            int stride = int.Parse(accessor.Attribute("stride").Value);
            List<XElement> param = accessor.Elements(XName.Get("param", Namespace)).ToList();
            int X = 0,Y = 1,Z = 2; //uso para pegar qual a ordem de armazenamento pode estar YZX por exemplo
            for (int i = 0; i < stride; i++)
            {
                if (param[i].Attribute("name").Value == "X")
                    X = i;
                else if (param[i].Attribute("name").Value == "Y")
                    Y = i;
                else
                    Z = i;
            }

            XElement float_array = positionElement.Element(XName.Get("float_array", Namespace));
            string strPositions = float_array.Value;
            int count = int.Parse(float_array.Attribute("count").Value); //pega o contado de valores pra testar depois q fizer o split
            string[] values = strPositions.Replace('\n',' ').Trim().Split(' ');

            //se for igual entao o split fez certo a budega
            if (values.Length == count)
            {
                //adiciona os valores na ordem certa XYZ
                for (int i = 0; i < values.Length; i+=stride)
			    {
                    positions.Add(new Vector3(float.Parse(values[i + X], CultureInfo.InvariantCulture), 
                                              float.Parse(values[i + Y], CultureInfo.InvariantCulture), 
                                              float.Parse(values[i + Z], CultureInfo.InvariantCulture)));			 
			    }
            }

            return positions;
        }

        private List<Vector3> GetNormals(XElement normalElement)
        {
            List<Vector3> normals = new List<Vector3>();

            XElement technique_common = normalElement.Element(XName.Get("technique_common", Namespace));
            XElement accessor = technique_common.Element(XName.Get("accessor", Namespace));
            int vector3Count = int.Parse(accessor.Attribute("count").Value);
            int stride = int.Parse(accessor.Attribute("stride").Value);
            List<XElement> param = accessor.Elements(XName.Get("param", Namespace)).ToList();
            int X = 0,Y = 1,Z = 2; //uso para pegar qual a ordem de armazenamento pode estar YZX por exemplo
            for (int i = 0; i < stride; i++)
            {
                if (param[i].Attribute("name").Value == "X")
                    X = i;
                else if (param[i].Attribute("name").Value == "Y")
                    Y = i;
                else
                    Z = i;
            }
                

            XElement float_array = normalElement.Element(XName.Get("float_array", Namespace));
            string strPositions = float_array.Value;
            int count = int.Parse(float_array.Attribute("count").Value); //pega o contado de valores pra testar depois q fizer o split
            string[] values = strPositions.Replace('\n',' ').Trim().Split(' ');

            //se for igual entao o split fez certo a budega
            if (values.Length == count)
            {
                //adiciona os valores na ordem certa XYZ
                for (int i = 0; i < values.Length; i+=stride)
			    {
                    normals.Add(new Vector3(float.Parse(values[i + X], CultureInfo.InvariantCulture), 
                                            float.Parse(values[i + Y], CultureInfo.InvariantCulture), 
                                            float.Parse(values[i + Z], CultureInfo.InvariantCulture)));			 
			    }
            }

            return normals;
        }

        private List<Vector2> GetTexCoords(XElement texcoordElement)
        {
            List<Vector2> texcoords = new List<Vector2>();

            XElement technique_common = texcoordElement.Element(XName.Get("technique_common", Namespace));
            XElement accessor = technique_common.Element(XName.Get("accessor", Namespace));
            int vector3Count = int.Parse(accessor.Attribute("count").Value);
            int stride = int.Parse(accessor.Attribute("stride").Value);
            List<XElement> param = accessor.Elements(XName.Get("param", Namespace)).ToList();
            int S = 0,T = 1; //uso para pegar qual a ordem de armazenamento pode estar YZX por exemplo
            for (int i = 0; i < stride; i++)
            {
                if (param[i].Attribute("name").Value == "S")
                    S = i;
                else if (param[i].Attribute("name").Value == "T")
                    T = i;
            }
                

            XElement float_array = texcoordElement.Element(XName.Get("float_array", Namespace));
            string strPositions = float_array.Value;
            int count = int.Parse(float_array.Attribute("count").Value); //pega o contado de valores pra testar depois q fizer o split
            string[] values = strPositions.Replace('\n',' ').Trim().Split(' ');

            //se for igual entao o split fez certo a budega
            if (values.Length == count)
            {
                //adiciona os valores na ordem certa XYZ
                for (int i = 0; i < values.Length; i += stride)
			    {
                    texcoords.Add(new Vector2(float.Parse(values[i + S], CultureInfo.InvariantCulture), 
                                              float.Parse(values[i + T], CultureInfo.InvariantCulture)));			 
			    }
            }

            return texcoords;
        }
        #endregion

    }
}
