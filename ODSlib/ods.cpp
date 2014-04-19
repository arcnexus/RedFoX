#include "ods.h"

#include <QtSql>
#include <QtXml>

#include "../quazip/quazip.h"
#include "../quazip/quazipfile.h"

QString ODS::lastError = "";
ODS::ODS()
{
}

bool ODS::SqlToODS(QString fileName, QString query, QSqlDatabase db, QStringList headers)
{
    QSqlQuery q(db);

    if(q.exec(query))
    {
        QFile f(qApp->applicationDirPath()+"/content.xml");
        if(!f.open(QFile::WriteOnly))
        {
            ODS::lastError = "Content.xml:: open";
            return false;
        }
        QTextStream out(&f);
        const int Indent = 4;
        QDomDocument doc;

        QDomElement root = doc.createElement("office:document-content");
        root.setAttribute("xmlns:office","urn:oasis:names:tc:opendocument:xmlns:office:1.0");
        root.setAttribute("xmlns:meta","urn:oasis:names:tc:opendocument:xmlns:meta:1.0");
        root.setAttribute("xmlns:config","urn:oasis:names:tc:opendocument:xmlns:config:1.0");
        root.setAttribute("xmlns:text","urn:oasis:names:tc:opendocument:xmlns:text:1.0");
        root.setAttribute("xmlns:table","urn:oasis:names:tc:opendocument:xmlns:table:1.0");
        root.setAttribute("xmlns:draw","urn:oasis:names:tc:opendocument:xmlns:drawing:1.0" );
        root.setAttribute("xmlns:presentation","urn:oasis:names:tc:opendocument:xmlns:presentation:1.0");
        root.setAttribute("xmlns:dr3d","urn:oasis:names:tc:opendocument:xmlns:dr3d:1.0");
        root.setAttribute("xmlns:chart","urn:oasis:names:tc:opendocument:xmlns:chart:1.0");
        root.setAttribute("xmlns:form","urn:oasis:names:tc:opendocument:xmlns:form:1.0");
        root.setAttribute("xmlns:script","urn:oasis:names:tc:opendocument:xmlns:script:1.0");
        root.setAttribute("xmlns:style","urn:oasis:names:tc:opendocument:xmlns:style:1.0");
        root.setAttribute("xmlns:number","urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0");
        root.setAttribute("xmlns:math","http://www.w3.org/1998/Math/MathML");
        root.setAttribute("xmlns:svg","urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0");
        root.setAttribute("xmlns:fo","urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0");
        root.setAttribute("xmlns:anim","urn:oasis:names:tc:opendocument:xmlns:animation:1.0");
        root.setAttribute("xmlns:smil","urn:oasis:names:tc:opendocument:xmlns:smil-compatible:1.0");
        root.setAttribute("xmlns:officeooo","http://openoffice.org/2009/office");
        root.setAttribute("xmlns:delta","http://www.deltaxml.com/ns/track-changes/delta-namespace");
        root.setAttribute("xmlns:split","http://www.deltaxml.com/ns/track-changes/split-namespace");
        root.setAttribute("xmlns:ac","http://www.deltaxml.com/ns/track-changes/attribute-change-namespace");
        root.setAttribute("office:version","1.2");
        root.setAttribute("xmlns:dc","http://purl.org/dc/elements/1.1/");
        root.setAttribute("xmlns:xlink","http://www.w3.org/1999/xlink");

        doc.appendChild(root);

        QDomElement office_styles = doc.createElement("office:automatic-styles");

        QDomElement _style = doc.createElement("style:style");
        _style.setAttribute("style:name","ta1");
        _style.setAttribute("style:family","table");
        _style.setAttribute("style:master-page-name","Standard1");
        QDomElement style_table = doc.createElement("style:table-properties");
        style_table.setAttribute("table:display","true");

        QDomElement style_style = doc.createElement("style:style");
        style_style.setAttribute("style:name","ce1");
        style_style.setAttribute("style:family","table-cell");

        _style.appendChild(style_table);
        _style.appendChild(style_style);
        office_styles.appendChild(_style);
        root.appendChild(office_styles);

        QDomElement office_body = doc.createElement("office:body");
        QDomElement office_spreadsheet = doc.createElement("office:spreadsheet");
        QDomElement table_table = doc.createElement("table:table");
        table_table.setAttribute("table:name","Hoja1");
        table_table.setAttribute("table:style-name","ta1");

        QDomElement table_table_columnA = doc.createElement("table:table-column");
        table_table_columnA.setAttribute("table:default-cell-style-name","ce1");

        QDomElement table_table_columnB = doc.createElement("table:table-column");
        table_table_columnB.setAttribute("table:number-columns-repeated","2");



        root.appendChild(office_body);
        office_body.appendChild(office_spreadsheet);
        office_spreadsheet.appendChild(table_table);
        table_table.appendChild(table_table_columnA);
        table_table.appendChild(table_table_columnB);

        while(q.next())
        {
            QSqlRecord r = q.record();
            static bool do_headers = true;
            if(do_headers)
            {
                QDomElement table_table_row = doc.createElement("table:table-row");
                table_table.appendChild(table_table_row);

                if(!headers.isEmpty())
                {
                    for(int c = 0; c< headers.size(); c++)
                    {
                        QString value = headers.at(c);

                        QDomElement table_table_cell = doc.createElement("table:table-cell");
                        table_table_cell.setAttribute("office:value-type","string");
                        table_table_cell.setAttribute("office:string-value",value);

                        QDomElement text_p = doc.createElement("text:p");
                        QDomText txt = doc.createTextNode(value);
                        text_p.appendChild(txt);
                        table_table_cell.appendChild(text_p);

                        table_table_row.appendChild(table_table_cell);
                    }
                }
                else
                {
                    for(int c = 0; c< r.count(); c++)
                    {
                        QString value = r.fieldName(c);

                        QDomElement table_table_cell = doc.createElement("table:table-cell");
                        table_table_cell.setAttribute("office:value-type","string");
                        table_table_cell.setAttribute("office:string-value",value);

                        QDomElement text_p = doc.createElement("text:p");
                        QDomText txt = doc.createTextNode(value);
                        text_p.appendChild(txt);
                        table_table_cell.appendChild(text_p);

                        table_table_row.appendChild(table_table_cell);
                    }
                }
                do_headers = false;
            }

            QDomElement table_table_row = doc.createElement("table:table-row");
            table_table.appendChild(table_table_row);

            for(int c = 0; c< r.count(); c++)
            {
                QString value = r.value(c).toString();

                QDomElement table_table_cell = doc.createElement("table:table-cell");
                table_table_cell.setAttribute("office:value-type","string");
                table_table_cell.setAttribute("office:string-value",value);

                QDomElement text_p = doc.createElement("text:p");
                QDomText txt = doc.createTextNode(value);
                text_p.appendChild(txt);
                table_table_cell.appendChild(text_p);

                table_table_row.appendChild(table_table_cell);
            }
        }

        QDomNode xmlNode = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
        doc.insertBefore(xmlNode, doc.firstChild());
        out.setCodec("UTF-8");
        doc.save(out, Indent);
        f.close();

        QString content = qApp->applicationDirPath()+"/content.xml";

        QuaZip zip(fileName);
        zip.setFileNameCodec("IBM866");

        if (!zip.open(QuaZip::mdCreate)) {
            ODS::lastError =  QString("Zip open: %1").arg(zip.getZipError());
            return false;
        }

        QuaZipFile outFile(&zip);
        QFile inFile;

        QStringList _root_list;
        _root_list << content <<  ":/ODS/Auxiliares/QuaZIP/ODS/meta.xml" << ":/ODS/Auxiliares/QuaZIP/ODS/mimetype" << ":/ODS/Auxiliares/QuaZIP/ODS/settings.xml" << ":/ODS/Auxiliares/QuaZIP/ODS/styles.xml";
        _root_list << ":/ODS/Auxiliares/QuaZIP/ODS/META-INF/manifest.xml";
        _root_list << ":/ODS/Auxiliares/QuaZIP/ODS/Thumbnails/thumbnail.png";

        QFileInfoList _root_files;
        foreach (QString fn, _root_list) _root_files << QFileInfo(fn);

        char c;
        foreach(QFileInfo fileInfo, _root_files)
        {
            QString fileNameWithRelativePath;
            if(fileInfo.filePath() == content)
               fileNameWithRelativePath = fileInfo.fileName();
            else
                fileNameWithRelativePath= fileInfo.filePath().replace(":/ODS/Auxiliares/QuaZIP/ODS/","");

            inFile.setFileName(fileInfo.filePath());

            if (!inFile.open(QIODevice::ReadOnly)) {
                ODS::lastError =  QString("inFile open: %1").arg(inFile.errorString().toLocal8Bit().constData());
                return false;
            }

            if (!outFile.open(QIODevice::WriteOnly, QuaZipNewInfo(fileNameWithRelativePath, fileInfo.filePath()))) {
                ODS::lastError = QString("outFile open: %1").arg(outFile.getZipError());
                return false;
            }

            while (inFile.getChar(&c) && outFile.putChar(c));

            if (outFile.getZipError() != UNZ_OK) {
                ODS::lastError = QString("outFile putChar: %1").arg(outFile.getZipError());
                return false;
            }

            outFile.close();

            if (outFile.getZipError() != UNZ_OK) {
                ODS::lastError = QString("outFile close: %1").arg(outFile.getZipError());
                return false;
            }

            inFile.close();
        }

        zip.close();

        if (zip.getZipError() != 0) {
            ODS::lastError = QString("Zip close: %1").arg(zip.getZipError());
            return false;
        }

        f.remove();

        return true;
    }
    else
    {
        ODS::lastError = q.lastError().text();
        return false;
    }
}

