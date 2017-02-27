#ifndef AC_H
#define AC_H

#include "wrap.h"

#define TYPENUM 50
#define BUFLINE 1000
#define INITPLANS 4

using namespace std;




typedef struct 
{
    int father;
    int mother;
}parants_t;

class Pcinfor
{
	public:
		int type;
		int memory;
};

class Type2id
{
	public:
		set<int> id;
};

class Appinfor
{
	public:
		int typenum;
		int memory;
		set<int> type;
};

class Plan
{
	public:
		string dna;
		int fitness;
		
	public:
		Plan()
		{
			dna = "";
            fitness = -1;
		}
		virtual ~Plan()
		{
			
		}

        Plan operator + (const Plan son);
		
};

class AC
{
	public:
		AC();
		virtual ~AC();
		void loadinfor();
		void showinfor();
		void ga();
		void showret();

    public:
        vector<Plan> plans;
		
	private:
		FILE *pcinforfp, *appinforfp, *delayinforfp;
		char buf[BUFLINE], *token;
		size_t  len;
		int appnum, pcnum;
		Type2id * pctype2id;
		Pcinfor * pcinfor;
		Appinfor* appinfor;
        vector<int> * pcidforapp_v;
		vector<vector<int>> delayinfor;
		int days;
		int year;
        int * bits;
		
	
	private:
		vector<int> pcidforapp(int appid);
		int dnatoid(const string & appdna);
		int calfit(const string & dna);
        parants_t choose(const vector<Plan> & plans);
        void kill(vector<Plan> & plans);
        static bool compare(Plan a, Plan b);
        void variation(vector<Plan> & plans);
        bool issame(vector<Plan> & plans);
	
};

void showplans(vector<Plan> & plans);


#endif

















