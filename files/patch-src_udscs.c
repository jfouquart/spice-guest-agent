--- src/udscs.c.orig	2018-06-06 09:27:15 UTC
+++ src/udscs.c
@@ -30,6 +30,7 @@
 #include <unistd.h>
 #include <errno.h>
 #include <sys/socket.h>
+#include <sys/ucred.h>
 #include <sys/un.h>
 #include <glib.h>
 #include <glib-unix.h>
@@ -523,8 +524,17 @@ static void udscs_server_accept(struct udscs_server *s
     new_conn->read_callback = server->read_callback;
     new_conn->disconnect_callback = server->disconnect_callback;
 
-    length = sizeof(new_conn->peer_cred);
-    r = getsockopt(fd, SOL_SOCKET, SO_PEERCRED, &new_conn->peer_cred, &length);
+    struct xucred peer_cred;
+    length = sizeof(peer_cred);
+    r = getsockopt(fd, SOL_LOCAL, LOCAL_PEERCRED, &peer_cred, &length);
+#if __FreeBSD__ >= 13
+    new_conn->peer_cred.pid = peer_cred.cr_pid;
+#else
+    new_conn->peer_cred.pid = 0;
+#endif
+    new_conn->peer_cred.uid = peer_cred.cr_uid;
+    new_conn->peer_cred.gid = peer_cred.cr_groups[0];
+
     if (r != 0) {
         syslog(LOG_ERR, "Could not get peercred, disconnecting new client");
         close(fd);
