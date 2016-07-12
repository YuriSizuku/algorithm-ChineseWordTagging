#include "DivideWord.h"
#ifndef _TAGWORD_H
#define _TAGWORD_H
const int NUM_TAG=40;
class TagWord:public DivideWord
{
	protected:
		DivideWord *spliter;//�ִ���
		int tag2index(const std::string &tag);
		int index2tag(int index,std::string &tag); 
	public:
		TagWord(DivideWord *spliter):spliter(spliter){}
		void setSpliter(DivideWord *spliter){this->spliter=spliter;}
		enum WordTag{
			Ag /*������: ���ݴ������ء����ݴʴ���Ϊa�����ش����ǰ������A��*/,
			a /*���ݴ�: ȡӢ�����ݴ�adjective�ĵ�1����ĸ��*/,
			ad /*���δ�: ֱ����״������ݴʡ����ݴʴ���a�͸��ʴ���d����һ��*/,
			an /*���δ�: �������ʹ��ܵ����ݴʡ����ݴʴ���a�����ʴ���n����һ�� */,
			b /*�����: ȡ���֡��𡱵���ĸ��*/,
			c /*����: ȡӢ������conjunction�ĵ�1����ĸ��*/,
			Dg /*������: ���������ء����ʴ���Ϊd�����ش����ǰ������D��*/,
			d /*����: ȡadverb�ĵ�2����ĸ�������1����ĸ���������ݴʡ�*/,
			e /*̾��: ȡӢ��̾��exclamation�ĵ�1����ĸ��*/,
			f /*��λ��: ȡ���֡����� ����ĸ��*/,
			g /*����: ����������ض�����Ϊ�ϳɴʵġ��ʸ�����ȡ���֡���������ĸ��*/,
			h /*ǰ�ӳɷ�: ȡӢ��head�ĵ�1����ĸ��*/,
			i /*����: ȡӢ�����idiom�ĵ�1����ĸ��*/,
			j /*�������: ȡ���֡��򡱵���ĸ��*/,
			k /*��ӳ�: ��*/,
			l /*ϰ����: ϰ������δ��Ϊ����е㡰��ʱ�ԡ���ȡ���١�����ĸ��*/,
			m /*����: ȡӢ��numeral�ĵ�3����ĸ��n��u�������á�*/,
			Ng /*������: ���������ء����ʴ���Ϊn�����ش����ǰ������N��*/,
			n /*����: ȡӢ������noun�ĵ�1����ĸ�� */,
			nr /*����: ���ʴ���n�͡���(ren)������ĸ����һ��*/,
			ns /*����: ���ʴ���n�ʹ����ʴ���s����һ��*/,
			nt /*��������: ���š�����ĸΪt�����ʴ���n��t����һ��*/,
			nz /*����ר��: ��ר������ĸ�ĵ�1����ĸΪz�����ʴ���n��z����һ��  */,
			o /*������: ȡӢ��������onomatopoeia�ĵ�1����ĸ��*/,
			p /*���: ȡӢ����prepositional�ĵ�1����ĸ��*/,
			q /*����: ȡӢ��quantity�ĵ�1����ĸ��*/,
			r /*����: ȡӢ�����pronoun�ĵ�2����ĸ,��p�����ڽ�ʡ�*/,
			s /*������: ȡӢ��space�ĵ�1����ĸ��*/,
			Tg /*ʱ����: ʱ��������ء�ʱ��ʴ���Ϊt,�����صĴ���gǰ������T��*/,
			t /*ʱ���: ȡӢ��time�ĵ�1����ĸ��*/,
			u /*����: ȡӢ������auxiliary �ĵ�2����ĸ,��a���������ݴʡ�*/,
			Vg /*������: ���������ء����ʴ���Ϊv�������صĴ���gǰ������V��*/,
			v /*����: ȡӢ�ﶯ��verb�ĵ�һ����ĸ��*/,
			vd /*������: ֱ����״��Ķ��ʡ����ʺ͸��ʵĴ��벢��һ��*/,
			vn /*������: ָ�������ʹ��ܵĶ��ʡ����ʺ����ʵĴ��벢��һ��*/,
			w /*������:  */,
			x /*��������: ��������ֻ��һ�����ţ���ĸxͨ�����ڴ���δ֪�������š�*/,
			y /*������: ȡ���֡������ĸ��*/,
			z /*״̬��: ȡ���֡�״������ĸ��ǰһ����ĸ��*/};
		/*http://www.icl.pku.edu.cn/icl_groups/corpus/addition.htm �ϵ�˵����ƥ������ʽ
		   ^    ([a-zA-Z]+)(\s*)(\w+)(\s*)(.+) 
		*/
		//��ע�麯����DivideWord�е�divide�麯������
		virtual int tagword_sentence(std::string &sentence,std::string seperator)=0;//��עû�б��ľ���
		virtual int tagword_passage(const std::string &str_in,std::string &str_out,std::string seperator="/");//��עһ���ַ��������б�㣩
		virtual int tagword_file(const char *inpath,const char *outpath,std::string seperator="/");//��ע�ļ�
};
#endif