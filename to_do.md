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