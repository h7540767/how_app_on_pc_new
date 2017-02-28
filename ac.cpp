#include <math.h>
#include "wrap.h"
#include "ac.h"

using namespace std;

Plan Plan::operator + (Plan son)
{
	int start = myrand(0, son.dna.size() - 1);
	int length = myrand(1, son.dna.size() - start);
	//cout << "start " << start << " length " << length << endl;
	//cout << "father dna piece " << son.dna.substr(start, length) << endl;
	//cout << "mother dna piece " << this->dna.substr(start, length) << endl;
	for (int i = start; i < start + length; i++)
	{
		son.dna[i] = dna[i];
	}

	return son;
}

AC::AC()
{
	pcinforfp       = Fopen("pcinfor.csv", "r");
	appinforfp      = Fopen("appinfor.csv", "r");
	pcdelayinforfp  = Fopen("pcdelayinfor.csv", "r");
	appdelayinforfp = Fopen("appdelayinfor.csv", "r");
	pctype2id    = NULL;
    pcinfor      = NULL;
    appinfor     = NULL;
	bits         = NULL;
	pcidforapp_v = NULL;
	days         = 1e4;
	year         = 30;
	plans.resize(INITPLANS);
	pcfreemem    = NULL;
}

AC::~AC()
{
	Fclose(pcinforfp);
	Fclose(appinforfp);
	Fclose(pcdelayinforfp);
	Fclose(appdelayinforfp);
	
	if (pctype2id != NULL)
	{
		delete[] pctype2id;
	}
	
	if (pcinfor != NULL)
	{
		delete[] pcinfor;
	}
	
	if (appinfor != NULL)
	{
		delete[] appinfor;
	}

	if (bits != NULL)
	{
		delete[] bits;
	}
	
	if (pcidforapp_v != NULL)
	{
		delete[] pcidforapp_v;
	}

	if (pcfreemem != NULL)
	{
		delete[] pcfreemem;
	}
	
	/*if (pcdelay != NULL)
	{
		for (int i = 0; i < pcnum; i++)
		{
			delete[] pcdelay[i];
		}
		
		delete[] pcdelay;
	}*/
}

void AC::loadinfor()
{
	fgets(buf, sizeof(buf), pcinforfp);
	
	appnum = atoi(strtok(buf, " "));
	pcnum  = atoi(strtok(NULL, " "));
	
	pctype2id 	= new Type2id[TYPENUM + 1];
	pcinfor 	= new Pcinfor[pcnum];
	appinfor 	= new Appinfor[appnum];
	pcfreemem   = new int[pcnum];
	pcdelay.resize(pcnum, vector<int>(pcnum));
	appdelay.resize(appnum - 1);
	/*pcdelay = new int * [pcnum];
    for (int i = 0; i < pcnum; i++)  
    {  
        pcdelay[i] = new int[pcnum];  
    }*/  
	
	for (int i = 0; fgets(buf, sizeof(buf), pcinforfp) != NULL; i++)
	{
		strtok(buf, " ");
		pcinfor[i].type = atoi(strtok(NULL, " "));
		pcinfor[i].memory = atoi(strtok(NULL, " "));

		pctype2id[pcinfor[i].type].id.insert(i);

	}
	
	for (int i = 0; fgets(buf, sizeof(buf), appinforfp) != NULL; i++)
	{
		strtok(buf, " ");
		appinfor[i].typenum = atoi(strtok(NULL, " "));
		appinfor[i].memory = atoi(strtok(NULL, " "));
		fgets(buf, sizeof(buf), appinforfp);
		token = strtok(buf, " ");
		appinfor[i].type.insert(atoi(token));
		for (int j = 0; j < appinfor[i].typenum - 1; j++)
		{
			token = strtok(NULL, " ");
			appinfor[i].type.insert(atoi(token));
		}
		
		
		
	}
	
	while (fgets(buf, sizeof(buf), pcdelayinforfp) != NULL)
	{
		int id0 = atoi(strtok(buf, " "));
		int id1 = atoi(strtok(NULL, " "));
		int delay = atoi(strtok(NULL, " "));
		pcdelay[id0][id1] = delay;
		pcdelay[id1][id0] = delay;
	}

	fgets(buf, sizeof(buf), appdelayinforfp);
	
	appdelay[0] = atoi(strtok(buf, " "));

	for (int i = 1; i < appnum - 1; i++)
	{
		appdelay[i] = atoi(strtok(NULL, " "));
	}
	
	return ;
}
	
