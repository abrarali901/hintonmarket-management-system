QT += core gui widgets

CONFIG += c++17

TARGET = HintonMarket2
TEMPLATE = app

# Include paths for subdirectories
INCLUDEPATH += src/entity \
               src/control \
               src/boundary

# Entity classes
HEADERS += \
    src/entity/User.h \
    src/entity/Vendor.h \
    src/entity/MarketOperator.h \
    src/entity/SystemAdmin.h \
    src/entity/MarketDate.h \
    src/entity/StallBooking.h \
    src/entity/WaitlistEntry.h \
    src/entity/ComplianceDocument.h \
    src/entity/Notification.h \
    src/entity/DataManager.h

SOURCES += \
    src/entity/User.cpp \
    src/entity/Vendor.cpp \
    src/entity/MarketOperator.cpp \
    src/entity/SystemAdmin.cpp \
    src/entity/MarketDate.cpp \
    src/entity/StallBooking.cpp \
    src/entity/WaitlistEntry.cpp \
    src/entity/ComplianceDocument.cpp \
    src/entity/Notification.cpp \
    src/entity/DataManager.cpp

# Control classes
HEADERS += \
    src/control/AuthenticationController.h \
    src/control/MarketScheduleController.h \
    src/control/BookingController.h \
    src/control/WaitlistController.h \
    src/control/DashboardController.h

SOURCES += \
    src/control/AuthenticationController.cpp \
    src/control/MarketScheduleController.cpp \
    src/control/BookingController.cpp \
    src/control/WaitlistController.cpp \
    src/control/DashboardController.cpp

# Boundary classes
HEADERS += \
    src/boundary/MainWindow.h \
    src/boundary/LoginView.h \
    src/boundary/MarketScheduleView.h \
    src/boundary/BookingView.h \
    src/boundary/WaitlistView.h \
    src/boundary/VendorDashboardView.h

SOURCES += \
    src/boundary/MainWindow.cpp \
    src/boundary/LoginView.cpp \
    src/boundary/MarketScheduleView.cpp \
    src/boundary/BookingView.cpp \
    src/boundary/WaitlistView.cpp \
    src/boundary/VendorDashboardView.cpp

# Main
SOURCES += src/main.cpp
