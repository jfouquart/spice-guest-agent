--- src/udscs.c.orig	2018-06-06 09:27:15 UTC
+++ src/udscs.c
@@ -523,8 +523,14 @@ static void udscs_server_accept(struct udscs_server *s
     new_conn->read_callback = server->read_callback;
     new_conn->disconnect_callback = server->disconnect_callback;
 
+#if 0
     length = sizeof(new_conn->peer_cred);
     r = getsockopt(fd, SOL_SOCKET, SO_PEERCRED, &new_conn->peer_cred, &length);
+#else
+    r = getpeereid(fd, &new_conn->peer_cred.uid, &new_conn->peer_cred.gid);
+    new_conn->peer_cred.pid = 0; /* strtol(getenv("AGENT_PID"), NULL, 10); */
+#endif
+
     if (r != 0) {
         syslog(LOG_ERR, "Could not get peercred, disconnecting new client");
         close(fd);
