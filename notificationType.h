#ifndef NOTIFICATION_TYPE_H
#define NOTIFICATION_TYPE_H

enum NotificationType { NT_NONE,
                        NT_TUT_START, NT_TUT_SHOW_CONTROLS,
                        NT_TUT_MISSION_LOG,
                        NT_TUT_AVATARS,
                        NT_TUT_HEALTH_BARS,
                        NT_TUT_CHARGE_BARS,
                        NT_TUT_POWER_BARS,
                        NT_TUT_MINI_RADAR,
                        NT_TUT_MINI_MAP,
                        NT_TUT_PILOT_ROLE,
                        NT_TUT_ENGINEER_ROLE,
                        NT_TUT_NAVIGATOR_ROLE,
                        NT_TUT_FIRE_WEAPON,
                        NT_TUT_MOVE_SHIP,
                        NT_TUT_SHOW_MAP, NT_TUT_CLOSE_MAP,
                        NT_TUT_SHOW_RADAR, NT_TUT_CLOSE_RADAR,
                        NT_TUT_OPEN_CONSOLE, NT_TUT_CLOSE_CONSOLE, NT_TUT_WAITING,
                        NT_TUT_REPAIR_WEAPONS, NT_TUT_REPAIR_ENGINES, NT_TUT_REPAIR_HULL,
                        NT_CONTROLS, NT_UNDER_ATTACK, NT_FLEE,
                        NT_WEAPON_CHARGE_STUCK, NT_SHIELD_CHARGE_STUCK,
                        NT_HULL_CRITICAL, NT_ENGINES_CRITICAL, NT_WEAPONS_CRITICAL,
                        NT_SENSORS_CRITICAL,
                        NT_OBJECTIVE_SEEK, NT_OBJECTIVE_DESTROY, NT_OBJECTIVE_ESCAPE,
                        NT_COMMENT_ONE, NT_COMMENT_TWO, NT_COMMENT_THREE,
                        NT_COUNTDOWN_5, NT_COUNTDOWN_10, NT_COUNTDOWN_15, NT_COUNTDOWN_30,
                        NT_COUNTDOWN_60,
                        NT_MISSION_COMPLETE, NT_DISOBEY, NT_GAME_OVER, NT_NEAR_BC, NT_SHIP_ON, NT_ENUM_END };

#endif
