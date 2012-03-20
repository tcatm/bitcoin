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

  sidebar = new QVBoxLayout;
  sidebar->setContentsMargins(5, 5, 5, 5);
  this->setLayout(sidebar);

  toolbar = new QToolBar;
  toolbar->setOrientation(Qt::Vertical);
  toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  toolbar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
  toolbar->setObjectName("toolbar");

  QWidget *boxWalletLabel = new QWidget;
  boxWalletLabel->setLayout(new QVBoxLayout);
  boxWalletLabel->setProperty("class", "box");

  labelEncryptionIcon = new QLabel;
  labelEncryptionIcon->setProperty("class", "icon");

  QLabel *walletLabel = new QLabel(tr("My Wallet"));
  walletLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  walletLabel->setProperty("class", "section");

  boxWalletLabel->layout()->addWidget(walletLabel);

  balanceLabel = new QLabel(tr("12,345.12345678 BTC"));
  balanceLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  balanceLabel->setAlignment(Qt::AlignRight);
  balanceLabel->setObjectName("balance");

  boxWalletLabel->layout()->addWidget(balanceLabel);

  sidebar->addWidget(boxWalletLabel);
  sidebar->addWidget(toolbar);

  QWidget *iconBox = new QWidget;

  iconHolder = new QHBoxLayout;
  iconHolder->setSpacing(8);
  iconHolder->setContentsMargins(3, 3, 3, 3);

  iconBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  iconBox->setLayout(iconHolder);

  sidebar->addStretch(100);

  sidebar->addWidget(iconBox);
  iconHolder->addWidget(labelEncryptionIcon);
  //boxWalletLabel->layout()->addWidget(iconBox);
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

void Sidebar::createSidebarButton(QAction *action) {
    QToolButton *btn = new QToolButton;
    btn->setDefaultAction(action);
    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    toolbar->addWidget(btn);
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

void Sidebar::addStretch()
{  
  sidebar->addStretch();
}
