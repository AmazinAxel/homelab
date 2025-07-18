#!/usr/bin/env fish

set -l api_url "https://api.github.com/user/repos?per_page=100"

curl -s -H "Authorization: token $GITHUB_TOKEN" $api_url | jq '.[].full_name'
