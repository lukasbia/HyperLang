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
  case Kind::KwFunc: return "KwFunc";
  case Kind::KwMain: return "KwMain";
  case Kind::KwVar: return "KwVar";
  case Kind::KwLet: return "KwLet";
  case Kind::KwConst: return "KwConst";
  case Kind::KwIf: return "KwIf";
  case Kind::KwElse: return "KwElse";
  case Kind::KwThen: return "KwThen";
  case Kind::KwWhile: return "KwWhile";
  case Kind::KwDo: return "KwDo";
  case Kind::KwFor: return "KwFor";
  case Kind::KwIn: return "KwIn";
  case Kind::KwLoop: return "KwLoop";
  case Kind::KwBreak: return "KwBreak";
  case Kind::KwContinue: return "KwContinue";
  case Kind::KwReturn: return "KwReturn";
  case Kind::KwDefer: return "KwDefer";
  case Kind::KwGuard: return "KwGuard";
  case Kind::KwSwitch: return "KwSwitch";
  case Kind::KwCase: return "KwCase";
  case Kind::KwDefault: return "KwDefault";
  case Kind::KwEnum: return "KwEnum";
  case Kind::KwStruct: return "KwStruct";
  case Kind::KwClass: return "KwClass";
  case Kind::KwProtocol: return "KwProtocol";
  case Kind::KwExtension: return "KwExtension";
  case Kind::KwType: return "KwType";
  case Kind::KwTypealias: return "KwTypealias";
  case Kind::KwInit: return "KwInit";
  case Kind::KwDeinit: return "KwDeinit";
  case Kind::KwStatic: return "KwStatic";
  case Kind::KwPublic: return "KwPublic";
  case Kind::KwPrivate: return "KwPrivate";
  case Kind::KwInternal: return "KwInternal";
  case Kind::KwProtected: return "KwProtected";
  case Kind::KwImport: return "KwImport";
  case Kind::KwInclude: return "KwInclude";
  case Kind::KwModule: return "KwModule";
  case Kind::KwNamespace: return "KwNamespace";
  case Kind::KwPackage: return "KwPackage";
  case Kind::KwExtern: return "KwExtern";
  case Kind::KwInline: return "KwInline";
  case Kind::KwNoReturn: return "KwNoReturn";
  case Kind::KwAsync: return "KwAsync";
  case Kind::KwAwait: return "KwAwait";
  case Kind::KwThrows: return "KwThrows";
  case Kind::KwThrow: return "KwThrow";
  case Kind::KwCatch: return "KwCatch";
  case Kind::KwTry: return "KwTry";
  case Kind::KwSome: return "KwSome";
  case Kind::KwAny: return "KwAny";
  case Kind::KwSelf: return "KwSelf";
  case Kind::KwSuper: return "KwSuper";
  case Kind::KwThis: return "KwThis";
  case Kind::KwTrue: return "KwTrue";
  case Kind::KwFalse: return "KwFalse";
  case Kind::KwNil: return "KwNil";
  case Kind::KwNull: return "KwNull";
  case Kind::KwOptional: return "KwOptional";
  case Kind::KwResult: return "KwResult";
  case Kind::KwError: return "KwError";
  case Kind::KwPanic: return "KwPanic";
  case Kind::KwMove: return "KwMove";
  case Kind::KwCopy: return "KwCopy";
  case Kind::KwBorrow: return "KwBorrow";
  case Kind::KwMut: return "KwMut";
  case Kind::KwUnsafe: return "KwUnsafe";
  case Kind::KwAtomic: return "KwAtomic";
  case Kind::KwVolatile: return "KwVolatile";
  case Kind::KwFinal: return "KwFinal";
  case Kind::KwOverride: return "KwOverride";
  case Kind::KwOperator: return "KwOperator";
  case Kind::KwWhere: return "KwWhere";
  case Kind::KwWhen: return "KwWhen";
  case Kind::KwWith: return "KwWith";
  case Kind::KwAs: return "KwAs";
  case Kind::KwIs: return "KwIs";
  case Kind::KwFrom: return "KwFrom";
  case Kind::KwTo: return "KwTo";
  case Kind::KwUntil: return "KwUntil";
  case Kind::KwBy: return "KwBy";
  case Kind::KwOutput: return "KwOutput";
  case Kind::KwPrint: return "KwPrint";
  case Kind::KwData: return "KwData";
  case Kind::KwGetData: return "KwGetData";
  case Kind::KwCreateData: return "KwCreateData";
  case Kind::KwBytes: return "KwBytes";
  case Kind::KwString: return "KwString";
  case Kind::KwBool: return "KwBool";
  case Kind::KwInt: return "KwInt";
  case Kind::KwUInt: return "KwUInt";
  case Kind::KwInt8: return "KwInt8";
  case Kind::KwInt16: return "KwInt16";
  case Kind::KwInt32: return "KwInt32";
  case Kind::KwInt64: return "KwInt64";
  case Kind::KwUInt8: return "KwUInt8";
  case Kind::KwUInt16: return "KwUInt16";
  case Kind::KwUInt32: return "KwUInt32";
  case Kind::KwUInt64: return "KwUInt64";
  case Kind::KwFloat: return "KwFloat";
  case Kind::KwDouble: return "KwDouble";
  case Kind::KwNum: return "KwNum";
  case Kind::KwDecimal: return "KwDecimal";
  case Kind::KwRange: return "KwRange";
  case Kind::KwArray: return "KwArray";
  case Kind::KwMap: return "KwMap";
  case Kind::KwSet: return "KwSet";
  case Kind::KwTuple: return "KwTuple";
  case Kind::KwFile: return "KwFile";
  case Kind::KwFileName: return "KwFileName";
  case Kind::KwSection: return "KwSection";
  case Kind::KwKernel: return "KwKernel";
  case Kind::KwOS: return "KwOS";
  case Kind::KwBinary: return "KwBinary";
  case Kind::KwConnect: return "KwConnect";
  case Kind::KwBackup: return "KwBackup";
  case Kind::KwPass: return "KwPass";
  case Kind::KwDelete: return "KwDelete";
  case Kind::KwDestroy: return "KwDestroy";
  case Kind::KwCut: return "KwCut";
  case Kind::KwEnd: return "KwEnd";
  case Kind::KwBlock: return "KwBlock";
  case Kind::KwShrink: return "KwShrink";
  case Kind::KwMath: return "KwMath";
  case Kind::KwLine: return "KwLine";
  case Kind::KwBase: return "KwBase";
  case Kind::KwMessage: return "KwMessage";
  case Kind::KwSink: return "KwSink";
  case Kind::KwPlay: return "KwPlay";
  case Kind::KwControl: return "KwControl";
  case Kind::KwChange: return "KwChange";
  case Kind::KwUse: return "KwUse";
  case Kind::KwDont: return "KwDont";
  case Kind::KwInstead: return "KwInstead";
  case Kind::KwOf: return "KwOf";
  case Kind::KwSingle: return "KwSingle";
  case Kind::KwPlaceholder: return "KwPlaceholder";
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
