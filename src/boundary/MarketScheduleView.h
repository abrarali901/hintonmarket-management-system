#ifndef MARKETSCHEDULEVIEW_H
#define MARKETSCHEDULEVIEW_H

#include <QWidget>

class QTableWidget;
class QPushButton;
class MarketScheduleController;
class Vendor;

class MarketScheduleView : public QWidget {
    Q_OBJECT

public:
    explicit MarketScheduleView(Vendor* vendor, QWidget *parent = nullptr);
    ~MarketScheduleView();

    void refreshSchedule();

private slots:
    void onBookClicked();
    void onWaitlistClicked();

private:
    // UI elements
    QTableWidget* m_scheduleTable;
    QPushButton* m_bookButton;
    QPushButton* m_waitlistButton;

    // Controller
    MarketScheduleController* m_scheduleController;

    // Current vendor
    Vendor* m_vendor;

    // Setup UI
    void setupUI();
};

#endif // MARKETSCHEDULEVIEW_H