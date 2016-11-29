#include "MapScene.h"
#include "BattleScene.h"

#include <stdlib.h>
#include <string.h>
#include "SimpleAudioEngine.h"
#include <time.h>
#include "Language.h"
#include "Game.h"

//z축
/*
1: turn 표시하는 스프라이트
2: 슬라임들과 에너지 바
3: 스킬트리 보여주는 하단의 바



*/


//tag number
/*
0: background layer
1: background sprite
2: 슬라이을 선택하면 하단에 나오는 스킬트리 바
3: scratch스킬버튼(1)
5: bomb스킬버튼(1)
7: steal스킬버튼(1)
9: recovery스킬버튼(1)
11: meteo스킬버튼(1)

12~16: slime turn 표시(현재 공격권 갖고 있는 쪽을 표시)
17: skill_tree에 들어가는 xbox버튼태그
18: skill_tree에 들어가는 ok버튼 태그
19~21: meteo액션에 들어가는 sprite tag
22~26: computer가 meteo맞았을 때 뜨는 stun상태 표시 sprite
27~31: user가 meteo맞았을 때 뜨는 stun상태 표시 sprite
32: 승패를 보여주는 layer
33: 배틀 종료후 농장으로 돌아가는 버튼
34: 배틀 종료후 map으로 돌아가는 버튼


10000대역: 사용자 슬라임 스프라이트
15000대역: 사용자 슬라임 에너지바 스프라이트
20000대역: 컴퓨터 슬라임 스프라이트
25000대역: 컴퓨터 슬라임 에너지바 스프라이트

*/



//슬라임의 랭크, 인덱스, exp, hp, 스킬 갯수를 담는 구조체
struct select_s {
	int rank;
	int index;
	int id;
	int exp;
	int hp;
	int skill_n;
	int critical[5];
	int increase_hp[5];
	String property;
	int property_num;
	char * spr_name;
	int my_s_numbering;

	int swintch_slime_btn = 0;

};

extern int select_stage_num;

extern select_s select_slime[5];//선택한 슬라임들을 구조체 타입의 배열에 담기
extern select_s com_slime[5];
extern select_s user_slime_list[5000];

extern float property_attack[7][7];

select_s temp_slime;//사용자가 자신의 슬라임을 선택
select_s target_slime;//사용자가 공격할 슬라임을 선택


Sprite * turn[5];
Sprite * slime_u[5];
Sprite * slime_c[5];
Sprite * hp_bar_u[5];
Sprite * hp_bar_c[5];



int user_hp[5];
int com_hp[5];

int select_skill_num = 0;
int select_user_slime_tag = 0;
CocosDenshion::SimpleAudioEngine * pAudio_b = CocosDenshion::SimpleAudioEngine::getInstance();


int testa = 0;

Scene* BattleScene::createScene(){
	auto scene = Scene::create();
	auto layer = BattleScene::create();
	scene->addChild(layer);

	return scene;
}


bool BattleScene::init()
{
	if (!Layer::init()){
		return false;
	}
	time_t timer;
	struct tm *t;

	timer = time(NULL); // 현재 시각을 초 단위로 얻기

	t = localtime(&timer); // 초 단위의 시간을 분리하여 구조체에 넣기

	//int hour = 3;
	//int second = 3;
	int hour = t->tm_hour;
	int second = t->tm_sec;
	//	CocosDenshion::SimpleAudioEngine * pAudio = CocosDenshion::SimpleAudioEngine::getInstance();
	//현재시간 정보에 따른 배경음악 재생
	if (hour > 5 && hour < 10){//오전
		int type = (rand() * second) % 2;
		if (type == 0){
			pAudio_b->playBackgroundMusic("music/morning_C_Major_Prelude.mp3", true);

			//	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/morning_C_Major_Prelude.mp3", true);
		}

		else{
			pAudio_b->playBackgroundMusic("music/morning_lunch_Claudio_The_Worm.mp3", true);
			//	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/morning_lunch_Claudio_The_Worm.mp3", true);

		}

	}
	else if (hour > 10 && hour < 21){//오후
		int type = (rand() * second) % 2;
		if (type == 0)
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/afternoon_Get_Outside.mp3", true);
		else
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/afternoon_Good Starts.mp3", true);
	}
	else{//밤과 새벽
		int type = (rand() * second) % 2;
		if (type == 0)
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/evening_Are_You_Sleeping.mp3", true);
		else
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/evening_Twinkle_Twinkle_Little_Star.mp3", true);
	}

	auto layer = LayerColor::create(Color4B(0, 0, 0, 0));
	layer->setContentSize(Size(1280, 3600));//1280, 3600
	layer->setTag(0);

	auto backgroundimg = Sprite::create("map/mapbackground.png", Rect(0, 0, 1280, 720));
	backgroundimg->setAnchorPoint(Point(0, 0));
	backgroundimg->setTag(1);
	layer->addChild(backgroundimg);

	this->addChild(layer);

	int tag[5];

	/*auto new_slime = Sprite::create(StringUtils::format(rank1_slime_filename[decide_index_id], 1));////
	new_slime->setPosition(Point(-90, 100));
	new_slime->setAnchorPoint(Point(0,1));
	new_slime->setScale(2);
	layer->addChild(new_slime);

	auto animation = Animation::create();
	animation->setDelayPerUnit(0.1);

	for (int h = 0; h < rank1_slime_frame_number[decide_index_id]; h++){////
	animation->addSpriteFrameWithFile(StringUtils::format(rank1_slime_filename[decide_index_id],h+1));////
	}

	auto animate = Animate::create(animation);
	auto act = RepeatForever::create(animate);
	new_slime->runAction(act);*/

	for (int n = 0; n < 5; n++){

		turn[n] = Sprite::create("battle/turn.png");
		turn[n]->setAnchorPoint(Point(0, 0));
		turn[n]->setScale(1.5);

		slime_u[n] = Sprite::create(StringUtils::format(select_slime[n].spr_name, 1));
		slime_u[n]->setAnchorPoint(Point(0, 0));
		slime_u[n]->setScale(1.3);
		slime_u[n]->setTag(10000 + n);
		user_hp[n] = select_slime[n].hp;

		if (n < 3){
			turn[n]->setPosition(Point(100, 520 - (200 * n)));//태그값 넣기
			turn[n]->setTag(12 + n);
			slime_u[n]->setPosition(Point(100, 520 - (200 * n)));

			//	turn[n]->addChild(slime_u[n]);
		}
		else{
			turn[n]->setPosition(Point(270, 420 - (240 * (n - 3))));
			turn[n]->setTag(12 + n);
			slime_u[n]->setPosition(Point(270, 420 - (240 * (n - 3))));

			//	turn[n]->addChild(slime_u[n]);
		}

		EventDispatcher * dispatcher_slime_u = Director::getInstance()->getEventDispatcher();
		auto positionListener_slime_u = EventListenerTouchOneByOne::create();
		positionListener_slime_u->setSwallowTouches(false);
		positionListener_slime_u->onTouchBegan = CC_CALLBACK_2(BattleScene::TouchBegan, this);
		dispatcher_slime_u->addEventListenerWithSceneGraphPriority(positionListener_slime_u->clone(), slime_u[n]);

		this->addChild(turn[n], 1);
		this->addChild(slime_u[n], 2);

		hp_bar_u[n] = Sprite::create("battle/whitebox.png");
		hp_bar_u[n]->setTextureRect(Rect(0, 0, 100, 5));
		hp_bar_u[n]->setColor(Color3B::RED);
		hp_bar_u[n]->setTag(15000 + n);

		Size parentSize;
		parentSize = slime_u[n]->getContentSize();
		hp_bar_u[n]->setPosition(Point(parentSize.width / 2.0, parentSize.height + 10));

		slime_u[n]->addChild(hp_bar_u[n], 2);




		tag[n] = slime_u[n]->getTag();
		//int tag = slime_u[n]->getTag();


	}

	for (int n = 0; n < 5; n++){
		slime_c[n] = Sprite::create(StringUtils::format(com_slime[n].spr_name, 1));
		slime_c[n]->setAnchorPoint(Point(0, 0));
		slime_c[n]->setScale(1.3);
		slime_c[n]->setTag(20000 + n);
		com_hp[n] = com_slime[n].hp;


		if (n < 3){
			slime_c[n]->setPosition(Point(1100, 520 - (200 * n)));

		}
		else{
			slime_c[n]->setPosition(Point(930, 420 - (240 * (n - 3))));
		}

		EventDispatcher * dispatcher_slime_c = Director::getInstance()->getEventDispatcher();
		auto positionListener_slime_c = EventListenerTouchOneByOne::create();
		positionListener_slime_c->setSwallowTouches(false);
		positionListener_slime_c->onTouchBegan = CC_CALLBACK_2(BattleScene::TouchBegan, this);
		dispatcher_slime_c->addEventListenerWithSceneGraphPriority(positionListener_slime_c->clone(), slime_c[n]);

		this->addChild(slime_c[n], 1);

		hp_bar_c[n] = Sprite::create("battle/whiteBox.png");
		hp_bar_c[n]->setTextureRect(Rect(0, 0, 100, 5));
		hp_bar_c[n]->setColor(Color3B::RED);
		hp_bar_c[n]->setTag(25000 + n);

		Size parentSize;
		parentSize = slime_c[n]->getContentSize();
		hp_bar_c[n]->setPosition(Point(parentSize.width / 2.0, parentSize.height + 10));

		slime_c[n]->addChild(hp_bar_c[n], 2);

		slime_c[n]->getEventDispatcher()->pauseEventListenersForTarget(slime_c[n], false);
	}

	//컴퓨터측 슬라임의 이벤트 pause
	/*for (int i = 0; i < 5; i++){
	slime_c[i]->getEventDispatcher()->pauseEventListenersForTarget(slime_c[i], false);
	}
	*/
	return true;
}

