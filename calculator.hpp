#ifndef VARIANT_PARSE_CALCULATOR_HPP
#define VARIANT_PARSE_CALCULATOR_HPP

#include <memory>
#include <map>
#include <string>
#include <string_view>
#include "lex.hpp"

class calculator
{
public:
  double parse(std::string s);

private:
  using variable_store = std::map<std::string, double, std::less<>>;
  double parse_forget_var();
  double parse_remember_var();
  double parse_expr();
  double parse_factor();
  double parse_term();
  double parse_paren();

  double drop_variable(std::string_view name);
  double lookup(std::string_view name);

  auto find_variable(std::string_view name) -> variable_store::iterator;

  std::unique_ptr<lex> lexer;
  variable_store       memory;
};

#endif //VARIANT_PARSE_CALCULATOR_HPP
