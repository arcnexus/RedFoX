#include "openchart.h"
#include <QDebug>
#include <QTime>
#include <qmath.h>
#include <QPropertyAnimation>
OpenChart::OpenChart(QWidget *parent) :
    QWidget(parent)
{
    qsrand(QTime::currentTime().msec());

    m_mayor =0;
    m_maxValue = 0;
    m_menor = 0;

    m_type = Barras;
    m_sombra = true;
    m_useLeyenda = true;
    m_tipoLeyenda = Vertical;
    m_usingTitle = true;
    m_title = "Un titulo";
    m_valuesEnY = true;
    m_animation = true;
    m_aniDuration = 700;
}

QSize OpenChart::minimumSizeHint() const
{
    return QSize(200,100);
}

void OpenChart::addItem(QString nombre, float value)
{
    int r = qrand()%255;
    int g = qrand()%255;
    int b = qrand()%255;
    addItem(nombre,value,QColor(r,g,b));
}

void OpenChart::addItem(QString nombre, float value, QColor color)
{
    QChartPiece p;
    p.nombre = nombre;
    p.addValue(value);
    p.color = color;
    pieces.append(p);

    m_mayor = qMax(m_mayor,(int)value);
    m_menor = qMin(m_menor,(int)value);
    m_maxValue = qMax(m_mayor, -m_menor);
    startAnimation();
}

void OpenChart::addItem(QString nombre, QVector<float> values)
{
    int r = qrand()%255;
    int g = qrand()%255;
    int b = qrand()%255;
    addItem(nombre,values,QColor(r,g,b));
}

void OpenChart::addItem(QString nombre, QVector<float> values, QColor color)
{
    QChartPiece p;
    p.nombre = nombre;
    p.addValue(values);
    p.color = color;
    pieces.append(p);

    for(int i=0;i<values.size();i++)
    {
        float value = values.at(i);
        m_mayor = qMax(m_mayor,(int)value);
        m_menor = qMin(m_menor,(int)value);
    }
    m_maxValue = qMax(m_mayor, -m_menor);
    startAnimation();
}

void OpenChart::addMulibarColor(QString nombre, QColor color)
{
    QPair<QString , QColor> par;
    par.first = nombre;
    par.second = color;
    multibarColors.append(par);
}

void OpenChart::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
   // painter.drawRect(0,0,width()-1,height()-1);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setFont(m_letra);

    if(pieces.isEmpty())
    {
        m_xAxis = height()/2;
        m_left = 5;
        m_rigth = width();
        drawAxis(&painter);
        return;
    }

    int fontH = painter.fontMetrics().height();

    if(m_type == Sectores_2D || m_type == Sectores_3D)
    {
        m_left = 0;
        m_rigth = this->width();


        m_top = 0;
        m_bottom = this->height();

        if(m_usingTitle)
        {
            QFont title = m_letra;
            title.setItalic(true);
            title.setBold(true);
            title.setUnderline(true);
            painter.save();
            painter.setFont(title);
            m_top+=fontH;
            m_top+=3;
            painter.drawText(this->width()/2-painter.fontMetrics().width(m_title)/2,fontH,m_title);
            painter.restore();
        }

        if(m_useLeyenda)
        {
            int maxLength = 0;
            for(int i=0;i< pieces.size(); i++)
            {
                if(m_type != Lineas)
                    maxLength = qMax(maxLength,painter.fontMetrics().width(QString("%1 : %2").arg(pieces.at(i).nombre).arg(pieces.at(i).value())));
                else
                    maxLength = qMax(maxLength,painter.fontMetrics().width(pieces.at(i).nombre));
            }

            if(m_tipoLeyenda == Circular)
            {
                m_top+= fontH+15;
                m_bottom -= fontH+15;
                maxLength+= fontH + 15;
                m_rigth-=maxLength;
                m_left += maxLength;
            }
            else
            {
                maxLength+= fontH + 10;
                m_rigth-=maxLength;
            }
        }
        m_width = m_rigth - m_left;
        m_heigth = m_bottom - m_top;
    }
    else
    {
        m_left = 5;
        m_rigth = this->width();
        if(m_valuesEnY)
        {
            QString s = QString::number(m_maxValue);
            if(m_menor<0)
                s.prepend("-");
            m_left += painter.fontMetrics().width(s);
        }
        if(m_useLeyenda)
        {
            int maxLength = 0;
            if(m_tipoLeyenda == Vertical && m_type != BarraMultiple)
            {
                for(int i=0;i< pieces.size(); i++)
                {
                    if(m_type != Lineas)
                        maxLength = qMax(maxLength,painter.fontMetrics().width(QString("%1 : %2").arg(pieces.at(i).nombre).arg(pieces.at(i).value())));
                    else
                        maxLength = qMax(maxLength,painter.fontMetrics().width(pieces.at(i).nombre));
                }
            }
            else if(m_tipoLeyenda == Vertical && m_type == BarraMultiple)
            {
                for(int i=0;i< multibarColors.size(); i++)
                    maxLength = qMax(maxLength,painter.fontMetrics().width(multibarColors.at(i).first));
            }
            maxLength+= fontH + 10;
            m_rigth-=maxLength;
        }
        m_width = m_rigth - m_left;

        m_top = 0;
        m_bottom = this->height();
        if(m_labels)
        {
            if(m_type == BarraMultiple || m_type == Lineas)
            {
                if(m_mayor >= 0 && m_menor >=0)
                    m_bottom -= fontH;
                else if(m_mayor <= 0 && m_menor <=0)
                    m_top+=fontH;
                else
                {
                    m_top+=fontH;
                    m_bottom -= fontH;
                }
            }
            else if(m_type == Barras )
            {
                if(m_mayor<=0)
                    m_top+=fontH;
                if(m_menor>=0)
                    m_bottom-=fontH;
            }
        }
        if(m_values)
        {
            if(m_mayor >0)
                m_top+=fontH+2;
            if(m_menor<0)
                m_bottom -=fontH+2;
        }
        if(m_usingTitle)
        {
            QFont title = m_letra;
            title.setItalic(true);
            title.setBold(true);
            title.setUnderline(true);
            painter.save();
            painter.setFont(title);
            m_top+=fontH;
            m_top+=3;
            painter.drawText(this->width()/2-painter.fontMetrics().width(m_title)/2,fontH,m_title);
            painter.restore();
        }
        m_heigth = m_bottom - m_top;

        int range = m_mayor;
        if(m_menor <0)
            range -= m_menor;

        if(range == 0)
            return;

        if(m_mayor == m_maxValue)
        {
            double pPerc = (m_mayor*100)/range;
            double p = pPerc/100.0;
            double pieceHeigth = m_heigth*p;
            m_xAxis = m_top + pieceHeigth;
        }
        else
        {
            double pPerc = (m_menor*100)/range;
            double p = pPerc/100.0;
            double pieceHeigth = m_heigth*p;
            m_xAxis = m_bottom + pieceHeigth;
        }
    }

