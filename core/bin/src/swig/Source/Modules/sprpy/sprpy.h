#ifndef SPRPY_H
#define SPRPY_H

#include<string>
#include<vector>
#include<set>
using namespace std;

#define ALERT(node_or_str,message) printf("(Alert) %s : " #message "\n", DohCheck(node_or_str) ? Char(Getattr((DOH*)node_or_str,"name")): (char*) node_or_str )

namespace SwigSprpy{;
	
//	enum field
struct EnumItem{
	string name;
	string value;
};
struct Enum{
	string name;
	string fullname;
	std::vector<EnumItem> fields;
};
bool operator < (const Enum& a, const Enum& b);

struct Field{
	string type;
	string name;
};
typedef std::vector<Field> Fields;

typedef std::vector<string> Strings;
typedef std::vector<Node*> Nodes;


#pragma region ÉOÉçÅ[ÉoÉãä÷êî
void FindNodeR(Nodes& rv, Node* n, char* type);
void FindNode(Nodes& rv, Node* n, char* type);

std::string TrimSuffix(DOHString* s, char* suffix);
std::string Trim(char* pre, DOHString* s, char* suf);
std::string GetString(DOHString* s);
std::string DecodeType(DOHString* n, std::string& post = string());
void GetBaseList(Strings& rv, DOH* baseList, char* suffix);

#pragma endregion

}
#endif