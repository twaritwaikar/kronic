macro(find_subdirectories result curdir)
    file(GLOB children RELATIVE ${curdir} ${curdir}/*)
    set(dirlist "")
    
    foreach(child ${children})
        if(IS_DIRECTORY ${curdir}/${child})
            set(dirlist ${dirlist} ${child})
        endif()
    endforeach()
    
    set(${result} ${dirlist})
endmacro()

find_subdirectories(subdirs_list ${CMAKE_CURRENT_SOURCE_DIR})

foreach(subdir ${subdirs_list})
    add_subdirectory(${subdir})    
endforeach()
