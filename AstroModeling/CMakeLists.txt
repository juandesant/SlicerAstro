
#-----------------------------------------------------------------------------
set(MODULE_NAME AstroModeling)
set(MODULE_TITLE "${MODULE_NAME}")

string(TOUPPER ${MODULE_NAME} MODULE_NAME_UPPER)

set(MODULE_TITLE "Astro Modeling")

#-----------------------------------------------------------------------------
add_subdirectory(Logic)

#-----------------------------------------------------------------------------
set(MODULE_EXPORT_DIRECTIVE "Q_SLICER_QTMODULES_${MODULE_NAME_UPPER}_EXPORT")

# Additional includes - Current_{source,binary} and Slicer_{Libs,Base} already included
set(MODULE_INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}/Logic
  ${CMAKE_CURRENT_BINARY_DIR}/Logic
  ${qSlicerAstroVolumeModule_INCLUDE_DIRS}
  ${qSlicerAstroVolumeModuleWidgets_INCLUDE_DIRS}
  ${qSlicerSegmentationsModuleWidgets_INCLUDE_DIRS}
  ${vtkSlicerSegmentationsModuleLogic_INCLUDES_DIRS}
  ${vtkSlicerSegmentationsModuleMRML_INCLUDES_DIRS}
  ${vtkSlicerAstroVolumeModuleLogic_INCLUDE_DIRS}
  ${WCSLIB_INCLUDE_DIR}
  )

set(MODULE_SRCS
  qSlicer${MODULE_NAME}Module.cxx
  qSlicer${MODULE_NAME}Module.h
  qSlicer${MODULE_NAME}ModuleWidget.cxx
  qSlicer${MODULE_NAME}ModuleWidget.h
  qSlicer${MODULE_NAME}ModuleWorker.cxx
  qSlicer${MODULE_NAME}ModuleWorker.h
  )

set(MODULE_MOC_SRCS
  qSlicer${MODULE_NAME}Module.h
  qSlicer${MODULE_NAME}ModuleWidget.h
  qSlicer${MODULE_NAME}ModuleWorker.h
  )

set(MODULE_UI_SRCS
  Resources/UI/qSlicer${MODULE_NAME}ModuleWidget.ui
  )

set(MODULE_TARGET_LIBRARIES
  qSlicerAstroVolumeModuleWidgets
  vtkSlicer${MODULE_NAME}ModuleLogic
  vtkSlicerAstroVolumeModuleLogic
  vtkSlicerAstroVolumeModuleMRML
  )

set(MODULE_RESOURCES
  Resources/qSlicer${MODULE_NAME}Module.qrc
  )

#-----------------------------------------------------------------------------
slicerMacroBuildLoadableModule(
  NAME ${MODULE_NAME}
  TITLE ${MODULE_TITLE}
  EXPORT_DIRECTIVE ${MODULE_EXPORT_DIRECTIVE}
  INCLUDE_DIRECTORIES ${MODULE_INCLUDE_DIRECTORIES}
  SRCS ${MODULE_SRCS}
  MOC_SRCS ${MODULE_MOC_SRCS}
  UI_SRCS ${MODULE_UI_SRCS}
  TARGET_LIBRARIES ${MODULE_TARGET_LIBRARIES}
  RESOURCES ${MODULE_RESOURCES}
  WITH_GENERIC_TESTS
  )

#-----------------------------------------------------------------------------
if(BUILD_TESTING)
  add_subdirectory(Testing)
endif()
