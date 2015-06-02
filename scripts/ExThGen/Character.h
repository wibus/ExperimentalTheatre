#ifndef PROJECTNAME_CHARACTER
#define PROJECTNAME_CHARACTER

#include <Scaena/Play/Character.h>


class PROJECTNAMECharacter : public scaena::Character
{
public:
    PROJECTNAMECharacter();

    void enterStage() override;
    void beginStep(const scaena::StageTime &time) override;
    void draw(const std::shared_ptr<scaena::View> &view,
              const scaena::StageTime &time) override;
    void exitStage() override;


private:
};

#endif //PROJECTNAME_CHARACTER
