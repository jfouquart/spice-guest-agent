# spice-guest-agent
spice-guest-agent FreeBSD ports.

This branch needs to rebuild and install libudev-devd with this line removed:
		} else if (!(bit_is_set(rel_bits, REL_X) &&
			             bit_is_set(rel_bits, REL_Y)) &&
			             has_lmr) {
				/* some touchscreens use BTN_LEFT rather than BTN_TOUCH */
				input_type = IT_TOUCHSCREEN;
				goto detected;
			}
in file udev-utils.c
