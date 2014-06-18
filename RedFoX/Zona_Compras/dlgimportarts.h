#ifndef DLGIMPORTARTS_H
#define DLGIMPORTARTS_H

#include <QDialog>

namespace Ui {
class DlgImportArts;
}

class DlgImportArts : public QDialog
{
    Q_OBJECT

public:
    explicit DlgImportArts(QWidget *parent = 0);
    ~DlgImportArts();

private:
    Ui::DlgImportArts *ui;
};

#endif // DLGIMPORTARTS_H
