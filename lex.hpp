#ifndef VARIANT_PARSE_LEX_HPP
#define VARIANT_PARSE_LEX_HPP

#include <string>


template <char>
struct C        {};

struct number   { double      value; };
struct ident    { std::string value; };
struct eof      {};
struct remember {};
struct forget   {};

class lex
{
public:
  lex(std::string s);
  std::string next_token();
  std::string peek();
  void drop();
private:
  std::string scan_token();
  std::string scan_id_or_keyword();
  std::string scan_number();

  std::string buffer;
  const char* iter;
  const char* end;
  std::exception_ptr lookahead;
};


#endif //VARIANT_PARSE_LEX_HPP
