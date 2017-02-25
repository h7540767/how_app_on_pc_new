#include <time.h>
#include "wrap.h"

#define MAXDELAY  5000
#define MINDELAY  1
#define MAXTYPE   50
#define MINTYPE   1
#define MAXMEMORY 1024
#define MINMEMORY 1



set<int> choosetype(int typenum)
{
	set<int> pool;
	if (typenum > MAXTYPE || typenum < MINTYPE)
	{
		cout << "choosetype usage: MINTYPE <= typenum <= MAXTYPE\n";
		exit(-1);
	}
	for (int i = MINTYPE; i <= MAXTYPE; i++)
	{
		pool.insert(i);
	}

	while (pool.size() != typenum)
	{
		pool.erase(myrand(MINTYPE, MAXTYPE));
	}

	return pool;
}


int main(int argc, char *argv[])
{
	int c;
	int appnum, pcnum;
	int fd;
	
	#ifdef debug
	cout << "debug version" << endl;
	#endif
	
	while ( (c = Getopt(argc, argv, "a:p:")) != -1) 
	{
		switch (c) 
		{
			case 'a':
				appnum = atoi(optarg);
				break;

			case 'p':
				pcnum = atoi(optarg);
				break;
		}
	}

	if (5 != argc)
	{
		printf("usage: casefactory < -a #appnum > < -p #pcnum >\n");
		exit(-1);
	}
	
	srand((unsigned int)time(NULL));	
	
	fd = Open("pcinfor.csv", O_CREAT | O_EXCL | O_RDWR, 0666);
	
	char buf[200];
	char appnumstr[20], pcnumstr[20], str[20];
	
	snprintf(buf, sizeof(buf), "%s  %s\n", 
		itoa(appnum, appnumstr, 10), itoa(pcnum, pcnumstr, 10));
	
	Write(fd, buf, strlen(buf));
	
	for (int i = 0; i < pcnum; i++)
	{
		char buf[200];
		char idstr[20], typestr[20], memorystr[20];
		int id = i;
		int type = myrand(1, 50);
		int memory = myrand(1, 1024);

		snprintf(buf, sizeof(buf), "%-10s%-10s%-5s\n", 
			itoa(id, idstr, 10), itoa(type, typestr, 10), itoa(memory, memorystr, 10));
			
		Write(fd, buf, strlen(buf));
	}
	
	Close(fd);
	
	fd = Open("appinfor.csv", O_CREAT | O_EXCL | O_RDWR, 0666);

	for (int i = 0; i < appnum; i++)
	{
		char buf[200];
		char idstr[20], typenumstr[20], memorystr[20], str[20];
		int id = i;
		int typenum = myrand(MINTYPE, MAXTYPE);
		int memory = myrand(MINMEMORY, MAXMEMORY);
		int type;
		
		snprintf(buf, sizeof(buf), "%-10s%-10s%-5s\n", 
			itoa(id, idstr, 10), itoa(typenum, typenumstr, 10), itoa(memory, memorystr, 10));
		
		Write(fd, buf, strlen(buf));

		set<int> choosen = choosetype(typenum);

		for (auto item : choosen)
		{
			snprintf(buf, sizeof(buf), "%s ", itoa(item, str, 10));
			Write(fd, buf, strlen(buf));
		}
		
		snprintf(buf, sizeof(buf), "\n");
		Write(fd, buf, strlen(buf));
	}

	Close(fd);

	fd = Open("pcdelayinfor.csv", O_CREAT | O_EXCL | O_RDWR, 0666);

	for (int i = 0; i < pcnum; i++)
	{
		for (int j = i; j < pcnum; j++)
		{
			char buf[200];
			char id0str[20], id1str[20], delaystr[20];
			int id0 = i;
			int id1 = j;
			int delay = myrand(1, 5000);
			
			if (id0 == id1)
			{
				delay = 0;
			}
			
			snprintf(buf, sizeof(buf), "%-10s%-10s%-5s\n", 
				itoa(id0, id0str, 10), itoa(id1, id1str, 10), itoa(delay, delaystr, 10));
				
			Write(fd, buf, strlen(buf));
		}
	}

	Close(fd);

	fd = Open("appdelayinfor.csv", O_CREAT | O_EXCL | O_RDWR, 0666);

	for (int i = 0; i < appnum - 2; i++)
	{
		char buf[200];
		char str[20];

		int delay = myrand(MINDELAY, MAXDELAY);
		snprintf(buf, sizeof(buf), "%s ", itoa(delay, str, 10));
		Write(fd, buf, strlen(buf));
	}

	snprintf(buf, sizeof(buf), "%s\n", itoa(myrand(MINDELAY, MAXDELAY);, str, 10));
	Write(fd, buf, strlen(buf));

	return 0;
}