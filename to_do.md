WHat's hapopening:
The kernel process is basically deleted

New processes are in the list
But when they are scheduled again
The RSP isn't correct anymore, only correct right after
initProcess()
because I don't save RSP before switching

Also the keyboard interrupt is only working half
because 1 press and release does 2 process switches

SOlution:
Make kernel process, but it in the list
And then save RSPs always before switching processses


Sunday:
Ok, currently trying to save the kernel process
RSP is being kinda iffy, though I should be able to just save it on first schedule.
Now I just wanna figure out how to make it only trigger once on a keypress and not trigger on release kon