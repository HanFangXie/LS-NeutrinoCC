#include "ActionInitialization.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "SteppingAction.hh"


namespace B1 {

ActionInitialization::ActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::Build() const
{

    SetUserAction(new B1::PrimaryGeneratorAction());  // 正确转换
    
    // 先创建EventAction实例
    auto eventAction = new EventAction();

    // 注册所有Action（注意顺序!!!）
    SetUserAction(new RunAction);
    SetUserAction(eventAction);  // 注册EventAction
    SetUserAction(new PrimaryGeneratorAction());
    SetUserAction(new SteppingAction(eventAction)); // 传递实例指针

}

void ActionInitialization::BuildForMaster() const
{
    // Typically, this method is used in parallel runs. You can initialize actions for the master process.
    SetUserAction(new RunAction);
}

} // namespace B1
