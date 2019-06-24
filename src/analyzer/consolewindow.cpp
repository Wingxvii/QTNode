#include "consolewindow.h"


ConsoleWindow::ConsoleWindow()
{
    window = new QWidget();
    consoleWindowLayout = new QGridLayout();
    window->setLayout(consoleWindowLayout);
    window->setVisible(false);

    consoleInput = new QLineEdit();
    consoleOutput = new QTextEdit(("<h3>JS Console </h3>"));

    consoleWindowLayout->addWidget(consoleOutput);
    consoleWindowLayout->addWidget(consoleInput);

    consoleInput->setAlignment(Qt::AlignLeft);
    consoleInput->setPlaceholderText("<< Instruction");
    consoleOutput->setReadOnly(true);

    connect(consoleInput, SIGNAL(returnPressed()), this,SLOT(consoleEnterSlot()) );


    builder = new JSBuilder();
    QJSValue scriptBuilder = engine.newQObject(builder);

    engine.globalObject().setProperty("x", scriptBuilder);

    openAction = new QAction(tr("&Console"), this);
    openAction->setStatusTip("Open/Close Console Window");
    openAction->setCheckable(true);

}

void ConsoleWindow::consoleEnterSlot()
{
    consoleOutput->append(QString("<font color='red'>></font> %1").arg(consoleInput->text()));
    auto result = engine.evaluate(consoleInput->text());
    consoleOutput->append(QString("<i>%1</i><br>").arg(result.toString()));
    consoleInput->clear();
}
