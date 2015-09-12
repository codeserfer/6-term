using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab
{
    
    public sealed class UnitFactory
    {
        private Int32 id = 0;

        private static readonly Lazy<UnitFactory> instance = new Lazy<UnitFactory>(() => new UnitFactory());

        private UnitFactory() {}

        public static UnitFactory Instance { get { return UnitFactory.instance.Value; } }

        public Unit CreateHealer()
        {
            //Console.WriteLine("CreateHealer");
            return new Healer(id++);
        }

        public Unit CreateInfantryman()
        {
            //Console.WriteLine("CreateInfantryman");
            return new Infantryman(id++);
        }

        public Unit CreateArcher()
        {
            //Console.WriteLine("CreateArcher");
            return new Archer(id++);
        }

        public Unit CreateWizard()
        {
            //Console.WriteLine("CreateWizard");
            return new Wizard(id++);
        }

        public Unit CreateKnight()
        {
            //Console.WriteLine("CreateKnight");
            return new Knight(id++);
        }

        public Unit CreateBarrier()
        {
            return new BarrierAdapter(id++);
        }
    }
}
