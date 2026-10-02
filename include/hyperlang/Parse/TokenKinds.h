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
  case Kind::func: return "KwFunc";
  case Kind::main: return "KwMain";
  case Kind::var: return "KwVar";
  case Kind::let: return "KwLet";
  case Kind::const: return "KwConst";
  case Kind::if: return "KwIf";
  case Kind::else: return "KwElse";
  case Kind::then: return "KwThen";
  case Kind::while: return "KwWhile";
  case Kind::do: return "KwDo";
  case Kind::for: return "KwFor";
  case Kind::in: return "KwIn";
  case Kind::loop: return "KwLoop";
  case Kind::break: return "KwBreak";
  case Kind::continue: return "KwContinue";
  case Kind::return: return "KwReturn";
  case Kind::defer: return "KwDefer";
  case Kind::guard: return "KwGuard";
  case Kind::switch: return "KwSwitch";
  case Kind::case: return "KwCase";
  case Kind::default: return "KwDefault";
  case Kind::enum: return "KwEnum";
  case Kind::struct: return "KwStruct";
  case Kind::class: return "KwClass";
  case Kind::protocol: return "KwProtocol";
  case Kind::extension: return "KwExtension";
  case Kind::type: return "KwType";
  case Kind::typealias: return "KwTypealias";
  case Kind::init: return "KwInit";
  case Kind::deinit: return "KwDeinit";
  case Kind::static: return "KwStatic";
  case Kind::public: return "KwPublic";
  case Kind::private: return "KwPrivate";
  case Kind::internal: return "KwInternal";
  case Kind::protected: return "KwProtected";
  case Kind::import: return "KwImport";
  case Kind::include: return "KwInclude";
  case Kind::module: return "KwModule";
  case Kind::namespace: return "KwNamespace";
  case Kind::package: return "KwPackage";
  case Kind::extern: return "KwExtern";
  case Kind::inline: return "KwInline";
  case Kind::noreturn: return "KwNoReturn";
  case Kind::async: return "KwAsync";
  case Kind::await: return "KwAwait";
  case Kind::throws: return "KwThrows";
  case Kind::throw: return "KwThrow";
  case Kind::catch: return "KwCatch";
  case Kind::try: return "KwTry";
  case Kind::some: return "KwSome";
  case Kind::any: return "KwAny";
  case Kind::self: return "KwSelf";
  case Kind::super: return "KwSuper";
  case Kind::this: return "KwThis";
  case Kind::true: return "KwTrue";
  case Kind::false: return "KwFalse";
  case Kind::nil: return "KwNil";
  case Kind::null: return "KwNull";
  case Kind::optional: return "KwOptional";
  case Kind::result: return "KwResult";
  case Kind::error: return "KwError";
  case Kind::panic: return "KwPanic";
  case Kind::move: return "KwMove";
  case Kind::copy: return "KwCopy";
  case Kind::borrow: return "KwBorrow";
  case Kind::mut: return "KwMut";
  case Kind::unsafe: return "KwUnsafe";
  case Kind::atomic: return "KwAtomic";
  case Kind::volatile: return "KwVolatile";
  case Kind::final: return "KwFinal";
  case Kind::override: return "KwOverride";
  case Kind::operator: return "KwOperator";
  case Kind::where: return "KwWhere";
  case Kind::when: return "KwWhen";
  case Kind::with: return "KwWith";
  case Kind::as: return "KwAs";
  case Kind::is: return "KwIs";
  case Kind::from: return "KwFrom";
  case Kind::to: return "KwTo";
  case Kind::until: return "KwUntil";
  case Kind::by: return "KwBy";
  case Kind::output: return "KwOutput";
  case Kind::print: return "KwPrint";
  case Kind::data: return "KwData";
  case Kind::getData: return "KwGetData";
  case Kind::createData: return "KwCreateData";
  case Kind::bytes: return "KwBytes";
  case Kind::string: return "KwString";
  case Kind::bool: return "KwBool";
  case Kind::int: return "KwInt";
  case Kind::uint: return "KwUInt";
  case Kind::int8: return "KwInt8";
  case Kind::int16: return "KwInt16";
  case Kind::int32: return "KwInt32";
  case Kind::int64: return "KwInt64";
  case Kind::uint8: return "KwUInt8";
  case Kind::uint16: return "KwUInt16";
  case Kind::uint32: return "KwUInt32";
  case Kind::uint64: return "KwUInt64";
  case Kind::float: return "KwFloat";
  case Kind::double: return "KwDouble";
  case Kind::num: return "KwNum";
  case Kind::decimal: return "KwDecimal";
  case Kind::range: return "KwRange";
  case Kind::array: return "KwArray";
  case Kind::map: return "KwMap";
  case Kind::set: return "KwSet";
  case Kind::tuple: return "KwTuple";
  case Kind::file: return "KwFile";
  case Kind::fileName: return "KwFileName";
  case Kind::section: return "KwSection";
  case Kind::kernel: return "KwKernel";
  case Kind::os: return "KwOS";
  case Kind::binary: return "KwBinary";
  case Kind::connect: return "KwConnect";
  case Kind::backup: return "KwBackup";
  case Kind::pass: return "KwPass";
  case Kind::delete: return "KwDelete";
  case Kind::destroy: return "KwDestroy";
  case Kind::cut: return "KwCut";
  case Kind::end: return "KwEnd";
  case Kind::block: return "KwBlock";
  case Kind::shrink: return "KwShrink";
  case Kind::math: return "KwMath";
  case Kind::line: return "KwLine";
  case Kind::base: return "KwBase";
  case Kind::message: return "KwMessage";
  case Kind::sink: return "KwSink";
  case Kind::play: return "KwPlay";
  case Kind::control: return "KwControl";
  case Kind::change: return "KwChange";
  case Kind::use: return "KwUse";
  case Kind::dont: return "KwDont";
  case Kind::instead: return "KwInstead";
  case Kind::of: return "KwOf";
  case Kind::single: return "KwSingle";
  case Kind::placeholder: return "KwPlaceholder";
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
