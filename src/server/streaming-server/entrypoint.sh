# compile code
gcc -o stream stream.c `pkg-config --cflags --libs gstreamer-rtsp-server-1.0 gstreamer-1.0`

# execute code
./stream "( filesrc location=\test.mp4 ! qtdemux ! h264parse ! rtph264pay name=pay0 pt=96 )"

ffmpeg -fflags nobuffer \
 -rtsp_transport tcp \
 -i rtsp://127.0.0.1:8554/test \
 -vsync 0 \
 -copyts \
 -vcodec copy \
 -movflags frag_keyframe+empty_moov \
 -an \
 -hls_flags delete_segments+append_list \
 -f segment \
 -segment_list_flags live \
 -segment_time 1 \
 -segment_list_size 3 \
 -segment_format mpegts \
 -segment_list /tmp/stream/index.m3u8 \
 -segment_list_type m3u8 \
 -segment_list_entry_prefix /stream/ \
 /tmp/stream/%d.ts
