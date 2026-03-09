#include "MarketScheduleView.h"
#include "MarketScheduleController.h"
#include "BookingController.h"
#include "WaitlistController.h"
#include "MarketDate.h"
#include "Vendor.h"

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>

MarketScheduleView::MarketScheduleView(Vendor* vendor, QWidget *parent)
    : QWidget(parent),
      m_scheduleTable(nullptr),
      m_bookButton(nullptr),
      m_waitlistButton(nullptr),
      m_backButton(nullptr),
      m_statusLabel(nullptr),
      m_infoLabel(nullptr),
      m_scheduleController(new MarketScheduleController()),
      m_bookingController(new BookingController()),
      m_waitlistController(new WaitlistController()),
      m_vendor(vendor) {
    setupUI();
    refreshSchedule();
}

MarketScheduleView::~MarketScheduleView() {
    delete m_scheduleController;
    delete m_bookingController;
    delete m_waitlistController;
}

void MarketScheduleView::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);

    // Title
    QLabel* titleLabel = new QLabel("Available Market Dates", this);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #2c5f2d; padding: 5px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    // Info about vendor
    m_infoLabel = new QLabel(this);
    if (m_vendor) {
        m_infoLabel->setText("Logged in as: " + m_vendor->getBusinessName() +
                            " (" + m_vendor->getCategoryString() + " Vendor)");
    }
    m_infoLabel->setStyleSheet("font-size: 12px; color: #555; padding: 5px;");
    m_infoLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_infoLabel);

    // Check if vendor has existing booking
    if (m_vendor && !m_vendor->getBookings().isEmpty()) {
        QLabel* bookingWarning = new QLabel(
            "Note: You already have an active booking. Vendors may only book one stall at a time.\n"
            "Cancel your current booking before booking a new one.", this);
        bookingWarning->setStyleSheet("font-size: 12px; color: #c0392b; padding: 5px; font-weight: bold;");
        bookingWarning->setAlignment(Qt::AlignCenter);
        layout->addWidget(bookingWarning);
    }

    // Table for market dates
    m_scheduleTable = new QTableWidget(this);
    m_scheduleTable->setColumnCount(5);
    m_scheduleTable->setHorizontalHeaderLabels(
        {"Market Date", "Food Stalls", "Artisan Stalls", "Your Category Status", "Waitlist"});
    m_scheduleTable->horizontalHeader()->setStretchLastSection(true);
    m_scheduleTable->setSelectionBehavior(QTableWidget::SelectRows);
    m_scheduleTable->setSelectionMode(QTableWidget::SingleSelection);
    m_scheduleTable->setEditTriggers(QTableWidget::NoEditTriggers);
    m_scheduleTable->setMinimumHeight(200);
    layout->addWidget(m_scheduleTable);

    // Status label
    m_statusLabel = new QLabel(this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet("font-size: 12px; padding: 5px;");
    m_statusLabel->hide();
    layout->addWidget(m_statusLabel);

    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    m_bookButton = new QPushButton("Book Selected Stall", this);
    m_bookButton->setStyleSheet(
        "QPushButton { background-color: #2c5f2d; color: white; padding: 8px 20px; "
        "font-size: 13px; font-weight: bold; border-radius: 5px; }"
        "QPushButton:hover { background-color: #3a7a3d; }"
        "QPushButton:disabled { background-color: #aaa; }"
    );

    m_waitlistButton = new QPushButton("Join Waitlist for Selected Date", this);
    m_waitlistButton->setStyleSheet(
        "QPushButton { background-color: #2980b9; color: white; padding: 8px 20px; "
        "font-size: 13px; font-weight: bold; border-radius: 5px; }"
        "QPushButton:hover { background-color: #3498db; }"
        "QPushButton:disabled { background-color: #aaa; }"
    );

    m_backButton = new QPushButton("Back to Dashboard", this);
    m_backButton->setStyleSheet(
        "QPushButton { background-color: #555; color: white; padding: 8px 20px; "
        "font-size: 13px; border-radius: 5px; }"
        "QPushButton:hover { background-color: #777; }"
    );

    buttonLayout->addWidget(m_bookButton);
    buttonLayout->addWidget(m_waitlistButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_backButton);
    layout->addLayout(buttonLayout);

    connect(m_bookButton, &QPushButton::clicked, this, &MarketScheduleView::onBookClicked);
    connect(m_waitlistButton, &QPushButton::clicked, this, &MarketScheduleView::onWaitlistClicked);
    connect(m_backButton, &QPushButton::clicked, this, &MarketScheduleView::onBackClicked);
}

