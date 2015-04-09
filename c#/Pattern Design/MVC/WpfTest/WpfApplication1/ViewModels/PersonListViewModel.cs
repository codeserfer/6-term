using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using WpfApplication1.Models;

namespace WpfApplication1.ViewModels
{
	class PersonListViewModel : ViewModelBase
	{
		public ObservableCollection<PersonViewModel> PersonList { get; set; }

		public PersonListViewModel(List<Person> persons)
		{
			PersonList = new ObservableCollection<PersonViewModel>
				(persons.Select(r => new PersonViewModel(r)));
		}
	}
}
