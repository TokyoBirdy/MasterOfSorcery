/*
 * Authors: Mos programming group
 
 // collision code ---> author: cecilia
 */

#include "Player.h"
#include "ProjectileSpell.h"
#include "SFML/Graphics.hpp"
#include <cmath>
#include "Vector2D.h"
#include "Gamepad.h"
#include "Spell.h"
#include "AnimatedSprite.h"
#include "MathHelper.h"
#include <cassert>
#include "ResourceFactory.h"
#include "ButtonListener.h"
#include "SpellCastHelper.h"
#include "SFML/Window.hpp"
#include "StunnedInputStrategy.h"
#include "NormalInputStrategy.h"
#include "Map.h"
#include <string>
#include "GameCore.h"
#include <sstream>
#include "TimerRenderer.h"
#include "PlayerSetupMenuState.h"
#include "SoundHandler.h"
#include "MosSound.h"
#include <algorithm>

using namespace std;
using namespace sf;

//Implementation constants
namespace
{
	const float ORIGINAL_MOVEMENT_SPEED = 140.0f;
	//const float MOVEMENT_LIMIT = 0.001;
	//const float COLLISION_PUSH_POWER = 6;
	const float CAST_COOLDOWN = 0.35f;
	//const float CAST_COOLDOWN = 0.0f;
	const int PIXEL=32;	

	const float SHOUT_COOLDOWN = 2.0f;

	//The folder where the character sprites are located.
	const string CHAR_FOLDER = "Images/Characters/";

	const string DIRECTION_ARROWS_FOLDER = "Images/Misc/Direction_Arrows/";

	const string SOUND_PATH = "Sounds/Characters/";

	//Simply converts an int to a string.
	string intToString(int i) {
		stringstream ss;
		ss << i;
		return ss.str();
	}
}

static bool randThree() { return ((std::rand() % 3) <= 1); }

Player::Player(int gamepadID, Spell** spellArray, const char* character, const sf::Vector2f& pos)
	: MoveableEntity(pos, ORIGINAL_MOVEMENT_SPEED)
	  //Connects the gamepad object to the correct player
	, mGamepad(new Gamepad(gamepadID))
	
	  //Gives the player its available spells
	, mSpellArray(spellArray)
	, mDirectionSprite(new Sprite(*ResourceFactory::getInstance()->loadImage(DIRECTION_ARROWS_FOLDER+"direction.png")))
	, mInputStrategies()
	, mCastCooldownTimer(new sf::Clock)
	, mShoutCooldownTimer(new sf::Clock)
	, mHitSound(new SoundHandler())
	, mPoints(0)
	, mCharacter(character)
{
	const string charpath = CHAR_FOLDER+character;

	//Adds the players available sprite sheets to the SpriteSheetMap
	mSheets["up"] = ResourceFactory::getInstance()->loadSpriteSheet(charpath + "/sheet_up.png",8,64,64,8);
	mSheets["down"] = ResourceFactory::getInstance()->loadSpriteSheet(charpath+"/sheet_down.png",8,64,64,8);
	mSheets["left"] = ResourceFactory::getInstance()->loadSpriteSheet(charpath + "/sheet_left.png",8,64,64,8);
	mSheets["right"] = ResourceFactory::getInstance()->loadSpriteSheet(charpath + "/sheet_right.png",8,64,64,8);

	//Initializes the players sprite
	mSprite->setSheet(*mSheets["down"]);

	//Add a standard normal input strategy to the bottom of the input strategy vector
	mInputStrategies.push_back(new NormalInputStrategy(*mGamepad));
	//Set the center of the players direction sprite to the center of the player
	mDirectionSprite->SetCenter(0, mDirectionSprite->GetSize().y / 2.0f);
	mDirectionSprite->SetColor(PlayerSetupMenuState::getColorOfPlayer(gamepadID));

	//Add sounds
	const string sPath = SOUND_PATH + character + "/";
	const string s1 = sPath + "hit_1.wav";
	mHitSound->putSoundInPlaylist(new MosSound(std::string(sPath + "hit_1.wav")));
	mHitSound->putSoundInPlaylist(new MosSound(std::string(sPath + "hit_2.wav")));
	mHitSound->putSoundInPlaylist(new MosSound(std::string(sPath + "hit_3.wav")));

	mSprite->freezeAnimation();
}

