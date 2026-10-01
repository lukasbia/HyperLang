#include "hyper/lib/Parse/Lexer.h"
#include <iostream>
using namespace hyper::parse;
static int failures=0;
static void check(bool v,const char*n){if(!v){std::cerr<<"FAIL "<<n<<"\n";++failures;}}
int main(){
  { Lexer l("=="); auto t=l.lex(); check(t.kind==TokenKind::EqualEqual,"operator_0"); check(t.text=="==","operator_text_0"); check(l.lex().isEOF(),"operator_eof_0"); }
  { Lexer l("x==y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_0"); check(b.kind==TokenKind::EqualEqual,"operator_middle_0"); check(c.kind==TokenKind::Identifier,"operator_right_0"); }
  { Lexer l("!="); auto t=l.lex(); check(t.kind==TokenKind::BangEqual,"operator_1"); check(t.text=="!=","operator_text_1"); check(l.lex().isEOF(),"operator_eof_1"); }
  { Lexer l("x!=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_1"); check(b.kind==TokenKind::BangEqual,"operator_middle_1"); check(c.kind==TokenKind::Identifier,"operator_right_1"); }
  { Lexer l("<="); auto t=l.lex(); check(t.kind==TokenKind::LessEqual,"operator_2"); check(t.text=="<=","operator_text_2"); check(l.lex().isEOF(),"operator_eof_2"); }
  { Lexer l("x<=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_2"); check(b.kind==TokenKind::LessEqual,"operator_middle_2"); check(c.kind==TokenKind::Identifier,"operator_right_2"); }
  { Lexer l(">="); auto t=l.lex(); check(t.kind==TokenKind::GreaterEqual,"operator_3"); check(t.text==">=","operator_text_3"); check(l.lex().isEOF(),"operator_eof_3"); }
  { Lexer l("x>=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_3"); check(b.kind==TokenKind::GreaterEqual,"operator_middle_3"); check(c.kind==TokenKind::Identifier,"operator_right_3"); }
  { Lexer l("+="); auto t=l.lex(); check(t.kind==TokenKind::PlusEqual,"operator_4"); check(t.text=="+=","operator_text_4"); check(l.lex().isEOF(),"operator_eof_4"); }
  { Lexer l("x+=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_4"); check(b.kind==TokenKind::PlusEqual,"operator_middle_4"); check(c.kind==TokenKind::Identifier,"operator_right_4"); }
  { Lexer l("-="); auto t=l.lex(); check(t.kind==TokenKind::MinusEqual,"operator_5"); check(t.text=="-=","operator_text_5"); check(l.lex().isEOF(),"operator_eof_5"); }
  { Lexer l("x-=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_5"); check(b.kind==TokenKind::MinusEqual,"operator_middle_5"); check(c.kind==TokenKind::Identifier,"operator_right_5"); }
  { Lexer l("*="); auto t=l.lex(); check(t.kind==TokenKind::StarEqual,"operator_6"); check(t.text=="*=","operator_text_6"); check(l.lex().isEOF(),"operator_eof_6"); }
  { Lexer l("x*=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_6"); check(b.kind==TokenKind::StarEqual,"operator_middle_6"); check(c.kind==TokenKind::Identifier,"operator_right_6"); }
  { Lexer l("/="); auto t=l.lex(); check(t.kind==TokenKind::SlashEqual,"operator_7"); check(t.text=="/=","operator_text_7"); check(l.lex().isEOF(),"operator_eof_7"); }
  { Lexer l("x/=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_7"); check(b.kind==TokenKind::SlashEqual,"operator_middle_7"); check(c.kind==TokenKind::Identifier,"operator_right_7"); }
  { Lexer l("%="); auto t=l.lex(); check(t.kind==TokenKind::PercentEqual,"operator_8"); check(t.text=="%=","operator_text_8"); check(l.lex().isEOF(),"operator_eof_8"); }
  { Lexer l("x%=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_8"); check(b.kind==TokenKind::PercentEqual,"operator_middle_8"); check(c.kind==TokenKind::Identifier,"operator_right_8"); }
  { Lexer l("&&"); auto t=l.lex(); check(t.kind==TokenKind::AmpAmp,"operator_9"); check(t.text=="&&","operator_text_9"); check(l.lex().isEOF(),"operator_eof_9"); }
  { Lexer l("x&&y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_9"); check(b.kind==TokenKind::AmpAmp,"operator_middle_9"); check(c.kind==TokenKind::Identifier,"operator_right_9"); }
  { Lexer l("||"); auto t=l.lex(); check(t.kind==TokenKind::PipePipe,"operator_10"); check(t.text=="||","operator_text_10"); check(l.lex().isEOF(),"operator_eof_10"); }
  { Lexer l("x||y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_10"); check(b.kind==TokenKind::PipePipe,"operator_middle_10"); check(c.kind==TokenKind::Identifier,"operator_right_10"); }
  { Lexer l("->"); auto t=l.lex(); check(t.kind==TokenKind::Arrow,"operator_11"); check(t.text=="->","operator_text_11"); check(l.lex().isEOF(),"operator_eof_11"); }
  { Lexer l("x->y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_11"); check(b.kind==TokenKind::Arrow,"operator_middle_11"); check(c.kind==TokenKind::Identifier,"operator_right_11"); }
  { Lexer l("=>"); auto t=l.lex(); check(t.kind==TokenKind::FatArrow,"operator_12"); check(t.text=="=>","operator_text_12"); check(l.lex().isEOF(),"operator_eof_12"); }
  { Lexer l("x=>y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_12"); check(b.kind==TokenKind::FatArrow,"operator_middle_12"); check(c.kind==TokenKind::Identifier,"operator_right_12"); }
  { Lexer l(".."); auto t=l.lex(); check(t.kind==TokenKind::Range,"operator_13"); check(t.text=="..","operator_text_13"); check(l.lex().isEOF(),"operator_eof_13"); }
  { Lexer l("x..y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_13"); check(b.kind==TokenKind::Range,"operator_middle_13"); check(c.kind==TokenKind::Identifier,"operator_right_13"); }
  { Lexer l("..."); auto t=l.lex(); check(t.kind==TokenKind::Ellipsis,"operator_14"); check(t.text=="...","operator_text_14"); check(l.lex().isEOF(),"operator_eof_14"); }
  { Lexer l("x...y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_14"); check(b.kind==TokenKind::Ellipsis,"operator_middle_14"); check(c.kind==TokenKind::Identifier,"operator_right_14"); }
  { Lexer l("..="); auto t=l.lex(); check(t.kind==TokenKind::RangeInclusive,"operator_15"); check(t.text=="..=","operator_text_15"); check(l.lex().isEOF(),"operator_eof_15"); }
  { Lexer l("x..=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_15"); check(b.kind==TokenKind::RangeInclusive,"operator_middle_15"); check(c.kind==TokenKind::Identifier,"operator_right_15"); }
  { Lexer l("??"); auto t=l.lex(); check(t.kind==TokenKind::NullCoalescing,"operator_16"); check(t.text=="??","operator_text_16"); check(l.lex().isEOF(),"operator_eof_16"); }
  { Lexer l("x??y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_16"); check(b.kind==TokenKind::NullCoalescing,"operator_middle_16"); check(c.kind==TokenKind::Identifier,"operator_right_16"); }
  { Lexer l("?."); auto t=l.lex(); check(t.kind==TokenKind::QuestionDot,"operator_17"); check(t.text=="?.","operator_text_17"); check(l.lex().isEOF(),"operator_eof_17"); }
  { Lexer l("x?.y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_17"); check(b.kind==TokenKind::QuestionDot,"operator_middle_17"); check(c.kind==TokenKind::Identifier,"operator_right_17"); }
  { Lexer l("!"); auto t=l.lex(); check(t.kind==TokenKind::Bang,"operator_18"); check(t.text=="!","operator_text_18"); check(l.lex().isEOF(),"operator_eof_18"); }
  { Lexer l("x!y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_18"); check(b.kind==TokenKind::Bang,"operator_middle_18"); check(c.kind==TokenKind::Identifier,"operator_right_18"); }
  { Lexer l("="); auto t=l.lex(); check(t.kind==TokenKind::Equal,"operator_19"); check(t.text=="=","operator_text_19"); check(l.lex().isEOF(),"operator_eof_19"); }
  { Lexer l("x=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_19"); check(b.kind==TokenKind::Equal,"operator_middle_19"); check(c.kind==TokenKind::Identifier,"operator_right_19"); }
  { Lexer l("+"); auto t=l.lex(); check(t.kind==TokenKind::Plus,"operator_20"); check(t.text=="+","operator_text_20"); check(l.lex().isEOF(),"operator_eof_20"); }
  { Lexer l("x+y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_20"); check(b.kind==TokenKind::Plus,"operator_middle_20"); check(c.kind==TokenKind::Identifier,"operator_right_20"); }
  { Lexer l("-"); auto t=l.lex(); check(t.kind==TokenKind::Minus,"operator_21"); check(t.text=="-","operator_text_21"); check(l.lex().isEOF(),"operator_eof_21"); }
  { Lexer l("x-y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_21"); check(b.kind==TokenKind::Minus,"operator_middle_21"); check(c.kind==TokenKind::Identifier,"operator_right_21"); }
  { Lexer l("*"); auto t=l.lex(); check(t.kind==TokenKind::Star,"operator_22"); check(t.text=="*","operator_text_22"); check(l.lex().isEOF(),"operator_eof_22"); }
  { Lexer l("x*y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_22"); check(b.kind==TokenKind::Star,"operator_middle_22"); check(c.kind==TokenKind::Identifier,"operator_right_22"); }
  { Lexer l("/"); auto t=l.lex(); check(t.kind==TokenKind::Slash,"operator_23"); check(t.text=="/","operator_text_23"); check(l.lex().isEOF(),"operator_eof_23"); }
  { Lexer l("x/y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_23"); check(b.kind==TokenKind::Slash,"operator_middle_23"); check(c.kind==TokenKind::Identifier,"operator_right_23"); }
  { Lexer l("%"); auto t=l.lex(); check(t.kind==TokenKind::Percent,"operator_24"); check(t.text=="%","operator_text_24"); check(l.lex().isEOF(),"operator_eof_24"); }
  { Lexer l("x%y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_24"); check(b.kind==TokenKind::Percent,"operator_middle_24"); check(c.kind==TokenKind::Identifier,"operator_right_24"); }
  { Lexer l("&"); auto t=l.lex(); check(t.kind==TokenKind::Ampersand,"operator_25"); check(t.text=="&","operator_text_25"); check(l.lex().isEOF(),"operator_eof_25"); }
  { Lexer l("x&y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_25"); check(b.kind==TokenKind::Ampersand,"operator_middle_25"); check(c.kind==TokenKind::Identifier,"operator_right_25"); }
  { Lexer l("|"); auto t=l.lex(); check(t.kind==TokenKind::Pipe,"operator_26"); check(t.text=="|","operator_text_26"); check(l.lex().isEOF(),"operator_eof_26"); }
  { Lexer l("x|y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_26"); check(b.kind==TokenKind::Pipe,"operator_middle_26"); check(c.kind==TokenKind::Identifier,"operator_right_26"); }
  { Lexer l("^"); auto t=l.lex(); check(t.kind==TokenKind::Caret,"operator_27"); check(t.text=="^","operator_text_27"); check(l.lex().isEOF(),"operator_eof_27"); }
  { Lexer l("x^y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_27"); check(b.kind==TokenKind::Caret,"operator_middle_27"); check(c.kind==TokenKind::Identifier,"operator_right_27"); }
  { Lexer l("~"); auto t=l.lex(); check(t.kind==TokenKind::Tilde,"operator_28"); check(t.text=="~","operator_text_28"); check(l.lex().isEOF(),"operator_eof_28"); }
  { Lexer l("x~y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_28"); check(b.kind==TokenKind::Tilde,"operator_middle_28"); check(c.kind==TokenKind::Identifier,"operator_right_28"); }
  { Lexer l("<"); auto t=l.lex(); check(t.kind==TokenKind::Less,"operator_29"); check(t.text=="<","operator_text_29"); check(l.lex().isEOF(),"operator_eof_29"); }
  { Lexer l("x<y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_29"); check(b.kind==TokenKind::Less,"operator_middle_29"); check(c.kind==TokenKind::Identifier,"operator_right_29"); }
  { Lexer l(">"); auto t=l.lex(); check(t.kind==TokenKind::Greater,"operator_30"); check(t.text==">","operator_text_30"); check(l.lex().isEOF(),"operator_eof_30"); }
  { Lexer l("x>y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_30"); check(b.kind==TokenKind::Greater,"operator_middle_30"); check(c.kind==TokenKind::Identifier,"operator_right_30"); }
  { Lexer l("++"); auto t=l.lex(); check(t.kind==TokenKind::Increment,"operator_31"); check(t.text=="++","operator_text_31"); check(l.lex().isEOF(),"operator_eof_31"); }
  { Lexer l("x++y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_31"); check(b.kind==TokenKind::Increment,"operator_middle_31"); check(c.kind==TokenKind::Identifier,"operator_right_31"); }
  { Lexer l("--"); auto t=l.lex(); check(t.kind==TokenKind::Decrement,"operator_32"); check(t.text=="--","operator_text_32"); check(l.lex().isEOF(),"operator_eof_32"); }
  { Lexer l("x--y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_32"); check(b.kind==TokenKind::Decrement,"operator_middle_32"); check(c.kind==TokenKind::Identifier,"operator_right_32"); }
  { Lexer l("<<"); auto t=l.lex(); check(t.kind==TokenKind::ShiftLeft,"operator_33"); check(t.text=="<<","operator_text_33"); check(l.lex().isEOF(),"operator_eof_33"); }
  { Lexer l("x<<y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_33"); check(b.kind==TokenKind::ShiftLeft,"operator_middle_33"); check(c.kind==TokenKind::Identifier,"operator_right_33"); }
  { Lexer l(">>"); auto t=l.lex(); check(t.kind==TokenKind::ShiftRight,"operator_34"); check(t.text==">>","operator_text_34"); check(l.lex().isEOF(),"operator_eof_34"); }
  { Lexer l("x>>y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_34"); check(b.kind==TokenKind::ShiftRight,"operator_middle_34"); check(c.kind==TokenKind::Identifier,"operator_right_34"); }
  { Lexer l("<<="); auto t=l.lex(); check(t.kind==TokenKind::ShiftLeftEqual,"operator_35"); check(t.text=="<<=","operator_text_35"); check(l.lex().isEOF(),"operator_eof_35"); }
  { Lexer l("x<<=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_35"); check(b.kind==TokenKind::ShiftLeftEqual,"operator_middle_35"); check(c.kind==TokenKind::Identifier,"operator_right_35"); }
  { Lexer l(">>="); auto t=l.lex(); check(t.kind==TokenKind::ShiftRightEqual,"operator_36"); check(t.text==">>=","operator_text_36"); check(l.lex().isEOF(),"operator_eof_36"); }
  { Lexer l("x>>=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_36"); check(b.kind==TokenKind::ShiftRightEqual,"operator_middle_36"); check(c.kind==TokenKind::Identifier,"operator_right_36"); }
  { Lexer l("+="); auto t=l.lex(); check(t.kind==TokenKind::PlusEqual,"operator_37"); check(t.text=="+=","operator_text_37"); check(l.lex().isEOF(),"operator_eof_37"); }
  { Lexer l("x+=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_37"); check(b.kind==TokenKind::PlusEqual,"operator_middle_37"); check(c.kind==TokenKind::Identifier,"operator_right_37"); }
  { Lexer l("-="); auto t=l.lex(); check(t.kind==TokenKind::MinusEqual,"operator_38"); check(t.text=="-=","operator_text_38"); check(l.lex().isEOF(),"operator_eof_38"); }
  { Lexer l("x-=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_38"); check(b.kind==TokenKind::MinusEqual,"operator_middle_38"); check(c.kind==TokenKind::Identifier,"operator_right_38"); }
  { Lexer l("=="); auto t=l.lex(); check(t.kind==TokenKind::EqualEqual,"operator_39"); check(t.text=="==","operator_text_39"); check(l.lex().isEOF(),"operator_eof_39"); }
  { Lexer l("x==y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_39"); check(b.kind==TokenKind::EqualEqual,"operator_middle_39"); check(c.kind==TokenKind::Identifier,"operator_right_39"); }
  { Lexer l("!="); auto t=l.lex(); check(t.kind==TokenKind::BangEqual,"operator_40"); check(t.text=="!=","operator_text_40"); check(l.lex().isEOF(),"operator_eof_40"); }
  { Lexer l("x!=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_40"); check(b.kind==TokenKind::BangEqual,"operator_middle_40"); check(c.kind==TokenKind::Identifier,"operator_right_40"); }
  { Lexer l("<="); auto t=l.lex(); check(t.kind==TokenKind::LessEqual,"operator_41"); check(t.text=="<=","operator_text_41"); check(l.lex().isEOF(),"operator_eof_41"); }
  { Lexer l("x<=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_41"); check(b.kind==TokenKind::LessEqual,"operator_middle_41"); check(c.kind==TokenKind::Identifier,"operator_right_41"); }
  { Lexer l(">="); auto t=l.lex(); check(t.kind==TokenKind::GreaterEqual,"operator_42"); check(t.text==">=","operator_text_42"); check(l.lex().isEOF(),"operator_eof_42"); }
  { Lexer l("x>=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_42"); check(b.kind==TokenKind::GreaterEqual,"operator_middle_42"); check(c.kind==TokenKind::Identifier,"operator_right_42"); }
  { Lexer l("+="); auto t=l.lex(); check(t.kind==TokenKind::PlusEqual,"operator_43"); check(t.text=="+=","operator_text_43"); check(l.lex().isEOF(),"operator_eof_43"); }
  { Lexer l("x+=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_43"); check(b.kind==TokenKind::PlusEqual,"operator_middle_43"); check(c.kind==TokenKind::Identifier,"operator_right_43"); }
  { Lexer l("-="); auto t=l.lex(); check(t.kind==TokenKind::MinusEqual,"operator_44"); check(t.text=="-=","operator_text_44"); check(l.lex().isEOF(),"operator_eof_44"); }
  { Lexer l("x-=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_44"); check(b.kind==TokenKind::MinusEqual,"operator_middle_44"); check(c.kind==TokenKind::Identifier,"operator_right_44"); }
  { Lexer l("*="); auto t=l.lex(); check(t.kind==TokenKind::StarEqual,"operator_45"); check(t.text=="*=","operator_text_45"); check(l.lex().isEOF(),"operator_eof_45"); }
  { Lexer l("x*=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_45"); check(b.kind==TokenKind::StarEqual,"operator_middle_45"); check(c.kind==TokenKind::Identifier,"operator_right_45"); }
  { Lexer l("/="); auto t=l.lex(); check(t.kind==TokenKind::SlashEqual,"operator_46"); check(t.text=="/=","operator_text_46"); check(l.lex().isEOF(),"operator_eof_46"); }
  { Lexer l("x/=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_46"); check(b.kind==TokenKind::SlashEqual,"operator_middle_46"); check(c.kind==TokenKind::Identifier,"operator_right_46"); }
  { Lexer l("%="); auto t=l.lex(); check(t.kind==TokenKind::PercentEqual,"operator_47"); check(t.text=="%=","operator_text_47"); check(l.lex().isEOF(),"operator_eof_47"); }
  { Lexer l("x%=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_47"); check(b.kind==TokenKind::PercentEqual,"operator_middle_47"); check(c.kind==TokenKind::Identifier,"operator_right_47"); }
  { Lexer l("&&"); auto t=l.lex(); check(t.kind==TokenKind::AmpAmp,"operator_48"); check(t.text=="&&","operator_text_48"); check(l.lex().isEOF(),"operator_eof_48"); }
  { Lexer l("x&&y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_48"); check(b.kind==TokenKind::AmpAmp,"operator_middle_48"); check(c.kind==TokenKind::Identifier,"operator_right_48"); }
  { Lexer l("||"); auto t=l.lex(); check(t.kind==TokenKind::PipePipe,"operator_49"); check(t.text=="||","operator_text_49"); check(l.lex().isEOF(),"operator_eof_49"); }
  { Lexer l("x||y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_49"); check(b.kind==TokenKind::PipePipe,"operator_middle_49"); check(c.kind==TokenKind::Identifier,"operator_right_49"); }
  { Lexer l("->"); auto t=l.lex(); check(t.kind==TokenKind::Arrow,"operator_50"); check(t.text=="->","operator_text_50"); check(l.lex().isEOF(),"operator_eof_50"); }
  { Lexer l("x->y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_50"); check(b.kind==TokenKind::Arrow,"operator_middle_50"); check(c.kind==TokenKind::Identifier,"operator_right_50"); }
  { Lexer l("=>"); auto t=l.lex(); check(t.kind==TokenKind::FatArrow,"operator_51"); check(t.text=="=>","operator_text_51"); check(l.lex().isEOF(),"operator_eof_51"); }
  { Lexer l("x=>y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_51"); check(b.kind==TokenKind::FatArrow,"operator_middle_51"); check(c.kind==TokenKind::Identifier,"operator_right_51"); }
  { Lexer l(".."); auto t=l.lex(); check(t.kind==TokenKind::Range,"operator_52"); check(t.text=="..","operator_text_52"); check(l.lex().isEOF(),"operator_eof_52"); }
  { Lexer l("x..y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_52"); check(b.kind==TokenKind::Range,"operator_middle_52"); check(c.kind==TokenKind::Identifier,"operator_right_52"); }
  { Lexer l("..."); auto t=l.lex(); check(t.kind==TokenKind::Ellipsis,"operator_53"); check(t.text=="...","operator_text_53"); check(l.lex().isEOF(),"operator_eof_53"); }
  { Lexer l("x...y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_53"); check(b.kind==TokenKind::Ellipsis,"operator_middle_53"); check(c.kind==TokenKind::Identifier,"operator_right_53"); }
  { Lexer l("..="); auto t=l.lex(); check(t.kind==TokenKind::RangeInclusive,"operator_54"); check(t.text=="..=","operator_text_54"); check(l.lex().isEOF(),"operator_eof_54"); }
  { Lexer l("x..=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_54"); check(b.kind==TokenKind::RangeInclusive,"operator_middle_54"); check(c.kind==TokenKind::Identifier,"operator_right_54"); }
  { Lexer l("??"); auto t=l.lex(); check(t.kind==TokenKind::NullCoalescing,"operator_55"); check(t.text=="??","operator_text_55"); check(l.lex().isEOF(),"operator_eof_55"); }
  { Lexer l("x??y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_55"); check(b.kind==TokenKind::NullCoalescing,"operator_middle_55"); check(c.kind==TokenKind::Identifier,"operator_right_55"); }
  { Lexer l("?."); auto t=l.lex(); check(t.kind==TokenKind::QuestionDot,"operator_56"); check(t.text=="?.","operator_text_56"); check(l.lex().isEOF(),"operator_eof_56"); }
  { Lexer l("x?.y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_56"); check(b.kind==TokenKind::QuestionDot,"operator_middle_56"); check(c.kind==TokenKind::Identifier,"operator_right_56"); }
  { Lexer l("!"); auto t=l.lex(); check(t.kind==TokenKind::Bang,"operator_57"); check(t.text=="!","operator_text_57"); check(l.lex().isEOF(),"operator_eof_57"); }
  { Lexer l("x!y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_57"); check(b.kind==TokenKind::Bang,"operator_middle_57"); check(c.kind==TokenKind::Identifier,"operator_right_57"); }
  { Lexer l("="); auto t=l.lex(); check(t.kind==TokenKind::Equal,"operator_58"); check(t.text=="=","operator_text_58"); check(l.lex().isEOF(),"operator_eof_58"); }
  { Lexer l("x=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_58"); check(b.kind==TokenKind::Equal,"operator_middle_58"); check(c.kind==TokenKind::Identifier,"operator_right_58"); }
  { Lexer l("+"); auto t=l.lex(); check(t.kind==TokenKind::Plus,"operator_59"); check(t.text=="+","operator_text_59"); check(l.lex().isEOF(),"operator_eof_59"); }
  { Lexer l("x+y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_59"); check(b.kind==TokenKind::Plus,"operator_middle_59"); check(c.kind==TokenKind::Identifier,"operator_right_59"); }
  { Lexer l("-"); auto t=l.lex(); check(t.kind==TokenKind::Minus,"operator_60"); check(t.text=="-","operator_text_60"); check(l.lex().isEOF(),"operator_eof_60"); }
  { Lexer l("x-y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_60"); check(b.kind==TokenKind::Minus,"operator_middle_60"); check(c.kind==TokenKind::Identifier,"operator_right_60"); }
  { Lexer l("*"); auto t=l.lex(); check(t.kind==TokenKind::Star,"operator_61"); check(t.text=="*","operator_text_61"); check(l.lex().isEOF(),"operator_eof_61"); }
  { Lexer l("x*y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_61"); check(b.kind==TokenKind::Star,"operator_middle_61"); check(c.kind==TokenKind::Identifier,"operator_right_61"); }
  { Lexer l("/"); auto t=l.lex(); check(t.kind==TokenKind::Slash,"operator_62"); check(t.text=="/","operator_text_62"); check(l.lex().isEOF(),"operator_eof_62"); }
  { Lexer l("x/y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_62"); check(b.kind==TokenKind::Slash,"operator_middle_62"); check(c.kind==TokenKind::Identifier,"operator_right_62"); }
  { Lexer l("%"); auto t=l.lex(); check(t.kind==TokenKind::Percent,"operator_63"); check(t.text=="%","operator_text_63"); check(l.lex().isEOF(),"operator_eof_63"); }
  { Lexer l("x%y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_63"); check(b.kind==TokenKind::Percent,"operator_middle_63"); check(c.kind==TokenKind::Identifier,"operator_right_63"); }
  { Lexer l("&"); auto t=l.lex(); check(t.kind==TokenKind::Ampersand,"operator_64"); check(t.text=="&","operator_text_64"); check(l.lex().isEOF(),"operator_eof_64"); }
  { Lexer l("x&y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_64"); check(b.kind==TokenKind::Ampersand,"operator_middle_64"); check(c.kind==TokenKind::Identifier,"operator_right_64"); }
  { Lexer l("|"); auto t=l.lex(); check(t.kind==TokenKind::Pipe,"operator_65"); check(t.text=="|","operator_text_65"); check(l.lex().isEOF(),"operator_eof_65"); }
  { Lexer l("x|y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_65"); check(b.kind==TokenKind::Pipe,"operator_middle_65"); check(c.kind==TokenKind::Identifier,"operator_right_65"); }
  { Lexer l("^"); auto t=l.lex(); check(t.kind==TokenKind::Caret,"operator_66"); check(t.text=="^","operator_text_66"); check(l.lex().isEOF(),"operator_eof_66"); }
  { Lexer l("x^y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_66"); check(b.kind==TokenKind::Caret,"operator_middle_66"); check(c.kind==TokenKind::Identifier,"operator_right_66"); }
  { Lexer l("~"); auto t=l.lex(); check(t.kind==TokenKind::Tilde,"operator_67"); check(t.text=="~","operator_text_67"); check(l.lex().isEOF(),"operator_eof_67"); }
  { Lexer l("x~y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_67"); check(b.kind==TokenKind::Tilde,"operator_middle_67"); check(c.kind==TokenKind::Identifier,"operator_right_67"); }
  { Lexer l("<"); auto t=l.lex(); check(t.kind==TokenKind::Less,"operator_68"); check(t.text=="<","operator_text_68"); check(l.lex().isEOF(),"operator_eof_68"); }
  { Lexer l("x<y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_68"); check(b.kind==TokenKind::Less,"operator_middle_68"); check(c.kind==TokenKind::Identifier,"operator_right_68"); }
  { Lexer l(">"); auto t=l.lex(); check(t.kind==TokenKind::Greater,"operator_69"); check(t.text==">","operator_text_69"); check(l.lex().isEOF(),"operator_eof_69"); }
  { Lexer l("x>y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_69"); check(b.kind==TokenKind::Greater,"operator_middle_69"); check(c.kind==TokenKind::Identifier,"operator_right_69"); }
  { Lexer l("++"); auto t=l.lex(); check(t.kind==TokenKind::Increment,"operator_70"); check(t.text=="++","operator_text_70"); check(l.lex().isEOF(),"operator_eof_70"); }
  { Lexer l("x++y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_70"); check(b.kind==TokenKind::Increment,"operator_middle_70"); check(c.kind==TokenKind::Identifier,"operator_right_70"); }
  { Lexer l("--"); auto t=l.lex(); check(t.kind==TokenKind::Decrement,"operator_71"); check(t.text=="--","operator_text_71"); check(l.lex().isEOF(),"operator_eof_71"); }
  { Lexer l("x--y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_71"); check(b.kind==TokenKind::Decrement,"operator_middle_71"); check(c.kind==TokenKind::Identifier,"operator_right_71"); }
  { Lexer l("<<"); auto t=l.lex(); check(t.kind==TokenKind::ShiftLeft,"operator_72"); check(t.text=="<<","operator_text_72"); check(l.lex().isEOF(),"operator_eof_72"); }
  { Lexer l("x<<y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_72"); check(b.kind==TokenKind::ShiftLeft,"operator_middle_72"); check(c.kind==TokenKind::Identifier,"operator_right_72"); }
  { Lexer l(">>"); auto t=l.lex(); check(t.kind==TokenKind::ShiftRight,"operator_73"); check(t.text==">>","operator_text_73"); check(l.lex().isEOF(),"operator_eof_73"); }
  { Lexer l("x>>y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_73"); check(b.kind==TokenKind::ShiftRight,"operator_middle_73"); check(c.kind==TokenKind::Identifier,"operator_right_73"); }
  { Lexer l("<<="); auto t=l.lex(); check(t.kind==TokenKind::ShiftLeftEqual,"operator_74"); check(t.text=="<<=","operator_text_74"); check(l.lex().isEOF(),"operator_eof_74"); }
  { Lexer l("x<<=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_74"); check(b.kind==TokenKind::ShiftLeftEqual,"operator_middle_74"); check(c.kind==TokenKind::Identifier,"operator_right_74"); }
  { Lexer l(">>="); auto t=l.lex(); check(t.kind==TokenKind::ShiftRightEqual,"operator_75"); check(t.text==">>=","operator_text_75"); check(l.lex().isEOF(),"operator_eof_75"); }
  { Lexer l("x>>=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_75"); check(b.kind==TokenKind::ShiftRightEqual,"operator_middle_75"); check(c.kind==TokenKind::Identifier,"operator_right_75"); }
  { Lexer l("+="); auto t=l.lex(); check(t.kind==TokenKind::PlusEqual,"operator_76"); check(t.text=="+=","operator_text_76"); check(l.lex().isEOF(),"operator_eof_76"); }
  { Lexer l("x+=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_76"); check(b.kind==TokenKind::PlusEqual,"operator_middle_76"); check(c.kind==TokenKind::Identifier,"operator_right_76"); }
  { Lexer l("-="); auto t=l.lex(); check(t.kind==TokenKind::MinusEqual,"operator_77"); check(t.text=="-=","operator_text_77"); check(l.lex().isEOF(),"operator_eof_77"); }
  { Lexer l("x-=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_77"); check(b.kind==TokenKind::MinusEqual,"operator_middle_77"); check(c.kind==TokenKind::Identifier,"operator_right_77"); }
  { Lexer l("=="); auto t=l.lex(); check(t.kind==TokenKind::EqualEqual,"operator_78"); check(t.text=="==","operator_text_78"); check(l.lex().isEOF(),"operator_eof_78"); }
  { Lexer l("x==y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_78"); check(b.kind==TokenKind::EqualEqual,"operator_middle_78"); check(c.kind==TokenKind::Identifier,"operator_right_78"); }
  { Lexer l("!="); auto t=l.lex(); check(t.kind==TokenKind::BangEqual,"operator_79"); check(t.text=="!=","operator_text_79"); check(l.lex().isEOF(),"operator_eof_79"); }
  { Lexer l("x!=y"); auto a=l.lex(); auto b=l.lex(); auto c=l.lex(); check(a.kind==TokenKind::Identifier,"operator_left_79"); check(b.kind==TokenKind::BangEqual,"operator_middle_79"); check(c.kind==TokenKind::Identifier,"operator_right_79"); }

  return failures?1:0;
}
