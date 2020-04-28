--- src/vdagent/vdagent.c.orig	2018-06-06 09:27:15 UTC
+++ src/vdagent/vdagent.c
@@ -228,11 +228,14 @@ static void daemon_read_complete(struct udscs_connecti
         break;
     case VDAGENTD_AUDIO_VOLUME_SYNC: {
         VDAgentAudioVolumeSync *avs = (VDAgentAudioVolumeSync *)data;
+        uint16_t *volume = g_memdup(avs->volume, sizeof(uint16_t) * avs->nchannels);
+
         if (avs->is_playback) {
-            vdagent_audio_playback_sync(avs->mute, avs->nchannels, avs->volume);
+            vdagent_audio_playback_sync(avs->mute, avs->nchannels, volume);
         } else {
-            vdagent_audio_record_sync(avs->mute, avs->nchannels, avs->volume);
+            vdagent_audio_record_sync(avs->mute, avs->nchannels, volume);
         }
+        g_free(volume);
         break;
     }
     case VDAGENTD_FILE_XFER_DATA:
