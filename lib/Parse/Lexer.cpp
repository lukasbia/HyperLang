#include "hyper/Lexer.h"
#include <cctype>
#include <cstring>
#include <unordered_map>

namespace hyper {
namespace {
using K=TokenKind;
const std::unordered_map<std::string_view,K> keywords={
 {"if",K::KwIf},{"else",K::KwElse},{"while",K::KwWhile},{"do",K::KwDo},{"for",K::KwFor},{"in",K::KwIn},
 {"func",K::KwFunc},{"var",K::KwVar},{"let",K::KwLet},{"const",K::KwConst},{"return",K::KwReturn},
 {"struct",K::KwStruct},{"class",K::KwClass},{"enum",K::KwEnum},{"protocol",K::KwProtocol},{"extension",K::KwExtension},{"import",K::KwImport},
 {"module",K::KwModule},{"package",K::KwPackage},{"namespace",K::KwNamespace},{"public",K::KwPublic},{"private",K::KwPrivate},{"internal",K::KwInternal},{"protected",K::KwProtected},
 {"static",K::KwStatic},{"final",K::KwFinal},{"override",K::KwOverride},{"init",K::KwInit},{"deinit",K::KwDeinit},{"defer",K::KwDefer},
 {"break",K::KwBreak},{"continue",K::KwContinue},{"true",K::KwTrue},{"false",K::KwFalse},{"nil",K::KwNil},{"and",K::KwAnd},{"or",K::KwOr},{"not",K::KwNot},
 {"move",K::KwMove},{"borrow",K::KwBorrow},{"consume",K::KwConsume},{"copy",K::KwCopy},{"own",K::KwOwn},{"shared",K::KwShared},{"weak",K::KwWeak},{"unowned",K::KwUnowned},{"auto",K::KwAuto},
 {"type",K::KwType},{"typealias",K::KwTypealias},{"associatedtype",K::KwAssociatedType},{"int",K::KwInt},{"float",K::KwFloat},{"bool",K::KwBool},{"string",K::KwString},{"bytes",K::KwBytes},{"num",K::KwNum},
 {"guard",K::KwGuard},{"switch",K::KwSwitch},{"case",K::KwCase},{"default",K::KwDefault},{"throw",K::KwThrow},{"throws",K::KwThrows},{"catch",K::KwCatch},
 {"async",K::KwAsync},{"await",K::KwAwait},{"task",K::KwTask},{"actor",K::KwActor},{"some",K::KwSome},{"any",K::KwAny},{"self",K::KwSelf},
 {"where",K::KwWhere},{"get",K::KwGet},{"set",K::KwSet},{"mutating",K::KwMutating},{"operator",K::KwOperator},{"subscript",K::KwSubscript},{"yield",K::KwYield},
 {"macro",K::KwMacro},{"attribute",K::KwAttribute},{"observe",K::KwObserve},{"synchronize",K::KwSynchronize},{"compile",K::KwCompile},{"extern",K::KwExtern},
 {"output",K::KwOutput},{"input",K::KwInput},{"panic",K::KwPanic},{"loop",K::KwLoop},{"endloop",K::KwEndLoop},{"then",K::KwThen},{"endif",K::KwEndIf},
 {"source",K::KwSource},{"file",K::KwFile},{"function",K::KwFunction},{"property",K::KwProperty},{"event",K::KwEvent},{"signal",K::KwSignal},{"detached",K::KwDetached},
 {"isolated",K::KwIsolated},{"nonisolated",K::KwNonisolated},{"sendable",K::KwSendable}
};
struct Op { std::string_view text; K kind; };
const Op ops[]={
 {">>=",K::ShiftRightEqual},{"<<=",K::ShiftLeftEqual},{"===",K::EqualEqual},{"!==",K::NotEqual},{"...",K::ClosedRange},{"..<",K::Range},{"??",K::QuestionQuestion},{"=>",K::FatArrow},{"->",K::Arrow},{"==",K::EqualEqual},{"!=",K::NotEqual},{">=",K::GreaterEqual},{"<=",K::LessEqual},{"+=",K::PlusEqual},{"-=",K::MinusEqual},{"*=",K::StarEqual},{"/=",K::SlashEqual},{"%=",K::PercentEqual},{"&=",K::AmpersandEqual},{"|=",K::PipeEqual},{"^=",K::CaretEqual},{"<<",K::ShiftLeft},{">>",K::ShiftRight},{"&&",K::AndAnd},{"||",K::OrOr},{"~=",K::TildeEqual},{"**",K::Power}
};
bool idStart(char c){return std::isalpha((unsigned char)c)||c=='_';}
bool idPart(char c){return std::isalnum((unsigned char)c)||c=='_';}
bool digit(char c){return std::isdigit((unsigned char)c);}
bool hex(char c){return std::isxdigit((unsigned char)c);}
}

Lexer::Lexer(std::string_view s):source_(s){}
char Lexer::peek(std::size_t d) const noexcept{return current_+d<source_.size()?source_[current_+d]:'\0';}
char Lexer::advance() noexcept{char c=peek();if(!c)return c;++current_;updateLineState(c);return c;}
bool Lexer::match(char c) noexcept{if(peek()!=c)return false;advance();return true;}
bool Lexer::startsWith(std::string_view s) const noexcept{return current_+s.size()<=source_.size()&&source_.substr(current_,s.size())==s;}
void Lexer::updateLineState(char c) noexcept{if(c=='\n'){++line_;column_=1;atStartOfLine_=true;}else{++column_;if(c!='\r'&&c!=' '&&c!='\t')atStartOfLine_=false;}}
SourceLocation Lexer::locationFromOffset(std::size_t p) const{SourceLocation l{0,1,1};for(std::size_t i=0;i<p&&i<source_.size();++i){++l.offset;if(source_[i]=='\n'){++l.line;l.column=1;}else ++l.column;}return l;}
Token Lexer::makeToken(K k,std::size_t s,SourceLocation l,bool bad) const{Token t;t.kind=k;t.text=std::string(source_.substr(s,current_-s));t.location=l;t.malformed=bad;t.atStartOfLine=l.column==1;t.hasLeadingComment=hadLeadingComment_;return t;}

K Lexer::keywordKind(std::string_view s) noexcept{auto i=keywords.find(s);return i==keywords.end()?K::Identifier:i->second;}
K Lexer::punctuationKind(std::string_view s) noexcept{if(s==".")return K::Dot;if(s==",")return K::Comma;if(s==":")return K::Colon;if(s=="::")return K::DoubleColon;if(s==";")return K::Semicolon;if(s=="\\")return K::Backslash;if(s=="(")return K::LeftParen;if(s==")")return K::RightParen;if(s=="{")return K::LeftBrace;if(s=="}")return K::RightBrace;if(s=="[")return K::LeftBracket;if(s=="]")return K::RightBracket;return K::Unknown;}
K Lexer::operatorKind(std::string_view s) noexcept{for(const auto&o:ops)if(o.text==s)return o.kind;if(s=="+")return K::Plus;if(s=="-")return K::Minus;if(s=="*")return K::Star;if(s=="/")return K::Slash;if(s=="%")return K::Percent;if(s=="&")return K::Ampersand;if(s=="|")return K::Pipe;if(s=="^")return K::Caret;if(s=="~")return K::Tilde;if(s=="!")return K::Bang;if(s=="?")return K::Question;if(s=="=")return K::Equal;if(s==">")return K::Greater;if(s=="<")return K::Less;return K::Unknown;}

Token Lexer::lexIdentifierOrKeyword(){auto s=current_;auto l=SourceLocation{current_,line_,column_};while(idPart(peek()))advance();auto text=source_.substr(s,current_-s);return makeToken(keywordKind(text),s,l);}
Token Lexer::lexEscapedIdentifier(){auto s=current_;auto l=SourceLocation{current_,line_,column_};advance();while(peek()&&peek()!='`')advance();bool bad=!match('`');return makeToken(bad?K::Unknown:K::EscapedIdentifier,s,l,bad);}
Token Lexer::lexDollarIdentifier(){auto s=current_;auto l=SourceLocation{current_,line_,column_};advance();while(idPart(peek()))advance();return makeToken(K::DollarIdentifier,s,l);}

bool Lexer::scanDecimalDigits(std::size_t&p,bool allow,std::size_t&n,bool&sep){n=0;sep=false;bool last=false;while(p<source_.size()){char c=source_[p];if(digit(c)){++n;++p;last=true;continue;}if(c=='_'&&allow){sep=true;++p;if(!last||p>=source_.size()||!digit(source_[p]))diagnoseWarning(locationFromOffset(p-1),"numeric separator must separate digits");last=false;continue;}break;}return n>0;}
bool Lexer::scanBasedDigits(std::size_t&p,NumberBase b,std::size_t&n,bool&sep){n=0;sep=false;auto valid=[b](char c){if(b==NumberBase::Binary)return c=='0'||c=='1';if(b==NumberBase::Octal)return c>='0'&&c<='7';return std::isxdigit((unsigned char)c)!=0;};while(p<source_.size()&&(valid(source_[p])||source_[p]=='_')){if(source_[p]=='_')sep=true;else ++n;++p;}return n>0;}
Token Lexer::lexNumber(){auto s=current_;auto l=SourceLocation{current_,line_,column_};NumericLiteralInfo info;K kind=K::IntegerLiteral;std::size_t p=current_;std::size_t n=0;bool sep=false;if(startsWith("0b")||startsWith("0B")){advance();advance();p=current_;scanBasedDigits(p,NumberBase::Binary,n,sep);while(current_<p)advance();info.base=NumberBase::Binary;kind=K::BinaryIntegerLiteral;}else if(startsWith("0o")||startsWith("0O")){advance();advance();p=current_;scanBasedDigits(p,NumberBase::Octal,n,sep);while(current_<p)advance();info.base=NumberBase::Octal;kind=K::OctalIntegerLiteral;}else if(startsWith("0x")||startsWith("0X")){advance();advance();p=current_;scanBasedDigits(p,NumberBase::Hexadecimal,n,sep);while(current_<p)advance();info.base=NumberBase::Hexadecimal;if(peek()=='.'&&hex(peek(1))){kind=K::HexFloatLiteral;advance();while(hex(peek())||peek()=='_')advance();}else kind=K::HexIntegerLiteral;}else{while(digit(peek())||peek()=='_')advance();if(peek()=='.'&&digit(peek(1))){info.hasDecimalPoint=true;kind=K::FloatLiteral;advance();while(digit(peek())||peek()=='_')advance();}if(peek()=='e'||peek()=='E'){info.hasExponent=true;kind=K::FloatLiteral;advance();if(peek()=='+'||peek()=='-')advance();while(digit(peek())||peek()=='_')advance();}info.base=NumberBase::Decimal;}info.hasSeparators=sep;Token t=makeToken(kind,s,l);t.numeric=info;return t;}

Token Lexer::lexString(){auto s=current_;auto l=SourceLocation{current_,line_,column_};bool multi=startsWith("\"\"\"");StringLiteralInfo info;info.kind=multi?StringKind::Multiline:StringKind::Normal;if(multi){advance();advance();advance();}else advance();while(peek()){if(multi&&startsWith("\"\"\"")){advance();advance();advance();break;}if(!multi&&peek()=='\"'){advance();break;}if(peek()=='\\'){info.hasEscapes=true;advance();if(peek())advance();continue;}if(peek()=='$'&&peek(1)=='(')info.hasInterpolation=true;advance();}bool bad=multi?!source_.substr(s,current_-s).ends_with("\"\"\""):source_.substr(s,current_-s).size()<2||source_[current_-1]!='\"';return makeToken(bad?K::Unknown:(multi?K::MultilineStringLiteral:K::StringLiteral),s,l,bad);}
Token Lexer::lexCharacter(){auto s=current_;auto l=SourceLocation{current_,line_,column_};advance();if(peek()=='\\'){advance();if(peek())advance();}else if(peek()&&peek()!='\n'&&peek()!='\'')advance();bool bad=!match('\'');return makeToken(bad?K::Unknown:K::CharacterLiteral,s,l,bad);}
Token Lexer::lexRawString(unsigned d){auto s=current_;auto l=SourceLocation{current_,line_,column_};for(unsigned i=0;i<d;++i)advance();if(peek()=='"')advance();while(peek()){if(peek()=='"'){bool ok=true;for(unsigned i=0;i<d;++i)if(peek(1+i)!='#')ok=false;if(ok){advance();for(unsigned i=0;i<d;++i)advance();break;}}advance();}return makeToken(K::StringLiteral,s,l);}

bool Lexer::lexLineComment(){if(!startsWith("//"))return false;auto s=current_;auto l=SourceLocation{current_,line_,column_};advance();advance();while(peek()&&peek()!='\n')advance();hadLeadingComment_=true;if(keepComments_){Token t=makeToken(K::Comment,s,l);lookaheadBuffer_.push_back(std::move(t));}return true;}
bool Lexer::lexBlockComment(){if(!startsWith("/*"))return false;auto s=current_;auto l=SourceLocation{current_,line_,column_};advance();advance();while(peek()&&!startsWith("*/"))advance();bool bad=!peek();if(!bad){advance();advance();}hadLeadingComment_=true;if(keepComments_){Token t=makeToken(bad?K::Unknown:K::Comment,s,l,bad);lookaheadBuffer_.push_back(std::move(t));}if(bad)diagnoseError(l,"unterminated block comment");return true;}
bool Lexer::lexHashbang(){if(current_!=0||!startsWith("#!"))return false;auto s=current_;auto l=SourceLocation{current_,line_,column_};while(peek()&&peek()!='\n')advance();if(keepComments_)lookaheadBuffer_.push_back(makeToken(K::Hashbang,s,l));return true;}
bool Lexer::lexConflictMarker(){if(!atStartOfLine_)return false;if(startsWith("<<<<<<<")||startsWith(">>>>>>>")||startsWith("=======")){auto s=current_;auto l=SourceLocation{current_,line_,column_};while(peek()&&peek()!='\n')advance();lookaheadBuffer_.push_back(makeToken(K::ConflictMarker,s,l));return true;}return false;}
void Lexer::consumeTrivia(){hadLeadingComment_=false;for(;;){while(std::isspace((unsigned char)peek()))advance();if(lexHashbang()||lexLineComment()||lexBlockComment()||lexConflictMarker())continue;break;}}

Token Lexer::lexOperator(){auto s=current_;auto l=SourceLocation{current_,line_,column_};for(const auto&o:ops){if(startsWith(o.text)){for(char c:o.text)advance();Token t=makeToken(o.kind,s,l);t.binding=classifyOperatorBinding(source_,s,current_);return t;}}advance();std::string_view one=source_.substr(s,1);Token t=makeToken(operatorKind(one),s,l);t.binding=classifyOperatorBinding(source_,s,current_);return t;}
Token Lexer::lexDirective(){auto s=current_;auto l=SourceLocation{current_,line_,column_};advance();while(idPart(peek())||peek()=='-')advance();return makeToken(K::Directive,s,l);}
Token Lexer::lexHashConstruct(){if(peek()=='#'&&idStart(peek(1)))return lexDirective();auto s=current_;auto l=SourceLocation{current_,line_,column_};advance();return makeToken(K::Hash,s,l);}
Token Lexer::lexUnknown(){auto s=current_;auto l=SourceLocation{current_,line_,column_};char c=advance();diagnoseError(l,std::string("unexpected character '")+c+"'");return makeToken(K::Unknown,s,l,true);}

Token Lexer::next(){if(!lookaheadBuffer_.empty()){Token t=std::move(lookaheadBuffer_.front());lookaheadBuffer_.erase(lookaheadBuffer_.begin());return t;}consumeTrivia();auto s=current_;auto l=SourceLocation{current_,line_,column_};char c=peek();if(!c)return makeToken(K::EndOfFile,s,l);if(idStart(c))return lexIdentifierOrKeyword();if(c=='`')return lexEscapedIdentifier();if(c=='$')return lexDollarIdentifier();if(digit(c))return lexNumber();if(c=='"')return lexString();if(c=='\'')return lexCharacter();if(c=='#')return lexHashConstruct();if(c=='@'){advance();return makeToken(K::AtSign,s,l);}if(isOperatorStartCharacter(c))return lexOperator();K p=punctuationKind(std::string_view(&c,1));if(p!=K::Unknown){advance();return makeToken(p,s,l);}return lexUnknown();}
Token Lexer::peek(){return lookahead(0);}
Token Lexer::lookahead(std::size_t d){while(lookaheadBuffer_.size()<=d){Token t=next();lookaheadBuffer_.push_back(t);if(t.kind==K::EndOfFile)break;}return lookaheadBuffer_[d<lookaheadBuffer_.size()?d:lookaheadBuffer_.size()-1];}
std::vector<Token> Lexer::tokenize(){std::vector<Token> r;for(;;){Token t=next();r.push_back(t);if(t.kind==K::EndOfFile)break;}return r;}
void Lexer::reset(){current_=0;line_=1;column_=1;atStartOfLine_=true;hadLeadingComment_=false;lookaheadBuffer_.clear();diagnostics_.clear();}
void Lexer::setKeepComments(bool v){keepComments_=v;}void Lexer::setAllowHashbang(bool v){allowHashbang_=v;}void Lexer::setAllowRegexLiterals(bool v){allowRegexLiterals_=v;}void Lexer::setTreatEditorPlaceholdersAsTokens(bool v){treatEditorPlaceholdersAsTokens_=v;}
bool Lexer::keepComments()const noexcept{return keepComments_;}bool Lexer::allowHashbang()const noexcept{return allowHashbang_;}bool Lexer::allowRegexLiterals()const noexcept{return allowRegexLiterals_;}bool Lexer::atEnd()const noexcept{return current_>=source_.size();}
std::size_t Lexer::currentOffset()const noexcept{return current_;}SourceLocation Lexer::currentLocation()const noexcept{return {current_,line_,column_};}const std::vector<LexerDiagnostic>& Lexer::diagnostics()const noexcept{return diagnostics_;}void Lexer::clearDiagnostics(){diagnostics_.clear();}
void Lexer::diagnose(LexerDiagnostic::Severity s,SourceLocation l,std::string m,std::string r){diagnostics_.push_back({s,l,std::move(m),std::move(r)});}void Lexer::diagnoseError(SourceLocation l,std::string m,std::string r){diagnose(LexerDiagnostic::Severity::Error,l,std::move(m),std::move(r));}void Lexer::diagnoseWarning(SourceLocation l,std::string m,std::string r){diagnose(LexerDiagnostic::Severity::Warning,l,std::move(m),std::move(r));}

bool Lexer::isWhitespaceAt(std::size_t p)const noexcept{return p<source_.size()&&std::isspace((unsigned char)source_[p]);}bool Lexer::isLineBreakAt(std::size_t p)const noexcept{return p<source_.size()&&(source_[p]=='\n'||source_[p]=='\r');}
bool Lexer::isASCIIIdentifierStart(char c)noexcept{return idStart(c);}bool Lexer::isASCIIIdentifierContinue(char c)noexcept{return idPart(c);}bool Lexer::isASCIIDigit(char c)noexcept{return digit(c);}bool Lexer::isASCIIHexDigit(char c)noexcept{return hex(c);}bool Lexer::isASCIIOctalDigit(char c)noexcept{return c>='0'&&c<='7';}bool Lexer::isASCIIWhitespace(char c)noexcept{return std::isspace((unsigned char)c)!=0;}bool Lexer::isPrintableASCII(char c)noexcept{return c>=32&&c<127;}
bool Lexer::isUnicodeIdentifierStart(std::uint32_t c)noexcept{return c>=128;}bool Lexer::isUnicodeIdentifierContinue(std::uint32_t c)noexcept{return c>=128||c=='_';}bool Lexer::isForbiddenIdentifierCodePoint(std::uint32_t c)noexcept{return c<32||c==127;}
bool Lexer::isOperatorCharacter(char c)noexcept{return std::strchr("+-*/%=&|^~!?<>*",c)!=nullptr;}bool Lexer::isOperatorStartCharacter(char c)noexcept{return isOperatorCharacter(c);}bool Lexer::isOperatorContinuationCharacter(char c)noexcept{return isOperatorCharacter(c);}
bool Lexer::looksLikeEditorPlaceholder(std::string_view s)noexcept{return s.size()>2&&s.front()=='<'&&s.back()=='>';}bool Lexer::looksLikeDirective(std::string_view s)noexcept{return s.size()>1&&s.front()=='#'&&idStart(s[1]);}bool Lexer::looksLikeConflictMarker(std::string_view s)noexcept{return s.rfind("<<<<<<<",0)==0||s.rfind("=======",0)==0||s.rfind(">>>>>>>",0)==0;}
OperatorBinding Lexer::classifyOperatorBinding(std::string_view s,std::size_t a,std::size_t b)noexcept{bool left=a>0&&!std::isspace((unsigned char)s[a-1]);bool right=b<s.size()&&!std::isspace((unsigned char)s[b]);if(!left&&!right)return OperatorBinding::BinarySpaced;if(left&&right)return OperatorBinding::BinaryUnspaced;if(left)return OperatorBinding::Postfix;return OperatorBinding::Prefix;}

bool Lexer::isIdentifier(std::string_view s){if(s.empty()||!idStart(s.front()))return false;for(char c:s)if(!idPart(c))return false;return true;}bool Lexer::isOperator(std::string_view s){if(s.empty())return false;for(char c:s)if(!isOperatorCharacter(c))return false;return true;}bool Lexer::isValidEscapedIdentifier(std::string_view s){return s.size()>=2&&s.front()=='`'&&s.back()=='`';}
std::uint32_t Lexer::validateUTF8Character(std::string_view b,std::size_t&p){if(p>=b.size())return 0xFFFFFFFFu;unsigned char c=b[p++];if(c<0x80)return c;int n=(c&0xE0)==0xC0?1:(c&0xF0)==0xE0?2:(c&0xF8)==0xF0?3:0;if(!n)return 0xFFFFFFFFu;std::uint32_t v=c&((1u<<(7-n))-1);for(int i=0;i<n;++i){if(p>=b.size()||(b[p]&0xC0)!=0x80)return 0xFFFFFFFFu;v=(v<<6)|(b[p++]&0x3F);}return v;}
bool Lexer::encodeUTF8(std::uint32_t c,std::string&o){if(c>0x10FFFF||(c>=0xD800&&c<=0xDFFF))return false;if(c<0x80)o.push_back((char)c);else if(c<0x800){o.push_back((char)(0xC0|(c>>6)));o.push_back((char)(0x80|(c&63)));}else if(c<0x10000){o.push_back((char)(0xE0|(c>>12)));o.push_back((char)(0x80|((c>>6)&63)));o.push_back((char)(0x80|(c&63)));}else{o.push_back((char)(0xF0|(c>>18)));o.push_back((char)(0x80|((c>>12)&63)));o.push_back((char)(0x80|((c>>6)&63)));o.push_back((char)(0x80|(c&63)));}return true;}

bool Lexer::scanInterpolatedExpression(std::size_t,std::size_t&){return false;}bool Lexer::scanStringDelimiter(std::size_t&d,bool&m){d=0;m=false;return false;}bool Lexer::scanUnicodeEscape(std::size_t&,std::uint32_t&){return false;}bool Lexer::scanEscapeSequence(std::size_t&,std::string&,bool&){return false;}
Token Lexer::lexRawString(unsigned d){return lexString();}
Token Lexer::lexRegex(){return lexOperator();}
void Lexer::invalidateLookahead(){lookaheadBuffer_.clear();}

const char* tokenKindName(K k)noexcept{switch(k){case K::EndOfFile:return "eof";case K::Identifier:return "identifier";case K::IntegerLiteral:return "integer";case K::FloatLiteral:return "float";case K::StringLiteral:return "string";case K::CharacterLiteral:return "character";case K::KwFunc:return "func";case K::KwVar:return "var";case K::KwLet:return "let";case K::KwIf:return "if";case K::KwElse:return "else";case K::KwReturn:return "return";case K::Equal:return "=";case K::EqualEqual:return "==";case K::NotEqual:return "!=";case K::PlusEqual:return "+=";case K::MinusEqual:return "-=";case K::StarEqual:return "*=";case K::GreaterEqual:return ">=";case K::LessEqual:return "<=";case K::TildeEqual:return "~=";case K::LeftBrace:return "{";case K::RightBrace:return "}";case K::LeftParen:return "(";case K::RightParen:return ")";case K::Dot:return ".";case K::Comma:return ",";case K::Colon:return ":";case K::Semicolon:return ";";case K::Unknown:return "unknown";default:return "token";}}
}