//       painter.setPen(Qt::red);
//       painter.drawRect(m_left,m_top,m_width,m_heigth);
//       painter.drawLine(m_left,m_xAxis,m_rigth,m_xAxis);


    switch (m_type) {
    case Barras:
        if(m_valuesEnY)
            drawYValues(&painter);
        drawBar(&painter);
        drawAxis(&painter);
        break;
    case BarraMultiple:
        if(m_valuesEnY)
            drawYValues(&painter);
        drawMultiBar(&painter);
        drawAxis(&painter);
        break;
    case Lineas:
        if(m_valuesEnY)
            drawYValues(&painter);
        drawAxis(&painter);
        drawLines(&painter);
        break;
    case Sectores_2D:
        drawPie(&painter);
    default:
        break;
    }
}

void OpenChart::drawBar(QPainter *painter)
{
    painter->save();


    double pDist = 15;
    double pieceWidth = (m_width-(pieces.size())*pDist)/pieces.size();

    QPen pen;
    pen.setWidth(2);

    int range = m_mayor;
    if(m_menor <0)
        range -= m_menor;

    if(range == 0)
        return;

    for (int i=0;i<pieces.size();i++)
    {
        bool isPositive = pieces.at(i).value()>=0;

        double pPerc = (pieces.at(i).value()*100)/range;
        double p = pPerc/100.0;
        double pieceHeigth = m_heigth*p;
        if(pieceHeigth<0)
            pieceHeigth = -pieceHeigth;

        int pieceXPos = pDist+i*(pieceWidth + pDist);

        if (m_sombra)
        {
            painter->setPen(Qt::NoPen);
            painter->setBrush(Qt::darkGray);

            if(isPositive)
                painter->drawRect(m_left+pieceXPos-pDist/2,m_xAxis,pieceWidth,-pieceHeigth/*+pDist/2*/*0.8);
            else
                painter->drawRect(m_left+pieceXPos-pDist/2,m_xAxis,pieceWidth,pieceHeigth/*-pDist/2*/*0.8);
        }

        if(isPositive)
        {
            QLinearGradient gradient(m_left+m_width/2,m_xAxis-pieceHeigth-180,m_left+m_width/2,m_xAxis);
            gradient.setColorAt(0,Qt::black);
            gradient.setColorAt(1,pieces.at(i).color);
            painter->setBrush(gradient);
        }
        else
        {
            QLinearGradient gradientNeg(m_left+m_width/2,m_xAxis,m_left+m_width/2,m_xAxis+pieceHeigth+180);
            gradientNeg.setColorAt(1,Qt::black);
            gradientNeg.setColorAt(0,pieces.at(i).color);
            painter->setBrush(gradientNeg);
        }

        pen.setColor(pieces.at(i).color);
        painter->setPen(pen);

        if(isPositive)
            painter->drawRect(m_left+pieceXPos , m_xAxis , pieceWidth , -pieceHeigth);
        else
            painter->drawRect(m_left+pieceXPos , m_xAxis , pieceWidth , pieceHeigth);

        QString label = QString::number(pieces.at(i).value());

        painter->setPen(Qt::SolidLine);
        int fontHeigth = painter->fontMetrics().height();
        if(m_labels)
        {
            QString s = pieces.at(i).nombre;
            if(painter->fontMetrics().width(s)> pieceWidth)
            {
                s = "...";
                for (int a = 0; a<pieces.at(i).nombre.size();a++)
                {
                    if(painter->fontMetrics().width(s)>= pieceWidth-10)
                        break;
                    s.insert(a,pieces.at(i).nombre.at(a));
                }
            }
            if(isPositive)
                painter->drawText(m_left+pieceXPos+pieceWidth/2-painter->fontMetrics().width(s)/2,m_xAxis+fontHeigth,s);
            else
                painter->drawText(m_left+pieceXPos+pieceWidth/2-painter->fontMetrics().width(s)/2,m_xAxis-fontHeigth/2,s);
        }
        if(m_values)
        {
            if(isPositive)
                painter->drawText(m_left+pieceXPos+pieceWidth/2-painter->fontMetrics().width(label)/2,
                                  m_xAxis-pieceHeigth-fontHeigth/2+2,
                                  label);
            else
                painter->drawText(m_left+pieceXPos+pieceWidth/2-painter->fontMetrics().width(label)/2,
                                  m_xAxis+pieceHeigth+fontHeigth-1,
                                  label);
        }
    }
    painter->restore();
    if(m_useLeyenda)
        drawLeyendaVertical(painter);
}

