#include "reportimage.h"
#include "editimagedlg.h"
ReportImage::ReportImage(QGraphicsItem *parent) :
    Container(parent)
{
    m_ruta = "";
    m_fromDB = false;
    m_dinamica = false;
}

QDomElement ReportImage::xml(QDomDocument doc, QPointF relPos)
{
    QDomElement node = doc.createElement("Element");
    node.setAttribute("id","Image");

    Container::apendXML(node,doc, relPos);

    QDomElement Path = doc.createElement("Path");
    Path.setAttribute("value",m_ruta);

    QDomElement fromBD = doc.createElement("fromBD");
    fromBD.setAttribute("value",m_fromDB);

    QDomElement d= doc.createElement("Dinamic");
    d.setAttribute("value",m_dinamica);

    node.appendChild(Path);
    node.appendChild(fromBD);
    node.appendChild(d);

    return node;
}

void ReportImage::parseXml(QDomElement element, QPointF origin)
{
    QDomNodeList list = element.childNodes();
    for(int i=0;i<list.size();i++)
    {
        QDomElement el = list.at(i).toElement();
        QString tag = el.tagName();
        //Common tag for every container subclass
        if(tag == "Pos")
            this->setPos(el.attribute("x").toInt() + origin.x(),el.attribute("y").toInt()+origin.y());
        else if(tag=="Size")
            this->setSize(el.attribute("w").toInt(),el.attribute("h").toInt());

        //Specific tags
        //"Text""Orientacion""Alineacion""fontFamily""fontSize""fontWeigth""italicFont"
        else if(tag== "Path")
            this->setruta(el.attribute("value"));
        else if(tag== "fromBD")
            this->setfromDB(el.attribute("value").toInt());
        else if(tag== "Dinamic")
            this->setdinamica(el.attribute("value").toInt());
    }
}


void ReportImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);
    painter->drawImage(this->rect(),m_image);
    painter->restore();
    if(this->isSelected())
        Container::paint(painter, option, widget);
}

QString ReportImage::ruta() const {
    return m_ruta;
}

bool ReportImage::fromDB() const {
    return m_fromDB;
}

void ReportImage::setruta(QString arg) {
    if (m_ruta != arg) {
        m_ruta = arg;

        if(m_fromDB)
        {
            //TODO get image from db
        }
        else
        {
            QFile f(arg);
            if(f.open(QFile::ReadOnly))
            {
                m_image = QImage(arg);
                this->setRect(0,0,m_image.size().width(),m_image.size().height());
            }
        }
        emit rutaChanged(arg);
    }
}

void ReportImage::setfromDB(bool arg) {
    if (m_fromDB != arg) {
        m_fromDB = arg;
        emit fromDBChanged(arg);
    }
}

bool ReportImage::dinamica() const {
    return m_dinamica;
}

void ReportImage::setdinamica(bool arg) {
    if (m_dinamica != arg) {
        m_dinamica = arg;
        emit dinamicaChanged(arg);
    }
}


void ReportImage::editMe()
{
    EditImageDlg dlg(this,qApp->activeWindow());
    dlg.exec();
}
