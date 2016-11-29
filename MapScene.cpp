#include <stdlib.h>
#include <string.h>
#include "SimpleAudioEngine.h"
#include <time.h>
#include "Language.h"
#include "Game.h"
#include "MapScene.h"
#include "BattleScene.h"


USING_NS_CC;
USING_NS_CC_EXT;

//z�� ����



//tag ����
/*
0: mapbackground sprite�� �ø� layer�� �±�
1~999: �������� ��ȣ(����� 1���� 15����)
1000�뿪: �ݺ��Ǵ� backgroundimage ��������Ʈ���� �±�
2001: map�� ��ü���̾ ���� scrollview
2002: stage�� ������ �����ִ� layer(stagelayer)
2003: stage�� ������ �����ִ� sprite
2004: user�� sidebar sprite
2005: com�� sidebar sprite
2006: user�� sidebar�� scrollview
2007: com�� sidebar�� scrollview
2008: fight ��ư sprite
2009: cancel ��ư sprite
2010~ 2014: ������� slime_seat_u �±׹�ȣ
2015~ 2019: ��ǻ���� slime_seat_c �±׹�ȣ
2020: stagelayer���� stage�� ��ȣ�� �����ִ� label

5000�뿪�� ������� sidebar�� ��ġ�� ������ sprite���� �±׹�ȣ

20000�뿪�� slime_seat_u�� �� �� �����ӵ��� sprite �±� ��ȣ(2000�뿪�� �±� ��ȣ�� ���µ� select_slime[5]�� �� 5�� �̹Ƿ� 20000���� 20004���� ���)
30000�뿪�� slime_seat_c�� �� �� �����ӵ��� sprite �±� ��ȣ

*/

//��޺� �����ӵ��� ����
const int rank1_slime_ea = 2;
const int rank2_slime_ea = 2;
const int rank3_slime_ea = 4;
const int rank4_slime_ea = 4;
const int rank5_slime_ea = 6;
//��޺� �����ӵ��� ���ϳ���
extern char rank1_slime_filename[rank1_slime_ea][50];
extern char rank2_slime_filename[rank2_slime_ea][50];
extern char rank3_slime_filename[rank3_slime_ea][50];
extern char rank4_slime_filename[rank4_slime_ea][50];
extern char rank5_slime_filename[rank5_slime_ea][50];

int save_stage_num = 0;//���� ����ڰ� Ŭ�����ؾ��ϴ� stage number
int select_slime_num = 0;//����ڰ� ������ �������� ����


int switch_stg_btn = 0;//�������� ��ư Ȱ��ȭ:0, ��Ȱ��ȭ:1
LayerColor * stagelayer;//������������ �̺�Ʈ�� �߻����� �� ��Ÿ���� layer

int empty_idx[5];//������ �������� ��Ҹ� �ϸ�(select_slime[2]�� ����ߴٰ� �ϸ�) �� �������� index�� empty_idx[]�� ����(empty_idx[2]=1 �� �Ͽ� 1�̸� select_slime[2]�� ������ ����ٴ� ��)
int tg[5];

//�����ӵ��� �Ӽ����� [7]:�Ӽ�����, [10]:���ڼ�
char property[7][10] = {
	"���", "��", "��", "��", "��", "�ٶ�", "õ��"
};
//�����ӵ��� �Ӽ��� �󼺰���
float property_attack[7][7] = {
	{ 1.0, 1.0, 1.3, 1.0, 1.0, 1.3, 0.8 },
	{ 1.0, 1.0, 0.8, 0.8, 1.3, 1.3, 0.8 },
	{ 1.3, 0.8, 1.0, 1.0, 0.8, 0.8, 0.8 },
	{ 1.0, 0.8, 1.0, 1.0, 0.8, 1.0, 0.8 },
	{ 1.0, 1.3, 0.8, 0.8, 1.0, 0.8, 0.8 },
	{ 1.3, 1.3, 0.8, 1.0, 0.8, 1.0, 0.8 },
	{ 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 1.0 }
};

//�������� ��ũ, �ε���, exp, hp, ��ų ������ ��� ����ü
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

int slime_total_num = 18;


select_s select_slime[5];//������ �����ӵ��� ����ü Ÿ���� �迭�� ���
select_s user_slime_list[5000];
select_s slime[18];//������ �� 18�� 
select_s com_slime[5];//computer�� ������ ������

int u_s_list_numbering[5];


char skill[5][15] = {
	"�⺻����", "��ź����", "ü�½�ƿ", "ȸ��", "���׿�"
};

