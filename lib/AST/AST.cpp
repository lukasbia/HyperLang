#include "hyper/AST/AST.h"
namespace hyper::ast {
const char* nodeKindName(NodeKind k)noexcept{switch(k){case NodeKind::TranslationUnit:return "TranslationUnit";case NodeKind::Declaration:return "Declaration";case NodeKind::Expression:return "Expression";case NodeKind::Statement:return "Statement";case NodeKind::Type:return "Type";case NodeKind::Pattern:return "Pattern";case NodeKind::Attribute:return "Attribute";case NodeKind::Modifier:return "Modifier";case NodeKind::Unknown:return "Unknown";}return "Unknown";}
bool isDeclaration(NodeKind k)noexcept{return k==NodeKind::Declaration;} bool isExpression(NodeKind k)noexcept{return k==NodeKind::Expression;} bool isStatement(NodeKind k)noexcept{return k==NodeKind::Statement;} bool isType(NodeKind k)noexcept{return k==NodeKind::Type;} bool isPattern(NodeKind k)noexcept{return k==NodeKind::Pattern;}
bool contains(const SourceRange&r,std::size_t o)noexcept{return o>=r.begin.offset&&o<=r.end.offset;} bool precedes(const SourcePosition&a,const SourcePosition&b)noexcept{return a.offset<b.offset;}
} // namespace hyper::ast
