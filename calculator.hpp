#ifndef VARIANT_PARSE_CALCULATOR_HPP
#define VARIANT_PARSE_CALCULATOR_HPP

#include <memory>
#include <map>
#include <string>
#include "lex.hpp"

class calculator
{
public:
  double parse(std::string s);

private:
  using variable_store = std::map<std::string, double>;
  double parse_forget_var();
  double parse_remember_var();
  double parse_expr();
  double parse_factor();
  double parse_term();
  double parse_paren();

  double drop_variable(const std::string& name);
  double lookup(const std::string& name);

  auto find_variable(const std::string& name) -> variable_store::iterator;

  std::unique_ptr<lex> lexer;
  variable_store       memory;
};

#endif //VARIANT_PARSE_CALCULATOR_HPP