bool ODS::ODStoTable(QString fileName, QAbstractItemModel *model)
{
    QuaZip zip(fileName);
    if(!zip.open(QuaZip::mdUnzip))
    {
        ODS::lastError = tr("Error abriendo el archivo %1").arg(fileName);
        return false;
    }

    QuaZipFile file(&zip);
    for(bool f = zip.goToFirstFile(); f; f = zip.goToNextFile())
    {
        if(file.getActualFileName() == "content.xml")
            break;
    }

    if(file.getActualFileName() != "content.xml")
    {
        ODS::lastError = tr("No se encontró el contenido");
        zip.close();
        return false;
    }

    if(!file.open(QIODevice::ReadOnly))
    {
        ODS::lastError = tr("No se pudo abrir contenido");
        zip.close();
        return false;
    }

    QByteArray data = file.readAll();
    file.close();
    zip.close();

    QDomDocument doc;
    if(!doc.setContent(data, &ODS::lastError))
        return false;

    QFile f("/home/marco/asd.xml");
    f.open(QFile::WriteOnly);
    QTextStream t(&f);
    doc.save(t,4);

    QDomNode root = doc.firstChild().nextSibling();
    if(root.isNull() || root.toElement().tagName() != "office:document-content")
    {
        ODS::lastError = tr("No es un documento office válido :: office:document-content");
        return false;
    }

    QDomNode office_body = root.namedItem("office:body");
    if(office_body.isNull())
    {
        ODS::lastError = tr("No es un documento office válido :: office:body");
        return false;
    }

    QDomNode office_spreadsheet = office_body.namedItem("office:spreadsheet");
    if(office_spreadsheet.isNull())
    {
        ODS::lastError = tr("No es un documento office válido :: office:spreadsheet");
        return false;
    }

    QDomNode table_table = office_spreadsheet.namedItem("table:table");
    if(table_table.isNull())
    {
        ODS::lastError = tr("No es un documento office válido :: table:table");
        return false;
    }

    QDomNode table_table_column = table_table.namedItem("table:table-column");
    if(table_table_column.isNull())
    {
        ODS::lastError = tr("No es un documento office válido :: table:table-column");
        return false;
    }

    int total_columns = table_table_column.toElement().attribute("table:number-columns-repeated").toInt();

    QDomNode table_table_row = table_table.namedItem("table:table-row");
    if(table_table_row.isNull())
    {
        ODS::lastError = tr("No es un documento office válido :: table:table-row");
        return false;
    }

    while(model->columnCount() < total_columns)
    {
        if(!model->insertColumn(model->columnCount()))
        {
            ODS::lastError = tr("Error escribiendo columnas");
            return false;
        }
    }

    while(!table_table_row.isNull())
    {
        if(table_table_row.toElement().tagName() == "table:table-row")
        {
            int row = model->rowCount();
            int column = 0;

            if(!model->insertRow(row))
            {
                ODS::lastError = tr("Error escribiendo filas");
                return false;
            }

            QDomNode table_cell = table_table_row.namedItem("table:table-cell");
            while(!table_cell.isNull())
            {
                QString content = "";
                QDomNode text_p = table_cell.firstChildElement("text:p");
                if(!text_p.isNull())
                {
                    content = text_p.toElement().text();
                    model->setData(model->index(row,column),content);
                    ++column;
                }
                table_cell = table_cell.nextSiblingElement("table:table-cell");
            }
        }
        table_table_row = table_table_row.nextSiblingElement("table:table-row");
    }
    return true;
}
