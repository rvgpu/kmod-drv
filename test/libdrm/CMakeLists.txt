function(kmodtest_drm testname)
    set(target kmodtest_drm_${testname})
    add_executable(${target} ${testname}.c)
    target_link_libraries(${target} gtest drm)
    target_include_directories(${target} PRIVATE /usr/include/drm)
    add_test(NAME ${target} COMMAND ${target})
endfunction()

kmodtest_drm(drmdevice)
