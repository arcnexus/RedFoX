#include "paper.h"
#include "sectioneditordlg.h"
//using namespace Paper_size;
struct _size {
        double width;
        double heigth;
    };

_size StandarSizes [] = {
    {841 , 1189},   {594 , 841},    {420 , 594},    {297 , 420},
    {210 , 297},    {148 , 210},    {105 , 148},    {74 , 105},
    {52 , 74},      {37 , 52},      {1000 , 1414},  {707 , 1000},
    {500 , 707},    {353 , 500},    {250 , 353},    {176 , 250},
    {125 , 176},    {88 , 125},     {62 , 88},      {33 , 62},
    {31 , 44},      {163 , 229},    {105 , 241},    {110 , 220},
    {190.5 , 254},  {210 , 330},    {431.8 , 279.4},{215.9 , 355.6},
    {215.9 , 279.4},{279.4 , 431.8 }
                                    };

Paper::Paper(QGraphicsItem *parent) :
    QGraphicsRectItem(parent)
{    
    _inserting = false;
    mySize = A4;
    m_orientacion = Retrato;
}

QRectF Paper::margin()
{
    return QRectF(cmToPx(margenSuperior()),
                  cmToPx(margenSuperior()),
                  this->rect().width() - cmToPx(margenSuperior()) - cmToPx(margenSuperior())- 5,
                  this->rect().height()- cmToPx(margenSuperior()) - cmToPx(margenSuperior())- 5);
}

QRectF Paper::paper()
{
    return QRectF(0,
                  0,
                  this->rect().width() -  5,
                  this->rect().height()-  5);
}

Paper::_Sizes Paper::StandartSize()
{
    return mySize;
}

Paper::_Orientacion Paper::Orientacion() const {
    return m_orientacion;
}

void Paper::setOrientacion(Paper::_Orientacion arg) {
    if (m_orientacion != arg) {
        QRectF r = this->rect();
//        if(arg == Retrato)
//            this->setRect(0,0, qMin(r.height(),r.width()),qMax(r.height(),r.width()));
//        else
            this->setRect(0,0, r.height(),r.width());
        m_orientacion = arg;
        emit orientacionChanged(arg);
    }
}

void Paper::setMargen(double arg)
{
    setmargenDerecho(arg);
    setmargenIzquierdo(arg);
    setmargenSuperior(arg);
    setmargenInferior(arg);
}

void Paper::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF r = this->rect();
    painter->fillRect(5,5,r.width()-5,r.height()-5,Qt::black);
    painter->fillRect(0,0,r.width()-5,r.height()-5,Qt::white);
    painter->drawRect(0,0,r.width()-5,r.height()-5);
    painter->setPen(Qt::DotLine);
    painter->drawRect(margin());
}

void Paper::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(_inserting)
    {
        if(margin().contains(event->pos()))
        {
            _insertingPoint = event->pos();
            switch (_insertingType)
            {
            case Paper::CampoRelacional:
                insertCampoRelacional();
                break;
            case Paper::Campo:
                insertCampo();
                break;
            case Paper::Imagen:
                insertImagen();
                break;
            case Paper::CodeBarIt:
                insertCodeBar();
                break;
            case Paper::Linea:
                insertLinea();
                break;
            case Paper::Label:
                insertLabel();
                break;
            case RoundRectIt:
                insertRoundRect();
                break;
            default:
                break;
            }
            emit itemInserted();
        }
    }
    else
    {
        QList<QGraphicsItem*> list = this->scene()->selectedItems();
        for(int i=0; i<list.size();i++)
            list.at(i)->setSelected(false);
    }
}

void Paper::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    SectionEditorDlg dlg(this,qApp->activeWindow());
    dlg.exec();
}

qreal Paper::cmToPx(double cm)
{
    double inch = cm * 0.3937008;
    return inch * 96.0;
}

qreal Paper::pxTocm(int px)
{
    double inch = px/96.0;
    return inch/0.3937008;
}