Player::~Player()
{
	delete mHitSound;
	delete mShoutCooldownTimer;
	delete mCastCooldownTimer;

	while(!mInputStrategies.empty())
	{
		delete mInputStrategies.back();
		mInputStrategies.pop_back();
	}

	delete mDirectionSprite;
	delete mSpellArray;
	delete mGamepad;
}

void Player::stun(float duration)
{
	mInputStrategies.push_back(new StunnedInputStrategy(*mGamepad, duration));
}

void Player::addInputStrategy(InputStrategy& strat)
{
	mInputStrategies.push_back(&strat);
}

Gamepad* Player::getGamepad() const {
	return mGamepad;
}

void Player::update()
{
	if(mGamepad->isConnected())
		handleInput();
	move(*mSpeed);
	applyFriction();
	mDirectionSprite->SetPosition(mSprite->getPosition()); 
	handleInputStrategies();
	PureEntity::update();
}

void Player::handleInput()
{
	handleMovement();
	handleTurning();
	handleSpells();
}

void Player::handleMovement()
{

	const InputStrategy* currStrategy = mInputStrategies.back();
	*mSpeed += currStrategy->getMovement(mMovementSpeed);
	if(mSpeed->x && mSpeed->y)
		//cout<<mSpeed->x<<':'<<mSpeed->y<<endl;
					
	//Sets the players current sprite sheet
	updateSpriteSheet();
}

void Player::updateSpriteSheet()
{
	Vector2D moveDirection;
	if(mGamepad->getLeftStickMagnitude() > Gamepad::getInputDeadzoneLeft())
		moveDirection = Vector2D(mGamepad->getThumbLX(), mGamepad->getThumbLY());

	//Decide if we are going to turn left/right or up/down
	if(moveDirection != Vector2D::ZERO_VECTOR)
	{
		mSprite->unfreezeAnimation();
		const float x = moveDirection.getX();
		const float y = moveDirection.getY();
		SpriteSheet* newSheet;

		if(abs(x) >= abs(y)) {
			// Right/left
			if(x >= 0.0f)
				newSheet = mSheets["right"];
			else
				newSheet = mSheets["left"];

		} else {
			// Up/down
			if(y >= 0.0f)
				newSheet = mSheets["up"];
			else
				newSheet = mSheets["down"];
		}

		//If the new sheet is the same as the current one, we don't need to change it
		if(newSheet != &mSprite->getSheet())
			mSprite->setSheet(*newSheet);
	}
	else
		mSprite->freezeAnimation();
}

void Player::handleTurning()
{
	//Get the angle from the current input strategy
	const float angle = mInputStrategies.back()->getTurning();

	//If the angle is not zero, assign the direction sprites angle to it.
	if(angle != 0)
		mDirectionSprite->SetRotation(angle);

	assert(mSprite->getRotation() >= 0 && mSprite->getRotation() <= 360);
}

void Player::handleSpells()
{
	//Gets the index of the spell that the player tries to cast.
	if(mCastCooldownTimer->GetElapsedTime() > CAST_COOLDOWN)
	{
		const int spellDownIndex = mInputStrategies.back()->getSpellDown();
		if(spellDownIndex != -1)
		{
			mSpellArray[spellDownIndex]->castSpellDown(this);
			mCastCooldownTimer->Reset();
		}
	}

	//Gets the index of the spell that the player tries to cast.
	const int spellUpIndex = mInputStrategies.back()->getSpellUp();
	if(spellUpIndex != -1)
		mSpellArray[spellUpIndex]->castSpellUp(this);
}

