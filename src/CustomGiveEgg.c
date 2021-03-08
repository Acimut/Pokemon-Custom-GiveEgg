
#include "include/global.h"                         //pokefirered/include/global.h
#include "include/gba/types.h"                      //pokefirered/include/gba/types.h
#include "include/constants/items.h"                //pokefirered/include/constants/items.h
//#include "include/constants/daycare.h"              //pokefirered/include/constants/daycare.h
#include "include/constants/region_map_sections.h"  //pokefirered/include/constants/region_map_sections.h
#include "include/script.h"                         //pokefirered/include/script.h


extern void *AllocZeroed(u32 size);                         //pokefirered/src/malloc.c
extern void SetMonData(struct Pokemon *mon, s32 field, const void *dataArg);            //pokefirered/src/pokemon.c
extern void CreateMon(struct Pokemon *mon, u16 species, u8 level, u8 fixedIV, u8 hasFixedPersonality, u32 fixedPersonality, u8 otIdType, u32 fixedOtId);//pokefirered/src/pokemon.c
extern u8 GiveMonToPlayer(struct Pokemon *mon);             //pokefirered/src/pokemon.c
extern void Free(void *pointer);                            //pokefirered/src/malloc.c
extern u16 Random(void);                                    //pokefirered/src/random.c
extern const u8 sJapaneseEggNickname[];                     //pokefirered/src/daycare.c
//extern struct Evolution gEvolutionTable[NUM_SPECIES][EVOS_PER_MON];//[412][5] ya definido en pokemon.h

extern u16 gSpecialVar_0x8000;//Especie, si es 0 obtiene una especie random.
extern u16 gSpecialVar_0x8001;//nivel, mínimo 1
extern u16 gSpecialVar_0x8002;//ball del 1 al 12, predeterminado es 4 = pokeball
extern u16 gSpecialVar_0x8003;//ciclos es igual a (n + 1) * 256 pasos para eclosionar. máximo 255 ciclos.
extern u16 gSpecialVar_0x8004;//IVs de 0 a 31 todos iguales, 32 = random
extern u16 gSpecialVar_0x8005;

#define NUM_ESPECIES    386 //0x182 //cantidad de especies pokemon del hack rom
#define ESPACIOS_VACIOS 25  //0x19  //espacios vacíos entre celebi y treecko

void CustomGiveEgg()
{
    u16 species = gSpecialVar_0x8000;
    struct Pokemon *mon = AllocZeroed(sizeof(struct Pokemon));
    bool8 isEgg;
    u8 metLevel;
    u16 ball;
    u8 language;
    u8 metLocation;
    u8 ciclos;
    u8 level = gSpecialVar_0x8001;
    if (!level) //si dejas el nivel en 0, automáticamente pasa a ser 5
        level = 5;
    if (species > (NUM_ESPECIES + ESPACIOS_VACIOS)) //si la especie es mayor a la cantidad predefinida, escoje una especie random
        species = 0;
    while(!species)
    {
        species = (Random() % NUM_ESPECIES) + 1;
        if (species > SPECIES_CELEBI) //si es mayor a 251 sume 25 al resultado. (252 + 25 = 277) ó 386 + 25 = 411
            species += ESPACIOS_VACIOS;
        if (gEvolutionTable[species][0].targetSpecies == 0 || gEvolutionTable[species-1][0].targetSpecies == species)
        {
            species = 0;
        }
    }
    CreateMon(mon, species, level, gSpecialVar_0x8004, FALSE, 0, OT_ID_PLAYER_ID, 0);
    metLevel = 0;

    if(gSpecialVar_0x8002 > 0 && gSpecialVar_0x8002 < 13)
    {
        ball = gSpecialVar_0x8002;
    }else
    {
        ball = 4; //pokéball predeterminada
    }

    language = LANGUAGE_JAPANESE;
    ciclos = gSpecialVar_0x8003;
    SetMonData(mon, MON_DATA_POKEBALL, &ball);
    SetMonData(mon, MON_DATA_NICKNAME, sJapaneseEggNickname);
    SetMonData(mon, MON_DATA_FRIENDSHIP, &ciclos);
    SetMonData(mon, MON_DATA_MET_LEVEL, &metLevel);
    SetMonData(mon, MON_DATA_LANGUAGE, &language);
    metLocation = METLOC_SPECIAL_EGG;
    SetMonData(mon, MON_DATA_MET_LOCATION, &metLocation);
    isEgg = TRUE;
    SetMonData(mon, MON_DATA_IS_EGG, &isEgg);
    GiveMonToPlayer(mon);
    Free(mon);
}

