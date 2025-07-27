project (kon-core VERSION 1.0.0)

file(GLOB_RECURSE SRCS src/kon/*.cpp)
file(GLOB_RECURSE HDRS src/kon/*.hpp)

add_compile_options(-Wall -Wextra -Wpedantic)

# Add a library with the above sources
add_library(${PROJECT_NAME} ${SRCS} ${HDRS})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}
	nlohmann_json::nlohmann_json
	spdlog
)


