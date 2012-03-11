#ifndef SIDEBAR_H
#define SIDEBAR_H

#include "optionsmodel.h"
#include "bitcoinunits.h"
#include <QWidget>

QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE

class BitcoinGUI;
class WalletModel;
class QToolBar;
class QToolButton;
class QLabel;
class QHBoxLayout;

/* Sidebar widget 
 *
 * This widget will show general wallet information (balance) and a
 * list of "tabs" to switch between views. It will also show basic
 * blockchain statistics.
 * */
class Sidebar : public QWidget
{
    Q_OBJECT

public:
    explicit Sidebar(BitcoinGUI *gui);
    ~Sidebar();

    void paintEvent(QPaintEvent *pe);
    void setModel(WalletModel *model);
    void createSidebarButton(QAction *action);

    QToolBar *toolbar;
    QHBoxLayout *iconHolder;
    QLabel *labelEncryptionIcon;

public slots:
    void setBalance(qint64 balance, qint64 unconfirmedBalance);

private:
    QLabel *balanceLabel;
    WalletModel *model;
    qint64 currentBalance;
    qint64 currentUnconfirmedBalance;

private slots:
    void displayUnitChanged();
};

#endif // SIDEBAR_H
