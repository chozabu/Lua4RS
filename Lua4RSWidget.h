#ifndef LUA4RSWIDGET_H
#define LUA4RSWIDGET_H

#include <retroshare-gui/mainpage.h>

#include <QWidget>

namespace Ui {
class Lua4RSWidget;
}

class LuaCore;

class Lua4RSWidget : public MainPage
{
  Q_OBJECT

  public:
    explicit Lua4RSWidget(QWidget *parent = 0);
    ~Lua4RSWidget();

// void clearOutput();
// void appendOutput(const std::string &s);
// void appendOutput(const QString &s);

  private:
    Ui::Lua4RSWidget *ui;
    LuaCore* _lua;

  private slots:
    void on_pb_run_clicked(); // void runLua();
};

#endif // LUA4RSMAINWIDGET_H