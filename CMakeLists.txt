cmake_minimum_required(VERSION 3.7)

project(yourCityIsInvadedByAliens_Tomsk)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include(${CMAKE_CURRENT_SOURCE_DIR}/src.wsjcpp/CMakeLists.txt)

#### BEGIN_WSJCPP_APPEND
#### END_WSJCPP_APPEND

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${yourCityIsInvadedByAliens_Tomsk_SOURCE_DIR})

# Sources

# include header dirs
list (APPEND WSJCPP_INCLUDE_DIRS "src")
list (APPEND WSJCPP_INCLUDE_DIRS "src/ai")
list (APPEND WSJCPP_INCLUDE_DIRS "src/game")
list (APPEND WSJCPP_INCLUDE_DIRS "src/render")
list (APPEND WSJCPP_INCLUDE_DIRS "src/utils")

# source files
list (APPEND WSJCPP_SOURCES "src/main.cpp")
list (APPEND WSJCPP_SOURCES "src/main_controller.h")
list (APPEND WSJCPP_SOURCES "src/main_controller.cpp")
list (APPEND WSJCPP_SOURCES "src/game/coordxy.cpp")
list (APPEND WSJCPP_SOURCES "src/game/coordxy.h")
list (APPEND WSJCPP_SOURCES "src/game/game_alien_ship_state.h")
list (APPEND WSJCPP_SOURCES "src/game/game_alien_ship_state.cpp")
list (APPEND WSJCPP_SOURCES "src/game/game_bioplast_state.h")
list (APPEND WSJCPP_SOURCES "src/game/game_bioplast_state.cpp")
list (APPEND WSJCPP_SOURCES "src/game/game_rocket_state.h")
list (APPEND WSJCPP_SOURCES "src/game/game_rocket_state.cpp")
list (APPEND WSJCPP_SOURCES "src/game/game_building.h")
list (APPEND WSJCPP_SOURCES "src/game/game_building.cpp")
list (APPEND WSJCPP_SOURCES "src/game/game_state.h")
list (APPEND WSJCPP_SOURCES "src/game/game_state.cpp")
list (APPEND WSJCPP_SOURCES "src/game/game_tank0_state.h")
list (APPEND WSJCPP_SOURCES "src/game/game_tank0_state.cpp")
list (APPEND WSJCPP_SOURCES "src/game/move_object_direction.h")
list (APPEND WSJCPP_SOURCES "src/game/move_object_direction.cpp")
list (APPEND WSJCPP_SOURCES "src/ai/ai_object.h")
list (APPEND WSJCPP_SOURCES "src/ai/ai_object.cpp")
list (APPEND WSJCPP_SOURCES "src/ai/ai_tank0.h")
list (APPEND WSJCPP_SOURCES "src/ai/ai_tank0.cpp")
list (APPEND WSJCPP_SOURCES "src/ai/main_ai_thread.h")
list (APPEND WSJCPP_SOURCES "src/ai/main_ai_thread.cpp")
list (APPEND WSJCPP_SOURCES "src/render/render.h")
list (APPEND WSJCPP_SOURCES "src/render/render.cpp")
list (APPEND WSJCPP_SOURCES "src/render/render_base.h")
list (APPEND WSJCPP_SOURCES "src/render/render_base.cpp")
list (APPEND WSJCPP_SOURCES "src/render/render_bioplast.h")
list (APPEND WSJCPP_SOURCES "src/render/render_bioplast.cpp")
list (APPEND WSJCPP_SOURCES "src/render/render_button_music_on_off.h")
list (APPEND WSJCPP_SOURCES "src/render/render_button_music_on_off.cpp")
list (APPEND WSJCPP_SOURCES "src/render/render_button_simple.h")
list (APPEND WSJCPP_SOURCES "src/render/render_button_simple.cpp")
list (APPEND WSJCPP_SOURCES "src/render/render_dialog.h")
list (APPEND WSJCPP_SOURCES "src/render/render_dialog.cpp")
list (APPEND WSJCPP_SOURCES "src/render/render_left_panel_info.h")
list (APPEND WSJCPP_SOURCES "src/render/render_left_panel_info.cpp")
list (APPEND WSJCPP_SOURCES "src/render/render_player_alient_ship.h")
list (APPEND WSJCPP_SOURCES "src/render/render_player_alient_ship.cpp")
list (APPEND WSJCPP_SOURCES "src/render/render_rocket.h")
list (APPEND WSJCPP_SOURCES "src/render/render_rocket.cpp")
list (APPEND WSJCPP_SOURCES "src/render/render_tank0.h")
list (APPEND WSJCPP_SOURCES "src/render/render_tank0.cpp")
list (APPEND WSJCPP_SOURCES "src/render/render_window.h")
list (APPEND WSJCPP_SOURCES "src/render/render_window.cpp")
list (APPEND WSJCPP_SOURCES "src/utils/utils_loader_screen.h")
list (APPEND WSJCPP_SOURCES "src/utils/utils_loader_screen.cpp")
list (APPEND WSJCPP_SOURCES "src/utils/utils_music_player.h")
list (APPEND WSJCPP_SOURCES "src/utils/utils_music_player.cpp")
list (APPEND WSJCPP_SOURCES "src/utils/utils_start_dialog.h")
list (APPEND WSJCPP_SOURCES "src/utils/utils_start_dialog.cpp")

############################
# SDL2
find_package(SDL2 REQUIRED)
if ( SDL2_FOUND )
    list (APPEND WSJCPP_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS})
    list (APPEND WSJCPP_LIBRARIES ${SDL2_LIBRARIES})
endif( SDL2_FOUND )
############################


############################
# SDL2_image
find_package(SDL2_image REQUIRED)
if ( SDL2_image_FOUND )
    list (APPEND WSJCPP_INCLUDE_DIRS ${SDL2_IMAGE_INCLUDE_DIRS})
    list (APPEND WSJCPP_LIBRARIES ${SDL2_IMAGE_LIBRARIES})
endif( SDL2_image_FOUND )
############################

############################
# SDL2_ttf
find_package(SDL2TTF REQUIRED)
if ( SDL2TTF_FOUND )
    list (APPEND WSJCPP_INCLUDE_DIRS ${SDL2TTF_INCLUDE_DIRS})
    list (APPEND WSJCPP_LIBRARIES ${SDL2TTF_LIBRARY})
endif( SDL2TTF_FOUND )
############################

############################
# SDL2_mixer
find_package(SDL2_mixer REQUIRED)
if ( SDL2_MIXER_FOUND )
    list (APPEND WSJCPP_INCLUDE_DIRS ${SDL2_MIXER_INCLUDE_DIRS})
    list (APPEND WSJCPP_LIBRARIES ${SDL2_MIXER_LIBRARY})
endif( SDL2_MIXER_FOUND )
############################

# include(FindPkgConfig)
# pkg_check_modules(SDL2_Mixer REQUIRED IMPORTED_TARGET SDL2_mixer)
# list (APPEND WSJCPP_LIBRARIES PkgConfig::SDL2_Mixer)

include_directories(${WSJCPP_INCLUDE_DIRS})

add_executable (yourCityIsInvadedByAliens_Tomsk ${WSJCPP_SOURCES})

target_link_libraries(yourCityIsInvadedByAliens_Tomsk ${WSJCPP_LIBRARIES} )
