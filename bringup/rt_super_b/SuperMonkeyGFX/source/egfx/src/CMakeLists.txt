cmake_minimum_required(VERSION 3.10)

if(DEFINED EGFX_CONFIG_LOC)
  message(STATUS "eGFX config location set to ${EGFX_CONFIG_LOC}")
else()
  message(FATAL_ERROR "EGFX_CONFIG_LOC not set")
endif()



set(eGFX_SRC 

	${CMAKE_CURRENT_LIST_DIR}/Core/eGFX_Animator.c
    ${CMAKE_CURRENT_LIST_DIR}/Core/eGFX_Blit.c
    ${CMAKE_CURRENT_LIST_DIR}/Core/eGFX_PixelOps.c
    ${CMAKE_CURRENT_LIST_DIR}/Core/eGFX_Plane.c
    ${CMAKE_CURRENT_LIST_DIR}/Core/eGFX_PrimitiveOps.c
    ${CMAKE_CURRENT_LIST_DIR}/Core/eGFX_StringOps.c
                       
    ${CMAKE_CURRENT_LIST_DIR}/Core/Fonts/FONT_3_5_1BPP.c
    ${CMAKE_CURRENT_LIST_DIR}/Core/Fonts/FONT_5_7_1BPP.c
    ${CMAKE_CURRENT_LIST_DIR}/Core/Fonts/FONT_10_14_1BPP.c
    )
                        

#add_library(eGFX STATIC ${eGFX_SRC})

#target_compile_options(eGFX PUBLIC -Og -g3 -fdiagnostics-color=always -Wall)

target_sources(${TARGET} PRIVATE ${eGFX_SRC})

target_include_directories(${TARGET} PUBLIC 
                          ${CMAKE_CURRENT_LIST_DIR}
                          ${CMAKE_CURRENT_LIST_DIR}/Core 
                          ${CMAKE_CURRENT_LIST_DIR}/Core/fonts
                          ${CMAKE_CURRENT_LIST_DIR}/Assets
                          ${CMAKE_CURRENT_LIST_DIR}/Config
                          ${EGFX_CONFIG_LOC}
                          )


