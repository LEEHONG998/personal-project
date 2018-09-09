#include "pch.h"
#include <iostream>
#include<cstdio>
#include<cstring>
#include<string>
#include<utility>
#include<set>
#include<map>
#pragma warning(disable:4996)
using namespace std;
const int N = 100007;
#define de(x) cout<<#x<<" = "<<x<<endl
#define rep(i,a,b) for(int i=(a);i<(b);++i)
struct node {
	node *next[36];
	int gs;
}root;
string line[N];
int linenum, efline, efchar, efword;                    /* 变量依次为文件总行数、有效行数、字符数、有效单词数  */
int Atoatoint(char c)                     /*  英文的大小写转化  */
{ 
	if (c >= 'A'&&c <= 'Z')c += 'A' - 'a';
	if (c >= 'a'&&c <= 'z') return c - 'a';
	else return c - '0' + 26;
}
char FAtoatoint(int c) 
{ 
	if (c >= 0 && c <= 25)return 'a' + c;
	else return c - 26 + '0';
}
void qinsert(string s)
{  //字典树插入单词
	int len = s.size(), tmp;
	node *p;
	p = &root;
	rep(i, 0, len) {
		tmp = Atoatoint(s[i]);
		if (p->next[tmp] == NULL) {
			p->next[tmp] = new node(); p->next[tmp]->gs = 0;
		}
		p = p->next[tmp];
		if (i == len - 1) { p->gs++; }
	}
}
void MYscanf(char s[]) {  
	freopen(s, "r", stdin);
	string str;
	while (getline(cin, str)) {
		int len = str.size();
		line[efline++] = str;
		if (len == 0)continue;
		bool f = 0;
		rep(i, 0, len) {
			if (str[i] != ' ')f = 1;
		}
		if (f) { efline++; }
	}
}
int CountChar() {
	int ret = 0;
	rep(i, 0, linenum) {
		int lim = line[i].size();
		rep(j, 0, lim) {
			if (line[i][j] >= 0 && line[i][j] <= 255)ret++;       /* 计算有效字符 */
		}
		ret++;      /* 除了最后一行每一行都会存在换行符 */
	}
	return ret - 1;       /* 此处减去的是前面提到的多算的最后一行的换行符 */
}
bool is_efct_char(char c) {
	if ((c >= '0'&&c <= '9') || (c >= 'A'&&c <= 'Z') || (c >= 'a'&&c <= 'z'))return true;
	return false;
}
int CountWord() {
	int ret = 0;
	rep(i, 0, linenum) {
		bool f = 1; int lim = line[i].size();
		rep(j, 0, lim) {
			if (is_efct_char(line[i][j])) {
				string str = ""; str += line[i][j];
				rep(k, j + 1, lim) {
					if (is_efct_char(line[i][k]))str += line[i][k], j = k;
					else { j = k;  break; }
				}
				bool f = 0;; int siz = str.size() - 1;
				rep(k, 0, siz) {
					if (str[k] >= '0'&&str[k] <= '9' && (str[k + 1]<'0' || str[k + 1]>'9'))f = 1;
				}
				if (!f)            /* 是否有效字符的判断 */
				{          
					ret++;
					qinsert(str);//在字典树中插入这个单词,为计算单词出现次数做准备。字典树节约空间以及为后续统计大幅度缩减时间复杂度
				}
			}
		}
	}
	return ret;
}
string tmp;
set<pair<int, string> >qur;              /* set函数排序，先按照次数排列，相同次数使用字典序排 */
set<pair<int, string> >::iterator it;
void dfs_getword(node u) {
	if (u.gs != 0) {
		qur.insert(make_pair(-u.gs, tmp));
		if (qur.size() > 10) {
			it = qur.end();
			it--;
			qur.erase(it);
		}
	}
	rep(i, 0, 36) {
		if (u.next[i] != NULL) {
			tmp += FAtoatoint(i);
			dfs_getword(*u.next[i]);
			tmp.erase(tmp.size() - 1);
		}
	}
}
void CountMxWord() {
	qur.clear(); tmp = "";
	dfs_getword(root);
}
void MYprint() {
	freopen("result.txt", "w", stdout);
	cout << "character: " << efchar << endl;
	cout << "words: " << efword << endl;
	cout << "lines: " << efline << endl;
	for (it = qur.begin(); it != qur.end(); it++) {
		cout << "<" << it->second << ">: " << -it->first << endl;
	}
}
void init() {
	root.gs = 0;
}
int main(int argc, char *argv[])
{
	init();
	MYscanf(argv[1]);
	efchar = CountChar();
	efword = CountWord();
	CountMxWord();
	MYprint();
	return 0;
}