void OpenChart::drawMultiBar(QPainter *painter)
{
    double pDist = 15;
    double iDist = 4;
    double pieceWidth = (m_width-(pieces.size())*pDist)/pieces.size();

    QPen pen;
    pen.setWidth(3);

    int range = m_mayor;
    if(m_menor <0)
        range -= m_menor;

    if(range == 0)
        return;

    for (int i=0;i<pieces.size();i++)
    {
        while(multibarColors.size() < pieces.at(i).getValues().size())
        {
            int r = qrand()%255;
            int g = qrand()%255;
            int b = qrand()%255;
            QPair<QString , QColor> par;
            par.first="";
            par.second = QColor(r,g,b);
            multibarColors.append(par);
        }
        double subWidth = (pieceWidth-((pieces.at(i).getValues().size()-1)*iDist))/pieces.at(i).getValues().size();

        for(int a=pieces.at(i).getValues().size()-1;a>-1;a--)
        {
            bool isPositive = pieces.at(i).getValues().at(a) >=0;

            double pPerc = (pieces.at(i).getValues().at(a)*100)/range;
            double p = pPerc/100.0;
            double pieceHeigth = m_heigth*p;
            if(pieceHeigth<0)
                pieceHeigth = -pieceHeigth;

            int pieceXPos = pDist+i*(pieceWidth + pDist) + (a*(iDist + subWidth));

            int blockXPos = pDist+i*(pieceWidth + pDist);

            if (m_sombra)
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush(Qt::darkGray);

                if(isPositive)
                    painter->drawRect(m_left+pieceXPos-pDist/2,m_xAxis,subWidth,-pieceHeigth/*+pDist/2*/*0.8);
                else
                    painter->drawRect(m_left+pieceXPos-pDist/2,m_xAxis,subWidth,pieceHeigth/*-pDist/2*/*0.8);
            }

            if(isPositive)
            {
                QLinearGradient gradient(m_left+m_width/2,m_xAxis-pieceHeigth-180,m_left+m_width/2,m_xAxis);
                gradient.setColorAt(0,Qt::black);
                gradient.setColorAt(1,multibarColors.at(a).second);
                painter->setBrush(gradient);
            }
            else
            {
                QLinearGradient gradientNeg(m_left+m_width/2,m_xAxis,m_left+m_width/2,m_xAxis+pieceHeigth+180);
                gradientNeg.setColorAt(1,Qt::black);
                gradientNeg.setColorAt(0,multibarColors.at(a).second);
                painter->setBrush(gradientNeg);
            }

            pen.setColor(multibarColors.at(a).second);
            painter->setPen(pen);

            if(isPositive)
                painter->drawRect(m_left+pieceXPos , m_xAxis , subWidth , -pieceHeigth);
            else
                painter->drawRect(m_left+pieceXPos , m_xAxis , subWidth , pieceHeigth);

            QString label = QString::number(pieces.at(i).getValues().at(a));

            painter->setPen(Qt::SolidLine);
            int fontHeigth = painter->fontMetrics().height();

            if(m_values)
            {
                if(isPositive)
                    painter->drawText(m_left+pieceXPos+subWidth/2-painter->fontMetrics().width(label)/2,
                                      m_xAxis-pieceHeigth-fontHeigth/2,
                                      label);
                else
                    painter->drawText(m_left+pieceXPos+subWidth/2-painter->fontMetrics().width(label)/2,
                                      m_xAxis+pieceHeigth+fontHeigth,
                                      label);
            }
            if(m_labels)
            {
                QString s = pieces.at(i).nombre;
                if(painter->fontMetrics().width(s)> pieceWidth)
                {
                    s = "...";
                    for (int a = 0; a<pieces.at(i).nombre.size();a++)
                    {
                        if(painter->fontMetrics().width(s)>= pieceWidth-10)
                            break;
                        s.insert(a,pieces.at(i).nombre.at(a));
                    }
                }
                if(m_mayor<=0)
                    painter->drawText(m_left+blockXPos+pieceWidth/2-painter->fontMetrics().width(s)/2,m_top-fontHeigth/2+3,s);
                else
                    painter->drawText(m_left+blockXPos+pieceWidth/2-painter->fontMetrics().width(s)/2,height()-fontHeigth/2+3,s);
            }
        }
    }
    if(m_useLeyenda)
        drawLeyendaVertical(painter);
}

