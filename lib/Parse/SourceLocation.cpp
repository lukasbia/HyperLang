#include "hyper/lib/Parse/SourceLocation.h"
#include <algorithm>
#include <stdexcept>
namespace hyper::parse {
SourceText::SourceText(std::string t){reset(std::move(t));}
SourceText::SourceText(std::string_view t){reset(t);}
void SourceText::reset(std::string t){text_=std::move(t);rebuildLineTable();}
void SourceText::reset(std::string_view t){text_.assign(t);rebuildLineTable();}
const std::string& SourceText::str()const noexcept{return text_;} std::string_view SourceText::view()const noexcept{return text_;}
const char* SourceText::data()const noexcept{return text_.data();} std::size_t SourceText::size()const noexcept{return text_.size();}
bool SourceText::empty()const noexcept{return text_.empty();}
char SourceText::at(SourceOffset o)const noexcept{return has(o)?text_[o]:'\0';}
bool SourceText::has(SourceOffset o)const noexcept{return o<text_.size();}
SourcePosition SourceText::positionAt(SourceOffset o)const noexcept{
 if(o>text_.size())o=text_.size(); SourceLine line=1; SourceOffset start=0;
 for(SourceOffset i=0;i<o;++i)if(text_[i]=='\n'){++line;start=i+1;}
 return {o,line,static_cast<SourceColumn>(o-start+1)};
}
SourceRange SourceText::range(SourceOffset b,SourceOffset e)const noexcept{return {positionAt(b),positionAt(e)};}
std::string_view SourceText::slice(SourceRange r)const noexcept{return slice(r.begin.offset,r.end.offset);}
std::string_view SourceText::slice(SourceOffset b,SourceOffset e)const noexcept{
 if(b>text_.size())b=text_.size(); if(e>text_.size())e=text_.size(); if(e<b)e=b; return {text_.data()+b,e-b};
}
SourceLine SourceText::lineAt(SourceOffset o)const noexcept{return positionAt(o).line;}
SourceColumn SourceText::columnAt(SourceOffset o)const noexcept{return positionAt(o).column;}
std::size_t SourceText::lineCount()const noexcept{return lines_.size();}
void SourceText::rebuildLineTable(){lines_.clear();SourceOffset start=0;SourceLine line=1;for(SourceOffset i=0;i<text_.size();++i)if(text_[i]=='\n'){lines_.push_back({line++,start,i});start=i+1;}lines_.push_back({line,start,text_.size()});}
SourceManager::FileID SourceManager::addSource(std::string p,std::string t){FileID id=entries_.size()+1;entries_.push_back({id,std::move(p),SourceText(std::move(t))});return id;}
SourceManager::FileID SourceManager::addSource(std::string p,std::string_view t){return addSource(std::move(p),std::string(t));}
bool SourceManager::contains(FileID id)const noexcept{return id>0&&id<=entries_.size();}
const std::string& SourceManager::path(FileID id)const{if(!contains(id))throw std::out_of_range("invalid file id");return entries_[id-1].path;}
const SourceText& SourceManager::source(FileID id)const{if(!contains(id))throw std::out_of_range("invalid file id");return entries_[id-1].text;}
SourceText& SourceManager::source(FileID id){if(!contains(id))throw std::out_of_range("invalid file id");return entries_[id-1].text;}
SourcePosition SourceManager::position(FileID id,SourceOffset o)const{return source(id).positionAt(o);}
SourceRange SourceManager::range(FileID id,SourceOffset b,SourceOffset e)const{return source(id).range(b,e);}
std::string_view SourceManager::slice(FileID id,SourceRange r)const{return source(id).slice(r);}
std::size_t SourceManager::size()const noexcept{return entries_.size();}
}
