## Headers ##
# Acts
SET(SCAENA_ACT_HEADERS
    ${SCAENA_SRC_DIR}/Act/AbstractAct.h)

# Characters
SET(SCAENA_CHARACTER_HEADERS
    ${SCAENA_SRC_DIR}/Character/AbstractCharacter.h)

# Plays
SET(SCAENA_PLAY_HEADERS
    ${SCAENA_SRC_DIR}/Play/AbstractPlay.h
    ${SCAENA_SRC_DIR}/Play/SingleActPlay.h
    ${SCAENA_SRC_DIR}/Play/TrivialPlay.h)

# Application
SET(SCAENA_SCAENA_APPLICATION_HEADERS
    ${SCAENA_SRC_DIR}/ScaenaApplication/Application.h
    ${SCAENA_SRC_DIR}/ScaenaApplication/StageChooserDialog.h)

# Exceptions
SET(SCAENA_SCAENA_EXCEPTION_HEADERS
    ${SCAENA_SRC_DIR}/ScaenaException/ActNotFound.h
    ${SCAENA_SRC_DIR}/ScaenaException/CharacterNotFound.h
    ${SCAENA_SRC_DIR}/ScaenaException/PlayNotFound.h
    ${SCAENA_SRC_DIR}/ScaenaException/ScaenaException.h
    ${SCAENA_SRC_DIR}/ScaenaException/StageNotFound.h)

# Events
SET(SCAENA_STAGE_EVENT_HEADERS
    ${SCAENA_SRC_DIR}/Stage/Event/EventTypes.h
    ${SCAENA_SRC_DIR}/Stage/Event/KeyboardEvent.h
    ${SCAENA_SRC_DIR}/Stage/Event/MouseEvent.h
    ${SCAENA_SRC_DIR}/Stage/Event/StageTime.h
    ${SCAENA_SRC_DIR}/Stage/Event/SynchronousKeyboard.h
    ${SCAENA_SRC_DIR}/Stage/Event/SynchronousMouse.h)

# Managers
SET(SCAENA_STAGE_MANAGER_HEADERS
    ${SCAENA_SRC_DIR}/Stage/Manager/BeginStepCaller.h
    ${SCAENA_SRC_DIR}/Stage/Manager/DrawCaller.h
    ${SCAENA_SRC_DIR}/Stage/Manager/EndStepCaller.h
    ${SCAENA_SRC_DIR}/Stage/Manager/KeyPressDispatcher.h
    ${SCAENA_SRC_DIR}/Stage/Manager/KeyReleaseDispatcher.h
    ${SCAENA_SRC_DIR}/Stage/Manager/MouseMoveDispatcher.h
    ${SCAENA_SRC_DIR}/Stage/Manager/MousePressDispatcher.h
    ${SCAENA_SRC_DIR}/Stage/Manager/MouseReleaseDispatcher.h
    ${SCAENA_SRC_DIR}/Stage/Manager/StageManager.h)

# Stages
SET(SCAENA_STAGE_HEADERS
    ${SCAENA_SRC_DIR}/Stage/AbstractStage.h
    ${SCAENA_SRC_DIR}/Stage/QGLStage.h)

# All the header files #
SET(SCAENA_HEADERS
    ${SCAENA_ACT_HEADERS}
    ${SCAENA_CHARACTER_HEADERS}
    ${SCAENA_PLAY_HEADERS}
    ${SCAENA_SCAENA_APPLICATION_HEADERS}
    ${SCAENA_SCAENA_EXCEPTION_HEADERS}
    ${SCAENA_STAGE_EVENT_HEADERS}
    ${SCAENA_STAGE_MANAGER_HEADERS}
    ${SCAENA_STAGE_HEADERS}
    ${SCAENA_SRC_DIR}/libScaena_global.h)



## Sources ##
# Acts
SET(SCAENA_ACT_SOURCES
    ${SCAENA_SRC_DIR}/Act/AbstractAct.cpp)

# Characters
SET(SCAENA_CHARACTER_SOURCES
    ${SCAENA_SRC_DIR}/Character/AbstractCharacter.cpp)

# Plays
SET(SCAENA_PLAY_SOURCES
    ${SCAENA_SRC_DIR}/Play/AbstractPlay.cpp
    ${SCAENA_SRC_DIR}/Play/SingleActPlay.cpp
    ${SCAENA_SRC_DIR}/Play/TrivialPlay.cpp)

# Scaena application
SET(SCAENA_SCAENA_APPLICATION_SOURCES
    ${SCAENA_SRC_DIR}/ScaenaApplication/Application.cpp
    ${SCAENA_SRC_DIR}/ScaenaApplication/StageChooserDialog.cpp)

# Exceptions
SET(SCAENA_SCAENA_EXCEPTION_SOURCES
    ${SCAENA_SRC_DIR}/ScaenaException/ActNotFound.cpp
    ${SCAENA_SRC_DIR}/ScaenaException/CharacterNotFound.cpp
    ${SCAENA_SRC_DIR}/ScaenaException/PlayNotFound.cpp
    ${SCAENA_SRC_DIR}/ScaenaException/ScaenaException.cpp
    ${SCAENA_SRC_DIR}/ScaenaException/StageNotFound.cpp)

