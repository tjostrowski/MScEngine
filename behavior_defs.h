#ifndef __BEHAVIOR_DEFS
#define __BEHAVIOR_DEFS

// whenMove
enum eBehWhenMove
{
	E_BEH_UNDEF						= 0,

	E_BEH_RETREAT_FROM_HEAVY_ATTACK	= 1,
	E_BEH_TRACE_ATTACKED_ETG		= (1 << 1),

};

// whenAttack
enum eBehWhenAttack
{
	E_BEH_ATTACK_WEAK_ETGS			= (1 << 2),
	E_BEH_ATTACK_STRONG_ETGS		= (1 << 3),
	E_BEH_ATTACK_HLP_ATTACKED_FRTGS	= (1 << 4),

};

// whenJoin 
enum eBenWhenJoin
{
	E_BEH_JOIN_NEAR_FRTGS			= (1 << 5),
    

};

// when Split
enum eBehWhenSplit
{
	E_BEH_SPLIT_WHEN_MANY_NEAR_ETGS	= (1 << 6),
	E_BEH_SPLIT_WHEN_OVERFORCE		= (1 << 7),

};

// Misc
enum eBehMisc
{
	E_BEH_MISC_USE_CANNON			= (1 << 8),
	E_BEH_EXP_TG					= (1 << 9),
	E_BEH_PCK_TG					= (1 << 10),

};


extern string getBehaviorName(ulong behId);
extern map<ulong, string> mapBehId2Name; 

#endif

