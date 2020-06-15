add_executable(blockrunnerserver blockrunnerserver.cpp)

target_link_libraries(blockrunnerserver PRIVATE spdlog::spdlog BlockRunner::BlockRunnerLib)

install(TARGETS blockrunnerserver
        EXPORT  ${PROJECT_NAME})
