#include "blinkink.h"
#include "blinkinkplugin.h"

#include <QtPlugin>

BlinkinkPlugin::BlinkinkPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void BlinkinkPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;
    
    // Add extension registrations, etc. here
    
    m_initialized = true;
}

bool BlinkinkPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *BlinkinkPlugin::createWidget(QWidget *parent)
{
    return new Blinkink(parent);
}

QString BlinkinkPlugin::name() const
{
    return QLatin1String("Blinkink");
}

QString BlinkinkPlugin::group() const
{
    return QLatin1String("Display widgets");
}

QIcon BlinkinkPlugin::icon() const
{
    return QIcon();
}

QString BlinkinkPlugin::toolTip() const
{
    return QLatin1String("");
}

QString BlinkinkPlugin::whatsThis() const
{
    return QLatin1String("A label with color that blink");
}

bool BlinkinkPlugin::isContainer() const
{
    return false;
}

QString BlinkinkPlugin::domXml() const
{
    return QLatin1String("<widget class=\"Blinkink\" name=\"blinkink\">\n</widget>\n");
}

QString BlinkinkPlugin::includeFile() const
{
    return QLatin1String("blinkink.h");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(blinkinkplugin, BlinkinkPlugin)
#endif // QT_VERSION < 0x050000
