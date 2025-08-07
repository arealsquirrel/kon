
CPMAddPackage(
  NAME nlohmann_json
  VERSION 3.12.0
  # the git repo is incredibly large, so we download the archived include directory
  URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
  URL_HASH SHA256=42f6e95cad6ec532fd372391373363b62a14af6d771056dbfc86160e6dfff7aa
)

set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_DOCS OFF)
CPMAddPackage("gh:glfw/glfw#7b6aead")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	if(KN_BUILD_BENCHMARKS)
		set(BENCHMARK_ENABLE_TESTING OFF)
		set(BENCHMARK_ENABLE_ASSEMBLY_TESTS OFF)
		CPMAddPackage("gh:google/benchmark@1.9.4")
	endif()

	CPMAddPackage("gh:gabime/spdlog@1.15.3")
	CPMAddPackage("gh:google/googletest@1.17.0")
endif()

CPMAddPackage(
  NAME imgui
  VERSION 1.91.2
  GITHUB_REPOSITORY ocornut/imgui
  GIT_TAG v1.92.1-docking
  DOWNLOAD_ONLY TRUE
)

# CMakeLists.txt from https://gist.githubusercontent.com/rokups/f771217b2d530d170db5cb1e08e9a8f4
file(
  DOWNLOAD
  "https://gist.githubusercontent.com/rokups/f771217b2d530d170db5cb1e08e9a8f4/raw/4c2c14374ab878ca2f45daabfed4c156468e4e27/CMakeLists.txt"
  "${imgui_SOURCE_DIR}/CMakeLists.txt"
  EXPECTED_HASH SHA256=fd62f69364ce13a4f7633a9b50ae6672c466bcc44be60c69c45c0c6e225bb086
)

add_subdirectory(${imgui_SOURCE_DIR} EXCLUDE_FROM_ALL SYSTEM)

