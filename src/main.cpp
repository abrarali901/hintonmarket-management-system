/**
 * @file main.cpp
 * @brief Application entry point for HintonMarket.
 *
 * Initializes the SQLite database connection, loads all data
 * into memory, and launches the main window.
 *
 * @author Team 24
 */
#include <QApplication>
#include <QDebug>
#include "boundary/MainWindow.h"
#include "entity/DataManager.h"
#include "entity/DatabaseManager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Step 1: Connect to SQLite database
    if (!DatabaseManager::instance().initialize()) {
        qDebug() << "FATAL: Could not connect to database.";
        return 1;
    }

    // Step 2: Load all data from SQLite into in-memory objects
    // This populates users, market dates, compliance docs, bookings, etc.
    DataManager::instance().loadFromDatabase();

    // Step 3: Show the main application window
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
