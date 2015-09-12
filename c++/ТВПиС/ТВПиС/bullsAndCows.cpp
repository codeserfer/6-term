int N = rand(9999);
N = N + 1000;
int oldN = N;

int arrayN[4];
int i = 3;
	
while (i >= 0)
{
	arrayN[i] = N % 10;
	N = (N - arrayN[i]) / 10;
	i--;
}

while ((arrayN[0] == arrayN[1]) || (arrayN[0] == arrayN[2]) || (arrayN[0] == arrayN[3]) || (arrayN[1] == arrayN[2]) || (arrayN[1] == arrayN[3]) || (arrayN[2] == arrayN[3]))
{
	N = ++oldN;

	i = 3;
	while (i >= 0)
	{
		arrayN[i] = N % 10;
		N = (N - arrayN[i]) / 10;
		i--;
	}
}


bool win = 0;
int game = 0;
while (game < 20)
{
	output("Enter four-digit number: ");
	int t;
	input(t);
	if (t >= 10000 || t < 1000)
	{
		output("You too stipid to play!");
	}
	else
	{
		int cows = 0;
		int bulls = 0;
		int temp;
		i = 3;
		while (i >= 0)
		{
			temp = t % 10;
			if (temp == arrayN[i])
			{
				bulls++;
			}
			else
			{
				if (temp == arrayN[0] || temp == arrayN[1] || temp == arrayN[2] || temp == arrayN[3])
				{
					cows++;
				}
			}
			t = (t - temp) / 10;
			i = i - 1;
		}
		if (bulls == 4)(
		{
			win = 1;
			game = 20;
		}
		else
		{
			output(" You have ");
			output(cows);
			output(" cows and ");
			output(bulls);
			output (" bulls\n");
			game++;
		}
	}
}
if (win)
{
	output("You win! =)");
}
else
{
	output("You loose. =(");
}
