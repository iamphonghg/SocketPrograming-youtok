#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>
#include <stdio.h>

guint exit_timeout_id = 0;

static gboolean
timeout(GstRTSPServer *server)
{
  GstRTSPSessionPool *pool;

  pool = gst_rtsp_server_get_session_pool(server);
  gst_rtsp_session_pool_cleanup(pool);
  g_object_unref(pool);

  return TRUE;
}

static GstRTSPFilterResult
client_filter(GstRTSPServer *server, GstRTSPClient *client,
              gpointer user_data)
{
  /* Simple filter that shuts down all clients. */
  return GST_RTSP_FILTER_REMOVE;
}

/* Timeout that runs 10 seconds after the first client connects and triggers
 * the shutdown of the server */
static gboolean
shutdown_timeout(GstRTSPServer *server)
{
  GstRTSPMountPoints *mounts;
  g_print("Time for everyone to go. Removing mount point\n");
  /* Remove the mount point to prevent new clients connecting */
  mounts = gst_rtsp_server_get_mount_points(server);
  gst_rtsp_mount_points_remove_factory(mounts, "/test");
  g_object_unref(mounts);

  /* Filter existing clients and remove them */
  g_print("Disconnecting existing clients\n");
  gst_rtsp_server_client_filter(server, client_filter, NULL);
  return FALSE;
}

static void
client_connected(GstRTSPServer *server, GstRTSPClient *client)
{
  if (exit_timeout_id == 0)
  {
    g_print("First Client connected. Disconnecting everyone in 10 seconds\n");
    exit_timeout_id =
        g_timeout_add_seconds(10, (GSourceFunc)shutdown_timeout, server);
  }
}

static gboolean
remove_sessions(GstRTSPServer *server)
{
  GstRTSPSessionPool *pool;

  pool = gst_rtsp_server_get_session_pool(server);
  guint removed = gst_rtsp_session_pool_cleanup(pool);
  g_object_unref(pool);
  g_print("Removed %d sessions\n", removed);

  return TRUE;
}

int start_stream(const char *filename)
{
  GMainLoop *loop;
  GstRTSPServer *server;
  GstRTSPMountPoints *mounts;
  GstRTSPMediaFactory *factory;
  char source[250];
  strcpy(source, "( filesrc location=\\");
  strcat(source, filename);
  strcat(source, " ! qtdemux ! h264parse ! rtph264pay name=pay0 pt=96 )");

  printf("%s\n", filename);
  printf("%s\n", source);

  gst_init(NULL, NULL);
  loop = g_main_loop_new(NULL, FALSE);

  server = gst_rtsp_server_new();

  mounts = gst_rtsp_server_get_mount_points(server);
  factory = gst_rtsp_media_factory_new();
  gst_rtsp_media_factory_set_launch(factory, source);

  char mountPointStr[100];
  strcpy(mountPointStr, "/");
  strcat(mountPointStr, filename);

  gst_rtsp_mount_points_add_factory(mounts, mountPointStr, factory);

  g_object_unref(mounts);
  if (gst_rtsp_server_attach(server, NULL) == 0)
    goto failed;

  // g_signal_connect(server, "client-connected", (GCallback)client_connected, NULL);

  /* add a timeout for the session cleanup */
  g_timeout_add_seconds(2, (GSourceFunc)timeout, server);
  g_print("stream ready at rtsp://127.0.0.1:8554%s\n", mountPointStr);
  g_main_loop_run(loop);

  return 0;

  /* ERRORS */
  failed:
  {
    g_print("failed to attach the server\n");
    return -1;
  }
}

int main(int argc, char *argv[]) {
  start_stream(argv[1]);
}
