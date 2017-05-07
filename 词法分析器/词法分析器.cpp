// �ʷ�������.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

//ö���࣬ö������״̬
enum LR{
	identifier = 0,		//id
	decimal ,		//����
	/*����Ϊ������*/
	thread ,
	features , 
	flows,
	properties,
	end,
	none,
	in,
	out,
	data,
	port,
	event,
	parameter,
	flow,
	source,
	sink,
	path,
	constant,
	access,
	/*����Ϊ����*/
	eg,		//=>
	aeg,	//+=>
	sem,	//;
	colon,	//:
	dcolon,	//::
	lb,		//{
	rb,		//}
	to,		//->
	/*����Ϊ����״̬*/
	start,
	id1,
	dec0,
	dec1,
	dec2,
	dec3,
	eg1,
	aeg1,
	/*����״̬*/
	error
};

//ö������ַ�����
vector<string> mark = {"identifier","decimal","thread","features","flows","properties","end","none","in","out","data","port",
						"event","parameter","flow","source","sink","path","constant","access","eg","aeg","sem","colon","dcolon",
						"lb","rb","to","start","id1","dec0","dec1","dec2","dec3","eg1","aeg1","error"};

string s = "";		//�洢���ڽ����ĵ���
LR lr = start;		//��ʼ״̬��start
int p = 0;	//�ļ���ȡָ��
bool flg;	//��ǣ���ǰ�ַ����ڵ�ǰ����
int line = 1;	//�к�

//���溯���������˴���
void alert(char c,string expect = "") {
	cout << endl << "�����У� "<<line-1 <<" �޷��������ַ���" << c << "(\'\\" << (int)c <<"\')" << endl;
	cout << "��ǰ״̬Ϊ " << mark[lr];
	if(expect!="")cout << "���ڴ�����Ϊ" << expect;
	cout<< endl;
	lr = error;		//תΪ����״̬
}

//�������������Ѿ�ƥ��ĵ���
void print() {
	if (lr == identifier) {		//�����Ѿ�ƥ��Ϊid�ĵ��ʣ��п����Ǳ����֣���Ҫ��һ��ɸѡ
		for (int i = 2; mark[i] != "eg"; i++) {
			if (s == mark[i])lr = LR(i);
		}
	}
	cout << '(' << s << ',' << mark[lr] << ')' << "  ";
	lr = start;		//�ع��ʼ״̬
	s = "";		//����Դ洢��һ������
}

//��ȡ���˲����ڵ�ǰ���ʵ��ַ��ĳ�������
void redo() {
	print();
	p--;
	flg = 0;	//���
}

//����������ÿ�ζ�ȡһ��char
void parse(char c) {
	flg = 1;	//��ǻ�ԭ
	//���˿��ַ�
	if (isspace(c)) {	
		if(lr != start)print();
		if (c == '\n')cout << endl << endl << "line " << line++ << " : ";
		return;
	}
	
	//״̬ת��
	switch (lr) {
	case start:
		if (isalpha(c))lr = identifier;
		else if (c == '-')lr = dec0;
		else if (c == '+')lr = dec1;
		else if (isdigit(c))lr = dec2;
		else if (c == '=')lr = eg1;
		else if (c == '+')lr = aeg1;
		else if (c == ';')lr = sem;
		else if (c == ':')lr = colon;
		else if (c == '{')lr = lb;
		else if (c == '}')lr = rb;
		else alert(c);
		break;
	case identifier:
		if (isalnum(c))break;
		else if (c == '_')lr = id1;
		else redo();
		break;
	case id1:
		if (isalnum(c))lr = identifier;
		else alert(c,"��ĸ������");
		break;
	case dec0:
		if (isdigit(c))lr = dec2;
		else if (c == '>')lr = to;
		else alert(c,"С����>");
		break;
	case dec1:
		if (isdigit(c))lr = dec2;
		else if (c == '=')lr = aeg1;
		else alert(c,"С����=");
		break;
	case dec2:
		if (isdigit(c))break;
		else if (c == '.')lr = dec3;
		else alert(c,"С���������");
		break;
	case dec3:
		if (isdigit(c))lr = decimal;
		else alert(c,"����");
		break;
	case decimal:
		if (isdigit(c))break;
		else redo();
		break;
	case eg1:
		if (c == '>')lr = eg;
		else alert(c,">");
		break;
	case aeg1:
		if (c == '>')lr = aeg;
		else alert(c,">");
		break;
	case colon:
		if (c == ':')lr = dcolon;
		else redo();
		break;
	case error:
		if (isspace(c))lr = start;
	default:
		redo();
	}
	if(flg)s += c;	//���ַ���¼��ǰƥ��ĵ���
}
int main()
{
	std::ifstream t("test3.txt");
	std::string all((istreambuf_iterator<char>(t)),istreambuf_iterator<char>());	//��ȡ�����ĵ�����
	cout << "line" << line++ << " : " ;
	for (; p < all.length(); p++)	
		parse(all[p]);	//���ֽ���
	getchar();
    return 0;
}