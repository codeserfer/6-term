//Делает Builder
//И делаем Director
using System;

namespace Application {

	public interface IHouseBuilder
	{
		void BuildFloor ();
		void BuildRoof ();
		void BuildWalls ();
		void BuildCeiling ();
		string GetHouse ();
	}


	public class PrettyHouseBuilder : IHouseBuilder
	{
		private string _house="";
		public void BuildFloor ()
		{
			_house="൳൳൳\n"+_house;
		}
		public void BuildRoof () 
		{
			_house="ᚚᚚᚚ\n"+_house;
		}
		public void BuildWalls ()
		{
			_house="ᚌ  ᚌ\n"+_house;
		}
		public void BuildCeiling ()
		{
			_house="ᚅᚅ\n"+_house;
		}
		public string GetHouse () 
		{
			return _house;
		}
	}


	public interface IHouseDirector
	{
		IHouseBuilder house {get; set; }
		void Construct ();
	}

	public class OneFloorHouseDirector : IHouseDirector
	{
		public IHouseBuilder house { get; set; }

		public void Construct ()
		{
			if (house==null)
				throw new NullReferenceException ("You need to set HouseBuilder property first!");

			house.BuildFloor ();
			house.BuildWalls ();
			house.BuildCeiling ();
			house.BuildRoof ();

		}
	}

	public class ThreeFloorHouseDirector : IHouseDirector
	{
		public IHouseBuilder house { get; set; }

		public void Construct ()
		{
			if (house==null)
				throw new NullReferenceException ("You need to set HouseBuilder property first!");
			for (int i = 0; i < 3; i++) {
				house.BuildFloor ();
				house.BuildWalls ();
				house.BuildCeiling ();	
			}

			house.BuildRoof ();

		}
	}



	public class HouseBuilder : IHouseBuilder
	{
		private string _house="";
		public void BuildFloor ()
		{
			_house="===\n"+_house;
		}
		public void BuildRoof () 
		{
			_house="/ \\\n"+_house;
		}
		public void BuildWalls ()
		{
			_house="|  |\n"+_house;
		}
		public void BuildCeiling ()
		{
			_house="---\n"+_house;
		}
		public string GetHouse () 
		{
			return _house;
		}

	}





	class MainClass {

		private static IHouseBuilder GetHouseBuilder ()
		{
			//return new HouseBuilder ();
			return new PrettyHouseBuilder ();
		}

		private static IHouseDirector GetHouseDirector ()
		{
			return new OneFloorHouseDirector ();
			//return new ThreeFloorHouseDirector ();
		}

		public static void Main (string [] args) {
			var houseBuilder=GetHouseBuilder ();
			var houseDirector=GetHouseDirector ();
			houseDirector.house=houseBuilder;
			houseDirector.Construct ();

			Console.WriteLine (houseBuilder.GetHouse());
		}
	}
}

