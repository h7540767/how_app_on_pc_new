#include "wrap.h"
#include "ac.h"
#include <time.h>
#include <signal.h>

using namespace std;

AC ac;
void handler(int signo)
{
	showplans(ac.plans);
	exit(-1);
	return;
}

int main()
{
	signal(SIGINT, handler);
	srand((unsigned int)time(NULL));
	
	ac.loadinfor();
	//ac.showinfor();
	ac.ga();
	ac.showret();
	return 0;
}