void Player::handleInputStrategies()
{
	InputStrategyVector::iterator i = mInputStrategies.begin();

	//Loops through the players input strategies. Updates them and deletes the ones that's timed out.
	while(i != mInputStrategies.end())
	{
		(*i)->update();
		if(!(*i)->isActive())
		{
			delete (*i);
			i = mInputStrategies.erase(i);
		}
		else
			i++;
	}
}

void Player::collideWith(Entity*& e)
{
	ProjectileSpell* ps = dynamic_cast<ProjectileSpell*>(e);
	if(ps != NULL)
		if(ps->getCaster() != *this &&
			mShoutCooldownTimer->GetElapsedTime() >= SHOUT_COOLDOWN)
		{
				mHitSound->playSound();
				mShoutCooldownTimer->Reset();
		}


	//cout << mGamepad->getId() << ": " << mSpeed->x << "      " << mSpeed->y << endl;
	//if(e->getType() == Type::PLAYER)
	//{
	//	Vector2f pushVec;
	//	pushVec.x = (mSpeed->x * COLLISION_PUSH_POWER);
	//	pushVec.y = (mSpeed->y * COLLISION_PUSH_POWER);

	//	this->push(-pushVec);
	//}


}

void Player::awardPoints(float points) { mPoints += points; }

bool Player::isAlive() const { return mIsAlive; }

Entity::Type Player::getType() const { return Type::PLAYER; }

void Player::render(RenderWindow& rw) const
{
	rw.Draw(*mDirectionSprite);
	PureEntity::render(rw);
	static sf::Shape rect = sf::Shape::Rectangle(0,0,32, 54, sf::Color(30, 150, 30, 150));
	/*rect.SetPosition(collision_origin.Left, collision_origin.Top);*/
	//rw.Draw(rect);
}

float Player::getRotation() const { return mDirectionSprite->GetRotation(); }

const char* Player::getCharacter() const { return mCharacter; }

float Player::getPoints() const { return mPoints; }

void Player::updateBounds() {
	const int SIZE_MOD_W = -32;
	const int SIZE_MOD_H = -10;

	//Get the size and position of the sprite.
	const Vector2f& pos = mSprite->getPosition();
	const Vector2f& cent = mSprite->getCenter();
	const float x = pos.x - cent.x - (SIZE_MOD_W / 2);
	const float y = pos.y - cent.y - (SIZE_MOD_H / 2);
	const float w = mSprite->getWidth() + SIZE_MOD_W;
	const float h = mSprite->getHeight() + SIZE_MOD_H;

	//Update mBounds.
	mBounds->Right = (mBounds->Left = x) + w;
	mBounds->Bottom = (mBounds->Top = y) + h;
}


// Authors Cecilia
//void Player::handleCollisionWithMap(const sf::RenderWindow& rw) 
void Player::handleCollisionWithMap(const sf::IntRect& colRect, const sf::RenderWindow& rw)
{
	IntRect playerRect=this->getBounds();
	//IntRect bigRect=playerRect;
	//if(mSpeed->x > 0)
	//	bigRect.Left -= mSpeed->x;
	//else
	//	bigRect.Right-= mSpeed->x;

	//if(mSpeed->y > 0)
	//	bigRect.Top -= mSpeed->y;
	//else 
	//	bigRect.Bottom -= mSpeed->y;
	
	// 4 collison blocks that around the entity
	for(int i=colRect.Top; i<colRect.Bottom;i++)
	{
		for(int j=colRect.Left; j<colRect.Right;j++)
		{
			if(GameCore::getInstance()->getMap()->mCollision[i][j] == 0)
				continue;

			IntRect tileRect= IntRect(j*PIXEL, i*PIXEL, (j+1)*PIXEL, (i+1)*PIXEL);
			if(!tileRect.Intersects(playerRect))
				continue;

			float differenceX;
			float differenceY;
			differenceX = tileRect.Left - playerRect.Left;
			differenceY = tileRect.Top - playerRect.Top;

			//left +right collision
			if(abs(differenceX)>=abs(differenceY))
			{
			//left collision
				if(differenceX < 0)
				{
					//cout<<"left collision. move to the right"<<endl;
					
					mSprite->move( 32 + differenceX, 0 );
					
					colData.left = true;
				}

				//right collision
				else 
				{
					//cout<<"right collision. move to the left"<<endl;
					mSprite->move( -32 + differenceX, 0 );
					colData.right = true;
				}
				mSpeed->x=0;
			}

			// up and down collision
			else
			{
				// up collision
				if(differenceY<0)
				{						 
					//cout<<"up collision. move downwards"<<endl;
					mSprite->move( 0, 32 + differenceY );
					colData.up = true;
				}

				// down collision
				else 
				{
					//cout<<"down collision. move upwards"<<endl;
					mSprite->move( 0, -54 + differenceY );
					colData.down = true;
				}
				mSpeed->y=0;
			}
			
			sf::Vector2f pos = mSprite->getPosition();
			if(pos.x < 32+16)
				pos.x = 32+16;
			else if(pos.x > 32*31-16)
				pos.x = 32*31-16;

			if(pos.y < 32*4+(54/2))
				pos.y = 32*4+(54/2);
			else if(pos.y > 32*23-(54/2))
				pos.y = 32*23-(54/2);

			mSprite->setPosition(pos);
			updateBounds();
			playerRect=this->getBounds();
		}
	}	
}

