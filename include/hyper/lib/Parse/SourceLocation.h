#pragma once
#ifndef HYPER_LIB_PARSE_SOURCE_LOCATION_H
#define HYPER_LIB_PARSE_SOURCE_LOCATION_H
#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>
#include <string_view>
#include <vector>
namespace hyper::parse {
using SourceOffset=std::uint64_t; using SourceLine=std::uint32_t; using SourceColumn=std::uint32_t;
inline constexpr SourceOffset InvalidSourceOffset=std::numeric_limits<SourceOffset>::max();
struct SourcePosition {
 SourceOffset offset=InvalidSourceOffset; SourceLine line=0; SourceColumn column=0;
 constexpr SourcePosition() noexcept=default; constexpr SourcePosition(SourceOffset o,SourceLine l,SourceColumn c) noexcept:offset(o),line(l),column(c){}
 constexpr bool isValid()const noexcept{return offset!=InvalidSourceOffset;}
 constexpr bool operator==(const SourcePosition&x)const noexcept{return offset==x.offset&&line==x.line&&column==x.column;}
 constexpr bool operator!=(const SourcePosition&x)const noexcept{return !(*this==x);}
};
struct SourceRange {
 SourcePosition begin{},end{};
 constexpr bool isValid()const noexcept{return begin.isValid()&&end.isValid();}
 constexpr SourceOffset length()const noexcept{return isValid()&&end.offset>=begin.offset?end.offset-begin.offset:0;}
 constexpr bool empty()const noexcept{return length()==0;}
 constexpr bool contains(SourceOffset o)const noexcept{return isValid()&&o>=begin.offset&&o<end.offset;}
};
struct SourceLineInfo { SourceLine line=0; SourceOffset startOffset=0,endOffset=0; };
class SourceText {
public:
 SourceText()=default; explicit SourceText(std::string text); explicit SourceText(std::string_view text);
 void reset(std::string text); void reset(std::string_view text);
 const std::string& str()const noexcept; std::string_view view()const noexcept; const char* data()const noexcept;
 std::size_t size()const noexcept; bool empty()const noexcept; char at(SourceOffset)const noexcept;
 bool has(SourceOffset)const noexcept; SourcePosition positionAt(SourceOffset)const noexcept;
 SourceRange range(SourceOffset,SourceOffset)const noexcept; std::string_view slice(SourceRange)const noexcept;
 std::string_view slice(SourceOffset,SourceOffset)const noexcept; SourceLine lineAt(SourceOffset)const noexcept;
 SourceColumn columnAt(SourceOffset)const noexcept; std::size_t lineCount()const noexcept;
private: std::string text_; std::vector<SourceLineInfo> lines_; void rebuildLineTable();
};
class SourceManager {
public:
 using FileID=std::uint32_t; static constexpr FileID InvalidFileID=0;
 FileID addSource(std::string path,std::string text); FileID addSource(std::string path,std::string_view text);
 bool contains(FileID)const noexcept; const std::string& path(FileID)const; const SourceText& source(FileID)const;
 SourceText& source(FileID); SourcePosition position(FileID,SourceOffset)const; SourceRange range(FileID,SourceOffset,SourceOffset)const;
 std::string_view slice(FileID,SourceRange)const; std::size_t size()const noexcept;
private: struct Entry{FileID id;std::string path;SourceText text;}; std::vector<Entry> entries_;
};
}
#endif
