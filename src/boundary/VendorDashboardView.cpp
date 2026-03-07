#include "VendorDashboardView.h"
#include "DashboardController.h"
#include "Vendor.h"
#include "ComplianceDocument.h"
#include "StallBooking.h"
#include "WaitlistEntry.h"
#include "Notification.h"
#include "MarketDate.h"

#include <QLabel>
#include <QTableWidget>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGroupBox>

VendorDashboardView::VendorDashboardView(Vendor* vendor, QWidget *parent)
    : QWidget(parent),
      m_businessNameLabel(nullptr),
      m_categoryLabel(nullptr),
      m_contactLabel(nullptr),
      m_complianceTable(nullptr),
      m_bookingsTable(nullptr),
      m_waitlistTable(nullptr),
      m_notificationsList(nullptr),
      m_browseMarketButton(nullptr),
      m_logoutButton(nullptr),
      m_dashboardController(new DashboardController()) {
    m_dashboardController->setVendor(vendor);
    setupUI();
    refresh();
}

VendorDashboardView::~VendorDashboardView() {
    delete m_dashboardController;
}

void VendorDashboardView::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);

    // Business info section
    QGroupBox* businessGroup = new QGroupBox("Business Information", this);
    QVBoxLayout* businessLayout = new QVBoxLayout(businessGroup);
    m_businessNameLabel = new QLabel(businessGroup);
    m_categoryLabel = new QLabel(businessGroup);
    m_contactLabel = new QLabel(businessGroup);
    businessLayout->addWidget(m_businessNameLabel);
    businessLayout->addWidget(m_categoryLabel);
    businessLayout->addWidget(m_contactLabel);

    // Compliance docs table
    QGroupBox* complianceGroup = new QGroupBox("Compliance Documents", this);
    QVBoxLayout* complianceLayout = new QVBoxLayout(complianceGroup);
    m_complianceTable = new QTableWidget(complianceGroup);
    m_complianceTable->setColumnCount(3);
    m_complianceTable->setHorizontalHeaderLabels({"Type", "Number", "Expiration"});
    complianceLayout->addWidget(m_complianceTable);

    // Bookings table
    QGroupBox* bookingsGroup = new QGroupBox("Active Bookings", this);
    QVBoxLayout* bookingsLayout = new QVBoxLayout(bookingsGroup);
    m_bookingsTable = new QTableWidget(bookingsGroup);
    m_bookingsTable->setColumnCount(2);
    m_bookingsTable->setHorizontalHeaderLabels({"Market Date", "Booked On"});
    bookingsLayout->addWidget(m_bookingsTable);

    // Waitlist table
    QGroupBox* waitlistGroup = new QGroupBox("Waitlist Positions", this);
    QVBoxLayout* waitlistLayout = new QVBoxLayout(waitlistGroup);
    m_waitlistTable = new QTableWidget(waitlistGroup);
    m_waitlistTable->setColumnCount(2);
    m_waitlistTable->setHorizontalHeaderLabels({"Market Date", "Position"});
    waitlistLayout->addWidget(m_waitlistTable);

    // Notifications list
    QGroupBox* notificationsGroup = new QGroupBox("Notifications", this);
    QVBoxLayout* notificationsLayout = new QVBoxLayout(notificationsGroup);
    m_notificationsList = new QListWidget(notificationsGroup);
    notificationsLayout->addWidget(m_notificationsList);

    // Buttons
    m_browseMarketButton = new QPushButton("Browse Market Schedule", this);
    m_logoutButton = new QPushButton("Logout", this);

    layout->addWidget(businessGroup);
    layout->addWidget(complianceGroup);
    layout->addWidget(bookingsGroup);
    layout->addWidget(waitlistGroup);
    layout->addWidget(notificationsGroup);
    layout->addWidget(m_browseMarketButton);
    layout->addWidget(m_logoutButton);

    connect(m_browseMarketButton, &QPushButton::clicked, this, &VendorDashboardView::onBrowseMarketClicked);
    connect(m_logoutButton, &QPushButton::clicked, this, &VendorDashboardView::onLogoutClicked);
}

void VendorDashboardView::refresh() {
    loadBusinessInfo();
    loadComplianceDocs();
    loadBookings();
    loadWaitlistEntries();
    loadNotifications();
}

void VendorDashboardView::loadBusinessInfo() {
    Vendor* vendor = m_dashboardController->getVendor();
    if (!vendor) return;

    m_businessNameLabel->setText("Business: " + vendor->getBusinessName());
    m_categoryLabel->setText("Category: " + vendor->getCategoryString());
    m_contactLabel->setText("Contact: " + vendor->getEmail() + " | " + vendor->getPhone());
}

void VendorDashboardView::loadComplianceDocs() {
    m_complianceTable->setRowCount(0);
    QVector<ComplianceDocument*> docs = m_dashboardController->getComplianceDocuments();

    for (ComplianceDocument* doc : docs) {
        int row = m_complianceTable->rowCount();
        m_complianceTable->insertRow(row);
        // TODO: Populate table cells
    }
}

void VendorDashboardView::loadBookings() {
    m_bookingsTable->setRowCount(0);
    QVector<StallBooking*> bookings = m_dashboardController->getBookings();

    for (StallBooking* booking : bookings) {
        int row = m_bookingsTable->rowCount();
        m_bookingsTable->insertRow(row);
        // TODO: Populate table cells
    }
}

void VendorDashboardView::loadWaitlistEntries() {
    m_waitlistTable->setRowCount(0);
    QVector<WaitlistEntry*> entries = m_dashboardController->getWaitlistEntries();

    for (WaitlistEntry* entry : entries) {
        int row = m_waitlistTable->rowCount();
        m_waitlistTable->insertRow(row);
        // TODO: Populate table cells
    }
}

void VendorDashboardView::loadNotifications() {
    m_notificationsList->clear();
    QVector<Notification*> notifications = m_dashboardController->getNotifications();

    for (Notification* notification : notifications) {
        // TODO: Add notification to list
    }
}

void VendorDashboardView::onBrowseMarketClicked() {
    // TODO: Open MarketScheduleView
}

void VendorDashboardView::onLogoutClicked() {
    // TODO: Return to LoginView
}