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


target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2-static)
add_definitions(-DSDL_MAIN_HANDLED)

target_link_libraries(${PROJECT_NAME} PUBLIC glm::glm)

target_link_libraries(${PROJECT_NAME} PUBLIC ${GL_TARGETS})

target_link_libraries(${PROJECT_NAME} PUBLIC ViXeL)
target_include_directories(${PROJECT_NAME} PUBLIC ViXel ../ViXeL/../)


# IDE crashes when trying to use these for some reason

# file(REMOVE_RECURSE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources")

# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
