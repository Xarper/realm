    #include "ScriptPCH.h"
     
    class TeachOnLogin : public PlayerScript
    {
    public:
        TeachOnLogin() : PlayerScript("TeachOnLogin") { }
       
        void OnCreate(Player* player)
        {
            // HandleLearnAllMySpellsCommand Copy paste
            ChrClassesEntry const* classEntry = sChrClassesStore.LookupEntry(player->getClass());
            if (!classEntry)
                return;
            uint32 family = classEntry->spellfamily;
     
            for (uint32 i = 0; i < sSkillLineAbilityStore.GetNumRows(); ++i)
            {
                SkillLineAbilityEntry const* entry = sSkillLineAbilityStore.LookupEntry(i);
                if (!entry)
                    continue;
     
                SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(entry->spellId);
                if (!spellInfo)
                    continue;
     
                // skip server-side/triggered spells
                if (spellInfo->SpellLevel == 0)
                    continue;
     
                // skip wrong class/race skills
                if (!player->IsSpellFitByClassAndRace(spellInfo->Id))
                    continue;
     
                // skip other spell families
                if (spellInfo->SpellFamilyName != family)
                    continue;
     
                // skip spells with first rank learned as talent (and all talents then also)
                uint32 firstRank = sSpellMgr->GetFirstSpellInChain(spellInfo->Id);
                if (GetTalentSpellCost(firstRank) > 0)
                    continue;
     
                // skip broken spells
                if (!SpellMgr::IsSpellValid(spellInfo, player, false))
                    continue;
     
                player->learnSpell(spellInfo->Id, false);
            }
        }
    };

