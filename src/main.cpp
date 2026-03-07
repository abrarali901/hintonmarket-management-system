#include <QApplication>
#include "boundary/LoginView.h"
#include "entity/DataManager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Initialize default data (10 users, 4 market weeks)
    DataManager::instance().initializeDefaultData();
    
    // Show login window
    LoginView loginView;
    loginView.setWindowTitle("HintonMarket");
    loginView.resize(400, 200);
    loginView.show();

    // Starts the event loop for clicks, typing, etc.
    return app.exec();
}