#ifndef FRMEDITLINE_H
#define FRMEDITLINE_H

#include <QDialog>

namespace Ui {
class frmEditLine;
}

class frmEditLine : public QDialog
{
    Q_OBJECT

public:
    explicit frmEditLine(QWidget *parent = 0);
    ~frmEditLine();

private:
    Ui::frmEditLine *ui;
};

#endif // FRMEDITLINE_H