bool BattleScene::TouchBegan(Touch * touch, Event * e){
	auto target = e->getCurrentTarget();

	Point pos = target->convertToNodeSpace(touch->getLocation());
	Rect rect = Rect(0, 0, target->getContentSize().width, target->getContentSize().height);

	int damage_hp = 0;//
	int recovery_hp = 0;

	if (rect.containsPoint(pos)){
		if (target->getTag() >= 10000 && target->getTag() < 15000){//사용자측 슬라임을 선택했을 때

			int target_idx = target->getTag() - 10000;
			temp_slime = select_slime[target_idx];

			select_user_slime_tag = target_idx;
			for (int i = 0; i < 5; i++){//사용자측 슬라임을 선택했으니까 잠시 사용자측 슬라임을 다시 pause
				slime_u[i]->getEventDispatcher()->pauseEventListenersForTarget(slime_u[i], false);
			}

			auto skill_tree = Sprite::create("map/skilltree.png");
			skill_tree->setAnchorPoint(Point(0, 0));
			skill_tree->setPosition(Point(50, -200));
			skill_tree->setScale(0.85);
			skill_tree->setTag(2);

			auto xbox = Sprite::create("box/x.png");
			xbox->setScale(0.9);
			xbox->setAnchorPoint(Point(0, 0));
			xbox->setPosition(Point(1200, 110));
			xbox->setTag(17);

			EventDispatcher * dispatcher_xbox = Director::getInstance()->getEventDispatcher();
			auto positionListener_xbox = EventListenerTouchOneByOne::create();
			positionListener_xbox->setSwallowTouches(false);
			positionListener_xbox->onTouchBegan = CC_CALLBACK_2(BattleScene::TouchBegan, this);
			dispatcher_xbox->addEventListenerWithSceneGraphPriority(positionListener_xbox->clone(), xbox);

			auto skill1 = Sprite::create("map/scratch1.png");
			skill1->setAnchorPoint(Point(0, 0));
			skill1->setPosition(Point(70, 20));
			skill1->setScale(1.6);
			skill1->setTag(3);

			EventDispatcher * dispatcher_skill1 = Director::getInstance()->getEventDispatcher();
			auto positionListener_skill1 = EventListenerTouchOneByOne::create();
			positionListener_skill1->setSwallowTouches(false);
			positionListener_skill1->onTouchBegan = CC_CALLBACK_2(BattleScene::TouchBegan, this);
			dispatcher_skill1->addEventListenerWithSceneGraphPriority(positionListener_skill1->clone(), skill1);

			skill_tree->addChild(skill1);

			//슬라임 랭크별로 스킬트리 세팅
			if (temp_slime.rank == 1){
				auto skill2 = Sprite::create("map/bomb1.png");
				skill2->setAnchorPoint(Point(0, 0));
				skill2->setPosition(Point(220, 20));
				skill2->setScale(1.6);
				skill2->setTag(5);

				EventDispatcher * dispatcher_skill2 = Director::getInstance()->getEventDispatcher();
				auto positionListener_skill2 = EventListenerTouchOneByOne::create();
				positionListener_skill2->setSwallowTouches(false);
				positionListener_skill2->onTouchBegan = CC_CALLBACK_2(BattleScene::TouchBegan, this);
				dispatcher_skill2->addEventListenerWithSceneGraphPriority(positionListener_skill2->clone(), skill2);

				skill_tree->addChild(skill2);

				auto skill3 = Sprite::create("map/steal1.png");
				skill3->setAnchorPoint(Point(0, 0));
				skill3->setPosition(Point(370, 20));
				skill3->setScale(1.6);
				skill3->setTag(7);

				EventDispatcher * dispatcher_skill3 = Director::getInstance()->getEventDispatcher();
				auto positionListener_skill3 = EventListenerTouchOneByOne::create();
				positionListener_skill3->setSwallowTouches(false);
				positionListener_skill3->onTouchBegan = CC_CALLBACK_2(BattleScene::TouchBegan, this);
				dispatcher_skill3->addEventListenerWithSceneGraphPriority(positionListener_skill3->clone(), skill3);

				skill_tree->addChild(skill3);

				auto skill4 = Sprite::create("map/recovery1.png");
				skill4->setAnchorPoint(Point(0, 0));
				skill4->setPosition(Point(520, 20));
				skill4->setScale(1.6);
				skill4->setTag(9);

				EventDispatcher * dispatcher_skill4 = Director::getInstance()->getEventDispatcher();
				auto positionListener_skill4 = EventListenerTouchOneByOne::create();
				positionListener_skill4->setSwallowTouches(false);
				positionListener_skill4->onTouchBegan = CC_CALLBACK_2(BattleScene::TouchBegan, this);
				dispatcher_skill4->addEventListenerWithSceneGraphPriority(positionListener_skill4->clone(), skill4);

				skill_tree->addChild(skill4);

				auto skill5 = Sprite::create("map/meteo1.png");
				skill5->setAnchorPoint(Point(0, 0));
				skill5->setPosition(Point(670, 20));
				skill5->setScale(1.6);
				skill5->setTag(11);

				EventDispatcher * dispatcher_skill5 = Director::getInstance()->getEventDispatcher();
				auto positionListener_skill5 = EventListenerTouchOneByOne::create();
				positionListener_skill5->setSwallowTouches(false);
				positionListener_skill5->onTouchBegan = CC_CALLBACK_2(BattleScene::TouchBegan, this);
				dispatcher_skill5->addEventListenerWithSceneGraphPriority(positionListener_skill5->clone(), skill5);

				skill_tree->addChild(skill5);
			}
			else if (temp_slime.rank == 2){
				auto skill2 = Sprite::create("map/bomb1.png");
				skill2->setAnchorPoint(Point(0, 0));
				skill2->setPosition(Point(220, 20));
				skill2->setScale(1.6);
				skill2->setTag(5);

				EventDispatcher * dispatcher_skill2 = Director::getInstance()->getEventDispatcher();
				auto positionListener_skill2 = EventListenerTouchOneByOne::create();
				positionListener_skill2->setSwallowTouches(false);
				positionListener_skill2->onTouchBegan = CC_CALLBACK_2(BattleScene::TouchBegan, this);
				dispatcher_skill2->addEventListenerWithSceneGraphPriority(positionListener_skill2->clone(), skill2);

				skill_tree->addChild(skill2);

				auto skill3 = Sprite::create("map/steal1.png");
				skill3->setAnchorPoint(Point(0, 0));
				skill3->setPosition(Point(370, 20));
				skill3->setScale(1.6);
				skill3->setTag(7);

				EventDispatcher * dispatcher_skill3 = Director::getInstance()->getEventDispatcher();
				auto positionListener_skill3 = EventListenerTouchOneByOne::create();
				positionListener_skill3->setSwallowTouches(false);
				positionListener_skill3->onTouchBegan = CC_CALLBACK_2(BattleScene::TouchBegan, this);
				dispatcher_skill3->addEventListenerWithSceneGraphPriority(positionListener_skill3->clone(), skill3);

				skill_tree->addChild(skill3);

				auto skill4 = Sprite::create("map/recovery1.png");
				skill4->setAnchorPoint(Point(0, 0));
				skill4->setPosition(Point(520, 20));
				skill4->setScale(1.6);
				skill4->setTag(9);

				EventDispatcher * dispatcher_skill4 = Director::getInstance()->getEventDispatcher();
				auto positionListener_skill4 = EventListenerTouchOneByOne::create();
				positionListener_skill4->setSwallowTouches(false);
				positionListener_skill4->onTouchBegan = CC_CALLBACK_2(BattleScene::TouchBegan, this);
				dispatcher_skill4->addEventListenerWithSceneGraphPriority(positionListener_skill4->clone(), skill4);

				skill_tree->addChild(skill4);
			}
			else if (temp_slime.rank == 3){
				auto skill2 = Sprite::create("map/bomb1.png");
				skill2->setAnchorPoint(Point(0, 0));
				skill2->setPosition(Point(220, 20));
				skill2->setScale(1.6);
				skill2->setTag(5);

				EventDispatcher * dispatcher_skill2 = Director::getInstance()->getEventDispatcher();
				auto positionListener_skill2 = EventListenerTouchOneByOne::create();
				positionListener_skill2->setSwallowTouches(false);
				positionListener_skill2->onTouchBegan = CC_CALLBACK_2(BattleScene::TouchBegan, this);
				dispatcher_skill2->addEventListenerWithSceneGraphPriority(positionListener_skill2->clone(), skill2);

				skill_tree->addChild(skill2);

				auto skill3 = Sprite::create("map/steal1.png");
				skill3->setAnchorPoint(Point(0, 0));
				skill3->setPosition(Point(370, 20));
				skill3->setScale(1.6);
				skill3->setTag(7);

				EventDispatcher * dispatcher_skill3 = Director::getInstance()->getEventDispatcher();
				auto positionListener_skill3 = EventListenerTouchOneByOne::create();
				positionListener_skill3->setSwallowTouches(false);
				positionListener_skill3->onTouchBegan = CC_CALLBACK_2(BattleScene::TouchBegan, this);
				dispatcher_skill3->addEventListenerWithSceneGraphPriority(positionListener_skill3->clone(), skill3);

				skill_tree->addChild(skill3);

			}
			else if (temp_slime.rank == 4){
				auto skill2 = Sprite::create("map/bomb1.png");
				skill2->setAnchorPoint(Point(0, 0));
				skill2->setPosition(Point(220, 20));
				skill2->setScale(1.6);
				skill2->setTag(5);

				EventDispatcher * dispatcher_skill2 = Director::getInstance()->getEventDispatcher();
				auto positionListener_skill2 = EventListenerTouchOneByOne::create();
				positionListener_skill2->setSwallowTouches(false);
				positionListener_skill2->onTouchBegan = CC_CALLBACK_2(BattleScene::TouchBegan, this);
				dispatcher_skill2->addEventListenerWithSceneGraphPriority(positionListener_skill2->clone(), skill2);

				skill_tree->addChild(skill2);
			}
			else {

			}
			//스킬트리 올라오는 액션 넣기
			auto skilltree_act = MoveTo::create(1.0, Point(50, 10));
			skill_tree->runAction(skilltree_act);
			skill_tree->addChild(xbox);
			this->addChild(skill_tree, 3);
			/*
			if (this->getChildByTag(22)){
			for (int s = 0; s < 5; s++){
			this->removeChildByTag(22 + s);
			}

			}
			*/

		}
		else if (target->getTag() == 17){
			auto skill_tree = (Sprite*)this->getChildByTag(2);
			this->removeChild(skill_tree);

			for (int i = 0; i < 5; i++){
				slime_u[i]->getEventDispatcher()->resumeEventListenersForTarget(slime_u[i], false);
			}
		}
		else if (target->getTag() == 18){
			auto skill_tree = (Sprite*)this->getChildByTag(2);
			this->removeChild(skill_tree);

			if (select_skill_num == 3 || select_skill_num == 5 || select_skill_num == 7){
				//컴퓨터측 슬라임이벤트 resume
				for (int i = 0; i < 5; i++){
					slime_c[i]->getEventDispatcher()->resumeEventListenersForTarget(slime_c[i], false);
				}
			}
			else if (select_skill_num == 9){
				recovery_hp = temp_slime.increase_hp[3] * property_attack[temp_slime.property_num][target_slime.property_num] + temp_slime.exp * 2;

				for (int i = 0; i < 5; i++){
					user_hp[i] += recovery_hp;
					if (user_hp[i] >= select_slime[i].hp){
						user_hp[i] = select_slime[i].hp;
					}
					if (user_hp[i]>0 && user_hp[i] < select_slime[i].hp){
						hp_bar_u[i]->setTextureRect(Rect(0, 0, user_hp[i] / 100, 5));
					}
				}

				for (int i = 0; i < 5; i++){
					slime_u[i]->getEventDispatcher()->resumeEventListenersForTarget(slime_u[i], false);
				}

				if (com_hp[0] <= 0 && com_hp[1] <= 0 && com_hp[2] <= 0 && com_hp[3] <= 0 && com_hp[4] <= 0){
				//	UserWin();
				}




				//	ComAttackToUser(target->getTag(), 10000 + select_user_slime_tag);
			}
			else if (select_skill_num == 11){
				/*
				auto meteo = Sprite::create("battle/meteo_action.png");
				meteo->setAnchorPoint(Point(0.5, 0.5));
				meteo->setPosition(Point(-50, 1300));
				meteo->setScale(1.3);
				meteo->setTag(19);
				this->addChild(meteo, 10);

				auto action = MoveTo::create(1.0, Point(1100, 100));
				meteo->runAction(action);
				if (action->isDone()){
				this->removeChild(meteo);
				}

				auto meteo1 = Sprite::create("battle/meteo_action.png");
				meteo1->setAnchorPoint(Point(0.5, 0.5));
				meteo1->setPosition(Point(-50, 1200));
				meteo1->setScale(1.2);
				meteo1->setTag(20);
				this->addChild(meteo1, 10);

				auto action0 = DelayTime::create(0.1);
				auto action1 = MoveTo::create(1.0, Point(1000, 100));
				auto action2 = Sequence::create(action0, action1, NULL);
				meteo1->runAction(action2);
				if (action2->isDone()){
				this->removeChild(meteo1);
				}


				auto meteo2 = Sprite::create("battle/meteo_action.png");
				meteo2->setAnchorPoint(Point(0.5, 0.5));
				meteo2->setPosition(Point(-50, 1100));
				meteo2->setScale(1.2);
				meteo2->setTag(21);
				this->addChild(meteo2, 10);

				auto action3 = DelayTime::create(0.2);
				auto action4 = MoveTo::create(1.0, Point(900, 100));
				auto action5 = Sequence::create(action3, action4, NULL);
				meteo2->runAction(action5);
				if (action5->isDone()){
				this->removeChild(meteo2);
				}


				*/

				damage_hp = temp_slime.critical[4] * property_attack[temp_slime.property_num][target_slime.property_num] + temp_slime.exp * 2;
				for (int i = 0; i < 5; i++){
					com_hp[i] -= damage_hp;
					if (com_hp[i]>0){
						hp_bar_c[i]->setTextureRect(Rect(0, 0, com_hp[i] / 100, 5));
					}
					else{
						hp_bar_c[i]->setTextureRect(Rect(0, 0, 0, 5));
					}

				}

				for (int i = 0; i < 5; i++){
					slime_u[i]->getEventDispatcher()->resumeEventListenersForTarget(slime_u[i], false);
				}
				/*
				//stun 상태 표시
				Sprite *stun[5];
				for (int s = 0; s < 5; s++){
					stun[s] = Sprite::create("battle/stun.png");
					stun[s]->setAnchorPoint(Point(0.5, 0.5));
					stun[s]->setScale(0.6*s);
					stun[s]->setTag(22 + s);

					if (s < 3){
						slime_c[s]->setPosition(Point(1100, 550 - (200 * s)));
						this->addChild(stun[s]);
					}
					else{
						slime_c[s]->setPosition(Point(930, 450 - (240 * (s - 3))));
						this->addChild(stun[s]);
					}

				}
				*/
				if (com_hp[0] <= 0 && com_hp[1] <= 0 && com_hp[2] <= 0 && com_hp[3] <= 0 && com_hp[4] <= 0){
					UserWin();
				}
				//		ComAttackToUser(target->getTag(), 10000 + select_user_slime_tag);
			}
		}
		else if (target->getTag() >= 20000 && target->getTag() < 25000){//컴퓨터측 슬라임을 선택하면 그 슬라임을 공격함

			int target_idx = target->getTag() - 20000;
			target_slime = com_slime[target_idx];
			for (int i = 0; i < 5; i++){
				slime_u[i]->getEventDispatcher()->pauseEventListenersForTarget(slime_u[i], false);
				slime_c[i]->getEventDispatcher()->pauseEventListenersForTarget(slime_c[i], false);
			}



			int x = slime_u[select_user_slime_tag]->getPosition().x;
			int y = slime_u[select_user_slime_tag]->getPosition().y;

			auto action1 = MoveTo::create(0.3, Point(slime_c[target_idx]->getPosition().x - 100, slime_c[target_idx]->getPosition().y - 100));
			auto action2 = EaseIn::create(action1, 5.0);
			auto action3 = MoveTo::create(0.3, Point(x, y));
			auto action4 = EaseIn::create(action3, 5.0);
			auto action5 = Sequence::create(action1, action2, action3, action4, NULL);
			slime_u[select_user_slime_tag]->runAction(action5);

			if (select_skill_num == 3)//3이면 할퀴기
			{
				damage_hp = temp_slime.critical[0] * property_attack[temp_slime.property_num][target_slime.property_num] + temp_slime.exp * 2;
				com_hp[target_idx] -= damage_hp;

				if (com_hp[target_idx] > 0){
					hp_bar_c[target_idx]->setTextureRect(Rect(0, 0, com_hp[target_idx] / 100, 5));
				}
				else{
					hp_bar_c[target_idx]->setTextureRect(Rect(0, 0, 0, 5));
				}

				if (com_hp[0] <= 0 && com_hp[1] <= 0 && com_hp[2] <= 0 && com_hp[3] <= 0 && com_hp[4] <= 0){
					UserWin();
				}
			}
			else if (select_skill_num == 5){//5이면 폭탄
				damage_hp = temp_slime.critical[1] * property_attack[temp_slime.property_num][target_slime.property_num] + temp_slime.exp * 2;

				if (target_idx == 0){
					com_hp[target_idx] -= damage_hp;
					com_hp[target_idx + 1] -= damage_hp;

					if (com_hp[target_idx] > 0){
						hp_bar_c[target_idx]->setTextureRect(Rect(0, 0, com_hp[target_idx] / 100, 5));
					}
					else{
						hp_bar_c[target_idx]->setTextureRect(Rect(0, 0, 0, 5));
					}
					if (com_hp[target_idx + 1] > 0){
						hp_bar_c[target_idx + 1]->setTextureRect(Rect(0, 0, com_hp[target_idx] / 100, 5));
					}
					else{
						hp_bar_c[target_idx + 1]->setTextureRect(Rect(0, 0, 0, 5));
					}

				}
				else if (target_idx == 1){
					com_hp[target_idx - 1] -= damage_hp;
					com_hp[target_idx] -= damage_hp;
					com_hp[target_idx + 1] -= damage_hp;

					if (com_hp[target_idx - 1] > 0){
						hp_bar_c[target_idx - 1]->setTextureRect(Rect(0, 0, com_hp[target_idx - 1] / 100, 5));

					}
					else{
						hp_bar_c[target_idx - 1]->setTextureRect(Rect(0, 0, 0, 5));

					}
					if (com_hp[target_idx]>0){
						hp_bar_c[target_idx]->setTextureRect(Rect(0, 0, com_hp[target_idx] / 100, 5));
					}
					else{
						hp_bar_c[target_idx]->setTextureRect(Rect(0, 0, 0, 5));
					}
					if (com_hp[target_idx + 1] > 0){
						hp_bar_c[target_idx + 1]->setTextureRect(Rect(0, 0, com_hp[target_idx] / 100, 5));

					}
					else{
						hp_bar_c[target_idx + 1]->setTextureRect(Rect(0, 0, 0, 5));
					}
				}
				else if (target_idx == 2){
					com_hp[target_idx - 1] -= damage_hp;
					com_hp[target_idx] -= damage_hp;

					if (com_hp[target_idx - 1] > 0){
						hp_bar_c[target_idx - 1]->setTextureRect(Rect(0, 0, com_hp[target_idx - 1] / 100, 5));
					}
					else{
						hp_bar_c[target_idx - 1]->setTextureRect(Rect(0, 0, 0, 5));

					}
					if (com_hp[target_idx]>0){
						hp_bar_c[target_idx]->setTextureRect(Rect(0, 0, com_hp[target_idx] / 100, 5));
					}
					else{
						hp_bar_c[target_idx]->setTextureRect(Rect(0, 0, 0, 5));
					}
				}
				else if (target_idx == 3 || target_idx == 4){
					com_hp[3] -= damage_hp;
					com_hp[4] -= damage_hp;

					if (com_hp[3] > 0){
						hp_bar_c[3]->setTextureRect(Rect(0, 0, com_hp[3] / 100, 5));
					}
					else{
						hp_bar_c[3]->setTextureRect(Rect(0, 0, 0, 5));
					}
					if (com_hp[4] > 0){
						hp_bar_c[4]->setTextureRect(Rect(0, 0, com_hp[4] / 100, 5));
					}
					else{
						hp_bar_c[4]->setTextureRect(Rect(0, 0, 0, 5));
					}
				}


				if (com_hp[0] <= 0 && com_hp[1] <= 0 && com_hp[2] <= 0 && com_hp[3] <= 0 && com_hp[4] <= 0){
					UserWin();
				}
			}
			else if (select_skill_num == 7){//7은 체력뺏어오기
				//temp_slime은 사용자가 선택한 자기 슬라임
				recovery_hp = temp_slime.increase_hp[2] * property_attack[temp_slime.property_num][target_slime.property_num] + temp_slime.exp * 2;
				user_hp[select_user_slime_tag] += recovery_hp;
				if (user_hp[select_user_slime_tag] >= select_slime[select_user_slime_tag].hp){
					user_hp[select_user_slime_tag] = select_slime[select_user_slime_tag].hp;
					hp_bar_u[select_user_slime_tag]->setTextureRect(Rect(0, 0, user_hp[select_user_slime_tag] / 100, 5));
				}
				else{
					hp_bar_u[select_user_slime_tag]->setTextureRect(Rect(0, 0, user_hp[select_user_slime_tag] / 100, 5));
				}
				com_hp[target_idx] -= recovery_hp;

				if (com_hp[target_idx] <= 0){
					//this->removeChildByTag(20000 + target_idx);
					hp_bar_c[target_idx]->setTextureRect(Rect(0, 0, 0, 5));
				}
				else{
					hp_bar_c[target_idx]->setTextureRect(Rect(0, 0, com_hp[target_idx] / 100, 5));
				}

				if (com_hp[0] <= 0 && com_hp[1] <= 0 && com_hp[2] <= 0 && com_hp[3] <= 0 && com_hp[4] <= 0){
					UserWin();
				}

				//	if (user_hp[select_user_slime_tag] > 0 && user_hp[select_user_slime_tag] <= select_slime[select_user_slime_tag].hp){
				//		hp_bar_u[select_user_slime_tag]->setTextureRect(Rect(0, 0, user_hp[select_user_slime_tag]/100, 5));
				//	}

			}
			/*
			else if (select_skill_num == 9){//9는 회복

			recovery_hp = temp_slime.increase_hp[3] * property_attack[temp_slime.property_num][target_slime.property_num] + temp_slime.exp * 2;

			for (int i = 0; i < 5; i++){
			user_hp[i] += recovery_hp;
			if (user_hp[i] >= select_slime[i].hp){
			user_hp[i] = select_slime[i].hp;
			}
			if (user_hp[i]>0 && user_hp[i] <= select_slime[i].hp){
			hp_bar_u[i]->setTextureRect(Rect(0, 0, user_hp[i] / 100, 5));
			}
			}

			}
			else if (select_skill_num == 11){//메테오

			damage_hp = temp_slime.critical[4] * property_attack[temp_slime.property_num][target_slime.property_num] + temp_slime.exp * 2;
			for (int i = 0; i < 5; i++){
			com_hp[i] -= damage_hp;
			if (com_hp[i]>0){
			hp_bar_c[i]->setTextureRect(Rect(0, 0, com_hp[i] / 100, 5));
			}
			else{
			hp_bar_c[i]->setTextureRect(Rect(0, 0, 0, 5));
			}

			}

			}*/

			if (com_hp[0] > 0 || com_hp[1] > 0 || com_hp[2] > 0 || com_hp[3] > 0 || com_hp[4] > 0){
				ComAttackToUser(target->getTag(), 10000 + select_user_slime_tag);
			}

			//	ComAttackToUser(target->getTag(), 10000 + select_user_slime_tag);

		}
		else if (target->getTag() == 3 || target->getTag() == 5 || target->getTag() == 7 || target->getTag() == 9 || target->getTag() == 11){//스킬버튼 클릭 하면 컴퓨터측 슬라임들을 이벤트 리스너에 등록
			select_skill_num = target->getTag();//선택한 스킬을 select_skill_num에 저장하자

			auto skill_tree = (Sprite*)this->getChildByTag(2);
			Sprite *okbtn = Sprite::create("box/button_ok2.png");
			okbtn->setScale(0.9);
			okbtn->setAnchorPoint(Point(0, 0));
			okbtn->setPosition(Point(1100, 50));
			okbtn->setTag(18);

			EventDispatcher * dispatcher_ok = Director::getInstance()->getEventDispatcher();
			auto positionListener_ok = EventListenerTouchOneByOne::create();
			positionListener_ok->setSwallowTouches(false);
			positionListener_ok->onTouchBegan = CC_CALLBACK_2(BattleScene::TouchBegan, this);
			dispatcher_ok->addEventListenerWithSceneGraphPriority(positionListener_ok->clone(), okbtn);

			skill_tree->addChild(okbtn);
		}
		else if (target->getTag()==33){
			Director::getInstance()->replaceScene(Game::createScene());
		}
		else if (target->getTag() == 34){
			Director::getInstance()->replaceScene(MapScene::createScene());
			
		}
		else{
			
		}
	}
	return true;
}

