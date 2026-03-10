# HintonMarket - Hintonville Farmers Market Management System

COMP 3004 - Deliverable 1
HintonMarket is a stall booking management system for the Hintonville Farmers 
Market. It supports four user roles: Vendors (Food & Artisan), Market Operators, 
and System Administrators. The system handles stall bookings, waitlist management, 
compliance tracking, and vendor notifications across a 4-week market schedule.

## Team Members
- Sheng Zhang 101311288
- Ali Abrar 101308238
- Osasuyi Uhunmwagho 101301977
- Victor Okenne 101302320 

## Build Instructions

1. Open Qt Creator 6.0.2
2. File → Open File or Project
3. Select `HintonMarket.pro`
4. Click Configure Project
5. Build → Build Project (Ctrl+B)
6. Run → Run (Ctrl+R)

## Test User Accounts

### Vendors (Food)
| Username | Name | Business |
|----------|------|----------|
| foodvendor1 | Alice Johnson | Alice's Fresh Produce |
| foodvendor2 | Bob Martinez | Bob's Bakery |
| foodvendor3 | Clara Nguyen | Clara's Jams & Preserves |
| foodvendor4 | David Park | David's Organic Juices |

### Vendors (Artisan)
| Username | Name | Business |
|----------|------|----------|
| artisanvendor1 | Emma Wilson | Emma's Handmade Pottery |
| artisanvendor2 | Frank Chen | Frank's Woodcraft |
| artisanvendor3 | Grace Lee | Grace's Jewelry Studio |
| artisanvendor4 | Henry Adams | Henry's Candles & Soaps |

### Market Operator
| Username | Name |
|----------|------|
| operator1 | Irene Thompson |

### System Administrator
| Username | Name |
|----------|------|
| admin1 | James Rivera |

## Features Implemented

- [x] Vendor Identification (Login by username)
- [x] Browse Available Market Dates (4 weeks)
- [x] Book a Stall (with compliance checking, one booking at a time)
- [x] Cancel Stall Booking (with waitlist notification trigger)
- [x] Place on Waitlist (FIFO queue per category per date)
- [x] Cancel Waitlist Placement (updates queue positions, notifies next)
- [x] View Vendor Status Dashboard (business info, compliance, bookings, waitlist, notifications)

## Business Rules Enforced

- Maximum 2 food vendors and 2 artisan vendors per market day
- Vendors can book only one market stall date at a time
- Waitlists are organized by vendor category AND by market week (FIFO)
- When a stall becomes available via cancellation, the first vendor in the waitlist queue is notified
- Compliance documents must be valid for the entire market season

## Notes

- All data is stored in memory only
- Data resets when application restarts
- Market dates are generated as the next 4 Sundays from the current date
- Food vendors have 3 compliance documents; Artisan vendors have 2
- All compliance documents are pre-loaded with valid expiration dates
