# spice-guest-agent
spice-guest-agent FreeBSD ports.

This branch needs to rebuild and install libudev-devd with these lines removed from file udev-utils.c:
```
		} else if (!(bit_is_set(rel_bits, REL_X) &&
		             bit_is_set(rel_bits, REL_Y)) &&
		             has_lmr) {
			/* some touchscreens use BTN_LEFT rather than BTN_TOUCH */
			input_type = IT_TOUCHSCREEN;
			goto detected;
```
