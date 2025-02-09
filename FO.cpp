#include "FO.h"

pub enum Token {
	EOF,
	#[token(r#"("#)]
	L1,
	#[regex(r#"[a-zA-Z]+"#)]
	L2,
	#[token(r#"|"#)]
	L3,
	#[token(r#"&"#)]
	L4,
	#[token(r#"->"#)]
	L5,
	#[token(r#")"#)]
	L6,
	#[token(r#"<->"#)]
	L7,
	#[token(r#"="#)]
	L8,
	#[token(r#","#)]
	L9,
	#[token(r#"\e"#)]
	L10,
	#[token(r#"\a"#)]
	L11,
	#[token(r#"!"#)]
	L12,
};
long action[][16]  = {
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{-16, 0, 0, 0, 0, 73, 0, 3, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{-13, 0, 0, 71, 5, -13, 0, -13, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 67, 65},
		{0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 9, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 11, 51, 19, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 21, 13, -15, -15, -15, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 22, 14, 46},
		{0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 16, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 11, 18, 19, 0, 0, 0, 0, 0},
		{0, 0, 0, -2, -2, -2, -2, -2, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 21, 13, -13, -13, -13, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 22, 14, 46},
		{0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 16, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, -1, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0},
		{0, 0, 0, -3, -3, -3, -3, -3, 0, 0, 0, 0, 0},
		{0, 27, 0, 0, 0, 0, 0, 0, -8, 0, 0, 0, 0},
		{0, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, -1, 0, 0, 0, 0, -1, 0, 0, -1, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 37, 0, 0, 30, 0, 0, 0},
		{0, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, -6, 0, 0, -6, 0, 0, 0},
		{0, 33, 0, 0, 0, 0, -8, 0, 0, -8, 0, 0, 0},
		{0, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, -9, 0, 0, -9, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 36, 0, 0, 30, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, -7, 0, 0, -7, 0, 0, 0},
		{0, 0, 0, -4, -4, -4, -4, -4, -7, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 39, 0, 0, 0, 0},
		{0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, -1, 0, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0},
		{0, 0, 0, -5, -5, -5, -5, -5, 0, 0, 0, 0, 0},
		{0, 43, 0, -8, -8, -8, -8, -8, 0, 0, 0, 0, 0},
		{0, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 45, 0, 0, 30, 0, 0, 0},
		{0, 0, 0, -7, -7, -7, -7, -7, 0, 0, 0, 0, 0},
		{0, 16, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, -10, -10, -10, -10, -10, 0, 0, 0, 0, 0},
		{0, 0, 0, -11, -11, -11, -11, -11, 0, 0, 0, 0, 0},
		{0, 0, 0, -12, -12, -12, -12, -12, 0, 0, 0, 0, 0},
		{0, 0, 0, -14, -14, -14, -14, -14, 0, 0, 0, 0, 0},
		{-2, 0, 0, -2, -2, -2, 0, -2, 0, 0, 0, 0, 0},
		{-3, 0, 0, -3, -3, -3, 0, -3, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0, 0},
		{0, 0, 55, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{-1, -1, 0, -1, -1, -1, 0, -1, 0, 0, 0, 0, 0},
		{-8, 57, 0, -8, -8, -8, 0, -8, 0, 0, 0, 0, 0},
		{0, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 59, 0, 0, 30, 0, 0, 0},
		{-7, 0, 0, -7, -7, -7, 0, -7, 0, 0, 0, 0, 0},
		{-5, 0, 0, -5, -5, -5, 0, -5, 0, 0, 0, 0, 0},
		{0, 62, 0, 0, 0, 0, 0, 0, -8, 0, 0, 0, 0},
		{0, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 64, 0, 0, 30, 0, 0, 0},
		{-4, 0, 0, -4, -4, -4, 0, -4, -7, 0, 0, 0, 0},
		{0, 9, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{-10, 0, 0, -10, -10, -10, 0, -10, 0, 0, 0, 0, 0},
		{0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 9, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{-12, 0, 0, -12, -12, -12, 0, -12, 0, 0, 0, 0, 0},
		{-14, 0, 0, -14, -14, -14, 0, -14, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 67, 65},
		{-11, 0, 0, -11, -11, -11, 0, -11, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{-15, 0, 0, 71, 5, -15, 0, -15, 0, 0, 0, 0, 0},
	};

long gotos[][16] = {
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 69, 0, 0, 0, 0, 0, 0, 69, 0, 69, 0, 0, 0, 0},
		{7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{60, 51, 0, 51, 51, 0, 52, 52, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 9, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 0, 0, 11, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 49, 0, 0, 0, 0, 0, 0, 49, 0, 49, 0, 0, 0, 0},
		{14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{25, 48, 0, 48, 48, 0, 37, 37, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 0, 16, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 0, 0, 19, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 47, 0, 0, 0, 0, 0, 0, 47, 0, 47, 0, 0, 0, 0},
		{22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{25, 24, 0, 24, 24, 0, 37, 37, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{31, 0, 0, 0, 0, 28, 33, 33, 28, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{31, 0, 0, 0, 0, 0, 30, 30, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{31, 0, 0, 0, 0, 34, 33, 33, 34, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{41, 0, 0, 0, 0, 0, 40, 40, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{31, 0, 0, 0, 0, 43, 33, 33, 43, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{25, 46, 0, 46, 46, 0, 37, 37, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{55, 0, 0, 0, 0, 0, 59, 59, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{31, 0, 0, 0, 0, 57, 33, 33, 57, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{31, 0, 0, 0, 0, 62, 33, 33, 62, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{60, 65, 0, 65, 65, 0, 52, 52, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{67, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{60, 68, 0, 68, 68, 0, 52, 52, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 71, 0, 0, 0, 0, 0, 0, 71, 0, 71, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 73, 0, 0, 73, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	};

	fn reduction2(mut var: str, mut a: Formula) -> Formula {EXISTS}
	fn reduction3(mut name: str, mut args: vecTerms) -> Formula {"rel"}
	fn reduction4(mut a: Term, mut b: Term) -> Formula {"eq"}
	fn reduction6(mut name: str, mut args: vecTerms) -> Term {"func"}
	fn reduction7(mut name: str) -> Term {"const"}
	fn reduction9(mut a: Formula) -> Formula {"not a"}
	fn reduction10(mut a: Formula, mut b: Formula) -> Formula {"or"}
	fn reduction11(mut var: str, mut a: Formula) -> Formula {FORALL}
	fn reduction12(mut a: Formula, mut b: Formula) -> Formula {"biimpl"}
	fn reduction13(mut a: Formula, mut b: Formula) -> Formula {"and"}
	fn reduction14(mut a: Formula, mut b: Formula) -> Formula {"impl"}

    fn parse(lex: logos::Lexer<'a, Token>) ->  {
        let mut parser = Self{
            parse_stack: vec![],
            state_stack: vec![0],
            lexer: lex
        };

        let mut token = match parser.lexer.next() {
            Some(Ok(t)) => t as usize,
            Some(Err(e)) => panic!("{:?}", e),
            None => 0
        };

        while parser.state_stack.len()>0 {
            let state = *parser.state_stack.last().unwrap();
            println!("stack: {:?}", parser.parse_stack);
            println!("stack: {:?}", parser.state_stack);
            println!("got: {}:{}", state, token.clone() as usize);
            let task = Self::ACTION[state][token];
            println!("task: {}", task);
            match task {
                0 => break,
			-1 => {}
			-2 => {}
			-3 => {
				let a2 = pop!(parser, T2);
 				let _ = parser.state_stack.pop();
				let a1 = pop!(parser, T3);
 				let _ = parser.state_stack.pop();
				let _ = parser.parse_stack.pop();
				let _ = parser.state_stack.pop();
				push!(parser, T2, Self::reduction2(a1, a2));
			}
			-4 => {
				let _ = parser.parse_stack.pop();
				let _ = parser.state_stack.pop();
				let a2 = pop!(parser, T4);
 				let _ = parser.state_stack.pop();
				let _ = parser.parse_stack.pop();
				let _ = parser.state_stack.pop();
				let a0 = pop!(parser, T3);
 				let _ = parser.state_stack.pop();
				push!(parser, T2, Self::reduction3(a0, a2));
			}
			-5 => {
				let a2 = pop!(parser, T5);
 				let _ = parser.state_stack.pop();
				let _ = parser.parse_stack.pop();
				let _ = parser.state_stack.pop();
				let a0 = pop!(parser, T5);
 				let _ = parser.state_stack.pop();
				push!(parser, T2, Self::reduction4(a0, a2));
			}
			-6 => {}
			-7 => {
				let _ = parser.parse_stack.pop();
				let _ = parser.state_stack.pop();
				let a2 = pop!(parser, T4);
 				let _ = parser.state_stack.pop();
				let _ = parser.parse_stack.pop();
				let _ = parser.state_stack.pop();
				let a0 = pop!(parser, T3);
 				let _ = parser.state_stack.pop();
				push!(parser, T5, Self::reduction6(a0, a2));
			}
			-8 => {
				let a0 = pop!(parser, T3);
 				let _ = parser.state_stack.pop();
				push!(parser, T5, Self::reduction7(a0));
			}
			-9 => {}
			-10 => {
				let a1 = pop!(parser, T2);
 				let _ = parser.state_stack.pop();
				let _ = parser.parse_stack.pop();
				let _ = parser.state_stack.pop();
				push!(parser, T2, Self::reduction9(a1));
			}
			-11 => {
				let a2 = pop!(parser, T2);
 				let _ = parser.state_stack.pop();
				let _ = parser.parse_stack.pop();
				let _ = parser.state_stack.pop();
				let a0 = pop!(parser, T2);
 				let _ = parser.state_stack.pop();
				push!(parser, T2, Self::reduction10(a0, a2));
			}
			-12 => {
				let a2 = pop!(parser, T2);
 				let _ = parser.state_stack.pop();
				let a1 = pop!(parser, T3);
 				let _ = parser.state_stack.pop();
				let _ = parser.parse_stack.pop();
				let _ = parser.state_stack.pop();
				push!(parser, T2, Self::reduction11(a1, a2));
			}
			-13 => {
				let a2 = pop!(parser, T2);
 				let _ = parser.state_stack.pop();
				let _ = parser.parse_stack.pop();
				let _ = parser.state_stack.pop();
				let a0 = pop!(parser, T2);
 				let _ = parser.state_stack.pop();
				push!(parser, T2, Self::reduction12(a0, a2));
			}
			-14 => {
				let a2 = pop!(parser, T2);
 				let _ = parser.state_stack.pop();
				let _ = parser.parse_stack.pop();
				let _ = parser.state_stack.pop();
				let a0 = pop!(parser, T2);
 				let _ = parser.state_stack.pop();
				push!(parser, T2, Self::reduction13(a0, a2));
			}
			-15 => {
				let a2 = pop!(parser, T2);
 				let _ = parser.state_stack.pop();
				let _ = parser.parse_stack.pop();
				let _ = parser.state_stack.pop();
				let a0 = pop!(parser, T2);
 				let _ = parser.state_stack.pop();
				push!(parser, T2, Self::reduction14(a0, a2));
			}
			-16 => {}

                new_state @ _ => {
                    parser.state_stack.push((new_state-1) as usize);
                    push!(parser, T1, parser.lexer.slice());
                    token = match parser.lexer.next() {
                        Some(Ok(t)) => t as usize,
            Some(Err(e)) =>{
                let mut line=0;
                let mut offset=0;
                let span = parser.lexer.span();
                for c in parser.lexer.source()[0..span.end].chars(){
                    if(c=='\n'){
                        offset=0;
                        line+=1;
                    }
                }

                panic!("Unexpected Token {:?} ({:?}) at {}:{}", e, parser.lexer.slice(), line, offset);
            },
                        None => 0
                    };
                    continue;
                }
            }
            let mut i = parser.state_stack.len();
            while i>0 {
                i-=1;
                let prev = *parser.state_stack.get(i).unwrap();
                let next = Self::GOTO[prev][-(task+1) as usize];
                if next!=0 {
                    parser.state_stack.push(next);
                    break
                }
            }
            if i<0{break}
        }
        if parser.state_stack.len() != 1 {
            panic!("Parsing failed! {:?} {:?}", parser.parse_stack, parser.state_stack);
        } else {
            match parser.parse_stack.into_iter().nth(0).unwrap() {
                Types::T6(s) => s,
                t@ _ => panic!("Parsing failed! {:?}", t)

            }
        }
    }
}
