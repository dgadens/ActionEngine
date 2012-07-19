using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace ACFramework.FileStruct
{
    public class Import
    {
        private string _progressText;
        public string ProgressText
        {
            set
            {
                if (_progressText != value)
                {
                    _progressText = value;
                    NotifyPropertyChanged(_progressText);
                }
            }

            get
            {
                return _progressText;
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;
        private void NotifyPropertyChanged(String info)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(info));
            }
        }
    }
}
