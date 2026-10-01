#pragma once
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <utility>
namespace hyper::ast {
enum class NodeKind { TranslationUnit,Declaration,Expression,Statement,Type,Pattern,Attribute,Modifier,Unknown };
struct SourcePosition { std::size_t offset=0,line=1,column=1; };
struct SourceRange { SourcePosition begin,end; };
class Node { public: virtual ~Node()=default; virtual NodeKind kind()const noexcept=0; virtual std::string_view kindName()const noexcept=0; };
class ASTContext { public: template<class T,class...A> T* create(A&&...a){auto p=std::make_unique<T>(std::forward<A>(a)...);T*r=p.get();nodes_.push_back(std::move(p));return r;} void clear(){nodes_.clear();} std::size_t size()const noexcept{return nodes_.size();} private: std::vector<std::unique_ptr<Node>> nodes_; };
const char* nodeKindName(NodeKind)noexcept; bool isDeclaration(NodeKind)noexcept; bool isExpression(NodeKind)noexcept; bool isStatement(NodeKind)noexcept; bool isType(NodeKind)noexcept; bool isPattern(NodeKind)noexcept; bool contains(const SourceRange&,std::size_t)noexcept; bool precedes(const SourcePosition&,const SourcePosition&)noexcept;
} // namespace hyper::ast
