﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ACFramework.FileStructs;

namespace ACFramework.FileStruct
{
    public interface IConverter
    {
        AMT_MODEL? Convert(string path, Import import);
    }
}
