//
// Created by Zero on 2020/9/8.
//

#pragma once

#include <utility>
#include <variant>

#include <compute/variable.h>

namespace luminous::compute::dsl {
    struct ExprVisitor;

    struct Expression : Noncopyable {
        virtual ~Expression() noexcept = default;
        virtual void accept(ExprVisitor &) const = 0;
    };

    // fwd-decl of derived expressions
    class UnaryExpr;
    class BinaryExpr;
    class MemberExpr;
    class ValueExpr;
    class CallExpr;
    class CastExpr;
    class TextureExpr;

    struct ExprVisitor {
        virtual void visit(const UnaryExpr *unary_expr) = 0;
        virtual void visit(const BinaryExpr *binary_expr) = 0;
        virtual void visit(const MemberExpr *member_expr) = 0;
        virtual void visit(const ValueExpr *literal_expr) = 0;
        virtual void visit(const CallExpr *func_expr) = 0;
        virtual void visit(const CastExpr *cast_expr) = 0;
        virtual void visit(const TextureExpr *tex_expr) = 0;
    };

    #define MAKE_EXPRESSION_ACCEPT_VISITOR()                                   \
    void accept(ExprVisitor &visitor) const override { visitor.visit(this); }  \


    enum struct UnaryOp {
        PLUS, MINUS,  // +x, -x
        NOT,          // !x
        BIT_NOT,      // ~x

        // Note: We deliberately support *NO* pre- and postfix inc/dec operators to avoid possible abuse
    };

    class UnaryExpr : public Expression {

    private:
        const Variable *_operand;
        UnaryOp _op;

    public:
        UnaryExpr(UnaryOp op, const Variable *operand) noexcept: _operand{operand}, _op{op} {}
        [[nodiscard]] const Variable *operand() const noexcept { return _operand; }
        [[nodiscard]] UnaryOp op() const noexcept { return _op; }
        MAKE_EXPRESSION_ACCEPT_VISITOR()
    };

    enum struct BinaryOp {

        // arithmetic
        ADD, SUB, MUL, DIV, MOD,
        BIT_AND, BIT_OR, BIT_XOR, SHL, SHR,
        AND, OR,

        // relational
        LESS, GREATER, LESS_EQUAL, GREATER_EQUAL, EQUAL, NOT_EQUAL,

        // operator[]
        ACCESS,
    };

    class BinaryExpr : public Expression {

    private:
        const Variable *_lhs;
        const Variable *_rhs;
        BinaryOp _op;

    public:
        BinaryExpr(BinaryOp op, const Variable *lhs, const Variable *rhs) noexcept
                : _op{op}, _lhs{lhs}, _rhs{rhs} {}

        [[nodiscard]] const Variable *lhs() const noexcept { return _lhs; }
        [[nodiscard]] const Variable *rhs() const noexcept { return _rhs; }
        [[nodiscard]] BinaryOp op() const noexcept { return _op; }
        MAKE_EXPRESSION_ACCEPT_VISITOR()
    };

    class MemberExpr : public Expression {

    private:
        const Variable *_self;
        std::string _member;

    public:
        MemberExpr(const Variable *self, std::string member) noexcept: _self{self}, _member{std::move(member)} {}
        [[nodiscard]] const Variable *self() const noexcept { return _self; }
        [[nodiscard]] const std::string &member() const noexcept { return _member; }
        MAKE_EXPRESSION_ACCEPT_VISITOR()
    };

    class ValueExpr : public Expression {

    public:
        using Value = std::variant<
                bool, float, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t,
                bool2, float2, char2, uchar2, short2, ushort2, int2, uint2,
                bool3, float3, char3, uchar3, short3, ushort3, int3, uint3,
                bool4, float4, char4, uchar4, short4, ushort4, int4, uint4,
                float3x3, float4x4>;

    private:
        Value _value;

    public:
        template<typename T,
                std::enable_if_t<std::is_convertible_v<T, Value>, int> = 0>
        explicit ValueExpr(T &&value) noexcept : _value{std::forward<T>(value)} {}

        [[nodiscard]] const Value &value() const noexcept { return _value; }
        MAKE_EXPRESSION_ACCEPT_VISITOR()
    };

    class CallExpr : public Expression {

    private:
        std::string _name;
        std::vector<const Variable *> _arguments;

    public:
        CallExpr(std::string name, std::vector<const Variable *> args) noexcept
                : _name{std::move(name)}, _arguments{std::move(args)} {}

        [[nodiscard]] const std::string &name() const noexcept { return _name; }
        [[nodiscard]] const std::vector<const Variable *> &arguments() const noexcept { return _arguments; }
        MAKE_EXPRESSION_ACCEPT_VISITOR()
    };

    enum struct CastOp {
        STATIC,
        REINTERPRET,
        BITWISE
    };

    class CastExpr : public Expression {

    private:
        const Variable *_source;
        CastOp _op;
        const TypeDesc *_dest_type;

    public:
        CastExpr(CastOp op, const Variable *src, const TypeDesc *dest) noexcept
                : _op{op}, _source{src}, _dest_type{dest} {}
        [[nodiscard]] CastOp op() const noexcept { return _op; }
        [[nodiscard]] const Variable *source() const noexcept { return _source; }
        [[nodiscard]] const TypeDesc *dest_type() const noexcept { return _dest_type; }
        MAKE_EXPRESSION_ACCEPT_VISITOR()
    };

    enum struct TextureOp : uint32_t {
        READ, WRITE, SAMPLE
    };

    class TextureExpr : public Expression {

    private:
        const Variable *_texture;
        const Variable *_coord;
        const Variable *_value;
        TextureOp _op;

    public:
        TextureExpr(TextureOp op, const Variable *tex, const Variable *uv, const Variable *value = nullptr) noexcept
                : _texture{tex}, _coord{uv}, _value{value}, _op{op} {}

        [[nodiscard]] TextureOp op() const noexcept { return _op; }
        [[nodiscard]] const Variable *texture() const noexcept { return _texture; }
        [[nodiscard]] const Variable *coord() const noexcept { return _coord; }
        [[nodiscard]] const Variable *value() const noexcept { return _value; }

        MAKE_EXPRESSION_ACCEPT_VISITOR()
    };
#undef MAKE_EXPRESSION_ACCEPT_VISITOR
}