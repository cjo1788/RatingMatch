#pragma once

enum class UserState
{
	NONE = 0,
	READY = 1,
	MATCHING = 2,
	START = 3,
};

enum class RoomState
{
	ROOM_NONE = 0,
	ROOM_ENTER,
	ROOM_READY,
	ROOM_STARTING,
	ROOM_END,
	ROOM_OVER
};