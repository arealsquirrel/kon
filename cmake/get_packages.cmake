
CPMAddPackage("gh:gabime/spdlog@1.15.3")
CPMAddPackage("gh:google/googletest@1.17.0")

CPMAddPackage(
  NAME nlohmann_json
  VERSION 3.12.0
  # the git repo is incredibly large, so we download the archived include directory
  URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
  URL_HASH SHA256=42f6e95cad6ec532fd372391373363b62a14af6d771056dbfc86160e6dfff7aa
)