void Paper::insertRoundRect()
{
    RoundedRect * rect = new RoundedRect(this);
    rect->setMargins(this->margin());
    rect->setSize(100,100);
    rect->setPos(_insertingPoint.x()-50,_insertingPoint.y()-50);
    itemPool.append(rect);
}

void Paper::insertLabel()
{
    CustomLabel * rect = new CustomLabel(this);
    rect->setMargins(this->margin());
    rect->setSize(100,15);
    rect->setPos(_insertingPoint.x()-20,_insertingPoint.y()-7);
    rect->setText("Lorem ipsum ad his scripta blandit partiendo, eum fastidii accumsan euripidis in, eum liber hendrerit an. Qui ut wisi vocibus suscipiantur, quo dicit ridens inciderint id. Quo mundi lobortis reformidans eu, legimus senserit definiebas an eos.");
    //rect->setfontSize(40);
    rect->setfontSize(10);
    itemPool.append(rect);
}

void Paper::insertLinea()
{
    ReportLine * line = new ReportLine (this);
    line->setMargins(this->margin());
    line->setSize(200,15);
    line->setPos(_insertingPoint.x()-100,_insertingPoint.y()-7);
    itemPool.append(line);
}

void Paper::insertCodeBar()
{
    CodeBar* code = new CodeBar(this);
    code->setMargins(this->margin());
    code->setcode("*123456789*");
    code->setvisibleCode(true);
    code->setPos(_insertingPoint.x()-100,_insertingPoint.y()-7);
    itemPool.append(code);
}

void Paper::insertImagen()
{
    ReportImage * img = new ReportImage(this);
    img->setruta("C:/Maya/maya/PNG/albaran_pro.png");
    img->setMargins(this->margin());
    img->setPos(_insertingPoint.x()-50,_insertingPoint.y()-7);
    itemPool.append(img);
}

void Paper::insertCampo()
{
    ReportField * fld = new ReportField(this)   ;
    fld->setMargins(this->margin());
    fld->setSize(100,20);
    fld->setPos(_insertingPoint.x()-50,_insertingPoint.y()-7);
    itemPool.append(fld );
}

void Paper::insertCampoRelacional()
{
    RelationalField * fld = new RelationalField(this)   ;
    fld->setMargins(this->margin());
    fld->setSize(150,20);
    fld->setPos(_insertingPoint.x()-50,_insertingPoint.y()-7);
    itemPool.append(fld );
}

double Paper::margenSuperior() const {
    return m_margenSuperior;
}

void Paper::setSize(double w, double h)
{
    this->setRect(0,0,cmToPx(w)+5,cmToPx(h)+5);
    scene()->setSceneRect(-10,-10,rect().width()+200,rect().height()+15);
}

void Paper::_insertSection(Section *sec)
{
    if(seccionPool.isEmpty())
        seccionPool.append(sec);
    else
    {
        bool inserted = false;
        Section::SectionType type = sec->sectionType();
        for(int i=0;i<seccionPool.size();i++)
        {
            if(seccionPool.at(i)->sectionType() > type)
            {
                seccionPool.insert(i,sec);
                inserted = true;
                break;
            }
        }
        if(!inserted)
            seccionPool.append(sec);
    }
}

void Paper::reCalculateSeccion(Section * secSender)
{
    int position = margin().top();
    int final = 0;
    QPair<Section *,int> maxHeigth;
    QListIterator<Section*> it(seccionPool);
    while(it.hasNext())
    {
        Section* sec = it.next();
        sec->setvPos(position);
        position += sec->Height()+1;

        if(sec->Height() > maxHeigth.second)
        {
            maxHeigth.first = sec;
            maxHeigth.second = sec->Height();
        }
        if(!it.hasNext())
            final = position ;//+ sec->Height();
    }
    if(final > margin().bottom())
    {
        int diff = final - margin().bottom();
        if(secSender->Height() > diff)
        {
            secSender->setOnMaxsize(true);
            secSender->setHeight(secSender->Height() - diff);
            int position = margin().top();
            QListIterator<Section*> it(seccionPool);
            while(it.hasNext())
            {
                Section* sec = it.next();
                sec->setvPos(position);
                position += sec->Height()+1;
            }
        }
        else
        {
            maxHeigth.first->setHeight(maxHeigth.second - diff);
            int position = margin().top();
            QListIterator<Section*> it(seccionPool);
            while(it.hasNext())
            {
                Section* sec = it.next();
                sec->setvPos(position);
                position += sec->Height()+1;
            }
        }
    }
}