void OpenChart::drawLines(QPainter *painter)
{
    int range = m_mayor;
    if(m_menor<0)
        range -= m_menor;

    if(range == 0)
        return;

    double pDist;
    int start = m_left+15;
    pDist = (m_width-start)/(lineasStops.size()-1);
    QVector<float> xPositions;
    for(int i=0;i<lineasStops.size();i++)
        xPositions.append(start+pDist*i);

    if(m_labels && !lineasStops.isEmpty())
    {
        xPositions.clear();
        start = m_left+15+painter->fontMetrics().width(lineasStops.at(0));
            pDist = (m_width-start-painter->fontMetrics().width(lineasStops.last()))/(lineasStops.size()-1);
            for(int i=0;i<lineasStops.size();i++)
                xPositions.append(start+pDist*i);
    }

    if(m_sombra)
    {
        painter->setPen(Qt::NoPen);
        for(int i=0;i<pieces.size();i++)
        {
            QColor c = pieces.at(i).color;
            c.setAlpha(5);

            QColor c1 = pieces.at(i).color;
            c1.setAlpha(50);


            int use = qMin(pieces.at(i).getValues().size(), lineasStops.size());
            int pointCount = use + 2;

            QVector<QPoint> points;
            points.reserve(pointCount);
            points.resize(pointCount);

            for (int a=0;a<pointCount-2;a++)
            {
                float _p = (pieces.at(i).getValues().at(a)*100)/range;
                if(_p < 0)
                    _p = -_p;

                double x = _p/100.0;
                double y = m_heigth*x;

                if(pieces.at(i).getValues().at(a)>=0)
                    points[a] = QPoint(xPositions.at(a),m_xAxis - y);
                else
                    points[a] = QPoint(xPositions.at(a),m_xAxis + y);
            }

            points[pointCount-2] = QPoint(xPositions.at(use-1),m_xAxis);
            points[pointCount-1] = QPoint(xPositions.at(0),m_xAxis);

            int _max=0;
            int _min=0;
            for (int a=0;a<pointCount-2;a++)
            {
                _max = qMax(_max,points[a].y());
                _min = qMin(_min,points[a].y());
            }
            QLinearGradient gradient(m_left,_max,m_left,_min);
            gradient.setColorAt(0,c);
            gradient.setColorAt(0.5,c1);
            gradient.setColorAt(1,c);
            painter->setBrush(gradient);

            painter->drawPolygon(points.data(),pointCount);
        }
    }
    if (m_labels)
    {
        painter->setPen(Qt::SolidLine);
        for(int i=0;i<lineasStops.size();i++)
        {
            if(m_mayor<=0)
                painter->drawText(xPositions.at(i)-painter->fontMetrics().width(lineasStops.at(i))/2,m_top-painter->fontMetrics().height()/2+3,lineasStops.at(i));
            else
                painter->drawText(xPositions.at(i)-painter->fontMetrics().width(lineasStops.at(i))/2,height()-painter->fontMetrics().height()/2+3,lineasStops.at(i));
        }
    }

    for(int i=0;i<pieces.size();i++)
    {
        int use = qMin(pieces.at(i).getValues().size(), lineasStops.size());
        int pointCount = use;

        //QPoint points[pointCount];
        QVector<QPoint> points;
        points.reserve(pointCount);
        points.resize(pointCount);

        QPen pen;
        pen.setColor(pieces.at(i).color);
        pen.setWidth(2);
        for (int a=0;a<pointCount;a++)
        {
            float _p = (pieces.at(i).getValues().at(a)*100)/range;
            if(_p < 0)
                _p = -_p;

            double x = _p/100.0;
            double y = m_heigth*x;

            if(pieces.at(i).getValues().at(a)>=0)
                points[a] = QPoint(xPositions.at(a),m_xAxis - y);
            else
                points[a] = QPoint(xPositions.at(a),m_xAxis + y);

        }
        painter->setPen(pen);
        painter->drawPolyline(points.data(),pointCount);
        QColor c = pieces.at(i).color;
        c.setAlpha(50);
        painter->setBrush(c);
        for (int a=0;a<pointCount;a++)
            painter->drawEllipse(points[a],3,3);

        if(m_values)
        {
            painter->setPen(Qt::SolidLine);
            for (int a=0;a<pointCount;a++)
            {
                QString s = QString::number(pieces.at(i).getValues().at(a));
                bool above = true;
                if(a!= pointCount-1)
                {
                    if(points[a].y()> points[a+1].y())
                        above = false;
                }
                if(/*points[a].y() <= m_top + m_xAxisPos*/above)
                {
                    painter->drawText(points[a].x()-painter->fontMetrics().width(s),points[a].y()-painter->fontMetrics().height()/2,s);
                }
                else
                {
                    painter->drawText(points[a].x()-painter->fontMetrics().width(s),points[a].y()+painter->fontMetrics().height(),s);
                }
            }
        }
    }
    if(m_useLeyenda)
        drawLeyendaVertical(painter);
}

