// VendorDashboardView implementation (Osasuyi)
// Shows all vendor info: business, compliance, bookings, waitlist, notifications
#include "VendorDashboardView.h"
#include "DashboardController.h"
#include "BookingController.h"
#include "WaitlistController.h"
#include "Vendor.h"
#include "ComplianceDocument.h"
#include "StallBooking.h"
#include "WaitlistEntry.h"
#include "Notification.h"
#include "MarketDate.h"

#include <QLabel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QScrollArea>
#include <QMessageBox>

VendorDashboardView::VendorDashboardView(Vendor* vendor, QWidget *parent)
    : QWidget(parent),
      m_vendor(vendor),
      m_dashboardController(new DashboardController()),
      m_bookingController(new BookingController()),
      m_waitlistController(new WaitlistController()) {
    m_dashboardController->setVendor(vendor);
    setupUI();
    refresh();
}

VendorDashboardView::~VendorDashboardView() {
    delete m_dashboardController;
    delete m_bookingController;
    delete m_waitlistController;
}

void VendorDashboardView::setupUI() {
    QVBoxLayout* outerLayout = new QVBoxLayout(this);

    // Dashboard title
    QLabel* titleLabel = new QLabel("Vendor Status Dashboard", this);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #2c5f2d; padding: 5px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    outerLayout->addWidget(titleLabel);

    // Scroll area for content
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    QWidget* scrollContent = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(scrollContent);

    // === Business Information ===
    QGroupBox* businessGroup = new QGroupBox("Business Information", scrollContent);
    businessGroup->setStyleSheet("QGroupBox { font-weight: bold; font-size: 13px; }");
    QVBoxLayout* businessLayout = new QVBoxLayout(businessGroup);
    m_businessNameLabel = new QLabel(businessGroup);
    m_ownerNameLabel = new QLabel(businessGroup);
    m_categoryLabel = new QLabel(businessGroup);
    m_contactLabel = new QLabel(businessGroup);
    m_addressLabel = new QLabel(businessGroup);
    businessLayout->addWidget(m_businessNameLabel);
    businessLayout->addWidget(m_ownerNameLabel);
    businessLayout->addWidget(m_categoryLabel);
    businessLayout->addWidget(m_contactLabel);
    businessLayout->addWidget(m_addressLabel);

    // === Compliance Documents ===
    QGroupBox* complianceGroup = new QGroupBox("Compliance Documentation Status", scrollContent);
    complianceGroup->setStyleSheet("QGroupBox { font-weight: bold; font-size: 13px; }");
    QVBoxLayout* complianceLayout = new QVBoxLayout(complianceGroup);
    m_complianceTable = new QTableWidget(complianceGroup);
    m_complianceTable->setColumnCount(4);
    m_complianceTable->setHorizontalHeaderLabels({"Document Type", "Number", "Provider", "Expiration"});
    m_complianceTable->horizontalHeader()->setStretchLastSection(true);
    m_complianceTable->setSelectionMode(QTableWidget::NoSelection);
    m_complianceTable->setEditTriggers(QTableWidget::NoEditTriggers);
    m_complianceTable->setMaximumHeight(130);
    complianceLayout->addWidget(m_complianceTable);

    // === Active Bookings ===
    QGroupBox* bookingsGroup = new QGroupBox("Active Stall Bookings", scrollContent);
    bookingsGroup->setStyleSheet("QGroupBox { font-weight: bold; font-size: 13px; }");
    QVBoxLayout* bookingsLayout = new QVBoxLayout(bookingsGroup);
    m_bookingsTable = new QTableWidget(bookingsGroup);
    m_bookingsTable->setColumnCount(2);
    m_bookingsTable->setHorizontalHeaderLabels({"Market Date", "Booked On"});
    m_bookingsTable->horizontalHeader()->setStretchLastSection(true);
    m_bookingsTable->setSelectionBehavior(QTableWidget::SelectRows);
    m_bookingsTable->setSelectionMode(QTableWidget::SingleSelection);
    m_bookingsTable->setEditTriggers(QTableWidget::NoEditTriggers);
    m_bookingsTable->setMaximumHeight(100);
    bookingsLayout->addWidget(m_bookingsTable);

    m_cancelBookingButton = new QPushButton("Cancel Selected Booking", bookingsGroup);
    m_cancelBookingButton->setStyleSheet(
        "QPushButton { background-color: #c0392b; color: white; padding: 5px 15px; border-radius: 3px; }"
        "QPushButton:hover { background-color: #e74c3c; }"
    );
    bookingsLayout->addWidget(m_cancelBookingButton);

    // === Waitlist Positions ===
    QGroupBox* waitlistGroup = new QGroupBox("Waitlist Positions", scrollContent);
    waitlistGroup->setStyleSheet("QGroupBox { font-weight: bold; font-size: 13px; }");
    QVBoxLayout* waitlistLayout = new QVBoxLayout(waitlistGroup);
    m_waitlistTable = new QTableWidget(waitlistGroup);
    m_waitlistTable->setColumnCount(2);
    m_waitlistTable->setHorizontalHeaderLabels({"Market Date", "Queue Position"});
    m_waitlistTable->horizontalHeader()->setStretchLastSection(true);
    m_waitlistTable->setSelectionBehavior(QTableWidget::SelectRows);
    m_waitlistTable->setSelectionMode(QTableWidget::SingleSelection);
    m_waitlistTable->setEditTriggers(QTableWidget::NoEditTriggers);
    m_waitlistTable->setMaximumHeight(100);
    waitlistLayout->addWidget(m_waitlistTable);

    m_cancelWaitlistButton = new QPushButton("Leave Selected Waitlist", waitlistGroup);
    m_cancelWaitlistButton->setStyleSheet(
        "QPushButton { background-color: #e67e22; color: white; padding: 5px 15px; border-radius: 3px; }"
        "QPushButton:hover { background-color: #f39c12; }"
    );
    waitlistLayout->addWidget(m_cancelWaitlistButton);

    // === Notifications ===
    QGroupBox* notificationsGroup = new QGroupBox("System Notifications", scrollContent);
    notificationsGroup->setStyleSheet("QGroupBox { font-weight: bold; font-size: 13px; }");
    QVBoxLayout* notificationsLayout = new QVBoxLayout(notificationsGroup);
    m_notificationsList = new QListWidget(notificationsGroup);
    m_notificationsList->setMaximumHeight(120);
    notificationsLayout->addWidget(m_notificationsList);

    layout->addWidget(businessGroup);
    layout->addWidget(complianceGroup);
    layout->addWidget(bookingsGroup);
    layout->addWidget(waitlistGroup);
    layout->addWidget(notificationsGroup);
    layout->addStretch();

    scrollArea->setWidget(scrollContent);
    outerLayout->addWidget(scrollArea);

    // === Bottom buttons ===
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    m_browseMarketButton = new QPushButton("Browse Market Schedule", this);
    m_browseMarketButton->setStyleSheet(
        "QPushButton { background-color: #2c5f2d; color: white; padding: 8px 20px; "
        "font-size: 13px; font-weight: bold; border-radius: 5px; }"
        "QPushButton:hover { background-color: #3a7a3d; }"
    );
    m_logoutButton = new QPushButton("Logout", this);
    m_logoutButton->setStyleSheet(
        "QPushButton { background-color: #555; color: white; padding: 8px 20px; "
        "font-size: 13px; border-radius: 5px; }"
        "QPushButton:hover { background-color: #777; }"
    );
    buttonLayout->addWidget(m_browseMarketButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_logoutButton);
    outerLayout->addLayout(buttonLayout);

    // Connections
    connect(m_browseMarketButton, &QPushButton::clicked, this, &VendorDashboardView::onBrowseMarketClicked);
    connect(m_logoutButton, &QPushButton::clicked, this, &VendorDashboardView::onLogoutClicked);
    connect(m_cancelBookingButton, &QPushButton::clicked, this, &VendorDashboardView::onCancelBookingClicked);
    connect(m_cancelWaitlistButton, &QPushButton::clicked, this, &VendorDashboardView::onCancelWaitlistClicked);
}