void BattleScene::ComAttackToUser(int com_slime_tag, int user_slime_tag){
	time_t t;

	t = time(NULL);
	int damage_hp = 0;
	int recovery_hp = 0;

	int skill_num = 0;

	int com_slime_idx = com_slime_tag - 20000;
	int user_slime_idx = user_slime_tag - 10000;
	/*
	float rand_s_num = 0;
	rand_s_num = (rand() + t) % 5;
	*/
	int attack_u_slime = 0;
	//temp_slime 사용자측 슬라임 target_slime 컴퓨터측 슬라임

	for (int i = 0; i < 5; i++){
		if (user_hp[i] >0){
			attack_u_slime = i;
			break;
		}
		if (i == 4){
			if (user_hp[i] <= 0){
				UserLose();
			}
		}
	}

	if (com_hp[com_slime_idx]>0){
	
		if (target_slime.rank == 1){//
			float skill_random_num = 0;

			srand(time(NULL) + GetProcessId(NULL));
			skill_random_num = (rand() % 2 + 1);
			skill_num = (int)skill_random_num;

			if (skill_num == 1){//컴퓨터가 사용자 공격 할퀴기
				damage_hp = target_slime.critical[0] * property_attack[select_slime[attack_u_slime].property_num][target_slime.property_num] + target_slime.exp * 2;
				user_hp[attack_u_slime] -= damage_hp;

				if (user_hp[attack_u_slime] > 0){
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, user_hp[attack_u_slime] / 100, 5));
				}
				else{
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, 0, 5));
				}

			}
			else if (skill_num == 2){//폭탄
				damage_hp = target_slime.critical[1] * property_attack[select_slime[attack_u_slime].property_num][target_slime.property_num] + target_slime.exp * 2;
				user_hp[attack_u_slime] -= damage_hp;

				if (user_hp[attack_u_slime] > 0){
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, user_hp[attack_u_slime] / 100, 5));
				}
				else{
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, 0, 5));
				}

			}
			else if (skill_num == 3){// hp 뺏어오기
				recovery_hp = target_slime.increase_hp[2] * property_attack[select_slime[attack_u_slime].property_num][target_slime.property_num] + target_slime.exp * 2;
				com_hp[com_slime_idx] += recovery_hp;

				if (com_hp[com_slime_idx] >= com_slime[com_slime_idx].hp){
					com_hp[com_slime_idx] = com_slime[com_slime_idx].hp;
					hp_bar_c[com_slime_idx]->setTextureRect(Rect(0, 0, com_hp[com_slime_idx] / 100, 5));
				}
				else{
					hp_bar_c[com_slime_idx]->setTextureRect(Rect(0, 0, com_hp[com_slime_idx] / 100, 5));

				}

				user_hp[attack_u_slime] -= recovery_hp;

				if (user_hp[attack_u_slime] <= 0){
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, 0, 5));
				}
				else{
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, user_hp[attack_u_slime] / 100, 5));
				}

			}
			else if (skill_num == 4){
				recovery_hp = target_slime.increase_hp[3] * property_attack[temp_slime.property_num][target_slime.property_num] + target_slime.exp * 2;

				for (int i = 0; i < 5; i++){
					com_hp[i] += recovery_hp;
					if (com_hp[i] >= com_slime[i].hp){
						com_hp[i] = com_slime[i].hp;
					}
					if (com_hp>0 && com_hp[i] <= com_slime[i].hp){
						hp_bar_c[i]->setTextureRect(Rect(0, 0, com_hp[i] / 100, 5));
					}

				}

			}
			else if (skill_num == 5){//컴퓨터가 사용자측에 전체공격(메테오)
				damage_hp = target_slime.critical[4] * property_attack[temp_slime.property_num][target_slime.property_num] + target_slime.exp * 2;

				for (int i = 0; i < 5; i++){
					user_hp[i] -= damage_hp;
					if (user_hp[i]>0){
						hp_bar_u[i]->setTextureRect(Rect(0, 0, user_hp[i] / 100, 5));
					}
					else{
						hp_bar_u[i]->setTextureRect(Rect(0, 0, 0, 5));
					}

				}

			}

		}
		else if (target_slime.rank == 2){//
			float skill_random_num = 0;

			srand(time(NULL) + GetProcessId(NULL));
			skill_random_num = (rand() % 2 + 1);
			skill_num = (int)skill_random_num;

			if (skill_num == 1){//컴퓨터가 사용자 공격
				damage_hp = target_slime.critical[0] * property_attack[select_slime[attack_u_slime].property_num][target_slime.property_num] + target_slime.exp * 2;
				user_hp[attack_u_slime] -= damage_hp;

				if (user_hp[attack_u_slime] > 0){
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, user_hp[attack_u_slime] / 100, 5));
				}
				else{
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, 0, 5));
				}

			}
			else if (skill_num == 2){
				damage_hp = target_slime.critical[1] * property_attack[select_slime[attack_u_slime].property_num][target_slime.property_num] + target_slime.exp * 2;
				user_hp[attack_u_slime] -= damage_hp;

				if (user_hp[attack_u_slime] > 0){
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, user_hp[attack_u_slime] / 100, 5));
				}
				else{
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, 0, 5));
				}

			}
			else if (skill_num == 3){// 컴퓨터가 자기 회복
				recovery_hp = target_slime.increase_hp[2] * property_attack[select_slime[attack_u_slime].property_num][target_slime.property_num] + target_slime.exp * 2;
				com_hp[com_slime_idx] += recovery_hp;

				if (com_hp[com_slime_idx] >= com_slime[com_slime_idx].hp){
					com_hp[com_slime_idx] = com_slime[com_slime_idx].hp;
					hp_bar_c[com_slime_idx]->setTextureRect(Rect(0, 0, com_hp[com_slime_idx] / 100, 5));
				}
				else{
					hp_bar_c[com_slime_idx]->setTextureRect(Rect(0, 0, com_hp[com_slime_idx] / 100, 5));

				}

				user_hp[attack_u_slime] -= recovery_hp;

				if (user_hp[attack_u_slime] <= 0){
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, 0, 5));
				}
				else{
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, user_hp[attack_u_slime] / 100, 5));
				}

			}
			else if (skill_num == 4){
				recovery_hp = target_slime.increase_hp[3] * property_attack[temp_slime.property_num][target_slime.property_num] + target_slime.exp * 2;

				for (int i = 0; i < 5; i++){
					com_hp[i] += recovery_hp;
					if (com_hp[i] >= com_slime[i].hp){
						com_hp[i] = com_slime[i].hp;
					}
					if (com_hp>0 && com_hp[i] <= com_slime[i].hp){
						hp_bar_c[i]->setTextureRect(Rect(0, 0, com_hp[i] / 100, 5));
					}

				}

			}


		}
		else if (target_slime.rank == 3){//
			float skill_random_num = 0;

			srand(time(NULL) + GetProcessId(NULL));
			skill_random_num = (rand() % 2 + 1);
			skill_num = (int)skill_random_num;

			if (skill_num == 1){//컴퓨터가 사용자 공격
				damage_hp = target_slime.critical[0] * property_attack[select_slime[attack_u_slime].property_num][target_slime.property_num] + target_slime.exp * 2;
				user_hp[attack_u_slime] -= damage_hp;

				if (user_hp[attack_u_slime] > 0){
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, user_hp[attack_u_slime] / 100, 5));
				}
				else{
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, 0, 5));
				}

			}
			else if (skill_num == 2){
				damage_hp = target_slime.critical[1] * property_attack[select_slime[attack_u_slime].property_num][target_slime.property_num] + target_slime.exp * 2;
				user_hp[attack_u_slime] -= damage_hp;

				if (user_hp[attack_u_slime] > 0){
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, user_hp[attack_u_slime] / 100, 5));
				}
				else{
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, 0, 5));
				}

			}
			else if (skill_num == 3){// 컴퓨터가 자기 회복
				recovery_hp = target_slime.increase_hp[2] * property_attack[select_slime[attack_u_slime].property_num][target_slime.property_num] + target_slime.exp * 2;
				com_hp[com_slime_idx] += recovery_hp;

				if (com_hp[com_slime_idx] >= com_slime[com_slime_idx].hp){
					com_hp[com_slime_idx] = com_slime[com_slime_idx].hp;
					hp_bar_c[com_slime_idx]->setTextureRect(Rect(0, 0, com_hp[com_slime_idx] / 100, 5));
				}
				else{
					hp_bar_c[com_slime_idx]->setTextureRect(Rect(0, 0, com_hp[com_slime_idx] / 100, 5));

				}

				user_hp[attack_u_slime] -= recovery_hp;

				if (user_hp[attack_u_slime] <= 0){
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, 0, 5));
				}
				else{
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, user_hp[attack_u_slime] / 100, 5));
				}

			}

		}
		else if (target_slime.rank == 4){//
			float skill_random_num = 0;

			srand(time(NULL) + GetProcessId(NULL));
			skill_random_num = (rand() % 2 + 1);
			skill_num = (int)skill_random_num;

			if (skill_num == 1){//컴퓨터가 사용자 공격
				damage_hp = target_slime.critical[0] * property_attack[select_slime[attack_u_slime].property_num][target_slime.property_num] + target_slime.exp * 2;
				user_hp[attack_u_slime] -= damage_hp;

				if (user_hp[attack_u_slime] > 0){
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, user_hp[attack_u_slime] / 100, 5));
				}
				else{
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, 0, 5));
				}

			}
			else if (skill_num == 2){
				damage_hp = target_slime.critical[1] * property_attack[select_slime[attack_u_slime].property_num][target_slime.property_num] + target_slime.exp * 2;
				user_hp[attack_u_slime] -= damage_hp;

				if (user_hp[attack_u_slime] > 0){
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, user_hp[attack_u_slime] / 100, 5));
				}
				else{
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, 0, 5));
				}

			}
			auto my_bottle_label4 = Label::createWithSystemFont(StringUtils::format("user_hp[%d]: %d", attack_u_slime, user_hp[attack_u_slime]), "Thonburi", 50);
			my_bottle_label4->setColor(Color3B::RED);
			my_bottle_label4->setAnchorPoint(Point(0.5, 0.5));
			my_bottle_label4->setPosition(Point(640, 500));
			this->addChild(my_bottle_label4, 9999);
		}
		else if (target_slime.rank == 5){//
			skill_num = 1;

			if (skill_num == 1){//컴퓨터가 사용자 공격
				damage_hp = target_slime.critical[0] * property_attack[select_slime[attack_u_slime].property_num][target_slime.property_num] + target_slime.exp * 2;
				user_hp[attack_u_slime] -= damage_hp;

				if (user_hp[attack_u_slime] > 0){
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, user_hp[attack_u_slime] / 100, 5));
				}
				else{
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, 0, 5));
				}

			}

		}
	
	}
	else{
		for (int c = 0; c < 5; c++){
			if (com_hp[c]>0){
				com_slime_idx = c;
				target_slime = com_slime[com_slime_idx];
			}
		}
		
		if (target_slime.rank == 1){//
			float skill_random_num = 0;

			srand(time(NULL) + GetProcessId(NULL));
			skill_random_num = (rand() % 2 + 1);
			skill_num = (int)skill_random_num;

			if (skill_num == 1){//컴퓨터가 사용자 공격 할퀴기
				damage_hp = target_slime.critical[0] * property_attack[select_slime[attack_u_slime].property_num][target_slime.property_num] + target_slime.exp * 2;
				user_hp[attack_u_slime] -= damage_hp;

				if (user_hp[attack_u_slime] > 0){
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, user_hp[attack_u_slime] / 100, 5));
				}
				else{
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, 0, 5));
				}

			}
			else if (skill_num == 2){//폭탄
				damage_hp = target_slime.critical[1] * property_attack[select_slime[attack_u_slime].property_num][target_slime.property_num] + target_slime.exp * 2;
				user_hp[attack_u_slime] -= damage_hp;

				if (user_hp[attack_u_slime] > 0){
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, user_hp[attack_u_slime] / 100, 5));
				}
				else{
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, 0, 5));
				}

			}
			else if (skill_num == 3){// hp 뺏어오기
				recovery_hp = target_slime.increase_hp[2] * property_attack[select_slime[attack_u_slime].property_num][target_slime.property_num] + target_slime.exp * 2;
				com_hp[com_slime_idx] += recovery_hp;

				if (com_hp[com_slime_idx] >= com_slime[com_slime_idx].hp){
					com_hp[com_slime_idx] = com_slime[com_slime_idx].hp;
					hp_bar_c[com_slime_idx]->setTextureRect(Rect(0, 0, com_hp[com_slime_idx] / 100, 5));
				}
				else{
					hp_bar_c[com_slime_idx]->setTextureRect(Rect(0, 0, com_hp[com_slime_idx] / 100, 5));

				}

				user_hp[attack_u_slime] -= recovery_hp;

				if (user_hp[attack_u_slime] <= 0){
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, 0, 5));
				}
				else{
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, user_hp[attack_u_slime] / 100, 5));
				}

			}
			else if (skill_num == 4){
				recovery_hp = target_slime.increase_hp[3] * property_attack[temp_slime.property_num][target_slime.property_num] + target_slime.exp * 2;

				for (int i = 0; i < 5; i++){
					com_hp[i] += recovery_hp;
					if (com_hp[i] >= com_slime[i].hp){
						com_hp[i] = com_slime[i].hp;
					}
					if (com_hp>0 && com_hp[i] <= com_slime[i].hp){
						hp_bar_c[i]->setTextureRect(Rect(0, 0, com_hp[i] / 100, 5));
					}

				}

			}
			else if (skill_num == 5){//컴퓨터가 사용자측에 전체공격(메테오)
				damage_hp = target_slime.critical[4] * property_attack[temp_slime.property_num][target_slime.property_num] + target_slime.exp * 2;

				for (int i = 0; i < 5; i++){
					user_hp[i] -= damage_hp;
					if (user_hp[i]>0){
						hp_bar_u[i]->setTextureRect(Rect(0, 0, user_hp[i] / 100, 5));
					}
					else{
						hp_bar_u[i]->setTextureRect(Rect(0, 0, 0, 5));
					}

				}

			}

		}
		else if (target_slime.rank == 2){//
			float skill_random_num = 0;

			srand(time(NULL) + GetProcessId(NULL));
			skill_random_num = (rand() % 2 + 1);
			skill_num = (int)skill_random_num;

			if (skill_num == 1){//컴퓨터가 사용자 공격
				damage_hp = target_slime.critical[0] * property_attack[select_slime[attack_u_slime].property_num][target_slime.property_num] + target_slime.exp * 2;
				user_hp[attack_u_slime] -= damage_hp;

				if (user_hp[attack_u_slime] > 0){
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, user_hp[attack_u_slime] / 100, 5));
				}
				else{
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, 0, 5));
				}

			}
			else if (skill_num == 2){
				damage_hp = target_slime.critical[1] * property_attack[select_slime[attack_u_slime].property_num][target_slime.property_num] + target_slime.exp * 2;
				user_hp[attack_u_slime] -= damage_hp;

				if (user_hp[attack_u_slime] > 0){
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, user_hp[attack_u_slime] / 100, 5));
				}
				else{
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, 0, 5));
				}

			}
			else if (skill_num == 3){// 컴퓨터가 자기 회복
				recovery_hp = target_slime.increase_hp[2] * property_attack[select_slime[attack_u_slime].property_num][target_slime.property_num] + target_slime.exp * 2;
				com_hp[com_slime_idx] += recovery_hp;

				if (com_hp[com_slime_idx] >= com_slime[com_slime_idx].hp){
					com_hp[com_slime_idx] = com_slime[com_slime_idx].hp;
					hp_bar_c[com_slime_idx]->setTextureRect(Rect(0, 0, com_hp[com_slime_idx] / 100, 5));
				}
				else{
					hp_bar_c[com_slime_idx]->setTextureRect(Rect(0, 0, com_hp[com_slime_idx] / 100, 5));

				}

				user_hp[attack_u_slime] -= recovery_hp;

				if (user_hp[attack_u_slime] <= 0){
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, 0, 5));
				}
				else{
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, user_hp[attack_u_slime] / 100, 5));
				}

			}
			else if (skill_num == 4){
				recovery_hp = target_slime.increase_hp[3] * property_attack[temp_slime.property_num][target_slime.property_num] + target_slime.exp * 2;

				for (int i = 0; i < 5; i++){
					com_hp[i] += recovery_hp;
					if (com_hp[i] >= com_slime[i].hp){
						com_hp[i] = com_slime[i].hp;
					}
					if (com_hp>0 && com_hp[i] <= com_slime[i].hp){
						hp_bar_c[i]->setTextureRect(Rect(0, 0, com_hp[i] / 100, 5));
					}

				}

			}


		}
		else if (target_slime.rank == 3){//
			float skill_random_num = 0;

			srand(time(NULL) + GetProcessId(NULL));
			skill_random_num = (rand() % 2 + 1);
			skill_num = (int)skill_random_num;

			if (skill_num == 1){//컴퓨터가 사용자 공격
				damage_hp = target_slime.critical[0] * property_attack[select_slime[attack_u_slime].property_num][target_slime.property_num] + target_slime.exp * 2;
				user_hp[attack_u_slime] -= damage_hp;

				if (user_hp[attack_u_slime] > 0){
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, user_hp[attack_u_slime] / 100, 5));
				}
				else{
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, 0, 5));
				}

			}
			else if (skill_num == 2){
				damage_hp = target_slime.critical[1] * property_attack[select_slime[attack_u_slime].property_num][target_slime.property_num] + target_slime.exp * 2;
				user_hp[attack_u_slime] -= damage_hp;

				if (user_hp[attack_u_slime] > 0){
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, user_hp[attack_u_slime] / 100, 5));
				}
				else{
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, 0, 5));
				}

			}
			else if (skill_num == 3){// 컴퓨터가 자기 회복
				recovery_hp = target_slime.increase_hp[2] * property_attack[select_slime[attack_u_slime].property_num][target_slime.property_num] + target_slime.exp * 2;
				com_hp[com_slime_idx] += recovery_hp;

				if (com_hp[com_slime_idx] >= com_slime[com_slime_idx].hp){
					com_hp[com_slime_idx] = com_slime[com_slime_idx].hp;
					hp_bar_c[com_slime_idx]->setTextureRect(Rect(0, 0, com_hp[com_slime_idx] / 100, 5));
				}
				else{
					hp_bar_c[com_slime_idx]->setTextureRect(Rect(0, 0, com_hp[com_slime_idx] / 100, 5));

				}

				user_hp[attack_u_slime] -= recovery_hp;

				if (user_hp[attack_u_slime] <= 0){
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, 0, 5));
				}
				else{
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, user_hp[attack_u_slime] / 100, 5));
				}

			}

		}
		else if (target_slime.rank == 4){//
			float skill_random_num = 0;

			srand(time(NULL) + GetProcessId(NULL));
			skill_random_num = (rand() % 2 + 1);
			skill_num = (int)skill_random_num;

			auto my_bottle_label = Label::createWithSystemFont(StringUtils::format("skill_random_num: %f  \n  skill_num: %d", skill_random_num, skill_num), "Thonburi", 50);
			my_bottle_label->setColor(Color3B::RED);
			my_bottle_label->setAnchorPoint(Point(0.5, 0.5));
			my_bottle_label->setPosition(Point(640, 200));
			this->addChild(my_bottle_label, 9999);

			if (skill_num == 1){//컴퓨터가 사용자 공격
				damage_hp = target_slime.critical[0] * property_attack[select_slime[attack_u_slime].property_num][target_slime.property_num] + target_slime.exp * 2;
				user_hp[attack_u_slime] -= damage_hp;

				if (user_hp[attack_u_slime] > 0){
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, user_hp[attack_u_slime] / 100, 5));
				}
				else{
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, 0, 5));
				}

			}
			else if (skill_num == 2){
				damage_hp = target_slime.critical[1] * property_attack[select_slime[attack_u_slime].property_num][target_slime.property_num] + target_slime.exp * 2;
				user_hp[attack_u_slime] -= damage_hp;

				if (user_hp[attack_u_slime] > 0){
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, user_hp[attack_u_slime] / 100, 5));
				}
				else{
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, 0, 5));
				}

			}
			auto my_bottle_label4 = Label::createWithSystemFont(StringUtils::format("user_hp[%d]: %d", attack_u_slime, user_hp[attack_u_slime]), "Thonburi", 50);
			my_bottle_label4->setColor(Color3B::RED);
			my_bottle_label4->setAnchorPoint(Point(0.5, 0.5));
			my_bottle_label4->setPosition(Point(640, 500));
			this->addChild(my_bottle_label4, 9999);
		}
		else if (target_slime.rank == 5){//
			skill_num = 1;

			if (skill_num == 1){//컴퓨터가 사용자 공격
				damage_hp = target_slime.critical[0] * property_attack[select_slime[attack_u_slime].property_num][target_slime.property_num] + target_slime.exp * 2;
				user_hp[attack_u_slime] -= damage_hp;

				if (user_hp[attack_u_slime] > 0){
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, user_hp[attack_u_slime] / 100, 5));
				}
				else{
					hp_bar_u[attack_u_slime]->setTextureRect(Rect(0, 0, 0, 5));
				}

			}

		}


	}
	


	for (int k = 0; k < 5; k++){
		slime_u[k]->getEventDispatcher()->resumeEventListenersForTarget(slime_u[k], false);
	}

	int x = slime_u[attack_u_slime]->getPosition().x;
	int y = slime_u[attack_u_slime]->getPosition().y;

	auto action11 = DelayTime::create(0.8);
	auto action12 = MoveTo::create(0.3, Point(x, y));
	auto action13 = EaseIn::create(action11, 5.0);
	auto action14 = MoveTo::create(0.3, Point(slime_c[com_slime_idx]->getPosition().x, slime_c[com_slime_idx]->getPosition().y));
	auto action15 = EaseIn::create(action13, 5.0);
	auto action16 = Sequence::create(action11, action12, action13, action14, action15, NULL);
	slime_c[com_slime_idx]->runAction(action16);

}

