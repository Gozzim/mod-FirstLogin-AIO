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

#include "Player.h"
#include "Pet.h"
#include "ObjectMgr.h"
#include "Creature.h"
#include "Chat.h"
#include "WorldSession.h"
#include "ScriptMgr.h"
#include "SharedDefines.h"
#include "Config.h"

static bool FLoginEnable;
static bool FLoginAnnounce;
static uint32 FLoginStable;
static bool FLoginPet;
static uint32 FLoginPetName;
static uint32 FLoginBank;
static uint32 FLoginBagID;
static bool FLoginDualSpec;

const std::string vowels = "aeiou";
const std::string consonants = "bcdfghjklmnpqrstvw";

/*
 * TODO:
 *  - Rework name generation
 *  - Fill sql files for name generation
 *  - Add learn all spells option (maybe rather on char creation?)
 */

class FirstLoginBeforeConfigLoad : public WorldScript {
public:
    FirstLoginBeforeConfigLoad() : WorldScript("FirstLoginBeforeConfigLoad") { }

    void OnBeforeConfigLoad(bool /*reload*/) override {
        FLoginEnable = sConfigMgr->GetBoolDefault("FirstLogin.Enable", 1);
        FLoginAnnounce = sConfigMgr->GetBoolDefault("FirstLogin.Announce", 1);
        FLoginStable = sConfigMgr->GetIntDefault("FirstLogin.StableSlots", 0);
        FLoginPet = sConfigMgr->GetBoolDefault("FirstLogin.Pet", 1);
        FLoginPetName = sConfigMgr->GetIntDefault("FirstLogin.RandPetName", 0);
        FLoginBank = sConfigMgr->GetIntDefault("FirstLogin.BankSlots", 0);
        FLoginBagID = sConfigMgr->GetIntDefault("FirstLogin.BagID", 0);
        FLoginDualSpec = sConfigMgr->GetBoolDefault("FirstLogin.DualSpec", 0);
    }
};

class first_login_scripts : public PlayerScript {
public:
    first_login_scripts() : PlayerScript("first_login_scripts") { }

    // Very basic implementation for testing purposes
    std::string randPetName() {
        const uint8 len = urand(4, 8);
        std::ostringstream new_name;
        std::string new_char;
        for (uint8 i = 1; i < len + 1; i++) {
            if (i == 1) {
                new_char = toupper(consonants[urand(0, consonants.length() - 1)]);
            } else if (i % 2 == 0 || i % 5 == 0) {
                new_char = vowels[urand(0, vowels.length() - 1)];
            } else {
                new_char = consonants[urand(0, consonants.length() - 1)];
            }
            new_name << new_char;
        }
        return new_name.str();
    }


