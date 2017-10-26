#include "calculator.hpp"

double calculator::parse(std::string s)
{
  lexer.reset(new lex(std::move(s)));
  try {
    lexer->peek();
  }
  catch (remember){ lexer->drop(); return parse_remember_var();}
  catch (forget)  { lexer->drop(); return parse_forget_var();}
  catch (...)     { return parse_expr();}
  try {
    lexer->next_token();
  }
  catch (eof) { }
  catch (...) { throw "garbage after expr";}
}

double calculator::parse_forget_var()
{
  try {
    lexer->next_token();
  }
  catch (const ident& i) { return drop_variable(i.value);}
  catch (...) { throw "expected variable name";}
}

double calculator::parse_remember_var()
{
  std::string id;
  try {
    lexer->next_token();
  }
  catch (ident i) { id = std::move(i.value);}
  catch (...) { throw "expected variable name";}
  try {
    lexer->next_token();
  }
  catch (C<'='>) {}
  catch (...) { throw "expected =";}
  auto v = parse_expr();
  memory[id] = v;
  return v;
}

double calculator::parse_expr()
{
  auto v = parse_term();
  for (bool done = false; !done;)
  {
    try {
      lexer->peek();
    }
    catch (C<'+'>) { lexer->drop();v += parse_term(); }
    catch (C<'-'>) { lexer->drop();v -= parse_term(); }
    catch (...)    { done = true; }
  }
  return v;
}

double calculator::parse_factor()
{
  try {
    lexer->next_token();
  }
  catch (ident var) { return lookup(var.value);}
  catch (number n)  { return n.value;}
  catch (C<'+'>)    { return parse_term(); }
  catch (C<'-'>)    { return -parse_term(); }
  catch (C<'('>)    { return parse_paren(); }
  catch (...)       {}
  throw "unexpected";
}

double calculator::parse_term()
{
  auto v = parse_factor();
  for (bool done = false; !done;)
  {
    try {
      lexer->peek();
    }
    catch (C<'/'>) { lexer->drop(); v /= parse_factor(); }
    catch (C<'*'>) { lexer->drop(); v *= parse_factor(); }
    catch (...)    { done = true; }
  }
  return v;
}

double calculator::parse_paren()
{
  auto v = parse_expr();
  try {
    lexer->next_token();
  }
  catch (C<')'>) { return v; }
  catch (...) { }
  throw "expected ')'";
}

double calculator::drop_variable(const std::string& name)
{
  auto i = find_variable(name);
  auto rv = i->second;
  memory.erase(i);
  return rv;
}

double calculator::lookup(const std::string& name)
{
  return find_variable(name)->second;
}

auto
calculator::find_variable(const std::string& name)->variable_store::iterator
{
  auto i = memory.find(name);
  if (i == memory.end()) throw "unknown variable";
  return i;
}

