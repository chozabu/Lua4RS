#include <QIcon>
#include <QTranslator>

#include <retroshare/rsplugin.h>
#include <retroshare/rsversion.h>

#include "gui/Lua4RSConfig.h"
#include "Lua4RSNotify.h"
#include "Lua4RSPlugin.h"
#include "gui/Lua4RSWidget.h"
#include "interface/L4RInterface.h"
#include "Lua/LuaCore.h"
#include "service/p3Lua4RS.h"

#define LUA_ICON_LINK ":/images/lua_logo.png"

#if LUA_VERSION_NUM < 502
    #error "###############################################"
    #error "# You need at least Lua 5.2 for this plugin   #"
    #error "# See contrib/get_lua5.2.3.sh                 #"
    #error "###############################################"
#endif

extern "C" {
#ifdef WIN32
    __declspec(dllexport)
#endif
    void *RETROSHARE_PLUGIN_provide()
    {
        static Lua4RSPlugin *p = new Lua4RSPlugin() ;

        return (void*)p ;
    }

    // This symbol contains the svn revision number grabbed from the executable.
    // It will be tested by RS to load the plugin automatically, since it is safe to load plugins
    // with same revision numbers, assuming that the revision numbers are up-to-date.
    //
#ifdef WIN32
    __declspec(dllexport)
#endif
    uint32_t RETROSHARE_PLUGIN_revision = RS_REVISION_NUMBER;

    // This symbol contains the svn revision number grabbed from the executable.
    // It will be tested by RS to load the plugin automatically, since it is safe to load plugins
    // with same revision numbers, assuming that the revision numbers are up-to-date.
    //
#ifdef WIN32
    __declspec(dllexport)
#endif
    uint32_t RETROSHARE_PLUGIN_api = RS_PLUGIN_API_VERSION ;
}

Lua4RSPlugin::Lua4RSPlugin()
{
    _icon = NULL;
    _mainpage = NULL;
    _notify = NULL;
    _peers = NULL;
    _pluginHandler = NULL;
    _service = NULL;
}

void Lua4RSPlugin::stop()
{
    L4R::L4RConfig->getCore()->shutDown();
}

void Lua4RSPlugin::getPluginVersion(int &major, int &minor, int &build, int &svn_rev) const
{
    major = RS_MAJOR_VERSION;
    minor = RS_MINOR_VERSION;
    build = RS_BUILD_NUMBER;
    svn_rev = RS_REVISION_NUMBER;
}

void Lua4RSPlugin::setInterfaces(RsPlugInInterfaces &interfaces)
{
    // get stuff
    _peers = interfaces.mPeers;
    _notify = interfaces.mNotify;

    // setup other stuff - pqi service is nor running yet -> don't use interface pointer
    LuaCore* lc = dynamic_cast<p3Lua4RS*>(rs_pqi_service())->getCore();
    _notify->registerNotifyClient(lc->notify());
}

MainPage* Lua4RSPlugin::qt_page() const
{
    if(_mainpage == NULL) {
        _mainpage = new Lua4RSWidget();
        L4R::L4RConfig->getCore()->setUi(dynamic_cast<Lua4RSWidget*>(_mainpage));
    }

    return _mainpage ;
}

QIcon* Lua4RSPlugin::qt_icon() const
{
    if(_icon == NULL)
    {
        Q_INIT_RESOURCE(Lua4RS_images);
        _icon = new QIcon(LUA_ICON_LINK);
    }

    return _icon ;
}

QTranslator* Lua4RSPlugin::qt_translator(QApplication* /*app*/, const QString& languageCode, const QString& externalDir) const
{
    if (languageCode == "en") {
        return NULL;
    }

    QTranslator* translator = new QTranslator();

    if (translator->load(externalDir + "/Lua4RS_" + languageCode + ".qm")) {
        return translator;
    } else if (translator->load(":/lang/Lua4RS_" + languageCode + ".qm")) {
        return translator;
    }

    delete(translator);
    return NULL;
}

ConfigPage *Lua4RSPlugin::qt_config_page() const
{
    return new Lua4RSConfig();
}

RsPQIService *Lua4RSPlugin::rs_pqi_service() const
{
    if(_service == NULL)
    {
        _service = new p3Lua4RS(_pluginHandler);
        L4R::L4RConfig = _service;
    }

    return _service;
}

uint16_t Lua4RSPlugin::rs_service_id() const
{
    return RS_SERVICE_TYPE_L4R_PLUGIN;
}

void Lua4RSPlugin::setPlugInHandler(RsPluginHandler *pgHandler)
{
    _pluginHandler = pgHandler;
}

std::string Lua4RSPlugin::getShortPluginDescription() const
{
    return QObject::tr("This plugin let you script RS with Lua.").toStdString();
}

std::string Lua4RSPlugin::getPluginName() const
{
    return "Lua4RS";
}
