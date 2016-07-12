#include <string>
#include<fstream>
#include "hmm.h"
using namespace std;

HMM::HMM()
{
	memset(start_p,0,NUM_STATE*sizeof(double));
	memset(trans_p,0,NUM_STATE*NUM_STATE*sizeof(double));
	memset(emit_p,0,NUM_STATE*NUM_OBS*sizeof(double));
}
HMM::HMM(const char *hmm_path)
{
	load_hmm(hmm_path);
}
int HMM::word2num(const std::string &word)
{
	return word2num(word[0],word[1]);
}
int HMM::word2num(char c1,char c2)
{
	int res=DivideWord::word2num(c1,c2);
	if(res<0) res+=NUM_OBS;
	return res;
}
int HMM::char2state(char c)
{
	switch(c)
	{
		case 'B':return B;
		case 'M':return M;
		case 'E':return E;
		case 'S':return S;
		default :return -1;
	}
}
void HMM::count2prob(int start[],int trans[][NUM_STATE],int emit[][NUM_OBS])
{
	int sum;
	//ini
	//start_p
	sum^=sum;
	for(int i=0;i<NUM_STATE;i++)
		sum+=start[i];
	for(int i=0;i<NUM_STATE;i++)
		start_p[i]=start[i]==0 ? 0:(double)start[i]/(double)sum;
	//trans_p
	for(int i=0;i<NUM_STATE;i++)
	{
		sum^=sum;
		for(int j=0;j<NUM_STATE;j++)
			sum+=trans[i][j];
		for(int j=0;j<NUM_STATE;j++)
			trans_p[i][j]=trans[i][j]==0 ? 0:(double) trans[i][j]/(double)sum;
	}
	//emit_p
	for(int i=0;i<NUM_STATE;i++)
	{
		sum^=sum;
		for(int j=0;j<NUM_OBS;j++)
			sum+=emit[i][j];
		for(int j=0;j<NUM_OBS;j++)
			emit_p[i][j]=emit[i][j]==0 ? 0:(double) emit[i][j]/(double)sum;
	}

}
int HMM::train_hmm(const char *path)
{
	ifstream fin(path,ios::in);
	if(fin.fail()) return -1;
	long count=0;
	int pos;
	string line;
	string word;
	
	int s0=E,s1;//隐藏态(BMES)
	int obs;//观察态索引（汉字）
	int start[NUM_STATE]={0};					//初始隐含态(BMES)
	int trans[NUM_STATE][NUM_STATE]={0};		//状态转移矩阵
	int emit[NUM_STATE][NUM_OBS]={0};			//发射矩阵

	while(getline(fin,line))
	{
		pos=line.find("/",0);
		while(pos!=string::npos)
		{
			s1=char2state(line[pos+1]);
			if(s0==S || s0==E) start[s1]++;//初始状态
			trans[s0][s1]++;//状态转移
			word=line.substr(pos-2,2);
			obs=word2num(word);
			if(obs<0) obs+=NUM_OBS;
			if(obs>NUM_OBS)
			{
				printf("the obs num is not enough(%d)\n",obs);
				continue;
			}
			emit[s1][obs]++;//隐含态发射
			s0=s1;
			pos=line.find("/",pos+1);
		}
		count++;
	}
	fin.close();
	count2prob(start,trans,emit);
	return count;
}
int HMM::save_hmm(const char *path)
{
	ofstream fout(path,ios::binary);//由于double形fout会损失精度，还是直接二进制吧
	if(fout.fail()) return -1;
	fout.write((char*)start_p,NUM_STATE*sizeof(double));
	fout.write((char*)trans_p,NUM_STATE*NUM_STATE*sizeof(double));
	fout.write((char*)trans_p,NUM_STATE*NUM_OBS*sizeof(double));
	fout.close();
	return 0;
}
int HMM::load_hmm(const char *path)
{
	ifstream fin(path,ios::binary);
	if(fin.fail()) return -1;
	fin.read((char*)start_p,NUM_STATE*sizeof(double));
	fin.read((char*)trans_p,NUM_STATE*NUM_STATE*sizeof(double));
	fin.read((char*)trans_p,NUM_STATE*NUM_OBS*sizeof(double));
	return 0;
}
int HMM::divide_sentence(string &sentence,std::string seperator)
{
	int count=0;
	int obs[MAX_LEN];//缓存区，总是new会降低速度
	int *p_obs;
	int *res;
	int states[NUM_STATE]={B,M,E,S};
	int len_obs=sentence.length()/2;
	
	if(len_obs > MAX_LEN *2 )
		p_obs=new int[len_obs];
	else p_obs=obs;
	
	for(int i=0;i<len_obs;i++)
		p_obs[i]=word2num(sentence[i<<1],sentence[(i<<1)+1]);
	res=Viterbi(NUM_OBS,NUM_STATE,len_obs,
				p_obs,states,start_p,
				(double **)trans_p,(double **)emit_p);
	
	for(int i=len_obs-1;i>=0;i--)//模型没问题，就最后分段这出问题了，注意下标。。。在纸上多画画，不要总是空想
	{
		if(res[i]==E || res[i]==S)
		{
			sentence.insert((i+1)<<1,seperator);
			count++;
		}
	}
	if(res[len_obs-1]!=E && res[len_obs-1]!=S)
		sentence+=seperator;//若最后不是结束符，强制加结束符
	if(len_obs > MAX_LEN )
		delete[] p_obs;
	delete[] res;
	return count;
}