#include "hyper/lib/Parse/Lexer.cpp"
#include <iostream>
using namespace hyper::parse;
static int failures=0; static void check(bool v,const char*n){if(!v){std::cerr<<"FAIL "<<n<<"\n";++failures;}}
int main(){
 { Lexer l("// comment 0\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_0"); check(!l.hasErrors(),"comment_error_0"); }
 { Lexer l("/* comment 1 */ var x: int = 1"); auto t=l.lex(); check(!t.isEOF(),"comment_case_1"); check(!l.hasErrors(),"comment_error_1"); }
 { Lexer l("function x() { // inline 2\\n return 2\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_2"); check(!l.hasErrors(),"comment_error_2"); }
 { Lexer l("\\n\\t // leading 3\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_3"); check(!l.hasErrors(),"comment_error_3"); }
 { Lexer l("// comment 4\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_4"); check(!l.hasErrors(),"comment_error_4"); }
 { Lexer l("/* comment 5 */ var x: int = 5"); auto t=l.lex(); check(!t.isEOF(),"comment_case_5"); check(!l.hasErrors(),"comment_error_5"); }
 { Lexer l("function x() { // inline 6\\n return 6\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_6"); check(!l.hasErrors(),"comment_error_6"); }
 { Lexer l("\\n\\t // leading 7\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_7"); check(!l.hasErrors(),"comment_error_7"); }
 { Lexer l("// comment 8\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_8"); check(!l.hasErrors(),"comment_error_8"); }
 { Lexer l("/* comment 9 */ var x: int = 9"); auto t=l.lex(); check(!t.isEOF(),"comment_case_9"); check(!l.hasErrors(),"comment_error_9"); }
 { Lexer l("function x() { // inline 10\\n return 10\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_10"); check(!l.hasErrors(),"comment_error_10"); }
 { Lexer l("\\n\\t // leading 11\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_11"); check(!l.hasErrors(),"comment_error_11"); }
 { Lexer l("// comment 12\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_12"); check(!l.hasErrors(),"comment_error_12"); }
 { Lexer l("/* comment 13 */ var x: int = 13"); auto t=l.lex(); check(!t.isEOF(),"comment_case_13"); check(!l.hasErrors(),"comment_error_13"); }
 { Lexer l("function x() { // inline 14\\n return 14\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_14"); check(!l.hasErrors(),"comment_error_14"); }
 { Lexer l("\\n\\t // leading 15\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_15"); check(!l.hasErrors(),"comment_error_15"); }
 { Lexer l("// comment 16\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_16"); check(!l.hasErrors(),"comment_error_16"); }
 { Lexer l("/* comment 17 */ var x: int = 17"); auto t=l.lex(); check(!t.isEOF(),"comment_case_17"); check(!l.hasErrors(),"comment_error_17"); }
 { Lexer l("function x() { // inline 18\\n return 18\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_18"); check(!l.hasErrors(),"comment_error_18"); }
 { Lexer l("\\n\\t // leading 19\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_19"); check(!l.hasErrors(),"comment_error_19"); }
 { Lexer l("// comment 20\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_20"); check(!l.hasErrors(),"comment_error_20"); }
 { Lexer l("/* comment 21 */ var x: int = 21"); auto t=l.lex(); check(!t.isEOF(),"comment_case_21"); check(!l.hasErrors(),"comment_error_21"); }
 { Lexer l("function x() { // inline 22\\n return 22\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_22"); check(!l.hasErrors(),"comment_error_22"); }
 { Lexer l("\\n\\t // leading 23\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_23"); check(!l.hasErrors(),"comment_error_23"); }
 { Lexer l("// comment 24\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_24"); check(!l.hasErrors(),"comment_error_24"); }
 { Lexer l("/* comment 25 */ var x: int = 25"); auto t=l.lex(); check(!t.isEOF(),"comment_case_25"); check(!l.hasErrors(),"comment_error_25"); }
 { Lexer l("function x() { // inline 26\\n return 26\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_26"); check(!l.hasErrors(),"comment_error_26"); }
 { Lexer l("\\n\\t // leading 27\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_27"); check(!l.hasErrors(),"comment_error_27"); }
 { Lexer l("// comment 28\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_28"); check(!l.hasErrors(),"comment_error_28"); }
 { Lexer l("/* comment 29 */ var x: int = 29"); auto t=l.lex(); check(!t.isEOF(),"comment_case_29"); check(!l.hasErrors(),"comment_error_29"); }
 { Lexer l("function x() { // inline 30\\n return 30\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_30"); check(!l.hasErrors(),"comment_error_30"); }
 { Lexer l("\\n\\t // leading 31\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_31"); check(!l.hasErrors(),"comment_error_31"); }
 { Lexer l("// comment 32\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_32"); check(!l.hasErrors(),"comment_error_32"); }
 { Lexer l("/* comment 33 */ var x: int = 33"); auto t=l.lex(); check(!t.isEOF(),"comment_case_33"); check(!l.hasErrors(),"comment_error_33"); }
 { Lexer l("function x() { // inline 34\\n return 34\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_34"); check(!l.hasErrors(),"comment_error_34"); }
 { Lexer l("\\n\\t // leading 35\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_35"); check(!l.hasErrors(),"comment_error_35"); }
 { Lexer l("// comment 36\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_36"); check(!l.hasErrors(),"comment_error_36"); }
 { Lexer l("/* comment 37 */ var x: int = 37"); auto t=l.lex(); check(!t.isEOF(),"comment_case_37"); check(!l.hasErrors(),"comment_error_37"); }
 { Lexer l("function x() { // inline 38\\n return 38\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_38"); check(!l.hasErrors(),"comment_error_38"); }
 { Lexer l("\\n\\t // leading 39\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_39"); check(!l.hasErrors(),"comment_error_39"); }
 { Lexer l("// comment 40\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_40"); check(!l.hasErrors(),"comment_error_40"); }
 { Lexer l("/* comment 41 */ var x: int = 41"); auto t=l.lex(); check(!t.isEOF(),"comment_case_41"); check(!l.hasErrors(),"comment_error_41"); }
 { Lexer l("function x() { // inline 42\\n return 42\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_42"); check(!l.hasErrors(),"comment_error_42"); }
 { Lexer l("\\n\\t // leading 43\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_43"); check(!l.hasErrors(),"comment_error_43"); }
 { Lexer l("// comment 44\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_44"); check(!l.hasErrors(),"comment_error_44"); }
 { Lexer l("/* comment 45 */ var x: int = 45"); auto t=l.lex(); check(!t.isEOF(),"comment_case_45"); check(!l.hasErrors(),"comment_error_45"); }
 { Lexer l("function x() { // inline 46\\n return 46\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_46"); check(!l.hasErrors(),"comment_error_46"); }
 { Lexer l("\\n\\t // leading 47\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_47"); check(!l.hasErrors(),"comment_error_47"); }
 { Lexer l("// comment 48\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_48"); check(!l.hasErrors(),"comment_error_48"); }
 { Lexer l("/* comment 49 */ var x: int = 49"); auto t=l.lex(); check(!t.isEOF(),"comment_case_49"); check(!l.hasErrors(),"comment_error_49"); }
 { Lexer l("function x() { // inline 50\\n return 50\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_50"); check(!l.hasErrors(),"comment_error_50"); }
 { Lexer l("\\n\\t // leading 51\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_51"); check(!l.hasErrors(),"comment_error_51"); }
 { Lexer l("// comment 52\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_52"); check(!l.hasErrors(),"comment_error_52"); }
 { Lexer l("/* comment 53 */ var x: int = 53"); auto t=l.lex(); check(!t.isEOF(),"comment_case_53"); check(!l.hasErrors(),"comment_error_53"); }
 { Lexer l("function x() { // inline 54\\n return 54\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_54"); check(!l.hasErrors(),"comment_error_54"); }
 { Lexer l("\\n\\t // leading 55\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_55"); check(!l.hasErrors(),"comment_error_55"); }
 { Lexer l("// comment 56\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_56"); check(!l.hasErrors(),"comment_error_56"); }
 { Lexer l("/* comment 57 */ var x: int = 57"); auto t=l.lex(); check(!t.isEOF(),"comment_case_57"); check(!l.hasErrors(),"comment_error_57"); }
 { Lexer l("function x() { // inline 58\\n return 58\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_58"); check(!l.hasErrors(),"comment_error_58"); }
 { Lexer l("\\n\\t // leading 59\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_59"); check(!l.hasErrors(),"comment_error_59"); }
 { Lexer l("// comment 60\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_60"); check(!l.hasErrors(),"comment_error_60"); }
 { Lexer l("/* comment 61 */ var x: int = 61"); auto t=l.lex(); check(!t.isEOF(),"comment_case_61"); check(!l.hasErrors(),"comment_error_61"); }
 { Lexer l("function x() { // inline 62\\n return 62\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_62"); check(!l.hasErrors(),"comment_error_62"); }
 { Lexer l("\\n\\t // leading 63\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_63"); check(!l.hasErrors(),"comment_error_63"); }
 { Lexer l("// comment 64\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_64"); check(!l.hasErrors(),"comment_error_64"); }
 { Lexer l("/* comment 65 */ var x: int = 65"); auto t=l.lex(); check(!t.isEOF(),"comment_case_65"); check(!l.hasErrors(),"comment_error_65"); }
 { Lexer l("function x() { // inline 66\\n return 66\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_66"); check(!l.hasErrors(),"comment_error_66"); }
 { Lexer l("\\n\\t // leading 67\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_67"); check(!l.hasErrors(),"comment_error_67"); }
 { Lexer l("// comment 68\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_68"); check(!l.hasErrors(),"comment_error_68"); }
 { Lexer l("/* comment 69 */ var x: int = 69"); auto t=l.lex(); check(!t.isEOF(),"comment_case_69"); check(!l.hasErrors(),"comment_error_69"); }
 { Lexer l("function x() { // inline 70\\n return 70\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_70"); check(!l.hasErrors(),"comment_error_70"); }
 { Lexer l("\\n\\t // leading 71\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_71"); check(!l.hasErrors(),"comment_error_71"); }
 { Lexer l("// comment 72\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_72"); check(!l.hasErrors(),"comment_error_72"); }
 { Lexer l("/* comment 73 */ var x: int = 73"); auto t=l.lex(); check(!t.isEOF(),"comment_case_73"); check(!l.hasErrors(),"comment_error_73"); }
 { Lexer l("function x() { // inline 74\\n return 74\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_74"); check(!l.hasErrors(),"comment_error_74"); }
 { Lexer l("\\n\\t // leading 75\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_75"); check(!l.hasErrors(),"comment_error_75"); }
 { Lexer l("// comment 76\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_76"); check(!l.hasErrors(),"comment_error_76"); }
 { Lexer l("/* comment 77 */ var x: int = 77"); auto t=l.lex(); check(!t.isEOF(),"comment_case_77"); check(!l.hasErrors(),"comment_error_77"); }
 { Lexer l("function x() { // inline 78\\n return 78\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_78"); check(!l.hasErrors(),"comment_error_78"); }
 { Lexer l("\\n\\t // leading 79\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_79"); check(!l.hasErrors(),"comment_error_79"); }
 { Lexer l("// comment 80\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_80"); check(!l.hasErrors(),"comment_error_80"); }
 { Lexer l("/* comment 81 */ var x: int = 81"); auto t=l.lex(); check(!t.isEOF(),"comment_case_81"); check(!l.hasErrors(),"comment_error_81"); }
 { Lexer l("function x() { // inline 82\\n return 82\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_82"); check(!l.hasErrors(),"comment_error_82"); }
 { Lexer l("\\n\\t // leading 83\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_83"); check(!l.hasErrors(),"comment_error_83"); }
 { Lexer l("// comment 84\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_84"); check(!l.hasErrors(),"comment_error_84"); }
 { Lexer l("/* comment 85 */ var x: int = 85"); auto t=l.lex(); check(!t.isEOF(),"comment_case_85"); check(!l.hasErrors(),"comment_error_85"); }
 { Lexer l("function x() { // inline 86\\n return 86\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_86"); check(!l.hasErrors(),"comment_error_86"); }
 { Lexer l("\\n\\t // leading 87\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_87"); check(!l.hasErrors(),"comment_error_87"); }
 { Lexer l("// comment 88\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_88"); check(!l.hasErrors(),"comment_error_88"); }
 { Lexer l("/* comment 89 */ var x: int = 89"); auto t=l.lex(); check(!t.isEOF(),"comment_case_89"); check(!l.hasErrors(),"comment_error_89"); }
 { Lexer l("function x() { // inline 90\\n return 90\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_90"); check(!l.hasErrors(),"comment_error_90"); }
 { Lexer l("\\n\\t // leading 91\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_91"); check(!l.hasErrors(),"comment_error_91"); }
 { Lexer l("// comment 92\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_92"); check(!l.hasErrors(),"comment_error_92"); }
 { Lexer l("/* comment 93 */ var x: int = 93"); auto t=l.lex(); check(!t.isEOF(),"comment_case_93"); check(!l.hasErrors(),"comment_error_93"); }
 { Lexer l("function x() { // inline 94\\n return 94\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_94"); check(!l.hasErrors(),"comment_error_94"); }
 { Lexer l("\\n\\t // leading 95\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_95"); check(!l.hasErrors(),"comment_error_95"); }
 { Lexer l("// comment 96\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_96"); check(!l.hasErrors(),"comment_error_96"); }
 { Lexer l("/* comment 97 */ var x: int = 97"); auto t=l.lex(); check(!t.isEOF(),"comment_case_97"); check(!l.hasErrors(),"comment_error_97"); }
 { Lexer l("function x() { // inline 98\\n return 98\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_98"); check(!l.hasErrors(),"comment_error_98"); }
 { Lexer l("\\n\\t // leading 99\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_99"); check(!l.hasErrors(),"comment_error_99"); }
 { Lexer l("// comment 100\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_100"); check(!l.hasErrors(),"comment_error_100"); }
 { Lexer l("/* comment 101 */ var x: int = 101"); auto t=l.lex(); check(!t.isEOF(),"comment_case_101"); check(!l.hasErrors(),"comment_error_101"); }
 { Lexer l("function x() { // inline 102\\n return 102\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_102"); check(!l.hasErrors(),"comment_error_102"); }
 { Lexer l("\\n\\t // leading 103\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_103"); check(!l.hasErrors(),"comment_error_103"); }
 { Lexer l("// comment 104\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_104"); check(!l.hasErrors(),"comment_error_104"); }
 { Lexer l("/* comment 105 */ var x: int = 105"); auto t=l.lex(); check(!t.isEOF(),"comment_case_105"); check(!l.hasErrors(),"comment_error_105"); }
 { Lexer l("function x() { // inline 106\\n return 106\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_106"); check(!l.hasErrors(),"comment_error_106"); }
 { Lexer l("\\n\\t // leading 107\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_107"); check(!l.hasErrors(),"comment_error_107"); }
 { Lexer l("// comment 108\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_108"); check(!l.hasErrors(),"comment_error_108"); }
 { Lexer l("/* comment 109 */ var x: int = 109"); auto t=l.lex(); check(!t.isEOF(),"comment_case_109"); check(!l.hasErrors(),"comment_error_109"); }
 { Lexer l("function x() { // inline 110\\n return 110\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_110"); check(!l.hasErrors(),"comment_error_110"); }
 { Lexer l("\\n\\t // leading 111\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_111"); check(!l.hasErrors(),"comment_error_111"); }
 { Lexer l("// comment 112\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_112"); check(!l.hasErrors(),"comment_error_112"); }
 { Lexer l("/* comment 113 */ var x: int = 113"); auto t=l.lex(); check(!t.isEOF(),"comment_case_113"); check(!l.hasErrors(),"comment_error_113"); }
 { Lexer l("function x() { // inline 114\\n return 114\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_114"); check(!l.hasErrors(),"comment_error_114"); }
 { Lexer l("\\n\\t // leading 115\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_115"); check(!l.hasErrors(),"comment_error_115"); }
 { Lexer l("// comment 116\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_116"); check(!l.hasErrors(),"comment_error_116"); }
 { Lexer l("/* comment 117 */ var x: int = 117"); auto t=l.lex(); check(!t.isEOF(),"comment_case_117"); check(!l.hasErrors(),"comment_error_117"); }
 { Lexer l("function x() { // inline 118\\n return 118\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_118"); check(!l.hasErrors(),"comment_error_118"); }
 { Lexer l("\\n\\t // leading 119\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_119"); check(!l.hasErrors(),"comment_error_119"); }
 { Lexer l("// comment 120\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_120"); check(!l.hasErrors(),"comment_error_120"); }
 { Lexer l("/* comment 121 */ var x: int = 121"); auto t=l.lex(); check(!t.isEOF(),"comment_case_121"); check(!l.hasErrors(),"comment_error_121"); }
 { Lexer l("function x() { // inline 122\\n return 122\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_122"); check(!l.hasErrors(),"comment_error_122"); }
 { Lexer l("\\n\\t // leading 123\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_123"); check(!l.hasErrors(),"comment_error_123"); }
 { Lexer l("// comment 124\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_124"); check(!l.hasErrors(),"comment_error_124"); }
 { Lexer l("/* comment 125 */ var x: int = 125"); auto t=l.lex(); check(!t.isEOF(),"comment_case_125"); check(!l.hasErrors(),"comment_error_125"); }
 { Lexer l("function x() { // inline 126\\n return 126\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_126"); check(!l.hasErrors(),"comment_error_126"); }
 { Lexer l("\\n\\t // leading 127\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_127"); check(!l.hasErrors(),"comment_error_127"); }
 { Lexer l("// comment 128\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_128"); check(!l.hasErrors(),"comment_error_128"); }
 { Lexer l("/* comment 129 */ var x: int = 129"); auto t=l.lex(); check(!t.isEOF(),"comment_case_129"); check(!l.hasErrors(),"comment_error_129"); }
 { Lexer l("function x() { // inline 130\\n return 130\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_130"); check(!l.hasErrors(),"comment_error_130"); }
 { Lexer l("\\n\\t // leading 131\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_131"); check(!l.hasErrors(),"comment_error_131"); }
 { Lexer l("// comment 132\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_132"); check(!l.hasErrors(),"comment_error_132"); }
 { Lexer l("/* comment 133 */ var x: int = 133"); auto t=l.lex(); check(!t.isEOF(),"comment_case_133"); check(!l.hasErrors(),"comment_error_133"); }
 { Lexer l("function x() { // inline 134\\n return 134\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_134"); check(!l.hasErrors(),"comment_error_134"); }
 { Lexer l("\\n\\t // leading 135\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_135"); check(!l.hasErrors(),"comment_error_135"); }
 { Lexer l("// comment 136\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_136"); check(!l.hasErrors(),"comment_error_136"); }
 { Lexer l("/* comment 137 */ var x: int = 137"); auto t=l.lex(); check(!t.isEOF(),"comment_case_137"); check(!l.hasErrors(),"comment_error_137"); }
 { Lexer l("function x() { // inline 138\\n return 138\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_138"); check(!l.hasErrors(),"comment_error_138"); }
 { Lexer l("\\n\\t // leading 139\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_139"); check(!l.hasErrors(),"comment_error_139"); }
 { Lexer l("// comment 140\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_140"); check(!l.hasErrors(),"comment_error_140"); }
 { Lexer l("/* comment 141 */ var x: int = 141"); auto t=l.lex(); check(!t.isEOF(),"comment_case_141"); check(!l.hasErrors(),"comment_error_141"); }
 { Lexer l("function x() { // inline 142\\n return 142\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_142"); check(!l.hasErrors(),"comment_error_142"); }
 { Lexer l("\\n\\t // leading 143\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_143"); check(!l.hasErrors(),"comment_error_143"); }
 { Lexer l("// comment 144\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_144"); check(!l.hasErrors(),"comment_error_144"); }
 { Lexer l("/* comment 145 */ var x: int = 145"); auto t=l.lex(); check(!t.isEOF(),"comment_case_145"); check(!l.hasErrors(),"comment_error_145"); }
 { Lexer l("function x() { // inline 146\\n return 146\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_146"); check(!l.hasErrors(),"comment_error_146"); }
 { Lexer l("\\n\\t // leading 147\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_147"); check(!l.hasErrors(),"comment_error_147"); }
 { Lexer l("// comment 148\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_148"); check(!l.hasErrors(),"comment_error_148"); }
 { Lexer l("/* comment 149 */ var x: int = 149"); auto t=l.lex(); check(!t.isEOF(),"comment_case_149"); check(!l.hasErrors(),"comment_error_149"); }
 { Lexer l("function x() { // inline 150\\n return 150\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_150"); check(!l.hasErrors(),"comment_error_150"); }
 { Lexer l("\\n\\t // leading 151\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_151"); check(!l.hasErrors(),"comment_error_151"); }
 { Lexer l("// comment 152\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_152"); check(!l.hasErrors(),"comment_error_152"); }
 { Lexer l("/* comment 153 */ var x: int = 153"); auto t=l.lex(); check(!t.isEOF(),"comment_case_153"); check(!l.hasErrors(),"comment_error_153"); }
 { Lexer l("function x() { // inline 154\\n return 154\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_154"); check(!l.hasErrors(),"comment_error_154"); }
 { Lexer l("\\n\\t // leading 155\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_155"); check(!l.hasErrors(),"comment_error_155"); }
 { Lexer l("// comment 156\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_156"); check(!l.hasErrors(),"comment_error_156"); }
 { Lexer l("/* comment 157 */ var x: int = 157"); auto t=l.lex(); check(!t.isEOF(),"comment_case_157"); check(!l.hasErrors(),"comment_error_157"); }
 { Lexer l("function x() { // inline 158\\n return 158\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_158"); check(!l.hasErrors(),"comment_error_158"); }
 { Lexer l("\\n\\t // leading 159\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_159"); check(!l.hasErrors(),"comment_error_159"); }
 { Lexer l("// comment 160\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_160"); check(!l.hasErrors(),"comment_error_160"); }
 { Lexer l("/* comment 161 */ var x: int = 161"); auto t=l.lex(); check(!t.isEOF(),"comment_case_161"); check(!l.hasErrors(),"comment_error_161"); }
 { Lexer l("function x() { // inline 162\\n return 162\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_162"); check(!l.hasErrors(),"comment_error_162"); }
 { Lexer l("\\n\\t // leading 163\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_163"); check(!l.hasErrors(),"comment_error_163"); }
 { Lexer l("// comment 164\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_164"); check(!l.hasErrors(),"comment_error_164"); }
 { Lexer l("/* comment 165 */ var x: int = 165"); auto t=l.lex(); check(!t.isEOF(),"comment_case_165"); check(!l.hasErrors(),"comment_error_165"); }
 { Lexer l("function x() { // inline 166\\n return 166\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_166"); check(!l.hasErrors(),"comment_error_166"); }
 { Lexer l("\\n\\t // leading 167\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_167"); check(!l.hasErrors(),"comment_error_167"); }
 { Lexer l("// comment 168\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_168"); check(!l.hasErrors(),"comment_error_168"); }
 { Lexer l("/* comment 169 */ var x: int = 169"); auto t=l.lex(); check(!t.isEOF(),"comment_case_169"); check(!l.hasErrors(),"comment_error_169"); }
 { Lexer l("function x() { // inline 170\\n return 170\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_170"); check(!l.hasErrors(),"comment_error_170"); }
 { Lexer l("\\n\\t // leading 171\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_171"); check(!l.hasErrors(),"comment_error_171"); }
 { Lexer l("// comment 172\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_172"); check(!l.hasErrors(),"comment_error_172"); }
 { Lexer l("/* comment 173 */ var x: int = 173"); auto t=l.lex(); check(!t.isEOF(),"comment_case_173"); check(!l.hasErrors(),"comment_error_173"); }
 { Lexer l("function x() { // inline 174\\n return 174\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_174"); check(!l.hasErrors(),"comment_error_174"); }
 { Lexer l("\\n\\t // leading 175\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_175"); check(!l.hasErrors(),"comment_error_175"); }
 { Lexer l("// comment 176\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_176"); check(!l.hasErrors(),"comment_error_176"); }
 { Lexer l("/* comment 177 */ var x: int = 177"); auto t=l.lex(); check(!t.isEOF(),"comment_case_177"); check(!l.hasErrors(),"comment_error_177"); }
 { Lexer l("function x() { // inline 178\\n return 178\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_178"); check(!l.hasErrors(),"comment_error_178"); }
 { Lexer l("\\n\\t // leading 179\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_179"); check(!l.hasErrors(),"comment_error_179"); }
 { Lexer l("// comment 180\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_180"); check(!l.hasErrors(),"comment_error_180"); }
 { Lexer l("/* comment 181 */ var x: int = 181"); auto t=l.lex(); check(!t.isEOF(),"comment_case_181"); check(!l.hasErrors(),"comment_error_181"); }
 { Lexer l("function x() { // inline 182\\n return 182\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_182"); check(!l.hasErrors(),"comment_error_182"); }
 { Lexer l("\\n\\t // leading 183\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_183"); check(!l.hasErrors(),"comment_error_183"); }
 { Lexer l("// comment 184\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_184"); check(!l.hasErrors(),"comment_error_184"); }
 { Lexer l("/* comment 185 */ var x: int = 185"); auto t=l.lex(); check(!t.isEOF(),"comment_case_185"); check(!l.hasErrors(),"comment_error_185"); }
 { Lexer l("function x() { // inline 186\\n return 186\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_186"); check(!l.hasErrors(),"comment_error_186"); }
 { Lexer l("\\n\\t // leading 187\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_187"); check(!l.hasErrors(),"comment_error_187"); }
 { Lexer l("// comment 188\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_188"); check(!l.hasErrors(),"comment_error_188"); }
 { Lexer l("/* comment 189 */ var x: int = 189"); auto t=l.lex(); check(!t.isEOF(),"comment_case_189"); check(!l.hasErrors(),"comment_error_189"); }
 { Lexer l("function x() { // inline 190\\n return 190\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_190"); check(!l.hasErrors(),"comment_error_190"); }
 { Lexer l("\\n\\t // leading 191\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_191"); check(!l.hasErrors(),"comment_error_191"); }
 { Lexer l("// comment 192\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_192"); check(!l.hasErrors(),"comment_error_192"); }
 { Lexer l("/* comment 193 */ var x: int = 193"); auto t=l.lex(); check(!t.isEOF(),"comment_case_193"); check(!l.hasErrors(),"comment_error_193"); }
 { Lexer l("function x() { // inline 194\\n return 194\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_194"); check(!l.hasErrors(),"comment_error_194"); }
 { Lexer l("\\n\\t // leading 195\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_195"); check(!l.hasErrors(),"comment_error_195"); }
 { Lexer l("// comment 196\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_196"); check(!l.hasErrors(),"comment_error_196"); }
 { Lexer l("/* comment 197 */ var x: int = 197"); auto t=l.lex(); check(!t.isEOF(),"comment_case_197"); check(!l.hasErrors(),"comment_error_197"); }
 { Lexer l("function x() { // inline 198\\n return 198\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_198"); check(!l.hasErrors(),"comment_error_198"); }
 { Lexer l("\\n\\t // leading 199\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_199"); check(!l.hasErrors(),"comment_error_199"); }
 { Lexer l("// comment 200\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_200"); check(!l.hasErrors(),"comment_error_200"); }
 { Lexer l("/* comment 201 */ var x: int = 201"); auto t=l.lex(); check(!t.isEOF(),"comment_case_201"); check(!l.hasErrors(),"comment_error_201"); }
 { Lexer l("function x() { // inline 202\\n return 202\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_202"); check(!l.hasErrors(),"comment_error_202"); }
 { Lexer l("\\n\\t // leading 203\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_203"); check(!l.hasErrors(),"comment_error_203"); }
 { Lexer l("// comment 204\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_204"); check(!l.hasErrors(),"comment_error_204"); }
 { Lexer l("/* comment 205 */ var x: int = 205"); auto t=l.lex(); check(!t.isEOF(),"comment_case_205"); check(!l.hasErrors(),"comment_error_205"); }
 { Lexer l("function x() { // inline 206\\n return 206\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_206"); check(!l.hasErrors(),"comment_error_206"); }
 { Lexer l("\\n\\t // leading 207\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_207"); check(!l.hasErrors(),"comment_error_207"); }
 { Lexer l("// comment 208\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_208"); check(!l.hasErrors(),"comment_error_208"); }
 { Lexer l("/* comment 209 */ var x: int = 209"); auto t=l.lex(); check(!t.isEOF(),"comment_case_209"); check(!l.hasErrors(),"comment_error_209"); }
 { Lexer l("function x() { // inline 210\\n return 210\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_210"); check(!l.hasErrors(),"comment_error_210"); }
 { Lexer l("\\n\\t // leading 211\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_211"); check(!l.hasErrors(),"comment_error_211"); }
 { Lexer l("// comment 212\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_212"); check(!l.hasErrors(),"comment_error_212"); }
 { Lexer l("/* comment 213 */ var x: int = 213"); auto t=l.lex(); check(!t.isEOF(),"comment_case_213"); check(!l.hasErrors(),"comment_error_213"); }
 { Lexer l("function x() { // inline 214\\n return 214\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_214"); check(!l.hasErrors(),"comment_error_214"); }
 { Lexer l("\\n\\t // leading 215\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_215"); check(!l.hasErrors(),"comment_error_215"); }
 { Lexer l("// comment 216\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_216"); check(!l.hasErrors(),"comment_error_216"); }
 { Lexer l("/* comment 217 */ var x: int = 217"); auto t=l.lex(); check(!t.isEOF(),"comment_case_217"); check(!l.hasErrors(),"comment_error_217"); }
 { Lexer l("function x() { // inline 218\\n return 218\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_218"); check(!l.hasErrors(),"comment_error_218"); }
 { Lexer l("\\n\\t // leading 219\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_219"); check(!l.hasErrors(),"comment_error_219"); }
 { Lexer l("// comment 220\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_220"); check(!l.hasErrors(),"comment_error_220"); }
 { Lexer l("/* comment 221 */ var x: int = 221"); auto t=l.lex(); check(!t.isEOF(),"comment_case_221"); check(!l.hasErrors(),"comment_error_221"); }
 { Lexer l("function x() { // inline 222\\n return 222\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_222"); check(!l.hasErrors(),"comment_error_222"); }
 { Lexer l("\\n\\t // leading 223\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_223"); check(!l.hasErrors(),"comment_error_223"); }
 { Lexer l("// comment 224\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_224"); check(!l.hasErrors(),"comment_error_224"); }
 { Lexer l("/* comment 225 */ var x: int = 225"); auto t=l.lex(); check(!t.isEOF(),"comment_case_225"); check(!l.hasErrors(),"comment_error_225"); }
 { Lexer l("function x() { // inline 226\\n return 226\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_226"); check(!l.hasErrors(),"comment_error_226"); }
 { Lexer l("\\n\\t // leading 227\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_227"); check(!l.hasErrors(),"comment_error_227"); }
 { Lexer l("// comment 228\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_228"); check(!l.hasErrors(),"comment_error_228"); }
 { Lexer l("/* comment 229 */ var x: int = 229"); auto t=l.lex(); check(!t.isEOF(),"comment_case_229"); check(!l.hasErrors(),"comment_error_229"); }
 { Lexer l("function x() { // inline 230\\n return 230\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_230"); check(!l.hasErrors(),"comment_error_230"); }
 { Lexer l("\\n\\t // leading 231\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_231"); check(!l.hasErrors(),"comment_error_231"); }
 { Lexer l("// comment 232\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_232"); check(!l.hasErrors(),"comment_error_232"); }
 { Lexer l("/* comment 233 */ var x: int = 233"); auto t=l.lex(); check(!t.isEOF(),"comment_case_233"); check(!l.hasErrors(),"comment_error_233"); }
 { Lexer l("function x() { // inline 234\\n return 234\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_234"); check(!l.hasErrors(),"comment_error_234"); }
 { Lexer l("\\n\\t // leading 235\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_235"); check(!l.hasErrors(),"comment_error_235"); }
 { Lexer l("// comment 236\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_236"); check(!l.hasErrors(),"comment_error_236"); }
 { Lexer l("/* comment 237 */ var x: int = 237"); auto t=l.lex(); check(!t.isEOF(),"comment_case_237"); check(!l.hasErrors(),"comment_error_237"); }
 { Lexer l("function x() { // inline 238\\n return 238\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_238"); check(!l.hasErrors(),"comment_error_238"); }
 { Lexer l("\\n\\t // leading 239\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_239"); check(!l.hasErrors(),"comment_error_239"); }
 { Lexer l("// comment 240\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_240"); check(!l.hasErrors(),"comment_error_240"); }
 { Lexer l("/* comment 241 */ var x: int = 241"); auto t=l.lex(); check(!t.isEOF(),"comment_case_241"); check(!l.hasErrors(),"comment_error_241"); }
 { Lexer l("function x() { // inline 242\\n return 242\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_242"); check(!l.hasErrors(),"comment_error_242"); }
 { Lexer l("\\n\\t // leading 243\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_243"); check(!l.hasErrors(),"comment_error_243"); }
 { Lexer l("// comment 244\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_244"); check(!l.hasErrors(),"comment_error_244"); }
 { Lexer l("/* comment 245 */ var x: int = 245"); auto t=l.lex(); check(!t.isEOF(),"comment_case_245"); check(!l.hasErrors(),"comment_error_245"); }
 { Lexer l("function x() { // inline 246\\n return 246\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_246"); check(!l.hasErrors(),"comment_error_246"); }
 { Lexer l("\\n\\t // leading 247\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_247"); check(!l.hasErrors(),"comment_error_247"); }
 { Lexer l("// comment 248\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_248"); check(!l.hasErrors(),"comment_error_248"); }
 { Lexer l("/* comment 249 */ var x: int = 249"); auto t=l.lex(); check(!t.isEOF(),"comment_case_249"); check(!l.hasErrors(),"comment_error_249"); }
 { Lexer l("function x() { // inline 250\\n return 250\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_250"); check(!l.hasErrors(),"comment_error_250"); }
 { Lexer l("\\n\\t // leading 251\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_251"); check(!l.hasErrors(),"comment_error_251"); }
 { Lexer l("// comment 252\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_252"); check(!l.hasErrors(),"comment_error_252"); }
 { Lexer l("/* comment 253 */ var x: int = 253"); auto t=l.lex(); check(!t.isEOF(),"comment_case_253"); check(!l.hasErrors(),"comment_error_253"); }
 { Lexer l("function x() { // inline 254\\n return 254\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_254"); check(!l.hasErrors(),"comment_error_254"); }
 { Lexer l("\\n\\t // leading 255\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_255"); check(!l.hasErrors(),"comment_error_255"); }
 { Lexer l("// comment 256\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_256"); check(!l.hasErrors(),"comment_error_256"); }
 { Lexer l("/* comment 257 */ var x: int = 257"); auto t=l.lex(); check(!t.isEOF(),"comment_case_257"); check(!l.hasErrors(),"comment_error_257"); }
 { Lexer l("function x() { // inline 258\\n return 258\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_258"); check(!l.hasErrors(),"comment_error_258"); }
 { Lexer l("\\n\\t // leading 259\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_259"); check(!l.hasErrors(),"comment_error_259"); }
 { Lexer l("// comment 260\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_260"); check(!l.hasErrors(),"comment_error_260"); }
 { Lexer l("/* comment 261 */ var x: int = 261"); auto t=l.lex(); check(!t.isEOF(),"comment_case_261"); check(!l.hasErrors(),"comment_error_261"); }
 { Lexer l("function x() { // inline 262\\n return 262\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_262"); check(!l.hasErrors(),"comment_error_262"); }
 { Lexer l("\\n\\t // leading 263\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_263"); check(!l.hasErrors(),"comment_error_263"); }
 { Lexer l("// comment 264\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_264"); check(!l.hasErrors(),"comment_error_264"); }
 { Lexer l("/* comment 265 */ var x: int = 265"); auto t=l.lex(); check(!t.isEOF(),"comment_case_265"); check(!l.hasErrors(),"comment_error_265"); }
 { Lexer l("function x() { // inline 266\\n return 266\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_266"); check(!l.hasErrors(),"comment_error_266"); }
 { Lexer l("\\n\\t // leading 267\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_267"); check(!l.hasErrors(),"comment_error_267"); }
 { Lexer l("// comment 268\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_268"); check(!l.hasErrors(),"comment_error_268"); }
 { Lexer l("/* comment 269 */ var x: int = 269"); auto t=l.lex(); check(!t.isEOF(),"comment_case_269"); check(!l.hasErrors(),"comment_error_269"); }
 { Lexer l("function x() { // inline 270\\n return 270\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_270"); check(!l.hasErrors(),"comment_error_270"); }
 { Lexer l("\\n\\t // leading 271\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_271"); check(!l.hasErrors(),"comment_error_271"); }
 { Lexer l("// comment 272\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_272"); check(!l.hasErrors(),"comment_error_272"); }
 { Lexer l("/* comment 273 */ var x: int = 273"); auto t=l.lex(); check(!t.isEOF(),"comment_case_273"); check(!l.hasErrors(),"comment_error_273"); }
 { Lexer l("function x() { // inline 274\\n return 274\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_274"); check(!l.hasErrors(),"comment_error_274"); }
 { Lexer l("\\n\\t // leading 275\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_275"); check(!l.hasErrors(),"comment_error_275"); }
 { Lexer l("// comment 276\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_276"); check(!l.hasErrors(),"comment_error_276"); }
 { Lexer l("/* comment 277 */ var x: int = 277"); auto t=l.lex(); check(!t.isEOF(),"comment_case_277"); check(!l.hasErrors(),"comment_error_277"); }
 { Lexer l("function x() { // inline 278\\n return 278\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_278"); check(!l.hasErrors(),"comment_error_278"); }
 { Lexer l("\\n\\t // leading 279\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_279"); check(!l.hasErrors(),"comment_error_279"); }
 { Lexer l("// comment 280\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_280"); check(!l.hasErrors(),"comment_error_280"); }
 { Lexer l("/* comment 281 */ var x: int = 281"); auto t=l.lex(); check(!t.isEOF(),"comment_case_281"); check(!l.hasErrors(),"comment_error_281"); }
 { Lexer l("function x() { // inline 282\\n return 282\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_282"); check(!l.hasErrors(),"comment_error_282"); }
 { Lexer l("\\n\\t // leading 283\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_283"); check(!l.hasErrors(),"comment_error_283"); }
 { Lexer l("// comment 284\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_284"); check(!l.hasErrors(),"comment_error_284"); }
 { Lexer l("/* comment 285 */ var x: int = 285"); auto t=l.lex(); check(!t.isEOF(),"comment_case_285"); check(!l.hasErrors(),"comment_error_285"); }
 { Lexer l("function x() { // inline 286\\n return 286\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_286"); check(!l.hasErrors(),"comment_error_286"); }
 { Lexer l("\\n\\t // leading 287\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_287"); check(!l.hasErrors(),"comment_error_287"); }
 { Lexer l("// comment 288\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_288"); check(!l.hasErrors(),"comment_error_288"); }
 { Lexer l("/* comment 289 */ var x: int = 289"); auto t=l.lex(); check(!t.isEOF(),"comment_case_289"); check(!l.hasErrors(),"comment_error_289"); }
 { Lexer l("function x() { // inline 290\\n return 290\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_290"); check(!l.hasErrors(),"comment_error_290"); }
 { Lexer l("\\n\\t // leading 291\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_291"); check(!l.hasErrors(),"comment_error_291"); }
 { Lexer l("// comment 292\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_292"); check(!l.hasErrors(),"comment_error_292"); }
 { Lexer l("/* comment 293 */ var x: int = 293"); auto t=l.lex(); check(!t.isEOF(),"comment_case_293"); check(!l.hasErrors(),"comment_error_293"); }
 { Lexer l("function x() { // inline 294\\n return 294\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_294"); check(!l.hasErrors(),"comment_error_294"); }
 { Lexer l("\\n\\t // leading 295\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_295"); check(!l.hasErrors(),"comment_error_295"); }
 { Lexer l("// comment 296\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_296"); check(!l.hasErrors(),"comment_error_296"); }
 { Lexer l("/* comment 297 */ var x: int = 297"); auto t=l.lex(); check(!t.isEOF(),"comment_case_297"); check(!l.hasErrors(),"comment_error_297"); }
 { Lexer l("function x() { // inline 298\\n return 298\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_298"); check(!l.hasErrors(),"comment_error_298"); }
 { Lexer l("\\n\\t // leading 299\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_299"); check(!l.hasErrors(),"comment_error_299"); }
 { Lexer l("// comment 300\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_300"); check(!l.hasErrors(),"comment_error_300"); }
 { Lexer l("/* comment 301 */ var x: int = 301"); auto t=l.lex(); check(!t.isEOF(),"comment_case_301"); check(!l.hasErrors(),"comment_error_301"); }
 { Lexer l("function x() { // inline 302\\n return 302\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_302"); check(!l.hasErrors(),"comment_error_302"); }
 { Lexer l("\\n\\t // leading 303\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_303"); check(!l.hasErrors(),"comment_error_303"); }
 { Lexer l("// comment 304\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_304"); check(!l.hasErrors(),"comment_error_304"); }
 { Lexer l("/* comment 305 */ var x: int = 305"); auto t=l.lex(); check(!t.isEOF(),"comment_case_305"); check(!l.hasErrors(),"comment_error_305"); }
 { Lexer l("function x() { // inline 306\\n return 306\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_306"); check(!l.hasErrors(),"comment_error_306"); }
 { Lexer l("\\n\\t // leading 307\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_307"); check(!l.hasErrors(),"comment_error_307"); }
 { Lexer l("// comment 308\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_308"); check(!l.hasErrors(),"comment_error_308"); }
 { Lexer l("/* comment 309 */ var x: int = 309"); auto t=l.lex(); check(!t.isEOF(),"comment_case_309"); check(!l.hasErrors(),"comment_error_309"); }
 { Lexer l("function x() { // inline 310\\n return 310\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_310"); check(!l.hasErrors(),"comment_error_310"); }
 { Lexer l("\\n\\t // leading 311\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_311"); check(!l.hasErrors(),"comment_error_311"); }
 { Lexer l("// comment 312\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_312"); check(!l.hasErrors(),"comment_error_312"); }
 { Lexer l("/* comment 313 */ var x: int = 313"); auto t=l.lex(); check(!t.isEOF(),"comment_case_313"); check(!l.hasErrors(),"comment_error_313"); }
 { Lexer l("function x() { // inline 314\\n return 314\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_314"); check(!l.hasErrors(),"comment_error_314"); }
 { Lexer l("\\n\\t // leading 315\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_315"); check(!l.hasErrors(),"comment_error_315"); }
 { Lexer l("// comment 316\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_316"); check(!l.hasErrors(),"comment_error_316"); }
 { Lexer l("/* comment 317 */ var x: int = 317"); auto t=l.lex(); check(!t.isEOF(),"comment_case_317"); check(!l.hasErrors(),"comment_error_317"); }
 { Lexer l("function x() { // inline 318\\n return 318\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_318"); check(!l.hasErrors(),"comment_error_318"); }
 { Lexer l("\\n\\t // leading 319\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_319"); check(!l.hasErrors(),"comment_error_319"); }
 { Lexer l("// comment 320\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_320"); check(!l.hasErrors(),"comment_error_320"); }
 { Lexer l("/* comment 321 */ var x: int = 321"); auto t=l.lex(); check(!t.isEOF(),"comment_case_321"); check(!l.hasErrors(),"comment_error_321"); }
 { Lexer l("function x() { // inline 322\\n return 322\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_322"); check(!l.hasErrors(),"comment_error_322"); }
 { Lexer l("\\n\\t // leading 323\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_323"); check(!l.hasErrors(),"comment_error_323"); }
 { Lexer l("// comment 324\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_324"); check(!l.hasErrors(),"comment_error_324"); }
 { Lexer l("/* comment 325 */ var x: int = 325"); auto t=l.lex(); check(!t.isEOF(),"comment_case_325"); check(!l.hasErrors(),"comment_error_325"); }
 { Lexer l("function x() { // inline 326\\n return 326\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_326"); check(!l.hasErrors(),"comment_error_326"); }
 { Lexer l("\\n\\t // leading 327\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_327"); check(!l.hasErrors(),"comment_error_327"); }
 { Lexer l("// comment 328\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_328"); check(!l.hasErrors(),"comment_error_328"); }
 { Lexer l("/* comment 329 */ var x: int = 329"); auto t=l.lex(); check(!t.isEOF(),"comment_case_329"); check(!l.hasErrors(),"comment_error_329"); }
 { Lexer l("function x() { // inline 330\\n return 330\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_330"); check(!l.hasErrors(),"comment_error_330"); }
 { Lexer l("\\n\\t // leading 331\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_331"); check(!l.hasErrors(),"comment_error_331"); }
 { Lexer l("// comment 332\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_332"); check(!l.hasErrors(),"comment_error_332"); }
 { Lexer l("/* comment 333 */ var x: int = 333"); auto t=l.lex(); check(!t.isEOF(),"comment_case_333"); check(!l.hasErrors(),"comment_error_333"); }
 { Lexer l("function x() { // inline 334\\n return 334\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_334"); check(!l.hasErrors(),"comment_error_334"); }
 { Lexer l("\\n\\t // leading 335\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_335"); check(!l.hasErrors(),"comment_error_335"); }
 { Lexer l("// comment 336\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_336"); check(!l.hasErrors(),"comment_error_336"); }
 { Lexer l("/* comment 337 */ var x: int = 337"); auto t=l.lex(); check(!t.isEOF(),"comment_case_337"); check(!l.hasErrors(),"comment_error_337"); }
 { Lexer l("function x() { // inline 338\\n return 338\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_338"); check(!l.hasErrors(),"comment_error_338"); }
 { Lexer l("\\n\\t // leading 339\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_339"); check(!l.hasErrors(),"comment_error_339"); }
 { Lexer l("// comment 340\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_340"); check(!l.hasErrors(),"comment_error_340"); }
 { Lexer l("/* comment 341 */ var x: int = 341"); auto t=l.lex(); check(!t.isEOF(),"comment_case_341"); check(!l.hasErrors(),"comment_error_341"); }
 { Lexer l("function x() { // inline 342\\n return 342\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_342"); check(!l.hasErrors(),"comment_error_342"); }
 { Lexer l("\\n\\t // leading 343\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_343"); check(!l.hasErrors(),"comment_error_343"); }
 { Lexer l("// comment 344\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_344"); check(!l.hasErrors(),"comment_error_344"); }
 { Lexer l("/* comment 345 */ var x: int = 345"); auto t=l.lex(); check(!t.isEOF(),"comment_case_345"); check(!l.hasErrors(),"comment_error_345"); }
 { Lexer l("function x() { // inline 346\\n return 346\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_346"); check(!l.hasErrors(),"comment_error_346"); }
 { Lexer l("\\n\\t // leading 347\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_347"); check(!l.hasErrors(),"comment_error_347"); }
 { Lexer l("// comment 348\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_348"); check(!l.hasErrors(),"comment_error_348"); }
 { Lexer l("/* comment 349 */ var x: int = 349"); auto t=l.lex(); check(!t.isEOF(),"comment_case_349"); check(!l.hasErrors(),"comment_error_349"); }
 { Lexer l("function x() { // inline 350\\n return 350\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_350"); check(!l.hasErrors(),"comment_error_350"); }
 { Lexer l("\\n\\t // leading 351\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_351"); check(!l.hasErrors(),"comment_error_351"); }
 { Lexer l("// comment 352\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_352"); check(!l.hasErrors(),"comment_error_352"); }
 { Lexer l("/* comment 353 */ var x: int = 353"); auto t=l.lex(); check(!t.isEOF(),"comment_case_353"); check(!l.hasErrors(),"comment_error_353"); }
 { Lexer l("function x() { // inline 354\\n return 354\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_354"); check(!l.hasErrors(),"comment_error_354"); }
 { Lexer l("\\n\\t // leading 355\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_355"); check(!l.hasErrors(),"comment_error_355"); }
 { Lexer l("// comment 356\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_356"); check(!l.hasErrors(),"comment_error_356"); }
 { Lexer l("/* comment 357 */ var x: int = 357"); auto t=l.lex(); check(!t.isEOF(),"comment_case_357"); check(!l.hasErrors(),"comment_error_357"); }
 { Lexer l("function x() { // inline 358\\n return 358\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_358"); check(!l.hasErrors(),"comment_error_358"); }
 { Lexer l("\\n\\t // leading 359\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_359"); check(!l.hasErrors(),"comment_error_359"); }
 { Lexer l("// comment 360\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_360"); check(!l.hasErrors(),"comment_error_360"); }
 { Lexer l("/* comment 361 */ var x: int = 361"); auto t=l.lex(); check(!t.isEOF(),"comment_case_361"); check(!l.hasErrors(),"comment_error_361"); }
 { Lexer l("function x() { // inline 362\\n return 362\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_362"); check(!l.hasErrors(),"comment_error_362"); }
 { Lexer l("\\n\\t // leading 363\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_363"); check(!l.hasErrors(),"comment_error_363"); }
 { Lexer l("// comment 364\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_364"); check(!l.hasErrors(),"comment_error_364"); }
 { Lexer l("/* comment 365 */ var x: int = 365"); auto t=l.lex(); check(!t.isEOF(),"comment_case_365"); check(!l.hasErrors(),"comment_error_365"); }
 { Lexer l("function x() { // inline 366\\n return 366\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_366"); check(!l.hasErrors(),"comment_error_366"); }
 { Lexer l("\\n\\t // leading 367\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_367"); check(!l.hasErrors(),"comment_error_367"); }
 { Lexer l("// comment 368\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_368"); check(!l.hasErrors(),"comment_error_368"); }
 { Lexer l("/* comment 369 */ var x: int = 369"); auto t=l.lex(); check(!t.isEOF(),"comment_case_369"); check(!l.hasErrors(),"comment_error_369"); }
 { Lexer l("function x() { // inline 370\\n return 370\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_370"); check(!l.hasErrors(),"comment_error_370"); }
 { Lexer l("\\n\\t // leading 371\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_371"); check(!l.hasErrors(),"comment_error_371"); }
 { Lexer l("// comment 372\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_372"); check(!l.hasErrors(),"comment_error_372"); }
 { Lexer l("/* comment 373 */ var x: int = 373"); auto t=l.lex(); check(!t.isEOF(),"comment_case_373"); check(!l.hasErrors(),"comment_error_373"); }
 { Lexer l("function x() { // inline 374\\n return 374\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_374"); check(!l.hasErrors(),"comment_error_374"); }
 { Lexer l("\\n\\t // leading 375\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_375"); check(!l.hasErrors(),"comment_error_375"); }
 { Lexer l("// comment 376\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_376"); check(!l.hasErrors(),"comment_error_376"); }
 { Lexer l("/* comment 377 */ var x: int = 377"); auto t=l.lex(); check(!t.isEOF(),"comment_case_377"); check(!l.hasErrors(),"comment_error_377"); }
 { Lexer l("function x() { // inline 378\\n return 378\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_378"); check(!l.hasErrors(),"comment_error_378"); }
 { Lexer l("\\n\\t // leading 379\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_379"); check(!l.hasErrors(),"comment_error_379"); }
 { Lexer l("// comment 380\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_380"); check(!l.hasErrors(),"comment_error_380"); }
 { Lexer l("/* comment 381 */ var x: int = 381"); auto t=l.lex(); check(!t.isEOF(),"comment_case_381"); check(!l.hasErrors(),"comment_error_381"); }
 { Lexer l("function x() { // inline 382\\n return 382\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_382"); check(!l.hasErrors(),"comment_error_382"); }
 { Lexer l("\\n\\t // leading 383\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_383"); check(!l.hasErrors(),"comment_error_383"); }
 { Lexer l("// comment 384\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_384"); check(!l.hasErrors(),"comment_error_384"); }
 { Lexer l("/* comment 385 */ var x: int = 385"); auto t=l.lex(); check(!t.isEOF(),"comment_case_385"); check(!l.hasErrors(),"comment_error_385"); }
 { Lexer l("function x() { // inline 386\\n return 386\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_386"); check(!l.hasErrors(),"comment_error_386"); }
 { Lexer l("\\n\\t // leading 387\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_387"); check(!l.hasErrors(),"comment_error_387"); }
 { Lexer l("// comment 388\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_388"); check(!l.hasErrors(),"comment_error_388"); }
 { Lexer l("/* comment 389 */ var x: int = 389"); auto t=l.lex(); check(!t.isEOF(),"comment_case_389"); check(!l.hasErrors(),"comment_error_389"); }
 { Lexer l("function x() { // inline 390\\n return 390\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_390"); check(!l.hasErrors(),"comment_error_390"); }
 { Lexer l("\\n\\t // leading 391\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_391"); check(!l.hasErrors(),"comment_error_391"); }
 { Lexer l("// comment 392\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_392"); check(!l.hasErrors(),"comment_error_392"); }
 { Lexer l("/* comment 393 */ var x: int = 393"); auto t=l.lex(); check(!t.isEOF(),"comment_case_393"); check(!l.hasErrors(),"comment_error_393"); }
 { Lexer l("function x() { // inline 394\\n return 394\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_394"); check(!l.hasErrors(),"comment_error_394"); }
 { Lexer l("\\n\\t // leading 395\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_395"); check(!l.hasErrors(),"comment_error_395"); }
 { Lexer l("// comment 396\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_396"); check(!l.hasErrors(),"comment_error_396"); }
 { Lexer l("/* comment 397 */ var x: int = 397"); auto t=l.lex(); check(!t.isEOF(),"comment_case_397"); check(!l.hasErrors(),"comment_error_397"); }
 { Lexer l("function x() { // inline 398\\n return 398\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_398"); check(!l.hasErrors(),"comment_error_398"); }
 { Lexer l("\\n\\t // leading 399\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_399"); check(!l.hasErrors(),"comment_error_399"); }
 { Lexer l("// comment 400\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_400"); check(!l.hasErrors(),"comment_error_400"); }
 { Lexer l("/* comment 401 */ var x: int = 401"); auto t=l.lex(); check(!t.isEOF(),"comment_case_401"); check(!l.hasErrors(),"comment_error_401"); }
 { Lexer l("function x() { // inline 402\\n return 402\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_402"); check(!l.hasErrors(),"comment_error_402"); }
 { Lexer l("\\n\\t // leading 403\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_403"); check(!l.hasErrors(),"comment_error_403"); }
 { Lexer l("// comment 404\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_404"); check(!l.hasErrors(),"comment_error_404"); }
 { Lexer l("/* comment 405 */ var x: int = 405"); auto t=l.lex(); check(!t.isEOF(),"comment_case_405"); check(!l.hasErrors(),"comment_error_405"); }
 { Lexer l("function x() { // inline 406\\n return 406\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_406"); check(!l.hasErrors(),"comment_error_406"); }
 { Lexer l("\\n\\t // leading 407\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_407"); check(!l.hasErrors(),"comment_error_407"); }
 { Lexer l("// comment 408\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_408"); check(!l.hasErrors(),"comment_error_408"); }
 { Lexer l("/* comment 409 */ var x: int = 409"); auto t=l.lex(); check(!t.isEOF(),"comment_case_409"); check(!l.hasErrors(),"comment_error_409"); }
 { Lexer l("function x() { // inline 410\\n return 410\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_410"); check(!l.hasErrors(),"comment_error_410"); }
 { Lexer l("\\n\\t // leading 411\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_411"); check(!l.hasErrors(),"comment_error_411"); }
 { Lexer l("// comment 412\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_412"); check(!l.hasErrors(),"comment_error_412"); }
 { Lexer l("/* comment 413 */ var x: int = 413"); auto t=l.lex(); check(!t.isEOF(),"comment_case_413"); check(!l.hasErrors(),"comment_error_413"); }
 { Lexer l("function x() { // inline 414\\n return 414\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_414"); check(!l.hasErrors(),"comment_error_414"); }
 { Lexer l("\\n\\t // leading 415\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_415"); check(!l.hasErrors(),"comment_error_415"); }
 { Lexer l("// comment 416\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_416"); check(!l.hasErrors(),"comment_error_416"); }
 { Lexer l("/* comment 417 */ var x: int = 417"); auto t=l.lex(); check(!t.isEOF(),"comment_case_417"); check(!l.hasErrors(),"comment_error_417"); }
 { Lexer l("function x() { // inline 418\\n return 418\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_418"); check(!l.hasErrors(),"comment_error_418"); }
 { Lexer l("\\n\\t // leading 419\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_419"); check(!l.hasErrors(),"comment_error_419"); }
 { Lexer l("// comment 420\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_420"); check(!l.hasErrors(),"comment_error_420"); }
 { Lexer l("/* comment 421 */ var x: int = 421"); auto t=l.lex(); check(!t.isEOF(),"comment_case_421"); check(!l.hasErrors(),"comment_error_421"); }
 { Lexer l("function x() { // inline 422\\n return 422\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_422"); check(!l.hasErrors(),"comment_error_422"); }
 { Lexer l("\\n\\t // leading 423\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_423"); check(!l.hasErrors(),"comment_error_423"); }
 { Lexer l("// comment 424\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_424"); check(!l.hasErrors(),"comment_error_424"); }
 { Lexer l("/* comment 425 */ var x: int = 425"); auto t=l.lex(); check(!t.isEOF(),"comment_case_425"); check(!l.hasErrors(),"comment_error_425"); }
 { Lexer l("function x() { // inline 426\\n return 426\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_426"); check(!l.hasErrors(),"comment_error_426"); }
 { Lexer l("\\n\\t // leading 427\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_427"); check(!l.hasErrors(),"comment_error_427"); }
 { Lexer l("// comment 428\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_428"); check(!l.hasErrors(),"comment_error_428"); }
 { Lexer l("/* comment 429 */ var x: int = 429"); auto t=l.lex(); check(!t.isEOF(),"comment_case_429"); check(!l.hasErrors(),"comment_error_429"); }
 { Lexer l("function x() { // inline 430\\n return 430\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_430"); check(!l.hasErrors(),"comment_error_430"); }
 { Lexer l("\\n\\t // leading 431\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_431"); check(!l.hasErrors(),"comment_error_431"); }
 { Lexer l("// comment 432\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_432"); check(!l.hasErrors(),"comment_error_432"); }
 { Lexer l("/* comment 433 */ var x: int = 433"); auto t=l.lex(); check(!t.isEOF(),"comment_case_433"); check(!l.hasErrors(),"comment_error_433"); }
 { Lexer l("function x() { // inline 434\\n return 434\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_434"); check(!l.hasErrors(),"comment_error_434"); }
 { Lexer l("\\n\\t // leading 435\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_435"); check(!l.hasErrors(),"comment_error_435"); }
 { Lexer l("// comment 436\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_436"); check(!l.hasErrors(),"comment_error_436"); }
 { Lexer l("/* comment 437 */ var x: int = 437"); auto t=l.lex(); check(!t.isEOF(),"comment_case_437"); check(!l.hasErrors(),"comment_error_437"); }
 { Lexer l("function x() { // inline 438\\n return 438\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_438"); check(!l.hasErrors(),"comment_error_438"); }
 { Lexer l("\\n\\t // leading 439\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_439"); check(!l.hasErrors(),"comment_error_439"); }
 { Lexer l("// comment 440\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_440"); check(!l.hasErrors(),"comment_error_440"); }
 { Lexer l("/* comment 441 */ var x: int = 441"); auto t=l.lex(); check(!t.isEOF(),"comment_case_441"); check(!l.hasErrors(),"comment_error_441"); }
 { Lexer l("function x() { // inline 442\\n return 442\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_442"); check(!l.hasErrors(),"comment_error_442"); }
 { Lexer l("\\n\\t // leading 443\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_443"); check(!l.hasErrors(),"comment_error_443"); }
 { Lexer l("// comment 444\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_444"); check(!l.hasErrors(),"comment_error_444"); }
 { Lexer l("/* comment 445 */ var x: int = 445"); auto t=l.lex(); check(!t.isEOF(),"comment_case_445"); check(!l.hasErrors(),"comment_error_445"); }
 { Lexer l("function x() { // inline 446\\n return 446\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_446"); check(!l.hasErrors(),"comment_error_446"); }
 { Lexer l("\\n\\t // leading 447\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_447"); check(!l.hasErrors(),"comment_error_447"); }
 { Lexer l("// comment 448\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_448"); check(!l.hasErrors(),"comment_error_448"); }
 { Lexer l("/* comment 449 */ var x: int = 449"); auto t=l.lex(); check(!t.isEOF(),"comment_case_449"); check(!l.hasErrors(),"comment_error_449"); }
 { Lexer l("function x() { // inline 450\\n return 450\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_450"); check(!l.hasErrors(),"comment_error_450"); }
 { Lexer l("\\n\\t // leading 451\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_451"); check(!l.hasErrors(),"comment_error_451"); }
 { Lexer l("// comment 452\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_452"); check(!l.hasErrors(),"comment_error_452"); }
 { Lexer l("/* comment 453 */ var x: int = 453"); auto t=l.lex(); check(!t.isEOF(),"comment_case_453"); check(!l.hasErrors(),"comment_error_453"); }
 { Lexer l("function x() { // inline 454\\n return 454\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_454"); check(!l.hasErrors(),"comment_error_454"); }
 { Lexer l("\\n\\t // leading 455\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_455"); check(!l.hasErrors(),"comment_error_455"); }
 { Lexer l("// comment 456\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_456"); check(!l.hasErrors(),"comment_error_456"); }
 { Lexer l("/* comment 457 */ var x: int = 457"); auto t=l.lex(); check(!t.isEOF(),"comment_case_457"); check(!l.hasErrors(),"comment_error_457"); }
 { Lexer l("function x() { // inline 458\\n return 458\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_458"); check(!l.hasErrors(),"comment_error_458"); }
 { Lexer l("\\n\\t // leading 459\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_459"); check(!l.hasErrors(),"comment_error_459"); }
 { Lexer l("// comment 460\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_460"); check(!l.hasErrors(),"comment_error_460"); }
 { Lexer l("/* comment 461 */ var x: int = 461"); auto t=l.lex(); check(!t.isEOF(),"comment_case_461"); check(!l.hasErrors(),"comment_error_461"); }
 { Lexer l("function x() { // inline 462\\n return 462\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_462"); check(!l.hasErrors(),"comment_error_462"); }
 { Lexer l("\\n\\t // leading 463\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_463"); check(!l.hasErrors(),"comment_error_463"); }
 { Lexer l("// comment 464\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_464"); check(!l.hasErrors(),"comment_error_464"); }
 { Lexer l("/* comment 465 */ var x: int = 465"); auto t=l.lex(); check(!t.isEOF(),"comment_case_465"); check(!l.hasErrors(),"comment_error_465"); }
 { Lexer l("function x() { // inline 466\\n return 466\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_466"); check(!l.hasErrors(),"comment_error_466"); }
 { Lexer l("\\n\\t // leading 467\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_467"); check(!l.hasErrors(),"comment_error_467"); }
 { Lexer l("// comment 468\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_468"); check(!l.hasErrors(),"comment_error_468"); }
 { Lexer l("/* comment 469 */ var x: int = 469"); auto t=l.lex(); check(!t.isEOF(),"comment_case_469"); check(!l.hasErrors(),"comment_error_469"); }
 { Lexer l("function x() { // inline 470\\n return 470\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_470"); check(!l.hasErrors(),"comment_error_470"); }
 { Lexer l("\\n\\t // leading 471\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_471"); check(!l.hasErrors(),"comment_error_471"); }
 { Lexer l("// comment 472\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_472"); check(!l.hasErrors(),"comment_error_472"); }
 { Lexer l("/* comment 473 */ var x: int = 473"); auto t=l.lex(); check(!t.isEOF(),"comment_case_473"); check(!l.hasErrors(),"comment_error_473"); }
 { Lexer l("function x() { // inline 474\\n return 474\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_474"); check(!l.hasErrors(),"comment_error_474"); }
 { Lexer l("\\n\\t // leading 475\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_475"); check(!l.hasErrors(),"comment_error_475"); }
 { Lexer l("// comment 476\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_476"); check(!l.hasErrors(),"comment_error_476"); }
 { Lexer l("/* comment 477 */ var x: int = 477"); auto t=l.lex(); check(!t.isEOF(),"comment_case_477"); check(!l.hasErrors(),"comment_error_477"); }
 { Lexer l("function x() { // inline 478\\n return 478\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_478"); check(!l.hasErrors(),"comment_error_478"); }
 { Lexer l("\\n\\t // leading 479\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_479"); check(!l.hasErrors(),"comment_error_479"); }
 { Lexer l("// comment 480\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_480"); check(!l.hasErrors(),"comment_error_480"); }
 { Lexer l("/* comment 481 */ var x: int = 481"); auto t=l.lex(); check(!t.isEOF(),"comment_case_481"); check(!l.hasErrors(),"comment_error_481"); }
 { Lexer l("function x() { // inline 482\\n return 482\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_482"); check(!l.hasErrors(),"comment_error_482"); }
 { Lexer l("\\n\\t // leading 483\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_483"); check(!l.hasErrors(),"comment_error_483"); }
 { Lexer l("// comment 484\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_484"); check(!l.hasErrors(),"comment_error_484"); }
 { Lexer l("/* comment 485 */ var x: int = 485"); auto t=l.lex(); check(!t.isEOF(),"comment_case_485"); check(!l.hasErrors(),"comment_error_485"); }
 { Lexer l("function x() { // inline 486\\n return 486\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_486"); check(!l.hasErrors(),"comment_error_486"); }
 { Lexer l("\\n\\t // leading 487\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_487"); check(!l.hasErrors(),"comment_error_487"); }
 { Lexer l("// comment 488\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_488"); check(!l.hasErrors(),"comment_error_488"); }
 { Lexer l("/* comment 489 */ var x: int = 489"); auto t=l.lex(); check(!t.isEOF(),"comment_case_489"); check(!l.hasErrors(),"comment_error_489"); }
 { Lexer l("function x() { // inline 490\\n return 490\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_490"); check(!l.hasErrors(),"comment_error_490"); }
 { Lexer l("\\n\\t // leading 491\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_491"); check(!l.hasErrors(),"comment_error_491"); }
 { Lexer l("// comment 492\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_492"); check(!l.hasErrors(),"comment_error_492"); }
 { Lexer l("/* comment 493 */ var x: int = 493"); auto t=l.lex(); check(!t.isEOF(),"comment_case_493"); check(!l.hasErrors(),"comment_error_493"); }
 { Lexer l("function x() { // inline 494\\n return 494\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_494"); check(!l.hasErrors(),"comment_error_494"); }
 { Lexer l("\\n\\t // leading 495\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_495"); check(!l.hasErrors(),"comment_error_495"); }
 { Lexer l("// comment 496\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_496"); check(!l.hasErrors(),"comment_error_496"); }
 { Lexer l("/* comment 497 */ var x: int = 497"); auto t=l.lex(); check(!t.isEOF(),"comment_case_497"); check(!l.hasErrors(),"comment_error_497"); }
 { Lexer l("function x() { // inline 498\\n return 498\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_498"); check(!l.hasErrors(),"comment_error_498"); }
 { Lexer l("\\n\\t // leading 499\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_499"); check(!l.hasErrors(),"comment_error_499"); }
 { Lexer l("// comment 500\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_500"); check(!l.hasErrors(),"comment_error_500"); }
 { Lexer l("/* comment 501 */ var x: int = 501"); auto t=l.lex(); check(!t.isEOF(),"comment_case_501"); check(!l.hasErrors(),"comment_error_501"); }
 { Lexer l("function x() { // inline 502\\n return 502\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_502"); check(!l.hasErrors(),"comment_error_502"); }
 { Lexer l("\\n\\t // leading 503\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_503"); check(!l.hasErrors(),"comment_error_503"); }
 { Lexer l("// comment 504\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_504"); check(!l.hasErrors(),"comment_error_504"); }
 { Lexer l("/* comment 505 */ var x: int = 505"); auto t=l.lex(); check(!t.isEOF(),"comment_case_505"); check(!l.hasErrors(),"comment_error_505"); }
 { Lexer l("function x() { // inline 506\\n return 506\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_506"); check(!l.hasErrors(),"comment_error_506"); }
 { Lexer l("\\n\\t // leading 507\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_507"); check(!l.hasErrors(),"comment_error_507"); }
 { Lexer l("// comment 508\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_508"); check(!l.hasErrors(),"comment_error_508"); }
 { Lexer l("/* comment 509 */ var x: int = 509"); auto t=l.lex(); check(!t.isEOF(),"comment_case_509"); check(!l.hasErrors(),"comment_error_509"); }
 { Lexer l("function x() { // inline 510\\n return 510\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_510"); check(!l.hasErrors(),"comment_error_510"); }
 { Lexer l("\\n\\t // leading 511\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_511"); check(!l.hasErrors(),"comment_error_511"); }
 { Lexer l("// comment 512\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_512"); check(!l.hasErrors(),"comment_error_512"); }
 { Lexer l("/* comment 513 */ var x: int = 513"); auto t=l.lex(); check(!t.isEOF(),"comment_case_513"); check(!l.hasErrors(),"comment_error_513"); }
 { Lexer l("function x() { // inline 514\\n return 514\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_514"); check(!l.hasErrors(),"comment_error_514"); }
 { Lexer l("\\n\\t // leading 515\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_515"); check(!l.hasErrors(),"comment_error_515"); }
 { Lexer l("// comment 516\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_516"); check(!l.hasErrors(),"comment_error_516"); }
 { Lexer l("/* comment 517 */ var x: int = 517"); auto t=l.lex(); check(!t.isEOF(),"comment_case_517"); check(!l.hasErrors(),"comment_error_517"); }
 { Lexer l("function x() { // inline 518\\n return 518\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_518"); check(!l.hasErrors(),"comment_error_518"); }
 { Lexer l("\\n\\t // leading 519\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_519"); check(!l.hasErrors(),"comment_error_519"); }
 { Lexer l("// comment 520\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_520"); check(!l.hasErrors(),"comment_error_520"); }
 { Lexer l("/* comment 521 */ var x: int = 521"); auto t=l.lex(); check(!t.isEOF(),"comment_case_521"); check(!l.hasErrors(),"comment_error_521"); }
 { Lexer l("function x() { // inline 522\\n return 522\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_522"); check(!l.hasErrors(),"comment_error_522"); }
 { Lexer l("\\n\\t // leading 523\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_523"); check(!l.hasErrors(),"comment_error_523"); }
 { Lexer l("// comment 524\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_524"); check(!l.hasErrors(),"comment_error_524"); }
 { Lexer l("/* comment 525 */ var x: int = 525"); auto t=l.lex(); check(!t.isEOF(),"comment_case_525"); check(!l.hasErrors(),"comment_error_525"); }
 { Lexer l("function x() { // inline 526\\n return 526\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_526"); check(!l.hasErrors(),"comment_error_526"); }
 { Lexer l("\\n\\t // leading 527\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_527"); check(!l.hasErrors(),"comment_error_527"); }
 { Lexer l("// comment 528\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_528"); check(!l.hasErrors(),"comment_error_528"); }
 { Lexer l("/* comment 529 */ var x: int = 529"); auto t=l.lex(); check(!t.isEOF(),"comment_case_529"); check(!l.hasErrors(),"comment_error_529"); }
 { Lexer l("function x() { // inline 530\\n return 530\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_530"); check(!l.hasErrors(),"comment_error_530"); }
 { Lexer l("\\n\\t // leading 531\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_531"); check(!l.hasErrors(),"comment_error_531"); }
 { Lexer l("// comment 532\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_532"); check(!l.hasErrors(),"comment_error_532"); }
 { Lexer l("/* comment 533 */ var x: int = 533"); auto t=l.lex(); check(!t.isEOF(),"comment_case_533"); check(!l.hasErrors(),"comment_error_533"); }
 { Lexer l("function x() { // inline 534\\n return 534\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_534"); check(!l.hasErrors(),"comment_error_534"); }
 { Lexer l("\\n\\t // leading 535\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_535"); check(!l.hasErrors(),"comment_error_535"); }
 { Lexer l("// comment 536\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_536"); check(!l.hasErrors(),"comment_error_536"); }
 { Lexer l("/* comment 537 */ var x: int = 537"); auto t=l.lex(); check(!t.isEOF(),"comment_case_537"); check(!l.hasErrors(),"comment_error_537"); }
 { Lexer l("function x() { // inline 538\\n return 538\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_538"); check(!l.hasErrors(),"comment_error_538"); }
 { Lexer l("\\n\\t // leading 539\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_539"); check(!l.hasErrors(),"comment_error_539"); }
 { Lexer l("// comment 540\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_540"); check(!l.hasErrors(),"comment_error_540"); }
 { Lexer l("/* comment 541 */ var x: int = 541"); auto t=l.lex(); check(!t.isEOF(),"comment_case_541"); check(!l.hasErrors(),"comment_error_541"); }
 { Lexer l("function x() { // inline 542\\n return 542\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_542"); check(!l.hasErrors(),"comment_error_542"); }
 { Lexer l("\\n\\t // leading 543\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_543"); check(!l.hasErrors(),"comment_error_543"); }
 { Lexer l("// comment 544\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_544"); check(!l.hasErrors(),"comment_error_544"); }
 { Lexer l("/* comment 545 */ var x: int = 545"); auto t=l.lex(); check(!t.isEOF(),"comment_case_545"); check(!l.hasErrors(),"comment_error_545"); }
 { Lexer l("function x() { // inline 546\\n return 546\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_546"); check(!l.hasErrors(),"comment_error_546"); }
 { Lexer l("\\n\\t // leading 547\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_547"); check(!l.hasErrors(),"comment_error_547"); }
 { Lexer l("// comment 548\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_548"); check(!l.hasErrors(),"comment_error_548"); }
 { Lexer l("/* comment 549 */ var x: int = 549"); auto t=l.lex(); check(!t.isEOF(),"comment_case_549"); check(!l.hasErrors(),"comment_error_549"); }
 { Lexer l("function x() { // inline 550\\n return 550\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_550"); check(!l.hasErrors(),"comment_error_550"); }
 { Lexer l("\\n\\t // leading 551\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_551"); check(!l.hasErrors(),"comment_error_551"); }
 { Lexer l("// comment 552\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_552"); check(!l.hasErrors(),"comment_error_552"); }
 { Lexer l("/* comment 553 */ var x: int = 553"); auto t=l.lex(); check(!t.isEOF(),"comment_case_553"); check(!l.hasErrors(),"comment_error_553"); }
 { Lexer l("function x() { // inline 554\\n return 554\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_554"); check(!l.hasErrors(),"comment_error_554"); }
 { Lexer l("\\n\\t // leading 555\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_555"); check(!l.hasErrors(),"comment_error_555"); }
 { Lexer l("// comment 556\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_556"); check(!l.hasErrors(),"comment_error_556"); }
 { Lexer l("/* comment 557 */ var x: int = 557"); auto t=l.lex(); check(!t.isEOF(),"comment_case_557"); check(!l.hasErrors(),"comment_error_557"); }
 { Lexer l("function x() { // inline 558\\n return 558\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_558"); check(!l.hasErrors(),"comment_error_558"); }
 { Lexer l("\\n\\t // leading 559\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_559"); check(!l.hasErrors(),"comment_error_559"); }
 { Lexer l("// comment 560\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_560"); check(!l.hasErrors(),"comment_error_560"); }
 { Lexer l("/* comment 561 */ var x: int = 561"); auto t=l.lex(); check(!t.isEOF(),"comment_case_561"); check(!l.hasErrors(),"comment_error_561"); }
 { Lexer l("function x() { // inline 562\\n return 562\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_562"); check(!l.hasErrors(),"comment_error_562"); }
 { Lexer l("\\n\\t // leading 563\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_563"); check(!l.hasErrors(),"comment_error_563"); }
 { Lexer l("// comment 564\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_564"); check(!l.hasErrors(),"comment_error_564"); }
 { Lexer l("/* comment 565 */ var x: int = 565"); auto t=l.lex(); check(!t.isEOF(),"comment_case_565"); check(!l.hasErrors(),"comment_error_565"); }
 { Lexer l("function x() { // inline 566\\n return 566\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_566"); check(!l.hasErrors(),"comment_error_566"); }
 { Lexer l("\\n\\t // leading 567\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_567"); check(!l.hasErrors(),"comment_error_567"); }
 { Lexer l("// comment 568\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_568"); check(!l.hasErrors(),"comment_error_568"); }
 { Lexer l("/* comment 569 */ var x: int = 569"); auto t=l.lex(); check(!t.isEOF(),"comment_case_569"); check(!l.hasErrors(),"comment_error_569"); }
 { Lexer l("function x() { // inline 570\\n return 570\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_570"); check(!l.hasErrors(),"comment_error_570"); }
 { Lexer l("\\n\\t // leading 571\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_571"); check(!l.hasErrors(),"comment_error_571"); }
 { Lexer l("// comment 572\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_572"); check(!l.hasErrors(),"comment_error_572"); }
 { Lexer l("/* comment 573 */ var x: int = 573"); auto t=l.lex(); check(!t.isEOF(),"comment_case_573"); check(!l.hasErrors(),"comment_error_573"); }
 { Lexer l("function x() { // inline 574\\n return 574\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_574"); check(!l.hasErrors(),"comment_error_574"); }
 { Lexer l("\\n\\t // leading 575\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_575"); check(!l.hasErrors(),"comment_error_575"); }
 { Lexer l("// comment 576\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_576"); check(!l.hasErrors(),"comment_error_576"); }
 { Lexer l("/* comment 577 */ var x: int = 577"); auto t=l.lex(); check(!t.isEOF(),"comment_case_577"); check(!l.hasErrors(),"comment_error_577"); }
 { Lexer l("function x() { // inline 578\\n return 578\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_578"); check(!l.hasErrors(),"comment_error_578"); }
 { Lexer l("\\n\\t // leading 579\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_579"); check(!l.hasErrors(),"comment_error_579"); }
 { Lexer l("// comment 580\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_580"); check(!l.hasErrors(),"comment_error_580"); }
 { Lexer l("/* comment 581 */ var x: int = 581"); auto t=l.lex(); check(!t.isEOF(),"comment_case_581"); check(!l.hasErrors(),"comment_error_581"); }
 { Lexer l("function x() { // inline 582\\n return 582\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_582"); check(!l.hasErrors(),"comment_error_582"); }
 { Lexer l("\\n\\t // leading 583\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_583"); check(!l.hasErrors(),"comment_error_583"); }
 { Lexer l("// comment 584\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_584"); check(!l.hasErrors(),"comment_error_584"); }
 { Lexer l("/* comment 585 */ var x: int = 585"); auto t=l.lex(); check(!t.isEOF(),"comment_case_585"); check(!l.hasErrors(),"comment_error_585"); }
 { Lexer l("function x() { // inline 586\\n return 586\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_586"); check(!l.hasErrors(),"comment_error_586"); }
 { Lexer l("\\n\\t // leading 587\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_587"); check(!l.hasErrors(),"comment_error_587"); }
 { Lexer l("// comment 588\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_588"); check(!l.hasErrors(),"comment_error_588"); }
 { Lexer l("/* comment 589 */ var x: int = 589"); auto t=l.lex(); check(!t.isEOF(),"comment_case_589"); check(!l.hasErrors(),"comment_error_589"); }
 { Lexer l("function x() { // inline 590\\n return 590\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_590"); check(!l.hasErrors(),"comment_error_590"); }
 { Lexer l("\\n\\t // leading 591\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_591"); check(!l.hasErrors(),"comment_error_591"); }
 { Lexer l("// comment 592\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_592"); check(!l.hasErrors(),"comment_error_592"); }
 { Lexer l("/* comment 593 */ var x: int = 593"); auto t=l.lex(); check(!t.isEOF(),"comment_case_593"); check(!l.hasErrors(),"comment_error_593"); }
 { Lexer l("function x() { // inline 594\\n return 594\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_594"); check(!l.hasErrors(),"comment_error_594"); }
 { Lexer l("\\n\\t // leading 595\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_595"); check(!l.hasErrors(),"comment_error_595"); }
 { Lexer l("// comment 596\\nfunction x() {}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_596"); check(!l.hasErrors(),"comment_error_596"); }
 { Lexer l("/* comment 597 */ var x: int = 597"); auto t=l.lex(); check(!t.isEOF(),"comment_case_597"); check(!l.hasErrors(),"comment_error_597"); }
 { Lexer l("function x() { // inline 598\\n return 598\\n}"); auto t=l.lex(); check(!t.isEOF(),"comment_case_598"); check(!l.hasErrors(),"comment_error_598"); }
 { Lexer l("\\n\\t // leading 599\\n const x: string = \"value\""); auto t=l.lex(); check(!t.isEOF(),"comment_case_599"); check(!l.hasErrors(),"comment_error_599"); }
 return failures?1:0; }
