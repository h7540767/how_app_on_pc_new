#include <bits/stdc++.h>
#include "wrap.h"

const char * usage = "usage: transfor <h2s / s2h>";

FILE *pcinforfp, *appinforfp, *pcdelayinforfp, *appdelayinforfp;
FILE *inforfp;

void s2h()
{
	pcinforfp       = Fopen("pcinfor.csv", "r");
	appinforfp      = Fopen("appinfor.csv", "r");
	pcdelayinforfp  = Fopen("pcdelayinfor.csv", "r");
	appdelayinforfp = Fopen("appdelayinfor.csv", "r");
	


	
	return ;
}

void h2s()
{
	return ;
}


int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		cout << usage << endl;
		exit(-1);
	}

	if (argv[1] == "h2s")
	{
		h2s();
		return 0;
	}

	if (argv[1] == "s2h")
	{
		s2h();
		return 0;
	}

	cout << usage << endl;
	
	return 0;
	
}
