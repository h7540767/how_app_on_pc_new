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
	pcinforfp    = Fopen("pcinfor.csv", "r");
	appinforfp   = Fopen("appinfor.csv", "r");
	delayinforfp = Fopen("delayinfor.csv", "r");
	pctype2id    = NULL;
    pcinfor      = NULL;
    appinfor     = NULL;
	bits         = NULL;
	pcidforapp_v = NULL;
	days         = 1e4;
	year         = 30;
	plans.resize(INITPLANS);
}

AC::~AC()
{
	Fclose(pcinforfp);
	Fclose(appinforfp);
	Fclose(delayinforfp);
	
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
	
	/*if (delayinfor != NULL)
	{
		for (int i = 0; i < pcnum; i++)
		{
			delete[] delayinfor[i];
		}
		
		delete[] delayinfor;
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

	delayinfor.resize(pcnum, vector<int>(pcnum));
	/*delayinfor = new int * [pcnum];
    for (int i = 0; i < pcnum; i++)  
    {  
        delayinfor[i] = new int[pcnum];  
    }*/  
	
	for (int i = 0; fgets(buf, sizeof(buf), pcinforfp) != NULL; i++)
	{
		strtok(buf, " ");
		pcinfor[i].type = atoi(strtok(NULL, " "));
		pcinfor[i].memory = atoi(strtok(NULL, " "));
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
		
		for (auto item : appinfor[i].type)
		{
			pctype2id[item].id.insert(i);
		}
		
	}
	
	while (fgets(buf, sizeof(buf), delayinforfp) != NULL)
	{
		int id0 = atoi(strtok(buf, " "));
		int id1 = atoi(strtok(NULL, " "));
		int delay = atoi(strtok(NULL, " "));
		delayinfor[id0][id1] = delay;
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
		ret += 2 * ret + appdna[i] - '0';
	}

	return ret;
}

int AC::calfit(const string &dna)
{
	int fitness = 0;
	int head = 0;
	int startid = dnatoid(dna.substr(head, bits[0]));
	int endid;
	
	if (startid >= pcidforapp_v[0].size())
	{
		return -1;
	}
	
	head += bits[0];
	
	for (int i = 1; i < appnum; i++)
	{
		endid = dnatoid(dna.substr(head, bits[i]));
		if (endid >= pcidforapp_v[i].size())
		{
			return -1;
		}
		fitness += delayinfor[startid][endid];
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
		
		bits[i] = (int)ceil(log2(pcidforapp_v[i].size()));
	}
	/*create DNA for each app randomly*/
	for (int i = 0; i < INITPLANS; i++)
	{
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
			
			plans[i].dna += appdna;
		}
		
		plans[i].fitness = calfit(plans[i].dna);
		
	}		
	//showplans(plans);
	//return;
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
			variation(plans);/*promise variation will not create unormal baby*/
		}
		//cout << days << endl;
		//showplans(plans);
	}

	sort(plans.begin(), plans.end(), compare);
	
	return ;
}	

/*show the end of plans*/
void AC::showret() 
{
	cout << "DNA:  " << plans[0].dna << endl;
	int start = 0;
	cout << "pcid:  ";
	for (int i = 0; i < appnum; i++)
	{
		cout << dnatoid(plans[0].dna.substr(start, bits[i])) << " ";
		start += bits[i]; 
	}
	cout << endl;
	cout << "fitness = " << plans[0].fitness << endl;
	return ;
}

	
















