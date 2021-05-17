--- src/vdagent-connection.c.orig	2021-01-13 08:52:43 UTC
+++ src/vdagent-connection.c
@@ -23,6 +23,11 @@
 #include <gio/gunixinputstream.h>
 #include <gio/gunixoutputstream.h>
 #include <gio/gunixsocketaddress.h>
+#include <sys/socket.h>
+#include <sys/types.h>
+#include <sys/ucred.h>
+#include <sys/un.h>
+#include <termios.h>
 
 #include "vdagent-connection.h"
 
@@ -48,7 +53,7 @@ GIOStream *vdagent_file_open(const gchar *path, GError
 {
     gint fd, errsv;
 
-    fd = g_open(path, O_RDWR);
+    fd = g_open(path, O_RDWR | O_NOCTTY | O_NONBLOCK | O_CLOEXEC);
     if (fd == -1) {
         errsv = errno;
         g_set_error_literal(err, G_FILE_ERROR,
@@ -57,6 +62,24 @@ GIOStream *vdagent_file_open(const gchar *path, GError
         return NULL;
     }
 
+    struct termios tio;
+    tcgetattr(fd, &tio);
+    /* set up serial port for non-canonical, dumb byte streaming */
+    tio.c_iflag &= ~(IGNBRK | BRKINT | IGNPAR | PARMRK | INPCK | ISTRIP |
+					 INLCR | IGNCR | ICRNL | IXON | IXOFF | IXANY | IMAXBEL);
+    tio.c_oflag = 0;
+    tio.c_lflag = 0;
+    tio.c_cflag |= B921600;
+    /* 1 available byte min or reads will block (we'll set non-blocking
+     * elsewhere, else we have to deal with read()=0 instead)
+     */
+    tio.c_cc[VMIN] = 1;
+    tio.c_cc[VTIME] = 0;
+    /* flush everything waiting for read/xmit, it's garbage at this point
+     */
+    tcflush(fd, TCIFLUSH);
+    tcsetattr(fd, TCSANOW, &tio);
+
     return g_simple_io_stream_new(g_unix_input_stream_new(fd, TRUE),
                                   g_unix_output_stream_new(fd, TRUE));
 }
@@ -147,17 +170,19 @@ PidUid vdagent_connection_get_peer_pid_uid(VDAgentConn
 {
     VDAgentConnectionPrivate *priv = vdagent_connection_get_instance_private(self);
     GSocket *sock;
-    GCredentials *cred;
+    struct xucred peer_cred;
     PidUid pid_uid = { -1, -1 };
+    socklen_t length = sizeof(peer_cred);
 
     g_return_val_if_fail(G_IS_SOCKET_CONNECTION(priv->io_stream), pid_uid);
 
     sock = g_socket_connection_get_socket(G_SOCKET_CONNECTION(priv->io_stream));
-    cred = g_socket_get_credentials(sock, err);
-    if (cred) {
-        pid_uid.pid = g_credentials_get_unix_pid(cred, err);
-        pid_uid.uid = g_credentials_get_unix_user(cred, err);
-        g_object_unref(cred);
+
+    if (getsockopt(g_socket_get_fd(sock), SOL_LOCAL, LOCAL_PEERCRED, &peer_cred, &length) == 0) {
+#if __FreeBSD__ >= 13
+        pid_uid.pid = peer_cred.cr_pid;
+#endif
+        pid_uid.uid = peer_cred.cr_uid;
     }
 
     return pid_uid;
