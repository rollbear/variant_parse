#include "calculator.hpp"
#include "overload.hpp"

double calculator::parse(std::string s)
{
  lexer = std::make_unique<lex>(move(s));
  auto t  = lexer->peek();
  auto rv = std::visit(
    overload{
      [=](remember) { lexer->drop(); return parse_remember_var();},
      [=](forget)   { lexer->drop(); return parse_forget_var();},
      [=](auto)     { return parse_expr();}
    },
    t);
  if (!std::holds_alternative<eof>(lexer->next_token())) throw "garbage after expr";
  return rv;
}

double calculator::parse_forget_var()
{
  auto t = lexer->next_token();
  return std::visit(
    overload{
      [=](ident i)        { return drop_variable(i.value);},
      [=](auto) -> double { throw "expected variable name";}
    },
    t);
}

double calculator::parse_remember_var()
{
  auto var = lexer->next_token();
  if (!std::holds_alternative<ident>(var)) { throw "expected variable name"; }
  if (!std::holds_alternative<C<'='>>(lexer->next_token())) { throw "expected ="; }
  auto v = parse_expr();
  memory[std::string(std::get<ident>(var).value)] = v;
  return v;
}

double calculator::parse_expr()
{
  auto v = parse_term();
  for (bool done = false; !done;)
  {
    auto t = lexer->peek();
    v = std::visit(
      overload{
        [=](C<'+'>) { lexer->drop(); return v + parse_term(); },
        [=](C<'-'>) { lexer->drop(); return v - parse_term(); },
        [&](auto)   { done = true; return v; }
      },
      t);
  }
  return v;
}

double calculator::parse_factor()
{
  auto t = lexer->next_token();
  return std::visit(
    overload{
      [=](ident var)      { return lookup(var.value); },
      [=](number n)       { return n.value; },
      [=](C<'+'>)         { return parse_term(); },
      [=](C<'-'>)         { return -parse_term(); },
      [=](C<'('>)         { return parse_paren(); },
      [=](auto) -> double { throw "unexpected"; }
    },
    t);
}

double calculator::parse_term()
{
  auto v = parse_factor();
  for (bool done = false; !done;)
  {
    auto t = lexer->peek();
    v = std::visit(
      overload{
        [=](C<'/'>) { lexer->drop(); return v / parse_factor(); },
        [=](C<'*'>) { lexer->drop(); return v * parse_factor(); },
        [&](auto)   { done = true; return v; }
      },
      t);
  }
  return v;
}

double calculator::parse_paren()
{
  auto v = parse_expr();
  auto t = lexer->next_token();
  if (!std::holds_alternative<C<')'>>(t))
  {
    throw "expected ')'";
  }
  return v;
}

double calculator::drop_variable(std::string_view name)
{
  auto i = find_variable(name);
  auto rv = i->second;
  memory.erase(i);
  return rv;
}

double calculator::lookup(std::string_view name)
{
  return find_variable(name)->second;
}

auto
calculator::find_variable(std::string_view name)->variable_store::iterator
{
  auto i = memory.find(name);
  if (i == memory.end()) throw "unknown variable";
  return i;
}

