#pragma once

#include <vector>
#include <memory>
#include <string>
#include <optional>
#include <cstdint>
#include "lexerTokens.h"
// Forward declaration of TokenKind (provided by the lexer)
// In practice you would include "Token.hpp" – here we just declare the enum.

namespace syntax {

// -----------------------------------------------------------------------------
// Node kinds
// -----------------------------------------------------------------------------
enum class NodeKind {
    // Translation unit
    TranslationUnit,
    IncludeDecl,

    // Types
    MemType,
    StructType,
    ArrayType,
    PointerType,
    ReferenceType,
    RegPinnedType,
    VRegType,
    VRegRefExpr,

    // Declarations
    VariableDecl,
    FunctionDecl,
    StructDecl,
    OperationDecl,
    GenericParam,
    Constraint,

    // Statements
    BlockStmt,
    ExprStmt,
    IfStmt,
    ForStmt,
    WhileStmt,
    DoWhileStmt,
    SwitchStmt,
    CaseLabel,
    DefaultLabel,
    BreakStmt,
    ContinueStmt,
    ReturnStmt,
    TryStmt,
    CatchClause,
    ThrowStmt,
    ErrStmt,

    // Expressions
    IntegerLiteral,
    FloatLiteral,
    CharLiteral,
    IdentifierExpr,
    UnaryOpExpr,
    BinaryOpExpr,
    CallExpr,
    ArrayIndexExpr,
    RangeExpr,
    RangeRefExpr,
    MemberAccessExpr,
    ThisExpr,
    ErrExpr,
    TernaryExpr,
    InitializerListExpr,

    // Compile‑time (meta)
    MetaBlock,
    DefineValue,
    MacroDefinition,
    CompileTimeFunctionDecl,
    MetaIfStmt,
    MetaForStmt,
};

// -----------------------------------------------------------------------------
// Base Node
// -----------------------------------------------------------------------------
struct Node {
    NodeKind kind;
    // SourceLocation loc; // omitted for brevity

    explicit Node(NodeKind k) : kind(k) {}
    virtual ~Node() = default;
};

// -----------------------------------------------------------------------------
// Translation unit
// -----------------------------------------------------------------------------
struct TranslationUnit : Node {
    std::vector<std::unique_ptr<Node>> declarations;
    TranslationUnit() : Node(NodeKind::TranslationUnit) {}
};

struct IncludeDecl : Node {
    std::string path;
    std::optional<std::string> alias;
    IncludeDecl() : Node(NodeKind::IncludeDecl) {}
};

// -----------------------------------------------------------------------------
// Types
// -----------------------------------------------------------------------------
struct MemType : Node {
    bool is_const = false;
    uint64_t integer_bits = 0;   // 0 means mem<> (void)
    uint64_t fraction_bits = 0;  // optional, default 0
    std::unique_ptr<Node> reg_pin; // RegPinnedType, if any

    MemType() : Node(NodeKind::MemType) {}
};

struct RegPinnedType : Node {
    std::string register_name;
    RegPinnedType() : Node(NodeKind::RegPinnedType) {}
};
struct VRegType : Node {
    std::vector<std::unique_ptr<Node>> parameters;
    VRegType() : Node(NodeKind::VRegType) {}
};
struct VRegRefExpr : Node {
    std::string variable_name;
    VRegRefExpr() : Node(NodeKind::VRegRefExpr) {}
};
struct StructType : Node {
    std::vector<std::string> struct_name;
    StructType() : Node(NodeKind::StructType) {}
};

struct ArrayType : Node {
    std::unique_ptr<Node> element_type;
    uint64_t size; // RangeExpr (e.g., [..N])
    ArrayType() : Node(NodeKind::ArrayType) {}
};

struct PointerType : Node {
    std::unique_ptr<Node> pointee_type;
    PointerType() : Node(NodeKind::PointerType) {}
};

struct ReferenceType : Node {
    std::unique_ptr<Node> referenced_type;
    bool is_const = false;
    ReferenceType() : Node(NodeKind::ReferenceType) {}
};

// -----------------------------------------------------------------------------
// Declarations
// -----------------------------------------------------------------------------
struct VariableDecl : Node {
    std::unique_ptr<Node> type;
    std::string name;
    std::unique_ptr<Node> initializer; // optional
    bool is_const = false;
    bool is_public = true;   // visibility: true = public, false = private