void Paper::setSize(_Sizes siz, double w, double h, _Orientacion o)
{
    mySize = siz;
    if(siz != QPrinter::Custom)
    {
        if(o== Retrato)
            this->setSize(StandarSizes[siz].width/10 , StandarSizes[siz].heigth/10);
        else
            this->setSize(StandarSizes[siz].heigth/10 , StandarSizes[siz].width/10);
    }
}

bool Paper::addSection(QString nombre, Section::SectionType type)
{
    bool valid = true;
    QListIterator<Section*> it(seccionPool);
    while (it.hasNext())
    {
        if(it.next()->SectionName() == nombre)
        {
            valid = false;
            break;
        }
    }
    if(!valid)
        return false;

    if(type != Section::Detail && type!= Section::PageHeader)
    {
        Section * seccion = new Section(this);
        seccion->setType(type);
        seccion->setSectionName(nombre);
        seccion->setMargin(margin());
        if(seccionPool.isEmpty())
        {
            seccionPool.append(seccion);
            seccion->setvPos(margin().top());
        }
        else
        {
            switch (type) {
            case Section::ReportHeader:
                seccionPool.prepend(seccion);
                break;
            case Section::PageFooter:
            case Section::Detail:
                _insertSection(seccion);
                break;
            default:
                seccionPool.append(seccion);
                break;
            }
            reCalculateSeccion(seccion);
        }
        connect(seccion,SIGNAL(resized(Section*)),this,SLOT(reCalculateSeccion(Section*)));
    }
    else if(type==Section::Detail)
    {
        DetailSection * seccion = new DetailSection(this);
        seccion->setType(type);
        seccion->setSectionName(nombre);
        seccion->setMargin(margin());
        if(seccionPool.isEmpty())
        {
            seccionPool.append(seccion);
            seccion->setvPos(margin().top());
        }
        else
        {
            _insertSection(seccion);
            reCalculateSeccion(seccion);
        }
        connect(seccion,SIGNAL(resized(Section*)),this,SLOT(reCalculateSeccion(Section*)));
    }
    else
    {
        PageHeaderSection * seccion = new PageHeaderSection(this);
        seccion->setType(type);
        seccion->setSectionName(nombre);
        seccion->setMargin(margin());
        if(seccionPool.isEmpty())
        {
            seccionPool.append(seccion);
            seccion->setvPos(margin().top());
        }
        else
        {
            _insertSection(seccion);
            reCalculateSeccion(seccion);
        }
        connect(seccion,SIGNAL(resized(Section*)),this,SLOT(reCalculateSeccion(Section*)));
    }
    return true;
}

void Paper::removeSection(QString nombre)
{
    QListIterator<Section*> it(seccionPool);
    while (it.hasNext())
    {
        Section* sec = it.next();
        if(sec->SectionName() == nombre)
        {
            seccionPool.removeOne(sec);
            sec->deleteLater();
        }
    }
    reCalculateSeccion();
}

void Paper::prepareItemInsert(itemType type)
{
    _insertingType = type;
    _inserting = true;
}

void Paper::stopInsertingItems()
{
    _inserting = false;
}

