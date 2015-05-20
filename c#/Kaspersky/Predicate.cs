using System;
using System.Collections.Generic;

namespace Predicate {
	public delegate bool UserPredicate (int elem);


	public static class Collection
	{
		private static List<int> collection=new List<int> ();

		public static void Add (int element) 
		{
			collection.Add (element);
		}

		public static int? Get (int n) 
		{
			if (n < collection.Count)
				return collection [n];

			return null;
		}

		public static int PredicateCounter (UserPredicate predicate) 
		{
			int count = 0;

			foreach (var item in collection) {
				if (predicate (item))
					count++;
			}

			return count;
		}
	}


	class MainClass 
	{

		public static bool MyPredicate (int elem) 
		{
			if (elem>45)
				return true;

			return false;
		}

		public static void Main (string [] args) 
		{
			for (int i = 0; i < 100; i++) {
				Collection.Add (i);
			}

			Console.WriteLine (Collection.PredicateCounter(MyPredicate));
		
		}
	}
}

