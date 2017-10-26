#ifndef VARIANT_PARSE_LEX_HPP
#define VARIANT_PARSE_LEX_HPP

#include <variant>
#include <string>
#include <optional>
#include <string_view>


template <char>
struct C        {};

struct number   { double           value; };
struct ident    { std::string_view value; };
struct eof      {};
struct remember {};
struct forget   {};

using token = std::variant<
  number,
  ident,
  C<'-'>,
  C<'+'>,
  C<'/'>,
  C<'*'>,
  C<'('>,
  C<')'>,
  C<'='>,
  eof,
  remember,
  forget
>;

class lex
{
public:
  lex(std::string s);
  token next_token();
  token peek();
  void drop();
private:
  token scan_token();
  token scan_id_or_keyword();
  token scan_number();

  std::string buffer;
  const char* iter;
  const char* end;
  std::optional<token> lookahead;
};


#endif //VARIANT_PARSE_LEX_HPP
