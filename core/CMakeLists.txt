project (kon-core VERSION 1.0.0)

file(GLOB_RECURSE SRCS src/kon/*.cpp)
file(GLOB_RECURSE WINDOWS_SRCS src/platform/windows/*.cpp)
file(GLOB_RECURSE LINUX_SRCS    src/platform/linux/*.cpp)

if (WIN32)
	list(APPEND SRCS ${WINDOWS_SRCS})
elseif(UNIX)
	list(APPEND SRCS ${LINUX_SRCS})
endif (WIN32)

add_compile_options(-Wall -Wextra -fms-extensions)

# Add a library with the above sources
add_library(${PROJECT_NAME} ${SRCS} ${HDRS} vendor/implementation.cpp)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}
	nlohmann_json::nlohmann_json
	spdlog
	glfw
)


