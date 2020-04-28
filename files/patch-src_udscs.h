--- src/udscs.h.orig	2018-06-06 09:27:15 UTC
+++ src/udscs.h
@@ -171,6 +171,14 @@ int udscs_server_fill_fds(struct udscs_server *server,
 void udscs_server_handle_fds(struct udscs_server *server, fd_set *readfds,
     fd_set *writefds);
 
+#if 1
+struct ucred {
+  pid_t pid; /* Process ID of the sending process */
+  uid_t uid; /* User ID of the sending process */
+  gid_t gid; /* Group ID of the sending process */
+};
+#endif
+
 /* Returns the peer's user credentials. */
 struct ucred udscs_get_peer_cred(struct udscs_connection *conn);
 
