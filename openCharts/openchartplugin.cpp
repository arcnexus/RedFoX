#include "openchart.h"
#include "openchartplugin.h"

#include <QtPlugin>

OpenChartPlugin::OpenChartPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void OpenChartPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;
    
    // Add extension registrations, etc. here
    
    m_initialized = true;
}

bool OpenChartPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *OpenChartPlugin::createWidget(QWidget *parent)
{
    return new OpenChart(parent);
}

QString OpenChartPlugin::name() const
{
    return QLatin1String("OpenChart");
}

QString OpenChartPlugin::group() const
{
    return QLatin1String("");
}

QIcon OpenChartPlugin::icon() const
{
    return QIcon();
}

QString OpenChartPlugin::toolTip() const
{
    return QLatin1String("");
}

QString OpenChartPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool OpenChartPlugin::isContainer() const
{
    return false;
}

QString OpenChartPlugin::domXml() const
{
    return QLatin1String("<widget class=\"OpenChart\" name=\"openChart\">\n</widget>\n");
}

QString OpenChartPlugin::includeFile() const
{
    return QLatin1String("openchart.h");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(openchartplugin, OpenChartPlugin)
#endif // QT_VERSION < 0x050000
