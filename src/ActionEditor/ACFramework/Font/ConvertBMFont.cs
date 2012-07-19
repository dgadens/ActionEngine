using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using System.IO;

namespace ACFramework.Font
{
    public class ConvertBMFont
    {
        string Namespace = null;

        public string Convert(string pathSrc, string pathDest)
        {
            XDocument doc = XDocument.Load(pathSrc);
            if (doc != null)
            {
                Namespace = doc.Root.Name.Namespace.NamespaceName;

                XElement font = doc.Element(XName.Get("font", Namespace));
                XElement info = font.Element(XName.Get("info", Namespace));

                string fontName = info.Attribute("face").Value;
                int size = int.Parse(info.Attribute("size").Value);
                int bold = int.Parse(info.Attribute("bold").Value);
                if (bold!=0) fontName += "Bold";
                int italic = int.Parse(info.Attribute("italic").Value);
                if (italic != 0) fontName += "Italic";
                string charType = info.Attribute("charset").Value;

                XElement common = font.Element(XName.Get("common", Namespace));
                int width = int.Parse(common.Attribute("scaleW").Value);
                int height = int.Parse(common.Attribute("scaleH").Value);

                //pega os caracteres
                XElement chars = font.Element(XName.Get("chars", Namespace));

                List<Character> characteres = new List<Character>();
                foreach (XElement charac in chars.Elements())
                {
                    Character c = new Character();
                    c.Id = int.Parse(charac.Attribute("id").Value);
                    c.X = int.Parse(charac.Attribute("x").Value);
                    c.Y = int.Parse(charac.Attribute("y").Value);
                    c.Width = int.Parse(charac.Attribute("width").Value);
                    c.Height = int.Parse(charac.Attribute("height").Value);
                    c.XOffset = int.Parse(charac.Attribute("xoffset").Value);
                    c.YOffset = int.Parse(charac.Attribute("yoffset").Value);
                    c.XAdvance = int.Parse(charac.Attribute("xadvance").Value);
                    characteres.Add(c);
                }

                //Começa o export
                if (charType == "ANSI")
                {
                    int VERSION = 1;
                    using (BinaryWriter bw = new BinaryWriter(File.Open(pathDest, FileMode.Create)))
                    {
                        bw.Write(VERSION);

                        char[] name = Tools.GetCharArray(fontName, 64);
                        bw.Write(name);

                        bw.Write(size);
                        bw.Write(width);
                        bw.Write(height);

                        bw.Write(characteres.Count);
                        foreach (var item in characteres)
                        {
                            bw.Write(item.Id);
                            bw.Write(item.X);
                            bw.Write(item.Y);
                            bw.Write(item.Width);
                            bw.Write(item.Height);
                            bw.Write(item.XOffset);
                            bw.Write(item.YOffset);
                            bw.Write(item.XAdvance);
                        }

                        bw.Close();
                    }

                    //copia o png tb
                    File.Copy(pathSrc.Replace(".fnt", "_0.png"), pathDest.Replace(".aft", ".png"), true);

                    return "Font created with success";
                }
                else
                    return "Character Type not supported";
            }

            return "Error";
        }
    }
}
