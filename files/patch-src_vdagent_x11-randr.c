--- src/vdagent/x11-randr.c.orig	2018-06-06 09:27:15 UTC
+++ src/vdagent/x11-randr.c
@@ -584,20 +584,20 @@ static void zero_base_monitors(struct vdagent_x11 *x11
                                int *width, int *height)
 {
     int i, min_x = INT_MAX, min_y = INT_MAX, max_x = INT_MIN, max_y = INT_MIN;
-    int *mon_height, *mon_width;
+    int mon_height, mon_width;
 
     for (i = 0; i < mon_config->num_of_monitors; i++) {
         if (!monitor_enabled(&mon_config->monitors[i]))
             continue;
         mon_config->monitors[i].x &= ~7;
         mon_config->monitors[i].width &= ~7;
-        mon_width = (int *)&mon_config->monitors[i].width;
-        mon_height = (int *)&mon_config->monitors[i].height;
-        constrain_to_screen(x11, mon_width, mon_height);
+        mon_width = mon_config->monitors[i].width;
+        mon_height = mon_config->monitors[i].height;
+        constrain_to_screen(x11, &mon_width, &mon_height);
         min_x = min_int(mon_config->monitors[i].x, min_x);
         min_y = min_int(mon_config->monitors[i].y, min_y);
-        max_x = max_int(mon_config->monitors[i].x + *mon_width, max_x);
-        max_y = max_int(mon_config->monitors[i].y + *mon_height, max_y);
+        max_x = max_int(mon_config->monitors[i].x + mon_width, max_x);
+        max_y = max_int(mon_config->monitors[i].y + mon_height, max_y);
     }
     if (min_x != 0 || min_y != 0) {
         syslog(LOG_ERR, "%s: agent config %d,%d rooted, adjusting to 0,0.",
