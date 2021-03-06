
include(CTest)

if (CMAKE_CONFIGURATION_TYPES)
    add_custom_target(VERBOSE_RUN_TESTS COMMAND ${CMAKE_CTEST_COMMAND}
        --force-new-ctest-process --verbose
        --build-config "$<CONFIGURATION>")
else()
    add_custom_target(VERBOSE_RUN_TESTS COMMAND ${CMAKE_CTEST_COMMAND}
        --force-new-ctest-process --verbose)
endif()

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.46 COMPONENTS unit_test_framework REQUIRED)

find_package (SFML 2 COMPONENTS system REQUIRED)

macro(add_full_test testname)
	include_directories(${Boost_INCLUDE_DIRS})
	link_directories(${EXTERNAL_LIBRARIES_DIR})
	add_executable(${testname} ${testname}.cpp)
	target_link_libraries(${testname} ${Boost_LIBRARIES} ${SFML_SYSTEM_LIBRARY})

	if (WINDOWS)
		target_link_libraries(${testname} ${SFEMOVIE_LIB})
		add_dependencies(${testname} ${SFEMOVIE_LIB})

		# Copy DLLs next to the test executables
		add_custom_command(TARGET ${testname} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
			"${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}"
			$<TARGET_FILE_DIR:${testname}>)
	elseif(MACOSX)
		target_link_libraries(${testname} sfeMovie)
		set_target_properties(${SFEMOVIE_SAMPLE} PROPERTIES 
		                      BUILD_WITH_INSTALL_RPATH 1 
		                      INSTALL_RPATH "@executable_path/")

		# Copy dylibs next to the test executables
		add_custom_command(TARGET ${testname} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
			"${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}"
			$<TARGET_FILE_DIR:${testname}>)
	elseif(LINUX)
		target_link_libraries(${testname} sfeMovie)
	endif ()
	set_target_properties(${testname} PROPERTIES FOLDER "Tests")
	source_group("" FILES ${testname}.cpp)
	add_test(NAME ${testname} COMMAND ${testname})
endmacro()

# sfeMovie tests
add_full_test(TimerTest)
add_full_test(DemuxerTest)
configure_file("small_1.ogv" "small_1.ogv" COPYONLY)
configure_file("long_1.wav" "long_1.wav" COPYONLY)
configure_file("left-right.wav" "left-right.wav" COPYONLY)

configure_file("voice_1.mp3" "voice_1.mp3" COPYONLY)
configure_file("small_2.mp3" "small_2.mp3" COPYONLY)
configure_file("small_3.flac" "small_3.flac" COPYONLY)
configure_file("small_4.wav" "small_4.wav" COPYONLY)

# C++11 tests
add_full_test(RangeBasedLoop)
add_full_test(Nullptr)
add_full_test(Atomic)
add_full_test(SharedPtr)
add_full_test(MoveSemantic)