bool Paper::parseXML(QString xml, QString & error)
{
    QListIterator<Section*> secIt(seccionPool);
    QListIterator<Container*> itemIt(itemPool);

    while(secIt.hasNext())
        secIt.next()->deleteLater();

    while(itemIt.hasNext())
        itemIt.next()->deleteLater();

    seccionPool.clear();
    itemPool.clear();

    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument doc;
    if (!doc.setContent(xml, false, &errorStr, &errorLine,
                        &errorColumn))
    {
        error = QString("Error: Parse error at line %1, column %2 : %3").arg(errorLine).arg(errorColumn).arg(errorStr);
        return false;
    }
    else
    {
        QDomElement root = doc.documentElement();
        if (root.tagName() != "FoxReports") {
            error = "Error: Not a FoxReports file";
            return false;
        }
        else
        {
            QDomNode child = root.firstChild();
            while (!child.isNull()) //Papers TODO multipage report?
            {
                QDomNode sections = child.firstChild();
                while (!sections.isNull())
                {
                    QDomElement secEle = sections.toElement();

                    if(secEle.tagName() == "Size")
                        this->setSize(secEle.attribute("w").toDouble(),secEle.attribute("h").toDouble());
                    if(secEle.tagName() == "Size")
                        this->m_orientacion = secEle.attribute("type") == "V" ? Retrato : Apaisado;
                    else if(secEle.tagName() == "Margin")
                    {
                        this->setmargenDerecho(secEle.attribute("rigth").toDouble());
                        this->setmargenIzquierdo(secEle.attribute("left").toDouble());
                        this->setmargenSuperior(secEle.attribute("top").toDouble());
                        this->setmargenInferior(secEle.attribute("bottom").toDouble());
                    }
                    else if(secEle.tagName() == "Section")
                    {
                        int typeOfSection = secEle.attribute("id").toInt();
                        Section::SectionType type = static_cast<Section::SectionType>(typeOfSection);
                        if(type != Section::Detail && type != Section::PageHeader)
                        {
                            Section * sec = new Section(this);
                            sec->setHeight(secEle.attribute("size").toInt());
                            sec->setSectionName(secEle.attribute("name"));
                            sec->setMargin(margin());
                            sec->setType(type);
                            _insertSection(sec);
                            reCalculateSeccion(sec);
                            sec->parseXml(secEle.firstChild().toElement(), itemPool);
                            connect(sec,SIGNAL(resized(Section*)),this,SLOT(reCalculateSeccion(Section*)));
                        }
                        else if(type == Section::Detail)
                        {
                            DetailSection* sec = new DetailSection(this);
                            sec->setHeight(secEle.attribute("size").toInt());
                            sec->setSectionName(secEle.attribute("name"));
                            sec->setType(type);
                            sec->setMargin(margin());                            
                            sec->setSqlGlobal(secEle.attribute("SqlGlobal"));
                            sec->setSqlInterno(secEle.attribute("SqlInterno"));
                            sec->setClausulaInterna(secEle.attribute("ClausulaInterna"));
                            sec->setcolorear(secEle.attribute("colored").toInt());
                            sec->setcolor1(sec->ColorFromString(secEle.attribute("color1")));
                            sec->setuse2Colors(secEle.attribute("alternative").toInt());
                            sec->setcolor2(sec->ColorFromString(secEle.attribute("color2")));
                            _insertSection(sec);
                            reCalculateSeccion(sec);
                            connect(sec,SIGNAL(resized(Section*)),this,SLOT(reCalculateSeccion(Section*)));
                            sec->parseXml(secEle.firstChild().toElement(), itemPool);                            
                        }
                        else
                        {
                            PageHeaderSection * sec = new PageHeaderSection(this);
                            sec->setOnFisrtPage(secEle.attribute("OnFistPage").toInt());
                            sec->setHeight(secEle.attribute("size").toInt());
                            sec->setSectionName(secEle.attribute("name"));
                            sec->setMargin(margin());
                            sec->setType(type);
                            _insertSection(sec);
                            reCalculateSeccion(sec);
                            sec->parseXml(secEle.firstChild().toElement(), itemPool);
                            connect(sec,SIGNAL(resized(Section*)),this,SLOT(reCalculateSeccion(Section*)));
                        }
                    }
                    sections = sections.nextSibling();
                }
                child = child.nextSibling();
            }
        }
    }
    return true;
}

