using System;
using System.Collections;
using System.Collections.Generic;

namespace croc2 {
	class MainClass {
		public static void Main (string [] args) {

			while (true) {
				
			

				string s=Console.ReadLine ();

				bool error=false;
				var stack=new Stack<char> ();
				for (int i = 0; i < s.Length && !error; i++) {
					if (s [i]=='(' || s [i]=='[')
						stack.Push (s [i]);
					if (s [i]==')') {

						if (stack.Count<=0)
							error=true;

						if (stack.Count>0 && stack.Pop ()!='(')
							error=true;
					}

					if (s [i]==']') {

						if (stack.Count<=0)
							error=true;

						if (stack.Count>0 && stack.Pop ()!='[')
							error=true;
					}

				}

				if (stack.Count>0)
					error=true;

				if (!error)
					Console.WriteLine ("All right!");
				else
					Console.WriteLine ("Error!");
			}
		}
	}
}
