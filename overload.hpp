#ifndef VARIANT_PARSE_OVERLOAD_HPP
#define VARIANT_PARSE_OVERLOAD_HPP

template <typename ... F>
class overload : private F...
{
public:
  overload(F... f) : F(f)... {}
  using F::operator()...;
};

#endif //VARIANT_PARSE_OVERLOAD_HPP
