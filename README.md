# HintonMarket - Hintonville Farmers Market Management System

COMP 3004 - Deliverable 2
Team 24

HintonMarket is a stall booking management system for the Hintonville Farmers
Market. It supports four user roles: Vendors (Food & Artisan), Market Operators,
and System Administrators. The system handles stall bookings, waitlist management,
compliance tracking, vendor notifications, and persistent data storage via SQLite
across a 4-week market schedule.

## Team Members
- Sheng Zhang 101311288
- Ali Abrar 101308238
- Osasuyi Uhunmwagho 101301977
- Victor Okenne 101302320

## Build Instructions
1. Open Qt Creator 6.0.2 on the course VM
2. File → Open File or Project
3. Select `HintonMarket.pro`
4. Click Configure Project
5. Build → Build Project (Ctrl+B)
6. Run → Run (Ctrl+R)

**Database Setup:** The `.pro` file includes an auto-copy rule that copies
`hintonMarket.sqlite3` from the source directory to the build directory on first
build. No manual steps required.

**If auto-copy does not work:** Manually copy `hintonMarket.sqlite3` from the
project source folder to the build output directory (the folder where the
executable is generated, typically `build-HintonMarket-Desktop-Debug/`).

**Important:** If you encounter build errors after the first build, use
Build → Rebuild (not just Build) to clear stale object files.

## Architecture
ECB (Entity-Control-Boundary) pattern with a Repository architectural style,
where DataManager is the centralized in-memory data store and DatabaseManager
is the SQLite persistence layer (the repository).

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

### Carry Forward from D1 (Vendor Features)
- [x] Vendor Identification (Login by username)
- [x] Browse Available Market Dates (4 weeks)
- [x] Book a Stall (with compliance checking, one booking at a time)
- [x] Cancel Stall Booking (with waitlist notification trigger)
- [x] Place on Waitlist (FIFO queue per category per date)
- [x] Cancel Waitlist Placement (updates queue positions, notifies next)
- [x] View Vendor Status Dashboard (business info, compliance, bookings, waitlist, notifications)

### New D2 Features
- [x] SQLite persistent data storage (all data stored in objects AND in database)
- [x] Object-Relational Mapping (database queries create corresponding objects)
- [x] Data persists between program executions
- [x] Market Operator: Book a stall on behalf of a vendor
- [x] Market Operator: Cancel a stall booking on a vendor's behalf
- [x] Market Operator: Remove a vendor from a waitlist
- [x] All operator changes reflected in vendor dashboard on vendor login

### Business Rules Enforced
- Maximum 2 food vendors and 2 artisan vendors per market day
- Vendors can book only one market stall date at a time
- Waitlists are organized by vendor category AND by market week (FIFO)
- When a stall becomes available via cancellation, the first vendor in the waitlist queue is notified
- Compliance documents must be valid for the entire market season

### Excluded from Prototype
- No stall payment processing
- No booking deadline enforcement (Wednesday 11:59 PM booking window)
- No cancellation deadline enforcement (Wednesday 12:00 PM cancellation deadline)
- No refund processing

## Database Schema
- **users** - All user accounts (vendors, operator, admin) with business info
- **market_dates** - Market schedule with stall capacity
- **compliance_documents** - Vendor compliance docs with expiration dates
- **bookings** - Active stall bookings linking vendors to market dates
- **waitlist_entries** - Waitlist queue entries with FIFO positions
- **notifications** - Vendor notifications (booking confirmations, waitlist alerts)

## Notes
- Database file: `hintonMarket.sqlite3` (pre-populated with default data)
- Market dates are set to the first 4 Sundays of May 2026 (within market season)
- Food vendors have 3 compliance documents; Artisan vendors have 2
- All compliance documents are pre-loaded with valid expiration dates
