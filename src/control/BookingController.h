#ifndef BOOKINGCONTROLLER_H
#define BOOKINGCONTROLLER_H

class Vendor;
class MarketDate;
class StallBooking;

/**
 * @class BookingController
 * @brief Manages stall booking and cancellation logic.
 *
 * Enforces business rules: compliance check, one-booking-at-a-time,
 * category capacity limits. On cancellation, triggers waitlist notification.
 * D2: persists all changes to SQLite via DatabaseManager.
 *
 * @author Ali, Victor
 */
class BookingController {
public:
    BookingController();
    ~BookingController();

    bool bookStall(Vendor* vendor, MarketDate* date);
    bool cancelBooking(StallBooking* booking);
    bool canVendorBook(Vendor* vendor);  // checks compliance docs

private:
    void notifyBookingConfirmed(Vendor* vendor, MarketDate* date);
    void notifyBookingCancelled(Vendor* vendor, MarketDate* date);
};

#endif // BOOKINGCONTROLLER_H