void AC::showinfor()
{
	for (int i = 0; i < pcnum; i++)
	{
		cout << i << "    " << pcinfor[i].type << "    " << pcinfor[i].memory << endl;
	}
	
	cout << "################################################################"<<endl;
	cout << "################################################################"<<endl;
	cout << "################################################################"<<endl;
	
	for (int i = 0; i < appnum; i++)
	{
		cout << i << "    " << appinfor[i].typenum << "    " << appinfor[i].memory << endl;
		if (appinfor[i].typenum != appinfor[i].type.size())
		{
			cout << "load app information error\n";
			exit(-1);
		}
	}

	cout << "################################################################"<<endl;
	cout << "################################################################"<<endl;
	cout << "################################################################"<<endl;

	for (int i = 0; i < appdelay.size() - 1; i++)
	{
		cout << appdelay[i] << "  ";
	}

	cout << appdelay.back() << endl;

	for (int i = 0; i < pcnum; i++)
	{
		for (int j = 0; j < pcnum; j++)
		{
			if (j == i)
			{
				if (pcdelay[i][j] != 0)
				{
					cout << "load app information error\n";
					exit(-1);
				}
			}
			else
			{
				if (pcdelay[i][j] != pcdelay[j][i])
				{
					cout << "load app information error\n";
					exit(-1);
				}
			}

			cout << pcdelay[i][j] << " ";
		}
		cout << endl;
	}
	
	return ;
}

vector<int> AC::pcidforapp(int appid)
{
	vector<int> pcid;
	set<int> tmp;
	for (auto itemtype : appinfor[appid].type)
	{
		for (auto itemid : pctype2id[itemtype].id)
		{
			if (appinfor[appid].memory <= pcinfor[itemid].memory)
			{
				tmp.insert(itemid);
			}	
		}
	}

	for (auto item : tmp)
	{
		pcid.push_back(item);
	}
	
	return pcid;
}

int AC::dnatoid(const string &appdna)
{
	int ret = 0;
	for (int i = 0; i < appdna.size(); i++)
	{
		ret = (2 * ret + appdna[i] - '0');
	}

	return ret;
}

int AC::calfit(const string &dna)
{
	int fitness = 0;
	int head = 0;
	int endid;
	int startid = dnatoid(dna.substr(head, bits[0]));

	if (startid >= pcidforapp_v[0].size())
	{
		return -1;
	}

	for (int i = 0; i < pcnum; i++)
	{
		pcfreemem[i] = pcinfor[i].memory;
	}
	
	head += bits[0];
	
	for (int i = 1; i < appnum; i++)
	{
		endid = dnatoid(dna.substr(head, bits[i]));
		if (endid >= pcidforapp_v[i].size())
		{
			return -1;
		}

		if (pcdelay[pcidforapp_v[i - 1][startid]][pcidforapp_v[i][endid]] > appdelay[i - 1])
		{
			//printf("appdelay[%d] = %d\n", i - 1, appdelay[i - 1]);
			return -1;
		}

		if ((pcfreemem[pcidforapp_v[i - 1][startid]] -= appinfor[i - 1].memory) < 0)
		{
			return -1;
		}

		if (i == appnum - 1 && 
			(pcfreemem[pcidforapp_v[i][startid]] -= appinfor[i].memory) < 0)
		{
			return -1;
		}
		fitness += pcdelay[pcidforapp_v[i - 1][startid]][pcidforapp_v[i][endid]];
		startid = endid;
		head += bits[i];
	}

	

	
	
	return fitness;
}

parants_t AC::choose(const vector<Plan> &plans)
{
	parants_t parants = {0, 0};
	int sum = 0;
	vector<int> ruler;
	for (int i = 0; i < plans.size(); i++)
	{
		sum += plans[i].fitness;
	}

	ruler.push_back(sum - plans[0].fitness);

	for (int i = 1; i < plans.size(); i++)
	{
		ruler.push_back(ruler.back() + sum - plans[i].fitness);
	}

	int randnum = myrand(0, ruler.back());
	for (int i = 0; i < ruler.size(); i++)
	{
		if (randnum <= ruler[i])
		{
			parants.father = i;
			break;
		}
	}

	do
	{
		randnum = myrand(0, ruler.back());
		for (int i = 0; i < ruler.size(); i++)
		{
			if (randnum <= ruler[i])
			{
				parants.mother = i;
				break;
			}
		}
	}while(parants.mother == parants.father);
	
	
	return parants;
}

