#ifndef BOOKINGCONTROLLER_H
#define BOOKINGCONTROLLER_H

class Vendor;
class MarketDate;
class StallBooking;

class BookingController {
public:
    BookingController();
    ~BookingController();

    bool bookStall(Vendor* vendor, MarketDate* date);
    bool cancelBooking(StallBooking* booking);
    bool canVendorBook(Vendor* vendor);

private:
    void notifyBookingConfirmed(Vendor* vendor, MarketDate* date);
    void notifyBookingCancelled(Vendor* vendor, MarketDate* date);
};

#endif // BOOKINGCONTROLLER_H
