#include "example_demos.h"

#include <iostream>
#include "tools.h"
#include "Graphics.h"

void green_matrix_demo()
{
	graphics::Graphics graph;
	const int amount = 60;
	int rain[amount] = { 0 };
	int fade[amount] = { 0 };
	for (int i = 0; i < amount; i++)
	{
		rain[i] = rand() % graph.getBufferSize().Y;
	}

	bool invert = true;

	while (true)
	{
		Sleep(10);

		CHAR_INFO* buffer = graph.getBuffer();
		int MX = graph.getBufferSize().X, MY = graph.getBufferSize().Y;
		
		// clear the buffer
		for (int i = 0; i < MX; i++)
		{
			for (int j = 0; j < MY; j++)
			{
				buffer[i * MY + j].Char.UnicodeChar = CHAR(' ');
				buffer[i * MY + j].Attributes = 0;
			}
		}
		// from   U+00C0  DEC: 192

		// to     U+02AF  DEC: 687

		//update the fading effect
		for (int i = 0; i < amount; i++)
		{
			fade[i]++;
			if (fade[i] >= 60)
			{
				fade[i] = rand() % 50;
				rain[i] = rand() % MY;
			}
		}

		//invert colors
		int c = FOREGROUND_GREEN;
		int k = FOREGROUND_INTENSITY;

		if (invert)
		{
			c = BACKGROUND_GREEN;
			k = BACKGROUND_INTENSITY;
		}
		invert = !invert;

		for (int i = 0; i < amount; i++)
		{
			for (int j = fade[i] / 2; j < fade[i]; j++)
			{
				buffer[(j + fade[i]) * MY + rain[i]].Char.UnicodeChar = CHAR((rand() % 495) + 192);
				buffer[(j + fade[i]) * MY + rain[i]].Attributes = (j > 20 ? (j > 40 ? c ^ k : (j % 3 == 0 ? c ^ k : c)) : ((j % (rand() % 3 + 1)) == 0 ? c : 0));
			}
		}


		graph.render();
	}
}