void VendorDashboardView::refresh() {
    loadBusinessInfo();
    loadComplianceDocs();
    loadBookings();
    loadWaitlistEntries();
    loadNotifications();
}

void VendorDashboardView::loadBusinessInfo() {
    if (!m_vendor) return;
    m_businessNameLabel->setText("Business Name: " + m_vendor->getBusinessName());
    m_ownerNameLabel->setText("Owner: " + m_vendor->getName());
    m_categoryLabel->setText("Category: " + m_vendor->getCategoryString());
    m_contactLabel->setText("Contact: " + m_vendor->getEmail() + " | " + m_vendor->getPhone());
    m_addressLabel->setText("Address: " + m_vendor->getAddress());
}

void VendorDashboardView::loadComplianceDocs() {
    m_complianceTable->setRowCount(0);
    QVector<ComplianceDocument*>& docs = m_vendor->getComplianceDocuments();

    for (ComplianceDocument* doc : docs) {
        int row = m_complianceTable->rowCount();
        m_complianceTable->insertRow(row);
        m_complianceTable->setItem(row, 0, new QTableWidgetItem(doc->getTypeString()));
        m_complianceTable->setItem(row, 1, new QTableWidgetItem(doc->getDocumentNumber()));
        m_complianceTable->setItem(row, 2, new QTableWidgetItem(doc->getInsuranceProvider()));
        m_complianceTable->setItem(row, 3, new QTableWidgetItem(doc->getExpirationDateString()));

        // Color expired or expiring documents
        if (doc->isExpired()) {
            for (int c = 0; c < 4; c++) {
                m_complianceTable->item(row, c)->setBackground(QColor(255, 200, 200));
            }
        } else if (!doc->isValidForSeason()) {
            for (int c = 0; c < 4; c++) {
                m_complianceTable->item(row, c)->setBackground(QColor(255, 255, 200));
            }
        }
    }
    m_complianceTable->resizeColumnsToContents();
}