CocosDenshion::SimpleAudioEngine * pAudio = CocosDenshion::SimpleAudioEngine::getInstance();
bool MapScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//	UserDefault::getInstance()->setIntegerForKey("SAVE_STAGE_NUM", 1);
//	UserDefault::getInstance()->setIntegerForKey("SAVE_STAGE_NUM", 10);
	save_stage_num = UserDefault::getInstance()->getIntegerForKey("SAVE_STAGE_NUM", 1);
	time_t timer;
	struct tm *t;
	/*
	UserDefault::getInstance()->setIntegerForKey("TOTAL_SLIME", 10);

	UserDefault::getInstance()->setIntegerForKey("SLIME_ID5", 45);
	UserDefault::getInstance()->setIntegerForKey("SLIME_EXP5", 100);
	UserDefault::getInstance()->setIntegerForKey("SLIME_RANK5", 4);
	UserDefault::getInstance()->setIntegerForKey("SLIME_INDEX5", 3);

	UserDefault::getInstance()->setIntegerForKey("SLIME_ID6", 48);
	UserDefault::getInstance()->setIntegerForKey("SLIME_EXP6", 100);
	UserDefault::getInstance()->setIntegerForKey("SLIME_RANK6", 3);
	UserDefault::getInstance()->setIntegerForKey("SLIME_INDEX6", 3);

	UserDefault::getInstance()->setIntegerForKey("SLIME_ID4", 44);
	UserDefault::getInstance()->setIntegerForKey("SLIME_EXP4", 100);
	UserDefault::getInstance()->setIntegerForKey("SLIME_RANK4", 3);
	UserDefault::getInstance()->setIntegerForKey("SLIME_INDEX4", 2);

	UserDefault::getInstance()->setIntegerForKey("SLIME_ID3", 43);
	UserDefault::getInstance()->setIntegerForKey("SLIME_EXP3", 100);
	UserDefault::getInstance()->setIntegerForKey("SLIME_RANK3", 3);
	UserDefault::getInstance()->setIntegerForKey("SLIME_INDEX3", 1);

	UserDefault::getInstance()->setIntegerForKey("SLIME_ID2", 42);
	UserDefault::getInstance()->setIntegerForKey("SLIME_EXP2", 100);
	UserDefault::getInstance()->setIntegerForKey("SLIME_RANK2", 3);
	UserDefault::getInstance()->setIntegerForKey("SLIME_INDEX2", 1);

	UserDefault::getInstance()->setIntegerForKey("SLIME_ID1", 41);
	UserDefault::getInstance()->setIntegerForKey("SLIME_EXP1", 100);
	UserDefault::getInstance()->setIntegerForKey("SLIME_RANK1", 3);
	UserDefault::getInstance()->setIntegerForKey("SLIME_INDEX1", 0);

	UserDefault::getInstance()->setIntegerForKey("SLIME_ID7", 49);
	UserDefault::getInstance()->setIntegerForKey("SLIME_EXP7", 100);
	UserDefault::getInstance()->setIntegerForKey("SLIME_RANK7", 1);
	UserDefault::getInstance()->setIntegerForKey("SLIME_INDEX7", 0);

	UserDefault::getInstance()->setIntegerForKey("SLIME_ID8", 50);
	UserDefault::getInstance()->setIntegerForKey("SLIME_EXP8", 100);
	UserDefault::getInstance()->setIntegerForKey("SLIME_RANK8", 2);
	UserDefault::getInstance()->setIntegerForKey("SLIME_INDEX8", 1);

	UserDefault::getInstance()->setIntegerForKey("SLIME_ID9", 51);
	UserDefault::getInstance()->setIntegerForKey("SLIME_EXP9", 100);
	UserDefault::getInstance()->setIntegerForKey("SLIME_RANK9", 1);
	UserDefault::getInstance()->setIntegerForKey("SLIME_INDEX9", 1);

	UserDefault::getInstance()->setIntegerForKey("SLIME_ID10", 52);
	UserDefault::getInstance()->setIntegerForKey("SLIME_EXP10", 100);
	UserDefault::getInstance()->setIntegerForKey("SLIME_RANK10", 2);
	UserDefault::getInstance()->setIntegerForKey("SLIME_INDEX10", 0);
	*/

	timer = time(NULL); // ���� �ð��� �� ������ ���

	t = localtime(&timer); // �� ������ �ð��� �и��Ͽ� ����ü�� �ֱ�

	//int hour = 3;
	//int second = 3;
	int hour = t->tm_hour;
	int second = t->tm_sec;
	//	CocosDenshion::SimpleAudioEngine * pAudio = CocosDenshion::SimpleAudioEngine::getInstance();
	//����ð� ������ ���� ������� ���
	if (hour > 5 && hour < 10){//����
		int type = (rand() * second) % 2;
		if (type == 0){
			pAudio->playBackgroundMusic("music/morning_C_Major_Prelude.mp3", true);

			//	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/morning_C_Major_Prelude.mp3", true);
		}

		else{
			pAudio->playBackgroundMusic("music/morning_lunch_Claudio_The_Worm.mp3", true);
			//	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/morning_lunch_Claudio_The_Worm.mp3", true);

		}

	}
	else if (hour > 10 && hour < 21){//����
		int type = (rand() * second) % 2;
		if (type == 0)
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/afternoon_Get_Outside.mp3", true);
		else
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/afternoon_Good Starts.mp3", true);
	}
	else{//��� ����
		int type = (rand() * second) % 2;
		if (type == 0)
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/evening_Are_You_Sleeping.mp3", true);
		else
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/evening_Twinkle_Twinkle_Little_Star.mp3", true);
	}

	auto background_lay = LayerColor::create(Color4B(255, 255, 255, 255));
	background_lay->setContentSize(Size(1280, 3600));
	background_lay->setTag(0);

	char buffer[100];
	int radix = 10; //10�� ���ڿ��� ��ȯ

	for (int i = 1; i < 6; i++){

		auto backgroundimage = Sprite::create("map/mapbackground.png", Rect(0, 0, 1280, 720));//����̹����� �ݺ��� �� �ֵ��� �̾����� �̹����� �ٲܰ�
		backgroundimage->setAnchorPoint(Point(0, 1));
		backgroundimage->setPosition(Point(0, i * 720));
		backgroundimage->setTag(1000 + i);

		int number1 = 3 * i - 2;
		int number2 = 3 * i - 1;
		int number3 = 3 * i;

		//ù��° �����̹��� + �����̹��� + �̺�Ʈ ������
		auto mapbtn1 = Sprite::create("map/mapbutton.png");
		mapbtn1->setScale(0.5);
		mapbtn1->setAnchorPoint(Point(0.5, 0.5));
		mapbtn1->setPosition(Point(500, 720 * i - 580));//����:-540
		mapbtn1->setTag(number1);

		auto label1 = Label::createWithSystemFont(itoa(number1, buffer, radix), "", 34);
		label1->setAnchorPoint(Point(0.5, 0.5));
		label1->setPosition(Point(500, 720 * i - 580));//����:-540

		EventDispatcher * dispatcher1 = Director::getInstance()->getEventDispatcher();
		auto positionListener1 = EventListenerTouchOneByOne::create();
		positionListener1->setSwallowTouches(false);
		positionListener1->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
		dispatcher1->addEventListenerWithSceneGraphPriority(positionListener1->clone(), mapbtn1);

		//�ι�° �����̹��� + �����̹��� + �̺�Ʈ ������
		auto mapbtn2 = Sprite::create("map/mapbutton.png");
		mapbtn2->setScale(0.5);
		mapbtn2->setAnchorPoint(Point(0.5, 0.5));
		mapbtn2->setPosition(Point(850, 720 * i - 360));//����:1000
		mapbtn2->setTag(number2);

		auto label2 = Label::createWithSystemFont(itoa(number2, buffer, radix), "", 34);
		label2->setAnchorPoint(Point(0.5, 0.5));
		label2->setPosition(Point(850, 720 * i - 360));//����:1000

		EventDispatcher * dispatcher2 = Director::getInstance()->getEventDispatcher();
		auto positionListener2 = EventListenerTouchOneByOne::create();
		positionListener2->setSwallowTouches(false);
		positionListener2->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
		dispatcher2->addEventListenerWithSceneGraphPriority(positionListener2, mapbtn2);


		//����° �����̹��� + �����̹��� + �̺�Ʈ ������
		auto mapbtn3 = Sprite::create("map/mapbutton.png");
		mapbtn3->setScale(0.5);
		mapbtn3->setAnchorPoint(Point(0.5, 0.5));
		mapbtn3->setPosition(Point(500, 720 * i - 120));//����: -180
		mapbtn3->setTag(number3);

		auto label3 = Label::createWithSystemFont(itoa(number3, buffer, radix), "", 34);
		label3->setAnchorPoint(Point(0.5, 0.5));
		label3->setPosition(Point(500, 720 * i - 120));//����: -180

		EventDispatcher * dispatcher3 = Director::getInstance()->getEventDispatcher();
		auto positionListener3 = EventListenerTouchOneByOne::create();
		positionListener3->setSwallowTouches(false);
		positionListener3->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
		dispatcher3->addEventListenerWithSceneGraphPriority(positionListener3, mapbtn3);

		background_lay->addChild(backgroundimage);
		//	layer->addChild(backgroundimage);
		background_lay->addChild(mapbtn1);
		background_lay->addChild(label1);
		background_lay->addChild(mapbtn2);
		background_lay->addChild(label2);
		background_lay->addChild(mapbtn3);
		background_lay->addChild(label3);

	}

	auto scroll = ScrollView::create(Size(1280, 720), background_lay);//layer, background) �� �����Ѱ�?
	scroll->setDirection(ScrollView::Direction::VERTICAL);
	scroll->setBounceable(false);
	scroll->setTag(2001);
	//	scroll->addChild(background_lay);
	this->addChild(scroll);


	settingSkill();
	//skill setting
	/*	int p = 0;
	while (p < slime_total_num){//�����ӵ� �ϳ��ϳ� ��ũ�� �ε��� �� �ɷ�ġ ����

	settingSkill(p);
	p++;

	}
	*/
	/*	for (p = 0; p < 3; p++){
	settingComSlime(p);

	}*/

	return true;
}

Scene* MapScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MapScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	return scene;
}

void MapScene::settingSkill(){
	int p = 0;
	while (p < slime_total_num){
		if (p >= 0 && p <= 1){//p�� 0, 1 �� ���� ��� ��ũ�� 1 �̰�, index�� ���� 0, 1 �̴�.
			slime[p].rank = 1;
			slime[p].index = p;
			slime[p].hp = 20000;
			slime[p].exp = 100;
			slime[p].skill_n = 5;
			slime[p].critical[0] = 10000 * 0.2;//2000
			slime[p].critical[1] = 10000 * 0.25;//2500
			slime[p].increase_hp[2] = 3000;
			slime[p].increase_hp[3] = 4000;
			slime[p].critical[4] = 10000 * 0.5;//5000
			
			if (p == 0){
				slime[p].property = property[2];
				slime[p].property_num = 2;
			}
			else if (p == 1){
				slime[p].property = property[5];
				slime[p].property_num = 5;
			}
			else{}
		}
		else if (p >= 2 && p <= 3){//p�� 2, 3 �϶��� ��� ��ũ�� 2 �̰�, index�� ���� 0, 1 �̴�.
			slime[p].rank = 2;
			slime[p].index = p - 2;
			slime[p].hp = 16000;
			slime[p].exp = 100;
			slime[p].skill_n = 4;
			slime[p].critical[0] = 10000 * 0.2;//2000
			slime[p].critical[1] = 10000 * 0.25;//2500
			slime[p].increase_hp[2] = 3000;
			slime[p].increase_hp[3] = 4000;
			

			if (p == 2){
				slime[p].property = property[2];
				slime[p].property_num = 2;
			}
			else if (p == 3){
				slime[p].property = property[6];
				slime[p].property_num = 6;
			}
		}
		else if (p >= 4 && p <= 7){//p�� 4, 5, 6, 7, �� ���� ��� ��ũ�� 3�̰�, index�� ���� 0, 1, 2, 3 �̴�.
			slime[p].rank = 3;
			slime[p].index = p - 4;
			slime[p].hp = 12000;
			slime[p].exp = 100;
			slime[p].skill_n = 3;
			slime[p].critical[0] = 10000 * 0.2;//2000
			slime[p].critical[1] = 10000 * 0.25;//2500
			slime[p].increase_hp[2] = 3000;
		

			if (p == 4){
				slime[p].property = property[4];
				slime[p].property_num = 4;
			}
			else if (p == 5){
				slime[p].property = property[6];
				slime[p].property_num = 6;
			}
			else if (p == 6){
				slime[p].property = property[0];
				slime[p].property_num = 0;
			}
			else if (p == 7){
				slime[p].property = property[0];
				slime[p].property_num = 0;
			}

		}
		else if (p >= 8 && p <= 11){//p�� 8, 9, 10, 11 �� ���� ��� ��ũ�� 4�̰�, index�� ���� 0, 1, 2, 3 �̴�.
			slime[p].rank = 4;
			slime[p].index = p - 8;
			slime[p].hp = 8000;
			slime[p].exp = 100;
			slime[p].skill_n = 2;
			slime[p].critical[0] = 10000 * 0.2;//2000
			slime[p].critical[1] = 10000 * 0.25;//2500
			

			if (p == 8){
				slime[p].property = property[4];
				slime[p].property_num = 4;
			}
			else if (p == 9){
				slime[p].property = property[3];
				slime[p].property_num = 3;
			}
			else if (p == 10){
				slime[p].property = property[2];
				slime[p].property_num = 2;
			}
			else if (p == 11){
				slime[p].property = property[1];
				slime[p].property_num = 1;
			}
		}
		else if (p >= 12 && p <= 17){//p�� 12, 13, 14, 15, 16, 17 �� ���� ��� ��ũ�� 5�̰�, index�� ���� 0, 1, 2, 3, 4, 5 �̴�.
			slime[p].rank = 5;
			slime[p].index = p - 12;
			slime[p].hp = 4000;
			slime[p].exp = 100;
			slime[p].skill_n = 1;
			slime[p].critical[0] = 10000 * 0.2;//2000
			
			slime[p].property = property[p - 12];
			slime[p].property_num = p - 12;
		}
		else{

		}
		p++;
	}

}


