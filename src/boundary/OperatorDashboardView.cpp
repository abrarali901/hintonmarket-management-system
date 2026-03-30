/**
 * @file OperatorDashboardView.cpp
 * @brief Market Operator dashboard UI (D2 feature).
 *
 * Provides vendor selection dropdown, market schedule view,
 * and action buttons for booking/cancelling/waitlist management
 * on behalf of a selected vendor.
 *
 * @author Victor
 */
#include "OperatorDashboardView.h"
#include "OperatorController.h"
#include "MarketScheduleController.h"
#include "Vendor.h"
#include "MarketDate.h"
#include "StallBooking.h"
#include "WaitlistEntry.h"
#include "ComplianceDocument.h"

#include <QLabel>
#include <QComboBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QScrollArea>

OperatorDashboardView::OperatorDashboardView(QWidget *parent)
    : QWidget(parent),
      m_vendorComboBox(nullptr),
      m_vendorInfoLabel(nullptr),
      m_scheduleTable(nullptr),
      m_bookingsTable(nullptr),
      m_waitlistTable(nullptr),
      m_bookStallButton(nullptr),
      m_cancelBookingButton(nullptr),
      m_removeWaitlistButton(nullptr),
      m_logoutButton(nullptr),
      m_statusLabel(nullptr),
      m_operatorController(new OperatorController()),
      m_scheduleController(new MarketScheduleController()),
      m_selectedVendor(nullptr) {
    setupUI();
    refreshAll();
}

OperatorDashboardView::~OperatorDashboardView() {
    delete m_operatorController;
    delete m_scheduleController;
}

