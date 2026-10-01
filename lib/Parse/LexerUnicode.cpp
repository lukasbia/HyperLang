#include "hyper/lib/Parse/Lexer.h"
#include <cstdint>
namespace hyper::parse {
std::uint32_t Lexer::decodeUTF8(SourceOffset off,std::size_t&width)const noexcept{
 width=0;if(off>=input_.size())return 0;const auto p=[&](std::size_t i){return static_cast<unsigned char>(off+i<input_.size()?input_[off+i]:0);};
 unsigned char b=p(0);
 if(b<0x80){width=1;return b;}
 if((b&0xe0)==0xc0){if((p(1)&0xc0)!=0x80)return 0;width=2;return ((b&0x1f)<<6)|(p(1)&0x3f);}
 if((b&0xf0)==0xe0){if((p(1)&0xc0)!=0x80||(p(2)&0xc0)!=0x80)return 0;width=3;auto cp=((b&0xf)<<12)|((p(1)&0x3f)<<6)|(p(2)&0x3f);return cp<0x800?0:cp;}
 if((b&0xf8)==0xf0){if((p(1)&0xc0)!=0x80||(p(2)&0xc0)!=0x80||(p(3)&0xc0)!=0x80)return 0;width=4;auto cp=((b&7)<<18)|((p(1)&0x3f)<<12)|((p(2)&0x3f)<<6)|(p(3)&0x3f);return cp<0x10000||cp>0x10ffff?0:cp;}
 return 0;
}
bool Lexer::lexUTF8Identifier(){
 std::size_t width=0;auto cp=decodeUTF8(cursor_,width);if(cp==0||width==0)return false;cursor_+=width;column_+=static_cast<SourceColumn>(width);return true;
}
}