void MarketScheduleView::refreshSchedule() {
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
        if (date->getAvailableFoodStalls() == 0) {
            foodItem->setBackground(QColor(255, 200, 200));
        } else {
            foodItem->setBackground(QColor(200, 255, 200));
        }
        m_scheduleTable->setItem(row, 1, foodItem);

        // Artisan stalls
        QString artisanInfo = QString("%1 / %2 available")
            .arg(date->getAvailableArtisanStalls())
            .arg(date->getMaxArtisanStalls());
        QTableWidgetItem* artisanItem = new QTableWidgetItem(artisanInfo);
        if (date->getAvailableArtisanStalls() == 0) {
            artisanItem->setBackground(QColor(255, 200, 200));
        } else {
            artisanItem->setBackground(QColor(200, 255, 200));
        }
        m_scheduleTable->setItem(row, 2, artisanItem);

        // Your category status
        bool hasStall = m_scheduleController->hasAvailableStall(date, m_vendor);
        QString statusText = hasStall ? "Available" : "Full";
        QTableWidgetItem* statusItem = new QTableWidgetItem(statusText);
        statusItem->setBackground(hasStall ? QColor(200, 255, 200) : QColor(255, 200, 200));
        m_scheduleTable->setItem(row, 3, statusItem);

        // Waitlist info
        int pos = m_waitlistController->getPosition(m_vendor, date);
        QString waitlistText = (pos > 0) ? "Position: " + QString::number(pos) : "Not on waitlist";
        m_scheduleTable->setItem(row, 4, new QTableWidgetItem(waitlistText));
    }

    m_scheduleTable->resizeColumnsToContents();
}

void MarketScheduleView::onBookClicked() {
    int row = m_scheduleTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a market date to book.");
        return;
    }

    QVector<MarketDate*> dates = m_scheduleController->getAvailableMarketDates();
    if (row >= dates.size()) return;

    MarketDate* date = dates[row];

    // Check if already have a booking
    if (!m_vendor->getBookings().isEmpty()) {
        QMessageBox::warning(this, "Booking Limit",
            "You already have an active booking. Vendors may only book one stall at a time.\n"
            "Please cancel your existing booking first.");
        return;
    }

    // Check compliance
    if (!m_vendor->hasAllComplianceDocuments()) {
        QMessageBox::warning(this, "Compliance Required",
            "You must have all required compliance documents before booking.\n"
            "Please check your dashboard for document status.");
        return;
    }

    // Check availability
    if (!m_scheduleController->hasAvailableStall(date, m_vendor)) {
        QMessageBox::information(this, "No Availability",
            "No stalls are available for your category on " + date->getDateString() +
            ".\nWould you like to join the waitlist instead?");
        return;
    }

    // Confirm booking
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Booking",
        "Book a stall for " + date->getDate().toString("dddd, yyyy-MM-dd") + "?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        bool success = m_bookingController->bookStall(m_vendor, date);
        if (success) {
            QMessageBox::information(this, "Booking Confirmed",
                "Your stall has been successfully booked for " + date->getDateString() + "!");
            refreshSchedule();
        } else {
            QMessageBox::warning(this, "Booking Failed",
                "Failed to book the stall. Please check availability and compliance.");
        }
    }
}

void MarketScheduleView::onWaitlistClicked() {
    int row = m_scheduleTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a market date to join the waitlist.");
        return;
    }

    QVector<MarketDate*> dates = m_scheduleController->getAvailableMarketDates();
    if (row >= dates.size()) return;

    MarketDate* date = dates[row];

    // Check if already on waitlist
    int pos = m_waitlistController->getPosition(m_vendor, date);
    if (pos > 0) {
        QMessageBox::information(this, "Already on Waitlist",
            "You are already on the waitlist for this date at position " + QString::number(pos) + ".");
        return;
    }

    // Check if stalls are actually full (no point in waitlist if available)
    if (m_scheduleController->hasAvailableStall(date, m_vendor)) {
        QMessageBox::information(this, "Stalls Available",
            "Stalls are still available for your category on this date.\n"
            "You can book directly instead of joining the waitlist.");
        return;
    }

    // Confirm waitlist
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Join Waitlist",
        "Join the waitlist for " + date->getDate().toString("dddd, yyyy-MM-dd") + "?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        bool success = m_waitlistController->joinWaitlist(m_vendor, date);
        if (success) {
            int newPos = m_waitlistController->getPosition(m_vendor, date);
            QMessageBox::information(this, "Joined Waitlist",
                "You have been added to the waitlist at position " + QString::number(newPos) + ".");
            refreshSchedule();
        } else {
            QMessageBox::warning(this, "Waitlist Failed",
                "Failed to join the waitlist.");
        }
    }
}

void MarketScheduleView::onBackClicked() {
    emit backToDashboard();
}
