function log
    set logFile /home/alec/logs.txt

    printf "%s\n%s" "$argv" (cat $logFile) | head -n 5 > $logFile.tmp
    mv $logFile.tmp $logFile
end
