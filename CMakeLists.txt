cmake_minimum_required(VERSION 3.5.2)
project(legend_of_three)
add_definitions(-Wall -Wextra)

set(CMAKE_CXX_STANDARD 14)

set(EXECUTABLE_NAME "legend_of_three")
set(SOURCE_FILES
        main.cpp
        area.cpp area.h
        field/game_field.cpp field/game_field.h
        field/game_cell.cpp field/game_cell.h
        utiles.h
        field/cell_content.cpp
        field/cell_content.h
        utiles.cpp
        designer_t.cpp
        designer_t.h
        content_actions/i_action_content.h
        content_actions/swap_action.cpp
        content_actions/swap_action.h
        content_actions/move_action.cpp
        content_actions/move_action.h
        content_actions/create_action.cpp
        content_actions/create_action.h
        content_actions/delete_action.cpp
        content_actions/delete_action.h
        content_actions/all_content_actions.h
        controllers/i_controller.cpp
        controllers/i_controller.h
        controllers/button.cpp
        controllers/button.h
        controllers/switcher.cpp
        controllers/switcher.h
        controllers/slider.cpp
        controllers/slider.h
        controllers/grid.cpp
        controllers/grid.h
        controllers/all_controllers.h
        global_parameters.cpp
        global_parameters.h
        controllers/frame.cpp
        controllers/frame.h
        game_managing/i_state.h
        game_managing/game_engine.cpp
        game_managing/game_engine.h
        game_managing/menu.cpp
        game_managing/menu.h
        game_managing/exit_program.cpp
        game_managing/exit_program.h
        game_managing/help.cpp
        game_managing/help.h
        game_managing/levels.cpp
        game_managing/levels.h
        game_managing/settings.cpp
        game_managing/settings.h
        game_managing/play.cpp
        game_managing/play.h
        game_managing/pause.cpp
        game_managing/pause.h
        game_managing/win.cpp
        game_managing/win.h
        game_managing/game_ower.cpp
        game_managing/game_ower.h
        level.cpp
        level.h)



add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

include_directories(tinyxml2-master)
add_subdirectory(tinyxml2-master)
target_link_libraries(${EXECUTABLE_NAME} tinyxml2)

# Detect and add SFML
set(SFML_DIR "/usr/share/SFML/cmake/Modules")
set(CMAKE_MODULE_PATH "/usr/share/SFML/cmake/Modules" ${CMAKE_MODULE_PATH})
find_package(SFML REQUIRED system window graphics network audio)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif()
