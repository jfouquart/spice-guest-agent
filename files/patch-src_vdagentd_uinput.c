--- src/vdagentd/uinput.c.orig	2018-06-06 09:27:15 UTC
+++ src/vdagentd/uinput.c
@@ -155,6 +155,25 @@ void vdagentd_uinput_update_size(struct vdagentd_uinpu
     ioctl(uinput->fd, UI_SET_ABSBIT, ABS_X);
     ioctl(uinput->fd, UI_SET_ABSBIT, ABS_Y);
 
+    ioctl(uinput->fd, UI_ABS_SETUP,
+          (struct uinput_abs_setup[]){{.code = ABS_X,
+                                       .absinfo = {
+#ifdef WITH_STATIC_UINPUT
+                                           .maximum = 32767,
+#else
+                                           .maximum = width - 1,
+#endif
+                                       }}});
+    ioctl(uinput->fd, UI_ABS_SETUP,
+          (struct uinput_abs_setup[]){{.code = ABS_Y,
+                                       .absinfo = {
+#ifdef WITH_STATIC_UINPUT
+                                           .maximum = 32767,
+#else
+                                           .maximum = height - 1,
+#endif
+                                       }}});
+
     rc = ioctl(uinput->fd, UI_DEV_CREATE);
     if (rc < 0) {
         syslog(LOG_ERR, "create %s: %m", uinput->devname);
@@ -163,7 +182,7 @@ void vdagentd_uinput_update_size(struct vdagentd_uinpu
 }
 
 static void uinput_send_event(struct vdagentd_uinput **uinputp,
-    __u16 type, __u16 code, __s32 value)
+    uint16_t type, uint16_t code, int32_t value)
 {
     struct vdagentd_uinput *uinput = *uinputp;
     struct input_event event = {
