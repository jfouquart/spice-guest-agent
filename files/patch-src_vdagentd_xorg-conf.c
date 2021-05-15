--- src/vdagentd/xorg-conf.c.orig	2021-05-15 18:42:04 UTC
+++ src/vdagentd/xorg-conf.c
@@ -54,8 +54,8 @@ void vdagentd_write_xorg_conf(VDAgentMonitorsConfig *m
         .subvendor_id = PCI_MATCH_ANY,
         .subdevice_id = PCI_MATCH_ANY,
     };
-    const char *xorg_conf = "/run/spice-vdagentd/xorg.conf.spice";
-    const char *xorg_conf_old = "/run/spice-vdagentd/xorg.conf.spice.old";
+    const char *xorg_conf = "/var/run/spice-vdagentd/xorg.conf.spice";
+    const char *xorg_conf_old = "/var/run/spice-vdagentd/xorg.conf.spice.old";
 
     r = rename(xorg_conf, xorg_conf_old);
     if (r && errno != ENOENT) {
