// 词法分析器.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//枚举类，枚举所有状态
enum LR{
	identifier = 0,		//id
	decimal ,		//数字
	/*以下为保留字*/
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
	/*以下为符号*/
	eg,		//=>
	aeg,	//+=>
	sem,	//;
	colon,	//:
	dcolon,	//::
	lb,		//{
	rb,		//}
	to,		//->
	/*以下为过渡状态*/
	start,
	id1,
	dec0,
	dec1,
	dec2,
	dec3,
	eg1,
	aeg1,
	/*错误状态*/
	error
};

//枚举类的字符名字
vector<string> mark = {"identifier","decimal","thread","features","flows","properties","end","none","in","out","data","port",
						"event","parameter","flow","source","sink","path","constant","access","eg","aeg","sem","colon","dcolon",
						"lb","rb","to","start","id1","dec0","dec1","dec2","dec3","eg1","aeg1","error"};

string s = "";		//存储正在解析的单词
LR lr = start;		//初始状态：start
int p = 0;	//文件读取指针
bool flg;	//标记：当前字符属于当前单词
int line = 1;	//行号

//警告函数，发生了错误
void alert(char c,string expect = "") {
	cout << endl << "错误行： "<<line-1 <<" 无法解析的字符：" << c << "(\'\\" << (int)c <<"\')" << endl;
	cout << "当前状态为 " << mark[lr];
	if(expect!="")cout << "，期待输入为" << expect;
	cout<< endl;
	lr = error;		//转为错误状态
}

//输出函数，输出已经匹配的单词
void print() {
	if (lr == identifier) {		//对于已经匹配为id的单词，有可能是保留字，需要进一步筛选
		for (int i = 2; mark[i] != "eg"; i++) {
			if (s == mark[i])lr = LR(i);
		}
	}
	cout << '(' << s << ',' << mark[lr] << ')' << "  ";
	lr = start;		//回归初始状态
	s = "";		//清空以存储下一个单词
}

//读取到了不属于当前单词的字符的撤销处理
void redo() {
	print();
	p--;
	flg = 0;	//标记
}

//解析函数，每次读取一个char
void parse(char c) {
	flg = 1;	//标记还原
	//过滤空字符
	if (isspace(c)) {	
		if(lr != start)print();
		if (c == '\n')cout << endl << endl << "line " << line++ << " : ";
		return;
	}
	
	//状态转移
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
		else alert(c,"字母或数字");
		break;
	case dec0:
		if (isdigit(c))lr = dec2;
		else if (c == '>')lr = to;
		else alert(c,"小数或>");
		break;
	case dec1:
		if (isdigit(c))lr = dec2;
		else if (c == '=')lr = aeg1;
		else alert(c,"小数或=");
		break;
	case dec2:
		if (isdigit(c))break;
		else if (c == '.')lr = dec3;
		else alert(c,"小数点或数字");
		break;
	case dec3:
		if (isdigit(c))lr = decimal;
		else alert(c,"数字");
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
	if(flg)s += c;	//逐字符记录当前匹配的单词
}
int main()
{
	std::ifstream t("test3.txt");
	std::string all((istreambuf_iterator<char>(t)),istreambuf_iterator<char>());	//读取整个文档内容
	cout << "line" << line++ << " : " ;
	for (; p < all.length(); p++)	
		parse(all[p]);	//逐字解析
	getchar();
    return 0;
}