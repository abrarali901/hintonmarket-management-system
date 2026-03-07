#ifndef WAITLISTVIEW_H
#define WAITLISTVIEW_H

#include <QWidget>

class QLabel;
class QPushButton;
class WaitlistController;
class Vendor;
class MarketDate;

class WaitlistView : public QWidget {
    Q_OBJECT

public:
    explicit WaitlistView(Vendor* vendor, MarketDate* date, QWidget *parent = nullptr);
    ~WaitlistView();

signals:
    void waitlistJoined();
    void waitlistLeft();

private slots:
    void onJoinClicked();
    void onLeaveClicked();

private:
    // UI elements
    QLabel* m_dateLabel;
    QLabel* m_positionLabel;
    QLabel* m_statusLabel;
    QPushButton* m_joinButton;
    QPushButton* m_leaveButton;

    // Controller
    WaitlistController* m_waitlistController;

    // Data
    Vendor* m_vendor;
    MarketDate* m_marketDate;

    // Setup UI
    void setupUI();
    void updatePositionDisplay();
};

#endif // WAITLISTVIEW_H