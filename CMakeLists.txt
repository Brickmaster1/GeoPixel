project(GeoPixel)

add_executable(${PROJECT_NAME}
               main.cpp
#
               MainGame.h                        MainGame.cpp
               AutoLoader.h
               Projectile.h                      Projectile.cpp
#
               events/WindowEvent.h              events/WindowEvent.cpp
               events/PhysicsProcessingEvent.h   events/PhysicsProcessingEvent.cpp
               events/PlayerMoveEvent.h          events/PlayerMoveEvent.cpp
               EventRegistration.h
)

target_include_directories(${PROJECT_NAME} PUBLIC "/")

if(VCPKG_TARGET_TRIPLET STREQUAL "wasm32-emscripten")
    # Output an html file (along with the js and wasm of course)
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")

    # For clangd
    execute_process(COMMAND em++ --cflags
                    OUTPUT_VARIABLE EM_CFLAGS
                    )

    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "${EM_CFLAGS}")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2-static)
if(NOT VCPKG_TARGET_TRIPLET STREQUAL "wasm32-emscripten")
    add_definitions(-DSDL_MAIN_HANDLED)

    target_link_libraries(${PROJECT_NAME} PUBLIC GLEW::GLEW-static)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC ViXeL)
target_include_directories(${PROJECT_NAME} PUBLIC ViXel ../ViXeL/../)

target_link_libraries(${PROJECT_NAME} PUBLIC glm::glm)