#include <QApplication>
#include "boundary/MainWindow.h"
#include "entity/DataManager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Initialize default data (10 users, 4 market weeks)
    DataManager::instance().initializeDefaultData();

    // Show main window
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
