#include "sidebar.h"
#include "walletmodel.h"
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

  QLabel *walletLabel = new QLabel(tr("MY WALLET"));
  walletLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  QLabel *toolbarLabel = new QLabel(tr("VIEWS"));
  toolbarLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  balanceLabel = new QLabel(tr("12,345.12345678 BTC"));
  balanceLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  balanceLabel->setAlignment(Qt::AlignRight);
  balanceLabel->setObjectName("balance");

  sidebar->addWidget(walletLabel);
  sidebar->addWidget(balanceLabel);
  sidebar->addWidget(toolbarLabel);
  sidebar->addWidget(toolbar);

  QWidget *iconBox = new QWidget;

  iconHolder = new QHBoxLayout;
  iconHolder->setSpacing(0);
  iconHolder->setContentsMargins(0, 4, 0, 4);

  iconBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
  iconBox->setLayout(iconHolder);

  sidebar->addWidget(iconBox);
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

void Sidebar::setBalance(qint64 balance, qint64 unconfirmedBalance)
{
    int unit = model->getOptionsModel()->getDisplayUnit();
    currentBalance = balance;
    currentUnconfirmedBalance = unconfirmedBalance;
    balanceLabel->setText(BitcoinUnits::formatWithUnit(unit, balance));
    //    labelUnconfirmed->setText(BitcoinUnits::formatWithUnit(unit, unconfirmedBalance));
}

void Sidebar::setModel(WalletModel *model)
{
    this->model = model;
    if(model)
    {
        // Keep up to date with wallet
        setBalance(model->getBalance(), model->getUnconfirmedBalance());
        connect(model, SIGNAL(balanceChanged(qint64, qint64)), this, SLOT(setBalance(qint64, qint64)));

        connect(model->getOptionsModel(), SIGNAL(displayUnitChanged(int)), this, SLOT(displayUnitChanged()));
    }
}

void Sidebar::displayUnitChanged()
{
    if(!model || !model->getOptionsModel())
        return;
    if(currentBalance != -1)
        setBalance(currentBalance, currentUnconfirmedBalance);
}
