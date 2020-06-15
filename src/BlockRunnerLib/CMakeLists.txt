set(BlockRunnerLib_Headers src/BlockRunner/Task.h
                           src/BlockRunner/Diagram.h)

add_library(BlockRunnerLib ${BlockRunnerLib_Headers}
                           src/BlockRunner/Task.cpp
                           src/BlockRunner/Diagram.cpp)
add_library(BlockRunner::BlockRunnerLib ALIAS BlockRunnerLib)

set_target_properties(BlockRunnerLib PROPERTIES VERSION       ${${PROJECT_NAME}_VERSION}
                                                PUBLIC_HEADER "${BlockRunnerLib_Headers}")

target_include_directories(BlockRunnerLib PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
                                                 "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

target_compile_features(BlockRunnerLib PUBLIC cxx_std_14)
target_link_libraries(BlockRunnerLib PRIVATE spdlog::spdlog ${USED_DRAKE_LIBRARY})


install(TARGETS BlockRunnerLib
        EXPORT  ${PROJECT_NAME})