int Paper::save(QString file)
{
    QFile f(file);
    if(f.open(QFile::WriteOnly))
    {
        QTextStream out(&f);
        const int Indent = 4;
        QDomDocument doc;
        QDomElement root = doc.createElement("FoxReports");

        QDomElement paperNode = doc.createElement("Paper");

        QDomElement siz = doc.createElement("Size");
        siz.setAttribute("w", QString::number( pxTocm(this->paper().width ()), 'f', 2));
        siz.setAttribute("h", QString::number( pxTocm(this->paper().height()), 'f', 2));

        QDomElement din = doc.createElement("StandartSize");
        din.setAttribute("type",this->mySize);

        QDomElement orientacion = doc.createElement("Orientation");
        orientacion.setAttribute("type",this->m_orientacion == Retrato ? "V" : "H");

        QDomElement margin = doc.createElement("Margin");
        margin.setAttribute("top",QString::number(this->margenSuperior(), 'f', 2));
        margin.setAttribute("bottom",QString::number(this->margenInferior(), 'f', 2));
        margin.setAttribute("left",QString::number(this->margenIzquierdo(), 'f', 2));
        margin.setAttribute("rigth",QString::number(this->margenDerecho(), 'f', 2));

        doc.appendChild(root);
        root.appendChild(paperNode);
        paperNode.appendChild(siz);
        paperNode.appendChild(din);
        paperNode.appendChild(orientacion);
        paperNode.appendChild(margin);

        QList<Section*>::Iterator it;
        QList<Container*> usedItems;
        QMap<QString,bool> querys;
        for(it=seccionPool.begin();it!=seccionPool.end();++it)
            paperNode.appendChild((*it)->xml(doc,usedItems,querys));

        QMapIterator<QString,bool> mIt(querys);
        while(mIt.hasNext())
        {
            QString s = mIt.next().key();
            if(!s.isEmpty())
            {
                QDomElement sql = doc.createElement("SQL");
                sql.setAttribute("target",s);
                paperNode.appendChild(sql);
            }
        }
        QDomNode xmlNode = doc.createProcessingInstruction("xml",
                                                           "version=\"1.0\" encoding=\"ISO-8859-1\"");
        doc.insertBefore(xmlNode, doc.firstChild());
        doc.save(out, Indent);
        f.close();

        if (usedItems.size() < itemPool.size())
            return -2;
        else
            return 1;
    }
    else
        return -1;
}

QDomDocument Paper::preview()
{
    QDomDocument doc;
    QDomElement root = doc.createElement("FoxReports");

    QDomElement paperNode = doc.createElement("Paper");

    QDomElement siz = doc.createElement("Size");
    siz.setAttribute("w", QString::number( pxTocm(this->paper().width ()), 'f', 2));
    siz.setAttribute("h", QString::number( pxTocm(this->paper().height()), 'f', 2));

    QDomElement din = doc.createElement("StandartSize");
    din.setAttribute("type",this->mySize);

    QDomElement orientacion = doc.createElement("Orientation");
    orientacion.setAttribute("type",this->m_orientacion == Retrato ? "V" : "H");

    QDomElement margin = doc.createElement("Margin");
    margin.setAttribute("top",QString::number(this->margenSuperior(), 'f', 2));
    margin.setAttribute("bottom",QString::number(this->margenInferior(), 'f', 2));
    margin.setAttribute("left",QString::number(this->margenIzquierdo(), 'f', 2));
    margin.setAttribute("rigth",QString::number(this->margenDerecho(), 'f', 2));

    doc.appendChild(root);
    root.appendChild(paperNode);
    paperNode.appendChild(siz);
    paperNode.appendChild(din);
    paperNode.appendChild(orientacion);
    paperNode.appendChild(margin);

    QList<Section*>::Iterator it;
    QList<Container*> usedItems;
    QMap<QString,bool> querys;
    for(it=seccionPool.begin();it!=seccionPool.end();++it)
        paperNode.appendChild((*it)->xml(doc,usedItems,querys));

    QMapIterator<QString,bool> mIt(querys);
    while(mIt.hasNext())
    {
        QString s = mIt.next().key();
        if(!s.isEmpty())
        {
            QDomElement sql = doc.createElement("SQL");
            sql.setAttribute("target",s);
            paperNode.appendChild(sql);
        }
    }
    QDomNode xmlNode = doc.createProcessingInstruction("xml",
                                                       "version=\"1.0\" encoding=\"ISO-8859-1\"");
    doc.insertBefore(xmlNode, doc.firstChild());



    QFile f("/home/arcnexus/pre.xml");
    if(f.open(QFile::WriteOnly))
    {
        QTextStream t(&f);
        doc.save(t,4);
    }
    f.close();

    return doc;
}

