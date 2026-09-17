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

  std::string hi = "asdfadf";

  return 0;
}
