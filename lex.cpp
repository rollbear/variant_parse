#include <cassert>
#include "lex.hpp"

lex::lex(std::string s)
  : buffer{std::move(s)}
  , iter{buffer.c_str()}
  , end{iter + buffer.length()}
{
}

token lex::next_token()
{
  if (lookahead)
  {
    auto t = std::move(*lookahead);
    drop();
    return t;
  }
  return scan_token();
}

token lex::scan_token()
{
  for (;;)
  {
    if (iter == end) return {eof{}};
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
    case '(': ++iter;return {C<'('>{}};
    case ')': ++iter;return {C<')'>{}};
    case '/': ++iter;return {C<'/'>{}};
    case '*': ++iter;return {C<'*'>{}};
    case '=': ++iter;return {C<'='>{}};
    case '+': ++iter;return {C<'+'>{}};
    case '-': ++iter;return {C<'-'>{}};
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9': case '.':
      return scan_number();
  }
  throw "unknown";
}

token lex::scan_id_or_keyword()
{
  auto start = iter++;
  while (iter != end && (std::isalnum(*iter) || *iter == '_'))
    ++iter;
  std::string_view s{start, size_t(iter - start)};
  if (s == "remember") return token{remember{}};
  if (s == "forget") return token{forget{}};
  return token{ident{std::move(s)}};
}

token lex::scan_number()
{
  char* number_end;

  number rv;
  rv.value = std::strtod(iter, &number_end);
  iter = number_end;
  return {rv};
}

token lex::peek()
{
  if (!lookahead)
  {
    lookahead = scan_token();
  }
  return *lookahead;
}

void lex::drop()
{
  lookahead = std::nullopt;
}
