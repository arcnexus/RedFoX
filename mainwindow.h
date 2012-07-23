#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <login.h>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Login frmLogin1;
    QString usuario;
    QString empresa;
    
private slots:

    void on_btnCerrarSesion_clicked();

    void on_btnClientes_clicked();

    void on_btnFacturaCliente_clicked();

private:
    Ui::MainWindow *ui;
    QWorkspace *workspace;
};

#endif // MAINWINDOW_H
