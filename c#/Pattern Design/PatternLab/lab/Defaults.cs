using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab
{
    public enum UnitType
    {
        heavy,
        light
    }
   

    public static class Defaults
    {
        public delegate Unit FactoryMethod();

        public static class Healer
        {
            public static readonly UnitType type = UnitType.light;
            public static readonly int health = 100;
            public static readonly Int32 strenght = 30;
            public static readonly Int32 defense = 50;
            public static readonly Int32 cost = 1000;
            public static readonly bool treatable = true;
            public static readonly bool clonable = false;
            public static readonly FactoryMethod factoryMethod = UnitFactory.Instance.CreateHealer;
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
            public static readonly FactoryMethod factoryMethod = UnitFactory.Instance.CreateInfantryman;
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
            public static readonly FactoryMethod factoryMethod = UnitFactory.Instance.CreateArcher;
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
            public static readonly FactoryMethod factoryMethod = UnitFactory.Instance.CreateWizard;
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
            public static readonly FactoryMethod factoryMethod = UnitFactory.Instance.CreateKnight;
        }

        public static class Barrier
        {
            public static readonly UnitType type = UnitType.heavy;
            public static readonly int health = 100;
            public static readonly Int32 strenght = 0;
            public static readonly Int32 defense = 500;
            public static readonly Int32 cost = 2000;
            public static readonly bool treatable = false;
            public static readonly bool clonable = false;
            public static readonly FactoryMethod factoryMethod = UnitFactory.Instance.CreateBarrier;
        }
    }
}
