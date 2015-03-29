using System;
using System.Collections;
using System.Collections.Generic;

namespace croc1 {
	class MainClass {
		public static void Main (string [] args) {
			var list=new List<int> ();
			var r=new Random ();
			for (int i = 0; i < 100; i++) {
				list.Add (r.Next (15));
			}

			list.Sort ();


			var l1=new List<int> ();
			var l2=new List<int> ();


			int item=list [0];
			int count=1;
			for (int i = 1; i < list.Count; i++) {
				if (item==list [i])
					count++;
				else 
				{
					l1.Add (item);
					l2.Add (count);
					item=list [i];
					count=0;
				}

			}

			for (int i = 0; i < l1.Count-1; i++) {
				for (int j = 0; j < l1.Count-i-1; j++) {
					if (l2 [j]<l2 [j+1]) 
					{
						var temp=l2 [j];
						l2 [j]=l2 [j+1];
						l2 [j+1]=temp;

						temp=l1 [j];
						l1 [j]=l1 [j+1];
						l1 [j+1]=temp;
					}
				}
			}




			for (int i = 0; i < l1.Count; i++) {
				Console.WriteLine ("{0} - {1}", l1[i], l2[i]);
			}
		}
	}
}
