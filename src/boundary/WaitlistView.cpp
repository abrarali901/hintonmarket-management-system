#include "WaitlistView.h"
#include "WaitlistController.h"
#include "Vendor.h"
#include "MarketDate.h"
#include "WaitlistEntry.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

WaitlistView::WaitlistView(Vendor* vendor, MarketDate* date, QWidget *parent)
    : QWidget(parent),
      m_dateLabel(nullptr),
      m_positionLabel(nullptr),
      m_statusLabel(nullptr),
      m_joinButton(nullptr),
      m_leaveButton(nullptr),
      m_waitlistController(new WaitlistController()),
      m_vendor(vendor),
      m_marketDate(date) {
    setupUI();
    updatePositionDisplay();
}

WaitlistView::~WaitlistView() {
    delete m_waitlistController;
}

void WaitlistView::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);

    m_dateLabel = new QLabel(this);
    if (m_marketDate) {
        m_dateLabel->setText("Waitlist for: " + m_marketDate->getDateString());
    }

    m_positionLabel = new QLabel(this);
    m_statusLabel = new QLabel(this);
    m_statusLabel->hide();

    m_joinButton = new QPushButton("Join Waitlist", this);
    m_leaveButton = new QPushButton("Leave Waitlist", this);

    layout->addWidget(m_dateLabel);
    layout->addWidget(m_positionLabel);
    layout->addWidget(m_statusLabel);
    layout->addWidget(m_joinButton);
    layout->addWidget(m_leaveButton);

    connect(m_joinButton, &QPushButton::clicked, this, &WaitlistView::onJoinClicked);
    connect(m_leaveButton, &QPushButton::clicked, this, &WaitlistView::onLeaveClicked);
}

void WaitlistView::updatePositionDisplay() {
    if (!m_vendor || !m_marketDate) return;

    int position = m_waitlistController->getPosition(m_vendor, m_marketDate);

    if (position > 0) {
        m_positionLabel->setText("Your position: " + QString::number(position));
        m_joinButton->setEnabled(false);
        m_leaveButton->setEnabled(true);
    } else {
        m_positionLabel->setText("Not on waitlist");
        m_joinButton->setEnabled(true);
        m_leaveButton->setEnabled(false);
    }
}

void WaitlistView::onJoinClicked() {
    if (!m_vendor || !m_marketDate) return;

    bool success = m_waitlistController->joinWaitlist(m_vendor, m_marketDate);

    if (success) {
        m_statusLabel->setText("Joined waitlist!");
        m_statusLabel->setStyleSheet("color: green;");
        m_statusLabel->show();
        updatePositionDisplay();
        emit waitlistJoined();
    } else {
        m_statusLabel->setText("Failed to join waitlist");
        m_statusLabel->setStyleSheet("color: red;");
        m_statusLabel->show();
    }
}

void WaitlistView::onLeaveClicked() {
    if (!m_vendor || !m_marketDate) return;

    // TODO: Find the WaitlistEntry for this vendor and date
    // TODO: Call m_waitlistController->leaveWaitlist(entry);
    // TODO: Update display and emit signal

    emit waitlistLeft();
}