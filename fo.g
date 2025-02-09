start: F0 -> BoxFormula<S>;
F0: F0=a "->" F1=b {return new Formula<S>(Formula<S>::IF,typename Formula<S>::J(a,b));}
  | F0=a "<->" F1=b {return new Formula<S>(Formula<S>::IFF,typename Formula<S>::J(a,b));}
  | F1=a {return a;}
  -> BoxFormula<S>;
F1: F1=a "&" F2=b {return new Formula<S>(Formula<S>::AND,typename Formula<S>::J(a,b));}
  | F1=a "|" F2=b {return new Formula<S>(Formula<S>::OR,typename Formula<S>::J(a,b));}
  | F2=a {return a;}
  -> BoxFormula<S>;

F2: "\e" id=var F2=a {return new Formula<S>(Formula<S>::EXISTS,typename Formula<S>::Q(a,std::string{var}));}
  | "\a" id=var F2=a {return new Formula<S>(Formula<S>::FORALL,typename Formula<S>::Q(a,std::string{var}));}
  | "!" F2=a {return new Formula<S>(Formula<S>::NOT,a);}
  | F3=a {return a;}
  -> BoxFormula<S>;
F3: "(" F0=a ")" {return a;}
  | id=name "(" LIST=args ")" {return new Formula<S>(Formula<S>::REL, typename Formula<S>::Rel(std::string{name},args));}
  | Term=a "=" Term=b {return new Formula<S>(Formula<S>::EQ, typename Formula<S>::Eq(a,b));}
  -> BoxFormula<S>;

id: r"[a-zA-Z]+" -> string_view;

LIST: Term=t {return {t};}
    | LIST=l "," Term=t {l.push_back(t); return l; }
    -> vecTerms<S>;

Term: id=name "(" LIST=args ")" {return Term(Term<S>::FUNCTION, std::string{name}, args);}
    | id=name {return Term<S>(Term<S>::VAR, std::string{name});}
    -> Term<S>;