//PlayerHUD implementation starts here.
PlayerHUD::PlayerHUD(Player* pl)
: mPlayer(pl)
, mSpriteBox(new sf::Sprite(*ResourceFactory::getInstance()->loadImage("Images/HUD/hud_box.png")))
, mCharThumb(new sf::Sprite(*ResourceFactory::getInstance()->loadImage(string("Images/Characters/")+ pl->getCharacter()+"/portrait_hud.png")))
, mCharGlobe(new sf::Sprite(*ResourceFactory::getInstance()->loadImage(string("Images/HUD/globe_")+intToString(pl->mGamepad->getId())+".png")))
, mTimerRenderer(new TimerRenderer(300))
, mScoreSprite(new AnimatedSprite(Vector2f(0, 0), *ResourceFactory::getInstance()->loadSpriteSheet("Images/HUD/score_diamonds.png", 11, 31, 48, 11))) {
	const sf::Vector2f& globeSize = mCharGlobe->GetSize();

	//If this is player 2 or 4, we have to re-orient the charthumb slightly.
	if(pl->mGamepad->getId() % 2) {
		mSpriteBox->FlipX(true);
		mCharGlobe->Move(1024 - 15 - globeSize.x, 0);
	} else {
		mCharGlobe->Move(15, 0);
	}

	//If this is player 3 or 4, we have to re-orient the charthumb slightly.
	if(pl->mGamepad->getId() >= 2) {
		mSpriteBox->FlipY(true);
		mCharGlobe->Move(0, 768 - 10 - globeSize.y);
	} else {
		mCharGlobe->Move(0, 10);
	}

	mScoreSprite->setCenter(Vector2f(0, 0));
}

PlayerHUD::~PlayerHUD() {
	delete mSpriteBox;
	delete mCharThumb;
	delete mCharGlobe;
	delete mTimerRenderer;
}

void PlayerHUD::render(sf::RenderTarget& target) const {
	const float PADDING_X = 0.0f;
	const float PADDING_Y = 0.0f;
	const int id = mPlayer->mGamepad->getId();

	float scaleX;
	float scaleY;

	if(id%2)
		scaleX = -1;
	else
		scaleX = 1;

	if(id>=2)
		scaleY = -1;
	else
		scaleY = 1;

	renderFrame(target);
	renderChars(target);
	renderSpells(target, scaleX, scaleY);
	renderScore(target, scaleX, scaleY);
}

