#include "hyper/lib/Parse/Lexer.h"
#include <iostream>
using namespace hyper::parse;
static int failures=0;
static void check(bool v,const char*n){if(!v){std::cerr<<"FAIL "<<n<<"\n";++failures;}}
int main(){
  { Lexer l("café"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_0"); check(t.text=="café","unicode_text_0"); check(l.lex().isEOF(),"unicode_eof_0"); }
  { Lexer l("// comment café\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_0"); }
  { Lexer l("世界"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_1"); check(t.text=="世界","unicode_text_1"); check(l.lex().isEOF(),"unicode_eof_1"); }
  { Lexer l("// comment 世界\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_1"); }
  { Lexer l("Привет"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_2"); check(t.text=="Привет","unicode_text_2"); check(l.lex().isEOF(),"unicode_eof_2"); }
  { Lexer l("// comment Привет\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_2"); }
  { Lexer l("مرحبا"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_3"); check(t.text=="مرحبا","unicode_text_3"); check(l.lex().isEOF(),"unicode_eof_3"); }
  { Lexer l("// comment مرحبا\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_3"); }
  { Lexer l("こんにちは"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_4"); check(t.text=="こんにちは","unicode_text_4"); check(l.lex().isEOF(),"unicode_eof_4"); }
  { Lexer l("// comment こんにちは\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_4"); }
  { Lexer l("한국어"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_5"); check(t.text=="한국어","unicode_text_5"); check(l.lex().isEOF(),"unicode_eof_5"); }
  { Lexer l("// comment 한국어\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_5"); }
  { Lexer l("γειά"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_6"); check(t.text=="γειά","unicode_text_6"); check(l.lex().isEOF(),"unicode_eof_6"); }
  { Lexer l("// comment γειά\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_6"); }
  { Lexer l("שלום"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_7"); check(t.text=="שלום","unicode_text_7"); check(l.lex().isEOF(),"unicode_eof_7"); }
  { Lexer l("// comment שלום\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_7"); }
  { Lexer l("नमस्ते"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_8"); check(t.text=="नमस्ते","unicode_text_8"); check(l.lex().isEOF(),"unicode_eof_8"); }
  { Lexer l("// comment नमस्ते\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_8"); }
  { Lexer l("გამარჯობა"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_9"); check(t.text=="გამარჯობა","unicode_text_9"); check(l.lex().isEOF(),"unicode_eof_9"); }
  { Lexer l("// comment გამარჯობა\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_9"); }
  { Lexer l("café"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_10"); check(t.text=="café","unicode_text_10"); check(l.lex().isEOF(),"unicode_eof_10"); }
  { Lexer l("// comment café\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_10"); }
  { Lexer l("世界"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_11"); check(t.text=="世界","unicode_text_11"); check(l.lex().isEOF(),"unicode_eof_11"); }
  { Lexer l("// comment 世界\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_11"); }
  { Lexer l("Привет"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_12"); check(t.text=="Привет","unicode_text_12"); check(l.lex().isEOF(),"unicode_eof_12"); }
  { Lexer l("// comment Привет\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_12"); }
  { Lexer l("مرحبا"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_13"); check(t.text=="مرحبا","unicode_text_13"); check(l.lex().isEOF(),"unicode_eof_13"); }
  { Lexer l("// comment مرحبا\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_13"); }
  { Lexer l("こんにちは"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_14"); check(t.text=="こんにちは","unicode_text_14"); check(l.lex().isEOF(),"unicode_eof_14"); }
  { Lexer l("// comment こんにちは\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_14"); }
  { Lexer l("한국어"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_15"); check(t.text=="한국어","unicode_text_15"); check(l.lex().isEOF(),"unicode_eof_15"); }
  { Lexer l("// comment 한국어\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_15"); }
  { Lexer l("γειά"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_16"); check(t.text=="γειά","unicode_text_16"); check(l.lex().isEOF(),"unicode_eof_16"); }
  { Lexer l("// comment γειά\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_16"); }
  { Lexer l("שלום"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_17"); check(t.text=="שלום","unicode_text_17"); check(l.lex().isEOF(),"unicode_eof_17"); }
  { Lexer l("// comment שלום\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_17"); }
  { Lexer l("नमस्ते"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_18"); check(t.text=="नमस्ते","unicode_text_18"); check(l.lex().isEOF(),"unicode_eof_18"); }
  { Lexer l("// comment नमस्ते\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_18"); }
  { Lexer l("გამარჯობა"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_19"); check(t.text=="გამარჯობა","unicode_text_19"); check(l.lex().isEOF(),"unicode_eof_19"); }
  { Lexer l("// comment გამარჯობა\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_19"); }
  { Lexer l("café"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_20"); check(t.text=="café","unicode_text_20"); check(l.lex().isEOF(),"unicode_eof_20"); }
  { Lexer l("// comment café\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_20"); }
  { Lexer l("世界"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_21"); check(t.text=="世界","unicode_text_21"); check(l.lex().isEOF(),"unicode_eof_21"); }
  { Lexer l("// comment 世界\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_21"); }
  { Lexer l("Привет"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_22"); check(t.text=="Привет","unicode_text_22"); check(l.lex().isEOF(),"unicode_eof_22"); }
  { Lexer l("// comment Привет\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_22"); }
  { Lexer l("مرحبا"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_23"); check(t.text=="مرحبا","unicode_text_23"); check(l.lex().isEOF(),"unicode_eof_23"); }
  { Lexer l("// comment مرحبا\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_23"); }
  { Lexer l("こんにちは"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_24"); check(t.text=="こんにちは","unicode_text_24"); check(l.lex().isEOF(),"unicode_eof_24"); }
  { Lexer l("// comment こんにちは\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_24"); }
  { Lexer l("한국어"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_25"); check(t.text=="한국어","unicode_text_25"); check(l.lex().isEOF(),"unicode_eof_25"); }
  { Lexer l("// comment 한국어\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_25"); }
  { Lexer l("γειά"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_26"); check(t.text=="γειά","unicode_text_26"); check(l.lex().isEOF(),"unicode_eof_26"); }
  { Lexer l("// comment γειά\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_26"); }
  { Lexer l("שלום"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_27"); check(t.text=="שלום","unicode_text_27"); check(l.lex().isEOF(),"unicode_eof_27"); }
  { Lexer l("// comment שלום\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_27"); }
  { Lexer l("नमस्ते"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_28"); check(t.text=="नमस्ते","unicode_text_28"); check(l.lex().isEOF(),"unicode_eof_28"); }
  { Lexer l("// comment नमस्ते\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_28"); }
  { Lexer l("გამარჯობა"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_29"); check(t.text=="გამარჯობა","unicode_text_29"); check(l.lex().isEOF(),"unicode_eof_29"); }
  { Lexer l("// comment გამარჯობა\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_29"); }
  { Lexer l("café"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_30"); check(t.text=="café","unicode_text_30"); check(l.lex().isEOF(),"unicode_eof_30"); }
  { Lexer l("// comment café\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_30"); }
  { Lexer l("世界"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_31"); check(t.text=="世界","unicode_text_31"); check(l.lex().isEOF(),"unicode_eof_31"); }
  { Lexer l("// comment 世界\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_31"); }
  { Lexer l("Привет"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_32"); check(t.text=="Привет","unicode_text_32"); check(l.lex().isEOF(),"unicode_eof_32"); }
  { Lexer l("// comment Привет\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_32"); }
  { Lexer l("مرحبا"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_33"); check(t.text=="مرحبا","unicode_text_33"); check(l.lex().isEOF(),"unicode_eof_33"); }
  { Lexer l("// comment مرحبا\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_33"); }
  { Lexer l("こんにちは"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_34"); check(t.text=="こんにちは","unicode_text_34"); check(l.lex().isEOF(),"unicode_eof_34"); }
  { Lexer l("// comment こんにちは\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_34"); }
  { Lexer l("한국어"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_35"); check(t.text=="한국어","unicode_text_35"); check(l.lex().isEOF(),"unicode_eof_35"); }
  { Lexer l("// comment 한국어\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_35"); }
  { Lexer l("γειά"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_36"); check(t.text=="γειά","unicode_text_36"); check(l.lex().isEOF(),"unicode_eof_36"); }
  { Lexer l("// comment γειά\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_36"); }
  { Lexer l("שלום"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_37"); check(t.text=="שלום","unicode_text_37"); check(l.lex().isEOF(),"unicode_eof_37"); }
  { Lexer l("// comment שלום\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_37"); }
  { Lexer l("नमस्ते"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_38"); check(t.text=="नमस्ते","unicode_text_38"); check(l.lex().isEOF(),"unicode_eof_38"); }
  { Lexer l("// comment नमस्ते\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_38"); }
  { Lexer l("გამარჯობა"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_39"); check(t.text=="გამარჯობა","unicode_text_39"); check(l.lex().isEOF(),"unicode_eof_39"); }
  { Lexer l("// comment გამარჯობა\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_39"); }
  { Lexer l("café"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_40"); check(t.text=="café","unicode_text_40"); check(l.lex().isEOF(),"unicode_eof_40"); }
  { Lexer l("// comment café\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_40"); }
  { Lexer l("世界"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_41"); check(t.text=="世界","unicode_text_41"); check(l.lex().isEOF(),"unicode_eof_41"); }
  { Lexer l("// comment 世界\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_41"); }
  { Lexer l("Привет"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_42"); check(t.text=="Привет","unicode_text_42"); check(l.lex().isEOF(),"unicode_eof_42"); }
  { Lexer l("// comment Привет\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_42"); }
  { Lexer l("مرحبا"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_43"); check(t.text=="مرحبا","unicode_text_43"); check(l.lex().isEOF(),"unicode_eof_43"); }
  { Lexer l("// comment مرحبا\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_43"); }
  { Lexer l("こんにちは"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_44"); check(t.text=="こんにちは","unicode_text_44"); check(l.lex().isEOF(),"unicode_eof_44"); }
  { Lexer l("// comment こんにちは\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_44"); }
  { Lexer l("한국어"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_45"); check(t.text=="한국어","unicode_text_45"); check(l.lex().isEOF(),"unicode_eof_45"); }
  { Lexer l("// comment 한국어\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_45"); }
  { Lexer l("γειά"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_46"); check(t.text=="γειά","unicode_text_46"); check(l.lex().isEOF(),"unicode_eof_46"); }
  { Lexer l("// comment γειά\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_46"); }
  { Lexer l("שלום"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_47"); check(t.text=="שלום","unicode_text_47"); check(l.lex().isEOF(),"unicode_eof_47"); }
  { Lexer l("// comment שלום\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_47"); }
  { Lexer l("नमस्ते"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_48"); check(t.text=="नमस्ते","unicode_text_48"); check(l.lex().isEOF(),"unicode_eof_48"); }
  { Lexer l("// comment नमस्ते\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_48"); }
  { Lexer l("გამარჯობა"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_49"); check(t.text=="გამარჯობა","unicode_text_49"); check(l.lex().isEOF(),"unicode_eof_49"); }
  { Lexer l("// comment გამარჯობა\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_49"); }
  { Lexer l("café"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_50"); check(t.text=="café","unicode_text_50"); check(l.lex().isEOF(),"unicode_eof_50"); }
  { Lexer l("// comment café\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_50"); }
  { Lexer l("世界"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_51"); check(t.text=="世界","unicode_text_51"); check(l.lex().isEOF(),"unicode_eof_51"); }
  { Lexer l("// comment 世界\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_51"); }
  { Lexer l("Привет"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_52"); check(t.text=="Привет","unicode_text_52"); check(l.lex().isEOF(),"unicode_eof_52"); }
  { Lexer l("// comment Привет\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_52"); }
  { Lexer l("مرحبا"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_53"); check(t.text=="مرحبا","unicode_text_53"); check(l.lex().isEOF(),"unicode_eof_53"); }
  { Lexer l("// comment مرحبا\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_53"); }
  { Lexer l("こんにちは"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_54"); check(t.text=="こんにちは","unicode_text_54"); check(l.lex().isEOF(),"unicode_eof_54"); }
  { Lexer l("// comment こんにちは\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_54"); }
  { Lexer l("한국어"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_55"); check(t.text=="한국어","unicode_text_55"); check(l.lex().isEOF(),"unicode_eof_55"); }
  { Lexer l("// comment 한국어\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_55"); }
  { Lexer l("γειά"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_56"); check(t.text=="γειά","unicode_text_56"); check(l.lex().isEOF(),"unicode_eof_56"); }
  { Lexer l("// comment γειά\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_56"); }
  { Lexer l("שלום"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_57"); check(t.text=="שלום","unicode_text_57"); check(l.lex().isEOF(),"unicode_eof_57"); }
  { Lexer l("// comment שלום\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_57"); }
  { Lexer l("नमस्ते"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_58"); check(t.text=="नमस्ते","unicode_text_58"); check(l.lex().isEOF(),"unicode_eof_58"); }
  { Lexer l("// comment नमस्ते\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_58"); }
  { Lexer l("გამარჯობა"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_59"); check(t.text=="გამარჯობა","unicode_text_59"); check(l.lex().isEOF(),"unicode_eof_59"); }
  { Lexer l("// comment გამარჯობა\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_59"); }
  { Lexer l("café"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_60"); check(t.text=="café","unicode_text_60"); check(l.lex().isEOF(),"unicode_eof_60"); }
  { Lexer l("// comment café\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_60"); }
  { Lexer l("世界"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_61"); check(t.text=="世界","unicode_text_61"); check(l.lex().isEOF(),"unicode_eof_61"); }
  { Lexer l("// comment 世界\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_61"); }
  { Lexer l("Привет"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_62"); check(t.text=="Привет","unicode_text_62"); check(l.lex().isEOF(),"unicode_eof_62"); }
  { Lexer l("// comment Привет\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_62"); }
  { Lexer l("مرحبا"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_63"); check(t.text=="مرحبا","unicode_text_63"); check(l.lex().isEOF(),"unicode_eof_63"); }
  { Lexer l("// comment مرحبا\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_63"); }
  { Lexer l("こんにちは"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_64"); check(t.text=="こんにちは","unicode_text_64"); check(l.lex().isEOF(),"unicode_eof_64"); }
  { Lexer l("// comment こんにちは\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_64"); }
  { Lexer l("한국어"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_65"); check(t.text=="한국어","unicode_text_65"); check(l.lex().isEOF(),"unicode_eof_65"); }
  { Lexer l("// comment 한국어\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_65"); }
  { Lexer l("γειά"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_66"); check(t.text=="γειά","unicode_text_66"); check(l.lex().isEOF(),"unicode_eof_66"); }
  { Lexer l("// comment γειά\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_66"); }
  { Lexer l("שלום"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_67"); check(t.text=="שלום","unicode_text_67"); check(l.lex().isEOF(),"unicode_eof_67"); }
  { Lexer l("// comment שלום\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_67"); }
  { Lexer l("नमस्ते"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_68"); check(t.text=="नमस्ते","unicode_text_68"); check(l.lex().isEOF(),"unicode_eof_68"); }
  { Lexer l("// comment नमस्ते\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_68"); }
  { Lexer l("გამარჯობა"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_69"); check(t.text=="გამარჯობა","unicode_text_69"); check(l.lex().isEOF(),"unicode_eof_69"); }
  { Lexer l("// comment გამარჯობა\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_69"); }
  { Lexer l("café"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_70"); check(t.text=="café","unicode_text_70"); check(l.lex().isEOF(),"unicode_eof_70"); }
  { Lexer l("// comment café\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_70"); }
  { Lexer l("世界"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_71"); check(t.text=="世界","unicode_text_71"); check(l.lex().isEOF(),"unicode_eof_71"); }
  { Lexer l("// comment 世界\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_71"); }
  { Lexer l("Привет"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_72"); check(t.text=="Привет","unicode_text_72"); check(l.lex().isEOF(),"unicode_eof_72"); }
  { Lexer l("// comment Привет\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_72"); }
  { Lexer l("مرحبا"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_73"); check(t.text=="مرحبا","unicode_text_73"); check(l.lex().isEOF(),"unicode_eof_73"); }
  { Lexer l("// comment مرحبا\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_73"); }
  { Lexer l("こんにちは"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_74"); check(t.text=="こんにちは","unicode_text_74"); check(l.lex().isEOF(),"unicode_eof_74"); }
  { Lexer l("// comment こんにちは\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_74"); }
  { Lexer l("한국어"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_75"); check(t.text=="한국어","unicode_text_75"); check(l.lex().isEOF(),"unicode_eof_75"); }
  { Lexer l("// comment 한국어\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_75"); }
  { Lexer l("γειά"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_76"); check(t.text=="γειά","unicode_text_76"); check(l.lex().isEOF(),"unicode_eof_76"); }
  { Lexer l("// comment γειά\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_76"); }
  { Lexer l("שלום"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_77"); check(t.text=="שלום","unicode_text_77"); check(l.lex().isEOF(),"unicode_eof_77"); }
  { Lexer l("// comment שלום\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_77"); }
  { Lexer l("नमस्ते"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_78"); check(t.text=="नमस्ते","unicode_text_78"); check(l.lex().isEOF(),"unicode_eof_78"); }
  { Lexer l("// comment नमस्ते\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_78"); }
  { Lexer l("გამარჯობა"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_79"); check(t.text=="გამარჯობა","unicode_text_79"); check(l.lex().isEOF(),"unicode_eof_79"); }
  { Lexer l("// comment გამარჯობა\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_79"); }
  { Lexer l("café"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_80"); check(t.text=="café","unicode_text_80"); check(l.lex().isEOF(),"unicode_eof_80"); }
  { Lexer l("// comment café\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_80"); }
  { Lexer l("世界"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_81"); check(t.text=="世界","unicode_text_81"); check(l.lex().isEOF(),"unicode_eof_81"); }
  { Lexer l("// comment 世界\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_81"); }
  { Lexer l("Привет"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_82"); check(t.text=="Привет","unicode_text_82"); check(l.lex().isEOF(),"unicode_eof_82"); }
  { Lexer l("// comment Привет\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_82"); }
  { Lexer l("مرحبا"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_83"); check(t.text=="مرحبا","unicode_text_83"); check(l.lex().isEOF(),"unicode_eof_83"); }
  { Lexer l("// comment مرحبا\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_83"); }
  { Lexer l("こんにちは"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_84"); check(t.text=="こんにちは","unicode_text_84"); check(l.lex().isEOF(),"unicode_eof_84"); }
  { Lexer l("// comment こんにちは\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_84"); }
  { Lexer l("한국어"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_85"); check(t.text=="한국어","unicode_text_85"); check(l.lex().isEOF(),"unicode_eof_85"); }
  { Lexer l("// comment 한국어\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_85"); }
  { Lexer l("γειά"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_86"); check(t.text=="γειά","unicode_text_86"); check(l.lex().isEOF(),"unicode_eof_86"); }
  { Lexer l("// comment γειά\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_86"); }
  { Lexer l("שלום"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_87"); check(t.text=="שלום","unicode_text_87"); check(l.lex().isEOF(),"unicode_eof_87"); }
  { Lexer l("// comment שלום\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_87"); }
  { Lexer l("नमस्ते"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_88"); check(t.text=="नमस्ते","unicode_text_88"); check(l.lex().isEOF(),"unicode_eof_88"); }
  { Lexer l("// comment नमस्ते\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_88"); }
  { Lexer l("გამარჯობა"); auto t=l.lex(); check(t.kind==TokenKind::Identifier,"unicode_89"); check(t.text=="გამარჯობა","unicode_text_89"); check(l.lex().isEOF(),"unicode_eof_89"); }
  { Lexer l("// comment გამარჯობა\nfunction x() {}"); auto a=l.lex(); check(a.kind==TokenKind::Keyword,"comment_keyword_89"); }

  { Lexer l("/* unterminated"); auto t=l.lex(); check(l.hasErrors(),"unterminated_comment"); check(t.isEOF()||t.kind==TokenKind::Unknown,"unterminated_comment_token"); }
  return failures?1:0;
}
