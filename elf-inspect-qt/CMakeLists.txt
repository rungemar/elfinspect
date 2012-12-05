cmake_minimum_required (VERSION 2.6)

# to see the compiler output
set(CMAKE_VERBOSE_MAKEFILE ON)

project (elf-inspect-qt)

find_package ( Qt4 COMPONENTS QtGui QtCore REQUIRED )

add_definitions( ${QT4_DEFINITIONS} )


set( SRC           elf-inspect-qt.cpp
                   main.cpp )
                  

set( MOC_H         elf-inspect-qt.h
                   )
                   
SET(UIS            elf-inspect-qt.ui )

set(RCCS           elf-inspect-qt.qrc )      

QT4_WRAP_UI(UIS_H ${UIS})
QT4_ADD_RESOURCES( RCCS_SRC  ${RCCS} )

include ( ${QT_USE_FILE} )
qt4_wrap_cpp( MOC_SOURCES ${MOC_H} )

# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
# include_directories( ${ALSA_INCLUDE_DIRS} )
include_directories( ${CMAKE_CURRENT_BINARY_DIR}  ${CMAKE_SOURCE_DIR} )

link_directories( ${QT_LIB_DIR} )

add_executable( elf-inspect-qt ${SRC} ${MOC_SOURCES} ${UIS_H} ${RCCS_SRC})
target_link_libraries( elf-inspect-qt ${QT_LIBRARIES} )


install(TARGETS   elf-inspect-qt   DESTINATION  /bin)




#
# This step merges a gdb_index section into the binary to speed up debugging.
# Doing this by add_custom_command will become obsolete as soon as the linker does it. 
#
add_custom_command(TARGET elf-inspect-qt 
                   POST_BUILD  
                   COMMAND ARCH=${ARCH} /home/buildsys4/bin/mk-gdb-index elf-inspect-qt 
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "building gdb index")

