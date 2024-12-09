#include "PCH.h"

SKSEPluginLoad(const SKSE::LoadInterface* skse) {

    SKSE::Init(skse);

    YGGDRASIL::Init(YGGDRASIL::Manager::Configuration);
    YGGDRASIL::Init(YGGDRASIL::Manager::Log);

    return true;

};