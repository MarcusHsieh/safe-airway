QT += core widgets printsupport

CONFIG += c++17

TARGET = safe-airway
TEMPLATE = app

SOURCES += \
    main.cpp \
    src/core/Application.cpp \
    src/core/CaseManager.cpp \
    src/models/Case.cpp \
    src/models/EmergencyScenario.cpp \
    src/utils/ConfigManager.cpp \
    src/utils/StyleManager.cpp \
    src/utils/SATrachTube.cpp \
    src/views/MainWindow.cpp \
    src/views/CaseSelectionView.cpp \
    src/views/TracheostomyFormView.cpp \
    src/views/NewTracheostomyFormView.cpp \
    src/views/DifficultAirwayFormView.cpp \
    src/views/LTRFormView.cpp \
    src/views/BaseDisplayView.cpp \
    src/views/TracheostomyDisplayView.cpp \
    src/views/NewTracheostomyDisplayView.cpp \
    src/widgets/BaseFormWidget.cpp \
    src/widgets/EmergencyPanel.cpp \
    src/widgets/EmergencyPanelOverlay.cpp \
    src/widgets/EscOverlayMenu.cpp \
    src/widgets/NotificationWidget.cpp \
    src/widgets/PatientInfoWidget.cpp \
    src/widgets/TextWrapDelegate.cpp \
    src/widgets/TubeSpecificationWidget.cpp

HEADERS += \
    src/core/Application.h \
    src/core/CaseManager.h \
    src/models/Case.h \
    src/models/EmergencyScenario.h \
    src/utils/ConfigManager.h \
    src/utils/StyleManager.h \
    src/utils/SATrachTube.h \
    src/views/MainWindow.h \
    src/views/CaseSelectionView.h \
    src/views/TracheostomyFormView.h \
    src/views/NewTracheostomyFormView.h \
    src/views/DifficultAirwayFormView.h \
    src/views/LTRFormView.h \
    src/views/BaseDisplayView.h \
    src/views/TracheostomyDisplayView.h \
    src/views/NewTracheostomyDisplayView.h \
    src/widgets/BaseFormWidget.h \
    src/widgets/EmergencyPanel.h \
    src/widgets/EmergencyPanelOverlay.h \
    src/widgets/EscOverlayMenu.h \
    src/widgets/NotificationWidget.h \
    src/widgets/PatientInfoWidget.h \
    src/widgets/TextWrapDelegate.h \
    src/widgets/TubeSpecificationWidget.h


RESOURCES += \
    resources.qrc

INCLUDEPATH += \
    src \
    src/core \
    src/models \
    src/utils \
    src/views \
    src/widgets

DEFINES += QT_DEPRECATED_WARNINGS

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target