void MapScene::settingComSlime(int p){
	int q = 0;
	//for (p = 0; p < 3; p++){
	if (p % 5 == 1){//  1, 6, 11, 16, ......
		for (q = 0; q < 5; q++){//������ 5�� ���ϱ�
			com_slime[q].rank = slime[slime_total_num - ((q + p) % 6 + 1)].rank;//5���...�̰���? ����� ���� �³�?
			com_slime[q].index = slime[slime_total_num - ((q + p) % 6 + 1)].index;
			com_slime[q].hp = slime[slime_total_num - ((q + p) % 6 + 1)].hp + (p * 500);
			com_slime[q].exp = slime[slime_total_num - ((q + p) % 6 + 1)].exp + 20 * p;
			com_slime[q].skill_n = slime[slime_total_num - ((q + p) % 6 + 1)].skill_n;
			com_slime[q].critical[0] = slime[slime_total_num - ((q + p) % 6 + 1)].critical[0] + (p * 500);
			com_slime[q].property = slime[slime_total_num - ((q + p) % 6 + 1)].property;
			com_slime[q].spr_name = rank5_slime_filename[com_slime[q].index];
			//Touch began �Լ��� if(target->getTag()<16) �κп��� ��ǻ�͵��� ������ �����ֱ� 
		}
	}
	else if (p % 5 == 2){//2, 7, 12, 17, ......
		for (q = 0; q < 5; q++){
			com_slime[q].rank = slime[slime_total_num - 6 - ((q + p) % 4 + 1)].rank;//4���...�̰���? ����� ���� �³�?
			com_slime[q].index = slime[slime_total_num - 6 - ((q + p) % 4 + 1)].index;
			com_slime[q].hp = slime[slime_total_num - 6 - ((q + p) % 4 + 1)].hp + (p * 500);
			com_slime[q].exp = slime[slime_total_num - 6 - ((q + p) % 4 + 1)].exp + 20 * p;
			com_slime[q].skill_n = slime[slime_total_num - 6 - ((q + p) % 4 + 1)].skill_n;
			com_slime[q].critical[0] = slime[slime_total_num - 6 - ((q + p) % 4 + 1)].critical[0] + (p * 500);
			com_slime[q].critical[1] = slime[slime_total_num - 6 - ((q + p) % 4 + 1)].critical[1] + (p * 500);
			com_slime[q].property = slime[slime_total_num - 6 - ((q + p) % 4 + 1)].property;
			com_slime[q].spr_name = rank4_slime_filename[com_slime[q].index];
			//Touch began �Լ��� if(target->getTag()<16) �κп��� ��ǻ�͵��� ������ �����ֱ� 
		}
	}
	else if (p % 5 == 3){//3, 8, 13, 18, .....
		for (q = 0; q < 5; q++){
			com_slime[q].rank = slime[slime_total_num - 10 - ((q + p) % 4 + 1)].rank;//3���...�̰���? ����� ���� �³�?
			com_slime[q].index = slime[slime_total_num - 10 - ((q + p) % 4) + 1].index;
			com_slime[q].hp = slime[slime_total_num - 10 - ((q + p) % 4 + 1)].hp + (p * 500);
			com_slime[q].exp = slime[slime_total_num - 10 - ((q + p) % 4 + 1)].exp + 20 * p;
			com_slime[q].skill_n = slime[slime_total_num - 10 - ((q + p) % 4 + 1)].skill_n;
			com_slime[q].critical[0] = slime[slime_total_num - 10 - ((q + p) % 4 + 1)].critical[0] + (p * 500);
			com_slime[q].critical[1] = slime[slime_total_num - 10 - ((q + p) % 4 + 1)].critical[1] + (p * 500);
			com_slime[q].increase_hp[2] = slime[slime_total_num - 10 - ((q + p) % 4 + 1)].increase_hp[2] + (p * 500);
			com_slime[q].property = slime[slime_total_num - 10 - ((q + p) % 4 + 1)].property;
			com_slime[q].spr_name = rank3_slime_filename[com_slime[q].index];
			//Touch began �Լ��� if(target->getTag()<16) �κп��� ��ǻ�͵��� ������ �����ֱ� 
		}
	}
	else if (p % 5 == 4){//4, 9, 14, 19, ......
		for (q = 0; q < 5; q++){
			com_slime[q].rank = slime[slime_total_num - 14 - ((q + p) % 2 + 1)].rank;//2���...�̰���? ����� ���� �³�?
			com_slime[q].index = slime[slime_total_num - 14 - ((q + p) % 2 + 1)].index;
			com_slime[q].hp = slime[slime_total_num - 14 - ((q + p) % 2 + 1)].hp + (p * 500);
			com_slime[q].exp = slime[slime_total_num - 14 - ((q + p) % 2 + 1)].exp + 20 * p;
			com_slime[q].skill_n = slime[slime_total_num - 14 - ((q + p) % 2 + 1)].skill_n;
			com_slime[q].critical[0] = slime[slime_total_num - 14 - ((q + p) % 2 + 1)].critical[0] + (p * 500);
			com_slime[q].critical[1] = slime[slime_total_num - 14 - ((q + p) % 2 + 1)].critical[1] + (p * 500);
			com_slime[q].increase_hp[2] = slime[slime_total_num - 14 - ((q + p) % 2 + 1)].increase_hp[2] + (p * 500);
			com_slime[q].increase_hp[3] = slime[slime_total_num - 14 - ((q + p) % 2 + 1)].increase_hp[3] + (p * 500);
			com_slime[q].property = slime[slime_total_num - 14 - ((q + p) % 2 + 1)].property;
			com_slime[q].spr_name = rank2_slime_filename[com_slime[q].index];
			//Touch began �Լ��� if(target->getTag()<16) �κп��� ��ǻ�͵��� ������ �����ֱ� 
		}
	}
	else if (p % 5 == 0){//5, 10, 15, 20, ......
		com_slime[0].rank = slime[slime_total_num - 16 - ((q + p) % 2 + 1)].rank;//1���...�̰���? ����� ���� �³�?
		com_slime[0].index = slime[slime_total_num - 16 - ((q + p) % 2 + 1)].index;
		com_slime[0].hp = slime[slime_total_num - 16 - ((q + p) % 2 + 1)].hp * p;
		com_slime[0].exp = slime[slime_total_num - 16 - ((q + p) % 2 + 1)].exp + 20 * p;
		com_slime[0].skill_n = slime[slime_total_num - 16 - ((q + p) % 2 + 1)].skill_n;
		com_slime[0].critical[0] = slime[slime_total_num - 16 - ((q + p) % 2 + 1)].critical[0] + (p * 500);
		com_slime[0].critical[1] = slime[slime_total_num - 16 - ((q + p) % 2 + 1)].critical[1] + (p * 500);
		com_slime[0].increase_hp[2] = slime[slime_total_num - 16 - ((q + p) % 2 + 1)].increase_hp[2] + (p * 500);
		com_slime[0].increase_hp[3] = slime[slime_total_num - 16 - ((q + p) % 2 + 1)].increase_hp[3] + (p * 500);
		com_slime[0].critical[4] = slime[slime_total_num - 16 - ((q + p) % 2 + 1)].critical[4] + (p * 500);
		com_slime[0].property = slime[slime_total_num - 16 - ((q + p) % 2 + 1)].property;
		com_slime[0].spr_name = rank1_slime_filename[com_slime[q].index];
		//Touch began �Լ��� if(target->getTag()<16) �κп��� ��ǻ�͵��� ������ �����ֱ� 
	}
	//	}
}

