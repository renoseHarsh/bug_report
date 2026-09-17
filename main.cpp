import std;
import glaze_wrapper;

int main() {
  type::Company company{};

  std::string json = R"({
        "name": "Drum",
        "address": "Mumbai"
    })";

  auto error = glaze::read_json(company, json);
  std::string buffer;
  std::string err = glaze::format_error(error, buffer);
  // start typing after this line
  // e.g. std::string hi = "random" crashes clangd

  return 0;
}
