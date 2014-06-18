#ifndef FUNCTIONS_H
#define FUNCTIONS_H

class QLabel;
#include <QString>
#include <QPixmap>
namespace RedFoX {

namespace Core {

namespace Functions {

    enum UploadMetod {
        FTP = 0, LOCAL
    };
    QPixmap getPixmap(QString url);
    void loadImage(QLabel* label, QString url);
    QString uploadImage(QString imgFile, QString folder , QString server ,
                     QString user , QString password ,int port, UploadMetod metod );
    QString uploadImage(QString imgFile, QString target_folder);

}//End Functions
}//End Core
}//End RedFoX
#endif // FUNCTIONS_H