void BattleScene::UserLose(){
	for (int i = 0; i < 5; i++){//사용자측 슬라임을 선택했으니까 잠시 사용자측 슬라임을 다시 pause
		slime_u[i]->getEventDispatcher()->pauseEventListenersForTarget(slime_u[i], false);
	}
	for (int i = 0; i < 5; i++){//사용자측 슬라임을 선택했으니까 잠시 사용자측 슬라임을 다시 pause
		slime_c[i]->getEventDispatcher()->pauseEventListenersForTarget(slime_c[i], false);
	}


	Sprite * tmp_s[5];

	for (int s = 0; s < 5; s++){
		tmp_s[s] = slime_u[s];
	}

	auto win_label = Label::createWithSystemFont(StringUtils::format("You Lose..."), "Thonburi", 70);
	win_label->setColor(Color3B::BLUE);
	win_label->setAnchorPoint(Point(0.5, 0.5));
	win_label->setPosition(Point(640, 600));

	this->addChild(win_label, 21);

	for (int s = 0; s < 5; s++){
		char str[20] = { "SLIME_EXP" };
		char buffer[100];
		sprintf(buffer, "%d", select_slime[s].my_s_numbering);
		for (int y = 9; y < 20; y++){
			if (buffer[y - 9] != '\0')
				str[y] = buffer[y - 9];
		}
		int beforeEXP = UserDefault::getInstance()->getIntegerForKey(str);
		int afterEXP = beforeEXP + select_stage_num * 10;

		UserDefault::getInstance()->setIntegerForKey(str, afterEXP);
	}
}

