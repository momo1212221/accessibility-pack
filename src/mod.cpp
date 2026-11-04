#include <algorithm>
#include <cstdint>
#include "openrac/mod.hpp"
#include "logic.hpp"

namespace {
openrac::Mod* g=nullptr;
bool cross_on=true,circle_on=false,square_on=false,skip_scenes=false;
int interval=6;
void assist(std::uint8_t* report,std::uint8_t bit,bool enabled,std::uint32_t tick){
    if(!enabled) return;
    // L2/R2/L1/R1 are the low nibble of the report's second active-low button byte.
    // Leave shoulder chords untouched so this mod composes with QoL shortcuts from other mods.
    if((report[1] & 0x0fu) != 0x0fu) return;
    const bool physically_held=(report[1]&bit)==0;
    if(physically_held && qol::release_tick(tick,interval)) report[1]=static_cast<std::uint8_t>(report[1]|bit);
}
}
void load(openrac::Mod& mod){
    if(mod.game()!="rac1") throw openrac::Error("Accessibility Pack is for RAC1");
    g=&mod;
    cross_on=mod.setting<bool>("turbo_cross"); circle_on=mod.setting<bool>("turbo_circle");
    square_on=mod.setting<bool>("turbo_square"); skip_scenes=mod.setting<bool>("skip_cutscenes");
    interval=std::clamp(static_cast<int>(mod.setting<std::int64_t>("turbo_interval")),2,20);
    mod.on_pad([](int port,std::uint8_t* report,int size){
        if(port!=0 || report==nullptr || size<2) return;
        const auto tick=g->input().raw().tick;
        assist(report,0x40,cross_on,tick);
        assist(report,0x20,circle_on,tick);
        assist(report,0x80,square_on,tick);
    });
    mod.on_frame([]{
        if(!skip_scenes || !g->has_scenes()) return;
        int frame=0;
        if(g->scene_playing(&frame)>=0 && frame>=60) g->play_skip();
    });
    mod.info("hold-to-mash accessibility enabled");
}
OPENRAC_MOD_CPP("accessibility-pack","0.1.0",load)