void OperatorDashboardView::setupUI() {
    QVBoxLayout* outerLayout = new QVBoxLayout(this);

    // Title
    QLabel* titleLabel = new QLabel("Market Operator Dashboard", this);
    titleLabel->setStyleSheet("font-size: 22px; font-weight: bold; color: #2c5f2d; padding: 5px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    outerLayout->addWidget(titleLabel);

    QLabel* subtitleLabel = new QLabel(
        "Manage stall bookings, cancellations, and waitlists on behalf of vendors", this);
    subtitleLabel->setStyleSheet("font-size: 12px; color: #555; padding-bottom: 10px;");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    outerLayout->addWidget(subtitleLabel);

    // Scroll area
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    QWidget* scrollContent = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(scrollContent);

    // === Vendor Selection ===
    QGroupBox* vendorSelectGroup = new QGroupBox("Select Vendor", scrollContent);
    vendorSelectGroup->setStyleSheet("QGroupBox { font-weight: bold; font-size: 13px; }");
    QVBoxLayout* vendorSelectLayout = new QVBoxLayout(vendorSelectGroup);

    QLabel* selectLabel = new QLabel("Choose a vendor to act on their behalf:", vendorSelectGroup);
    m_vendorComboBox = new QComboBox(vendorSelectGroup);
    m_vendorComboBox->setMinimumHeight(30);
    m_vendorComboBox->setStyleSheet("font-size: 13px; padding: 3px;");

    // Populate vendor combo box
    QVector<Vendor*> vendors = m_operatorController->getAllVendors();
    m_vendorComboBox->addItem("-- Select a Vendor --", -1);
    for (Vendor* v : vendors) {
        QString display = v->getBusinessName() + " (" + v->getCategoryString() +
                         ") - " + v->getName();
        m_vendorComboBox->addItem(display, v->getId());
    }

    m_vendorInfoLabel = new QLabel(vendorSelectGroup);
    m_vendorInfoLabel->setStyleSheet("font-size: 12px; color: #333; padding: 5px; "
                                     "background-color: #f5f5f5; border-radius: 3px;");
    m_vendorInfoLabel->setWordWrap(true);
    m_vendorInfoLabel->hide();

    vendorSelectLayout->addWidget(selectLabel);
    vendorSelectLayout->addWidget(m_vendorComboBox);
    vendorSelectLayout->addWidget(m_vendorInfoLabel);
    layout->addWidget(vendorSelectGroup);

    // === Market Schedule ===
    QGroupBox* scheduleGroup = new QGroupBox("Market Schedule", scrollContent);
    scheduleGroup->setStyleSheet("QGroupBox { font-weight: bold; font-size: 13px; }");
    QVBoxLayout* scheduleLayout = new QVBoxLayout(scheduleGroup);

    m_scheduleTable = new QTableWidget(scheduleGroup);
    m_scheduleTable->setColumnCount(5);
    m_scheduleTable->setHorizontalHeaderLabels(
        {"Market Date", "Food Stalls", "Artisan Stalls", "Vendor Category Status", "Action"});
    m_scheduleTable->horizontalHeader()->setStretchLastSection(true);
    m_scheduleTable->setSelectionBehavior(QTableWidget::SelectRows);
    m_scheduleTable->setSelectionMode(QTableWidget::SingleSelection);
    m_scheduleTable->setEditTriggers(QTableWidget::NoEditTriggers);
    m_scheduleTable->setMinimumHeight(180);

    m_bookStallButton = new QPushButton("Book Stall for Selected Vendor", scheduleGroup);
    m_bookStallButton->setStyleSheet(
        "QPushButton { background-color: #2c5f2d; color: white; padding: 8px 20px; "
        "font-size: 13px; font-weight: bold; border-radius: 5px; }"
        "QPushButton:hover { background-color: #3a7a3d; }"
        "QPushButton:disabled { background-color: #aaa; }"
    );
    m_bookStallButton->setEnabled(false);

    scheduleLayout->addWidget(m_scheduleTable);
    scheduleLayout->addWidget(m_bookStallButton);
    layout->addWidget(scheduleGroup);

    // === Vendor's Current Bookings ===
    QGroupBox* bookingsGroup = new QGroupBox("Vendor's Active Bookings", scrollContent);
    bookingsGroup->setStyleSheet("QGroupBox { font-weight: bold; font-size: 13px; }");
    QVBoxLayout* bookingsLayout = new QVBoxLayout(bookingsGroup);

    m_bookingsTable = new QTableWidget(bookingsGroup);
    m_bookingsTable->setColumnCount(3);
    m_bookingsTable->setHorizontalHeaderLabels({"Market Date", "Booked On", "Vendor"});
    m_bookingsTable->horizontalHeader()->setStretchLastSection(true);
    m_bookingsTable->setSelectionBehavior(QTableWidget::SelectRows);
    m_bookingsTable->setSelectionMode(QTableWidget::SingleSelection);
    m_bookingsTable->setEditTriggers(QTableWidget::NoEditTriggers);
    m_bookingsTable->setMaximumHeight(130);

    m_cancelBookingButton = new QPushButton("Cancel Selected Booking", bookingsGroup);
    m_cancelBookingButton->setStyleSheet(
        "QPushButton { background-color: #c0392b; color: white; padding: 8px 20px; "
        "font-size: 13px; font-weight: bold; border-radius: 5px; }"
        "QPushButton:hover { background-color: #e74c3c; }"
        "QPushButton:disabled { background-color: #aaa; }"
    );
    m_cancelBookingButton->setEnabled(false);

    bookingsLayout->addWidget(m_bookingsTable);
    bookingsLayout->addWidget(m_cancelBookingButton);
    layout->addWidget(bookingsGroup);

    // === Vendor's Current Waitlist Entries ===
    QGroupBox* waitlistGroup = new QGroupBox("Vendor's Waitlist Positions", scrollContent);
    waitlistGroup->setStyleSheet("QGroupBox { font-weight: bold; font-size: 13px; }");
    QVBoxLayout* waitlistLayout = new QVBoxLayout(waitlistGroup);

    m_waitlistTable = new QTableWidget(waitlistGroup);
    m_waitlistTable->setColumnCount(3);
    m_waitlistTable->setHorizontalHeaderLabels({"Market Date", "Queue Position", "Vendor"});
    m_waitlistTable->horizontalHeader()->setStretchLastSection(true);
    m_waitlistTable->setSelectionBehavior(QTableWidget::SelectRows);
    m_waitlistTable->setSelectionMode(QTableWidget::SingleSelection);
    m_waitlistTable->setEditTriggers(QTableWidget::NoEditTriggers);
    m_waitlistTable->setMaximumHeight(130);

    m_removeWaitlistButton = new QPushButton("Remove from Waitlist", waitlistGroup);
    m_removeWaitlistButton->setStyleSheet(
        "QPushButton { background-color: #e67e22; color: white; padding: 8px 20px; "
        "font-size: 13px; font-weight: bold; border-radius: 5px; }"
        "QPushButton:hover { background-color: #f39c12; }"
        "QPushButton:disabled { background-color: #aaa; }"
    );
    m_removeWaitlistButton->setEnabled(false);

    waitlistLayout->addWidget(m_waitlistTable);
    waitlistLayout->addWidget(m_removeWaitlistButton);
    layout->addWidget(waitlistGroup);

    // Status label
    m_statusLabel = new QLabel(scrollContent);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet("font-size: 13px; padding: 8px; font-weight: bold;");
    m_statusLabel->hide();
    layout->addWidget(m_statusLabel);

    layout->addStretch();
    scrollArea->setWidget(scrollContent);
    outerLayout->addWidget(scrollArea);

    // === Bottom buttons ===
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    m_logoutButton = new QPushButton("Logout", this);
    m_logoutButton->setStyleSheet(
        "QPushButton { background-color: #555; color: white; padding: 8px 20px; "
        "font-size: 13px; border-radius: 5px; }"
        "QPushButton:hover { background-color: #777; }"
    );
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_logoutButton);
    outerLayout->addLayout(buttonLayout);

    // Connections
    connect(m_vendorComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &OperatorDashboardView::onVendorSelected);
    connect(m_bookStallButton, &QPushButton::clicked, this, &OperatorDashboardView::onBookStallClicked);
    connect(m_cancelBookingButton, &QPushButton::clicked, this, &OperatorDashboardView::onCancelBookingClicked);
    connect(m_removeWaitlistButton, &QPushButton::clicked, this, &OperatorDashboardView::onRemoveFromWaitlistClicked);
    connect(m_logoutButton, &QPushButton::clicked, this, &OperatorDashboardView::onLogoutClicked);
}

