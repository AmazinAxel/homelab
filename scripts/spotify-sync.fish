#!/usr/bin/env fish

# Check & get mounted drive
set subdirs (find . -mindepth 1 -maxdepth 1 -type d)
if test (count $subdirs) -eq 1
    set dir $subdirs[1]
else
    echo "Improper drive amount detected (expected one)"
    exit 1
end

mkdir "$dir/Music"
set playlists \
    "Synthwave https://open.spotify.com/playlist/1YIe34rcmLjCYpY9wJoM2p" \
    "Focus https://open.spotify.com/playlist/3Qk9br14pjEo2aRItDhb2f" \
    "Study https://open.spotify.com/playlist/0vvXsWCC9xrXsKd4FyS8kM" \
    "SynthAmbient https://open.spotify.com/playlist/4murW7FWRb0LFbG7eUwDy0" \
    "Ambient https://open.spotify.com/playlist/07lYUEyTkWP3NqIa7Kzyqx"

# Loop & download each playlist
for playlist in $playlists
    set name (echo $playlist | awk '{print $1}')
    set url (echo $playlist | awk '{print $2}')
    sudo spotdl download "$url" --output "$dir/Music/$name"
end
