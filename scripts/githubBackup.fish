#!/usr/bin/env fish

# Check & get mounted drive
set drives (find /media -mindepth 1 -maxdepth 1 -type d)
if test (count $drives) -eq 1
    set driveDir $drives[1]
else
    echo "Improper drive amount detected"
    exit 1
end

mkdir -p "$driveDir/Projects"
for repo in (curl -s -H "Authorization: token $GITHUB_TOKEN" https://api.github.com/user/repos?per_page=100 | jq -r '.[].full_name')
    if string match -q 'AmazinAxel/*' $repo
        set repoName (string split '/' $repo)[2]
        set targetDir "$driveDir/Projects/$repoName"

        if test -d "$targetDir/.git"
            echo "Pulling repo: $repoName"
            git -C "$targetDir" pull
        else
            echo "Cloning repo: $repoName"
            git clone "https://github.com/$repo.git" "$targetDir"
        end
    end
end