void VendorDashboardView::loadBookings() {
    m_bookingsTable->setRowCount(0);
    QVector<StallBooking*>& bookings = m_vendor->getBookings();

    for (StallBooking* booking : bookings) {
        int row = m_bookingsTable->rowCount();
        m_bookingsTable->insertRow(row);
        m_bookingsTable->setItem(row, 0, new QTableWidgetItem(booking->getMarketDate()->getDateString()));
        m_bookingsTable->setItem(row, 1, new QTableWidgetItem(
            booking->getBookingTime().toString("yyyy-MM-dd hh:mm")));
    }
    m_bookingsTable->resizeColumnsToContents();
}

void VendorDashboardView::loadWaitlistEntries() {
    m_waitlistTable->setRowCount(0);
    QVector<WaitlistEntry*>& entries = m_vendor->getWaitlistEntries();

    for (WaitlistEntry* entry : entries) {
        int row = m_waitlistTable->rowCount();
        m_waitlistTable->insertRow(row);
        m_waitlistTable->setItem(row, 0, new QTableWidgetItem(entry->getMarketDate()->getDateString()));
        m_waitlistTable->setItem(row, 1, new QTableWidgetItem(QString::number(entry->getPosition())));
    }
    m_waitlistTable->resizeColumnsToContents();
}

void VendorDashboardView::loadNotifications() {
    m_notificationsList->clear();
    QVector<Notification*>& notifications = m_vendor->getNotifications();

    // Show newest first
    for (int i = notifications.size() - 1; i >= 0; i--) {
        Notification* n = notifications[i];
        QString text = "[" + n->getTimestamp().toString("yyyy-MM-dd hh:mm") + "] "
                      + n->getTypeString() + ": " + n->getMessage();
        m_notificationsList->addItem(text);

        if (!n->isRead()) {
            m_notificationsList->item(m_notificationsList->count() - 1)
                ->setBackground(QColor(220, 255, 220));
            n->markAsRead();
        }
    }
}

void VendorDashboardView::onBrowseMarketClicked() {
    emit browseMarketRequested();
}

void VendorDashboardView::onLogoutClicked() {
    emit logoutRequested();
}

void VendorDashboardView::onCancelBookingClicked() {
    int row = m_bookingsTable->currentRow();
    if (row < 0 || row >= m_vendor->getBookings().size()) {
        QMessageBox::warning(this, "No Selection", "Please select a booking to cancel.");
        return;
    }

    StallBooking* booking = m_vendor->getBookings()[row];

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Cancellation",
        "Are you sure you want to cancel your booking for " +
        booking->getMarketDate()->getDateString() + "?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        bool success = m_bookingController->cancelBooking(booking);
        if (success) {
            QMessageBox::information(this, "Booking Cancelled",
                "Your booking has been successfully cancelled.");
            refresh();
        } else {
            QMessageBox::warning(this, "Error", "Failed to cancel booking.");
        }
    }
}

void VendorDashboardView::onCancelWaitlistClicked() {
    int row = m_waitlistTable->currentRow();
    if (row < 0 || row >= m_vendor->getWaitlistEntries().size()) {
        QMessageBox::warning(this, "No Selection", "Please select a waitlist entry to leave.");
        return;
    }

    WaitlistEntry* entry = m_vendor->getWaitlistEntries()[row];

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Leave Waitlist",
        "Are you sure you want to leave the waitlist for " +
        entry->getMarketDate()->getDateString() + "?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        bool success = m_waitlistController->leaveWaitlist(entry);
        if (success) {
            QMessageBox::information(this, "Left Waitlist",
                "You have been removed from the waitlist.");
            refresh();
        } else {
            QMessageBox::warning(this, "Error", "Failed to leave waitlist.");
        }
    }
}