# Events
SET(SCAENA_STAGE_EVENT_SOURCES
    ${SCAENA_SRC_DIR}/Stage/Event/KeyboardEvent.cpp
    ${SCAENA_SRC_DIR}/Stage/Event/MouseEvent.cpp
    ${SCAENA_SRC_DIR}/Stage/Event/StageTime.cpp
    ${SCAENA_SRC_DIR}/Stage/Event/SynchronousKeyboard.cpp
    ${SCAENA_SRC_DIR}/Stage/Event/SynchronousMouse.cpp)

# Managers
SET(SCAENA_STAGE_MANAGER_SOURCES
    ${SCAENA_SRC_DIR}/Stage/Manager/BeginStepCaller.cpp
    ${SCAENA_SRC_DIR}/Stage/Manager/DrawCaller.cpp
    ${SCAENA_SRC_DIR}/Stage/Manager/EndStepCaller.cpp
    ${SCAENA_SRC_DIR}/Stage/Manager/KeyPressDispatcher.cpp
    ${SCAENA_SRC_DIR}/Stage/Manager/KeyReleaseDispatcher.cpp
    ${SCAENA_SRC_DIR}/Stage/Manager/MouseMoveDispatcher.cpp
    ${SCAENA_SRC_DIR}/Stage/Manager/MousePressDispatcher.cpp
    ${SCAENA_SRC_DIR}/Stage/Manager/MouseReleaseDispatcher.cpp)

# Stages
SET(SCAENA_STAGE_SOURCES
    ${SCAENA_SRC_DIR}/Stage/AbstractStage.cpp
    ${SCAENA_SRC_DIR}/Stage/QGLStage.cpp)

# All the source files #
SET(SCAENA_SOURCES
    ${SCAENA_ACT_SOURCES}
    ${SCAENA_CHARACTER_SOURCES}
    ${SCAENA_PLAY_SOURCES}
    ${SCAENA_SCAENA_APPLICATION_SOURCES}
    ${SCAENA_SCAENA_EXCEPTION_SOURCES}
    ${SCAENA_STAGE_EVENT_SOURCES}
    ${SCAENA_STAGE_MANAGER_SOURCES}
    ${SCAENA_STAGE_SOURCES})



## Global ##
SET(SCAENA_SRC_FILES
    ${SCAENA_HEADERS}
    ${SCAENA_SOURCES})

SET(PROP2_CONFIG_FILES
    ${SCAENA_SRC_DIR}/CMakeLists.txt
    ${SCAENA_SRC_DIR}/FileLists.cmake
    ${SCAENA_SRC_DIR}/LibLists.cmake)

SET(SCAENA_INCLUDE_DIR
    ${SCAENA_SRC_DIR})
SET(SCAENA_INCLUDE_DIR
    ${SCAENA_INCLUDE_DIR}
    PARENT_SCOPE)


## Source groups ##
FILE(GLOB SCAENA_HEADERS_FILTER RELATIVE ${SCAENA_HEADERS})
SOURCE_GROUP("Header Files" FILES ${SCAENA_HEADERS_FILTER})
SOURCE_GROUP("Header Files\\Act" FILES ${SCAENA_ACT_HEADERS})
SOURCE_GROUP("Header Files\\Character" FILES ${SCAENA_CHARACTER_HEADERS})
SOURCE_GROUP("Header Files\\Play" FILES ${SCAENA_PLAY_HEADERS})
SOURCE_GROUP("Header Files\\Application" FILES ${SCAENA_SCAENA_APPLICATION_HEADERS})
SOURCE_GROUP("Header Files\\Exception" FILES ${SCAENA_SCAENA_EXCEPTION_HEADERS})
SOURCE_GROUP("Header Files\\Stage" FILES ${SCAENA_STAGE_HEADERS})
SOURCE_GROUP("Header Files\\Stage\\Event" FILES ${SCAENA_STAGE_EVENT_HEADERS})
SOURCE_GROUP("Header Files\\Stage\\Manager" FILES ${SCAENA_STAGE_MANAGER_HEADERS})

FILE(GLOB SCAENA_SOURCES_FILTER RELATIVE ${SCAENA_SOURCES})
SOURCE_GROUP("Source Files" FILES ${SCAENA_SOURCES_FILTER})
SOURCE_GROUP("Source Files\\Act" FILES ${SCAENA_ACT_SOURCES})
SOURCE_GROUP("Source Files\\Character" FILES ${SCAENA_CHARACTER_SOURCES})
SOURCE_GROUP("Source Files\\Play" FILES ${SCAENA_PLAY_SOURCES})
SOURCE_GROUP("Source Files\\Application" FILES ${SCAENA_SCAENA_APPLICATION_SOURCES})
SOURCE_GROUP("Source Files\\Exception" FILES ${SCAENA_SCAENA_EXCEPTION_SOURCES})
SOURCE_GROUP("Source Files\\Stage" FILES ${SCAENA_STAGE_SOURCES})
SOURCE_GROUP("Source Files\\Stage\\Event" FILES ${SCAENA_STAGE_EVENT_SOURCES})
SOURCE_GROUP("Source Files\\Stage\\Manager" FILES ${SCAENA_STAGE_MANAGER_SOURCES})

SOURCE_GROUP("Config" FILES SCAENA_CONFIG_FILES)