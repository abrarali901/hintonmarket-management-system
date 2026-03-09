#ifndef MARKETSCHEDULEVIEW_H
#define MARKETSCHEDULEVIEW_H

#include <QWidget>

class QTableWidget;
class QPushButton;
class QLabel;
class MarketScheduleController;
class BookingController;
class WaitlistController;
class Vendor;

class MarketScheduleView : public QWidget {
    Q_OBJECT

public:
    explicit MarketScheduleView(Vendor* vendor, QWidget *parent = nullptr);
    ~MarketScheduleView();

    void refreshSchedule();

signals:
    void backToDashboard();

private slots:
    void onBookClicked();
    void onWaitlistClicked();
    void onBackClicked();

private:
    QTableWidget* m_scheduleTable;
    QPushButton* m_bookButton;
    QPushButton* m_waitlistButton;
    QPushButton* m_backButton;
    QLabel* m_statusLabel;
    QLabel* m_infoLabel;

    MarketScheduleController* m_scheduleController;
    BookingController* m_bookingController;
    WaitlistController* m_waitlistController;

    Vendor* m_vendor;

    void setupUI();
};

#endif // MARKETSCHEDULEVIEW_H
