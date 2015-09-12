using System;
using System.Collections.Generic;
using System.Reflection;


namespace lab
{

    /*public interface IArmyGeneratorFacade
    {
        List<List<Unit>> GenarateArmyList(Int32 count, Int32 cost);
    }

    public class ArmyGenerate : IArmyGeneratorFacade
    {
        public List<List<Unit>> GenarateArmyList(Int32 count, Int32 cost)
        {
            var temp = new List<List<Unit>>();

            for (int i = 0; i < count; i++)
            {
                temp.Add(ArmyGenerator.GenerateArmy(cost));
            }

            return temp;
        }
    }*/


    public class MyRandom
    {
        private readonly Random rand = new Random();

        public Double Generate(Double max)
        {
            return rand.NextDouble() * max;
        }
    }

    public class ArmyGenerator
    {
        private Int32 index;
        List<Unit> army;
        private Int32 maxCost;

        public void Reset()
        {
            index = 0;
            army = GenerateArmy(maxCost);
        }

        public ArmyGenerator(Int32 cost)
        {
            maxCost = cost;
            army = GenerateArmy(cost);
        }

        public Unit CreateUnit()
        {
            return army[index++];
        }

        private struct FactoryReverseCost
        {
            public Defaults.FactoryMethod factory;
            public Int32 cost;
        }

        private static FactoryReverseCost CostToFactory(Double random, List<ReverseCostsFactory> costFactories)
        {
            foreach (var i in costFactories)
            {
                if (i.startReverseCost <= random && i.finishReverseCost >= random)
                    return new FactoryReverseCost() {factory = i.factory, cost = i.cost};
            }

            return new FactoryReverseCost() {factory = costFactories[0].factory, cost = costFactories[0].cost};
        }

        private struct ReverseCostsFactory
        {
            public Int32 cost;
            public Double startReverseCost;
            public Double finishReverseCost;
            public Defaults.FactoryMethod factory;
        }

        private static List<Unit> GenerateArmy(Int32 cost)
        {
            var costFactories = new List<ReverseCostsFactory>();
            var randomGenerator = new MyRandom();
            Int32 minCost = Int32.MaxValue;
            Double sumReverseCost = 0;
            Double prevReverseCost = 0;

            foreach (var i in Assembly.GetExecutingAssembly().GetType("lab.Defaults", true, true).GetMembers())
            {
                if (i.GetType().ToString() == "System.RuntimeType")
                {

                    Type type = Assembly.GetExecutingAssembly().GetType(i.ToString());

                    if (type.ToString() != "lab.Defaults+FactoryMethod")
                    {
                        var unitCost = (Int32)type.GetField("cost").GetValue(null);
                        var unitFactory = (Defaults.FactoryMethod)type.GetField("factoryMethod").GetValue(null);
                        costFactories.Add(new ReverseCostsFactory
                        {
                            startReverseCost = prevReverseCost, 
                            finishReverseCost = (Double)1/unitCost + prevReverseCost, 
                            factory = unitFactory,
                            cost = unitCost
                        });

                        prevReverseCost = (Double)1 / unitCost + prevReverseCost;

                        sumReverseCost += (Double)1/unitCost;

                        if (minCost > unitCost)
                            minCost = unitCost;
                    }
                }
            }

            var unitList = new List<Unit>();

            while (cost >= minCost)
            {
                var random = randomGenerator.Generate(sumReverseCost);
                var factoryCost = CostToFactory(random, costFactories);

                if (factoryCost.cost <= cost)
                {
                    cost -= factoryCost.cost;
                    unitList.Add(factoryCost.factory.Invoke());
                }
            }


            return unitList;
        }
    }
}
