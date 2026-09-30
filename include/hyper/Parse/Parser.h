#pragma once

#include "hyper/Parser.h"
#include "hyper/Parse/ParserCore.h"
#include "hyper/Parse/ParserDecl.h"
#include "hyper/Parse/ParserDeclName.h"
#include "hyper/Parse/ParserDiagnostics.h"
#include "hyper/Parse/ParserExpr.h"
#include "hyper/Parse/ParserGeneric.h"
#include "hyper/Parse/ParserIfConfig.h"
#include "hyper/Parse/ParserPattern.h"
#include "hyper/Parse/ParserRegex.h"
#include "hyper/Parse/ParserRequests.h"
#include "hyper/Parse/ParserStmt.h"
#include "hyper/Parse/ParserType.h"
#include "hyper/Parse/ParserVersion.h"
#include "hyper/Parse/PersistentParserState.h"
#include "hyper/Parse/ParserDeclaration.h"
#include "hyper/Parse/ParserExpression.h"

namespace hyper::parse {

using ParserFacade = ::hyper::Parser;

} // namespace hyper::parse
