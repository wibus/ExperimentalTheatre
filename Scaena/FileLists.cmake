## Headers ##
# Plays
SET(SCAENA_PLAY_HEADERS
    ${SCAENA_SRC_DIR}/Play/Act.h
    ${SCAENA_SRC_DIR}/Play/Character.h
    ${SCAENA_SRC_DIR}/Play/Play.h
    ${SCAENA_SRC_DIR}/Play/View.h)

# Application
SET(SCAENA_SCAENA_APPLICATION_HEADERS
    ${SCAENA_SRC_DIR}/ScaenaApplication/Application.h
    ${SCAENA_SRC_DIR}/ScaenaApplication/QGlWidgetView.h)

# Events
SET(SCAENA_STAGEMANAGEMENT_EVENT_HEADERS
    ${SCAENA_SRC_DIR}/StageManagement/Event/EventTypes.h
    ${SCAENA_SRC_DIR}/StageManagement/Event/KeyboardEvent.h
    ${SCAENA_SRC_DIR}/StageManagement/Event/MouseEvent.h
    ${SCAENA_SRC_DIR}/StageManagement/Event/StageTime.h
    ${SCAENA_SRC_DIR}/StageManagement/Event/SynchronousKeyboard.h
    ${SCAENA_SRC_DIR}/StageManagement/Event/SynchronousMouse.h)

# Managers
SET(SCAENA_STAGEMANAGEMENT_MANAGER_HEADERS
    ${SCAENA_SRC_DIR}/StageManagement/Manager/BeginStepCaller.h
    ${SCAENA_SRC_DIR}/StageManagement/Manager/DrawCaller.h
    ${SCAENA_SRC_DIR}/StageManagement/Manager/EndStepCaller.h
    ${SCAENA_SRC_DIR}/StageManagement/Manager/KeyPressDispatcher.h
    ${SCAENA_SRC_DIR}/StageManagement/Manager/KeyReleaseDispatcher.h
    ${SCAENA_SRC_DIR}/StageManagement/Manager/MouseMoveDispatcher.h
    ${SCAENA_SRC_DIR}/StageManagement/Manager/MousePressDispatcher.h
    ${SCAENA_SRC_DIR}/StageManagement/Manager/MouseReleaseDispatcher.h
    ${SCAENA_SRC_DIR}/StageManagement/Manager/StageManager.h)

# Stage Management
SET(SCAENA_STAGEMANAGEMENT_HEADERS)

# All the header files #
SET(SCAENA_HEADERS
    ${SCAENA_PLAY_HEADERS}
    ${SCAENA_SCAENA_APPLICATION_HEADERS}
    ${SCAENA_STAGEMANAGEMENT_EVENT_HEADERS}
    ${SCAENA_STAGEMANAGEMENT_MANAGER_HEADERS}
    ${SCAENA_STAGEMANAGEMENT_HEADERS}
    ${SCAENA_SRC_DIR}/libScaena_global.h)



## Sources ##
# Plays
SET(SCAENA_PLAY_SOURCES
    ${SCAENA_SRC_DIR}/Play/Act.cpp
    ${SCAENA_SRC_DIR}/Play/Character.cpp
    ${SCAENA_SRC_DIR}/Play/Play.cpp
    ${SCAENA_SRC_DIR}/Play/View.cpp)

# Scaena application
SET(SCAENA_SCAENA_APPLICATION_SOURCES
    ${SCAENA_SRC_DIR}/ScaenaApplication/Application.cpp
    ${SCAENA_SRC_DIR}/ScaenaApplication/QGlWidgetView.cpp)

# Events
SET(SCAENA_STAGEMANAGEMENT_EVENT_SOURCES
    ${SCAENA_SRC_DIR}/StageManagement/Event/KeyboardEvent.cpp
    ${SCAENA_SRC_DIR}/StageManagement/Event/MouseEvent.cpp
    ${SCAENA_SRC_DIR}/StageManagement/Event/StageTime.cpp
    ${SCAENA_SRC_DIR}/StageManagement/Event/SynchronousKeyboard.cpp
    ${SCAENA_SRC_DIR}/StageManagement/Event/SynchronousMouse.cpp)