bool AC::compare(Plan a, Plan b)
{
	return a.fitness < b.fitness;
}

void AC::kill(vector<Plan> & plans)
{
	sort(plans.begin(), plans.end(), compare);
	/*for (int i = 0; i < plans.size(); i++)
	{
		cout <<"plan " << i << " fitness = " << plans[i].fitness; 
	}
	cout << endl;*/
	for (int i = plans.size(); i > INITPLANS; i--)
	{
		plans.pop_back();
	}
	
	return ;
}

void AC::variation(vector <Plan> & plans)
{
	int planid = myrand(0, plans.size() - 1);
	Plan tmp;
	do
	{
		tmp = plans[planid];
		int start = myrand(0, tmp.dna.size() - 1);
		int length = myrand(1, tmp.dna.size() - start);

		for (int i = start; i < start + length; i++)
		{
			tmp.dna[i] = (tmp.dna[i] == '0') ? '1' : '0';/*upside down*/
		}

		

	}while((tmp.fitness = calfit(tmp.dna)) == -1);

	plans[planid] = tmp;
	return ;
}

bool AC::issame(vector<Plan> & plans)
{
	bool ret = true;
	int sample = plans[0].fitness;
	for (int i = 1; i < plans.size(); i++)
	{
		if (sample != plans[i].fitness)
		{
			ret = false;
			return ret;
		}
			
	}

	return ret;
}

void showplans(vector<Plan> & plans)
{
	for (int i = 0; i < plans.size(); i++)
	{
		cout << "plan " << i << ":" << endl;
		//cout << "dna: " << plans[i].dna << endl;
		cout << "fitness: " << plans[i].fitness << endl;
	}
	return ;
}
void AC::ga()
{
	bits = new int[appnum];
	pcidforapp_v = new vector<int>[appnum];
	
	/*consult the PCs which can be put on for each app and the bits of DNA*/
	for (int i = 0; i < appnum; i++)
	{
		/*init bits and pcids*/
		pcidforapp_v[i] = pcidforapp(i);
		if (pcidforapp_v[i].empty())
		{
			printf("data error. try another casefactory\n");
			exit(-1);
		}
		
		bits[i] = (int)ceil(log2(pcidforapp_v[i].size()) ? log2(pcidforapp_v[i].size()) : 1);
	}
	
	/*for (int i = 0; i < appnum; i++)
	{
		cout << bits[i] << " ";
	}
	cout << endl;
	return ;*/
	/*check if this case is good*/

	/*for (int i = 0; i < appnum - 1; i++)
	{
		bool good = false;
		int cnt = 0;
		for (auto start : pcidforapp_v[i])
		{
			for (auto end : pcidforapp_v[i + 1])
			{
				if (pcdelay[start][end] <= appdelay[i])
				{
					cnt++;
					good = true;
				}
			}
		}
		//cout << "pass " << i + 1 << " " << cnt << " ways\n";
		if (!good)
		{
			cout << "bad case\n";
			exit(-1);
		}
	}

	cout << "good case !\n";*/
	/*for (int k = 0; k < appnum; k++)
	{
		cout << "app " << k << " ";
		for (int i = 0; i < pcidforapp_v[k].size(); i++)
		{
			cout << pcidforapp_v[k][i] << " ";
		}
		cout << endl;
	}*/
	
	//return;
	/*create DNA for each app randomly*/
	
	for (int i = 0; i < INITPLANS; i++)
	{
		
		for (;;)
		{
			Plan tmp;
			for (int j = 0; j < appnum; j++)
			{
				string appdna;
				int tmpid;
				do 
				{
					appdna.clear();
					for (int k = 0; k < bits[j]; k++)
					{
						appdna += myrand(0, 1) ? "1" : "0";
					}
					tmpid = dnatoid(appdna);
				}while (tmpid >= pcidforapp_v[j].size());
				
				tmp.dna += appdna;
			}
			tmp.fitness = calfit(tmp.dna);
			if (tmp.fitness != -1)
			{
				plans[i] = tmp;
				break;
			}
			

			//showpcid(tmp);
			//usleep(50000);
			
			//cout << tmp.dna << endl;
			//exit(-1);
		}
		
		
	}		
	//showplans(plans);
	return ;
	while (days--)
	{
		//cout << "day: " << days << endl;
		/*create children*/
		//cout << plans.size() << "  plans\n";
		for (int i = 0; i < INITPLANS * (INITPLANS - 1); i++)
		{
			parants_t parants = choose(plans);
			Plan son0;
			Plan son1;
			/*not stop until created a normal baby, because after fucking, the pcid represented
			  by DNA is inexistent*/
			while((son0.fitness = 
					calfit((son0 = plans[parants.father] + plans[parants.mother]).dna)) == -1)
			{
				//cout << "create an unnormal son0 and i = " << i << endl;
				//cout << "day = " << days << endl;
				//cout << "father = " << parants.father << " mother = " << parants.mother << endl;
			}
			
			while((son1.fitness = 
				    calfit((son1 = plans[parants.father] + plans[parants.mother]).dna)) == -1)
			{
				//cout << "create an unnormal son1\n";
			}
			//cout << plans[parants.father].fitness << " " << plans[parants.mother].fitness << " ";
			//cout << son1.fitness << endl;
			plans.push_back(son0);
			plans.push_back(son1);
			//return ;
		}
		/*kill some plans to INITPLANS plans*/
		//showplans(plans);
		//return ;
		kill(plans);
		
		//return ;
		/*variation(变异) after one year but one year may not is 365 days*/
		if (issame(plans))
		{
			//cout << "variation" << endl;
			variation(plans);/*promise variation will not create unormal baby*/
		}
		//cout << days << endl;
		//showplans(plans);
		//return ;
	}

	sort(plans.begin(), plans.end(), compare);
	
	return ;
}	

