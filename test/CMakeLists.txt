cmake_minimum_required(VERSION 3.12)
project(KMOD_DRV_TEST)

enable_testing()
add_compile_options(-Werror -Wall)

add_subdirectory(libdrm)
