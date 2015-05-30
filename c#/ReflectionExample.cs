using System;
using System.Reflection;


public static class Test
{
    public static Int32 q = 5;
}

public enum UnitType
{
    heavy,
    light
}

public static class Defaults
{
    public static class Healer
    {
        public static readonly UnitType type = UnitType.light;
        public static readonly int health = 100;
        public static readonly Int32 strenght = 30;
        public static readonly Int32 defense = 50;
        public static readonly Int32 cost = 1000;
        public static readonly bool treatable = true;
        public static readonly bool clonable = false;
    }

    public static class Infantryman
    {
        public static readonly UnitType type = UnitType.light;
        public static readonly int health = 100;
        public static readonly Int32 strenght = 100;
        public static readonly Int32 defense = 80;
        public static readonly Int32 cost = 100;
        public static readonly bool treatable = true;
        public static readonly bool clonable = true;
    }

    public static class Archer
    {
        public static readonly UnitType type = UnitType.light;
        public static readonly int health = 100;
        public static readonly Int32 strenght = 80;
        public static readonly Int32 defense = 70;
        public static readonly Int32 cost = 100;
        public static readonly bool treatable = true;
        public static readonly bool clonable = true;
    }

    public static class Wizard
    {
        public static readonly UnitType type = UnitType.light;
        public static readonly int health = 100;
        public static readonly Int32 strenght = 30;
        public static readonly Int32 defense = 20;
        public static readonly Int32 cost = 1000;
        public static readonly bool treatable = false;
        public static readonly bool clonable = false;
    }

    public static class Knight
    {
        public static readonly UnitType type = UnitType.heavy;
        public static readonly int health = 100;
        public static readonly Int32 strenght = 200;
        public static readonly Int32 defense = 150;
        public static readonly Int32 cost = 500;
        public static readonly bool treatable = true;
        public static readonly bool clonable = true;
    }

    public static class GulyayGorod
    {
        public static readonly UnitType type = UnitType.heavy;
        public static readonly int health = 100;
        public static readonly Int32 strenght = 0;
        public static readonly Int32 defense = 0;
        public static readonly Int32 cost = 5000;
        public static readonly bool treatable = false;
        public static readonly bool clonable = false;

    }
}


public class Program
{
    public static void Main()
    {

        foreach (var i in Assembly.GetExecutingAssembly().GetType("Defaults", true, true).GetMembers())
        {
            if (i.GetType().ToString() == "System.RuntimeType")
            {
             
                Type type = Assembly.GetExecutingAssembly().GetType(i.ToString());
                //FieldInfo[] fields = type.GetFields();

                var test = type.GetField("cost");
                Console.WriteLine("!{0}", test.GetValue(null));

                /*foreach (var field in fields) // Loop through fields
                {
                    string name = field.Name; // Get string name
                    object temp = field.GetValue(null); // Get value

                    if (temp is int) // See if it is an integer.
                    {
                        int value = (int) temp;
                        Console.Write(name);
                        Console.Write(" (int) = ");
                        Console.WriteLine(value);
                    }
                    else if (temp is string) // See if it is a string.
                    {
                        string value = temp as string;
                        Console.Write(name);
                        Console.Write(" (string) = ");
                        Console.WriteLine(value);
                    }
                }*/

            }
        }

    }
}