/*show the end of plans*/
void AC::showret() 
{
	cout << "DNA:  ";
	int start = 0;
	for (int i = 0; i < appnum; i++)
	{
		cout << plans[0].dna.substr(start, bits[i]) << "-";
		start += bits[i];
	}
	cout << endl;
	start = 0;
	cout << "pcid:  ";
	for (int i = 0; i < appnum; i++)
	{
		cout << pcidforapp_v[i][dnatoid(plans[0].dna.substr(start, bits[i]))] << " ";
		start += bits[i]; 
	}
	cout << endl;
	cout << "fitness = " << plans[0].fitness << endl;
	return ;
}

void AC::showpcid(Plan plan)
{
	int start = 0;
	cout << "pcid:  ";
	for (int i = 0; i < appnum; i++)
	{
		cout << pcidforapp_v[i][dnatoid(plan.dna.substr(start, bits[i]))] << " ";
		start += bits[i]; 
	}
	cout << endl;
	return ;
}

void AC::s2h()
{
	int fd = Open("infor.csv", O_CREAT | O_TRUNC | O_RDWR, 0666);
	
	char buf[200], str[20];
	
	snprintf(buf, sizeof(buf), "%s\n", itoa(pcnum, str, 10));
	Write(fd, buf, strlen(buf));
	
	for (int i = 0; i < pcnum; i++)
	{
		snprintf(buf, sizeof(buf), "%s ", itoa(pcinfor[i].memory, str, 10));
		Write(fd, buf, strlen(buf));
	}
	Write(fd, "\n", 1);
	
	for (int i = 0; i < pcnum; i++)
	{
		for (int k = 0; k < pcnum; k++)
		{
			snprintf(buf, sizeof(buf), "%s ", itoa(pcdelay[i][k], str, 10));
			Write(fd, buf, strlen(buf));
		}
		Write(fd, "\n", 1);
	}

	snprintf(buf, sizeof(buf), "%s\n", itoa(appnum, str, 10));
	Write(fd, buf, strlen(buf));

	for (int i = 0; i < appnum; i++)
	{
		char str1[20], str2[20];
		snprintf(buf, sizeof(buf), "%s %s ", 
			itoa(appinfor[i].memory, str1, 10), itoa(pcidforapp_v[i].size(), str2, 10));
		Write(fd, buf, strlen(buf));

		for (int j = 0; j < pcidforapp_v[i].size(); j++)
		{
			snprintf(buf, sizeof(buf), "%s ", itoa(pcidforapp_v[i][j], str, 10));
			Write(fd, buf, strlen(buf));
		}

		Write(fd, "\n", 1);
	}

	for (int i = 0; i < appdelay.size(); i++)
	{
		snprintf(buf, sizeof(buf), "%s ", itoa(appdelay[i], str, 10));
		Write(fd, buf, strlen(buf));
	}

	Write(fd, "\n", 1);
	
	return ;
}

	
















