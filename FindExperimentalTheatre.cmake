# - Try to find ExperimentalTheatre libraries
# Once done, this will define
#
#  ExTh_FOUND - system has ExperimentalTheatre
#  ExTh_INCLUDE_DIRS - the ExperimentalTheatre include directories
#  ExTh_LIBRARIES - link these to use ExperimentalTheatre

IF(WIN32)
    IF($ENV{ProgramW6432})
        SET(EXPERIMENTAL_THEATRE_INSTALL_PREFIX $ENV{ProgramW6432}/ExperimentalTheatre)
    ELSE($ENV{ProgramW6432})
        SET(EXPERIMENTAL_THEATRE_INSTALL_PREFIX $ENV{ProgramW6432}/ExperimentalTheatre)
    ENDIF($ENV{ProgramW6432})
    LINK_DIRECTORIES(${EXPERIMENTAL_THEATRE_INSTALL_PREFIX}/lib)
ELSE(WIN32)
    SET(EXPERIMENTAL_THEATRE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
ENDIF(WIN32)


SET(ExTh_FOUND TRUE)

SET(ExTh_INCLUDE_DIRS
    ${EXPERIMENTAL_THEATRE_INSTALL_PREFIX}/include/CellarWorkbench
    ${EXPERIMENTAL_THEATRE_INSTALL_PREFIX}/include/MediaWorkbench
    ${EXPERIMENTAL_THEATRE_INSTALL_PREFIX}/include/PropRoom2D
    ${EXPERIMENTAL_THEATRE_INSTALL_PREFIX}/include/Scaena
    ${EXPERIMENTAL_THEATRE_INSTALL_PREFIX}/include/_ots)

    
SET(ExTh_LIBRARIES
    CellarWorkbench
    MediaWorkbench
    PropRoom2D
    Scaena
    GL3)