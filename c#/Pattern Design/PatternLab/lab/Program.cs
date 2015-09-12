using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace lab
{

    static class Program
    {
        private static void Main(string[] args)
        {


            var armyGenarate = new ArmyGenerate();
            var army = armyGenarate.GenarateArmyList(2, 10000);

            foreach (var i in army)
            {
                Console.WriteLine("\n\n");
                int cost = 0;
                foreach (var j in i)
                {
                    Console.WriteLine(j);
                    cost += j.cost;
                }
                Console.WriteLine("Cost: {0}", cost);
            }


            army[0][0].Melee(army[1][0].strenght);

            if (army[0][0].AreDeath())
            {
                Console.WriteLine("Killed!");
                army[0].RemoveAt(0);
            }
            else
            {
                Console.WriteLine("Not killed");
                Console.WriteLine(army[0][0].health);
            }
        }

    }
}
