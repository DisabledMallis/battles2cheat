#include "hook_manager.h"
#include <iostream>

#include "opengl/glhook.h"
#include "../player_info.h"
#include "game_instance/ginsthook.h"
#include "balances_hook/balances_hook.h"
#include "encrypt/setfloathook.h"
#include "event/AddEventFieldHook.h"
#include "event/logevent.h"
#include "event/LogHeroChangeHook.h"
#include "event/LogTowerChangeHook.h"
#include "event/LogTowerEventHook.h"
#include "packet/SerializeNetworkMessageHook.h"
#include "packet/DeserializeNetworkMessageHook.h"
#include "redshift/formdatahook.h"
#include "redshift/redshift_hook.h"
#include "gamehud/hudhook.h"
#include "gamehud/hud_destruct.h"
#include "gamescreen/gscreen_load.h"
#include "network_queue/send_event_hook.h"
#include "NKModuleStorage/SendTransactionsHook.h"
#include "module_user/send_request_hook.h"
#include "steamapi/gethsteamuser_hook.h"
#include "steamapi/getuseridhook.h"
#include "network_queue/send_request_hook.h"
#include "network_queue/get_response_hook.h"
#include "NetworkEvent/SNetRequestQueueBloonSetEventHook.h"
#include "unlocks/istowerunlockedhook.h"
#include "unlocks/isupgradeunlockedhook.h"
//#include "wndproc/wndhook.h"
#include "lobbyscreen/lscreen_load.h"
#include "user_instance/getdeviceidhook.h"
#include "user_instance/getuseridhook.h"
#include "user_instance/userhook.h"

void hook_manager::apply_all()
{
    hook_manager::apply_hook(new balances_hook());
    hook_manager::apply_hook(new setFloatHook());
    hook_manager::apply_hook(new AddEventFieldHook());
    hook_manager::apply_hook(new LogEventHook());
    //hook_manager::apply_hook(new LogHeroChangeHook());
    //hook_manager::apply_hook(new LogTowerChangeHook());
    //hook_manager::apply_hook(new LogTowerEventHook());
    hook_manager::apply_hook(new glhook());
    //hook_manager::apply_hook(new wndhook());
    //hook_manager::apply_hook(new ginsthook());
    hook_manager::apply_hook(new Steam::getuseridhook());
    hook_manager::apply_hook(new userhook());
    hook_manager::apply_hook(new SerializeNetworkMessageHook());
    hook_manager::apply_hook(new DeserializeNetworkMessageHook());
    //hook_manager::apply_hook(new formdatahook());
    //hook_manager::apply_hook(new redshift_hook());
    hook_manager::apply_hook(new hudhook());
    hook_manager::apply_hook(new hud_destructor());
    //hook_manager::apply_hook(new gscreen_load());
    hook_manager::apply_hook(new sendevent_hook());
    hook_manager::apply_hook(new SendTransactionsHook());
    //hook_manager::apply_hook(new module_user::sendrequest_hook());
    //hook_manager::apply_hook(new gethsteamuser_hook());
    hook_manager::apply_hook(new NK::getdeviceidhook());
    hook_manager::apply_hook(new NK::getuseridhook());
    hook_manager::apply_hook(new network_queue::sendrequest_hook());
    hook_manager::apply_hook(new network_queue::getresponse_hook());
    hook_manager::apply_hook(new SNetRequestQueueBloonSetEventHook());
    hook_manager::apply_hook(new istowerunlockedhook());
    hook_manager::apply_hook(new isupgradeunlockedhook());
    hook_manager::apply_hook(new lscreen_load());
}

void hook_manager::apply_hook(hook* to_add)
{
    ilog("Applying %s hook", to_add->get_name().c_str());
    PLH::Log::registerLogger(std::shared_ptr<PLH::ErrorLog>(new PLH::ErrorLog()));
    if(to_add->apply())
    {
        hook_manager::hooks->push_back(to_add);
        ilog("Successfully applied %s patch!", to_add->get_name().c_str());
    }
    else
    {
        ilog("Failed to apply %s patch!", to_add->get_name().c_str());
	}
};