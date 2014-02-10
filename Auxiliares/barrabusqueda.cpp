#include "barrabusqueda.h"
#include "ui_barrabusqueda.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QTextEdit>
#include <QDateEdit>
#include <QDebug>
BarraBusqueda::BarraBusqueda(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BarraBusqueda)
{
    ui->setupUi(this);
    m_Cache = 0;

    this->setAutoFillBackground(true);
    this->setMouseTracking(true);

    connect(ui->cboModo,SIGNAL(currentIndexChanged(QString)),this,SLOT(textChanged(QString)));
    connect(ui->txtBuscar,SIGNAL(textEdited(QString)),this,SLOT(textChanged(QString)));
    connect(ui->cboOrden,SIGNAL(currentIndexChanged(QString)),this,SLOT(textChanged(QString)));
    ui->txtBuscar->installEventFilter(this);
    m_show = false;
}

BarraBusqueda::~BarraBusqueda()
{
    delete ui;
    if(m_Cache)
      delete m_Cache;
}

void BarraBusqueda::setOrderCombo(QStringList l)
{
    ui->cboOrden->clear();
    ui->cboOrden->addItems(l);
}

void BarraBusqueda::setModeCombo(QStringList l)
{
    ui->cboModo->clear();
    ui->cboModo->addItems(l);
}

void BarraBusqueda::addWidget(QWidget *w)
{
    ui->layout->addWidget(w);
}

void BarraBusqueda::addLayoutZ1(QLayout *z)
{
    ui->zona1->setLayout(z);
}

void BarraBusqueda::addSpacer()
{
    ui->layout->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Preferred,QSizePolicy::Expanding));
}

void BarraBusqueda::addLayout(QLayout *l)
{
    ui->layout->addLayout(l);
}

void BarraBusqueda::block(bool state)
{
    QList<QLineEdit *> lineEditList = this->findChildren<QLineEdit *>();
    QLineEdit *lineEdit;
    foreach (lineEdit, lineEditList) {
        lineEdit->setReadOnly(state);
    }
    // ComboBox
    QList<QComboBox *> ComboBoxList = this->findChildren<QComboBox *>();
    QComboBox *ComboBox;
    foreach (ComboBox, ComboBoxList) {
        ComboBox->setEnabled(!state);
    }

    QList<QDoubleSpinBox *> DSpinBoxList = this->findChildren<QDoubleSpinBox *>();
    QDoubleSpinBox *DSpinBox;
    foreach (DSpinBox, DSpinBoxList) {
        DSpinBox->setReadOnly(state);
    }
    // CheckBox
    QList<QCheckBox *> CheckBoxList = this->findChildren<QCheckBox *>();
    QCheckBox *CheckBox;
    foreach (CheckBox, CheckBoxList) {
        CheckBox->setEnabled(!state);
    }
    // QTextEdit
    QList<QTextEdit *> TextEditList = this->findChildren<QTextEdit *>();
    QTextEdit *TextEdit;
    foreach (TextEdit,TextEditList) {
        TextEdit->setReadOnly(state);
    }
    // QDateEdit
    QList<QDateEdit *> DateEditList = this->findChildren<QDateEdit *>();
    QDateEdit *DateEdit;
    foreach (DateEdit, DateEditList) {
        DateEdit->setEnabled(!state);
    }
    // QPushButton
    QList<QPushButton *> btnList = this->findChildren<QPushButton *>();
    QPushButton *btn;
    foreach (btn, btnList) {
        btn->setEnabled(!state);
    }
    this->blockSignals(state);
}

void BarraBusqueda::doFocustoText()
{
    ui->txtBuscar->setFocus();
}

bool BarraBusqueda::isShow()
{
    return m_show;
}

void BarraBusqueda::setShow(bool b)
{
    m_show = b;
}

bool BarraBusqueda::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(obj == ui->txtBuscar){
            if(keyEvent->key() == Qt::Key_Down)
                emit key_Down_Pressed();

        }
    }
    return false;
}

void BarraBusqueda::paintEvent(QPaintEvent *pe)
{
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
    if(m_Cache != 0)
    {
      QPainter painter(this);

      painter.drawPixmap(0, 0, *m_Cache);
    }
}

void BarraBusqueda::on_btnLimpiar_clicked()
{
    ui->txtBuscar->clear();
    ui->cboModo->setCurrentIndex(0);
    ui->cboOrden->setCurrentIndex(0);
    emit doSearch(ui->txtBuscar->text(),ui->cboOrden->currentText(),ui->cboModo->currentText());
}

void BarraBusqueda::textChanged(const QString &)
{
    emit doSearch(ui->txtBuscar->text(),ui->cboOrden->currentText(),ui->cboModo->currentText());
}

void BarraBusqueda::mousePressEvent(QMouseEvent * e)
{
    if(e->pos().x() < 21)
        if(e->pos().y()>20 && e->pos().y() < 100)
        {
            if(this->isShow())
                emit hideMe();
            else
                emit showMe();
        }

}

void BarraBusqueda::resizeEvent(QResizeEvent *)
{
    if(m_Cache)
      delete m_Cache;

    m_Cache = new QPixmap(size());

    m_Cache->fill(Qt::transparent);

    QPainter painter(m_Cache);
    if(painter.isActive())
    {
        painter.setRenderHint(QPainter::Antialiasing);

       // QColor darkBlue ( 182,159,64,230);
        QColor darkBlue (126,184,121,230);
        painter.setPen  (Qt::NoPen/*QPen  (darkBlue)*/);
        painter.setBrush(QBrush(darkBlue));

        QPainterPath path;
        path.setFillRule( Qt::WindingFill );
        path.addRoundedRect( QRect(0,20, 20, 80), 10, 10 );
        path.addRect( QRect( 10, 20, 10, 80 ) );
        painter.drawPath( path.simplified() ); // Only Top left & bottom right corner

       // painter.drawRoundedRect(0,20,20,80,10,10);
        //painter.drawRect(10,20,10,80);

        if(width()>20)
            painter.drawRoundedRect(20,0,width()-20,height(),20,20);

        QPixmap p(":/Icons/PNG/search2.png");
        painter.drawPixmap(QRect(2,50,20,20),p);
    }
}
