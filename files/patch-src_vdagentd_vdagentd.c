--- src/vdagentd/vdagentd.c.orig	2021-05-15 18:42:49 UTC
+++ src/vdagentd/vdagentd.c
@@ -63,7 +63,7 @@ struct agent_data {
     int screen_count;
 };
 
-static const char pidfilename[] = "/run/spice-vdagentd/spice-vdagentd.pid";
+static const char pidfilename[] = "/var/run/spice-vdagentd/spice-vdagentd.pid";
 
 /* variables */
 static gchar *portdev = NULL;
