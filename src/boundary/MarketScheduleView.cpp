#include "MarketScheduleView.h"
#include "MarketScheduleController.h"
#include "MarketDate.h"
#include "Vendor.h"

#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHeaderView>

MarketScheduleView::MarketScheduleView(Vendor* vendor, QWidget *parent)
    : QWidget(parent),
      m_scheduleTable(nullptr),
      m_bookButton(nullptr),
      m_waitlistButton(nullptr),
      m_scheduleController(new MarketScheduleController()),
      m_vendor(vendor) {
    setupUI();
    refreshSchedule();
}

MarketScheduleView::~MarketScheduleView() {
    delete m_scheduleController;
}

void MarketScheduleView::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);

    // Table for market dates
    m_scheduleTable = new QTableWidget(this);
    m_scheduleTable->setColumnCount(4);
    m_scheduleTable->setHorizontalHeaderLabels({"Date", "Food Stalls", "Artisan Stalls", "Status"});
    m_scheduleTable->horizontalHeader()->setStretchLastSection(true);
    m_scheduleTable->setSelectionBehavior(QTableWidget::SelectRows);

    // Buttons
    m_bookButton = new QPushButton("Book Stall", this);
    m_waitlistButton = new QPushButton("Join Waitlist", this);

    layout->addWidget(m_scheduleTable);
    layout->addWidget(m_bookButton);
    layout->addWidget(m_waitlistButton);

    connect(m_bookButton, &QPushButton::clicked, this, &MarketScheduleView::onBookClicked);
    connect(m_waitlistButton, &QPushButton::clicked, this, &MarketScheduleView::onWaitlistClicked);
}

void MarketScheduleView::refreshSchedule() {
    m_scheduleTable->setRowCount(0);

    QVector<MarketDate*> dates = m_scheduleController->getAvailableMarketDates();

    for (MarketDate* date : dates) {
        int row = m_scheduleTable->rowCount();
        m_scheduleTable->insertRow(row);

        // TODO: Populate table cells with date info
        // - Date string
        // - Available food stalls (e.g., "1/2")
        // - Available artisan stalls (e.g., "2/2")
        // - Status (Available / Full)
    }
}

void MarketScheduleView::onBookClicked() {
    // TODO: Get selected market date from table
    // TODO: Open BookingView or call BookingController directly
}

void MarketScheduleView::onWaitlistClicked() {
    // TODO: Get selected market date from table
    // TODO: Open WaitlistView or call WaitlistController directly
}