
#ifndef _ZEQ_PLAYER_H
#define _ZEQ_PLAYER_H

#include <irrlicht.h>

#include "types.h"
#include "mob.h"
#include "input.h"
#include "camera.h"
#include "renderer.h"
#include "zone_connection.h"
#include "zone_viewer.h"
#include "structs_mob.h"
#include "rocket.h"

using namespace irr;

class Player
{
private:
	ZoneConnection* mZoneConnection;
	Camera* mCamera;
	float mMovespeed;
	float mFallspeed;
	bool mIsFalling;
	float mFallStartingY;

	Mob* mMob;
	int mEntityID;
	MobPosition mPosition;
	float mHeading;
	ZoneViewerData* mZoneViewer;

private:
	//constants
	static const uint32 FALLING_SPEED_DEFAULT = 125;
	static const uint32 FALLING_SPEED_SWIMMING = 25;

private:
	void applyMovement(float delta);
	void applyGravity(float delta);
	void applyFallingDamage();
	void checkCollision(core::vector3df& from, core::vector3df& dest);

	void setEntityID(int id) { mEntityID = id; }

public:
	Player();

	void mainLoop();
	void zoneViewerLoop();

	ZoneViewerData* getZoneViewer() const { return mZoneViewer; }
	void setZoneViewer(ZoneViewerData* data) { mZoneViewer = data; }
	void updateViewerDisplay();

	void setZoneConnection(ZoneConnection* zc) { mZoneConnection = zc; }
	void setCamera(Camera* cam);
	int getEntityID() const { return mEntityID; }

	void setPosition(float x, float y, float z);
	const MobPosition& getCoords() const;

	void handleSpawn(Spawn_Struct* spawn);
	void handlePlayerProfile(PlayerProfile_Struct* pp);
};

#endif
