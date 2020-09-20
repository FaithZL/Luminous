//
// Created by Zero on 2020/9/11.
//

#pragma once

#include <ostream>

#include <compute/function.h>
#include <core/platform.h>
#include <core/header.h>
#include "expression.h"
#include "statement.h"

namespace luminous::compute {
    class Device;
}

namespace luminous::compute::dsl {

    class Codegen : Noncopyable {

    protected:
        std::ostream &_os;

    public:
        explicit Codegen(std::ostream &os) noexcept : _os{os} {}
        virtual ~Codegen() noexcept = default;
        virtual void emit(const Function &function) = 0;
    };

    // Example codegen for C++
    class CppCodegen : public Codegen, public ExprVisitor, public StmtVisitor {

    protected:
        int32_t _indent{0};

        bool _after_else{false};

        virtual void _emit_indent();
        virtual void _emit_function_decl(const Function &f);
        virtual void _emit_function_body(const Function &f);
        virtual void _emit_struct_decl(const TypeDesc *desc);
        virtual void _emit_variable(const Variable *v);
        virtual void _emit_type(const TypeDesc *desc);
        virtual void _emit_builtin_function_name(const std::string &func);

    public:
        explicit CppCodegen(std::ostream &os) noexcept : Codegen{os} {}
        ~CppCodegen() noexcept override = default;

        void emit(const Function &f) override;
        void visit(const UnaryExpr *unary_expr) override;
        void visit(const BinaryExpr *binary_expr) override;
        void visit(const MemberExpr *member_expr) override;
        void visit(const ValueExpr *literal_expr) override;
        void visit(const CallExpr *func_expr) override;
        void visit(const CastExpr *cast_expr) override;
        void visit(const TextureExpr *tex_expr) override;
        void visit(const EmptyStmt *stmt) override;
        void visit(const BreakStmt *stmt) override;
        void visit(const ContinueStmt *stmt) override;
        void visit(const ReturnStmt *stmt) override;
        void visit(const ScopeStmt *scope_stmt) override;
        void visit(const DeclareStmt *declare_stmt) override;
        void visit(const IfStmt *if_stmt) override;
        void visit(const WhileStmt *while_stmt) override;
        void visit(const DoWhileStmt *do_while_stmt) override;
        void visit(const ExprStmt *expr_stmt) override;
        void visit(const SwitchStmt *switch_stmt) override;
        void visit(const SwitchCaseStmt *case_stmt) override;
        void visit(const SwitchDefaultStmt *default_stmt) override;
        void visit(const AssignStmt *assign_stmt) override;
    };
}