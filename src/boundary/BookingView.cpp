#include "BookingView.h"
#include "BookingController.h"
#include "Vendor.h"
#include "MarketDate.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

BookingView::BookingView(Vendor* vendor, MarketDate* date, QWidget *parent)
    : QWidget(parent),
      m_dateLabel(nullptr),
      m_statusLabel(nullptr),
      m_confirmButton(nullptr),
      m_cancelButton(nullptr),
      m_bookingController(new BookingController()),
      m_vendor(vendor),
      m_marketDate(date) {
    setupUI();
}

BookingView::~BookingView() {
    delete m_bookingController;
}

void BookingView::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);

    m_dateLabel = new QLabel(this);
    if (m_marketDate) {
        m_dateLabel->setText("Book stall for: " + m_marketDate->getDateString());
    }

    m_statusLabel = new QLabel(this);
    m_statusLabel->hide();

    m_confirmButton = new QPushButton("Confirm Booking", this);
    m_cancelButton = new QPushButton("Cancel", this);

    layout->addWidget(m_dateLabel);
    layout->addWidget(m_statusLabel);
    layout->addWidget(m_confirmButton);
    layout->addWidget(m_cancelButton);

    connect(m_confirmButton, &QPushButton::clicked, this, &BookingView::onConfirmClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &BookingView::onCancelClicked);
}

void BookingView::onConfirmClicked() {
    if (!m_vendor || !m_marketDate) {
        m_statusLabel->setText("Error: Invalid vendor or date");
        m_statusLabel->setStyleSheet("color: red;");
        m_statusLabel->show();
        return;
    }

    bool success = m_bookingController->bookStall(m_vendor, m_marketDate);

    if (success) {
        m_statusLabel->setText("Booking confirmed!");
        m_statusLabel->setStyleSheet("color: green;");
        m_statusLabel->show();
        emit bookingCompleted();
    } else {
        m_statusLabel->setText("Booking failed. Check availability or compliance.");
        m_statusLabel->setStyleSheet("color: red;");
        m_statusLabel->show();
    }
}

void BookingView::onCancelClicked() {
    emit bookingCancelled();
}
