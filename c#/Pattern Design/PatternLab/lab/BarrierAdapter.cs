using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpecialUnits;

namespace lab
{
    public class BarrierAdapter : Unit
    {
        private readonly GulyayGorod gulyayGorod;


        public BarrierAdapter(Int32 id) : base(
            Defaults.Barrier.type,
            id,
            Defaults.Barrier.health,
            Defaults.Barrier.strenght,
            Defaults.Barrier.defense,
            Defaults.Barrier.cost, 
            Defaults.Barrier.treatable, 
            Defaults.Barrier.clonable)
        {
            gulyayGorod = new GulyayGorod(health, Defaults.Barrier.defense, Defaults.Barrier.cost);

        }

        public new Boolean AreDeath()
        {
            return !gulyayGorod.AreDeath;
        }

        public new Int32 cost
        {
            get { return gulyayGorod.GetCost(); }
            private set { }
        }

        public new Int32 defence
        {
            get { return gulyayGorod.GetDefence(); }
            private set { }
        }

        public new Int32 strenght
        {
            get { return gulyayGorod.GetStrength(); }
            private set { }
        }


    }
}
