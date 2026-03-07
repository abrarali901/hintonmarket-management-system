#ifndef BOOKINGVIEW_H
#define BOOKINGVIEW_H

#include <QWidget>

class QLabel;
class QPushButton;
class BookingController;
class Vendor;
class MarketDate;

class BookingView : public QWidget {
    Q_OBJECT

public:
    explicit BookingView(Vendor* vendor, MarketDate* date, QWidget *parent = nullptr);
    ~BookingView();

signals:
    void bookingCompleted();
    void bookingCancelled();

private slots:
    void onConfirmClicked();
    void onCancelClicked();

private:
    // UI elements
    QLabel* m_dateLabel;
    QLabel* m_statusLabel;
    QPushButton* m_confirmButton;
    QPushButton* m_cancelButton;

    // Controller
    BookingController* m_bookingController;

    // Data
    Vendor* m_vendor;
    MarketDate* m_marketDate;

    // Setup UI
    void setupUI();
};

#endif // BOOKINGVIEW_H