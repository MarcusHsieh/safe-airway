cmake_minimum_required(VERSION 3.16)
project(safe-airway VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt installation
find_package(Qt6 REQUIRED COMPONENTS Core Widgets PrintSupport)

# Enable automatic MOC, UIC and RCC processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Add include directories
include_directories(
    src
    src/core
    src/models
    src/utils
    src/views
    src/widgets
)

# Source files
set(SOURCES
    main.cpp
    src/core/Application.cpp
    src/core/CaseManager.cpp
    src/models/Case.cpp
    src/models/EmergencyScenario.cpp
    src/utils/ConfigManager.cpp
    src/utils/StyleManager.cpp
    src/views/MainWindow.cpp
    src/views/CaseSelectionView.cpp
    src/views/TracheostomyFormView.cpp
    src/views/NewTracheostomyFormView.cpp
    src/views/DifficultAirwayFormView.cpp
    src/views/LTRFormView.cpp
    src/widgets/BaseFormWidget.cpp
    src/widgets/EmergencyPanel.cpp
    src/widgets/PatientInfoWidget.cpp
    src/widgets/TextWrapDelegate.cpp
)

# Header files
set(HEADERS
    src/core/Application.h
    src/core/CaseManager.h
    src/models/Case.h
    src/models/EmergencyScenario.h
    src/utils/ConfigManager.h
    src/utils/StyleManager.h
    src/views/MainWindow.h
    src/views/CaseSelectionView.h
    src/views/TracheostomyFormView.h
    src/views/NewTracheostomyFormView.h
    src/views/DifficultAirwayFormView.h
    src/views/LTRFormView.h
    src/widgets/BaseFormWidget.h
    src/widgets/EmergencyPanel.h
    src/widgets/PatientInfoWidget.h
    src/widgets/TextWrapDelegate.h
)

# Resource files
set(RESOURCES
    resources.qrc
)

# Create executable
add_executable(safe-airway ${SOURCES} ${HEADERS} ${RESOURCES})

# Link Qt libraries
target_link_libraries(safe-airway Qt6::Core Qt6::Widgets Qt6::PrintSupport)

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(safe-airway PRIVATE -Wall -Wextra)
endif()

# Install target
install(TARGETS safe-airway
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
)