#include "qhotkeywidget.h"

qhotkeywidget::qhotkeywidget(QWidget *parent) :
    QWidget(parent)
{
    buttOK = new QPushButton(tr("OK"));
    buttDefault = new QPushButton(tr("Reset"));
    buttCancel = new QPushButton(tr("Cancel"));
    hotkeyEdit = new QHotkeyEdit(this);
    buttLayout = new QHBoxLayout;
    buttLayout->addWidget(buttOK);
    buttLayout->addWidget(buttDefault);
    buttLayout->addWidget(buttCancel);
    changehotkeyLayout = new QVBoxLayout;
    changehotkeyLayout->addWidget(hotkeyEdit);
    changehotkeyLayout->addLayout(buttLayout);
    setLayout(changehotkeyLayout);
    buttOK->setFocusPolicy(Qt::NoFocus);
    buttDefault->setFocusPolicy(Qt::NoFocus);
    buttCancel->setFocusPolicy(Qt::NoFocus);
    setWindowIcon(QIcon(QPixmap(":/res/hotkey.png")));

    connect(buttOK,SIGNAL(clicked()),this,SIGNAL(butOKClicked()));
    connect(buttCancel,SIGNAL(clicked()),this,SIGNAL(butCancelClicked()));
    connect(buttDefault,SIGNAL(clicked()),this,SLOT(buttDefaultClicked()));
    connect(hotkeyEdit,SIGNAL(needAccept()),this,SIGNAL(butOKClicked()));
    connect(hotkeyEdit,SIGNAL(needDecline()),this,SIGNAL(butCancelClicked()));

    setFixedSize(250,80);
}

void qhotkeywidget::loadItem(QTreeWidgetItem *item)
{
    hotkeyEdit->setMaxLength(20);
    hotkeyEdit->setText(item->text(1));
    defaulthotkey = item->whatsThis(2);
    changingItem = item;
}
void qhotkeywidget::buttDefaultClicked()
{
    hotkeyEdit->setMaxLength(defaulthotkey.size());
    hotkeyEdit->setText(defaulthotkey);
}

void qhotkeywidget::closeEvent(QCloseEvent *event)
{
    isChanging = false;
    event->accept();
}
