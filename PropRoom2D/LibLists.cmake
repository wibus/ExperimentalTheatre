# Qt
FIND_PACKAGE(Qt4 REQUIRED)
SET(QT_USE_QTOPENGL TRUE)
INCLUDE(${QT_USE_FILE})


# Global
SET(PROP2_LIBRARIES
    CellarWorkbench
    ${QT_LIBRARIES})
SET(PROP2_INCLUDE_DIR 
    ${PROP2_INCLUDE_DIR})
