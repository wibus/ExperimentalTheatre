#include "StageChooserDialog.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>


namespace scaena
{
    StageChooserDialog::StageChooserDialog() :
        QWidget(),
        _choices(),
        _finalChoice(),
        _userQuit(true)
    {
    }

    void StageChooserDialog::setChoices(const std::vector< std::string >& choices)
    {
        _choices = choices;

        _finalChoice = choices[0];
    }

    void StageChooserDialog::askUser()
    {
        QLabel* lbHeader = new QLabel("Choose a stage");

        QComboBox* cbChoices = new QComboBox;
        connect(cbChoices, SIGNAL(editTextChanged(QString)),
                this,       SLOT(stageChosen(QString)));
        for(unsigned int i=0; i < _choices.size(); ++i)
            cbChoices->addItem(_choices[i].c_str());


        QPushButton* btOk = new QPushButton();
        btOk->setText("OK");
        connect(btOk, SIGNAL( clicked() ), this, SLOT( okClicked() ));

        QPushButton* btQuit = new QPushButton();
        btQuit->setText("Quit");
        connect(btQuit, SIGNAL( clicked() ), this, SLOT( quitClicked() ));


        // Set layouts
        QHBoxLayout* btLayout = new QHBoxLayout();
        btLayout->addWidget(btQuit);
        btLayout->addWidget(btOk);

        QVBoxLayout* dLayout = new QVBoxLayout();
        dLayout->setAlignment(Qt::AlignHCenter);
        dLayout->addWidget(lbHeader);
        dLayout->addWidget(cbChoices);
        dLayout->addLayout(btLayout);

        setLayout(dLayout);


        // Show Dialog
        QRect frect = frameGeometry();
        frect.moveCenter(QDesktopWidget().availableGeometry().center());
        move(frect.topLeft());
        show();

        QApplication::exec();
    }


    void StageChooserDialog::stageChosen(const QString& stageName)
    {
        _finalChoice = stageName.toStdString();
    }

    void StageChooserDialog::okClicked()
    {
        _userQuit = false;
        close();
    }

    void StageChooserDialog::quitClicked()
    {
        _userQuit = true;
        close();
    }
}
