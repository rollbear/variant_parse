#include <cassert>
#include "lex.hpp"

lex::lex(std::string s)
  : buffer{std::move(s)}
  , iter{buffer.c_str()}
  , end{iter + buffer.length()}
{
}

std::string lex::next_token()
{
  if (lookahead)
  {
    auto t = std::move(lookahead);
    drop();
    std::rethrow_exception(t);
  }
  return scan_token();
}

std::string lex::scan_token()
{
  for (;;)
  {
    if (iter == end) throw eof{};
    if (*iter != ' ' && *iter != '\t' && *iter != '\n')
      break;
    ++iter;
  }
  if (isalpha(*iter))
  {
    return scan_id_or_keyword();
  }
  switch (*iter)
  {
    case '(': ++iter;throw C<'('>{};
    case ')': ++iter;throw C<')'>{};
    case '/': ++iter;throw C<'/'>{};
    case '*': ++iter;throw C<'*'>{};
    case '=': ++iter;throw C<'='>{};
    case '+': ++iter;throw C<'+'>{};
    case '-': ++iter;throw C<'-'>{};
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9': case '.':
      return scan_number();
  }
  return "unknown";
}

std::string lex::scan_id_or_keyword()
{
  auto start = iter++;
  while (iter != end && (std::isalnum(*iter) || *iter == '_'))
    ++iter;
  std::string s{start, iter};
  if (s == "remember") throw remember{};
  if (s == "forget") throw forget{};
  throw ident{std::move(s)};
}

std::string lex::scan_number()
{
  char* number_end;

  number rv;
  rv.value = std::strtod(iter, &number_end);
  iter = number_end;
  throw rv;
}

std::string lex::peek()
{
  if (!lookahead)
  {
    try
    {
      return scan_token();
    }
    catch (...)
    {
      lookahead = std::current_exception();
    }
  }
  std::rethrow_exception(lookahead);
}

void lex::drop()
{
  lookahead = nullptr;
}
