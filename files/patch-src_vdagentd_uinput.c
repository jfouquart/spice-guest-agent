--- src/vdagentd/uinput.c.orig	2021-01-13 08:52:43 UTC
+++ src/vdagentd/uinput.c
@@ -144,21 +144,35 @@ void vdagentd_uinput_update_size(struct vdagentd_uinpu
     /* wheel */
     ioctl(uinput->fd, UI_SET_EVBIT, EV_REL);
     ioctl(uinput->fd, UI_SET_RELBIT, REL_WHEEL);
+    ioctl(uinput->fd, UI_SET_RELBIT, REL_X);
+    ioctl(uinput->fd, UI_SET_RELBIT, REL_Y);
 
     /* abs ptr */
     ioctl(uinput->fd, UI_SET_EVBIT, EV_ABS);
-    ioctl(uinput->fd, UI_SET_ABSBIT, ABS_X);
-    ioctl(uinput->fd, UI_SET_ABSBIT, ABS_Y);
 
     rc = ioctl(uinput->fd, UI_DEV_CREATE);
     if (rc < 0) {
         syslog(LOG_ERR, "create %s: %m", uinput->devname);
         vdagentd_uinput_destroy(uinputp);
     }
+
+	/*
+	 * Bad Hack to move the cursor position to Coordinate 0:0.
+	 */
+	sleep(1);
+    write(uinput->fd,
+          (struct input_event[]){{.type = EV_REL, REL_X, -INT32_MAX}},
+          sizeof(struct input_event));
+    write(uinput->fd,
+          (struct input_event[]){{.type = EV_REL, REL_Y, -INT32_MAX}},
+          sizeof(struct input_event));
+    write(uinput->fd,
+          (struct input_event[]){{.type = EV_SYN, SYN_REPORT, 0}},
+          sizeof(struct input_event));
 }
 
 static void uinput_send_event(struct vdagentd_uinput **uinputp,
-    __u16 type, __u16 code, __s32 value)
+    uint16_t type, uint16_t code, int32_t value)
 {
     struct vdagentd_uinput *uinput = *uinputp;
     struct input_event event = {
@@ -228,12 +242,12 @@ void vdagentd_uinput_do_mouse(struct vdagentd_uinput *
     if (*uinputp && uinput->last.x != mouse->x) {
         if (uinput->debug)
             syslog(LOG_DEBUG, "mouse: abs-x %d", mouse->x);
-        uinput_send_event(uinputp, EV_ABS, ABS_X, mouse->x);
+        uinput_send_event(uinputp, EV_REL, REL_X, mouse->x - uinput->last.x);
     }
     if (*uinputp && uinput->last.y != mouse->y) {
         if (uinput->debug)
             syslog(LOG_DEBUG, "mouse: abs-y %d", mouse->y);
-        uinput_send_event(uinputp, EV_ABS, ABS_Y, mouse->y);
+        uinput_send_event(uinputp, EV_REL, REL_Y, mouse->y - uinput->last.y);
     }
     for (i = 0; i < sizeof(btns)/sizeof(btns[0]) && *uinputp; i++) {
         if ((uinput->last.buttons & btns[i].mask) ==