# Managers
SET(SCAENA_STAGEMANAGEMENT_MANAGER_SOURCES
    ${SCAENA_SRC_DIR}/StageManagement/Manager/BeginStepCaller.cpp
    ${SCAENA_SRC_DIR}/StageManagement/Manager/DrawCaller.cpp
    ${SCAENA_SRC_DIR}/StageManagement/Manager/EndStepCaller.cpp
    ${SCAENA_SRC_DIR}/StageManagement/Manager/KeyPressDispatcher.cpp
    ${SCAENA_SRC_DIR}/StageManagement/Manager/KeyReleaseDispatcher.cpp
    ${SCAENA_SRC_DIR}/StageManagement/Manager/MouseMoveDispatcher.cpp
    ${SCAENA_SRC_DIR}/StageManagement/Manager/MousePressDispatcher.cpp
    ${SCAENA_SRC_DIR}/StageManagement/Manager/MouseReleaseDispatcher.cpp)

# Stages
SET(SCAENA_STAGE_SOURCES)

# All the source files #
SET(SCAENA_SOURCES
    ${SCAENA_CHARACTER_SOURCES}
    ${SCAENA_PLAY_SOURCES}
    ${SCAENA_SCAENA_APPLICATION_SOURCES}
    ${SCAENA_STAGEMANAGEMENT_EVENT_SOURCES}
    ${SCAENA_STAGEMANAGEMENT_MANAGER_SOURCES}
    ${SCAENA_STAGEMANAGEMENT_SOURCES})



## Global ##
SET(SCAENA_CONFIG_FILES
    ${SCAENA_SRC_DIR}/CMakeLists.txt
    ${SCAENA_SRC_DIR}/FileLists.cmake
    ${SCAENA_SRC_DIR}/LibLists.cmake)
	
SET(SCAENA_SRC_FILES
    ${SCAENA_HEADERS}
    ${SCAENA_SOURCES}
    ${SCAENA_CONFIG_FILES}
    ${SCAENA_MOC_CPP_FILES})



## Source groups ##
SOURCE_GROUP("Header Files" FILES ${SCAENA_HEADERS_FILTER})
SOURCE_GROUP("Header Files\\Play" FILES ${SCAENA_PLAY_HEADERS})
SOURCE_GROUP("Header Files\\Application" FILES ${SCAENA_SCAENA_APPLICATION_HEADERS})
SOURCE_GROUP("Header Files\\Exception" FILES ${SCAENA_SCAENA_EXCEPTION_HEADERS})
SOURCE_GROUP("Header Files\\Stage" FILES ${SCAENA_STAGE_HEADERS})
SOURCE_GROUP("Header Files\\Stage\\Event" FILES ${SCAENA_STAGE_EVENT_HEADERS})
SOURCE_GROUP("Header Files\\Stage\\Manager" FILES ${SCAENA_STAGE_MANAGER_HEADERS})

SOURCE_GROUP("Source Files" FILES ${SCAENA_SOURCES_FILTER})
SOURCE_GROUP("Source Files\\Play" FILES ${SCAENA_PLAY_SOURCES})
SOURCE_GROUP("Source Files\\Application" FILES ${SCAENA_SCAENA_APPLICATION_SOURCES})
SOURCE_GROUP("Source Files\\Exception" FILES ${SCAENA_SCAENA_EXCEPTION_SOURCES})
SOURCE_GROUP("Source Files\\Stage" FILES ${SCAENA_STAGE_SOURCES})
SOURCE_GROUP("Source Files\\Stage\\Event" FILES ${SCAENA_STAGE_EVENT_SOURCES})
SOURCE_GROUP("Source Files\\Stage\\Manager" FILES ${SCAENA_STAGE_MANAGER_SOURCES})

SOURCE_GROUP("Config" FILES ${SCAENA_CONFIG_FILES})

SOURCE_GROUP("Mocked Files" FILES ${SCAENA_MOC_CPP_FILES})
