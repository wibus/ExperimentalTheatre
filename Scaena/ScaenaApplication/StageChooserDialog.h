#ifndef SCAENA_STAGECHOOSERDIALOG_H
#define SCAENA_STAGECHOOSERDIALOG_H

#include <vector>
#include <QWidget>

#include "libScaena_global.h"


namespace scaena
{
    class SCAENA_EXPORT StageChooserDialog : public QWidget
    {
        Q_OBJECT

    public:
        StageChooserDialog();


        void        setChoices(const std::vector< std::string >& choices);
        void        askUser();

        const std::string userFinalChoice() const;
        bool        userQuit() const;


    private slots:
        void stageChosen(const QString& stageName);
        void okClicked();
        void quitClicked();


    private:
        std::vector< std::string > _choices;
        std::string _finalChoice;
        bool _userQuit;
    };



    // IMPLEMENTATION //
    inline const std::string StageChooserDialog::userFinalChoice() const
        {return _finalChoice;}

    inline bool StageChooserDialog::userQuit() const
        {return _userQuit;}
}

#endif // SCAENA_STAGECHOOSERDIALOG_H
