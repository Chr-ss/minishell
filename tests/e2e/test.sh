#!/bin/bash

bash -c "bash -i &>output"
# sleep 2
# # Get the active window ID
# active_window_id=$(xprop -root | awk '/_NET_ACTIVE_WINDOW\(WINDOW\)/ {print $5}')
# LD_LIBRARY_PATH=xdotool ./xdotool/xdotool  windowactivate $active_window_id
# LD_LIBRARY_PATH=xdotool ./xdotool/xdotool  windowfocus $active_window_id
# LD_LIBRARY_PATH=xdotool ./xdotool/xdotool  key "ctrl+d"