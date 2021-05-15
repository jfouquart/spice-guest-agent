--- src/vdagentd-proto.h.orig	2021-05-15 18:41:38 UTC
+++ src/vdagentd-proto.h
@@ -23,7 +23,7 @@
 #ifndef __VDAGENTD_PROTO_H
 #define __VDAGENTD_PROTO_H
 
-#define VDAGENTD_SOCKET "/run/spice-vdagentd/spice-vdagent-sock"
+#define VDAGENTD_SOCKET "/var/run/spice-vdagentd/spice-vdagent-sock"
 
 #define DEFAULT_VIRTIO_PORT_PATH "/dev/virtio-ports/com.redhat.spice.0"
 
