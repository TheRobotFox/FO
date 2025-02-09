#include "FO.hpp"
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <iterator>
#include <variant>
using std::string_view;
struct Token {
    enum Kind {
		//EOF
		Tok0=0,
		//Regex: "[a-zA-Z]+"
		Tok1=1,
		//Token: "\a"
		Tok2=2,
		//Token: "\e"
		Tok3=3,
		//Token: "!"
		Tok4=4,
		//Token: "("
		Tok5=5,
		//Token: "="
		Tok6=6,
		//Token: ")"
		Tok7=7,
		//Token: ","
		Tok8=8,
		//Token: "->"
		Tok9=9,
		//Token: "&"
		Tok10=10,
		//Token: "<->"
		Tok11=11,
		//Token: "|"
		Tok12=12,
	} kind;
	std::string_view data;
};

auto read_token(std::vector<Token> &v, std::string_view str) -> int
{
    if(str[0]==' ') return 1;
	if(str.starts_with("\\a")){v.emplace_back(Token::Tok2);return strlen("\\a");}
	if(str.starts_with("\\e")){v.emplace_back(Token::Tok3);return strlen("\\e");}
	if(str.starts_with("!")){v.emplace_back(Token::Tok4);return strlen("!");}
	if(str.starts_with("(")){v.emplace_back(Token::Tok5);return strlen("(");}
	if(str.starts_with("=")){v.emplace_back(Token::Tok6);return strlen("=");}
	if(str.starts_with(")")){v.emplace_back(Token::Tok7);return strlen(")");}
	if(str.starts_with(",")){v.emplace_back(Token::Tok8);return strlen(",");}
	if(str.starts_with("->")){v.emplace_back(Token::Tok9);return strlen("->");}
	if(str.starts_with("&")){v.emplace_back(Token::Tok10);return strlen("&");}
	if(str.starts_with("<->")){v.emplace_back(Token::Tok11);return strlen("<->");}
	if(str.starts_with("|")){v.emplace_back(Token::Tok12);return strlen("|");}

    const auto *iter = str.begin();
    while((isalnum(*iter) != 0) || *iter=='_') iter++;
    v.emplace_back(Token::Tok1, std::string_view(str.begin(),iter));
    return iter-str.begin();
}
auto lex(std::string_view inp) -> std::vector<Token>
{
    std::vector<Token> v;

    while(inp.length()){
        int res = read_token(v, inp);
        if(res<1){
            std::cout << "Error while Lexing: " << inp << "\n";
            return {};
        }
        inp = inp.substr(res);
    }
    v.emplace_back(Token::Tok0);
    return v;
}
const char* to_str[] = {
	[Token::Tok0] = "EOF",
	[Token::Tok1] = "Regex(r\"[a-zA-Z]+\")",
	[Token::Tok2] = "Token(\"\a\")",
	[Token::Tok3] = "Token(\"\e\")",
	[Token::Tok4] = "Token(\"!\")",
	[Token::Tok5] = "Token(\"(\")",
	[Token::Tok6] = "Token(\"=\")",
	[Token::Tok7] = "Token(\")\")",
	[Token::Tok8] = "Token(\",\")",
	[Token::Tok9] = "Token(\"->\")",
	[Token::Tok10] = "Token(\"&\")",
	[Token::Tok11] = "Token(\"<->\")",
	[Token::Tok12] = "Token(\"|\")",

};
template<class S>
class Parser
{
	auto reduction1(string_view name, vecTerms<S> args) -> Term<S> {return Term(Term<S>::FUNCTION, std::string{name}, args);} 
	auto reduction2(string_view name, vecTerms<S> args) -> BoxFormula<S> {return new Formula<S>(Formula<S>::REL, typename Formula<S>::Rel(std::string{name},args));} 
	auto reduction3(vecTerms<S> l, Term<S> t) -> vecTerms<S> {l.push_back(t); return l; } 
	auto reduction4(string_view name) -> Term<S> {return Term<S>(Term<S>::VAR, std::string{name});} 
	auto reduction5(Term<S> t) -> vecTerms<S> {return {t};} 
	auto reduction6(string_view var, BoxFormula<S> a) -> BoxFormula<S> {return new Formula<S>(Formula<S>::EXISTS,typename Formula<S>::Q(a,std::string{var}));} 
	auto reduction7(BoxFormula<S> a) -> BoxFormula<S> {return a;} 
	auto reduction8(string_view var, BoxFormula<S> a) -> BoxFormula<S> {return new Formula<S>(Formula<S>::FORALL,typename Formula<S>::Q(a,std::string{var}));} 
	auto reduction9(Term<S> a, Term<S> b) -> BoxFormula<S> {return new Formula<S>(Formula<S>::EQ, typename Formula<S>::Eq(a,b));} 
	auto reduction10(BoxFormula<S> a) -> BoxFormula<S> {return a;} 
	auto reduction11(BoxFormula<S> a) -> BoxFormula<S> {return new Formula<S>(Formula<S>::NOT,a);} 
	auto reduction12(BoxFormula<S> a, BoxFormula<S> b) -> BoxFormula<S> {return new Formula<S>(Formula<S>::OR,typename Formula<S>::J(a,b));} 
	auto reduction13(BoxFormula<S> a, BoxFormula<S> b) -> BoxFormula<S> {return new Formula<S>(Formula<S>::AND,typename Formula<S>::J(a,b));} 
	auto reduction14(BoxFormula<S> a, BoxFormula<S> b) -> BoxFormula<S> {return new Formula<S>(Formula<S>::IFF,typename Formula<S>::J(a,b));} 
	auto reduction15(BoxFormula<S> a) -> BoxFormula<S> {return a;} 
	auto reduction16(BoxFormula<S> a, BoxFormula<S> b) -> BoxFormula<S> {return new Formula<S>(Formula<S>::IF,typename Formula<S>::J(a,b));} 
	auto reduction17(BoxFormula<S> a) -> BoxFormula<S> {return a;} 
	using Type = std::variant<vecTerms<S>,Term<S>,BoxFormula<S>,std::string_view>;
	std::vector<Type> data_stack;
	std::vector<long> state_stack;
	const long actions[80][13]= {
		{0, 7, 2, 52, 5, 6, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 7, 2, 52, 5, 6, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 7, 2, 52, 5, 6, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 7, 10, 12, 9, 8, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0}, 
		{0, 7, 10, 12, 9, 8, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 7, 10, 12, 9, 8, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 7, 10, 12, 9, 8, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 7, 10, 12, 9, 8, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 15, -5, 0, 0, 0, 0, 0, 0}, 
		{0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, -1, 0, -1, -1, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 18, 19, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, -2, -3, 0, -3, -3, -3, -3}, 
		{0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, -4, -4, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 22, 0, -5, -5, 0, 0, 0, 0}, 
		{0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, -6, -6, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 25, 19, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, -2, -2, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, -7, 0, -7, -7, -7, -7}, 
		{0, 0, 0, 0, 0, 0, 28, 0, 0, 0, 0, 0, 0}, 
		{0, 29, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, -1, 0, -1, 0, -1, -1, -1, -1}, 
		{0, 0, 0, 0, 0, 0, 0, -10, 0, -10, -10, -10, -10}, 
		{0, 0, 0, 0, 0, 32, 0, -5, 0, -5, -5, -5, -5}, 
		{0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 34, 19, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, -2, 0, -2, -2, -2, -2}, 
		{0, 0, 0, 0, 0, 0, 0, -8, 0, -8, -8, -8, -8}, 
		{0, 0, 0, 0, 0, 0, 0, -9, 0, -9, -9, -9, -9}, 
		{0, 0, 0, 0, 0, 0, 0, -12, 0, -12, -12, -12, -12}, 
		{0, 0, 0, 0, 0, 0, 0, 46, 0, 47, 0, 39, 0}, 
		{0, 7, 10, 12, 9, 8, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, -15, 0, -15, 43, -15, 41}, 
		{0, 7, 10, 12, 9, 8, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, -13, 0, -13, -13, -13, -13}, 
		{0, 7, 10, 12, 9, 8, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, -14, 0, -14, -14, -14, -14}, 
		{0, 0, 0, 0, 0, 0, 0, -16, 0, -16, -16, -16, -16}, 
		{0, 0, 0, 0, 0, 0, 0, -11, 0, -11, -11, -11, -11}, 
		{0, 7, 10, 12, 9, 8, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, -17, 0, -17, 43, -17, 41}, 
		{0, 0, 0, 0, 0, 0, 0, -18, 0, -18, 43, -18, 41}, 
		{0, 0, 0, 0, 0, 0, 0, 51, 0, 47, 0, 39, 0}, 
		{-11, 0, 0, 0, 0, 0, 0, 0, 0, -11, -11, -11, -11}, 
		{0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 7, 2, 52, 5, 6, 0, 0, 0, 0, 0, 0, 0}, 
		{-7, 0, 0, 0, 0, 0, 0, 0, 0, -7, -7, -7, -7}, 
		{-8, 0, 0, 0, 0, 0, 0, 0, 0, -8, -8, -8, -8}, 
		{0, 0, 0, 0, 0, 57, -5, 0, 0, 0, 0, 0, 0}, 
		{0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 59, 19, 0, 0, 0, 0}, 
		{-3, 0, 0, 0, 0, 0, -2, 0, 0, -3, -3, -3, -3}, 
		{0, 0, 0, 0, 0, 0, 61, 0, 0, 0, 0, 0, 0}, 
		{0, 62, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{-1, 0, 0, 0, 0, -1, 0, 0, 0, -1, -1, -1, -1}, 
		{-10, 0, 0, 0, 0, 0, 0, 0, 0, -10, -10, -10, -10}, 
		{-5, 0, 0, 0, 0, 65, 0, 0, 0, -5, -5, -5, -5}, 
		{0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 67, 19, 0, 0, 0, 0}, 
		{-2, 0, 0, 0, 0, 0, 0, 0, 0, -2, -2, -2, -2}, 
		{-12, 0, 0, 0, 0, 0, 0, 0, 0, -12, -12, -12, -12}, 
		{-9, 0, 0, 0, 0, 0, 0, 0, 0, -9, -9, -9, -9}, 
		{-16, 0, 0, 0, 0, 0, 0, 0, 0, -16, -16, -16, -16}, 
		{-18, 0, 0, 0, 0, 0, 0, 0, 0, -18, 72, -18, 74}, 
		{0, 7, 2, 52, 5, 6, 0, 0, 0, 0, 0, 0, 0}, 
		{-14, 0, 0, 0, 0, 0, 0, 0, 0, -14, -14, -14, -14}, 
		{0, 7, 2, 52, 5, 6, 0, 0, 0, 0, 0, 0, 0}, 
		{-13, 0, 0, 0, 0, 0, 0, 0, 0, -13, -13, -13, -13}, 
		{-19, 0, 0, 0, 0, 0, 0, 0, 0, 79, 0, 77, 0}, 
		{0, 7, 2, 52, 5, 6, 0, 0, 0, 0, 0, 0, 0}, 
		{-15, 0, 0, 0, 0, 0, 0, 0, 0, -15, 72, -15, 74}, 
		{0, 7, 2, 52, 5, 6, 0, 0, 0, 0, 0, 0, 0}, 
		{-17, 0, 0, 0, 0, 0, 0, 0, 0, -17, 72, -17, 74}, 
	};

	const long gotos[80][19] = {
		{55, 59, 54, 0, 59, 0, 69, 69, 69, 54, 54, 69, 70, 70, 75, 70, 75, 75, 0}, 
		{3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{55, 59, 54, 0, 59, 0, 68, 68, 68, 54, 54, 68, 0, 0, 0, 0, 0, 0, 0}, 
		{55, 59, 54, 0, 59, 0, 67, 67, 67, 54, 54, 67, 0, 0, 0, 0, 0, 0, 0}, 
		{13, 26, 34, 0, 26, 0, 44, 44, 44, 34, 34, 44, 48, 48, 49, 48, 49, 49, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{13, 26, 34, 0, 26, 0, 44, 44, 44, 34, 34, 44, 48, 48, 37, 48, 37, 37, 0}, 
		{13, 26, 34, 0, 26, 0, 36, 36, 36, 34, 34, 36, 0, 0, 0, 0, 0, 0, 0}, 
		{10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{13, 26, 34, 0, 26, 0, 35, 35, 35, 34, 34, 35, 0, 0, 0, 0, 0, 0, 0}, 
		{12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{13, 26, 34, 0, 26, 0, 25, 25, 25, 34, 34, 25, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{20, 22, 0, 16, 22, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{20, 19, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{20, 22, 0, 23, 22, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{30, 29, 0, 0, 29, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{20, 22, 0, 32, 22, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{13, 26, 34, 0, 26, 0, 44, 44, 44, 34, 34, 44, 39, 39, 0, 39, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{13, 26, 34, 0, 26, 0, 41, 41, 41, 34, 34, 41, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{13, 26, 34, 0, 26, 0, 43, 43, 43, 34, 34, 43, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{13, 26, 34, 0, 26, 0, 44, 44, 44, 34, 34, 44, 47, 47, 0, 47, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{52, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{55, 59, 54, 0, 59, 0, 53, 53, 53, 54, 54, 53, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{20, 22, 0, 57, 22, 57, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{63, 62, 0, 0, 62, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{20, 22, 0, 65, 22, 65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{55, 59, 54, 0, 59, 0, 72, 72, 72, 54, 54, 72, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{55, 59, 54, 0, 59, 0, 74, 74, 74, 54, 54, 74, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{55, 59, 54, 0, 59, 0, 69, 69, 69, 54, 54, 69, 77, 77, 0, 77, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{55, 59, 54, 0, 59, 0, 69, 69, 69, 54, 54, 69, 79, 79, 0, 79, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
	};


public:
    template<std::ranges::range R>
    auto parse(R tokens) -> BoxFormula<S>
    {
        auto start = tokens.begin();
        auto end = tokens.end();
        if(start==end) return {};

        state_stack = {0};
        data_stack.clear();

        auto token = *start++;

        while(!state_stack.empty()) {
            long state = state_stack.back();
            long task = actions[state][token.kind];
            switch(task){
            case 0: goto stop;
			case -1: break;
			case -2:{
				data_stack.pop_back();
				state_stack.pop_back();
				auto a2 = std::get<vecTerms<S>>(data_stack.back()); data_stack.pop_back();
 				state_stack.pop_back();
				data_stack.pop_back();
				state_stack.pop_back();
				auto a0 = std::get<string_view>(data_stack.back()); data_stack.pop_back();
 				state_stack.pop_back();
				data_stack.emplace_back(reduction1(std::move(a0), std::move(a2)));
			}
				break;
			case -3:{
				data_stack.pop_back();
				state_stack.pop_back();
				auto a2 = std::get<vecTerms<S>>(data_stack.back()); data_stack.pop_back();
 				state_stack.pop_back();
				data_stack.pop_back();
				state_stack.pop_back();
				auto a0 = std::get<string_view>(data_stack.back()); data_stack.pop_back();
 				state_stack.pop_back();
				data_stack.emplace_back(reduction2(std::move(a0), std::move(a2)));
			}
				break;
			case -4:{
				auto a2 = std::get<Term<S>>(data_stack.back()); data_stack.pop_back();
 				state_stack.pop_back();
				data_stack.pop_back();
				state_stack.pop_back();
				auto a0 = std::get<vecTerms<S>>(data_stack.back()); data_stack.pop_back();
 				state_stack.pop_back();
				data_stack.emplace_back(reduction3(std::move(a0), std::move(a2)));
			}
				break;
			case -5:{
				auto a0 = std::get<string_view>(data_stack.back()); data_stack.pop_back();
 				state_stack.pop_back();
				data_stack.emplace_back(reduction4(std::move(a0)));
			}
				break;
			case -6:{
				auto a0 = std::get<Term<S>>(data_stack.back()); data_stack.pop_back();
 				state_stack.pop_back();
				data_stack.emplace_back(reduction5(std::move(a0)));
			}
				break;
			case -7:{
				auto a2 = std::get<BoxFormula<S>>(data_stack.back()); data_stack.pop_back();
 				state_stack.pop_back();
				auto a1 = std::get<string_view>(data_stack.back()); data_stack.pop_back();
 				state_stack.pop_back();
				data_stack.pop_back();
				state_stack.pop_back();
				data_stack.emplace_back(reduction6(std::move(a1), std::move(a2)));
			}
				break;
			case -8:{
				auto a0 = std::get<BoxFormula<S>>(data_stack.back()); data_stack.pop_back();
 				state_stack.pop_back();
				data_stack.emplace_back(reduction7(std::move(a0)));
			}
				break;
			case -9:{
				auto a2 = std::get<BoxFormula<S>>(data_stack.back()); data_stack.pop_back();
 				state_stack.pop_back();
				auto a1 = std::get<string_view>(data_stack.back()); data_stack.pop_back();
 				state_stack.pop_back();
				data_stack.pop_back();
				state_stack.pop_back();
				data_stack.emplace_back(reduction8(std::move(a1), std::move(a2)));
			}
				break;
			case -10:{
				auto a2 = std::get<Term<S>>(data_stack.back()); data_stack.pop_back();
 				state_stack.pop_back();
				data_stack.pop_back();
				state_stack.pop_back();
				auto a0 = std::get<Term<S>>(data_stack.back()); data_stack.pop_back();
 				state_stack.pop_back();
				data_stack.emplace_back(reduction9(std::move(a0), std::move(a2)));
			}
				break;
			case -11:{
				data_stack.pop_back();
				state_stack.pop_back();
				auto a1 = std::get<BoxFormula<S>>(data_stack.back()); data_stack.pop_back();
 				state_stack.pop_back();
				data_stack.pop_back();
				state_stack.pop_back();
				data_stack.emplace_back(reduction10(std::move(a1)));
			}
				break;
			case -12:{
				auto a1 = std::get<BoxFormula<S>>(data_stack.back()); data_stack.pop_back();
 				state_stack.pop_back();
				data_stack.pop_back();
				state_stack.pop_back();
				data_stack.emplace_back(reduction11(std::move(a1)));
			}
				break;
			case -13:{
				auto a2 = std::get<BoxFormula<S>>(data_stack.back()); data_stack.pop_back();
 				state_stack.pop_back();
				data_stack.pop_back();
				state_stack.pop_back();
				auto a0 = std::get<BoxFormula<S>>(data_stack.back()); data_stack.pop_back();
 				state_stack.pop_back();
				data_stack.emplace_back(reduction12(std::move(a0), std::move(a2)));
			}
				break;
			case -14:{
				auto a2 = std::get<BoxFormula<S>>(data_stack.back()); data_stack.pop_back();
 				state_stack.pop_back();
				data_stack.pop_back();
				state_stack.pop_back();
				auto a0 = std::get<BoxFormula<S>>(data_stack.back()); data_stack.pop_back();
 				state_stack.pop_back();
				data_stack.emplace_back(reduction13(std::move(a0), std::move(a2)));
			}
				break;
			case -15:{
				auto a2 = std::get<BoxFormula<S>>(data_stack.back()); data_stack.pop_back();
 				state_stack.pop_back();
				data_stack.pop_back();
				state_stack.pop_back();
				auto a0 = std::get<BoxFormula<S>>(data_stack.back()); data_stack.pop_back();
 				state_stack.pop_back();
				data_stack.emplace_back(reduction14(std::move(a0), std::move(a2)));
			}
				break;
			case -16:{
				auto a0 = std::get<BoxFormula<S>>(data_stack.back()); data_stack.pop_back();
 				state_stack.pop_back();
				data_stack.emplace_back(reduction15(std::move(a0)));
			}
				break;
			case -17:{
				auto a2 = std::get<BoxFormula<S>>(data_stack.back()); data_stack.pop_back();
 				state_stack.pop_back();
				data_stack.pop_back();
				state_stack.pop_back();
				auto a0 = std::get<BoxFormula<S>>(data_stack.back()); data_stack.pop_back();
 				state_stack.pop_back();
				data_stack.emplace_back(reduction16(std::move(a0), std::move(a2)));
			}
				break;
			case -18:{
				auto a0 = std::get<BoxFormula<S>>(data_stack.back()); data_stack.pop_back();
 				state_stack.pop_back();
				data_stack.emplace_back(reduction17(std::move(a0)));
			}
				break;
			case -19: break;

                default: {
                    state_stack.push_back(task-1);
                    data_stack.emplace_back(token.data);
                    if(start==end) token = {Token::Tok0};
                    token = *start++;
                    continue;
                }
            }
            while(!state_stack.empty()){
                long prev = state_stack.back();
                long next = gotos[prev][-(task+1)];
                if(next!=0){
                    state_stack.push_back(next);
                    break;
                }
                state_stack.pop_back();
            }
        }
stop:
        if(!state_stack.empty()){
            std::cout << "Failed to Parse\n";
        } else {
            return std::move(std::get<BoxFormula<S>>(data_stack.back()));
        }
        return {};
    }
};

