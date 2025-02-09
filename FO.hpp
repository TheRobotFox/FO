#ifndef FO_H_
#define FO_H_

#include <concepts>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include <concepts>


template<typename S>
concept Signature = requires(S sig){
    typename S::T;
    {sig.relations} -> std::same_as<std::unordered_map<std::string, std::pair<std::function<bool(std::vector<const typename S::T*>)>, int>>&>;
    {sig.functions} -> std::same_as<std::unordered_map<std::string, std::pair<std::function<typename S::T*(std::vector<const typename S::T*>)>, int>>&>;
    {sig.constants} -> std::same_as<std::unordered_map<std::string, typename S::T*>&>;
};

template<class S>
struct Term
{
    using T = typename S::T;
    enum Type {
        NONE, ERROR, FUNCTION, VAR
    } type;
    std::string name;
    std::vector<Term<S>> data;

    const T* eval(S &s, std::unordered_map<std::string, const T*> env) const
    {
        switch (type) {
            case VAR:
            {
                if(env.contains(name)) return env[name];
                if(s.constants.contains(name)) return s.constants[name];

                std::cout << "Variable/Constant " << name << " not defined!";
                return nullptr;
            }
            case FUNCTION:
            {
                std::vector<const T*> res;
                if(!s.functions.contains(name)){
                    std::cout << "Function " << name << " not defined!";
                    return nullptr;
                }
                auto &[fn, ar] = s.functions[name];
                if(data.size()!=ar){
                    std::cout << "Syntax error Wrong arity " << data.size() << "/" << ar << "\n";
                    exit(-1);
                }
                for(const Term<S> &t : data)
                    res.push_back(t.eval(s, env));
                return fn(res);
            }
        }
    std::cout << "Unreacable!\n";
    exit(-1);
    }
};

template<class S>
struct Formula
{
    using T = typename S::T;
    enum Type {
        NONE, ERROR, EXISTS, FORALL, NOT, AND, OR, IF, IFF, REL, EQ
    } type;
    struct Q {
            Formula<S> *a;
            std::string var;
    };
    struct J {
            Formula<S> *a,*b;
    };
    struct Eq {
        Term<S> a,b;
    };
    struct Rel {
        std::string name;
        std::vector<Term<S>> terms;
    };
    std::variant<Formula<S>*, Q,J,Eq,Rel> data;
    bool eval(S &s, std::unordered_map<std::string, const T*> env) const
    {
        switch (type) {
            case EXISTS:
            {
                auto &q = std::get<1>(data);
                for(T& e : s){
                    env[q.var] = &e;
                    if(q.a->eval(s, env)) return true;
                }
                return false;
            }
            case FORALL:
            {
                auto &q = std::get<1>(data);
                for(T& e : s){
                    env[q.var] = &e;
                    if(!q.a->eval(s, env)) return false;
                }
                return true;
            }
            case NOT: return !std::get<0>(data)->eval(s, env);
            case OR:{
                auto &d = std::get<2>(data);
                return d.a->eval(s,env) || d.b->eval(s, env);
            }
            case AND:{
                auto &d = std::get<2>(data);
                return d.a->eval(s,env) && d.b->eval(s, env);
            }
            case IF:{
                auto &d = std::get<2>(data);
                return !d.a->eval(s,env) || d.b->eval(s, env);
            }
            case IFF:{
                auto &d = std::get<2>(data);
                return d.a->eval(s,env) == d.b->eval(s, env);
            }
            case REL:{
                auto &d = std::get<4>(data);
                std::vector<const T*> res;
                if(!s.relations.contains(d.name)){
                    std::cout << "Structure does not implement Relation " << d.name << "\n";
                    exit(-1);
                }
                auto &[fn, ar] = s.relations[d.name];
                if(d.terms.size()!=ar){
                    std::cout << "Syntax error Wrong arity " << d.terms.size() << "/" << ar << "\n";
                    exit(-1);
                }
                for(const Term<S> &t : d.terms)
                    res.push_back(t.eval(s, env));
                return fn(res);
            }
            case EQ:{
                auto &d = std::get<3>(data);
                return d.a.eval(s,env)==d.b.eval(s,env);
            }
        }
    }
};

class Node;

template<class S>
concept Structure = Signature<S> && std::ranges::range<typename S::T>;

struct GraphSignature
{
    virtual auto E(const Node &a, const Node &b) -> bool = 0;
    std::unordered_map<std::string, std::pair<std::function<bool(std::vector<const Node*>)>, int>> relations;
    GraphSignature(){

        relations["E"] = {[this](std::vector<const Node*> elems){return this->E(*elems[0],*elems[1]);}, 2};
    }
};


template<class S>
using BoxFormula = Formula<S>*;
template<class S>
using vecTerms = std::vector<Term<S>>;


#endif // FO_H_
