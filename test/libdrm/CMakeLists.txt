function(kmodtest_drm testname)
    set(target kmodtest_drm_${testname})
    add_executable(${target} ${testname}.c)
    target_link_libraries(${target} gtest ${dep_libdrm_LIBRARIES})
    target_include_directories(${target} PRIVATE ${dep_libdrm_INCLUDE_DIRS})
    add_test(NAME ${target} COMMAND ${target})
endfunction()

find_package(PkgConfig)
pkg_search_module(dep_libdrm REQUIRED libdrm>=2.4.100)

kmodtest_drm(drmdevice)
