--- src/vdagentd/virtio-port.c.orig	2018-06-06 09:27:15 UTC
+++ src/vdagentd/virtio-port.c
@@ -28,6 +28,7 @@
 #include <sys/select.h>
 #include <sys/socket.h>
 #include <sys/un.h>
+#include <termios.h>
 #include <glib.h>
 
 #include "virtio-port.h"
@@ -88,8 +89,7 @@ struct vdagent_virtio_port *vdagent_virtio_port_create
     vport = calloc(1, sizeof(*vport));
     if (!vport)
         return 0;
-
-    vport->fd = open(portname, O_RDWR);
+    vport->fd = open(portname, O_RDWR | O_NOCTTY | O_NONBLOCK | O_CLOEXEC);
     if (vport->fd == -1) {
         vport->fd = socket(PF_UNIX, SOCK_STREAM, 0);
         if (vport->fd == -1) {
@@ -104,6 +104,24 @@ struct vdagent_virtio_port *vdagent_virtio_port_create
             goto error;
         }
     } else {
+        struct termios tio;
+
+        tcgetattr(vport->fd, &tio);
+        /* set up serial port for non-canonical, dumb byte streaming */
+        tio.c_iflag &= ~(IGNBRK | BRKINT | IGNPAR | PARMRK | INPCK | ISTRIP |
+                         INLCR | IGNCR | ICRNL | IXON | IXOFF | IXANY | IMAXBEL);
+        tio.c_oflag = 0;
+        tio.c_lflag = 0;
+        tio.c_cflag |= B921600;
+        /* 1 available byte min or reads will block (we'll set non-blocking
+         * elsewhere, else we have to deal with read()=0 instead)
+         */
+        tio.c_cc[VMIN] = 1;
+        tio.c_cc[VTIME] = 0;
+        /* flush everything waiting for read/xmit, it's garbage at this point
+         */
+        tcflush(vport->fd, TCIFLUSH);
+        tcsetattr(vport->fd, TCSANOW, &tio);
         vport->is_uds = 0;
     }
     vport->opening = 1;
