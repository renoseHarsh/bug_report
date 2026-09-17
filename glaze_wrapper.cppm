module;

#include <glaze/glaze.hpp>

export module glaze_wrapper;

import std;

export namespace type {

struct Company {
  std::string name;
  std::optional<std::string> address;
};

}  // namespace type

template <>
struct glz::meta<type::Company> {
  static constexpr auto value = glz::object("name", &type::Company::name,
                                            "address", &type::Company::address);
};

export namespace glaze {
using glz::format_error;
using glz::read_json;
}  // namespace glaze
