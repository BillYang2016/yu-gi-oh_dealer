#include<iostream>
#include<fstream>
#include<cstring>
#include<vector>
#include<map>

using namespace std;

int deck = 0, type = 0;

int used[100] = {0}, Belong[100], cnt = 0, total = 0;
map<int, int> Map, Hash;
vector<map<int, pair<char, int>> > requireSet;

bool Check() {
	for(auto require: requireSet) {
		bool flag = true;
		for(auto it: require) {
			int id = it.first;
			pair<char, int> pii = it.second;
			char splitChar = pii.first;
			int amount = pii.second;
			if(splitChar == '<') {
				if(used[id] >= amount) {
					flag = false;
					break;
				}
			} else if(splitChar == '>') {
				if(used[id] <= amount) {
					flag = false;
					break;
				}
			} else {
				if(used[id] != amount) {
					flag = false;
					break;
				}
			}
		}
		if(flag) return true;
	}
	return false;
}

void Dfs(int Depth, int last) {
	if(Depth > 5) {
		total ++;
		if(Check()) cnt ++;
		return;
	}
	for(int i = last + 1; i <= deck; i ++) {
		used[Belong[i]] ++;
		Dfs(Depth + 1, i);
		used[Belong[i]] --;
	}
}

vector<string> stringSplit(const string &str,const char &pattern) {
	vector<std::string> resVec;
	if ("" == str)return resVec;
	string strs = str + pattern;
	size_t pos = strs.find(pattern);
	size_t size = strs.size();
	while (pos != string::npos){
		string x = strs.substr(0,pos);
		resVec.push_back(x);
		strs = strs.substr(pos+1,size);
		pos = strs.find(pattern);
	}
	return resVec;
}

int main() {
	ifstream fin;
	fin.open("deck.ydk");
	bool flag = 0;
	Map.clear();
	Hash.clear();
	requireSet.clear();
	while(!fin.eof()) {
		string line;
		getline(fin, line);
		if(!isdigit(line[0])) {
			if(flag) break; //主卡组已结束
			else continue; //否则跳过
		}
		flag = 1;
		int id = atoi(line.c_str());
		Map[id] ++;
	}
	fin.close();

	for(auto it: Map) {
		type ++;
		for(int i = deck + 1; i <= deck + it.second; i ++) Belong[i] = type;
		deck += it.second;
		Hash[it.first] = type; //记录下对应关系
	}

	fin.open("require.txt");
	while(!fin.eof()) {
		string line;
		getline(fin, line);
		vector<string> splitedLine = stringSplit(line, ',');
		map<int, pair<char, int> > require;
		require.clear();
		for(string card: splitedLine) {
			char splitChar = '?';
			if(card.find('<') != card.npos)splitChar = '<';
			if(card.find('>') != card.npos) {
				if(splitChar != '?') {
					cout << card << " 逻辑不清，程序停止运行！\n";
					return 0;
				}
				splitChar = '>';
			}
			if(card.find('=') != card.npos) {
				if(splitChar != '?') {
					cout << card << " 逻辑不清，程序停止运行！\n";
					return 0;
				}
				splitChar = '=';
			}
			if(splitChar == '?') {
				cout << card << " 格式错误，程序停止运行！\n";
				return 0;
			}
			
			vector<string> splitedCard = stringSplit(card, splitChar);
			if(splitedCard.size() != 2) {
				cout << card << " 格式错误，程序停止运行！\n";
				return 0;
			}

			auto it = Hash.find(atoi(splitedCard[0].c_str()));
			int amount = atoi(splitedCard[1].c_str());

			if(it != Hash.end()) {
				int id = it -> second;
				require[id] = make_pair(splitChar, amount);
			} else {
				cout << card << " 不在主卡组内，程序停止运行！\n";
				return 0;
			}
		}
		requireSet.push_back(require);
	}
	fin.close();

	Dfs(1, 0);

	cout<< cnt << " / " << total << " = " << 1.0 * cnt / total << endl;
	
	getchar();

	return 0;
}