#include <iostream>
#include <string>
#include "calculator.hpp"

int main()
{
  calculator c;
  std::string s;
  while (std::getline(std::cin, s))
  {
    try {
      std::cout << c.parse(std::move(s)) << '\n';
    }
    catch (const char* m)
    {
      std::cout << "oops: " << m << '\n';
    }
    s.clear();
  }

}