void Paper::setmargenSuperior(double arg) {
    if (m_margenSuperior != arg) {
        m_margenSuperior = arg;
        emit margenSuperiorChanged(arg);
    }
}

double Paper::margenInferior() const {
    return m_margenInferiro;
}

double Paper::margenIzquierdo() const {
    return m_margenIzquierdo;
}

double Paper::margenDerecho() const {
    return m_margenDerecho;
}

void Paper::setmargenInferior(double arg) {
    if (m_margenInferiro != arg) {
        m_margenInferiro = arg;
        emit margenInferiroChanged(arg);
    }
}

void Paper::setmargenIzquierdo(double arg) {
    if (m_margenIzquierdo != arg) {
        m_margenIzquierdo = arg;
        emit margenIzquierdoChanged(arg);
    }
}

void Paper::setmargenDerecho(double arg) {
    if (m_margenDerecho != arg) {
        m_margenDerecho = arg;
        emit margenDerechoChanged(arg);
    }
}

QList<Section *> Paper::getSeccionPool() const
{
    return seccionPool;
}

void Paper::removeItems(QList<QGraphicsItem *> items)
{
    QListIterator<Container*> it(itemPool);
    while(it.hasNext())
    {
        Container * cont = it.next();
        if(items.contains(cont))
        {
            itemPool.removeOne(cont);
            cont->deleteLater();
        }
    }
}

void Paper::insertSection(Section *sec)
{
    _insertSection(sec);
}

void Paper::subirSeccion(QString name)
{
    int i = 0;
    for (;i< seccionPool.size();i++)
    {
        if(seccionPool.at(i)->SectionName() == name)
        {
            if(i!= 0)
            {
                seccionPool.swap(i,i-1);
                break;
            }
        }
    }
    //TODO mover items tambien
    reCalculateSeccion(seccionPool.at(i-1));
}

void Paper::bajarSeccion(QString name)
{
    int i = 0;
    for (;i< seccionPool.size();i++)
    {
        if(seccionPool.at(i)->SectionName() == name)
        {
            if(i!= seccionPool.size()-1)
            {
                seccionPool.swap(i,i+1);
                break;
            }
        }
    }
    //TODO mover items tambien
    reCalculateSeccion(seccionPool.at(i+1));
}

void Paper::borrarSeccion(QString name)
{
    int i = 0;
    for (;i< seccionPool.size();i++)
    {
        if(seccionPool.at(i)->SectionName() == name)
        {
             Section * sec =  seccionPool.takeAt(i);
             sec->deleteLater();
             break;
        }
    }
    //TODO mover items tambien
    if(!seccionPool.isEmpty())
        reCalculateSeccion(seccionPool.at(0));
}

void Paper::updatePaper()
{
    for(int i=0;i<seccionPool.size();i++)
    {
        seccionPool.at(i)->setWidth(this->rect().width());
        seccionPool.at(i)->setMargin(this->margin());
    }
    for(int a = 0;a<itemPool.size();a++)
        itemPool.at(a)->setMargins(this->margin());
    this->scene()->setSceneRect(-15,-15,this->rect().width()+100,this->rect().height()+30);
}