void OpenChart::drawPie(QPainter *painter)
{
    if(m_useLeyenda)
    {
        if(m_tipoLeyenda == Vertical)
            drawLeyendaVertical(painter);
        else
            drawLeyendaCircular(painter);
    }
    painter->save();
    double pdegree = 0;
    double palpha = 0;
    double total = 0;
    for(int i=0;i<pieces.size();i++)
        total+=pieces.at(i).value();

    if (m_sombra)
    {
        painter->setBrush(Qt::darkGray);
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(m_left,m_top+5,m_width,m_heigth-5);
    }

    QPen pen;
    pen.setWidth(2);

    QRadialGradient gradient(QPoint(m_left+0.5*m_width,m_top+m_heigth*0.5),qMax(m_heigth,m_width));
    gradient.setColorAt(1,Qt::black);
    for (int i=0;i<pieces.size();i++)
    {
      pen.setColor(pieces.at(i).color);
      painter->setPen(pen);
      pdegree = 3.6*(pieces.at(i).value()*100/total);
      gradient.setColorAt(0,pieces.at(i).color);
      painter->setBrush(gradient);

      painter->drawPie(m_left,m_top,m_width,m_heigth-5,palpha*16,pdegree*16);

      palpha += pdegree;
    }
    painter->restore();
}

void OpenChart::drawAxis(QPainter *painter)
{
    painter->save();

    QPen pen;
    pen.setWidth(3);
    pen.setColor(Qt::darkGray);
    painter->setPen(pen);

    int top = 1;
    if(m_usingTitle)
        top+=painter->fontMetrics().height();

    painter->drawLine(m_left,top+2,m_left,this->height()-4);
    painter->drawLine(m_left,m_xAxis,m_left+m_width,m_xAxis);

    //painter->setPen(Qt::SolidLine);
    pen.setWidth(1);
    painter->setPen(pen);
    if(m_mayor >0 )
    {
        painter->drawLine(m_left,top,m_left+4,top+9);
        painter->drawLine(m_left,top,m_left-4,top+9);
    }
    if(m_menor < 0)
    {
        painter->drawLine(m_left,this->height()-1,m_left+4,this->height()-11);
        painter->drawLine(m_left,this->height()-1,m_left-4,this->height()-11);
    }

    painter->restore();
}

void OpenChart::drawLeyendaVertical(QPainter *painter)
{
    int dist = 5;
    if(m_type != BarraMultiple)
    {
        for (int i=pieces.size()-1;i>=0;i--)
        {
            painter->setBrush(pieces.at(i).color);
            float x = m_rigth+dist;
            float y = m_top+dist+i*(painter->fontMetrics().height()+2*dist);
            painter->drawRect(x,y,painter->fontMetrics().height(),painter->fontMetrics().height());
            QString s = pieces.at(i).nombre;
            if(m_type != Lineas)
                s.append(" : " + QString::number(pieces.at(i).value()));
            painter->drawText(x+painter->fontMetrics().height()+dist,y+painter->fontMetrics().height()/2+dist,s);
        }
    }
    else
    {
        for (int i = multibarColors.size()-1;i>=0;i--)
        {
            painter->setBrush(multibarColors.at(i).second);
            float x = m_rigth+dist;
            float y = m_top+dist+i*(painter->fontMetrics().height()+2*dist);
            painter->drawRect(x,y,painter->fontMetrics().height(),painter->fontMetrics().height());
            QString s = multibarColors.at(i).first;
            painter->drawText(x+painter->fontMetrics().height()+dist,y+painter->fontMetrics().height()/2+dist,s);
        }
    }
}

