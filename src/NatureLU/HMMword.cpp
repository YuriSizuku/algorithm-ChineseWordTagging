#include<iostream>
#include<string>
#include<fstream>
#include<map>
#include "HMMword.h"
using namespace std;
//不再头文件中定义const变量，防止重定义
HMMword::HMMword(DivideWord *spliter,const char *path_hmm,const char *path_wordmap):TagWord(spliter)
{
	load_args(path_hmm,path_wordmap);
}
void HMMword::count2prob(int start[],int trans[][NUM_TAG],int emit[][NUM_WORD])
{
	int sum;
	//ini
	//start_p
	sum^=sum;
	for(int i=0;i<NUM_TAG;i++)
		sum+=start[i];
	for(int i=0;i<NUM_TAG;i++)
		start_p[i]=start[i]==0 ? 0:(double)start[i]/(double)sum;
	//trans_p
	for(int i=0;i<NUM_TAG;i++)
	{
		sum^=sum;
		for(int j=0;j<NUM_TAG;j++)
			sum+=trans[i][j];
		for(int j=0;j<NUM_TAG;j++)
			trans_p[i][j]=trans[i][j]==0 ? 0:(double) trans[i][j]/(double)sum;
	}
	//emit_p
	for(int i=0;i<NUM_TAG;i++)
	{
		sum^=sum;
		for(int j=0;j<NUM_WORD;j++)
			sum+=emit[i][j];
		for(int j=0;j<NUM_WORD;j++)
			emit_p[i][j]=emit[i][j]==0 ? 0:(double) emit[i][j]/(double)sum;
	}

}
int HMMword::train_hmm(const char *path)
{
	ifstream fin(path,ios::in);
	if(fin.fail()) return -1;
	long count=0;
	int pos;
	int tpos;
	string line;
	string word;
	string tag;
	map<string,int>::iterator iter;
	
	int s0,s1;//隐藏态(词性)
	int obs;//观察态索引（词语索引）
	int start[NUM_TAG]={0};					//初始隐含态(BMES)
	int trans[NUM_TAG][NUM_TAG]={0};		//状态转移矩阵
	int emit[NUM_TAG][NUM_WORD]={0};			//发射矩阵

	while(getline(fin,line))
	{
		pos=line.find("/",0);
		s0=-1;
		while(pos!=string::npos)
		{
			//得到标签
			tag="";
			if(pos+1 >=line.length()) break;
			tag+=(char)line[pos+1];
			if(pos+2 < line.length()) 
			{
				if((char)line[pos+2]>='a' && (char)line[pos+2]<='z')
					tag+=(char)line[pos+2];
			}
			s1=tag2index(tag);
			//建立词语索引(不统计 /m /w /t /x)
			if(s1==m || s1==w || s1==t || s1==x)
				goto next;
			if(s1<0 || s1>=NUM_TAG)
				goto next;//超出范围
			tpos=pos;
			while(1)
			{
				tpos-=2;
				if((unsigned char)line[tpos+1]<128)//asci字符,假设语料库每个汉字前面都是半角字符
				{
					tpos+=2;
					break;
				}
				if(tpos<0) break;
			}
			if(tpos<0) goto next;
			if(pos-tpos<2 || (pos-tpos)%2!=0) goto next;
			word=line.substr(tpos,pos-tpos);
			iter=word_map.find(word);
			if(iter==word_map.end())//没找到
			{
				word_map.insert(pair<string,int>(word,word_map.size()));
				obs=word_map.size()-1;
			}
			else obs=iter->second;
			//统计
			if(s0==-1) 
			{
					start[s1]++;//开始
					s0=s1;
			}
			else 
			{
				trans[s0][s1]++;
				emit[s1][obs]++;
				s0=s1;	
				count++;
			}	
next:
			//继续查找下一个
			pos=line.find("/",pos+1);
		}
	}
	fin.close();
	count2prob(start,trans,emit);
	return count;
}
int HMMword::save_args(const char *path_hmm,const char *path_wordmap)
{
	int res;
	res=save_hmm(path_hmm);
	if(res<0) return -1;
	res=save_wordmap(path_wordmap);
	if(res<0) return -1;
	return res;
}
int HMMword::load_args(const char *path_hmm,const char *path_wordmap)
{
	int res;
	res=load_hmm(path_hmm);
	if(res<0) return -1;
	res=load_wordmap(path_wordmap);
	if(res<0) return -1;
	return res;
}
int HMMword::save_hmm(const char *path)
{
	ofstream fout(path,ios::binary);//由于double形fout会损失精度，还是直接二进制吧
	if(fout.fail()) return -1;
	fout.write((char*)start_p,NUM_TAG*sizeof(double));
	fout.write((char*)trans_p,NUM_TAG*NUM_TAG*sizeof(double));
	fout.write((char*)trans_p,NUM_TAG*NUM_WORD*sizeof(double));
	fout.close();
	return 0;
}
int HMMword::load_hmm(const char *path)
{
	ifstream fin(path,ios::binary);
	if(fin.fail()) return -1;
	fin.read((char*)start_p,NUM_TAG*sizeof(double));
	fin.read((char*)trans_p,NUM_TAG*NUM_TAG*sizeof(double));
	fin.read((char*)trans_p,NUM_TAG*NUM_WORD*sizeof(double));
	return 0;
}
int HMMword::save_wordmap(const char *path)
{
	ofstream fout(path);
	if(fout.fail()) return -1;
	
	map<string,int>::iterator iter;
	for(iter=word_map.begin();iter!=word_map.end();++iter)
	{
		fout<<iter->first<<"  "<<iter->second<<endl;
	}
	return 0;
}
int HMMword::load_wordmap(const char *path)
{
	ifstream fin(path);
	if(fin.fail()) return -1;

	string word;
	int index;
	while(!fin.eof())
	{
		fin>>word>>index;
		word_map[word]+=index;//map数组形式访问
	}
	return 0;
}
int HMMword::divide_sentence(string &sentence,string seperator)
{
	return spliter->divide_sentence(sentence,seperator);
}
int HMMword::tagword_sentence(string &sentence,string seperator)
{
	int obs[MAX_LEN];//缓存区，总是new会降低速度
	int *p_obs;
	int *res;
	int pos,pos2;
	int states[NUM_TAG];
	int len_obs=0;
	map<string,int>::iterator iter;
	string word;
	string tag;
	string sepr="  ";//分隔符

	divide_sentence(sentence,seperator);//词性标注的分词预处理
	
	for(int i=0;i<NUM_TAG;i++) states[i]=i;//词性数初始化
	pos=sentence.find(seperator);//统计词语个数
	while(pos!=string::npos)
	{
		len_obs++;
		pos=sentence.find(seperator,pos+1);
	}
	if(len_obs<0) return -1;

	if(len_obs> MAX_LEN *2 )
		p_obs=new int[len_obs];
	else p_obs=obs;
	
	pos=0;
	pos2=sentence.find(seperator);
	for(int i=0;i<len_obs;i++)//形成观察数据
	{
		word=sentence.substr(pos,pos2-pos);
		iter=word_map.find(word);
		if(iter==word_map.end())//没找到
		{
			p_obs[i]=x;
		}
		else
		{
			p_obs[i]=iter->second;
		}
		pos=pos2+seperator.length();
		pos2=sentence.find(seperator,pos2+1);
	}
	res=Viterbi(NUM_WORD,NUM_TAG,len_obs,
				p_obs,states,start_p,
				(double **)trans_p,(double **)emit_p);
	
	pos=sentence.length();
	for(int i=len_obs-1;i>=0;i--)
	{
		index2tag(res[i],tag);
		tag+=sepr;
		pos=sentence.rfind(seperator,pos-1);
		sentence.insert(pos+seperator.length(),tag);
	}
	if(len_obs > MAX_LEN )
		delete[] p_obs;
	delete[] res;
	return len_obs;
}