#include "include/pokedex.h"                         //pokefirered/include/pokedex.h

extern u32 GetMonData(struct Pokemon *mon, s32 field, u8* data);    //pokefirered/src/pokemon.c
extern void GetSpeciesName(u8 *name, u16 species);                  //pokefirered/src/pokemon.c
extern u16 SpeciesToNationalPokedexNum(u16 species);                //pokefirered/src/pokemon.c
extern s8 GetSetPokedexFlag(u16 nationalNum, u8 caseId);            //pokefirered/src/pokedex.c
extern u8 *DayCare_GetMonNickname(struct Pokemon *mon, u8 *dest);   //pokefirered/src/daycare.c
extern u8 GetCurrentRegionMapSectionId(void);                       //pokefirered/src/overworld.c
extern void MonRestorePP(struct Pokemon *mon);                      //pokefirered/src/pokemon.c
extern void CalculateMonStats(struct Pokemon *mon);                 //pokefirered/src/pokemon.c
extern u8 gStringVar1[32];                                          //pokefirered/src/string_util.c

static void NewCreatedHatchedMon(struct Pokemon *egg, struct Pokemon *temp)
{
    u16 species;
    u32 personality, pokerus;
    u8 i, friendship, language, gameMet, markings, isEventLegal;
    u16 moves[4];
    u32 ivs[NUM_STATS];
    u8 level;

    species = GetMonData(egg, MON_DATA_SPECIES, NULL);

    for (i = 0; i < 4; i++)
    {
        moves[i] = GetMonData(egg, MON_DATA_MOVE1 + i, NULL);
    }

    personality = GetMonData(egg, MON_DATA_PERSONALITY, NULL);

    for (i = 0; i < NUM_STATS; i++)
    {
        ivs[i] = GetMonData(egg, MON_DATA_HP_IV + i, NULL);
    }

    gameMet = GetMonData(egg, MON_DATA_MET_GAME, NULL);
    markings = GetMonData(egg, MON_DATA_MARKINGS, NULL);
    pokerus = GetMonData(egg, MON_DATA_POKERUS, NULL);
    isEventLegal = GetMonData(egg, MON_DATA_EVENT_LEGAL, NULL);

    level = GetMonData(egg, MON_DATA_LEVEL, NULL);
    CreateMon(temp, species, level, 32, TRUE, personality, 0, 0);

    for (i = 0; i < 4; i++)
    {
        SetMonData(temp, MON_DATA_MOVE1 + i,  &moves[i]);
    }

    for (i = 0; i < NUM_STATS; i++)
    {
        SetMonData(temp, MON_DATA_HP_IV + i,  &ivs[i]);
    }

    language = LANGUAGE_ENGLISH;
    SetMonData(temp, MON_DATA_LANGUAGE, &language);
    SetMonData(temp, MON_DATA_MET_GAME, &gameMet);
    SetMonData(temp, MON_DATA_MARKINGS, &markings);

    friendship = 120;
    SetMonData(temp, MON_DATA_FRIENDSHIP, &friendship);
    SetMonData(temp, MON_DATA_POKERUS, &pokerus);
    SetMonData(temp, MON_DATA_EVENT_LEGAL, &isEventLegal);

    *egg = *temp;
}

void NewAddHatchedMonToParty(u8 id)
{
    u8 isEgg = 0x46; // ?
    u16 pokeNum;
    u8 name[12];
    u16 ball;
    u16 caughtLvl;
    u8 mapNameID;

    struct Pokemon* mon = &gPlayerParty[id];

    ball = GetMonData(mon, MON_DATA_POKEBALL, NULL);

    NewCreatedHatchedMon(mon, &gEnemyParty[0]);
    SetMonData(mon, MON_DATA_IS_EGG, &isEgg);

    pokeNum = GetMonData(mon, MON_DATA_SPECIES, NULL);
    GetSpeciesName(name, pokeNum);
    SetMonData(mon, MON_DATA_NICKNAME, name);

    pokeNum = SpeciesToNationalPokedexNum(pokeNum);
    GetSetPokedexFlag(pokeNum, FLAG_SET_SEEN);
    GetSetPokedexFlag(pokeNum, FLAG_SET_CAUGHT);

    DayCare_GetMonNickname(mon, gStringVar1);

    SetMonData(mon, MON_DATA_POKEBALL, &ball);

    caughtLvl = 0;
    SetMonData(mon, MON_DATA_MET_LEVEL, &caughtLvl);

    mapNameID = GetCurrentRegionMapSectionId();
    SetMonData(mon, MON_DATA_MET_LOCATION, &mapNameID);

    MonRestorePP(mon);
    CalculateMonStats(mon);
}