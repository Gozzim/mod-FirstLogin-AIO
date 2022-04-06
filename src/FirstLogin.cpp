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

enum HunterVars
{
    SPELL_TAME_BEAST    = 13481,
    SPELL_MEND_PET      = 136,
    PET_MAX_HAPPINESS   = 1048000
};

FirstLogin* FirstLogin::instance()
{
    static FirstLogin instance;
    return &instance;
}

// Very basic implementation for testing purposes
std::string FirstLogin::RandName(uint16 minLen, uint16 maxLen)
{
    uint16 const len = urand(minLen, maxLen);
    std::ostringstream newName;
    std::string nextChar;

    for (uint8 i = 1; i < len + 1; i++)
    {
        if (i == 1)
        {
            nextChar = toupper(_consonants[urand(0, _consonants.length() - 1)]);
        }
        else if (i % 2 == 0 || i % 5 == 0)
        {
            nextChar = _vowels[urand(0, _vowels.length() - 1)];
        }
        else
        {
            nextChar = _consonants[urand(0, _consonants.length() - 1)];
        }
        newName << nextChar;
    }

    return newName.str();
}

void FirstLogin::CreateRandomPet(Player* player, uint32 petNameConf)
{
    uint32 entry;
    std::string newName;
    entry = _pets[urand(0, 204)];

    // Generate new name for pet
    if (petNameConf == 1)
    {
        newName = RandName(4, 8);
    }

    if (petNameConf == 2)
    {
        newName = sObjectMgr->GeneratePetName(entry);
    }

    Pet* pet = player->CreateTamedPetFrom(entry, SPELL_TAME_BEAST);
    if (!pet)
    {
        return;
    }

    float px, py, pz;
    player->GetClosePoint(px, py, pz, pet->GetObjectSize(), PET_FOLLOW_DIST, pet->GetFollowAngle());
    if (!pet->IsPositionValid())
    {
        LOG_DEBUG("module", "FirstLogin::CreateRandomPet - Pet (entry {}) not loaded. Suggested coordinates isn't valid (X: {} Y: {})", pet->GetEntry(), pet->GetPositionX(), pet->GetPositionY());
        delete pet;
        return;
    }
    pet->Relocate(px, py, pz, player->GetOrientation());

    pet->SetGuidValue(UNIT_FIELD_CREATEDBY, player->GetGUID());
    pet->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, player->GetFaction());
    pet->SetUInt32Value(UNIT_FIELD_LEVEL, player->getLevel());

    pet->SetPower(POWER_HAPPINESS, PET_MAX_HAPPINESS);
    pet->SetReactState(REACT_DEFENSIVE);

    // Add to world
    pet->GetMap()->AddToMap(pet->ToCreature());

    pet->SetFullHealth();
    pet->GetCharmInfo()->SetPetNumber(sObjectMgr->GeneratePetNumber(), true);

    // Caster has pet now
    player->SetMinion(pet, true);

    // Set pet name
    if (!newName.empty())
    {
        pet->SetName(newName);
    }

    // Initialize Pet Stats
    pet->InitTalentForLevel();
    pet->InitStatsForLevel(player->getLevel());

    pet->SavePetToDB(PET_SAVE_AS_CURRENT);
    player->PetSpellInitialize();
}

void FirstLogin::LearnPetSpells(Player* player)
{
    for (uint32 i = 0; i < _hunterPetSpells.size(); ++i)
    {
        LearnSpellAndRanksForLevel(_hunterPetSpells[i], player);
    }
}

void FirstLogin::LearnWarlockSpells(Player* player, uint32 minionMask)
{
    for (uint32 i = 0; i < 6; ++i)
    {
        if (i == 4)
            continue;

        if (minionMask & (1 << i))
        {
            LearnSpellAndRanksForLevel(_warlockSpells[i], player);
        }
    }
}

void FirstLogin::SummonWarlockMinion(Player* player, uint32 minionMask)
{
    std::vector<uint32> warlockMinions;

    for (uint32 i = 0; i < 5; ++i)
    {
        if (minionMask & (1 << i))
        {
            warlockMinions.push_back(_warlockSpells[i]);
        }
    }

    if (warlockMinions.empty())
        return;

    uint32 spellId = warlockMinions[urand(0, warlockMinions.size() - 1)];
    player->CastSpell(player, spellId, true);
}

void FirstLogin::LearnSpellAndRanksForLevel(uint32 spellId, Player* player)
{
    player->learnSpell(spellId);
    uint32 next = sSpellMgr->GetNextSpellInChain(spellId);
    if (next && sSpellMgr->GetSpellInfo(next)->BaseLevel <= player->getLevel())
    {
        LearnSpellAndRanksForLevel(next, player);
    }
}

