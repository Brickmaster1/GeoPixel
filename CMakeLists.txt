project(GeoPixel)

add_executable(${PROJECT_NAME}
               main.cpp
               AutoLoader.h
               EventRegistration.h
               MainGame.cpp
               Projectile.cpp
               MainGame.h
               Projectile.h
               events/WindowEvent.cpp
               events/WindowEvent.h
               events/PhysicsProcessingEvent.cpp
               events/PhysicsProcessingEvent.h
               events/PlayerMoveEvent.cpp
               events/PlayerMoveEvent.h
)

target_include_directories(${PROJECT_NAME} PUBLIC "/")


target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2-static)
add_definitions(-DSDL_MAIN_HANDLED)

target_link_libraries(${PROJECT_NAME} PUBLIC glm::glm)

target_link_libraries(${PROJECT_NAME} PUBLIC ${GL_TARGETS})

target_link_libraries(${PROJECT_NAME} PUBLIC ViXeL)
target_include_directories(${PROJECT_NAME} PUBLIC ViXel ../ViXeL/../)