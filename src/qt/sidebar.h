#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE

class BitcoinGUI;
class WalletModel;
class TxViewDelegate;
class QToolBar;
class QToolButton;

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

    void setModel(WalletModel *model);
    QToolButton *createSidebarButton(QAction *action);
    void createSidebarButtons(QToolBar *toolbar, BitcoinGUI *gui);

public slots:
    void setBalance(qint64 balance, qint64 unconfirmedBalance);

private:
    WalletModel *model;
    qint64 currentBalance;
    qint64 currentUnconfirmedBalance;

private slots:
    void displayUnitChanged();
};

#endif // SIDEBAR_H