void OperatorDashboardView::onVendorSelected(int index) {
    m_selectedVendor = nullptr;
    m_bookStallButton->setEnabled(false);
    m_cancelBookingButton->setEnabled(false);
    m_removeWaitlistButton->setEnabled(false);

    if (index <= 0) {
        m_vendorInfoLabel->hide();
        refreshAll();
        return;
    }

    int vendorId = m_vendorComboBox->itemData(index).toInt();
    QVector<Vendor*> vendors = m_operatorController->getAllVendors();
    for (Vendor* v : vendors) {
        if (v->getId() == vendorId) {
            m_selectedVendor = v;
            break;
        }
    }

    if (m_selectedVendor) {
        m_bookStallButton->setEnabled(true);
        m_cancelBookingButton->setEnabled(true);
        m_removeWaitlistButton->setEnabled(true);
    }

    refreshAll();
}

void OperatorDashboardView::refreshAll() {
    refreshVendorInfo();
    refreshScheduleTable();
    refreshBookingsTable();
    refreshWaitlistTable();
}

void OperatorDashboardView::refreshVendorInfo() {
    if (!m_selectedVendor) {
        m_vendorInfoLabel->hide();
        return;
    }

    QString info = QString("Business: %1  |  Owner: %2  |  Category: %3\n"
                          "Email: %4  |  Phone: %5\n"
                          "Address: %6\n"
                          "Compliance: %7  |  Active Booking: %8  |  Waitlist Entries: %9")
        .arg(m_selectedVendor->getBusinessName())
        .arg(m_selectedVendor->getName())
        .arg(m_selectedVendor->getCategoryString())
        .arg(m_selectedVendor->getEmail())
        .arg(m_selectedVendor->getPhone())
        .arg(m_selectedVendor->getAddress())
        .arg(m_selectedVendor->hasAllComplianceDocuments() ? "Complete" : "INCOMPLETE")
        .arg(m_selectedVendor->getBookings().size())
        .arg(m_selectedVendor->getWaitlistEntries().size());

    m_vendorInfoLabel->setText(info);
    m_vendorInfoLabel->show();
}

