/*
 *  Module for AzerothCore by Gozzim (https://github.com/Gozzim)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "FirstLogin.h"
#include "Player.h"
#include "Chat.h"
#include "WorldSession.h"
#include "ScriptMgr.h"
#include "Config.h"

static bool FLoginEnable;
static bool FLoginAnnounce;
static uint32 FLoginStable;
static bool FLoginPet;
static uint32 FLoginPetName;
static bool FLoginPetSpells;
static uint32 FLoginMinion;
static uint32 FLoginBank;
static uint32 FLoginBagID;
static bool FLoginDualSpec;

/*
 * TODO:
 *  - Rework name generation
 *  - Fill sql files for name generation
 *  - Add learn all spells option
 */

class FirstLoginBeforeConfigLoad : public WorldScript {
public:
    FirstLoginBeforeConfigLoad() : WorldScript("FirstLoginBeforeConfigLoad") { }

    void OnBeforeConfigLoad(bool /*reload*/) override {
        FLoginEnable = sConfigMgr->GetBoolDefault("FirstLogin.Enable", 1);
        FLoginAnnounce = sConfigMgr->GetBoolDefault("FirstLogin.Announce", 1);
        FLoginStable = sConfigMgr->GetIntDefault("FirstLogin.StableSlots", 0);
        FLoginPet = sConfigMgr->GetBoolDefault("FirstLogin.Pet", 0);
        FLoginPetName = sConfigMgr->GetIntDefault("FirstLogin.RandPetName", 0);
        FLoginPetSpells = sConfigMgr->GetBoolDefault("FirstLogin.LearnPetSpells", 0);
        FLoginMinion = sConfigMgr->GetIntDefault("FirstLogin.WarlockMinion", 0);
        FLoginBank = sConfigMgr->GetIntDefault("FirstLogin.BankSlots", 0);
        FLoginBagID = sConfigMgr->GetIntDefault("FirstLogin.BagID", 0);
        FLoginDualSpec = sConfigMgr->GetBoolDefault("FirstLogin.DualSpec", 0);
    }
};

class FirstLoginScripts : public PlayerScript {
public:
    FirstLoginScripts() : PlayerScript("FirstLoginScripts") { }

    void OnFirstLogin(Player* player) override {
        if (FLoginEnable)
        {
            if (FLoginAnnounce)
                ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00FirstLogin-AIO |rmodule.");

            if (player->getClass() == CLASS_HUNTER)
            {
                if (FLoginStable > 0 && player->m_stableSlots < MAX_PET_STABLES)
                    player->m_stableSlots = FLoginStable > 4 ? 4 : FLoginStable;

                if (FLoginPet)
                    sFirstLogin->CreateRandomPet(player, FLoginPetName);

                if (FLoginPetSpells)
                    sFirstLogin->LearnPetSpells(player);
            }

            if (FLogin > 0 && player->getClass() == CLASS_WARLOCK)
            {
                sFirstLogin->LearnWarlockSpells(player, FLogin);
                sFirstLogin->SummonWarlockMinion(player, FLogin);
            }

            if (FLoginBank > 0)
            {
                uint32 slots = FLoginBank > 7 ? 7 : FLoginBank;
                player->SetBankBagSlotCount(slots);
                if (FLoginBagID > 0)
                    for (uint16 i = BANK_SLOT_BAG_START; i < (BANK_SLOT_BAG_START + slots); i++)
                        player->EquipNewItem(i, FLoginBagID, true);
            }

            if (FLoginDualSpec)
            {
                player->CastSpell(player, 63680, true, nullptr, nullptr, player->GetGUID());
                player->CastSpell(player, 63624, true, nullptr, nullptr, player->GetGUID());
            }
        }
    }
};

void AddFirstLoginScripts() {
    new FirstLoginBeforeConfigLoad();
    new FirstLoginScripts();
}