void MapScene::settingStageLayer(int stage_num){//stagelayer setting
	stagelayer = LayerColor::create(Color4B(0, 0, 0, 0));
	stagelayer->setContentSize(Size(10, 720));//1280, 720
	stagelayer->setTag(2002);

	//�������� ������ �����ִ� ��潺������Ʈ
	auto stageinformation = Sprite::create("map/stageinformationlayer.png", Rect(0, 0, 1280, 720));
	stageinformation->setAnchorPoint(Point(0, 0.5));
	stageinformation->setPosition(Point(125, 190));//640,360
	stageinformation->setScale(1.4);
	stageinformation->setTag(2003);
	stagelayer->addChild(stageinformation);

	//������ �������� ��ȣ�� ������ܿ� ǥ��

	auto my_stage_label = Label::createWithSystemFont(StringUtils::format("STAGE %d", stage_num), "Thonburi", 30);
	my_stage_label->setColor(Color3B::BLACK);
	my_stage_label->setAnchorPoint(Point(0.5, 0.5));
	my_stage_label->setPosition(Point(240, 650));
	my_stage_label->setTag(2020);
	stagelayer->addChild(my_stage_label, 9999);


	//������� sidebar sprite
	auto side_bar1 = Sprite::create("map/stageinformside.png");
	side_bar1->setAnchorPoint(Point(0.5, 0.5));
	side_bar1->setScale(0.7);
	side_bar1->setPosition(Point(235, 360));
	side_bar1->setTag(2004);
	stagelayer->addChild(side_bar1);
	//������� scrollview
	auto side_layer1 = LayerColor::create(Color4B(255, 255, 255, 0));//LayerColor::create(Color4B(255, 255, 255, 255))
	side_layer1->setContentSize(Size(195, 5000));
	side_layer1->setAnchorPoint(Point(0.5, 0.5));
	side_layer1->setPosition(Point(140, 105));
	side_layer1->setTag(2021);

	auto scroll1 = ScrollView::create(Size(190, 490), side_layer1);//��ũ�� �Ǵ� ��ü�� side_layer
	scroll1->setDirection(ScrollView::Direction::VERTICAL);
	scroll1->setBounceable(true);
	scroll1->setAnchorPoint(Point(0.5, 0.5));
	scroll1->setPosition(Point(140, 115));
	scroll1->setTag(2006);
	//scroll1->setSwallowsTouches(false);

	stagelayer->addChild(scroll1);


	//������� ������ �ڸ�
	//1
	auto slime_seat_u1 = Sprite::create("map/slimeseat.png");
	slime_seat_u1->setAnchorPoint(Point(0.5, 0.5));
	slime_seat_u1->setScale(0.2);
	slime_seat_u1->setPosition(Point(560, 510));
	slime_seat_u1->setTag(2010);
	stagelayer->addChild(slime_seat_u1);


	auto slime_seat_u2 = Sprite::create("map/slimeseat.png");
	slime_seat_u2->setAnchorPoint(Point(0.5, 0.5));
	slime_seat_u2->setScale(0.2);
	slime_seat_u2->setPosition(Point(560, 360));
	slime_seat_u2->setTag(2011);
	stagelayer->addChild(slime_seat_u2);


	auto slime_seat_u3 = Sprite::create("map/slimeseat.png");
	slime_seat_u3->setAnchorPoint(Point(0.5, 0.5));
	slime_seat_u3->setScale(0.2);
	slime_seat_u3->setPosition(Point(560, 210));
	slime_seat_u3->setTag(2012);
	stagelayer->addChild(slime_seat_u3);


	auto slime_seat_u4 = Sprite::create("map/slimeseat.png");
	slime_seat_u4->setAnchorPoint(Point(0.5, 0.5));
	slime_seat_u4->setScale(0.2);
	slime_seat_u4->setPosition(Point(410, 440));
	slime_seat_u4->setTag(2013);
	stagelayer->addChild(slime_seat_u4);


	auto slime_seat_u5 = Sprite::create("map/slimeseat.png");
	slime_seat_u5->setAnchorPoint(Point(0.5, 0.5));
	slime_seat_u5->setScale(0.2);
	slime_seat_u5->setPosition(Point(410, 280));
	slime_seat_u5->setTag(2014);
	stagelayer->addChild(slime_seat_u5);

	//sidebar�� UserDefault�� ����Ǿ��ִ� ��� �����ӵ��� ��ġ�ϱ�
	//side_layer1�� ����ڰ� �����ϰ� �ִ� �����ӵ��� �ֱ�
	for (int j = 0; j < UserDefault::getInstance()->getIntegerForKey("TOTAL_SLIME", 0); j++){
		

		//������ ����
		char tag_buffer[20];
		sprintf(tag_buffer, "%d", j + 1);
		char slime_rank[20] = { "SLIME_RANK" };
		for (int y = 10; y < 20; y++){
			if (tag_buffer[y - 10] != '\0')
				slime_rank[y] = tag_buffer[y - 10];
		}

		char tag_buffer2[20];
		sprintf(tag_buffer2, "%d", j + 1);
		char slime_index[20] = { "SLIME_INDEX" };
		for (int y = 11; y < 20; y++){
			if (tag_buffer2[y - 11] != '\0')
				slime_index[y] = tag_buffer2[y - 11];
		}
		
		char tag_buffer3[20];
		sprintf(tag_buffer3, "%d", j + 1);
		char slime_id[20] = { "SLIME_ID" };
		for (int y = 8; y < 20; y++){
			if (tag_buffer3[y - 8] != '\0')
				slime_id[y] = tag_buffer3[y - 8];
		}
		int load_slime_rank = UserDefault::getInstance()->getIntegerForKey(slime_rank, 0);   //j��° �������� ��ũ��
		int load_slime_index = UserDefault::getInstance()->getIntegerForKey(slime_index, 0);   //j��° �������� �ε�����
		int random_id = UserDefault::getInstance()->getIntegerForKey(slime_id, 0);   //j��° �������� ID��

		auto slime_list = Sprite::create("map/slimeseat.png");
		slime_list->setAnchorPoint(Point(0.5, 0.5));
		slime_list->setScale(0.25);
		slime_list->setPosition(Point(97, 90 + 180 * j));


		user_slime_list[random_id].rank = load_slime_rank;
		user_slime_list[random_id].index = load_slime_index;
		user_slime_list[random_id].id = random_id;
		user_slime_list[random_id].my_s_numbering = j + 1;
		//	user_slime_list[j].exp = UserDefault::getInstance()->getIntegerForKey("")//���� Game.cpp���� exp �� ���� �κ��� �����Ǿ� ���� �ʴ�.
		if (load_slime_rank == 1){
			user_slime_list[random_id].hp = slime[0].hp;
			user_slime_list[random_id].skill_n = slime[0].skill_n;
			user_slime_list[random_id].critical[0] = slime[0].critical[0];
			user_slime_list[random_id].critical[1] = slime[0].critical[1];
			user_slime_list[random_id].increase_hp[2] = slime[0].increase_hp[2];
			user_slime_list[random_id].increase_hp[3] = slime[0].increase_hp[3];
			user_slime_list[random_id].critical[4] = slime[0].critical[4];
			if (load_slime_index == 0){
				user_slime_list[random_id].property = slime[0].property;
				user_slime_list[random_id].spr_name = rank1_slime_filename[user_slime_list[random_id].index];


				auto new_slime = Sprite::create(StringUtils::format(rank1_slime_filename[load_slime_index], 1));
				new_slime->setAnchorPoint(Point(-0.25, -0.1));
				new_slime->setScale(4.5);
				new_slime->setTag(5000 + random_id);

				slime_list->addChild(new_slime);
				side_layer1->addChild(slime_list);

				EventDispatcher * dispatcher_slime = Director::getInstance()->getEventDispatcher();
				auto positionListener_slime = EventListenerTouchOneByOne::create();
				positionListener_slime->setSwallowTouches(false);
				positionListener_slime->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
				dispatcher_slime->addEventListenerWithSceneGraphPriority(positionListener_slime->clone(), new_slime);//slime_list
				//		dispatcher_slime->setPriority(positionListener_slime, 5000 + random_id);


			}
			else if (load_slime_index == 1){
				user_slime_list[random_id].property = slime[1].property;
				user_slime_list[random_id].spr_name = rank1_slime_filename[user_slime_list[random_id].index];

				auto new_slime = Sprite::create(StringUtils::format(rank1_slime_filename[load_slime_index], 1));

				new_slime->setAnchorPoint(Point(-0.25, -0.1));
				new_slime->setScale(4.5);
				new_slime->setTag(5000 + random_id);

				slime_list->addChild(new_slime);
				side_layer1->addChild(slime_list);

				EventDispatcher * dispatcher_slime = Director::getInstance()->getEventDispatcher();
				auto positionListener_slime = EventListenerTouchOneByOne::create();
				positionListener_slime->setSwallowTouches(false);
				positionListener_slime->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
				dispatcher_slime->addEventListenerWithSceneGraphPriority(positionListener_slime->clone(), new_slime);//slime_list
			}
			else{}
		}
		else if (load_slime_rank == 2){
			user_slime_list[random_id].hp = slime[2].hp;
			user_slime_list[random_id].skill_n = slime[2].skill_n;
			user_slime_list[random_id].critical[0] = slime[2].critical[0];
			user_slime_list[random_id].critical[1] = slime[2].critical[1];
			user_slime_list[random_id].increase_hp[2] = slime[2].increase_hp[2];
			user_slime_list[random_id].increase_hp[3] = slime[2].increase_hp[3];
			if (load_slime_index == 0){
				user_slime_list[random_id].property = slime[2].property;
				user_slime_list[random_id].spr_name = rank2_slime_filename[user_slime_list[random_id].index];

				auto new_slime = Sprite::create(StringUtils::format(rank2_slime_filename[load_slime_index], 1));

				new_slime->setAnchorPoint(Point(-0.25, -0.1));
				new_slime->setScale(4.5);
				new_slime->setTag(5000 + random_id);

				slime_list->addChild(new_slime);
				side_layer1->addChild(slime_list);


				EventDispatcher * dispatcher_slime = Director::getInstance()->getEventDispatcher();
				auto positionListener_slime = EventListenerTouchOneByOne::create();
				positionListener_slime->setSwallowTouches(false);
				positionListener_slime->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
				dispatcher_slime->addEventListenerWithSceneGraphPriority(positionListener_slime->clone(), new_slime);//slime_list
			}
			else if (load_slime_index == 1){
				user_slime_list[random_id].property = slime[3].property;
				user_slime_list[random_id].spr_name = rank2_slime_filename[user_slime_list[random_id].index];

				auto new_slime = Sprite::create(StringUtils::format(rank2_slime_filename[load_slime_index], 1));

				new_slime->setAnchorPoint(Point(-0.25, -0.1));
				new_slime->setScale(4.5);
				new_slime->setTag(5000 + random_id);

				slime_list->addChild(new_slime);
				side_layer1->addChild(slime_list);


				EventDispatcher * dispatcher_slime = Director::getInstance()->getEventDispatcher();
				auto positionListener_slime = EventListenerTouchOneByOne::create();
				positionListener_slime->setSwallowTouches(false);
				positionListener_slime->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
				dispatcher_slime->addEventListenerWithSceneGraphPriority(positionListener_slime->clone(), new_slime);//slime_list
			}
			else{}
		}
		else if (load_slime_rank == 3){
			user_slime_list[random_id].hp = slime[4].hp;
			user_slime_list[random_id].skill_n = slime[4].skill_n;
			user_slime_list[random_id].critical[0] = slime[4].critical[0];
			user_slime_list[random_id].critical[1] = slime[4].critical[1];
			user_slime_list[random_id].increase_hp[2] = slime[4].increase_hp[2];

			if (load_slime_index == 0){
				user_slime_list[random_id].property = slime[4].property;
				user_slime_list[random_id].spr_name = rank3_slime_filename[user_slime_list[random_id].index];

				auto new_slime = Sprite::create(StringUtils::format(rank3_slime_filename[load_slime_index], 1));

				new_slime->setAnchorPoint(Point(-0.25, -0.1));
				new_slime->setScale(4.5);
				new_slime->setTag(5000 + random_id);

				slime_list->addChild(new_slime);
				side_layer1->addChild(slime_list);

				EventDispatcher * dispatcher_slime = Director::getInstance()->getEventDispatcher();
				auto positionListener_slime = EventListenerTouchOneByOne::create();
				positionListener_slime->setSwallowTouches(false);
				positionListener_slime->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
				dispatcher_slime->addEventListenerWithSceneGraphPriority(positionListener_slime->clone(), new_slime);//slime_list
			}
			else if (load_slime_index == 1){
				user_slime_list[random_id].property = slime[5].property;
				user_slime_list[random_id].spr_name = rank3_slime_filename[user_slime_list[random_id].index];

				auto new_slime = Sprite::create(StringUtils::format(rank3_slime_filename[load_slime_index], 1));

				new_slime->setAnchorPoint(Point(-0.25, -0.1));
				new_slime->setScale(4.5);
				new_slime->setTag(5000 + random_id);

				slime_list->addChild(new_slime);
				side_layer1->addChild(slime_list);


				EventDispatcher * dispatcher_slime = Director::getInstance()->getEventDispatcher();
				auto positionListener_slime = EventListenerTouchOneByOne::create();
				positionListener_slime->setSwallowTouches(false);
				positionListener_slime->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
				dispatcher_slime->addEventListenerWithSceneGraphPriority(positionListener_slime->clone(), new_slime);//slime_list
			}
			else if (load_slime_index == 2){
				user_slime_list[random_id].property = slime[6].property;
				user_slime_list[random_id].spr_name = rank3_slime_filename[user_slime_list[random_id].index];

				auto new_slime = Sprite::create(StringUtils::format(rank3_slime_filename[load_slime_index], 1));

				new_slime->setAnchorPoint(Point(-0.25, -0.1));
				new_slime->setScale(4.5);
				new_slime->setTag(5000 + random_id);

				slime_list->addChild(new_slime);
				side_layer1->addChild(slime_list);



				EventDispatcher * dispatcher_slime = Director::getInstance()->getEventDispatcher();
				auto positionListener_slime = EventListenerTouchOneByOne::create();
				positionListener_slime->setSwallowTouches(false);
				positionListener_slime->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
				dispatcher_slime->addEventListenerWithSceneGraphPriority(positionListener_slime->clone(), new_slime);//slime_list
			}
			else if (load_slime_index == 3){
				user_slime_list[random_id].property = slime[7].property;
				user_slime_list[random_id].spr_name = rank3_slime_filename[user_slime_list[random_id].index];

				auto new_slime = Sprite::create(StringUtils::format(rank3_slime_filename[load_slime_index], 1));

				new_slime->setAnchorPoint(Point(-0.25, -0.1));
				new_slime->setScale(4.5);
				new_slime->setTag(5000 + random_id);

				slime_list->addChild(new_slime);
				side_layer1->addChild(slime_list);


				EventDispatcher * dispatcher_slime = Director::getInstance()->getEventDispatcher();
				auto positionListener_slime = EventListenerTouchOneByOne::create();
				positionListener_slime->setSwallowTouches(false);
				positionListener_slime->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
				dispatcher_slime->addEventListenerWithSceneGraphPriority(positionListener_slime->clone(), new_slime);//slime_list
			}
			else {

			}
		}
		else if (load_slime_rank == 4){
			user_slime_list[random_id].hp = slime[8].hp;
			user_slime_list[random_id].skill_n = slime[8].skill_n;
			user_slime_list[random_id].critical[0] = slime[8].critical[0];
			user_slime_list[random_id].critical[1] = slime[8].critical[1];


			if (load_slime_index == 0){
				user_slime_list[random_id].property = slime[8].property;
				user_slime_list[random_id].spr_name = rank4_slime_filename[user_slime_list[random_id].index];

				auto new_slime = Sprite::create(StringUtils::format(rank4_slime_filename[load_slime_index], 1));

				new_slime->setAnchorPoint(Point(-0.25, -0.1));
				new_slime->setScale(4.5);
				new_slime->setTag(5000 + random_id);

				slime_list->addChild(new_slime);
				side_layer1->addChild(slime_list);


				EventDispatcher * dispatcher_slime = Director::getInstance()->getEventDispatcher();
				auto positionListener_slime = EventListenerTouchOneByOne::create();
				positionListener_slime->setSwallowTouches(false);
				positionListener_slime->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
				dispatcher_slime->addEventListenerWithSceneGraphPriority(positionListener_slime->clone(), new_slime);//slime_list
			}
			else if (load_slime_index == 1){
				user_slime_list[random_id].property = slime[9].property;
				user_slime_list[random_id].spr_name = rank4_slime_filename[user_slime_list[random_id].index];

				auto new_slime = Sprite::create(StringUtils::format(rank4_slime_filename[load_slime_index], 1));

				new_slime->setAnchorPoint(Point(-0.25, -0.1));
				new_slime->setScale(4.5);
				new_slime->setTag(5000 + random_id);

				slime_list->addChild(new_slime);
				side_layer1->addChild(slime_list);

				EventDispatcher * dispatcher_slime = Director::getInstance()->getEventDispatcher();
				auto positionListener_slime = EventListenerTouchOneByOne::create();
				positionListener_slime->setSwallowTouches(false);
				positionListener_slime->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
				dispatcher_slime->addEventListenerWithSceneGraphPriority(positionListener_slime->clone(), new_slime);//slime_list
			}
			else if (load_slime_index == 2){
				user_slime_list[random_id].property = slime[10].property;
				user_slime_list[random_id].spr_name = rank4_slime_filename[user_slime_list[random_id].index];

				auto new_slime = Sprite::create(StringUtils::format(rank4_slime_filename[load_slime_index], 1));

				new_slime->setAnchorPoint(Point(-0.25, -0.1));
				new_slime->setScale(4.5);
				new_slime->setTag(5000 + random_id);

				slime_list->addChild(new_slime);
				side_layer1->addChild(slime_list);


				EventDispatcher * dispatcher_slime = Director::getInstance()->getEventDispatcher();
				auto positionListener_slime = EventListenerTouchOneByOne::create();
				positionListener_slime->setSwallowTouches(false);
				positionListener_slime->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
				dispatcher_slime->addEventListenerWithSceneGraphPriority(positionListener_slime->clone(), new_slime);//slime_list
			}
			else if (load_slime_index == 3){
				user_slime_list[random_id].property = slime[11].property;
				user_slime_list[random_id].spr_name = rank4_slime_filename[user_slime_list[random_id].index];

				auto new_slime = Sprite::create(StringUtils::format(rank4_slime_filename[load_slime_index], 1));

				new_slime->setAnchorPoint(Point(-0.25, -0.1));
				new_slime->setScale(4.5);
				new_slime->setTag(5000 + random_id);

				slime_list->addChild(new_slime);
				side_layer1->addChild(slime_list);


				EventDispatcher * dispatcher_slime = Director::getInstance()->getEventDispatcher();
				auto positionListener_slime = EventListenerTouchOneByOne::create();
				positionListener_slime->setSwallowTouches(false);
				positionListener_slime->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
				dispatcher_slime->addEventListenerWithSceneGraphPriority(positionListener_slime->clone(), new_slime);//slime_list
			}
			else {

			}
		}
		else if (load_slime_rank == 5){
			user_slime_list[random_id].hp = slime[12].hp;
			user_slime_list[random_id].skill_n = slime[12].skill_n;
			user_slime_list[random_id].critical[0] = slime[12].critical[0];
			user_slime_list[random_id].critical[1] = slime[12].critical[1];


			if (load_slime_index == 0){
				user_slime_list[random_id].property = slime[12].property;
				user_slime_list[random_id].spr_name = rank5_slime_filename[user_slime_list[random_id].index];

				auto new_slime = Sprite::create(StringUtils::format(rank5_slime_filename[load_slime_index], 1));

				new_slime->setAnchorPoint(Point(-0.25, -0.1));
				new_slime->setScale(4.5);
				new_slime->setTag(5000 + random_id);

				slime_list->addChild(new_slime);
				side_layer1->addChild(slime_list);


				EventDispatcher * dispatcher_slime = Director::getInstance()->getEventDispatcher();
				auto positionListener_slime = EventListenerTouchOneByOne::create();
				positionListener_slime->setSwallowTouches(false);
				positionListener_slime->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
				dispatcher_slime->addEventListenerWithSceneGraphPriority(positionListener_slime->clone(), new_slime);//slime_list
			}
			else if (load_slime_index == 1){
				user_slime_list[random_id].property = slime[13].property;
				user_slime_list[random_id].spr_name = rank5_slime_filename[user_slime_list[random_id].index];

				auto new_slime = Sprite::create(StringUtils::format(rank5_slime_filename[load_slime_index], 1));

				new_slime->setAnchorPoint(Point(-0.25, -0.1));
				new_slime->setScale(4.5);
				new_slime->setTag(5000 + random_id);

				slime_list->addChild(new_slime);
				side_layer1->addChild(slime_list);


				EventDispatcher * dispatcher_slime = Director::getInstance()->getEventDispatcher();
				auto positionListener_slime = EventListenerTouchOneByOne::create();
				positionListener_slime->setSwallowTouches(false);
				positionListener_slime->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
				dispatcher_slime->addEventListenerWithSceneGraphPriority(positionListener_slime->clone(), new_slime);//slime_list
			}
			else if (load_slime_index == 2){
				user_slime_list[random_id].property = slime[14].property;
				user_slime_list[random_id].spr_name = rank5_slime_filename[user_slime_list[random_id].index];

				auto new_slime = Sprite::create(StringUtils::format(rank5_slime_filename[load_slime_index], 1));

				new_slime->setAnchorPoint(Point(-0.25, -0.1));
				new_slime->setScale(4.5);
				new_slime->setTag(5000 + random_id);

				slime_list->addChild(new_slime);
				side_layer1->addChild(slime_list);


				EventDispatcher * dispatcher_slime = Director::getInstance()->getEventDispatcher();
				auto positionListener_slime = EventListenerTouchOneByOne::create();
				positionListener_slime->setSwallowTouches(false);
				positionListener_slime->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
				dispatcher_slime->addEventListenerWithSceneGraphPriority(positionListener_slime->clone(), new_slime);//slime_list
			}
			else if (load_slime_index == 3){
				user_slime_list[random_id].property = slime[15].property;
				user_slime_list[random_id].spr_name = rank5_slime_filename[user_slime_list[random_id].index];

				auto new_slime = Sprite::create(StringUtils::format(rank5_slime_filename[load_slime_index], 1));

				new_slime->setAnchorPoint(Point(-0.25, -0.1));
				new_slime->setScale(4.5);
				new_slime->setTag(5000 + random_id);

				slime_list->addChild(new_slime);
				side_layer1->addChild(slime_list);


				EventDispatcher * dispatcher_slime = Director::getInstance()->getEventDispatcher();
				auto positionListener_slime = EventListenerTouchOneByOne::create();
				positionListener_slime->setSwallowTouches(false);
				positionListener_slime->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
				dispatcher_slime->addEventListenerWithSceneGraphPriority(positionListener_slime->clone(), new_slime);//slime_list
			}
			else if (load_slime_index == 4){
				user_slime_list[random_id].property = slime[16].property;
				user_slime_list[random_id].spr_name = rank5_slime_filename[user_slime_list[random_id].index];

				auto new_slime = Sprite::create(StringUtils::format(rank5_slime_filename[load_slime_index], 1));

				new_slime->setAnchorPoint(Point(-0.25, -0.1));
				new_slime->setScale(4.5);
				new_slime->setTag(5000 + random_id);

				slime_list->addChild(new_slime);
				side_layer1->addChild(slime_list);


				EventDispatcher * dispatcher_slime = Director::getInstance()->getEventDispatcher();
				auto positionListener_slime = EventListenerTouchOneByOne::create();
				positionListener_slime->setSwallowTouches(false);
				positionListener_slime->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
				dispatcher_slime->addEventListenerWithSceneGraphPriority(positionListener_slime->clone(), new_slime);//slime_list
			}
			else if (load_slime_index == 5){
				user_slime_list[random_id].property = slime[17].property;
				user_slime_list[random_id].spr_name = rank5_slime_filename[user_slime_list[random_id].index];

				auto new_slime = Sprite::create(StringUtils::format(rank5_slime_filename[load_slime_index], 1));

				new_slime->setAnchorPoint(Point(-0.25, -0.1));
				new_slime->setScale(4.5);
				new_slime->setTag(5000 + random_id);

				slime_list->addChild(new_slime);
				side_layer1->addChild(slime_list);


				EventDispatcher * dispatcher_slime = Director::getInstance()->getEventDispatcher();
				auto positionListener_slime = EventListenerTouchOneByOne::create();
				positionListener_slime->setSwallowTouches(false);
				positionListener_slime->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
				dispatcher_slime->addEventListenerWithSceneGraphPriority(positionListener_slime->clone(), new_slime);//slime_list

			}
			else{}

		}
		else{

		}

		//��������� sidebar�� UserDefault�� ����Ǿ��ִ� ��� �����ӵ��� ��ġ�� �κ�
	}




	//��ǻ���� sidebar sprite
	auto side_bar2 = Sprite::create("map/stageinformside.png");
	side_bar2->setAnchorPoint(Point(0.5, 0.5));
	side_bar2->setScale(0.7);
	side_bar2->setPosition(Point(1045, 360));
	side_bar2->setTag(2005);
	stagelayer->addChild(side_bar2);
	/*
	//com �� side_layer
	auto side_layer2 = LayerColor::create(Color4B(255, 255, 255, 0));//LayerColor::create(Color4B(255, 255, 255, 255))
	side_layer2->setContentSize(Size(195, 3000));
	side_layer2->setAnchorPoint(Point(0.5, 0.5));
	side_layer2->setPosition(Point(945, 105));
	side_layer2->setTag(2022);
	//��ǻ���� sidebar scrollview
	auto scroll2 = ScrollView::create(Size(190, 490), side_layer2);//��ũ�� �Ǵ� ��ü�� side_layer
	scroll2->setDirection(ScrollView::Direction::VERTICAL);
	scroll2->setBounceable(true);
	scroll2->setAnchorPoint(Point(0.5, 0.5));
	scroll2->setPosition(Point(950, 115));
	scroll2->setTag(2007);

	stagelayer->addChild(scroll2);
	*/



	//��ǻ���� ������ �ڸ�
	auto slime_seat_c1 = Sprite::create("map/slimeseat.png");
	slime_seat_c1->setAnchorPoint(Point(0.5, 0.5));
	slime_seat_c1->setScale(0.2);
	slime_seat_c1->setPosition(Point(720, 510));
	slime_seat_c1->setTag(30);
	//	stagelayer->addChild(slime_seat_c1);

	auto slime_seat_c2 = Sprite::create("map/slimeseat.png");
	slime_seat_c2->setAnchorPoint(Point(0.5, 0.5));
	slime_seat_c2->setScale(0.2);
	slime_seat_c2->setPosition(Point(720, 360));
	slime_seat_c1->setTag(31);
	//	stagelayer->addChild(slime_seat_c2);


	auto slime_seat_c3 = Sprite::create("map/slimeseat.png");
	slime_seat_c3->setAnchorPoint(Point(0.5, 0.5));
	slime_seat_c3->setScale(0.2);
	slime_seat_c3->setPosition(Point(720, 210));
	slime_seat_c1->setTag(32);
	//	stagelayer->addChild(slime_seat_c3);

	auto slime_seat_c4 = Sprite::create("map/slimeseat.png");
	slime_seat_c4->setAnchorPoint(Point(0.5, 0.5));
	slime_seat_c4->setScale(0.2);
	slime_seat_c4->setPosition(Point(870, 440));
	slime_seat_c1->setTag(33);
	//	stagelayer->addChild(slime_seat_c4);

	auto slime_seat_c5 = Sprite::create("map/slimeseat.png");
	slime_seat_c5->setAnchorPoint(Point(0.5, 0.5));
	slime_seat_c5->setScale(0.2);
	slime_seat_c5->setPosition(Point(870, 280));
	slime_seat_c1->setTag(34);
	//	stagelayer->addChild(slime_seat_c5);

	settingComSlime(stage_num);

	if (stage_num % 5 != 0){
		for (int n = 0; n < 5; n++){
			auto new_slime_c = Sprite::create(StringUtils::format(com_slime[n].spr_name, 1));
			new_slime_c->setScale(5.0);
			new_slime_c->setAnchorPoint(Point(-0.25, -0.1));
			new_slime_c->setTag(30000 + n);
			if (n == 0){
				slime_seat_c1->addChild(new_slime_c);
				EventDispatcher * dispatcher_slime = Director::getInstance()->getEventDispatcher();
				auto positionListener_slime = EventListenerTouchOneByOne::create();
				positionListener_slime->setSwallowTouches(false);
				positionListener_slime->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
				dispatcher_slime->addEventListenerWithSceneGraphPriority(positionListener_slime->clone(), new_slime_c);
			}
			else if (n == 1){
				slime_seat_c2->addChild(new_slime_c);
				EventDispatcher * dispatcher_slime = Director::getInstance()->getEventDispatcher();
				auto positionListener_slime = EventListenerTouchOneByOne::create();
				positionListener_slime->setSwallowTouches(false);
				positionListener_slime->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
				dispatcher_slime->addEventListenerWithSceneGraphPriority(positionListener_slime->clone(), new_slime_c);
			}
			else if (n == 2){
				slime_seat_c3->addChild(new_slime_c);
				EventDispatcher * dispatcher_slime = Director::getInstance()->getEventDispatcher();
				auto positionListener_slime = EventListenerTouchOneByOne::create();
				positionListener_slime->setSwallowTouches(false);
				positionListener_slime->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
				dispatcher_slime->addEventListenerWithSceneGraphPriority(positionListener_slime->clone(), new_slime_c);
			}
			else if (n == 3){
				slime_seat_c4->addChild(new_slime_c);
				EventDispatcher * dispatcher_slime = Director::getInstance()->getEventDispatcher();
				auto positionListener_slime = EventListenerTouchOneByOne::create();
				positionListener_slime->setSwallowTouches(false);
				positionListener_slime->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
				dispatcher_slime->addEventListenerWithSceneGraphPriority(positionListener_slime->clone(), new_slime_c);
			}
			else if (n == 4){
				slime_seat_c5->addChild(new_slime_c);
				EventDispatcher * dispatcher_slime = Director::getInstance()->getEventDispatcher();
				auto positionListener_slime = EventListenerTouchOneByOne::create();
				positionListener_slime->setSwallowTouches(false);
				positionListener_slime->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
				dispatcher_slime->addEventListenerWithSceneGraphPriority(positionListener_slime->clone(), new_slime_c);
			}
		}
		stagelayer->addChild(slime_seat_c1);
		stagelayer->addChild(slime_seat_c2);
		stagelayer->addChild(slime_seat_c3);
		stagelayer->addChild(slime_seat_c4);
		stagelayer->addChild(slime_seat_c5);
	}
	else if (stage_num % 5 == 0){
		auto new_slime_c = Sprite::create(StringUtils::format(com_slime[0].spr_name, 1));
		new_slime_c->setScale(3.0);
		new_slime_c->setAnchorPoint(Point(0.5, 0.5));
		new_slime_c->setPosition(Point(800, 350));
		new_slime_c->setTag(30000);
		//	stagelayer->addChild(new_slime_c);
		//if (n == 0){
		//	slime_seat_c1->addChild(new_slime_c);
		EventDispatcher * dispatcher_slime = Director::getInstance()->getEventDispatcher();
		auto positionListener_slime = EventListenerTouchOneByOne::create();
		positionListener_slime->setSwallowTouches(false);
		positionListener_slime->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
		dispatcher_slime->addEventListenerWithSceneGraphPriority(positionListener_slime->clone(), new_slime_c);
		stagelayer->addChild(new_slime_c);
		//	}

	}
	else{

	}
	//	stagelayer->addChild(slime_seat_c1);
	//	stagelayer->addChild(slime_seat_c2);
	//	stagelayer->addChild(slime_seat_c3);
	//	stagelayer->addChild(slime_seat_c4);
	//	stagelayer->addChild(slime_seat_c5);

	//Fight button 
	auto fight = Sprite::create("box/button_ok2.png");
	fight->setAnchorPoint(Point(0.5, 0.5));
	fight->setPosition(Point(400, 70));
	fight->setScale(1.5);
	fight->setTag(2008);
	stagelayer->addChild(fight);

	EventDispatcher * dispatcher_f = Director::getInstance()->getEventDispatcher();
	auto positionListener_f = EventListenerTouchOneByOne::create();
	//positionListener_f->setSwallowTouches(false);
	positionListener_f->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
	dispatcher_f->addEventListenerWithSceneGraphPriority(positionListener_f->clone(), fight);

	//Cancel button
	auto cancel = Sprite::create("box/button_cancel2.png");
	cancel->setAnchorPoint(Point(0.5, 0.5));
	cancel->setPosition(Point(890, 70));
	cancel->setScale(1.5);
	cancel->setTag(2009);
	stagelayer->addChild(cancel);

	EventDispatcher * dispatcher_c = Director::getInstance()->getEventDispatcher();
	auto positionListener_c = EventListenerTouchOneByOne::create();
	positionListener_c->setSwallowTouches(false);
	positionListener_c->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
	dispatcher_c->addEventListenerWithSceneGraphPriority(positionListener_c->clone(), cancel);


	this->addChild(stagelayer);
}



