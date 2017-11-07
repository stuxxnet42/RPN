#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP


#include <functional>
#include <iostream>
#include <sstream>
#include <map>
#include <cmath>
#include <stack>
#include <variant>

using op_t = std::variant<float, int, bool>;
using stack_t = std::stack<op_t>;
using op_fn_t = std::function<void(stack_t&)>;

template <typename Stack>
auto pop_top(Stack&& stack) {
    auto res = std::move(stack.top());
    stack.pop();
    return res;
}

template <typename Fn>
auto unpackSingleV(Fn fn) {
    return [fn](stack_t& stack) {
        std::visit([&](auto const& a) {
                fn(a);
        }, pop_top(stack));
    };
}

template <typename Fn>
auto unpackSingle(Fn fn) {
    return [fn](stack_t& stack) {
        std::visit([&](auto const& a) {
                stack.push(fn(a));
        }, pop_top(stack));
    };
}

template <typename Fn>
auto unpackPair(Fn fn) {
    return [fn](stack_t& stack) {
        std::visit([&](auto const& a) {
            std::visit([&](auto const& b) {
                stack.push(fn(b, a));
            }, pop_top(stack));
        }, pop_top(stack));
    };
}



namespace Operations{
    namespace Numeric{
        auto add = [](auto const& a, auto const& b) 
                      { return a + b; };
                      
        auto sub = [](auto const& a, auto const& b)
                      { return a - b; };
                      
        auto mul = [](auto const& a, auto const& b)
                      { return a * b; };
                      
        auto div = [](auto const& a, auto const& b)
                      { return a / b; };
                      
        auto print = [](auto const& a)
                      {std::cout<<a<<"\n"; };
    }
    
    namespace Int{
        auto mod = [](auto const& a, auto const& b)
                      { return int(a) % int(b); };
                      
        auto fac = [](auto const& a)
                      { int x=1; for(int i=1; i<a; i++)x*=i; return x; };
    }
    namespace Float{
        auto fmod = [](auto const& a, auto const& b)
                       { return fmodf(a,b); };
                       
        auto pow  = [](auto const& a, auto const& b)
                       { return powf(a,b); };
                       
        auto cos = [](auto const& a)
                      { return cosf(a); };
                      
        auto sin = [](auto const& a)
                      { return sinf(a); };
                      
        auto tan = [](auto const& a)
                      { return tanf(a); };
    }
    namespace Bool{
        auto add = [](auto const& a, auto const& b)
                      { return (bool(a) or bool(b)); };
        
        auto mul = [](auto const& a, auto const& b)
                      { return (bool(a) and bool(b)); };
        
        auto neg = [](auto const& a)
                      { return not(bool(a)); };
        
    }
}

auto genDefaultOps(){
    auto ops = std::map<std::string, op_fn_t>({
            {"+", unpackPair(Operations::Numeric::add)},
            {"-", unpackPair(Operations::Numeric::sub)},
            {"*", unpackPair(Operations::Numeric::mul)},
            {"/", unpackPair(Operations::Numeric::div)},
            {"p", unpackSingleV(Operations::Numeric::print)}
        });
    return ops;
}

auto genOps(int x){
    auto ops=genDefaultOps();
    ops.emplace( "%", unpackPair(Operations::Int::mod) );
    ops.emplace( "!", unpackSingle(Operations::Int::fac) );
    return ops;
}

auto genOps(float x){
    auto ops=genDefaultOps();
    ops.emplace( "%", unpackPair(Operations::Float::fmod) );
    ops.emplace( "cos", unpackSingle(Operations::Float::cos) );
    ops.emplace( "sin", unpackSingle(Operations::Float::sin) );
    ops.emplace( "tan", unpackSingle(Operations::Float::tan) );
    return ops;
}

auto genOps(bool x){
    auto ops = std::map<std::string, op_fn_t>({
        {"+", unpackPair(Operations::Bool::add)},
        {"*", unpackPair(Operations::Bool::mul)},
        {"!", unpackSingle(Operations::Bool::neg)},
        {"not", unpackSingle(Operations::Bool::neg)},
        {"p", unpackSingleV(Operations::Numeric::print)}
    });
    return ops;
}

auto getOps(op_t x){
    return std::visit([](auto &y){return genOps(y);}, x);
}

std::function<op_t(std::string&)> genTranslate(float x){
    auto lmbd = [](std::string& s){ return op_t(std::stof(s));};
    return lmbd;
}

std::function<op_t(std::string&)> genTranslate(int x){
    auto lmbd = [](std::string& s){ return op_t(std::stoi(s));};
    return lmbd;
}

std::function<op_t(std::string&)> genTranslate(bool x){
    auto lmbd = [](std::string& s){ return op_t(bool(std::stoi(s))); };
    return lmbd;
}

std::function<op_t(std::string&)> getTranslate(op_t x){
    return std::visit([](auto &y){return genTranslate(y);}, x);
}



#endif