void PlayerHUD::renderFrame(sf::RenderTarget& targ) const {
	const int id = mPlayer->mGamepad->getId();
	const sf::Vector2f& size = this->mSpriteBox->GetSize();

	glPushMatrix();

	//Check if the player should be aligned to the right side of the screen.
	if(id%2) {
		glTranslatef(targ.GetWidth() - size.x, 0, 0);
	}

	//Check if this frame is in the bottom of the screen
	if(id >= 2) {
		glTranslatef(0, targ.GetHeight() - size.y, 0);
	}

	targ.Draw(*mSpriteBox);

	glPopMatrix();
}

void PlayerHUD::renderChars(sf::RenderTarget& targ) const {
	glPushMatrix();

	//Draw the globe and portrait in the correct place.
	targ.Draw(*mCharGlobe);

	glTranslatef(mCharGlobe->GetPosition().x, mCharGlobe->GetPosition().y, 0);
	glTranslatef(16, -14, 0);
	targ.Draw(*mCharThumb);
	glPopMatrix();
}

void PlayerHUD::renderSpells(sf::RenderTarget& targ, float scaleX, float scaleY) const {
	glPushMatrix();
	const int id = mPlayer->mGamepad->getId();
	Spell** spells = mPlayer->mSpellArray;
	const sf::Vector2f& spellSize = spells[0]->getThumbnail()->GetSize();
	const int spellHalfW = spellSize.x / 2;
	const int spellHalfH = spellSize.y / 2;

	if(id%2) {
		glTranslatef(targ.GetWidth() - spellSize.x, 0, 0);
	}

	if(id>=2) {
		glTranslatef(0, targ.GetHeight() - spellSize.y, 0);
	}
	//LT
	renderSingleSpell(targ, spells[0], 5*scaleX, 110*scaleY);

	//LB
	renderSingleSpell(targ, spells[1], 53*scaleX, 108*scaleY);

	//RB
	renderSingleSpell(targ, spells[3], 96*scaleX, 91*scaleY);

	//RT
	renderSingleSpell(targ, spells[2], 115*scaleX, 45*scaleY);
	glPopMatrix();
}

void PlayerHUD::renderCooldownTimer(float x, float y, float percent) const {
	glPushMatrix();
	glTranslatef(x, y, 0);
	glRotatef(-90, 0, 0, 1);
	glScalef(22, 22, 1);
	glColor4f(1, 0, 0, 0.5f);

	mTimerRenderer->render(percent);
	glPopMatrix();
}

void PlayerHUD::renderSingleSpell(RenderTarget& targ, Spell* spell, float x, float y) const {
	const sf::Vector2f& spellSize = spell->getThumbnail()->GetSize();
	const int spellHalfW = spellSize.x / 2;
	const int spellHalfH = spellSize.y / 2;

	float cooldown = spell->getTimeSinceLastCast() / spell->getCooldown();
	Sprite* sp = (cooldown >= 1)
		? spell->getThumbnail()
		: spell->getDisabledThumbnail();
	sp->SetPosition(x, y);
	targ.Draw(*sp);
	if(cooldown < 1.0f)
		renderCooldownTimer(x + spellHalfW, y + spellHalfH, 1-cooldown);
}

void PlayerHUD::renderScore(RenderTarget& targ, float scaleX, float scaleY) const {
	glPushMatrix();
	const float x = 142;
	const float score = mPlayer->getPoints();
	const float diamondWidth = mScoreSprite->getWidth();
	const float diamondHeight = mScoreSprite->getHeight();

	const int id = mPlayer->mGamepad->getId();

	if(id%2)
		glTranslatef(1024-diamondWidth, 0, 0);

	if(id/2)
		glTranslatef(0, 768-diamondHeight, 0);

	//Render three diamonds.
	for(int i=0;i<3;i++) {
		const int scorePart = min(10.0f, max(0.0f, score-10.0f*i));
		//cout << scorePart << " : ";

		mScoreSprite->setFrame(scorePart);
		mScoreSprite->setPosition(Vector2f((x + diamondWidth*i)*scaleX, 1*scaleY));
		mScoreSprite->draw(targ);
	}
	//cout << endl;
	glPopMatrix();
}