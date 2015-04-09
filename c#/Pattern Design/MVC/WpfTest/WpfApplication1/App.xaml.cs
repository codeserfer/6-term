using System;
using System.Collections.Generic;
using System.Windows;
using WpfApplication1.Models;
using WpfApplication1.ViewModels;
using WpfApplication1.Views;

namespace WpfApplication1
{
	/// <summary>
	/// Interaction logic for App.xaml
	/// </summary>
	public partial class App : Application
	{
		protected override void OnStartup(StartupEventArgs e)
		{
			List<Person> persons = new List<Person>()
			{
				new Person(){Name = "Вася", Age = 33},
				new Person(){Name = "Петя", Age = 27},
				new Person(){Name = "Коля", Age = 30},
			};
			PersonListView view = new PersonListView();
			PersonListViewModel viewModel = new PersonListViewModel(persons);
			view.DataContext = viewModel;
			view.Show();
		}
	}
}
