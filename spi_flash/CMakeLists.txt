# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

list(APPEND BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../)
set(BOARD rt_super)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(spi_flash)


FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})