    VariableDecl() : Node(NodeKind::VariableDecl) {}
};

struct GenericParam : Node {
    enum class Kind { Type, Value } param_kind;
    std::string name;
    std::unique_ptr<Node> constraint; // Constraint node, optional
    GenericParam() : Node(NodeKind::GenericParam) {}
};

struct Constraint : Node {
    std::string trait_name;                // e.g., "Addable"
    std::unique_ptr<Node> requires_expr;   // e.g., operation<'+'>(...)
    Constraint() : Node(NodeKind::Constraint) {}
};

struct FunctionDecl : Node {
    std::vector<std::unique_ptr<GenericParam>> template_params;
    std::unique_ptr<Node> return_type;     // nullptr means void (mem<>)
    std::string name;
    std::vector<std::unique_ptr<VariableDecl>> parameters;
    std::unique_ptr<Node> body;            // BlockStmt, may be null
    bool is_operation = false;
    // Operation metadata (valid only if is_operation)
    char op_symbol = '\0';
    int precedence = 0;
    enum class Associativity { Left, Right } assoc = Associativity::Left;

    FunctionDecl() : Node(NodeKind::FunctionDecl) {}
};

// OperationDecl is just a FunctionDecl with is_operation = true
// (no extra data, but we keep a separate kind for clarity)
struct OperationDecl : FunctionDecl {
    OperationDecl() { kind = NodeKind::OperationDecl; is_operation = true; }
};

struct StructDecl : Node {
    std::string name;
    std::vector<std::unique_ptr<GenericParam>> template_params;
    std::vector<std::unique_ptr<Node>> base_classes; // StructType nodes
    std::vector<std::unique_ptr<Node>> members;      // VariableDecl, FunctionDecl, etc.
    StructDecl() : Node(NodeKind::StructDecl) {}
};

// Constructor and destructor are represented as FunctionDecl with special names
// e.g., name == struct_name for constructor, "~"+struct_name for destructor.

// -----------------------------------------------------------------------------
// Statements
// -----------------------------------------------------------------------------
struct BlockStmt : Node {
    std::vector<std::unique_ptr<Node>> statements;
    BlockStmt() : Node(NodeKind::BlockStmt) {}
};

struct ExprStmt : Node {
    std::unique_ptr<Node> expression;
    ExprStmt() : Node(NodeKind::ExprStmt) {}
};

struct IfStmt : Node {
    std::unique_ptr<Node> condition;
    std::unique_ptr<Node> then_stmt;
    std::unique_ptr<Node> else_stmt; // optional
    IfStmt() : Node(NodeKind::IfStmt) {}
};

struct ForStmt : Node {
    std::unique_ptr<Node> init;      // VariableDecl or ExprStmt
    std::unique_ptr<Node> condition;
    std::unique_ptr<Node> increment;
    std::unique_ptr<Node> body;
    ForStmt() : Node(NodeKind::ForStmt) {}
};

struct WhileStmt : Node {
    std::unique_ptr<Node> condition;
    std::unique_ptr<Node> body;
    WhileStmt() : Node(NodeKind::WhileStmt) {}
};

struct DoWhileStmt : Node {
    std::unique_ptr<Node> body;
    std::unique_ptr<Node> condition;
    DoWhileStmt() : Node(NodeKind::DoWhileStmt) {}
};

struct SwitchStmt : Node {
    std::unique_ptr<Node> condition;
    std::vector<std::unique_ptr<Node>> cases; // CaseLabel, DefaultLabel, BlockStmt
    SwitchStmt() : Node(NodeKind::SwitchStmt) {}
};

struct CaseLabel : Node {
    std::unique_ptr<Node> value;
    CaseLabel() : Node(NodeKind::CaseLabel) {}
};

struct DefaultLabel : Node {
    DefaultLabel() : Node(NodeKind::DefaultLabel) {}
};

struct BreakStmt : Node {
    BreakStmt() : Node(NodeKind::BreakStmt) {}
};

struct ContinueStmt : Node {
    ContinueStmt() : Node(NodeKind::ContinueStmt) {}
};

struct ReturnStmt : Node {
    std::unique_ptr<Node> value; // optional (void returns)
    ReturnStmt() : Node(NodeKind::ReturnStmt) {}
};

struct TryStmt : Node {
    std::unique_ptr<Node> try_block;                   // BlockStmt
    std::vector<std::unique_ptr<Node>> catches;        // CatchClause nodes
    std::unique_ptr<Node> finally_block;               // optional BlockStmt
    TryStmt() : Node(NodeKind::TryStmt) {}
};

struct CatchClause : Node {
    std::unique_ptr<Node> exception_type; // Type node
    std::string param_name;               // name of the caught variable (optional)
    std::unique_ptr<Node> handler_block;  // BlockStmt
    CatchClause() : Node(NodeKind::CatchClause) {}
};

struct ThrowStmt : Node {
    std::unique_ptr<Node> exception;
    ThrowStmt() : Node(NodeKind::ThrowStmt) {}
};

struct ErrStmt : Node {
    std::unique_ptr<Node> expression; // RHS of err = ...
    ErrStmt() : Node(NodeKind::ErrStmt) {}
};

// -----------------------------------------------------------------------------
// Expressions
// -----------------------------------------------------------------------------
struct IntegerLiteral : Node {
    uint64_t value;
    IntegerLiteral() : Node(NodeKind::IntegerLiteral) {}
};

struct FloatLiteral : Node {
    double value; // or store as string for arbitrary precision
    FloatLiteral() : Node(NodeKind::FloatLiteral) {}
};

struct CharLiteral : Node {
    char value;
    CharLiteral() : Node(NodeKind::CharLiteral) {}
};

struct IdentifierExpr : Node {
    std::string name;
    IdentifierExpr() : Node(NodeKind::IdentifierExpr) {}
};

struct UnaryOpExpr : Node {
    TokenKind op; // e.g., TOKEN_MINUS, TOKEN_TILDE
    std::unique_ptr<Node> operand;
    UnaryOpExpr() : Node(NodeKind::UnaryOpExpr) {}
};

struct BinaryOpExpr : Node {
    TokenKind op;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    BinaryOpExpr() : Node(NodeKind::BinaryOpExpr) {}
};

struct CallExpr : Node {
    std::unique_ptr<Node> callee; // IdentifierExpr or MemberAccessExpr
    std::vector<std::unique_ptr<Node>> arguments;
    CallExpr() : Node(NodeKind::CallExpr) {}
};

struct ArrayIndexExpr : Node {
    std::unique_ptr<Node> array;
    std::unique_ptr<Node> index; // expression
    ArrayIndexExpr() : Node(NodeKind::ArrayIndexExpr) {}
};

// Range: [lower .. upper] (lower optional, upper exclusive)
struct RangeExpr : Node {
    std::unique_ptr<Node> lower; // may be null (means 0)
    std::unique_ptr<Node> upper;
    RangeExpr() : Node(NodeKind::RangeExpr) {}
};

// Range reference: arr[lower..upper]&
struct RangeRefExpr : Node {
    std::unique_ptr<Node> array;
    std::unique_ptr<Node> range; // RangeExpr
    bool is_const = false;
    RangeRefExpr() : Node(NodeKind::RangeRefExpr) {}
};

struct MemberAccessExpr : Node {
    std::unique_ptr<Node> object;
    std::string member;
    bool is_arrow = false; // true for '->', false for '.'
    MemberAccessExpr() : Node(NodeKind::MemberAccessExpr) {}
};

struct ThisExpr : Node {
    ThisExpr() : Node(NodeKind::ThisExpr) {}
};

struct ErrExpr : Node {
    ErrExpr() : Node(NodeKind::ErrExpr) {}
};

struct TernaryExpr : Node {
    std::unique_ptr<Node> condition;
    std::unique_ptr<Node> true_expr;
    std::unique_ptr<Node> false_expr;
    TernaryExpr() : Node(NodeKind::TernaryExpr) {}
};

// Initializer list, e.g., [1, 2, 3] or character array from a string literal
struct InitializerListExpr : Node {
    std::vector<std::unique_ptr<Node>> elements;
    InitializerListExpr() : Node(NodeKind::InitializerListExpr) {}
};

// -----------------------------------------------------------------------------
// Compile‑time (<meta>) nodes
// -----------------------------------------------------------------------------
struct MetaBlock : Node {
    std::vector<std::unique_ptr<Node>> declarations;
    MetaBlock() : Node(NodeKind::MetaBlock) {}
};

struct DefineValue : Node {
    std::string name;
    std::unique_ptr<Node> value; // compile‑time expression
    DefineValue() : Node(NodeKind::DefineValue) {}
};

struct MacroDefinition : Node {
    std::string name;
    std::vector<std::unique_ptr<GenericParam>> template_params;
    std::unique_ptr<Node> body; // usually a BlockStmt or raw tokens
    MacroDefinition() : Node(NodeKind::MacroDefinition) {}
};

struct CompileTimeFunctionDecl : FunctionDecl {
    CompileTimeFunctionDecl() { kind = NodeKind::CompileTimeFunctionDecl; }
};

struct MetaIfStmt : Node {
    std::unique_ptr<Node> condition;
    std::unique_ptr<Node> then_body; // MetaBlock or list
    std::unique_ptr<Node> else_body;
    MetaIfStmt() : Node(NodeKind::MetaIfStmt) {}
};

struct MetaForStmt : Node {
    std::unique_ptr<Node> init;
    std::unique_ptr<Node> condition;
    std::unique_ptr<Node> increment;
    std::unique_ptr<Node> body;
    MetaForStmt() : Node(NodeKind::MetaForStmt) {}
};

// -----------------------------------------------------------------------------
// Simple Visitor (optional)
// -----------------------------------------------------------------------------
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    virtual void visit(const TranslationUnit&) {}
    virtual void visit(const IncludeDecl&) {}
    virtual void visit(const MemType&) {}
    virtual void visit(const RegPinnedType&) {}
    virtual void visit(const StructType&) {}
    virtual void visit(const ArrayType&) {}
    virtual void visit(const PointerType&) {}
    virtual void visit(const ReferenceType&) {}
    virtual void visit(const VariableDecl&) {}
    virtual void visit(const GenericParam&) {}
    virtual void visit(const Constraint&) {}
    virtual void visit(const FunctionDecl&) {}
    virtual void visit(const OperationDecl&) {}
    virtual void visit(const StructDecl&) {}
    virtual void visit(const BlockStmt&) {}
    virtual void visit(const ExprStmt&) {}
    virtual void visit(const IfStmt&) {}
    virtual void visit(const ForStmt&) {}
    virtual void visit(const WhileStmt&) {}
    virtual void visit(const DoWhileStmt&) {}
    virtual void visit(const SwitchStmt&) {}
    virtual void visit(const CaseLabel&) {}
    virtual void visit(const DefaultLabel&) {}
    virtual void visit(const BreakStmt&) {}
    virtual void visit(const ContinueStmt&) {}
    virtual void visit(const ReturnStmt&) {}
    virtual void visit(const TryStmt&) {}
    virtual void visit(const CatchClause&) {}
    virtual void visit(const ThrowStmt&) {}
    virtual void visit(const ErrStmt&) {}
    virtual void visit(const IntegerLiteral&) {}
    virtual void visit(const FloatLiteral&) {}
    virtual void visit(const CharLiteral&) {}
    virtual void visit(const IdentifierExpr&) {}
    virtual void visit(const UnaryOpExpr&) {}
    virtual void visit(const BinaryOpExpr&) {}
    virtual void visit(const CallExpr&) {}
    virtual void visit(const ArrayIndexExpr&) {}
    virtual void visit(const RangeExpr&) {}
    virtual void visit(const RangeRefExpr&) {}
    virtual void visit(const MemberAccessExpr&) {}
    virtual void visit(const ThisExpr&) {}
    virtual void visit(const ErrExpr&) {}
    virtual void visit(const TernaryExpr&) {}
    virtual void visit(const InitializerListExpr&) {}
    virtual void visit(const MetaBlock&) {}
    virtual void visit(const DefineValue&) {}
    virtual void visit(const MacroDefinition&) {}
    virtual void visit(const CompileTimeFunctionDecl&) {}
    virtual void visit(const MetaIfStmt&) {}
    virtual void visit(const MetaForStmt&) {}
    virtual void visit(const VRegType&) {}
    virtual void visit(const VRegRefExpr&) {}
};

} // namespace merix