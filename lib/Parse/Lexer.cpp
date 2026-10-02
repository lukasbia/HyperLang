#include "hyper/lib/Parse/Lexer.h"
#include <algorithm>
#include <array>
#include <cctype>
#include <string_view>
#include <utility>

namespace hyper::parse {
namespace {
struct Keyword { std::string_view spelling; TokenKind kind; };
constexpr std::array keywords{
    Keyword{"func",TokenKind::Keyword},
    Keyword{"var",TokenKind::Keyword}, Keyword{"const",TokenKind::Keyword},
    Keyword{"struct",TokenKind::Keyword}, Keyword{"class",TokenKind::Keyword},
    Keyword{"enum",TokenKind::Keyword}, Keyword{"protocol",TokenKind::Keyword},
    Keyword{"extension",TokenKind::Keyword}, Keyword{"type",TokenKind::Keyword},
    Keyword{"typealias",TokenKind::Keyword}, Keyword{"template",TokenKind::Keyword},
    Keyword{"typename",TokenKind::Keyword}, Keyword{"operator",TokenKind::Keyword},
    Keyword{"namespace",TokenKind::Keyword}, Keyword{"module",TokenKind::Keyword},
    Keyword{"init",TokenKind::Keyword}, Keyword{"deinit",TokenKind::Keyword},
    Keyword{"self",TokenKind::Keyword}, Keyword{"super",TokenKind::Keyword},
    Keyword{"new",TokenKind::Keyword}, Keyword{"get",TokenKind::Keyword},
    Keyword{"set",TokenKind::Keyword}, Keyword{"if",TokenKind::Keyword},
    Keyword{"elseif",TokenKind::Keyword}, Keyword{"else",TokenKind::Keyword},
    Keyword{"endif",TokenKind::Keyword}, Keyword{"switch",TokenKind::Keyword},
    Keyword{"case",TokenKind::Keyword}, Keyword{"default",TokenKind::Keyword},
    Keyword{"while",TokenKind::Keyword}, Keyword{"loop",TokenKind::Keyword},
    Keyword{"do",TokenKind::Keyword}, Keyword{"for",TokenKind::Keyword},
    Keyword{"in",TokenKind::Keyword}, Keyword{"break",TokenKind::Keyword},
    Keyword{"continue",TokenKind::Keyword}, Keyword{"return",TokenKind::Keyword},
    Keyword{"throw",TokenKind::Keyword}, Keyword{"try",TokenKind::Keyword},
    Keyword{"catch",TokenKind::Keyword}, Keyword{"finally",TokenKind::Keyword},
    Keyword{"defer",TokenKind::Keyword}, Keyword{"error",TokenKind::Keyword},
    Keyword{"panic",TokenKind::Keyword}, Keyword{"bool",TokenKind::Keyword},
    Keyword{"int",TokenKind::Keyword}, Keyword{"uint",TokenKind::Keyword},
    Keyword{"int8",TokenKind::Keyword}, Keyword{"int16",TokenKind::Keyword},
    Keyword{"int32",TokenKind::Keyword}, Keyword{"int64",TokenKind::Keyword},
    Keyword{"uint8",TokenKind::Keyword}, Keyword{"uint16",TokenKind::Keyword},
    Keyword{"uint32",TokenKind::Keyword}, Keyword{"uint64",TokenKind::Keyword},
    Keyword{"float",TokenKind::Keyword}, Keyword{"double",TokenKind::Keyword},
    Keyword{"byte",TokenKind::Keyword}, Keyword{"char",TokenKind::Keyword},
    Keyword{"string",TokenKind::Keyword}, Keyword{"void",TokenKind::Keyword},
    Keyword{"public",TokenKind::Keyword}, Keyword{"private",TokenKind::Keyword},
    Keyword{"protected",TokenKind::Keyword}, Keyword{"internal",TokenKind::Keyword},
    Keyword{"static",TokenKind::Keyword}, Keyword{"extern",TokenKind::Keyword},
    Keyword{"inline",TokenKind::Keyword}, Keyword{"volatile",TokenKind::Keyword},
    Keyword{"sizeof",TokenKind::Keyword}, Keyword{"async",TokenKind::Keyword},
    Keyword{"await",TokenKind::Keyword}, Keyword{"task",TokenKind::Keyword},
    Keyword{"actor",TokenKind::Keyword}, Keyword{"parallel",TokenKind::Keyword},
    Keyword{"atomic",TokenKind::Keyword}, Keyword{"input",TokenKind::Keyword},
    Keyword{"output",TokenKind::Keyword}, Keyword{"message",TokenKind::Keyword},
    Keyword{"show",TokenKind::Keyword}, Keyword{"getData",TokenKind::Keyword},
    Keyword{"createData",TokenKind::Keyword}, Keyword{"from",TokenKind::Keyword},
    Keyword{"import",TokenKind::Keyword}, Keyword{"true",TokenKind::TrueLiteral},
    Keyword{"false",TokenKind::FalseLiteral}, Keyword{"nil",TokenKind::NilLiteral}
};
constexpr std::array specialKeywords{
    Keyword{"@API",TokenKind::AtApi}, Keyword{"@APIID",TokenKind::AtApiId},
    Keyword{"@file",TokenKind::AtFile}, Keyword{"@filename",TokenKind::AtFilename},
    Keyword{"@fileID",TokenKind::AtFileId}, Keyword{"@audio",TokenKind::AtAudio},
    Keyword{"@playAudio",TokenKind::AtPlayAudio}, Keyword{"@image",TokenKind::AtImage},
    Keyword{"@imageID",TokenKind::AtImageId}
};
bool start(unsigned char c) noexcept {
    return (c>='a'&&c<='z')||(c>='A'&&c<='Z')||c=='_';
}
bool cont(unsigned char c) noexcept { return start(c)||(c>='0'&&c<='9'); }
bool dec(unsigned char c) noexcept { return c>='0'&&c<='9'; }
bool bin(unsigned char c) noexcept { return c=='0'||c=='1'; }
bool oct(unsigned char c) noexcept { return c>='0'&&c<='7'; }
bool hex(unsigned char c) noexcept {
    return dec(c)||(c>='a'&&c<='f')||(c>='A'&&c<='F');
}
unsigned hexValue(unsigned char c) noexcept {
    if(dec(c)) return c-'0';
    if(c>='a'&&c<='f') return c-'a'+10;
    return c-'A'+10;
}
}

const char* tokenKindName(TokenKind k) noexcept {
    switch(k) {
        case TokenKind::Eof:return "eof"; case TokenKind::Unknown:return "unknown";
        case TokenKind::Identifier:return "identifier"; case TokenKind::Keyword:return "keyword";
        case TokenKind::SpecialKeyword:return "special-keyword";
        case TokenKind::IntegerLiteral:return "integer"; case TokenKind::FloatingLiteral:return "floating";
        case TokenKind::StringLiteral:return "string"; case TokenKind::CharacterLiteral:return "character";
        case TokenKind::TrueLiteral:return "true"; case TokenKind::FalseLiteral:return "false";
        case TokenKind::NilLiteral:return "nil";
        case TokenKind::LParen:return "("; case TokenKind::RParen:return ")";
        case TokenKind::LBrace:return "{"; case TokenKind::RBrace:return "}";
        case TokenKind::LBracket:return "["; case TokenKind::RBracket:return "]";
        case TokenKind::Comma:return ","; case TokenKind::Dot:return ".";
        case TokenKind::Colon:return ":"; case TokenKind::Semicolon:return ";";
        case TokenKind::Question:return "?"; case TokenKind::At:return "@";
        case TokenKind::Plus:return "+"; case TokenKind::Minus:return "-";
        case TokenKind::Star:return "*"; case TokenKind::Slash:return "/";
        case TokenKind::Percent:return "%"; case TokenKind::Ampersand:return "&";
        case TokenKind::Pipe:return "|"; case TokenKind::Caret:return "^";
        case TokenKind::Tilde:return "~"; case TokenKind::Bang:return "!";
        case TokenKind::Equal:return "="; case TokenKind::Less:return "<";
        case TokenKind::Greater:return ">"; case TokenKind::PlusEqual:return "+=";
        case TokenKind::MinusEqual:return "-="; case TokenKind::StarEqual:return "*=";
        case TokenKind::SlashEqual:return "/="; case TokenKind::PercentEqual:return "%=";
        case TokenKind::AmpEqual:return "&="; case TokenKind::PipeEqual:return "|=";
        case TokenKind::CaretEqual:return "^="; case TokenKind::EqualEqual:return "==";
        case TokenKind::BangEqual:return "!="; case TokenKind::LessEqual:return "<=";
        case TokenKind::GreaterEqual:return ">="; case TokenKind::AmpAmp:return "&&";
        case TokenKind::PipePipe:return "||"; case TokenKind::Arrow:return "->";
        case TokenKind::FatArrow:return "=>"; case TokenKind::Increment:return "++";
        case TokenKind::Decrement:return "--"; case TokenKind::ShiftLeft:return "<<";
        case TokenKind::ShiftRight:return ">>"; case TokenKind::ShiftLeftEqual:return "<<=";
        case TokenKind::ShiftRightEqual:return ">>="; case TokenKind::Range:return "..";
        case TokenKind::RangeInclusive:return "..."; case TokenKind::NullCoalescing:return "??";
        case TokenKind::Scope:return "::"; case TokenKind::Ellipsis:return "...";
        case TokenKind::DoubleColon:return "::"; case TokenKind::QuestionDot:return "?.";
        case TokenKind::BangDot:return "!."; case TokenKind::AtApi:return "@API";
        case TokenKind::AtApiId:return "@APIID"; case TokenKind::AtFile:return "@file";
        case TokenKind::AtFilename:return "@filename"; case TokenKind::AtFileId:return "@fileID";
        case TokenKind::AtAudio:return "@audio"; case TokenKind::AtPlayAudio:return "@playAudio";
        case TokenKind::AtImage:return "@image"; case TokenKind::AtImageId:return "@imageID";
    }
    return "unknown";
}
bool isKeyword(TokenKind k) noexcept {
    return k==TokenKind::Keyword||k==TokenKind::TrueLiteral||
           k==TokenKind::FalseLiteral||k==TokenKind::NilLiteral;
}
bool isSpecialKeyword(TokenKind k) noexcept {
    return k>=TokenKind::AtApi && k<=TokenKind::AtImageId;
}
bool isLiteral(TokenKind k) noexcept {
    return k==TokenKind::IntegerLiteral||k==TokenKind::FloatingLiteral||
           k==TokenKind::StringLiteral||k==TokenKind::CharacterLiteral||
           k==TokenKind::TrueLiteral||k==TokenKind::FalseLiteral||k==TokenKind::NilLiteral;
}
bool isPunctuation(TokenKind k) noexcept {
    switch(k) {
        case TokenKind::LParen:case TokenKind::RParen:case TokenKind::LBrace:
        case TokenKind::RBrace:case TokenKind::LBracket:case TokenKind::RBracket:
        case TokenKind::Comma:case TokenKind::Dot:case TokenKind::Colon:
        case TokenKind::Semicolon:case TokenKind::Question:case TokenKind::At:return true;
        default:return false;
    }
}
bool isOperator(TokenKind k) noexcept {
    return !isPunctuation(k)&&!isKeyword(k)&&!isLiteral(k)&&
           k!=TokenKind::Identifier&&k!=TokenKind::SpecialKeyword&&
           k!=TokenKind::Eof&&k!=TokenKind::Unknown;
}

Lexer::Lexer(std::string_view source):source_(source) {
    if(source_.size()>=3&&static_cast<unsigned char>(source_[0])==0xEF&&
       static_cast<unsigned char>(source_[1])==0xBB&&static_cast<unsigned char>(source_[2])==0xBF)
        offset_=3;
}
bool Lexer::atEnd() const noexcept { return offset_>=source_.size(); }
char Lexer::current() const noexcept { return atEnd()?'\0':source_[offset_]; }
char Lexer::look(std::size_t d) const noexcept {
    std::size_t p=offset_+d; return p<source_.size()?source_[p]:'\0';
}
bool Lexer::consume(char c) noexcept { if(current()!=c)return false; ++offset_; return true; }

Lexer::UTF8 Lexer::decode(std::size_t p) const noexcept {
    UTF8 r; if(p>=source_.size())return r;
    unsigned char f=static_cast<unsigned char>(source_[p]);
    if(f<0x80){r.value=f;r.width=1;r.valid=true;return r;}
    std::size_t n=0; char32_t value=0,min=0;
    if((f&0xE0)==0xC0){n=2;value=f&0x1F;min=0x80;}
    else if((f&0xF0)==0xE0){n=3;value=f&0x0F;min=0x800;}
    else if((f&0xF8)==0xF0){n=4;value=f&0x07;min=0x10000;}
    else return r;
    if(p+n>source_.size())return r;
    for(std::size_t i=1;i<n;++i){
        unsigned char b=static_cast<unsigned char>(source_[p+i]);
        if((b&0xC0)!=0x80)return UTF8{};
        value=(value<<6)|(b&0x3F);
    }
    if(value<min||value>0x10FFFF||(value>=0xD800&&value<=0xDFFF))return UTF8{};
    r.value=value;r.width=n;r.valid=true;return r;
}
bool Lexer::isIdentifierStart(char32_t c) noexcept {
    if(c<0x80)return start(static_cast<unsigned char>(c));
    return (c>=0xC0&&c<=0x2FF)||(c>=0x370&&c<=0x1FFF)||
           (c>=0x2C00&&c<=0xD7FF)||(c>=0xF900&&c<=0xFDCF)||
           (c>=0xFDF0&&c<=0xFFFD)||(c>=0x10000&&c<=0xEFFFF);
}
bool Lexer::isIdentifierContinue(char32_t c) noexcept {
    return isIdentifierStart(c)||(c>=0x300&&c<=0x36F)||
           (c>=0x1AB0&&c<=0x1AFF)||(c>=0x1DC0&&c<=0x1DFF)||
           (c>=0x20D0&&c<=0x20FF)||(c>=0xFE20&&c<=0xFE2F);
}
bool Lexer::consumeIdentifierCodePoint(bool first) {
    UTF8 r=decode(offset_); if(!r.valid)return false;
    if(first?!isIdentifierStart(r.value):!isIdentifierContinue(r.value))return false;
    offset_+=r.width;return true;
}
bool Lexer::isKeyword(std::string_view s) noexcept {
    return std::any_of(keywords.begin(),keywords.end(),
        [s](const Keyword& k){return k.spelling==s;});
}
void Lexer::diagnose(DiagnosticSeverity severity,std::size_t b,std::size_t e,
                     std::string_view message) {
    SourcePosition first; first.offset=static_cast<SourceOffset>(b);
    SourcePosition last; last.offset=static_cast<SourceOffset>(e);
    diagnostics_.push_back({severity,{first,last},std::string(message)});
}
Token Lexer::make(TokenKind k,std::size_t b) {
    SourcePosition first;first.offset=static_cast<SourceOffset>(b);
    SourcePosition last;last.offset=static_cast<SourceOffset>(offset_);
    Token t(k,{first,last},std::string(source_.substr(b,offset_-b)));
    atStartOfLine_=false;leadingSpace_=false;return t;
}
void Lexer::lineComment() {
    offset_+=2;while(!atEnd()&&current()!='\n'&&current()!='\r')++offset_;
    if(!atEnd()){if(current()=='\r'&&look()=='\n')offset_+=2;else ++offset_;atStartOfLine_=true;}
}
void Lexer::blockComment() {
    std::size_t b=offset_;offset_+=2;unsigned depth=1;
    while(!atEnd()){
        if(current()=='/'&&look()=='*'){++depth;offset_+=2;continue;}
        if(current()=='*'&&look()=='/'){offset_+=2;if(--depth==0)return;continue;}
        if(current()=='\n'||current()=='\r'){
            atStartOfLine_=true;if(current()=='\r'&&look()=='\n')offset_+=2;else ++offset_;continue;
        }
        ++offset_;
    }
    diagnose(DiagnosticSeverity::Error,b,offset_,"unterminated block comment");
}
void Lexer::skipTrivia() {
    for(;;){
        if(atEnd())return;
        if(current()==' '||current()=='\t'||current()=='\v'||current()=='\f'){leadingSpace_=true;++offset_;continue;}
        if(current()=='\n'){leadingSpace_=true;atStartOfLine_=true;++offset_;continue;}
        if(current()=='\r'){leadingSpace_=true;atStartOfLine_=true;if(look()=='\n')offset_+=2;else ++offset_;continue;}
        if(current()=='/'&&look()=='/'){leadingSpace_=true;lineComment();continue;}
        if(current()=='/'&&look()=='*'){leadingSpace_=true;blockComment();continue;}
        UTF8 u=decode(offset_);
        if(u.valid&&(u.value==0x85||u.value==0xA0||(u.value>=0x2000&&u.value<=0x200A)||
                     u.value==0x2028||u.value==0x2029||u.value==0x202F||
                     u.value==0x205F||u.value==0x3000)){leadingSpace_=true;offset_+=u.width;continue;}
        return;
    }
}
Token Lexer::identifier() {
    std::size_t b=offset_;
    if(current()==static_cast<char>(96)){
        ++offset_;
        while(!atEnd()&&current()!=static_cast<char>(96)){
            if(current()=='\n'||current()=='\r'){diagnose(DiagnosticSeverity::Error,b,offset_,"unterminated escaped identifier");break;}
            UTF8 u=decode(offset_);
            if(static_cast<unsigned char>(current())>=0x80){
                if(!u.valid){diagnose(DiagnosticSeverity::Error,offset_,offset_+1,"invalid UTF-8 in identifier");++offset_;}
                else offset_+=u.width;
            } else ++offset_;
        }
        if(!consume(static_cast<char>(96)))diagnose(DiagnosticSeverity::Error,b,offset_,"unterminated escaped identifier");
        return make(TokenKind::Identifier,b);
    }
    if(static_cast<unsigned char>(current())<0x80)++offset_;
    else if(!consumeIdentifierCodePoint(true)){diagnose(DiagnosticSeverity::Error,offset_,offset_+1,"invalid UTF-8 sequence");++offset_;return make(TokenKind::Unknown,b);}
    while(!atEnd()){
        unsigned char c=static_cast<unsigned char>(current());
        if(c<0x80){if(!cont(c))break;++offset_;}
        else if(!consumeIdentifierCodePoint(false))break;
    }
    std::string_view s=source_.substr(b,offset_-b);
    for(const auto& k:keywords)if(k.spelling==s)return make(k.kind,b);
    return make(TokenKind::Identifier,b);
}
Token Lexer::specialIdentifier() {
    std::size_t b=offset_;++offset_;
    if(atEnd()||(!start(static_cast<unsigned char>(current()))&&static_cast<unsigned char>(current())<0x80))
        return make(TokenKind::At,b);
    ++offset_;while(!atEnd()&&cont(static_cast<unsigned char>(current())))++offset_;
    std::string_view s=source_.substr(b,offset_-b);
    for(const auto& k:specialKeywords)if(k.spelling==s)return make(k.kind,b);
    return make(TokenKind::At,b);
}
bool Lexer::consumeDigits(unsigned base) {
    bool digit=false,sep=false;
    while(!atEnd()){
        unsigned char c=static_cast<unsigned char>(current());
        bool valid=base==2?bin(c):base==8?oct(c):base==10?dec(c):hex(c);
        if(valid){digit=true;sep=false;++offset_;continue;}
        if(c=='_'){if(!digit||sep)diagnose(DiagnosticSeverity::Error,offset_,offset_+1,"invalid digit separator");sep=true;++offset_;continue;}
        break;
    }
    if(sep)diagnose(DiagnosticSeverity::Error,offset_-1,offset_,"numeric literal cannot end with a digit separator");
    return digit;
}
Token Lexer::number() {
    std::size_t b=offset_;bool floating=false;
    if(current()=='0'&&(look()=='x'||look()=='X'||look()=='b'||look()=='B'||look()=='o'||look()=='O')){
        char p=look();offset_+=2;unsigned base=(p=='x'||p=='X')?16:(p=='b'||p=='B')?2:8;
        if(!consumeDigits(base))diagnose(DiagnosticSeverity::Error,b,offset_,"numeric base prefix requires digits");
        if(base==16&&current()=='.'&&look()!='.'){
            floating=true;++offset_;consumeDigits(16);
            if(current()=='p'||current()=='P'){++offset_;if(current()=='+'||current()=='-')++offset_;if(!consumeDigits(10))diagnose(DiagnosticSeverity::Error,offset_,offset_,"hexadecimal floating literal requires exponent digits");}
        }
        return make(floating?TokenKind::FloatingLiteral:TokenKind::IntegerLiteral,b);
    }
    consumeDigits(10);
    if(current()=='.'&&dec(static_cast<unsigned char>(look()))){floating=true;++offset_;consumeDigits(10);}
    if(current()=='e'||current()=='E'){floating=true;++offset_;if(current()=='+'||current()=='-')++offset_;if(!consumeDigits(10))diagnose(DiagnosticSeverity::Error,offset_,offset_,"floating literal requires exponent digits");}
    return make(floating?TokenKind::FloatingLiteral:TokenKind::IntegerLiteral,b);
}
bool Lexer::consumeUnicodeEscape() {
    if(!consume('{')){diagnose(DiagnosticSeverity::Error,offset_,offset_,"expected '{' in Unicode escape");return false;}
    std::size_t b=offset_;char32_t value=0;unsigned n=0;
    while(!atEnd()&&current()!='}'){
        unsigned char c=static_cast<unsigned char>(current());
        if(!hex(c)){diagnose(DiagnosticSeverity::Error,offset_,offset_+1,"non-hex digit in Unicode escape");++offset_;continue;}
        if(n==6){diagnose(DiagnosticSeverity::Error,offset_,offset_+1,"Unicode escape contains too many digits");++offset_;continue;}
        value=(value<<4)|hexValue(c);++n;++offset_;
    }
    if(b==offset_)diagnose(DiagnosticSeverity::Error,b,offset_,"Unicode escape requires digits");
    if(!consume('}')){diagnose(DiagnosticSeverity::Error,b,offset_,"unterminated Unicode escape");return false;}
    if(value>0x10FFFF||(value>=0xD800&&value<=0xDFFF))diagnose(DiagnosticSeverity::Error,b,offset_,"invalid Unicode scalar");
    return true;
}
bool Lexer::consumeEscape() {
    std::size_t b=offset_;++offset_;if(atEnd()){diagnose(DiagnosticSeverity::Error,b,offset_,"unterminated escape");return false;}
    switch(current()){
        case '\\':case '"':case '\'':case 'n':case 'r':case 't':case '0':case 'b':case 'f':case 'v':++offset_;return true;
        case 'u':++offset_;return consumeUnicodeEscape();
        case 'x':++offset_;if(!hex(static_cast<unsigned char>(current()))||!hex(static_cast<unsigned char>(look()))){diagnose(DiagnosticSeverity::Error,b,offset_,"hex escape requires two digits");return false;}offset_+=2;return true;
        default:diagnose(DiagnosticSeverity::Error,b,offset_+1,"unknown escape sequence");++offset_;return false;
    }
}
Token Lexer::stringLiteral() {
    std::size_t b=offset_;++offset_;bool done=false;
    while(!atEnd()){
        if(current()=='"'){++offset_;done=true;break;}
        if(current()=='\\'){consumeEscape();continue;}
        if(current()=='\n'||current()=='\r'){diagnose(DiagnosticSeverity::Error,b,offset_,"newline is not allowed in a string literal");break;}
        if(static_cast<unsigned char>(current())<0x20){diagnose(DiagnosticSeverity::Error,offset_,offset_+1,"control character in string literal");++offset_;continue;}
        UTF8 u=decode(offset_);
        if(static_cast<unsigned char>(current())>=0x80){if(!u.valid){diagnose(DiagnosticSeverity::Error,offset_,offset_+1,"invalid UTF-8 in string literal");++offset_;}else offset_+=u.width;}
        else ++offset_;
    }
    if(!done)diagnose(DiagnosticSeverity::Error,b,offset_,"unterminated string literal");
    return make(TokenKind::StringLiteral,b);
}
Token Lexer::characterLiteral() {
    std::size_t b=offset_;++offset_;bool value=false,done=false;
    while(!atEnd()){
        if(current()=='\''){++offset_;done=true;break;}
        if(current()=='\\'){consumeEscape();value=true;continue;}
        if(current()=='\n'||current()=='\r'){diagnose(DiagnosticSeverity::Error,b,offset_,"newline is not allowed in character literal");break;}
        UTF8 u=decode(offset_);
        if(static_cast<unsigned char>(current())>=0x80){if(!u.valid){diagnose(DiagnosticSeverity::Error,offset_,offset_+1,"invalid UTF-8 in character literal");++offset_;}else{offset_+=u.width;value=true;}}
        else{++offset_;value=true;}
    }
    if(!done)diagnose(DiagnosticSeverity::Error,b,offset_,"unterminated character literal");
    else if(!value)diagnose(DiagnosticSeverity::Error,b,offset_,"empty character literal");
    return make(TokenKind::CharacterLiteral,b);
}
Token Lexer::punctuation() {
    std::size_t b=offset_;char c=current();
    switch(c){
        case '(':++offset_;return make(TokenKind::LParen,b); case ')':++offset_;return make(TokenKind::RParen,b);
        case '{':++offset_;return make(TokenKind::LBrace,b); case '}':++offset_;return make(TokenKind::RBrace,b);
        case '[':++offset_;return make(TokenKind::LBracket,b); case ']':++offset_;return make(TokenKind::RBracket,b);
        case ',':++offset_;return make(TokenKind::Comma,b); case ';':++offset_;return make(TokenKind::Semicolon,b);
        case ':':++offset_;if(consume(':'))return make(TokenKind::DoubleColon,b);return make(TokenKind::Colon,b);
        case '@':return specialIdentifier(); case '"':return stringLiteral(); case '\'':return characterLiteral();
        case '?':++offset_;if(consume('?'))return make(TokenKind::NullCoalescing,b);if(consume('.'))return make(TokenKind::QuestionDot,b);return make(TokenKind::Question,b);
        case '.':++offset_;if(consume('.')&&consume('.'))return make(TokenKind::Ellipsis,b);if(offset_-b==2)return make(TokenKind::Range,b);return make(TokenKind::Dot,b);
        case '!':++offset_;if(consume('='))return make(TokenKind::BangEqual,b);if(consume('.'))return make(TokenKind::BangDot,b);return make(TokenKind::Bang,b);
        case '+':++offset_;if(consume('+'))return make(TokenKind::Increment,b);if(consume('='))return make(TokenKind::PlusEqual,b);return make(TokenKind::Plus,b);
        case '-':++offset_;if(consume('-'))return make(TokenKind::Decrement,b);if(consume('='))return make(TokenKind::MinusEqual,b);if(consume('>'))return make(TokenKind::Arrow,b);return make(TokenKind::Minus,b);
        case '*':++offset_;if(consume('='))return make(TokenKind::StarEqual,b);return make(TokenKind::Star,b);
        case '/':++offset_;if(consume('='))return make(TokenKind::SlashEqual,b);return make(TokenKind::Slash,b);
        case '%':++offset_;if(consume('='))return make(TokenKind::PercentEqual,b);return make(TokenKind::Percent,b);
        case '&':++offset_;if(consume('&'))return make(TokenKind::AmpAmp,b);if(consume('='))return make(TokenKind::AmpEqual,b);return make(TokenKind::Ampersand,b);
        case '|':++offset_;if(consume('|'))return make(TokenKind::PipePipe,b);if(consume('='))return make(TokenKind::PipeEqual,b);return make(TokenKind::Pipe,b);
        case '^':++offset_;if(consume('='))return make(TokenKind::CaretEqual,b);return make(TokenKind::Caret,b);
        case '~':++offset_;return make(TokenKind::Tilde,b);
        case '=':++offset_;if(consume('='))return make(TokenKind::EqualEqual,b);if(consume('>'))return make(TokenKind::FatArrow,b);return make(TokenKind::Equal,b);
        case '<':++offset_;if(consume('<')){if(consume('='))return make(TokenKind::ShiftLeftEqual,b);return make(TokenKind::ShiftLeft,b);}if(consume('='))return make(TokenKind::LessEqual,b);return make(TokenKind::Less,b);
        case '>':++offset_;if(consume('>')){if(consume('='))return make(TokenKind::ShiftRightEqual,b);return make(TokenKind::ShiftRight,b);}if(consume('='))return make(TokenKind::GreaterEqual,b);return make(TokenKind::Greater,b);
        default:++offset_;diagnose(DiagnosticSeverity::Error,b,offset_,"unexpected character");return make(TokenKind::Unknown,b);
    }
}
Token Lexer::lexImpl() {
    // Main lexer dispatch. Trivia is consumed before token classification.
    skipTrivia();

    if (atEnd())
        return make(TokenKind::Eof, offset_);

    const std::size_t tokenStart = offset_;
    const unsigned char c =
        static_cast<unsigned char>(current());

    switch (c) {
    case '\n':
    case '\r':
    case ' ':
    case '\t':
    case '\v':
    case '\f':
        skipTrivia();
        return lexImpl();

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return number();

    case 96:
        return identifier();

    case '"':
        return stringLiteral();

    case '\'':
        return characterLiteral();

    case '@':
        return specialIdentifier();

    case '(':
    case ')':
    case '{':
    case '}':
    case '[':
    case ']':
    case ',':
    case '.':
    case ':':
    case ';':
    case '?':
    case '!':
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '&':
    case '|':
    case '^':
    case '~':
    case '=':
    case '<':
    case '>':
        return punctuation();

    default:
        break;
    }

    if (isASCIIIdentifierStart(c) || c >= 0x80)
        return identifier();

    ++offset_;
    diagnose(
        DiagnosticSeverity::Error,
        tokenStart,
        offset_,
        "unexpected character");

    return make(TokenKind::Unknown, tokenStart);
}

Token Lexer::lex() {
    if(!lookahead_.empty()){Token t=std::move(lookahead_.front());lookahead_.erase(lookahead_.begin());return t;}
    return lexImpl();
}
Token Lexer::peek(std::size_t n) {
    if(n>32)n=32;
    while(lookahead_.size()<=n)lookahead_.push_back(lexImpl());
    return lookahead_[n];
}
std::vector<Token> Lexer::lexAll() {
    std::vector<Token> result;
    for(;;){Token t=lex();result.push_back(t);if(t.isEOF())break;}
    return result;
}
const std::vector<LexerDiagnostic>& Lexer::diagnostics() const noexcept {return diagnostics_;}
bool Lexer::hasErrors() const noexcept {
    return std::any_of(diagnostics_.begin(),diagnostics_.end(),[](const auto& d){return d.severity==DiagnosticSeverity::Error;});
}
}
