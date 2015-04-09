using System.ComponentModel;
using WpfApplication1.Models;

namespace WpfApplication1.ViewModels
{
	class PersonViewModel : ModelBase
	{
		public Person Person { get; set; }

		public string Name
		{
			get { return Person.Name; }
			set { Person.Name = value; }
		}

		public int Age
		{
			get { return Person.Age; }
			set { Person.Age = value; }
		}

		public PersonViewModel(Person person)
		{
			Person = person;
			Person.PropertyChanged += personPropertyChanged;
		}

		private void personPropertyChanged(object sender, PropertyChangedEventArgs e)
		{
			OnPropertyChanged(e.PropertyName);
		}
	}
}
