namespace WpfApplication1.Models
{
	class Person : ModelBase
	{
		private string _name;
		public string Name
		{
			get { return _name; }
			set { _name = value; 
				OnPropertyChanged("Name");}
		}

		private int _age;
		public int Age
		{
			get { return _age; }
			set
			{
				_age = value; ;
				OnPropertyChanged("Age");
			}
		}
	}
}