    void createRandomPet(Player* player) {
        uint32 entry;
        std::string new_name;
        const uint32 pets[205] = {1513, 1554, 4861, 10357, 1186, 17345, 17348, 2354, 1130, 1815, 14280, 17661, 26643, 17053, 17034, 27244, 20109, 7099, 23051, 14343, 19429, 26369, 28004, 29044, 23945, 24747, 32481, 119, 345, 3098, 547, 16117, 2966, 4512, 4511, 4514, 7376, 1109, 5429, 2829, 2931,
                                  21470, 18707, 5983, 21515, 16973, 977, 18982, 21723, 24064, 17203, 2731, 682, 3619, 7430, 29327, 21724, 10200, 2175, 7434, 28001, 28297, 7431, 16348, 15651, 28097, 5807, 14222, 3475, 28404, 26521, 830, 23929, 24478, 4344, 17952, 20773, 1693, 25867, 15650, 15649,
                                  18155, 20502, 28096, 1559, 28098, 9622, 6585, 4690, 5985, 4534, 5427, 12418, 18468, 18469, 20611, 25464, 25498, 18131, 20196, 18880, 21901, 23873, 730, 1022, 29334, 1019, 3632, 3132, 2561, 20729, 20634, 21124, 20751, 17199, 19189, 16900, 16932, 17199, 19350, 16934,
                                  19349, 16933, 7022, 9691, 11736, 3252, 11737, 4140, 5423, 3125, 5048, 5225, 28011, 5755, 7268, 1505, 5856, 4379, 217, 15976, 22044, 15974, 20682, 22132, 28342, 17683, 17522, 32475, 1986, 14266, 5857, 442, 11738, 18128, 18280, 18129, 2955, 2956, 3068, 22807, 2172,
                                  3246, 3653, 4824, 4397, 2505, 25482, 18884, 18878, 22255, 23219, 28086, 28085, 22182, 20198, 20197, 18283, 5056, 5756, 5307, 5708, 26806, 8336, 26446, 21123, 20673, 20749, 28477, 20330, 1817, 1508, 2958, 10221, 19458, 8211, 118, 29889, 2753, 521, 24475, 24516,
                                  24517, 24677, 17669};
        entry = pets[urand(0, 204)];

        // Generate new name for pet
        if (FLoginPetName == 1)
            new_name = randPetName();
        if (FLoginPetName == 2)
            new_name = sObjectMgr->GeneratePetName(entry);

        Pet* pet = player->CreateTamedPetFrom(entry, 13481);
        if (!pet) {
            return;
        }

        float px, py, pz;
        player->GetClosePoint(px, py, pz, pet->GetObjectSize(), PET_FOLLOW_DIST, pet->GetFollowAngle());
        if (!pet->IsPositionValid()) {
            sLog->outError("Pet (entry %d) not loaded. Suggested coordinates isn't valid (X: %f Y: %f)", pet->GetEntry(), pet->GetPositionX(), pet->GetPositionY());
            delete pet;
            return;
        }
        pet->Relocate(px, py, pz, player->GetOrientation());

        pet->SetGuidValue(UNIT_FIELD_CREATEDBY, player->GetGUID());
        pet->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, player->getFaction());
        pet->SetUInt32Value(UNIT_FIELD_LEVEL, player->getLevel());

        pet->SetPower(POWER_HAPPINESS, 1048000);
        pet->SetReactState(REACT_DEFENSIVE);

        // Add to world
        pet->GetMap()->AddToMap(pet->ToCreature());

        pet->SetFullHealth();
        pet->GetCharmInfo()->SetPetNumber(sObjectMgr->GeneratePetNumber(), true);

        // Caster has pet now
        player->SetMinion(pet, true);

        // Set pet name
        if (!new_name.empty())
            pet->SetName(new_name);

        // Initialize Pet Stats
        pet->InitTalentForLevel();
        pet->InitStatsForLevel(player->getLevel());

        pet->SavePetToDB(PET_SAVE_AS_CURRENT, false);
        player->PetSpellInitialize();

    }

    void OnFirstLogin(Player* player) override {
        if (FLoginEnable) {

            if (FLoginAnnounce)
                ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00First-Login-Scripts |rmodule.");

            if (FLoginStable > 0 && player->getClass() == CLASS_HUNTER && player->m_stableSlots < MAX_PET_STABLES)
                player->m_stableSlots = FLoginStable > 4 ? 4 : FLoginStable;

            if (FLoginPet && player->getClass() == CLASS_HUNTER)
                createRandomPet(player);

            if (FLoginBank > 0) {
                uint32 slots = FLoginBank > 7 ? 7 : FLoginBank;
                player->SetBankBagSlotCount(slots);
                if (FLoginBagID > 0)
                    for (uint16 i = BANK_SLOT_BAG_START; i < (BANK_SLOT_BAG_START + slots); i++)
                        player->EquipNewItem(i, FLoginBagID, true);
            }

            if (FLoginDualSpec) {
                player->CastSpell(player, 63680, true, nullptr, nullptr, player->GetGUID());
                player->CastSpell(player, 63624, true, nullptr, nullptr, player->GetGUID());
            }
        }
    }
};

void AddFirstLoginScripts() {
    new FirstLoginBeforeConfigLoad();
    new first_login_scripts();
}