#include<string>
#include"TagWord.h"
using namespace std;
int TagWord::tag2index(const string &tag)
{
	enum WordTag index;
	if (tag=="Ag") index=Ag;
	else if (tag=="a") index=a;
	else if (tag=="ad") index=ad;
	else if (tag=="an") index=an;
	else if (tag=="b") index=b;
	else if (tag=="c") index=c;
	else if (tag=="Dg") index=Dg;
	else if (tag=="d") index=d;
	else if (tag=="e") index=e;
	else if (tag=="f") index=f;
	else if (tag=="g") index=g;
	else if (tag=="h") index=h;
	else if (tag=="i") index=i;
	else if (tag=="j") index=j;
	else if (tag=="k") index=k;
	else if (tag=="l") index=l;
	else if (tag=="m") index=m;
	else if (tag=="Ng") index=Ng;
	else if (tag=="n") index=n;
	else if (tag=="nr") index=nr;
	else if (tag=="ns") index=ns;
	else if (tag=="nt") index=nt;
	else if (tag=="nz") index=nz;
	else if (tag=="o") index=o;
	else if (tag=="p") index=p;
	else if (tag=="q") index=q;
	else if (tag=="r") index=r;
	else if (tag=="s") index=s;
	else if (tag=="Tg") index=Tg;
	else if (tag=="t") index=t;
	else if (tag=="u") index=u;
	else if (tag=="Vg") index=Vg;
	else if (tag=="v") index=v;
	else if (tag=="vd") index=vd;
	else if (tag=="vn") index=vn;
	else if (tag=="w") index=w;
	else if (tag=="x") index=x;
	else if (tag=="y") index=y;
	else if (tag=="z") index=z;
	else index=(enum WordTag)-1;
	return index;
}
int TagWord::index2tag(int index,string &tag)
{
	if (index==Ag) tag="Ag";
	else if (index==a) tag="a";
	else if (index==ad) tag="ad";
	else if (index==an) tag="an";
	else if (index==b) tag="b";
	else if (index==c) tag="c";
	else if (index==Dg) tag="Dg";
	else if (index==d) tag="d";
	else if (index==e) tag="e";
	else if (index==f) tag="f";
	else if (index==g) tag="g";
	else if (index==h) tag="h";
	else if (index==i) tag="i";
	else if (index==j) tag="j";
	else if (index==k) tag="k";
	else if (index==l) tag="l";
	else if (index==m) tag="m";
	else if (index==Ng) tag="Ng";
	else if (index==n) tag="n";
	else if (index==nr) tag="nr";
	else if (index==ns) tag="ns";
	else if (index==nt) tag="nt";
	else if (index==nz) tag="nz";
	else if (index==o) tag="o";
	else if (index==p) tag="p";
	else if (index==q) tag="q";
	else if (index==r) tag="r";
	else if (index==s) tag="s";
	else if (index==Tg) tag="Tg";
	else if (index==t) tag="t";
	else if (index==u) tag="u";
	else if (index==Vg) tag="Vg";
	else if (index==v) tag="v";
	else if (index==vd) tag="vd";
	else if (index==vn) tag="vn";
	else if (index==w) tag="w";
	else if (index==x) tag="x";
	else if (index==y) tag="y";
	else if (index==z) tag="z";
	else return -1;
	return index;
}
int TagWord::tagword_passage(const std::string &str_in,string &str_out,string seperator)//虽然有冗余，但是这样使结构清晰
{
	int count=0;
	int start=0,i,j;
	int flag=0;//0汉字，1全角非汉字，2半角字符
	unsigned int dchar=0;//双字节字符编码
	string sentence;
	str_out="";
	
	for(i=0;i <= str_in.length();)
	{
		if((unsigned char)str_in[i]<128)//半角字符
		{
			if(flag==0)
			{	
				if(i-start>0)
				{
					sentence=str_in.substr(start,i-start);
					this->tagword_sentence(sentence,seperator);
					str_out+=sentence;
				}
				flag=2;
			}
			if(str_in[i]=='\n')//换行,不是用二进制读取则不是0x0a,0x0d
			{
				str_out+=(char)str_in[i];
			}
			else if(str_in[i]==' ')//空格
			{
				str_out+=(char)str_in[i];
			}
			else if(ispunct(str_in[i]))//标点
			{
				str_out+=(char)str_in[i];
				str_out+=seperator;
			}
			else//半角字母、数字
			{
				str_out+=(char)str_in[i];
				if(i+1 > str_in.length())
				{
					str_out+=seperator;
				}
				else
				{
					if(!(str_in[i+1]>='a' && str_in[i+1]<='z' 
						|| str_in[i+1]>='A' && str_in[i+1]<='Z')
						|| str_in[i+1]>='0' && str_in[i+1]<='9')
						str_out+=seperator;//分隔半角字符单词、数字
				}
			}
			i++;
		}
		else//全角字符
		{
			dchar^=dchar;
			dchar|=(unsigned char)str_in[i];
			dchar<<=8;
			dchar|=(unsigned char)str_in[i+1];
			if(dchar >= 0xa1a0 && dchar <= 0xa9f0)//中文标点符号的编码范围是:0xA13F~0xAA40
			{
				if(flag==0)
				{
					if(i-start>0)
					{
						sentence=str_in.substr(start,i-start);
						this->tagword_sentence(sentence,seperator);
						str_out+=sentence;
					}
					flag=1;
				}
				str_out+=str_in.substr(i,2);
				if(dchar >= 0xa3b0 && dchar <=0xa3b9 /*0~9*/
					|| dchar >=0xa3c1 && dchar<=0xa3da /*a~z*/
					|| dchar >= 0xa3e1 && dchar <= 0xa3fa/*A~Z*/)
				{
					if(i+3 > str_in.length())
						str_out+=seperator;
					else
					{
						dchar^=dchar;
						dchar|=(unsigned char)str_in[i+2];
						dchar<<=8;
						dchar|=(unsigned char)str_in[i+3];
						if(!(dchar >= 0xa3b0 && dchar <=0xa3b9 /*0~9*/
						|| dchar >=0xa3c1 && dchar<=0xa3da /*a~z*/
						|| dchar >= 0xa3e1 && dchar <= 0xa3fa/*A~Z*/))
						str_out+=seperator;
					}
				}
				else
					str_out+=seperator;
			}
			else
			{
				if(flag==1 || flag==2)
				{
					start=i;
					flag=0;
				}
			}
			i+=2;
		}
	}
	return count;
}
int TagWord::tagword_file(const char *inpath,const char *outpath,string seperator)
{
	int count=0;
	string str_in,str_out;
	load_passage(inpath,str_in);
	count=tagword_passage(str_in,str_out,seperator);
	save_passage(outpath,str_out);
	return count;
}