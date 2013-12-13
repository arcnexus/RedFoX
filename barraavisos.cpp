#include "barraavisos.h"
#include "ui_barraavisos.h"
#include <QPainter>
#include "Auxiliares/frmeditaravisos.h"

BarraAvisos::BarraAvisos(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BarraAvisos)
{    
    ui->setupUi(this);
    m_Cache = 0;
    m_show = false;
    m_color = QColor(126,184,121,230);
    this->installEventFilter(this);
    t.setInterval(30000);
    connect(&t,SIGNAL(timeout()),this,SLOT(llenarAvisos()));
    t.start();
}


BarraAvisos::~BarraAvisos()
{
    delete ui;
}

void BarraAvisos::setPagina(int pagina)
{
   // ui->stackedWidget->setCurrentIndex(pagina);
}

void BarraAvisos::paintEvent(QPaintEvent *pe) {
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

void BarraAvisos::resizeEvent(QResizeEvent *)
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

        painter.setPen  (Qt::NoPen/*QPen  (darkBlue)*/);
        painter.setBrush(QBrush(m_color));

        QPainterPath path;
        path.setFillRule( Qt::WindingFill );
        path.addRoundedRect( QRect(this->width()-20,20, 20, 80), 10, 10 );
        path.addRect( QRect( this->width()-20, 20, 10, 80 ) );

        painter.save();
            painter.drawPath( path.simplified() ); // Only Top left & bottom right corner
        painter.restore();

       // painter.drawRoundedRect(0,20,20,80,10,10);
        //painter.drawRect(10,20,10,80);
        painter.save();
            if(width()>20)
                  painter.drawRoundedRect(-20,0,width(),height(),20,20);
        painter.restore();

        painter.save();
            painter.setPen(Qt::SolidLine);
            painter.setPen(Qt::white);
            //painter.setBrush(Qt::black);
            painter.translate(this->width()-20,100);
            painter.rotate(270);
            painter.drawText(QRectF(0,0,80,20),Qt::AlignCenter,tr("Avisos"));
        painter.restore();
    }
}

void BarraAvisos::showGlobo()
{
    if(m_globo)
        delete m_globo;
    m_globo = new GloboAviso(this);
    m_globo->setGeometry(mapToGlobal(this->pos()).x()+this->width(),mapToGlobal(this->pos()).y(),225,35);
    m_globo->show();
}

void BarraAvisos::hideGlobo()
{
    if(m_globo)
        m_globo->setVisible(false);
}

void BarraAvisos::restoreGlobo()
{
    if(m_globo)
        m_globo->setVisible(true);
}

void BarraAvisos::mousePressEvent(QMouseEvent * e)
{
    if(e->pos().x()> width()-20)
        if(e->pos().y()>20 && e->pos().y() < 100)
        {
            if(this->isShow())
                emit hideMe();
            else
            {
                llenarAvisos();
                emit showMe();
                hideGlobo();
            }
        }
}

bool BarraAvisos::eventFilter(QObject *, QEvent *e)
{
    if(e->type() == QEvent::Move)
        if(m_globo)
            m_globo->move(this->pos().x()+this->width(),this->pos().y()+50);
    return false;
}

void BarraAvisos::llenarAvisos()
{
    QMap <int,QSqlRecord> map;
    QStringList clausulas,headers;
    QString error;
    clausulas << QString("id_usuario_destino = %1").arg(Configuracion_global->id_usuario_activo);
    clausulas << QString("completado = 0");
    map = SqlCalls::SelectRecord("avisos",clausulas,Configuracion_global->groupDB,error);
    ui->tabla_avisos->setColumnCount(4);
    headers <<"id" <<tr("ORIGEN") << tr("AVISO") <<tr("FECHA/HORA");
    ui->tabla_avisos->setHorizontalHeaderLabels(headers);
    QVariantList  sizes;
    sizes <<0 << 150 <<260 << 115;
    for(int i = 0; i< sizes.size();i++ )
        ui->tabla_avisos->setColumnWidth(i,sizes.at(i).toInt());
    QMapIterator <int,QSqlRecord> it(map);
    int pos = 0;
    QColor rowcolor;
    while (it.hasNext())
    {
        it.next();
        if(pos%2==0)
            rowcolor.setRgb(240,240,255);
        else
            rowcolor.setRgb(255,255,255);
        ui->tabla_avisos->setRowCount(pos+1);
        QTableWidgetItem *item_col0 = new QTableWidgetItem(QString::number(it.value().value("id").toInt()));
        item_col0->setFlags(item_col0->flags() & (~Qt::ItemIsEditable));
        item_col0->setBackgroundColor(rowcolor);

        ui->tabla_avisos->setItem(pos,0,item_col0);
        ui->tabla_avisos->setColumnHidden(0,true);

        QTableWidgetItem *item_col1 = new QTableWidgetItem("User Origen");
        item_col1->setFlags(item_col1->flags() & (~Qt::ItemIsEditable));
        item_col1->setBackgroundColor(rowcolor);

        ui->tabla_avisos->setItem(pos,1,item_col1);

        QTableWidgetItem *item_col2 = new QTableWidgetItem(it.value().value("aviso").toString());
        item_col2->setFlags(item_col2->flags() & (~Qt::ItemIsEditable));
        item_col2->setBackgroundColor(rowcolor);

        ui->tabla_avisos->setItem(pos,2,item_col2);

        QTableWidgetItem *item_col3 = new QTableWidgetItem(it.value().value("fecha_hora_aviso").toDateTime().toString("dd/MM/yyyy-hh:mm"));
        item_col3->setFlags(item_col3->flags() & (~Qt::ItemIsEditable));
        item_col3->setBackgroundColor(rowcolor);

        ui->tabla_avisos->setItem(pos,3,item_col3);
        pos ++;
    }
    if(map.size()>0)
    {
        this->showGlobo();
        this->setColor(QColor(Qt::darkRed));
    }
}

void BarraAvisos::moveGloblo(QPoint topLeft)
{
    if(m_globo)
        m_globo->move(topLeft.x()+20,mapToGlobal(this->pos()).y());
}

bool BarraAvisos::isShow() const
{
    return m_show;
}

void BarraAvisos::setShow(bool show)
{
    m_show = show;
}

void BarraAvisos::setColor(QColor c)
{
    m_color = c;
    this->resize(this->width(),this->height());
}


void BarraAvisos::on_tabla_avisos_doubleClicked(const QModelIndex &index)
{
    int id = ui->tabla_avisos->model()->index(ui->tabla_avisos->currentRow(),0).data().toInt();
    FrmEditarAvisos formavisos(this);
    formavisos.setValores(id);
    formavisos.exec();
    llenarAvisos();
}