int select_stage_num = 0;
bool MapScene::TouchBegan(Touch * touch, Event *e){
	auto target = e->getCurrentTarget();

	Point pos = target->convertToNodeSpace(touch->getLocation());
	Rect rect = Rect(0, 0, target->getContentSize().width, target->getContentSize().height);

	int stage_select = 0;//����ڰ� ������ ��������
	//int cancel_slime_idx[5];//����ڰ� �������� ����ϸ� �� �ε��� ���� ���� 1�� �ٲܰ���
	//	int slime_list_switch = 0;


	if (rect.containsPoint(pos)){
		if (target->getTag() < 16 && switch_stg_btn == 0){
			stage_select = target->getTag();
			select_stage_num = stage_select;
			if (save_stage_num >= stage_select){
				//settingComSlime(target->getTag());

				switch_stg_btn = 1;
				auto scroll = (ScrollView*)this->getChildByTag(2001);
				scroll->setTouchEnabled(false);

				settingStageLayer(target->getTag());
			}
		}
		else if (target->getTag() == 2008){//fight ��ư Ŭ��
			//if (select_slime_num >= 1 && select_slime_num <= 5){//�������� �Ѹ��� �̻� �������� ���� BattleScene���� ��ȯ
			if (select_slime_num == 5){
				pAudio->stopBackgroundMusic(true);
				Director::getInstance()->replaceScene(BattleScene::createScene());
			}
			else{
				//�׽�Ʈ�� ��
				auto my_bottle_label = Label::createWithSystemFont(StringUtils::format("%s", "�������� �� ���� �̻� �����ϼ���"), "Thonburi", 50);
				my_bottle_label->setColor(Color3B::RED);
				my_bottle_label->setAnchorPoint(Point(0.5, 0.5));
				my_bottle_label->setPosition(Point(640, 360));
				stagelayer->addChild(my_bottle_label, 9999);
			}

		}
		else if (target->getTag() == 2009){//cancel ��ư Ŭ��

			for (int n = 0; n < 5; n++){
				select_slime[n].rank = 0;
				select_slime[n].index = 0;
				select_slime[n].id = 0;
				select_slime[n].exp = 0;
				select_slime[n].hp = 0;
				select_slime[n].skill_n = 0;
				select_slime[n].critical[0] = 0;
				select_slime[n].critical[1] = 0;
				select_slime[n].increase_hp[2] = 0;
				select_slime[n].increase_hp[3] = 0;
				select_slime[n].critical[4] = 0;
				select_slime[n].property = "";
				select_slime[n].spr_name = NULL;

			}


			for (int n = 0; n < 5; n++){
				if (user_slime_list[tg[n]].swintch_slime_btn == 1){
					user_slime_list[tg[n]].swintch_slime_btn = 0;
					//	tg[n] = 0;
				}
			}


			auto scroll = (ScrollView*)this->getChildByTag(2001);
			scroll->setTouchEnabled(true);

			//�׽�Ʈ�� ��
			auto my_bottle_label = Label::createWithSystemFont(StringUtils::format("%d", target->getTag()), "Thonburi", 50);
			my_bottle_label->setColor(Color3B::BLUE);
			my_bottle_label->setAnchorPoint(Point(0.5, 0.5));
			my_bottle_label->setPosition(Point(640, 360));
			stagelayer->addChild(my_bottle_label, 9999);

			this->removeChild(stagelayer);

			switch_stg_btn = 0;
			select_slime_num = 0;

		}
		else if (target->getTag() >= 5000 && target->getTag() < 10000){
			//int tg = target->getTag() - 5000;
			/*	auto dispatcher = (EventDispatcher*)target->getEventDispatcher();
			dispatcher->pauseEventListenersForTarget(target, false);*/

			for (int n = 0; n < 5; n++){
				if (select_slime[n].rank == 0 && user_slime_list[target->getTag() - 5000].swintch_slime_btn == 0){
					tg[n] = target->getTag() - 5000;
					user_slime_list[target->getTag() - 5000].swintch_slime_btn = 1;

					select_slime[n] = user_slime_list[target->getTag() - 5000];
					//select_slime[n].my_s_numbering = user_slime_list[target->];
					
					auto new_slime_u = Sprite::create(StringUtils::format(select_slime[n].spr_name, 1));
					new_slime_u->setScale(1.0);
					new_slime_u->setAnchorPoint(Point(0.5, 0.5));

					if (n < 3){
						new_slime_u->setPosition(Point(560, 510 - 150 * n));
					}
					else if (n >= 3 && n<5){
						new_slime_u->setPosition(Point(410, 440 - 150 * (n - 3)));
					}

					new_slime_u->setTag(20000 + n);//n���� �ٲٱ�

					EventDispatcher * dispatcher_slime = Director::getInstance()->getEventDispatcher();
					auto positionListener_slime = EventListenerTouchOneByOne::create();
					positionListener_slime->setSwallowTouches(false);
					positionListener_slime->onTouchBegan = CC_CALLBACK_2(MapScene::TouchBegan, this);
					dispatcher_slime->addEventListenerWithSceneGraphPriority(positionListener_slime->clone(), new_slime_u);//slime_list

					stagelayer->addChild(new_slime_u);
					select_slime_num++;

					break;

				}

			}

		}
		else if (target->getTag() >= 20000 && target->getTag() < 30000){
			int t = target->getTag() - 20000;
			//int id = select_slime[target->getTag() - 20000].id;
			/*		auto spr = (Sprite*)target->getChildByTag(target->getTag()-20000 + 5000);
			auto dispatch = (EventDispatcher*)spr->getEventDispatcher();
			dispatch->pauseEventListenersForTarget(target, true);
			*/
			for (int n = 0; n < 5; n++){
				if (user_slime_list[tg[n]].swintch_slime_btn == 1 && n == t){
					user_slime_list[tg[n]].swintch_slime_btn = 0;
					break;
				}
			}
			//auto dispatcher = (EventDispatcher*)target->getEventDispatcher();
			//dispatcher->pauseEventListenersForTarget(target, false);

			select_slime[target->getTag() - 20000].rank = 0;


			select_slime[target->getTag() - 20000].index = 0;
			select_slime[target->getTag() - 20000].id = 0;
			select_slime[target->getTag() - 20000].exp = 0;
			select_slime[target->getTag() - 20000].hp = 0;
			select_slime[target->getTag() - 20000].skill_n = 0;
			select_slime[target->getTag() - 20000].critical[0] = 0;
			select_slime[target->getTag() - 20000].critical[1] = 0;
			select_slime[target->getTag() - 20000].increase_hp[2] = 0;
			select_slime[target->getTag() - 20000].increase_hp[3] = 0;
			select_slime[target->getTag() - 20000].critical[4] = 0;
			select_slime[target->getTag() - 20000].property = "";
			select_slime[target->getTag() - 20000].spr_name = NULL;

			stagelayer->removeChild(target);

			select_slime_num--;

			//	auto spr = (Sprite*)this->getChildByTag(id);
			//	auto dispatch = (EventDispatcher*)
			//auto spr = (Sprite*)stagelayer->getChildByTag(id);
		}
		else if (target->getTag() >= 30000 && target->getTag() < 40000){
			int idx_num = target->getTag() - 30000;

			auto side_layer2 = LayerColor::create(Color4B(255, 255, 255, 0));//LayerColor::create(Color4B(255, 255, 255, 255))
			side_layer2->setContentSize(Size(195, 800));
			side_layer2->setAnchorPoint(Point(0.5, 0.5));
			side_layer2->setPosition(Point(0, 0));//945, 105
			side_layer2->setTag(2022);

			//�׽�Ʈ�� ��
			auto slime_inform = Label::createWithSystemFont(StringUtils::format("Rank : %d\nLevel : %ld\nHP : %ld\nSkill number : %d\n", com_slime[idx_num].rank, (long)com_slime[idx_num].exp / 50, (long)com_slime[idx_num].hp, (long)com_slime[idx_num].skill_n), "Thonburi", 25);
			slime_inform->setColor(Color3B::BLACK);
			slime_inform->setAnchorPoint(Point(0, 0));
			slime_inform->setPosition(Point(5, 350));

			side_layer2->addChild(slime_inform);

			if (com_slime[idx_num].skill_n == 5){

				auto skill_inform = Label::createWithSystemFont(StringUtils::format("Scratch\n  Critical : %ld\nBOMB!\n  Critical : %ld\nSteal HP\n  Increase HP: %ld\nRecovery\n  Increase HP: %ld\nMeteor\n  Critical: %ld\n", com_slime[idx_num].critical[0], com_slime[idx_num].critical[1], com_slime[idx_num].increase_hp[2], com_slime[idx_num].increase_hp[3], com_slime[idx_num].critical[4]), "Thonburi", 25);
				skill_inform->setColor(Color3B::BLACK);
				skill_inform->setAnchorPoint(Point(0, 1));
				skill_inform->setPosition(Point(5, 280));
				side_layer2->addChild(skill_inform);
			}
			else if (com_slime[idx_num].skill_n == 4){

				auto skill_inform = Label::createWithSystemFont(StringUtils::format("Scratch\n  Critical : %ld\nBOMB!\n  Critical : %ld\nSteal HP\n  Increase HP: %ld\nRecovery\n  Increase HP: %ld\n", com_slime[idx_num].critical[0], com_slime[idx_num].critical[1], com_slime[idx_num].increase_hp[2], com_slime[idx_num].increase_hp[3]), "Thonburi", 25);
				skill_inform->setColor(Color3B::BLACK);
				skill_inform->setAnchorPoint(Point(0, 1));
				skill_inform->setPosition(Point(5, 280));
				side_layer2->addChild(skill_inform);
			}
			else if (com_slime[idx_num].skill_n == 3){

				auto skill_inform = Label::createWithSystemFont(StringUtils::format("Scratch\n  Critical : %ld\nBOMB!\n  Critical : %ld\nSteal HP\n  Increase HP: %ld\n", com_slime[idx_num].critical[0], com_slime[idx_num].critical[1], com_slime[idx_num].increase_hp[2]), "Thonburi", 25);
				skill_inform->setColor(Color3B::BLACK);
				skill_inform->setAnchorPoint(Point(0, 1));
				skill_inform->setPosition(Point(5, 280));
				side_layer2->addChild(skill_inform);
			}
			else if (com_slime[idx_num].skill_n == 2){

				auto skill_inform = Label::createWithSystemFont(StringUtils::format("Scratch\n  Critical : %ld\nBOMB!\n  Critical : %ld\n", com_slime[idx_num].critical[0], com_slime[idx_num].critical[1]), "Thonburi", 25);
				skill_inform->setColor(Color3B::BLACK);
				skill_inform->setAnchorPoint(Point(0, 1));
				skill_inform->setPosition(Point(5, 280));
				side_layer2->addChild(skill_inform);
			}
			else if (com_slime[idx_num].skill_n == 1){

				auto skill_inform = Label::createWithSystemFont(StringUtils::format("Scratch\n  Critical : %ld\n", com_slime[idx_num].critical[0]), "Thonburi", 25);
				skill_inform->setColor(Color3B::BLACK);
				skill_inform->setAnchorPoint(Point(0, 1));
				skill_inform->setPosition(Point(5, 280));
				side_layer2->addChild(skill_inform);
			}
			else
			{

			}




			//��ǻ���� sidebar scrollview
			auto scroll2 = ScrollView::create(Size(190, 490), side_layer2);//��ũ�� �Ǵ� ��ü�� side_layer
			scroll2->setDirection(ScrollView::Direction::VERTICAL);
			scroll2->setBounceable(true);
			scroll2->setAnchorPoint(Point(0.5, 0.5));
			scroll2->setPosition(Point(950, 115));
			scroll2->setTag(2007);

			stagelayer->addChild(scroll2);



		}

	}

	return true;
}