void OpenChart::drawLeyendaCircular(QPainter *painter)
{
    painter->save();
    int fontH = painter->fontMetrics().height();
    double pdegree = 0;
    double palpha = 0;
    double total = 0;
    for(int i=0;i<pieces.size();i++)
        total+=pieces.at(i).value();


    QVector<roundLabelPoints> top_labels;
    QVector<roundLabelPoints> rigth_labels;
    QVector<roundLabelPoints> bottom_labels;
    QVector<roundLabelPoints> left_labels;
    for (int i=0;i<pieces.size();i++)
    {
        QString s = QString("%1 : %2").arg(pieces.at(i).nombre).arg(pieces.at(i).value());
        int fontW = painter->fontMetrics().width(s);
        pdegree = 3.6*(pieces.at(i).value()*100/total);

        double pUse = palpha + pdegree/2;
        QPointF start = GetPoint(palpha + pdegree/2,m_width,m_heigth-5);
        QLineF line(QPoint(m_left+m_width/2,m_top+(m_heigth-5)/2),start);
        // painter->drawLine(line);

        painter->setPen(Qt::SolidLine);
        QPointF intersec;
        QLineF top_line (QPoint(0,m_top-10),QPoint(m_width,m_top-10));
        QLineF left_line (QPoint(m_left-10,0),QPoint(m_left-10,height()));
        QLineF bottom_line (QPoint(0,m_bottom+10),QPoint(m_width,m_bottom+10));
        QLineF rigth_line (QPoint(m_rigth+10,0),QPoint(m_rigth+10,height()));

        if(pUse >=45 && pUse <=135)
        {
            if(line.intersect(top_line,&intersec) == QLineF::UnboundedIntersection
                    || line.intersect(top_line,&intersec) == QLineF::BoundedIntersection)
            {
               /* bool rigth = intersec.x() >= width()/2;
                if(rigth)
                {
                    if(intersec.x()+fontW+14 > width())
                    {
                        line.intersect(rigth_line,&intersec);
                        roundLabelPoints r;
                        r.name = pieces.at(i).nombre;
                        r.start = start;
                        r.intersec = intersec;
                        rigth_labels.append(r);
                    }
                    else
                    {
                        roundLabelPoints r;
                        r.name = pieces.at(i).nombre;
                        r.start = start;
                        r.intersec = intersec;
                        top_labels.append(r);
                    }
                }
                else
                {
                    if(intersec.x()-fontW-14 < 0)
                    {
                        line.intersect(left_line,&intersec);
                        roundLabelPoints r;
                        r.name = pieces.at(i).nombre;
                        r.start = start;
                        r.intersec = intersec;
                        left_labels.append(r);
                    }
                    else
                    {
                        roundLabelPoints r;
                        r.name = pieces.at(i).nombre;
                        r.start = start;
                        r.intersec = intersec;
                        top_labels.append(r);
                    }
                }*/
                roundLabelPoints r;
                r.name = pieces.at(i).nombre;
                r.start = start;
                r.value = pieces.at(i).value();
                top_labels.append(r);
            }
        }
        else if(pUse >135 && pUse <=225)
        {
            if(line.intersect(left_line,&intersec) == QLineF::UnboundedIntersection
                    || line.intersect(left_line,&intersec) == QLineF::BoundedIntersection)
            {
              /*  if(intersec.y()-fontH/2 < 0)
                {
                    line.intersect(top_line,&intersec);
                    roundLabelPoints r;
                    r.name = pieces.at(i).nombre;
                    r.start = start;
                    r.intersec = intersec;
                    top_labels.append(r);
                }
                else if(intersec.y()+fontH+2 > height())
                {
                    line.intersect(bottom_line,&intersec);
                    roundLabelPoints r;
                    r.name = pieces.at(i).nombre;
                    r.start = start;
                    r.intersec = intersec;
                    bottom_labels.append(r);
                }
                else
                {*/
                    roundLabelPoints r;
                    r.name = pieces.at(i).nombre;
                    r.start = start;
                    r.intersec = intersec;
                    r.value = pieces.at(i).value();
                    left_labels.append(r);
                //}
            }
        }
        else if(pUse >225 && pUse <= 315)
        {

            if(line.intersect(bottom_line,&intersec) == QLineF::UnboundedIntersection
                    || line.intersect(bottom_line,&intersec) == QLineF::BoundedIntersection)
            {
               /* bool rigth = intersec.x() >= width()/2;
                if(rigth)
                {
                    if(intersec.x()+fontW+14 > width())
                    {
                        line.intersect(rigth_line,&intersec);
                        roundLabelPoints r;
                        r.name = pieces.at(i).nombre;
                        r.start = start;
                        r.intersec = intersec;
                        if(rigth_labels.isEmpty())
                            rigth_labels.append(r);
                        else
                            rigth_labels.insert(0,r);
                    }
                    else
                    {
                        roundLabelPoints r;
                        r.name = pieces.at(i).nombre;
                        r.start = start;
                        r.intersec = intersec;
                        rigth_labels.append(r);
                    }
                }
                else
                {
                    if(intersec.x()-fontW-14 < 0)
                    {
                        line.intersect(left_line,&intersec);
                        roundLabelPoints r;
                        r.name = pieces.at(i).nombre;
                        r.start = start;
                        r.intersec = intersec;
                        left_labels.append(r);
                    }
                    else
                    {
                        roundLabelPoints r;
                        r.name = pieces.at(i).nombre;
                        r.start = start;
                        r.intersec = intersec;
                        rigth_labels.append(r);
                    }
                }*/
                roundLabelPoints r;
                r.name = pieces.at(i).nombre;
                r.start = start;
                r.value = pieces.at(i).value();
                bottom_labels.append(r);
            }
        }
        else
        {
            if(line.intersect(rigth_line,&intersec) == QLineF::UnboundedIntersection
                    || line.intersect(rigth_line,&intersec) == QLineF::BoundedIntersection)
            {
               /* if(intersec.y()-fontH/2 < 0)
                {
                    line.intersect(top_line,&intersec);
                    roundLabelPoints r;
                    r.name = pieces.at(i).nombre;
                    r.start = start;
                    r.intersec = intersec;
                    top_labels.append(r);
                }
                else if(intersec.y()+fontH+2 > height())
                {
                    line.intersect(bottom_line,&intersec);
                    roundLabelPoints r;
                    r.name = pieces.at(i).nombre;
                    r.start = start;
                    r.intersec = intersec;
                    bottom_labels.append(r);
                }
                else
                {
                    roundLabelPoints r;
                    r.name = pieces.at(i).nombre;
                    r.start = start;
                    r.intersec = intersec;
                    rigth_labels.append(r);
                }*/
                roundLabelPoints r;
                r.name = pieces.at(i).nombre;
                r.start = start;
                r.intersec = intersec;
                r.value = pieces.at(i).value();
                if(pUse <=45)
                    rigth_labels.prepend(r);
                else if(rigth_labels.isEmpty())
                    rigth_labels.prepend(r);
                else if(rigth_labels.first().intersec.y() > intersec.y())
                    rigth_labels.prepend(r);
                else if(rigth_labels.last().intersec.y() < intersec.y())
                    rigth_labels.append(r);
                else
                {
                    for(int b=rigth_labels.size()-1;b>-1;b--)
                    {
                        if(rigth_labels.at(b).intersec.y() <= intersec.y())
                        {
                            rigth_labels.insert(b+1,r);
                            break;
                        }
                    }
                }
            }
        }
        palpha += pdegree;
    }
    painter->setBrush(Qt::white);
    if(!top_labels.isEmpty())
    {
        int top_size = (this->width()-10)/top_labels.size();
        int start = 5;
        int y = top_labels.at(0).intersec.y();
        for (int i=top_labels.size()-1;i>-1;i--)
        {
            QString s = QString("%1 : %2").arg(top_labels.at(i).name).arg(top_labels.at(i).value);
            int w = painter->fontMetrics().width(s)+6;
            painter->drawLine(top_labels.at(i).start,QPoint(start+top_size/2,m_top-fontH*1.5));
            painter->drawRect(start+top_size/2-w/2,m_top-fontH*2,w,fontH);
            painter->drawText(start,m_top-fontH*2,top_size,fontH,Qt::AlignCenter,s);
            start+= top_size;
        }
    }
    if(!rigth_labels.isEmpty())
    {
        int start = m_top - fontH;
        int stop = height()-5;

        if(top_labels.isEmpty())
            start-= fontH;
        if(!bottom_labels.isEmpty())
            stop-= fontH;
        int rigth_size = (stop-start)/rigth_labels.size();
        for (int i=0;i<rigth_labels.size();i++)
        {
            QString s = QString("%1 : %2").arg(rigth_labels.at(i).name).arg(rigth_labels.at(i).value);
            int w = painter->fontMetrics().width(s)+6;
            painter->drawLine(rigth_labels.at(i).start,QPoint(width()-w/2,start+rigth_size/2));
            painter->drawRect(width()-w-1,start-fontH/2+rigth_size/2,w,fontH);
            painter->drawText(width()-w+1,start-fontH/2+rigth_size/2,w,fontH,Qt::AlignCenter,s);
            start+= rigth_size;
        }
    }
    if(!left_labels.isEmpty())
    {
        int start = m_top - fontH;
        int stop = height()-5;

        if(top_labels.isEmpty())
            start-= fontH;
        if(!bottom_labels.isEmpty())
            stop-= fontH;
        int left_size = (stop-start)/left_labels.size();
        for (int i=0;i<left_labels.size();i++)
        {
            QString s = QString("%1 : %2").arg(left_labels.at(i).name).arg(left_labels.at(i).value);
            int w = painter->fontMetrics().width(s)+6;
            painter->drawLine(left_labels.at(i).start,QPoint(w/2,start+left_size/2));
            painter->drawRect(1,start-fontH/2+left_size/2,w,fontH);
            painter->drawText(1,start-fontH/2+left_size/2,w,fontH,Qt::AlignCenter,s);
            start+= left_size;
        }
    }
    if(!bottom_labels.isEmpty())
    {
        int bottom_size = (this->width()-10)/bottom_labels.size();
        int start = 5;
        int y = bottom_labels.at(0).intersec.y();
        for (int i=0;i<bottom_labels.size();i++)
        {
            QString s = QString("%1 : %2").arg(bottom_labels.at(i).name).arg(bottom_labels.at(i).value);
            int w = painter->fontMetrics().width(s)+6;
            painter->drawLine(bottom_labels.at(i).start,QPoint(start+bottom_size/2,m_bottom+fontH*1.5));
            painter->drawRect(start+bottom_size/2-w/2,m_bottom+fontH,w,fontH);
            painter->drawText(start,m_bottom+fontH,bottom_size,fontH,Qt::AlignCenter,s);
            start+= bottom_size;
        }
    }
    painter->restore();
}

