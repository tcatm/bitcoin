#include "sidebar.h"
#include "bitcoingui.h"

#include <QVBoxLayout>
#include <QToolBar>
#include <QToolButton>
#include <QLabel>
#include <QFile>

Sidebar::Sidebar(BitcoinGUI *gui) :
  QWidget(gui)
{
  QVBoxLayout *sidebar = new QVBoxLayout;
  sidebar->setContentsMargins(0, 0, 0, 0);

  this->setLayout(sidebar);
  this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Ignored);
  this->setObjectName("sidebar");

  QFile sidebarQSS(":/stylesheets/sidebar");
  sidebarQSS.open(QIODevice::ReadOnly | QIODevice::Text);
  this->setStyleSheet(sidebarQSS.readAll());

  QToolBar *toolbar = new QToolBar;
  toolbar->setOrientation(Qt::Vertical);
  toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  toolbar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  toolbar->setObjectName("toolbar");

  createSidebarButtons(toolbar, gui);

  QLabel *balanceLabel = new QLabel(this);
  balanceLabel->setText("Balance...");
  balanceLabel->setAlignment(Qt::AlignLeft);
  balanceLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  sidebar->addWidget(balanceLabel);

  QLabel *balanceValue = new QLabel(this);
  balanceValue->setText("123.123 BTC");
  balanceValue->setAlignment(Qt::AlignRight);
  balanceValue->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  sidebar->addWidget(balanceValue);

  sidebar->addWidget(toolbar);
}

Sidebar::~Sidebar()
{
}

void Sidebar::setBalance(qint64 balance, qint64 unconfirmedBalance)
{
}

void Sidebar::displayUnitChanged()
{
}

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
