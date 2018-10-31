//
//  diyType.h
//  testC
//
//  Created by 陈帅 on 2018/9/18.
//

#ifndef diyType_h
#define diyType_h

enum actionType {
	atStart,
	atMove,
	atStop,
	atOver,
};

enum direction {
	dirUp,
	dirRight,
	dirDown,
	dirLeft,
	dirStop,
};

enum moveRoomType
{
	mrtStart,
	mrtEnter,
	mrtPass,
	mrtLeave,
	mrtStay,
	mrtNone,
};

enum configType
{
	ctIssue,
	ctRes,
	ctInfo,
	ctRoom,
	ctExam,
};

enum examType{
    etSpeed,
    etStrength,
    etSpirit,
    etKnowledge,
    etPhysicalDamage,
    etMindDamage,
	etDice,
	etItem,	//可以使用物品
    etNone,
};

enum itemBuffType
{
	ibtNone,
	ibtSeeCard,
	ibtUseDog,
	ibtUseMask,
	ibtAttackSpirit,
	ibtP2M,
	ibtAttackStrengthNum,
	ibtDiceAdd,
};

enum cardUseType
{
    cutGain,
    cutLoss,
};

enum cardClassType
{
	ccsUnlimitRes,
	ccsOnceRes,
	ccsInfo,
};

enum playerStatus
{
	psEnter,
	psReady,
	psStart,
};
#endif