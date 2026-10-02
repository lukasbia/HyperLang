//===--- TokenKinds.h - HyperLang Token Definitions -----------------------===//

#ifndef HYPERLANG_PARSE_TOKENKINDS_H
#define HYPERLANG_PARSE_TOKENKINDS_H

#include <cstdint>
#include <string_view>

namespace hyperlang::tok {

enum class Kind : std::uint16_t {
  Unknown,
  EndOfFile,
  Identifier,
  EscapedIdentifier,
  IntegerLiteral,
  FloatingLiteral,
  StringLiteral,
  CharacterLiteral,
  AttributedName,
  Comment,
  KwFunc,
  KwMain,
  KwVar,
  KwLet,
  KwConst,
  KwIf,
  KwElse,
  KwThen,
  KwWhile,
  KwDo,
  KwFor,
  KwIn,
  KwLoop,
  KwBreak,
  KwContinue,
  KwReturn,
  KwDefer,
  KwGuard,
  KwSwitch,
  KwCase,
  KwDefault,
  KwEnum,
  KwStruct,
  KwClass,
  KwProtocol,
  KwExtension,
  KwType,
  KwTypealias,
  KwInit,
  KwDeinit,
  KwStatic,
  KwPublic,
  KwPrivate,
  KwInternal,
  KwProtected,
  KwImport,
  KwInclude,
  KwModule,
  KwNamespace,
  KwPackage,
  KwExtern,
  KwInline,
  KwNoReturn,
  KwAsync,
  KwAwait,
  KwThrows,
  KwThrow,
  KwCatch,
  KwTry,
  KwSome,
  KwAny,
  KwSelf,
  KwSuper,
  KwThis,
  KwTrue,
  KwFalse,
  KwNil,
  KwNull,
  KwOptional,
  KwResult,
  KwError,
  KwPanic,
  KwMove,
  KwCopy,
  KwBorrow,
  KwMut,
  KwUnsafe,
  KwAtomic,
  KwVolatile,
  KwFinal,
  KwOverride,
  KwOperator,
  KwWhere,
  KwWhen,
  KwWith,
  KwAs,
  KwIs,
  KwFrom,
  KwTo,
  KwUntil,
  KwBy,
  KwOutput,
  KwPrint,
  KwData,
  KwGetData,
  KwCreateData,
  KwBytes,
  KwString,
  KwBool,
  KwInt,
  KwUInt,
  KwInt8,
  KwInt16,
  KwInt32,
  KwInt64,
  KwUInt8,
  KwUInt16,
  KwUInt32,
  KwUInt64,
  KwFloat,
  KwDouble,
  KwNum,
  KwDecimal,
  KwRange,
  KwArray,
  KwMap,
  KwSet,
  KwTuple,
  KwFile,
  KwFileName,
  KwSection,
  KwKernel,
  KwOS,
  KwBinary,
  KwConnect,
  KwBackup,
  KwPass,
  KwDelete,
  KwDestroy,
  KwCut,
  KwEnd,
  KwBlock,
  KwShrink,
  KwMath,
  KwLine,
  KwBase,
  KwMessage,
  KwSink,
  KwPlay,
  KwControl,
  KwChange,
  KwUse,
  KwDont,
  KwInstead,
  KwOf,
  KwSingle,
  KwPlaceholder,
  AtImport,
  AtInclude,
  AtFile,
  AtFileID,
  AtAPI,
  AtRepo,
  AtWebLink,
  AtDatabase,
  AtTarget,
  AtAvailable,
  AtMain,
  AtTest,
  AtDeprecated,
  AtAvailableFrom,
  AtUnknown,
  Plus,
  Minus,
  Star,
  Slash,
  Percent,
  Equal,
  EqualEqual,
  NotEqual,
  Less,
  LessEqual,
  Greater,
  GreaterEqual,
  Ampersand,
  AmpersandAmpersand,
  Pipe,
  PipePipe,
  Caret,
  Tilde,
  TildeEqual,
  Bang,
  Question,
  QuestionQuestion,
  Arrow,
  FatArrow,
  PlusEqual,
  MinusEqual,
  StarEqual,
  SlashEqual,
  PercentEqual,
  AmpersandEqual,
  PipeEqual,
  CaretEqual,
  ShiftLeft,
  ShiftRight,
  ShiftLeftEqual,
  ShiftRightEqual,
  Increment,
  Decrement,
  Dot,
  DotDot,
  DotDotLess,
  Colon,
  DoubleColon,
  Comma,
  Semicolon,
  OpenParen,
  CloseParen,
  OpenBrace,
  CloseBrace,
  OpenBracket,
  CloseBracket,
  Backslash,
  Hash,
  At,
  Dollar,
  Backtick,
  UnknownOperator,
};

constexpr std::string_view getTokenName(Kind kind) {
  switch (kind) {
  case Kind::Unknown: return "unknown";
  case Kind::EndOfFile: return "eof";
  case Kind::Identifier: return "identifier";
  case Kind::EscapedIdentifier: return "escaped_identifier";
  case Kind::IntegerLiteral: return "integer_literal";
  case Kind::FloatingLiteral: return "floating_literal";
  case Kind::StringLiteral: return "string_literal";
  case Kind::CharacterLiteral: return "character_literal";
  case Kind::AttributedName: return "attributed_name";
  case Kind::Comment: return "comment";
  case Kind::KwFunc: return "func";
  case Kind::KwMain: return "main";
  case Kind::KwVar: return "var";
  case Kind::KwLet: return "let";
  case Kind::KwConst: return "const";
  case Kind::KwIf: return "if";
  case Kind::KwElse: return "else";
  case Kind::KwThen: return "then";
  case Kind::KwWhile: return "while";
  case Kind::KwDo: return "do";
  case Kind::KwFor: return "for";
  case Kind::KwIn: return "in";
  case Kind::KwLoop: return "loop";
  case Kind::KwBreak: return "break";
  case Kind::KwContinue: return "continue";
  case Kind::KwReturn: return "return";
  case Kind::KwDefer: return "defer";
  case Kind::KwGuard: return "guard";
  case Kind::KwSwitch: return "switch";
  case Kind::KwCase: return "case";
  case Kind::KwDefault: return "default";
  case Kind::KwEnum: return "enum";
  case Kind::KwStruct: return "struct";
  case Kind::KwClass: return "class";
  case Kind::KwProtocol: return "protocol";
  case Kind::KwExtension: return "extension";
  case Kind::KwType: return "type";
  case Kind::KwTypealias: return "typealias";
  case Kind::KwInit: return "init";
  case Kind::KwDeinit: return "deinit";
  case Kind::KwStatic: return "static";
  case Kind::KwPublic: return "public";
  case Kind::KwPrivate: return "private";
  case Kind::KwInternal: return "internal";
  case Kind::KwProtected: return "protected";
  case Kind::KwImport: return "import";
  case Kind::KwInclude: return "include";
  case Kind::KwModule: return "module";
  case Kind::KwNamespace: return "namespace";
  case Kind::KwPackage: return "package";
  case Kind::KwExtern: return "extern";
  case Kind::KwInline: return "inline";
  case Kind::KwNoReturn: return "noreturn";
  case Kind::KwAsync: return "async";
  case Kind::KwAwait: return "await";
  case Kind::KwThrows: return "throws";
  case Kind::KwThrow: return "throw";
  case Kind::KwCatch: return "catch";
  case Kind::KwTry: return "try";
  case Kind::KwSome: return "some";
  case Kind::KwAny: return "any";
  case Kind::KwSelf: return "self";
  case Kind::KwSuper: return "super";
  case Kind::KwThis: return "this";
  case Kind::KwTrue: return "true";
  case Kind::KwFalse: return "false";
  case Kind::KwNil: return "nil";
  case Kind::KwNull: return "null";
  case Kind::KwOptional: return "optional";
  case Kind::KwResult: return "result";
  case Kind::KwError: return "error";
  case Kind::KwPanic: return "panic";
  case Kind::KwMove: return "move";
  case Kind::KwCopy: return "copy";
  case Kind::KwBorrow: return "borrow";
  case Kind::KwMut: return "mut";
  case Kind::KwUnsafe: return "unsafe";
  case Kind::KwAtomic: return "atomic";
  case Kind::KwVolatile: return "volatile";
  case Kind::KwFinal: return "final";
  case Kind::KwOverride: return "override";
  case Kind::KwOperator: return "operator";
  case Kind::KwWhere: return "where";
  case Kind::KwWhen: return "when";
  case Kind::KwWith: return "with";
  case Kind::KwAs: return "as";
  case Kind::KwIs: return "is";
  case Kind::KwFrom: return "from";
  case Kind::KwTo: return "to";
  case Kind::KwUntil: return "until";
  case Kind::KwBy: return "by";
  case Kind::KwOutput: return "output";
  case Kind::KwPrint: return "print";
  case Kind::KwData: return "data";
  case Kind::KwGetData: return "getData";
  case Kind::KwCreateData: return "createData";
  case Kind::KwBytes: return "bytes";
  case Kind::KwString: return "string";
  case Kind::KwBool: return "bool";
  case Kind::KwInt: return "int";
  case Kind::KwUInt: return "uint";
  case Kind::KwInt8: return "int8";
  case Kind::KwInt16: return "int16";
  case Kind::KwInt32: return "int32";
  case Kind::KwInt64: return "int64";
  case Kind::KwUInt8: return "uint8";
  case Kind::KwUInt16: return "uint16";
  case Kind::KwUInt32: return "uint32";
  case Kind::KwUInt64: return "uint64";
  case Kind::KwFloat: return "float";
  case Kind::KwDouble: return "double";
  case Kind::KwNum: return "num";
  case Kind::KwDecimal: return "decimal";
  case Kind::KwRange: return "range";
  case Kind::KwArray: return "array";
  case Kind::KwMap: return "map";
  case Kind::KwSet: return "set";
  case Kind::KwTuple: return "tuple";
  case Kind::KwFile: return "file";
  case Kind::KwFileName: return "fileName";
  case Kind::KwSection: return "section";
  case Kind::KwKernel: return "kernel";
  case Kind::KwOS: return "os";
  case Kind::KwBinary: return "binary";
  case Kind::KwConnect: return "connect";
  case Kind::KwBackup: return "backup";
  case Kind::KwPass: return "pass";
  case Kind::KwDelete: return "delete";
  case Kind::KwDestroy: return "destroy";
  case Kind::KwCut: return "cut";
  case Kind::KwEnd: return "end";
  case Kind::KwBlock: return "block";
  case Kind::KwShrink: return "shrink";
  case Kind::KwMath: return "math";
  case Kind::KwLine: return "line";
  case Kind::KwBase: return "base";
  case Kind::KwMessage: return "message";
  case Kind::KwSink: return "sink";
  case Kind::KwPlay: return "play";
  case Kind::KwControl: return "control";
  case Kind::KwChange: return "change";
  case Kind::KwUse: return "use";
  case Kind::KwDont: return "dont";
  case Kind::KwInstead: return "instead";
  case Kind::KwOf: return "of";
  case Kind::KwSingle: return "single";
  case Kind::KwPlaceholder: return "placeholder";
  case Kind::@import: return "AtImport";
  case Kind::@include: return "AtInclude";
  case Kind::@file: return "AtFile";
  case Kind::@fileID: return "AtFileID";
  case Kind::@api: return "AtAPI";
  case Kind::@repo: return "AtRepo";
  case Kind::@webLink: return "AtWebLink";
  case Kind::@database: return "AtDatabase";
  case Kind::@target: return "AtTarget";
  case Kind::@available: return "AtAvailable";
  case Kind::@main: return "AtMain";
  case Kind::@test: return "AtTest";
  case Kind::@deprecated: return "AtDeprecated";
  case Kind::@availableFrom: return "AtAvailableFrom";
  case Kind::@unknown: return "AtUnknown";
  case Kind::Plus: return "+";
  case Kind::Minus: return "-";
  case Kind::Star: return "*";
  case Kind::Slash: return "/";
  case Kind::Percent: return "%";
  case Kind::Equal: return "=";
  case Kind::EqualEqual: return "==";
  case Kind::NotEqual: return "!=";
  case Kind::Less: return "<";
  case Kind::LessEqual: return "<=";
  case Kind::Greater: return ">";
  case Kind::GreaterEqual: return ">=";
  case Kind::Ampersand: return "&";
  case Kind::AmpersandAmpersand: return "&&";
  case Kind::Pipe: return "|";
  case Kind::PipePipe: return "||";
  case Kind::Caret: return "^";
  case Kind::Tilde: return "~";
  case Kind::TildeEqual: return "~=";
  case Kind::Bang: return "!";
  case Kind::Question: return "?";
  case Kind::QuestionQuestion: return "??";
  case Kind::Arrow: return "->";
  case Kind::FatArrow: return "=>";
  case Kind::PlusEqual: return "+=";
  case Kind::MinusEqual: return "-=";
  case Kind::StarEqual: return "*=";
  case Kind::SlashEqual: return "/=";
  case Kind::PercentEqual: return "%=";
  case Kind::AmpersandEqual: return "&=";
  case Kind::PipeEqual: return "|=";
  case Kind::CaretEqual: return "^=";
  case Kind::ShiftLeft: return "<<";
  case Kind::ShiftRight: return ">>";
  case Kind::ShiftLeftEqual: return "<<=";
  case Kind::ShiftRightEqual: return ">>=";
  case Kind::Increment: return "++";
  case Kind::Decrement: return "--";
  case Kind::Dot: return ".";
  case Kind::DotDot: return "..";
  case Kind::DotDotLess: return "..<";
  case Kind::Colon: return ":";
  case Kind::DoubleColon: return "::";
  case Kind::Comma: return ",";
  case Kind::Semicolon: return ";";
  case Kind::OpenParen: return "(";
  case Kind::CloseParen: return ")";
  case Kind::OpenBrace: return "{";
  case Kind::CloseBrace: return "}";
  case Kind::OpenBracket: return "[";
  case Kind::CloseBracket: return "]";
  case Kind::Backslash: return "\\";
  case Kind::Hash: return "#";
  case Kind::At: return "@";
  case Kind::Dollar: return "$";
  case Kind::Backtick: return "`";
  case Kind::UnknownOperator: return "operator";
  }
  return "unknown";
}

} // namespace hyperlang::tok

#endif
