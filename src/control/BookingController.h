#ifndef BOOKINGCONTROLLER_H
#define BOOKINGCONTROLLER_H

class Vendor;
class MarketDate;
class StallBooking;

class BookingController {
public:
    BookingController();
    ~BookingController();

    // Book a stall - returns true if successful
    bool bookStall(Vendor* vendor, MarketDate* date);

    // Cancel a booking - returns true if successful
    bool cancelBooking(StallBooking* booking);

    // Check if vendor can book (has compliance docs, etc.)
    bool canVendorBook(Vendor* vendor);

private:
    // Helper to create notification for vendor
    void notifyBookingConfirmed(Vendor* vendor, MarketDate* date);
    void notifyBookingCancelled(Vendor* vendor, MarketDate* date);
};

#endif // BOOKINGCONTROLLER_H