void OpenChart::drawYValues(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::DotLine);
    painter->setFont(m_letra);

    QString s = QString::number(m_maxValue);
    s.prepend("-");
    int range = m_mayor;
    if(m_menor < 0)
        range-= m_menor;

    int w = painter->fontMetrics().width(s);
    int h = painter->fontMetrics().height();
    s= s.mid(1);

    painter->drawText(0,m_xAxis-h/2,w,h,Qt::AlignRight,"0");

    QString aux = s.left(1);
    int toDiv = aux.toInt();
    double porcion = m_maxValue/toDiv;
    porcion = (porcion>0)?porcion : -porcion;
    double pAux = porcion/range;
    double porcionHeigth = m_heigth*pAux;

    int i = m_xAxis - porcionHeigth;
    for(int a=0;a<m_mayor;a+= porcion)
    {
        if(a+porcion > m_mayor+5)
            break;
        painter->drawLine(m_left,i,m_left+m_width,i);
        painter->drawText(0,i-h/2,w,h,Qt::AlignRight,QString::number(a+porcion));
        i-=porcionHeigth;
    }
    i = m_xAxis + porcionHeigth;
    for(int a=0;a>m_menor;a-= porcion)
    {
        painter->drawLine(m_left,i,m_left+m_width,i);
        painter->drawText(0,i-h/2,w,h,Qt::AlignRight,QString::number(a-porcion));
        i+=porcionHeigth;
    }

    painter->restore();
}

QPointF OpenChart::GetPoint(double angle, double R1, double R2)
{
    if (R1 == 0 && R2 == 0)
    {
        R1 = m_width;
        R2 = m_heigth;
    }
    QPointF point;
    double x = R1/2*cos(angle*M_PI/180);
    x+=m_width/2+m_left;
    double y = -R2/2*sin(angle*M_PI/180);
    y+=m_heigth/2+m_top;
    point.setX(x);
    point.setY(y);
    return point;
}

void OpenChart::setPercent(float f)
{
    for(int i=0;i<pieces.size();i++)
        pieces[i].setPercent(f);
    this->repaint();
}

void OpenChart::startAnimation()
{
    if(m_animation)
    {
        QPropertyAnimation *ani = new QPropertyAnimation(this,"percent",this);
        connect(ani,SIGNAL(finished()),ani,SLOT(deleteLater()));
        ani->setStartValue(0);
        ani->setEndValue(100);
        ani->setDuration(m_aniDuration);
        ani->start();
    }
    else
        setPercent(100);
}
void OpenChart::Clear()
{
    pieces.clear();
    multibarColors.clear();lineasStops.clear();
    m_mayor =0;
    m_maxValue = 0;
    m_menor = 0;
}