void BattleScene::UserWin(){
	Sprite * tmp_s[5];

	for (int s = 0; s < 5; s++){
		tmp_s[s] = slime_u[s];
	}
	

	for (int i = 0; i < 5; i++){//사용자측 슬라임을 선택했으니까 잠시 사용자측 슬라임을 다시 pause
		slime_u[i]->getEventDispatcher()->pauseEventListenersForTarget(slime_u[i], false);
	}
	for (int i = 0; i < 5; i++){//사용자측 슬라임을 선택했으니까 잠시 사용자측 슬라임을 다시 pause
		slime_c[i]->getEventDispatcher()->pauseEventListenersForTarget(slime_c[i], false);
	}

	auto winloselayer = Sprite::create("battle/winloselayer.png");
	winloselayer->setAnchorPoint(Point(0.5, 0.5));
	winloselayer->setPosition(Point(640, 360));
	winloselayer->setScale(1.3);
	winloselayer->setTag(32);
	this->addChild(winloselayer, 20);
	
	auto win_label = Label::createWithSystemFont(StringUtils::format("You Win!"), "Thonburi", 70);
	win_label->setColor(Color3B::BLUE);
	win_label->setAnchorPoint(Point(0.5, 0.5));
	win_label->setPosition(Point(640, 600));

	this->addChild(win_label, 21);

	for (int s = 0; s < 5; s++){
		char str[20] = { "SLIME_EXP" };
		char buffer[100];
		sprintf(buffer, "%d", select_slime[s].my_s_numbering);
		for (int y = 9; y < 20; y++){
			if (buffer[y - 9] != '\0')
				str[y] = buffer[y - 9];
		}
		int beforeEXP=UserDefault::getInstance()->getIntegerForKey(str);
		int afterEXP = beforeEXP + select_stage_num * 150;

		UserDefault::getInstance()->setIntegerForKey(str, afterEXP);
	}
	int money = UserDefault::getInstance()->getIntegerForKey("MONEY");

	float rand_get_money = 0;

	srand(time(NULL) + GetProcessId(NULL));
	rand_get_money= (rand() % 1000 + 1);
	money = money + 1000 + ((int)rand_get_money * select_stage_num);


	UserDefault::getInstance()->setIntegerForKey("MONEY", money);

	if (select_stage_num == UserDefault::getInstance()->getIntegerForKey("SAVE_STAGE_NUM"))
		UserDefault::getInstance()->setIntegerForKey("SAVE_STAGE_NUM", select_stage_num+1);


	auto farm = Sprite::create("battle/farm.png");
	farm->setAnchorPoint(Point(0.5, 0.5));
	farm->setPosition(Point(250, 100));
	farm->setTag(33);

	winloselayer->addChild(farm);

	EventDispatcher * dispatcher_go_farm = Director::getInstance()->getEventDispatcher();
	auto positionListener_go_farm = EventListenerTouchOneByOne::create();
	positionListener_go_farm->setSwallowTouches(false);
	positionListener_go_farm->onTouchBegan = CC_CALLBACK_2(BattleScene::TouchBegan, this);
	dispatcher_go_farm->addEventListenerWithSceneGraphPriority(positionListener_go_farm->clone(), farm);

	
	auto map = Sprite::create("battle/map.png");
	map->setAnchorPoint(Point(0.5, 0.5));
	map->setPosition(Point(450, 100));
	map->setTag(34);

	winloselayer->addChild(map);

	EventDispatcher * dispatcher_go_map = Director::getInstance()->getEventDispatcher();
	auto positionListener_go_map = EventListenerTouchOneByOne::create();
	positionListener_go_map->setSwallowTouches(false);
	positionListener_go_map->onTouchBegan = CC_CALLBACK_2(BattleScene::TouchBegan, this);
	dispatcher_go_map->addEventListenerWithSceneGraphPriority(positionListener_go_map->clone(), map);

	/*
	auto slime_seat_u1 = Sprite::create("map/slimeseat.png");
	slime_seat_u1->setAnchorPoint(Point(0.5, 0.5));
	slime_seat_u1->setScale(0.25);
	slime_seat_u1->setPosition(Point(170, 300));
	slime_seat_u1->setTag(2010);
	winloselayer->addChild(slime_seat_u1);

	tmp_s[0]->setScale(1.5);
	slime_seat_u1->addChild(tmp_s[0]);
//	winloselayer->addChild(tmp_s[0]);


	auto slime_seat_u2 = Sprite::create("map/slimeseat.png");
	slime_seat_u2->setAnchorPoint(Point(0.5, 0.5));
	slime_seat_u2->setScale(0.25);
	slime_seat_u2->setPosition(Point(370, 300));
	slime_seat_u2->setTag(2011);
	winloselayer->addChild(slime_seat_u2);

//	tmp_s[1]->setScale(1.5);
//	winloselayer->addChild(tmp_s[1]);



	auto slime_seat_u3 = Sprite::create("map/slimeseat.png");
	slime_seat_u3->setAnchorPoint(Point(0.5, 0.5));
	slime_seat_u3->setScale(0.25);
	slime_seat_u3->setPosition(Point(570, 300));
	slime_seat_u3->setTag(2012);
	winloselayer->addChild(slime_seat_u3);

//	tmp_s[2]->setScale(1.5);
//	winloselayer->addChild(tmp_s[2]);

	auto slime_seat_u4 = Sprite::create("map/slimeseat.png");
	slime_seat_u4->setAnchorPoint(Point(0.5, 0.5));
	slime_seat_u4->setScale(0.25);
	slime_seat_u4->setPosition(Point(265, 100));
	slime_seat_u4->setTag(2013);
	winloselayer->addChild(slime_seat_u4);

//	tmp_s[3]->setScaleX(1.5);
//	winloselayer->addChild(tmp_s[3]);

	auto slime_seat_u5 = Sprite::create("map/slimeseat.png");
	slime_seat_u5->setAnchorPoint(Point(0.5, 0.5));
	slime_seat_u5->setScale(0.25);
	slime_seat_u5->setPosition(Point(490, 100));
	slime_seat_u5->setTag(2014);

	
	winloselayer->addChild(slime_seat_u5);
//	tmp_s[4]->setScale(1.6);
//	winloselayer->addChild(tmp_s[4]);
	*/
}