void OperatorDashboardView::refreshScheduleTable() {
    m_scheduleTable->setRowCount(0);

    QVector<MarketDate*> dates = m_scheduleController->getAvailableMarketDates();

    for (MarketDate* date : dates) {
        int row = m_scheduleTable->rowCount();
        m_scheduleTable->insertRow(row);

        // Date
        m_scheduleTable->setItem(row, 0, new QTableWidgetItem(
            date->getDate().toString("dddd, yyyy-MM-dd")));

        // Food stalls
        QString foodInfo = QString("%1 / %2 available")
            .arg(date->getAvailableFoodStalls())
            .arg(date->getMaxFoodStalls());
        QTableWidgetItem* foodItem = new QTableWidgetItem(foodInfo);
        foodItem->setBackground(date->getAvailableFoodStalls() == 0 ?
            QColor(255, 200, 200) : QColor(200, 255, 200));
        m_scheduleTable->setItem(row, 1, foodItem);

        // Artisan stalls
        QString artisanInfo = QString("%1 / %2 available")
            .arg(date->getAvailableArtisanStalls())
            .arg(date->getMaxArtisanStalls());
        QTableWidgetItem* artisanItem = new QTableWidgetItem(artisanInfo);
        artisanItem->setBackground(date->getAvailableArtisanStalls() == 0 ?
            QColor(255, 200, 200) : QColor(200, 255, 200));
        m_scheduleTable->setItem(row, 2, artisanItem);

        // Vendor category status
        if (m_selectedVendor) {
            bool hasStall = m_scheduleController->hasAvailableStall(date, m_selectedVendor);
            QString statusText = hasStall ? "Available" : "Full";
            QTableWidgetItem* statusItem = new QTableWidgetItem(statusText);
            statusItem->setBackground(hasStall ? QColor(200, 255, 200) : QColor(255, 200, 200));
            m_scheduleTable->setItem(row, 3, statusItem);
        } else {
            m_scheduleTable->setItem(row, 3, new QTableWidgetItem("Select a vendor"));
        }

        // Booked vendors summary
        QString booked = "";
        for (StallBooking* b : date->getBookings()) {
            if (!booked.isEmpty()) booked += ", ";
            booked += b->getVendor()->getBusinessName();
        }
        if (booked.isEmpty()) booked = "None";
        m_scheduleTable->setItem(row, 4, new QTableWidgetItem(booked));
    }

    m_scheduleTable->resizeColumnsToContents();
}

void OperatorDashboardView::refreshBookingsTable() {
    m_bookingsTable->setRowCount(0);

    if (!m_selectedVendor) return;

    QVector<StallBooking*>& bookings = m_selectedVendor->getBookings();
    for (StallBooking* booking : bookings) {
        int row = m_bookingsTable->rowCount();
        m_bookingsTable->insertRow(row);
        m_bookingsTable->setItem(row, 0, new QTableWidgetItem(
            booking->getMarketDate()->getDateString()));
        m_bookingsTable->setItem(row, 1, new QTableWidgetItem(
            booking->getBookingTime().toString("yyyy-MM-dd hh:mm")));
        m_bookingsTable->setItem(row, 2, new QTableWidgetItem(
            m_selectedVendor->getBusinessName()));
    }
    m_bookingsTable->resizeColumnsToContents();
}

void OperatorDashboardView::refreshWaitlistTable() {
    m_waitlistTable->setRowCount(0);

    if (!m_selectedVendor) return;

    QVector<WaitlistEntry*>& entries = m_selectedVendor->getWaitlistEntries();
    for (WaitlistEntry* entry : entries) {
        int row = m_waitlistTable->rowCount();
        m_waitlistTable->insertRow(row);
        m_waitlistTable->setItem(row, 0, new QTableWidgetItem(
            entry->getMarketDate()->getDateString()));
        m_waitlistTable->setItem(row, 1, new QTableWidgetItem(
            QString::number(entry->getPosition())));
        m_waitlistTable->setItem(row, 2, new QTableWidgetItem(
            m_selectedVendor->getBusinessName()));
    }
    m_waitlistTable->resizeColumnsToContents();
}

