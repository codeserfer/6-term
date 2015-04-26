using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Runtime.Remoting.Messaging;
using System.Text;
using System.Threading.Tasks;

namespace _25._04._2015
{
    public static class FileStore
    {
        public static void StoreToFile(string fileName, string text)
        {
            System.IO.File.WriteAllText(fileName, text);
        }
    }

    

    public class BoldText : BaseText
    {
        public override string ToString()
        {
            return "<b>" + base.ToString() + "</b>";
        }
    }

    public class BoldItalicText : BaseText
    {
        public override string ToString()
        {
            return "<b><i>" + base.ToString() + "</i></b>";
        }
    }

    public class ItalicText : BaseText
    {
        public override string ToString()
        {
            return "<b>" + base.ToString() + "</b>";
        }
    }

    public class InverseItalicText : BaseText
    {
        public override string ToString()
        {
            return "<i><span style=\"color:#000000; background-color: #FFFFFF;\">" + base.ToString() + "</span></i>";
        }
    }

    public class InverseBoldText : BaseText
    {
        public override string ToString()
        {
            return "<b><span style=\"color:#000000; background-color: #FFFFFF;\">" + base.ToString() + "</span></b>";
        }
    }

    public class InverseText : BaseText
    {
        public override string ToString()
        {
            return "<span style=\"color:#000000; background-color: #FFFFFF;\">" + base.ToString() + "</span>";
        }
    }

    

    public class BigText : BaseText
    {
        public override string ToString()
        {
            return "<span style=\"font-size: 2em;\">" + base.ToString() + "</span>";
        }
    }

    public class BigInverseText : BaseText
    {
        public override string ToString()
        {
            return "<span style=\"color:#000000; background-color: #FFFFFF;\"><span style=\"font-size: +9;\">" + base.ToString() + "</span></span>";
        }
    }

    public class BigInverseItalicText : BaseText
    {
        public override string ToString()
        {
            return "<i><span style=\"color:#000000; background-color: #FFFFFF;\"><span style=\"font-size: +9;\">" + base.ToString() + "</span></span></i>";
        }
    }

    public class BigItalicText : BaseText
    {
        public override string ToString()
        {
            return "<i><span style=\"font-size: 2em;\">" + base.ToString() + "</span></i>";
        }
    }

    public class BigBoldText : BaseText
    {
        public override string ToString()
        {
            return "<b><span style=\"font-size: 2em;\">" + base.ToString() + "</span></b>";
        }
    }

    public class BigBoldItalicText : BaseText
    {
        public override string ToString()
        {
            return "<i><b><span style=\"font-size: 2em;\">" + base.ToString() + "</span></b></i>";
        }
    }

    //BaseText + 4 декоратора

    public class BaseText
    {
        public override string ToString()
        {
            return "Hello, world!";
        }
    }

    public class BoldTextDecorator : BaseText
    {
        private readonly BaseText text;

        public BoldTextDecorator(BaseText baseText)
        {
            text = baseText;
        }

        public override string ToString()
        {
            return "<b>" + text.ToString() + "</b>";
        }
    }

    public class ItalicTextDecorator : BaseText
    {
        private readonly BaseText text;

        public ItalicTextDecorator(BaseText baseText)
        {
            text = baseText;
        }

        public override string ToString()
        {
            return "<i>" + text.ToString() + "</i>";
        }
    }

    public class BigTextDecorator : BaseText
    {
        private readonly BaseText text;

        public BigTextDecorator(BaseText baseText)
        {
            text = baseText;
        }

        public override string ToString()
        {
            return "<span style=\"font-size: 2em;\">" + text.ToString() + "</span>";
        }
    }



    class Program
    {
        static void Main(string[] args)
        {
            var bbi = new BoldTextDecorator(new BigTextDecorator(new BigTextDecorator(new ItalicTextDecorator(new BaseText()))));
            
            FileStore.StoreToFile("test.html", bbi.ToString());

        }
    }
}

