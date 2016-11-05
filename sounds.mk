sounds: data/sounds/sfx_bump.h data/sounds/sfx_bump_alien.h data/sounds/sfx_cat_disable.h data/sounds/sfx_cat_enable.h data/sounds/sfx_dash.h data/sounds/sfx_highscore_switch.h data/sounds/sfx_jetpack.h data/sounds/sfx_menu_cancel.h data/sounds/sfx_menu_confirm.h data/sounds/sfx_menu_locked.h data/sounds/sfx_menu_switch.h data/sounds/sfx_player_die.h data/sounds/sfx_stomp_alien.h data/sounds/sfx_stomp_bat.h data/sounds/sfx_stomp_bird.h data/sounds/sfx_stomp_ghost.h data/sounds/sfx_time_low.h data/sounds/sfx_time_out.h data/sounds/sfx_time_pickup.h data/sounds/sfx_warp_start.h

data/sounds/%.h: data/sounds/%.mml
	java -jar jar/MMLGB.jar $< $@
