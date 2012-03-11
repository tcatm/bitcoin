#include "sidebar.h"
#include "bitcoingui.h"

#include <QVBoxLayout>
#include <QToolBar>
#include <QToolButton>
#include <QLabel>
#include <QFile>
#include <QPainter>
#include <QStyle>
#include <QStyleOption>

Sidebar::Sidebar(BitcoinGUI *gui) :
  QWidget(gui)
{
  this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Ignored);
  this->setObjectName("sidebar");

  QFile sidebarQSS(":/stylesheets/sidebar");
  sidebarQSS.open(QIODevice::ReadOnly | QIODevice::Text);
  this->setStyleSheet(sidebarQSS.readAll());

  QVBoxLayout *sidebar = new QVBoxLayout;
  sidebar->setContentsMargins(0, 0, 0, 0);
  this->setLayout(sidebar);

  QToolBar *toolbar = new QToolBar;
  toolbar->setOrientation(Qt::Vertical);
  toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  toolbar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
  toolbar->setObjectName("toolbar");

  createSidebarButtons(toolbar, gui);

  QLabel *toolbarLabel = new QLabel(tr("MY WALLET"));
  toolbarLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  sidebar->addWidget(toolbarLabel);
  sidebar->addWidget(toolbar);
}

Sidebar::~Sidebar()
{
}

void Sidebar::paintEvent(QPaintEvent *pe) {
  QStyleOption o;
  o.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
};

QToolButton *Sidebar::createSidebarButton(QAction *action) {
    QToolButton *btn = new QToolButton;
    btn->setDefaultAction(action);
    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    return btn;
}

void Sidebar::createSidebarButtons(QToolBar *toolbar, BitcoinGUI *gui)
{
  toolbar->addWidget(createSidebarButton(gui->overviewAction));
  toolbar->addWidget(createSidebarButton(gui->sendCoinsAction));
  toolbar->addWidget(createSidebarButton(gui->receiveCoinsAction));
  toolbar->addWidget(createSidebarButton(gui->historyAction));
  toolbar->addWidget(createSidebarButton(gui->addressBookAction));
#ifdef FIRST_CLASS_MESSAGING
  toolbar->addWidget(createSidebarButton(gui->messageAction));
#endif
}