void OperatorDashboardView::onBookStallClicked() {
    if (!m_selectedVendor) {
        QMessageBox::warning(this, "No Vendor Selected",
            "Please select a vendor from the dropdown first.");
        return;
    }

    int row = m_scheduleTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "No Date Selected",
            "Please select a market date from the schedule.");
        return;
    }

    QVector<MarketDate*> dates = m_scheduleController->getAvailableMarketDates();
    if (row >= dates.size()) return;

    MarketDate* date = dates[row];

    // Check if vendor already has a booking for THIS specific date
    for (StallBooking* existing : m_selectedVendor->getBookings()) {
        if (existing->getMarketDate() == date) {
            QMessageBox::warning(this, "Already Booked",
                m_selectedVendor->getBusinessName() +
                " already has a booking for " + date->getDateString() + ".");
            return;
        }
    }

    // Check compliance
    if (!m_selectedVendor->hasAllComplianceDocuments()) {
        QMessageBox::warning(this, "Compliance Required",
            m_selectedVendor->getBusinessName() +
            " does not have all required compliance documents.");
        return;
    }

    // Check availability
    if (!m_scheduleController->hasAvailableStall(date, m_selectedVendor)) {
        QMessageBox::warning(this, "No Availability",
            "No stalls available for " + m_selectedVendor->getCategoryString() +
            " vendors on " + date->getDateString() + ".");
        return;
    }

    // Confirm
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Booking",
        "Book a stall for " + m_selectedVendor->getBusinessName() +
        " on " + date->getDate().toString("dddd, yyyy-MM-dd") + "?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        bool success = m_operatorController->bookStallForVendor(m_selectedVendor, date);
        if (success) {
            m_statusLabel->setText("Booking confirmed for " +
                m_selectedVendor->getBusinessName() + " on " + date->getDateString());
            m_statusLabel->setStyleSheet("font-size: 13px; padding: 8px; font-weight: bold; color: green;");
            m_statusLabel->show();
            refreshAll();
        } else {
            QMessageBox::warning(this, "Booking Failed",
                "Failed to book the stall. Possible reasons:\n"
                "- Another vendor on the waitlist has priority\n"
                "- Compliance documents are incomplete\n"
                "- No stalls available for this category");
        }
    }
}

void OperatorDashboardView::onCancelBookingClicked() {
    if (!m_selectedVendor) {
        QMessageBox::warning(this, "No Vendor Selected",
            "Please select a vendor from the dropdown first.");
        return;
    }

    int row = m_bookingsTable->currentRow();
    if (row < 0 || row >= m_selectedVendor->getBookings().size()) {
        QMessageBox::warning(this, "No Booking Selected",
            "Please select a booking to cancel.");
        return;
    }

    StallBooking* booking = m_selectedVendor->getBookings()[row];

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Cancellation",
        "Cancel booking for " + m_selectedVendor->getBusinessName() +
        " on " + booking->getMarketDate()->getDateString() + "?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        bool success = m_operatorController->cancelBookingForVendor(booking);
        if (success) {
            m_statusLabel->setText("Booking cancelled for " +
                m_selectedVendor->getBusinessName());
            m_statusLabel->setStyleSheet("font-size: 13px; padding: 8px; font-weight: bold; color: #c0392b;");
            m_statusLabel->show();
            refreshAll();
        } else {
            QMessageBox::warning(this, "Error", "Failed to cancel booking.");
        }
    }
}

void OperatorDashboardView::onRemoveFromWaitlistClicked() {
    if (!m_selectedVendor) {
        QMessageBox::warning(this, "No Vendor Selected",
            "Please select a vendor from the dropdown first.");
        return;
    }

    int row = m_waitlistTable->currentRow();
    if (row < 0 || row >= m_selectedVendor->getWaitlistEntries().size()) {
        QMessageBox::warning(this, "No Waitlist Entry Selected",
            "Please select a waitlist entry to remove.");
        return;
    }

    WaitlistEntry* entry = m_selectedVendor->getWaitlistEntries()[row];

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Removal",
        "Remove " + m_selectedVendor->getBusinessName() +
        " from the waitlist for " + entry->getMarketDate()->getDateString() + "?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        bool success = m_operatorController->removeVendorFromWaitlist(entry);
        if (success) {
            m_statusLabel->setText(m_selectedVendor->getBusinessName() +
                " removed from waitlist");
            m_statusLabel->setStyleSheet("font-size: 13px; padding: 8px; font-weight: bold; color: #e67e22;");
            m_statusLabel->show();
            refreshAll();
        } else {
            QMessageBox::warning(this, "Error", "Failed to remove from waitlist.");
        }
    }
}

void OperatorDashboardView::onLogoutClicked() {
    emit logoutRequested();
}
