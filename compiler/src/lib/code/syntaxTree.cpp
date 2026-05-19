#include "../../lib/header/syntaxTree.h"

namespace syntax {

// The base destructor is already virtual – we provide an empty implementation
// for completeness, but it's not strictly required.
// Most nodes are polymorphic, so we define a dummy function to satisfy the linker.

// Optional: utility to convert NodeKind to string (for debugging)
const char* nodeKindToString(NodeKind kind) {
    switch (kind) {
        case NodeKind::TranslationUnit: return "TranslationUnit";
        case NodeKind::IncludeDecl: return "IncludeDecl";
        case NodeKind::MemType: return "MemType";
        case NodeKind::StructType: return "StructType";
        case NodeKind::ArrayType: return "ArrayType";
        case NodeKind::PointerType: return "PointerType";
        case NodeKind::ReferenceType: return "ReferenceType";
        case NodeKind::RegPinnedType: return "RegPinnedType";
        case NodeKind::VariableDecl: return "VariableDecl";
        case NodeKind::FunctionDecl: return "FunctionDecl";
        case NodeKind::StructDecl: return "StructDecl";
        case NodeKind::OperationDecl: return "OperationDecl";
        case NodeKind::GenericParam: return "GenericParam";
        case NodeKind::Constraint: return "Constraint";
        case NodeKind::BlockStmt: return "BlockStmt";
        case NodeKind::ExprStmt: return "ExprStmt";
        case NodeKind::IfStmt: return "IfStmt";
        case NodeKind::ForStmt: return "ForStmt";
        case NodeKind::WhileStmt: return "WhileStmt";
        case NodeKind::DoWhileStmt: return "DoWhileStmt";
        case NodeKind::SwitchStmt: return "SwitchStmt";
        case NodeKind::CaseLabel: return "CaseLabel";
        case NodeKind::DefaultLabel: return "DefaultLabel";
        case NodeKind::BreakStmt: return "BreakStmt";
        case NodeKind::ContinueStmt: return "ContinueStmt";
        case NodeKind::ReturnStmt: return "ReturnStmt";
        case NodeKind::TryStmt: return "TryStmt";
        case NodeKind::CatchClause: return "CatchClause";
        case NodeKind::ThrowStmt: return "ThrowStmt";
        case NodeKind::ErrStmt: return "ErrStmt";
        case NodeKind::IntegerLiteral: return "IntegerLiteral";
        case NodeKind::FloatLiteral: return "FloatLiteral";
        case NodeKind::CharLiteral: return "CharLiteral";
        case NodeKind::IdentifierExpr: return "IdentifierExpr";
        case NodeKind::UnaryOpExpr: return "UnaryOpExpr";
        case NodeKind::BinaryOpExpr: return "BinaryOpExpr";
        case NodeKind::CallExpr: return "CallExpr";
        case NodeKind::ArrayIndexExpr: return "ArrayIndexExpr";
        case NodeKind::RangeExpr: return "RangeExpr";
        case NodeKind::RangeRefExpr: return "RangeRefExpr";
        case NodeKind::MemberAccessExpr: return "MemberAccessExpr";
        case NodeKind::ThisExpr: return "ThisExpr";
        case NodeKind::ErrExpr: return "ErrExpr";
        case NodeKind::TernaryExpr: return "TernaryExpr";
        case NodeKind::InitializerListExpr: return "InitializerListExpr";
        case NodeKind::MetaBlock: return "MetaBlock";
        case NodeKind::DefineValue: return "DefineValue";
        case NodeKind::MacroDefinition: return "MacroDefinition";
        case NodeKind::CompileTimeFunctionDecl: return "CompileTimeFunctionDecl";
        case NodeKind::MetaIfStmt: return "MetaIfStmt";
        case NodeKind::MetaForStmt: return "MetaForStmt";
    }
    return "Unknown";
}


} // namespace merix